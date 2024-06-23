#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/commands.h"
#include "../../headers/client_command_handler.h"

#define DEFAULT_BUFLEN 1024 // default buffer size for command input

// executes the specified command with the provided parameters.
int executeCommand(SOCKET *sock, SOCKADDR_IN *server, const char *command, char **parameters, char *message) {
    // check if the client is not connected and the command requires a connection.
    if ((strcmp(command,COMMAND_JOIN) != 0 && strcmp(command, COMMAND_HELP) != 0) && connectionStatus == DISCONNECTED) {
        // log error if trying to execute a command without being connected.
        if (strcmp(command, COMMAND_LEAVE) == 0) {
            fprintf(stderr, ERROR_DISCONNECT_FAILED "\n");
        } else {
            fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
        }
        return 0;
    }

    // check if the client is not registered and tries to execute commands that require registration.
    if ((strcmp(command, COMMAND_GET) == 0 || strcmp(command, COMMAND_STORE) == 0 
    || strcmp(command, COMMAND_BROADCAST) == 0 || strcmp(command, COMMAND_UNICAST) == 0) 
    && registrationStatus == REGISTRATION_NOT_REGISTERED) {
        fprintf(stderr, ERROR_REGISTRATION_FAILED "\n");
        return 0;
    }

    // execute the command based on its identifier.
    if (strcmp(command, COMMAND_JOIN) == 0) {
        // initialize socket connection and send join message.
        initSocketConnection(sock, server, parameters[0], atoi(parameters[1]));
        sendMessageToServer(sock, message);
    } else if (strcmp(command, COMMAND_LEAVE) == 0) {
        // send leave message and close the socket.
        sendMessageToServer(sock, message);
        closesocket(*sock);
        connectionStatus = DISCONNECTED;
        printf(MESSAGE_SUCCESSFUL_DISCONNECTION "\n");
        return 1; // indicate disconnection to the main loop.
    } else if (strcmp(command, COMMAND_REGISTER) == 0) {
        // send registration message to the server.
        sendMessageToServer(sock, message);
    } else if (strcmp(command, COMMAND_STORE) == 0) {
        // send a file to the server.
        sendMessageToServer(sock, message);
        sendFileToServer(sock, parameters[0]);
    } else if (strcmp(command, COMMAND_GET) == 0) {
        // request a file from the server.
        sendMessageToServer(sock, message);
        receiveFileFromServer(sock, parameters[0]);
    } else if (strcmp(command, COMMAND_DIR) == 0) {
        // send a directory listing request to the server.
        sendMessageToServer(sock, message);
    } else if (strcmp(command, COMMAND_HELP) == 0) {
        // print available commands.
        printCommands();
    } else if (strcmp(command, COMMAND_BROADCAST) == 0) {
        // send a broadcast message to the server.
        sendMessageToServer(sock, message);
    } else if (strcmp(command, COMMAND_UNICAST) == 0) {
        // send a unicast message to the server.
        sendMessageToServer(sock, message);
    } else {
        // handle unknown command.
        fprintf(stderr, ERROR_COMMAND_NOT_FOUND "\n");
    }
    return 0; // indicate no disconnection to the main loop.
}   

