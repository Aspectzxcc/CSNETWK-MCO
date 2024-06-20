#include <stdio.h>
#include <string.h>
#include "../../headers/server_command_handler.h"

// function to handle client connections
DWORD WINAPI client_handler(void* data) {
    SOCKET clientSocket = *(SOCKET*)data; // cast data pointer to SOCKET type
    char *clientAlias;
    
    // allocate memory for the client alias
    clientAlias = malloc(MAX_ALIAS_LENGTH * sizeof(char)); // allocate memory for client alias
    if (clientAlias == NULL) {
        fprintf(stderr, "Memory allocation for clientAlias failed\n"); // log error if allocation fails
        closesocket(clientSocket); // close the socket if memory allocation fails
        return 1; // return error code
    }
    clientAlias[0] = '\0'; // initialize client alias with empty string

    char clientMessage[DEFAULT_BUFLEN]; // buffer to store client messages
    int messageLength = DEFAULT_BUFLEN; // set message length to default buffer length

    // continuously receive messages from the client
    while (1) {
        int bytesRead = recv(clientSocket, clientMessage, messageLength, 0); // read data from socket

        // check if client alias is not null and print it
        if (clientAlias != NULL) {
            printf("Client: %s\n", clientAlias);
        } else {
            printf("Client: \n");
        }

        // handle recv errors
        if (bytesRead == SOCKET_ERROR) {
            fprintf(stderr, "recv failed with error code : %d", WSAGetLastError()); // log recv error
            break; // exit loop on error
        } else if (bytesRead == 0) {
            printf("Client disconnected\n"); // log client disconnection
            break; // exit loop if client disconnected
        } else {
            clientMessage[bytesRead] = '\0'; // null-terminate the received message
            printf("Message received from client: %s\n", clientMessage); // print received message

            const Command *command = getCommand(clientMessage); // get command from message

            // check if command is valid
            if (command == NULL) {
                printf(ERROR_COMMAND_NOT_FOUND "\n"); // print error if command not found
                continue; // skip to next iteration on error
            }

            char **parameters = parseCommandParameters(command, clientMessage); // parse command parameters

            // check if parameters are valid
            if (parameters == NULL) {
                printf(ERROR_INVALID_PARAMETERS "\n"); // print error if parameters are invalid
                continue; // skip to next iteration on error
            }

            handleCommand(clientSocket, command->command, parameters, &clientAlias); // handle the command
        }
    }

    // cleanup before exiting the thread
    closesocket(clientSocket); // close the client socket
    free(clientAlias); // free allocated memory for client alias
    return 0; // return success code
}

void handleCommand(SOCKET clientSocket, const char *command, char **parameters, char **clientAlias) {
    // handle JOIN command
    if (strcmp(command, COMMAND_JOIN) == 0) {
        send(clientSocket, MESSAGE_SUCCESSFUL_CONNECTION, strlen(MESSAGE_SUCCESSFUL_CONNECTION), 0); // send success message
    } else if (strcmp(command, COMMAND_LEAVE) == 0) {
        printf("Client disconnected\n"); // log client disconnection
        closesocket(clientSocket); // close the client socket
    } else if (strcmp(command, COMMAND_REGISTER) == 0) {
        // handle REGISTER command
        if (parameters[0] != NULL) {
            *clientAlias = parameters[0]; // set client alias
            clientAliases[clientAliasCount++] = parameters[0]; // store client alias
        }
        
        char response[DEFAULT_BUFLEN];
        sprintf(response, MESSAGE_SUCCESSFUL_REGISTRATION, *clientAlias); // prepare success message
        send(clientSocket, response, strlen(response), 0); // send success message
    } else if (strcmp(command, COMMAND_STORE) == 0) {
        // handle STORE command
        char response[DEFAULT_BUFLEN]; 
        sprintf(response, MESSAGE_SUCCESSFUL_FILE_UPLOAD, parameters[0], parameters[1], parameters[2]); // prepare success message
        send(clientSocket, response, strlen(response), 0); // send success message
    } else if (strcmp(command, COMMAND_DIR) == 0) {
        // handle DIR command
        char directoryListing[] = "File1.txt\nFile2.txt\nFile3.txt"; // example directory listing
        char response[DEFAULT_BUFLEN];
        sprintf(response, MESSAGE_SUCCESSFUL_DIR_LIST, directoryListing); // prepare success message
        send(clientSocket, response, strlen(response), 0); // send success message
    } else {
        send(clientSocket, ERROR_COMMAND_NOT_FOUND, strlen(ERROR_COMMAND_NOT_FOUND), 0); // send error message if command not found
    }
}