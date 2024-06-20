#include <stdio.h>
#include <string.h>
#include "../../headers/server_command_handler.h"

// function to handle client connections
DWORD WINAPI client_handler(void* data) {
    SOCKET clientSocket = *(SOCKET*)data;
    char *clientAlias;
    
    // allocate memory for the client alias
    clientAlias = malloc(MAX_ALIAS_LENGTH * sizeof(char));
    if (clientAlias == NULL) {
        fprintf(stderr, "Memory allocation for clientAlias failed\n");
        closesocket(clientSocket);
        return 1; // Return an error code
    }
    clientAlias[0] = '\0'; // Initialize with an empty string

    char clientMessage[DEFAULT_BUFLEN];
    int messageLength = DEFAULT_BUFLEN;

    // Loop to continuously receive and process messages from the client
    while (1) {
        int bytesRead = recv(clientSocket, clientMessage, messageLength, 0);

        if (clientAlias != NULL) {
            printf("Client: %s\n", clientAlias);
        } else {
            printf("Client: \n");
        }

        if (bytesRead == SOCKET_ERROR) {
            fprintf(stderr, "recv failed with error code : %d", WSAGetLastError());
            break; // exit loop on error
        } else if (bytesRead == 0) {
            printf("Client disconnected\n");
            break; // exit loop if client disconnected
        } else {
            clientMessage[bytesRead] = '\0'; // Null-terminate the received data
            printf("Message received from client: %s\n", clientMessage);

            const Command *command = getCommand(clientMessage);

            if (command == NULL) {
                printf(ERROR_COMMAND_NOT_FOUND "\n");
                continue; // skip to next iteration on error
            }

            char **parameters = parseCommandParameters(command, clientMessage);

            if (parameters == NULL) {
                printf(ERROR_INVALID_PARAMETERS "\n");
                continue; // skip to next iteration on error
            }

            handleCommand(clientSocket, command->command, parameters, &clientAlias);
        }
    }

    // Cleanup before exiting the thread
    closesocket(clientSocket);
    free(clientAlias);
    return 0;
}

void handleCommand(SOCKET clientSocket, const char *command, char **parameters, char **clientAlias) {
    if (strcmp(command, COMMAND_JOIN) == 0) {
        send(clientSocket, MESSAGE_SUCCESSFUL_CONNECTION, strlen(MESSAGE_SUCCESSFUL_CONNECTION), 0);
    } else if (strcmp(command, COMMAND_LEAVE) == 0) {
        printf("Client disconnected\n");
        closesocket(clientSocket); // Close the client socket to properly disconnect
    } else if (strcmp(command, COMMAND_REGISTER) == 0) {
        if (parameters[0] != NULL) {
            *clientAlias = parameters[0];
            clientAliases[clientAliasCount++] = parameters[0]; // store the client alias and increment the count
        }
        
        char response[DEFAULT_BUFLEN];
        sprintf(response, MESSAGE_SUCCESSFUL_REGISTRATION, *clientAlias);
        send(clientSocket, response, strlen(response), 0);
    } else if (strcmp(command, COMMAND_STORE) == 0) {
        // Assuming STORE command takes three parameters: user's handle, timestamp, and filename
        char response[DEFAULT_BUFLEN]; 
        sprintf(response, MESSAGE_SUCCESSFUL_FILE_UPLOAD, parameters[0], parameters[1], parameters[2]);
        send(clientSocket, response, strlen(response), 0);
    } else if (strcmp(command, COMMAND_DIR) == 0) {
        // Assuming LIST command requires no parameters and returns a directory listing
        char directoryListing[] = "File1.txt\nFile2.txt\nFile3.txt"; // Example directory listing
        char response[DEFAULT_BUFLEN];
        sprintf(response, MESSAGE_SUCCESSFUL_DIR_LIST, directoryListing);
        send(clientSocket, response, strlen(response), 0);
    } else {
        send(clientSocket, ERROR_COMMAND_NOT_FOUND, strlen(ERROR_COMMAND_NOT_FOUND), 0);
    }
}