// handles the server's response to a command
void handleServerResponse(SOCKET *sock, const char *command, char **parameters) {
    char serverReply[DEFAULT_BUFLEN]; // buffer for server's reply.
    char registrationSuccessMessage[DEFAULT_BUFLEN]; // format for registration success message.
    int replyLength; // length of the received data.

    // skip handling for commands that do not expect a server reply.
    if (strcmp(command, COMMAND_LEAVE) == 0 || strcmp(command, COMMAND_HELP) == 0 
    || strcmp(command, COMMAND_STORE) == 0 || strcmp(command, COMMAND_GET) == 0) {
        return;
    }

    if ((strcmp(command, COMMAND_BROADCAST) == 0 || strcmp(command, COMMAND_UNICAST) == 0) 
    && registrationStatus == REGISTRATION_NOT_REGISTERED) {
        return;
    }

    // do not handle if disconnected.
    if (connectionStatus == DISCONNECTED) {
        return;
    }

    // receive the server's reply.
    replyLength = recv(*sock, serverReply, DEFAULT_BUFLEN, 0);

    // handle possible errors in receiving.
    if (replyLength == SOCKET_ERROR) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
    } else if (replyLength == 0) {
        // no data received.
        return;
    } else {
        // null-terminate the received data and process it.
        serverReply[replyLength] = '\0';
        
        if (strcmp(command, COMMAND_REGISTER) == 0) {
            // check if registration was successful.
            sprintf(registrationSuccessMessage, MESSAGE_SUCCESSFUL_REGISTRATION, parameters[0]);
            if (strcmp(serverReply, registrationSuccessMessage) == 0) {
                registrationStatus = REGISTRATION_REGISTERED;
            } 
        }
        // print the server's reply.
        printf("%s\n", serverReply);
    }
}

// initializes a socket connection using the provided ip address and port
void initSocketConnection(SOCKET *sock, SOCKADDR_IN *server, const char *ip, int port) {
    // set up the server address structure
    server->sin_family = AF_INET; // set address family to internet
    server->sin_addr.s_addr = inet_addr(ip); // set ip address
    server->sin_port = htons(port); // set port, converting to network byte order

    // attempt to connect to the server
    if (connect(*sock, (struct sockaddr *)server, sizeof(*server)) < 0) {
        fprintf(stderr, "Could not connect to server : %d", WSAGetLastError()); // print error message
        closesocket(*sock); // close socket on failure
        WSACleanup(); // clean up Winsock
        *sock = INVALID_SOCKET; // set socket to invalid
        return;
    }

    connectionStatus = CONNECTED; // set connection status flag to connected
}

int checkConnectionStatus(SOCKET sockfd) {
    fd_set readfds; // file descriptor set for select
    FD_ZERO(&readfds); // clear the set
    FD_SET(sockfd, &readfds); // add socket to the set

    struct timeval tv; // timeout structure
    tv.tv_sec = 0;  // 0 seconds
    tv.tv_usec = 0; // 0 microseconds

    // use select to check socket status without waiting
    int selectResult = select(0, &readfds, NULL, NULL, &tv);

    if (selectResult > 0) {
        // socket is ready for reading
        char buffer[1]; // buffer for peeking data
        int recvResult = recv(sockfd, buffer, 1, MSG_PEEK); // peek at the data

        if (recvResult > 0) {
            // data is available; the connection is alive
            return CONNECTED; // connection is alive
        } else {
            // the connection has been gracefully closed or an error occurred
            return DISCONNECTED; // disconnected or error
        }
    } else if (selectResult == 0) {
        // the select call timed out, the socket is not ready for reading
        // this does not necessarily mean the connection is down
        return CONNECTED; // assume connection is alive
    } else {
        // an error occurred with select
        return DISCONNECTED; // disconnected or error
    }
}

// sends a message to the server
void sendMessageToServer(SOCKET *sock, char *message) {
    // check if socket is valid and message is not null
    if (*sock == INVALID_SOCKET || message == NULL) {
        return; // do nothing if socket is invalid or message is null
    }

    // send the message to the server
    int messageLength = strlen(message); // calculate message length
    int bytesSent = send(*sock, message, messageLength, 0); // send the message

    // check for sending errors
    if (bytesSent == SOCKET_ERROR) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message
        return;
    }
}

