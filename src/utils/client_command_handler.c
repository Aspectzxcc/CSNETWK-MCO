#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/commands.h"
#include "../../headers/client_command_handler.h"

#define DEFAULT_BUFLEN 1024 // default buffer size for command input

// executes the specified command with the provided parameters
int executeCommand(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const char *command, char **parameters, char *message) {
    // check if not connected and the command is not join so that messages cannot be sent
    if (strcmp(command,COMMAND_JOIN) != 0 && !connectionStatus) {
        return 0;
    }

    // determine and execute the command based on its identifier
    if (strcmp(command, COMMAND_JOIN) == 0) {
        // for join command, initialize socket connection and send message
        initSocketConnection(sock, wsaData, server, parameters[0], atoi(parameters[1]));
        sendMessageToServer(sock, message);
    } else if (strcmp(command, COMMAND_LEAVE) == 0) {
        // if the client is not connected and tries to leave, return 0
        sendMessageToServer(sock, message);
        return 1; // indicate client wishes to disconnect
    } else if (strcmp(command, COMMAND_REGISTER) == 0) {
        // for register command, send the message to the server
        sendMessageToServer(sock, message);
    } else if (strcmp(command, COMMAND_STORE) == 0) {
        // for store command, send a file to the server
        sendMessageToServer(sock, message);
        sendFileToServer(sock, parameters[0]);
    } else if (strcmp(command, COMMAND_GET) == 0) {
        // for get command, send the message to the server
        sendMessageToServer(sock, message);
        receiveFileFromServer(sock, parameters[0]);
    } else if (strcmp(command, COMMAND_DIR) == 0) {
        // for other commands, just send the message to the server
        sendMessageToServer(sock, message);
    } else if (strcmp(command, COMMAND_HELP) == 0) {
        // for help command, print the available commands
        printCommands();
    } else {
        // command not found
        fprintf(stderr, ERROR_COMMAND_NOT_FOUND "\n");
    }
    // future commands can be added here
    return 0; // indicate no disconnection by default
}   

void handleServerResponse(SOCKET *sock, const char *command, int disconnect) {
    char serverReply[DEFAULT_BUFLEN]; // server reply buffer
    int replyLength; // size of received data

    if (strcmp(command, COMMAND_LEAVE) == 0) {
        if (disconnect) {
            printf(MESSAGE_SUCCESSFUL_DISCONNECTION "\n");
        } else {
            fprintf(stderr, ERROR_DISCONNECT_FAILED "\n");
        }
        return;
    }

    if (strcmp(command, COMMAND_GET) == 0 || strcmp(command, COMMAND_HELP) == 0) {
        return;
    }

    if (!connectionStatus) {
        return;
    }

    replyLength = recv(*sock, serverReply, DEFAULT_BUFLEN, 0);

    if (replyLength == SOCKET_ERROR) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
    } else if (replyLength == 0) {
        return;
    } else {
        // null-terminate the received data before printing
        serverReply[replyLength] = '\0';
        printf("%s\n", serverReply);
    }
}

// initializes a socket connection using the provided ip address and port
void initSocketConnection(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const char *ip, int port) {
    // if socket is already open, close it first
    if (*sock != INVALID_SOCKET) {
        closesocket(*sock);
        WSACleanup();
    }

    // initialize winsock
    if (WSAStartup(MAKEWORD(2, 2), wsaData) != 0) {
        fprintf(stderr, "failed to initialize winsock. error code : %d", WSAGetLastError());
        *sock = INVALID_SOCKET;
        return;
    }

    // create the socket
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == INVALID_SOCKET) {
        fprintf(stderr, "could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return;
    }

    // set up the server address structure
    server->sin_family = AF_INET;
    server->sin_addr.s_addr = inet_addr(ip);
    server->sin_port = htons(port);

    // attempt to connect to the server
    if (connect(*sock, (struct sockaddr *)server, sizeof(*server)) < 0) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
        closesocket(*sock);
        WSACleanup();
        *sock = INVALID_SOCKET;
    }

    connectionStatus = 1; // set connection status flag to connected
}

// sends a message to the server
void sendMessageToServer(SOCKET *sock, char *message) {
    // check if socket is valid and message is not null
    if (*sock == INVALID_SOCKET || message == NULL) {
        return;
    }

    // send the message to the server
    int messageLength = strlen(message); // calculate message length
    int bytesSent = send(*sock, message, messageLength, 0); // send the message

    // check for sending errors
    if (bytesSent == SOCKET_ERROR) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
        return;
    }
}

// sends a file to the server
void sendFileToServer(SOCKET *sock, const char *filename) {
    FILE *file;
    char filePath[256] = "files/";
    strncat(filePath, filename, sizeof(filePath) - strlen(filePath) - 1);

    file = fopen(filePath, "rb");
    if (file == NULL) {
        fprintf(stderr, ERROR_FILE_NOT_FOUND_CLIENT "\n");
        return;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Convert fileSize to network byte order and send it
    long fileSizeNetOrder = htonl(fileSize);
    if (send(*sock, (char*)&fileSizeNetOrder, sizeof(fileSizeNetOrder), 0) == SOCKET_ERROR) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
        fclose(file);
        return;
    }

    char buffer[DEFAULT_BUFLEN];
    int bytesRead;
    
    // Send the file data
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(*sock, buffer, bytesRead, 0) == SOCKET_ERROR) {
            fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
            break;
        }
    }

    fclose(file);
}

void receiveFileFromServer(SOCKET *sock, const char *filename) {
    char filePath[256] = "files/";
    strncat(filePath, filename, sizeof(filePath) - strlen(filePath) - 1);

    // Open the file for writing in binary mode
    FILE *file = fopen(filePath, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    // Receive the file size
    long fileSizeNetOrder;
    if (recv(*sock, (char*)&fileSizeNetOrder, sizeof(fileSizeNetOrder), 0) <= 0) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
        fclose(file);
        return;
    }
    long fileSize = ntohl(fileSizeNetOrder); // Convert from network byte order to host byte order

    // Receive the file data
    char buffer[DEFAULT_BUFLEN];
    int bytesReceived;
    long totalBytesReceived = 0;
    while (totalBytesReceived < fileSize) {
        bytesReceived = recv(*sock, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            fwrite(buffer, 1, bytesReceived, file);
            totalBytesReceived += bytesReceived;
        } else if (bytesReceived == 0) {
            break;
        } else {
            fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
            break;
        }
    }

    fclose(file); // Close the file

    if (totalBytesReceived != fileSize) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
    } else {
        printf(MESSAGE_SUCCESSFUL_FILE_DOWNLOAD "\n", filename);
    }
}