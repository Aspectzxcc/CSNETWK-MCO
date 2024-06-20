#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/commands.h"
#include "../../headers/client_command_handler.h"

#define DEFAULT_BUFLEN 1024 // default buffer size for command input

// function to identify the command struct based on user input
const Command *getCommand(char *input) {
    char command[DEFAULT_BUFLEN]; // buffer to hold the extracted command
    sscanf(input, "%s", command); // extract the first word from input as command

    // loop through available commands to find a match
    for (int i = 0; i < commandsCount; i++) {
        if (strcmp(command, commands[i].command) == 0) {
            return &commands[i]; // return matching command struct
        }
    }

    return NULL; // return null if no matching command is found
}

// function to parse and return parameters from the command input
char **parseCommandParameters(const Command *command, char *input) {
    // allocate memory for parameters array based on expected parameter count
    char **parameters = malloc(command->parameterCount * sizeof(char *));
    if (parameters == NULL) {
        fprintf(stderr, "memory allocation failed for parameters\n");
        return NULL;
    }

    // tokenize input to skip the command itself and extract parameters
    char *token = strtok(input, " ");
    int index = 0;
    while (token != NULL && index < command->parameterCount) {
        token = strtok(NULL, " "); // get next parameter
        if (token != NULL) {
            parameters[index] = strdup(token); // duplicate parameter to ensure it's not overwritten
            if (parameters[index] == NULL) {
                fprintf(stderr, "memory allocation failed for parameter %d\n", index);
                // free allocated memory for parameters before error return
                for (int i = 0; i < index; i++) {
                    free(parameters[i]);
                }
                free(parameters);
                return NULL;
            }
            index++;
        }
    }

    // check if the number of parameters matches the expected count
    if (index != command->parameterCount) {
        // free allocated memory before returning null
        for (int i = 0; i < index; i++) {
            free(parameters[i]);
        }
        free(parameters);
        return NULL;
    }

    return parameters; // return the array of parameters
}

// function to execute the identified command with given parameters
int executeCommand(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const Command *command, char **parameters, char *message) {
    // execute command based on its type
    if (strcmp(command->command, COMMAND_JOIN) == 0) {
        initSocketConnection(sock, wsaData, server, parameters[0], atoi(parameters[1]));
        sendMessageToServer(sock, message);
    } else if (strcmp(command->command, COMMAND_LEAVE) == 0) {
        sendMessageToServer(sock, message);
        return 1; // indicate disconnection
    } else {
        sendMessageToServer(sock, message);
    }
    // additional command checks can be implemented here
    return 0; // no disconnection
}

// function to initialize socket connection based on provided ip and port
void initSocketConnection(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const char *ip, int port) {
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

void sendMessageToServer(SOCKET *sock, const char *message) {
    if (*sock == INVALID_SOCKET || message == NULL) {
        fprintf(stderr, "Invalid socket or message is NULL\n");
        return;
    }

    int messageLength = strlen(message); // Get the length of the message
    int bytesSent = send(*sock, message, messageLength, 0); // Send the message

    if (bytesSent == SOCKET_ERROR) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
        return;
    }
}