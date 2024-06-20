#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/commands.h"
#include "../../headers/command_handler.h"

#define DEFAULT_BUFLEN 1024 // default buffer length

// return the command struct from the input
const Command *getCommand(char *input) {
    char command[DEFAULT_BUFLEN]; // command buffer
    sscanf(input, "%s", command); // extract the command from the input

    for (int i = 0; i < commandsCount; i++) {
        if (strcmp(command, commands[i].command) == 0) {
            return &commands[i]; // return the command
        }
    }

    return NULL; // command not found
}

// returns an array of parameters from the input
char **parseCommandParameters(const Command *command, char *input) {
    char **parameters = malloc(command->parameterCount * sizeof(char *));
    if (parameters == NULL) {
        fprintf(stderr, "Memory allocation failed for parameters\n");
        return NULL;
    }

    char *token = strtok(input, " "); // skip command itself
    int index = 0;
    while (token != NULL && index < command->parameterCount) {
        token = strtok(NULL, " "); // get next token, NULL to continue from previous position
        if (token != NULL) {
            parameters[index] = strdup(token); // duplicate token stored in array to prevent token reference from being overwritten
            if (parameters[index] == NULL) {
                fprintf(stderr, "Memory allocation failed for parameter %d\n", index);
                // free previously allocated memory before returning
                for (int i = 0; i < index; i++) {
                    free(parameters[i]);
                }
                free(parameters);
                return NULL;
            }
            index++;
        }
    }

    if (index != command->parameterCount) { // if number of parameters do not match
        // free allocated memory before returning
        for (int i = 0; i < index; i++) {
            free(parameters[i]);
        }
        free(parameters);
        return NULL;
    }

    return parameters;
}

int executeCommand(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const Command *command, char **parameters) {
    if (strcmp(command->command, COMMAND_JOIN) == 0) {
        initSocketConnection(sock, wsaData, server, parameters[0], atoi(parameters[1]));
    } else if (strcmp(command->command, COMMAND_LEAVE) == 0) {
        return 1;
    } else if (strcmp(command->command, COMMAND_REGISTER) == 0) {
        // Implement other commands
    } else if (strcmp(command->command, COMMAND_STORE) == 0) { 
        // Implement other commands
    } else if (strcmp(command->command, COMMAND_DIR) == 0) {
        // Implement other commands
    } else if (strcmp(command->command, COMMAND_GET) == 0) {
        // Implement other commands
    } else if (strcmp(command->command, COMMAND_HELP) == 0) {
        // Implement other commands
    }
    return 0;
}

// Modified function to take WSADATA and SOCKADDR_IN as parameters
void initSocketConnection(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const char *ip, int port) {
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), wsaData) != 0) {
        fprintf(stderr, "Failed to initialize Winsock. Error Code : %d", WSAGetLastError());
        *sock = INVALID_SOCKET;
        return;
    }

    // Create socket
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == INVALID_SOCKET) {
        fprintf(stderr, "Could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return;
    }

    // Setup the server structure
    server->sin_family = AF_INET;
    server->sin_addr.s_addr = inet_addr(ip);
    server->sin_port = htons(port);

    // Connect to remote server
    if (connect(*sock, (struct sockaddr *)server, sizeof(*server)) < 0) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
        closesocket(*sock);
        WSACleanup();
        *sock = INVALID_SOCKET;
    }
}
