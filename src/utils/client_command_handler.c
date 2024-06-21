#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/commands.h"
#include "../../headers/client_command_handler.h"

#define DEFAULT_BUFLEN 1024 // default buffer size for command input

// function to execute the identified command with given parameters
int executeCommand(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const char *command, char **parameters, char *message) {
    // execute command based on its type
    if (strcmp(command, COMMAND_JOIN) == 0) {
        initSocketConnection(sock, wsaData, server, parameters[0], atoi(parameters[1]));
        sendMessageToServer(sock, message);
    } else if (strcmp(command, COMMAND_LEAVE) == 0) {
        sendMessageToServer(sock, message);
        return 1; // indicate disconnection
    } else if (strcmp(command, COMMAND_STORE) == 0) {
        sendFileToServer(sock, parameters[0], message);
    } else {
        sendMessageToServer(sock, message);
    }
    // additional command checks can be implemented here
    return 0; // no disconnection
}

// function to initialize socket connection based on provided ip and port
void initSocketConnection(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const char *ip, int port) {
    // close the socket if it is already open
    if (*sock != INVALID_SOCKET) {
        closesocket(*sock);
        WSACleanup();
    }

    // start winsock api
    if (WSAStartup(MAKEWORD(2, 2), wsaData) != 0) {
        fprintf(stderr, "failed to initialize winsock. error code : %d", WSAGetLastError());
        *sock = INVALID_SOCKET;
        return;
    }

    // create socket
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == INVALID_SOCKET) {
        fprintf(stderr, "could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return;
    }

    // setup server address structure
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
}

void sendMessageToServer(SOCKET *sock, char *message) {
    if (*sock == INVALID_SOCKET || message == NULL) {
        return;
    }

    int messageLength = strlen(message); // Get the length of the message
    int bytesSent = send(*sock, message, messageLength, 0); // Send the message

    if (bytesSent == SOCKET_ERROR) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
        return;
    }
}

// Function to send a file to the server
void sendFileToServer(SOCKET *sock, const char *filename, char *message) {
    FILE *file;
    char filePath[256] = "files/"; // Assuming files are stored in a subdirectory named 'files'
    strcat(filePath, filename); // Append the filename to the path

    // Open the file
    file = fopen(filePath, "rb"); // Open in binary mode to handle all types of files
    if (file == NULL) {
        fprintf(stderr, ERROR_FILE_NOT_FOUND "\n");
        return;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Send the message (command) to the server without filename and fileSize
    char buffer[DEFAULT_BUFLEN];
    sprintf(buffer, "%s", message); // copy message to buffer
    int bytesSent = send(*sock, buffer, strlen(buffer), 0);

    if (bytesSent == SOCKET_ERROR) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
        fclose(file);
        return;
    }

    // Read and send the file in chunks
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(*sock, buffer, bytesRead, 0);
    }

    // Close the file
    fclose(file);
}