// sends a file to the server
void sendFileToServer(SOCKET *sock, const char *filename) {
    FILE *file; // file pointer
    char filePath[256] = "files/"; // base file path
    strncat(filePath, filename, sizeof(filePath) - strlen(filePath) - 1); // append filename to path

    file = fopen(filePath, "rb"); // open file in binary read mode
    if (file == NULL) {
        fprintf(stderr, ERROR_FILE_NOT_FOUND_CLIENT "\n"); // print error message if file not found
        long errorCode = htonl(0); // indicate file not found with 0
        if(send(*sock, (char*)&errorCode, sizeof(errorCode), 0) == SOCKET_ERROR) {
            fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message on send failure
        }
        return;
    }

    long confirmationCode = htonl(1); // indicate file found with 1
    if (send(*sock, (char*)&confirmationCode, sizeof(confirmationCode), 0) == SOCKET_ERROR) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message on send failure
        fclose(file); // close file
        return;
    }

    // determine the file size
    fseek(file, 0, SEEK_END); // move to end of file
    long fileSize = ftell(file); // get current position (file size)
    fseek(file, 0, SEEK_SET); // move back to start of file

    // convert fileSize to network byte order and send it
    long fileSizeNetOrder = htonl(fileSize); // convert to network byte order
    if (send(*sock, (char*)&fileSizeNetOrder, sizeof(fileSizeNetOrder), 0) == SOCKET_ERROR) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message on send failure
        fclose(file); // close file
        return;
    }

    char buffer[DEFAULT_BUFLEN]; // buffer for file data
    int bytesRead; // bytes read from file
    
    // send the file data
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(*sock, buffer, bytesRead, 0) == SOCKET_ERROR) {
            fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message on send failure
            break; // exit loop on send failure
        }
    }

    char successMessage[DEFAULT_BUFLEN]; // buffer for success message
    int recvResult = recv(*sock, successMessage, sizeof(successMessage), 0); // receive success message
    if (recvResult <= 0) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message on receive failure
    } else {
        successMessage[recvResult] = '\0'; // null-terminate the received message
        printf("%s\n", successMessage); // print success message
    }

    fclose(file); // close the file
}

void receiveFileFromServer(SOCKET *sock, const char *filename) {
    char filePath[256] = "files/"; // base file path
    strncat(filePath, filename, sizeof(filePath) - strlen(filePath) - 1); // append filename to path

    // receive initial message from server to check for errors or file size
    char serverReply[DEFAULT_BUFLEN]; // buffer for server reply
    int msgLen = recv(*sock, serverReply, DEFAULT_BUFLEN, 0); // leave space for null terminator
    if (msgLen <= 0) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message on receive failure
        return;
    } else {
        serverReply[msgLen] = '\0'; // null-terminate the received message
    }

    // check if the received message is an error message
    if (strcmp(serverReply, ERROR_FILE_NOT_FOUND_SERVER) == 0) {
        printf("%s\n", serverReply); // print error message
        return;
    }

    // open the file for writing in binary mode
    FILE *file = fopen(filePath, "wb"); // open file in binary write mode
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n"); // print error message on file open failure
        return;
    }

    // receive the file size
    long fileSizeNetOrder; // variable for file size in network byte order
    if (recv(*sock, (char*)&fileSizeNetOrder, sizeof(fileSizeNetOrder), 0) <= 0) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message on receive failure
        fclose(file); // close file
        return;
    }
    long fileSize = ntohl(fileSizeNetOrder); // convert from network byte order to host byte order

    // receive the file data
    char buffer[DEFAULT_BUFLEN]; // buffer for file data
    int bytesReceived; // bytes received from server
    long totalBytesReceived = 0; // total bytes received
    while (totalBytesReceived < fileSize) {
        bytesReceived = recv(*sock, buffer, sizeof(buffer), 0); // receive file data
        if (bytesReceived > 0) {
            fwrite(buffer, 1, bytesReceived, file); // write data to file
            totalBytesReceived += bytesReceived; // update total bytes received
        } else if (bytesReceived == 0) {
            break; // break loop if connection closed
        } else {
            fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message on receive failure
            break; // break loop on receive failure
        }
    }

    fclose(file); // close the file

    if (totalBytesReceived != fileSize) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message if file size mismatch
    } else {
        printf(MESSAGE_SUCCESSFUL_FILE_DOWNLOAD "\n", filename); // print success message
    }
}