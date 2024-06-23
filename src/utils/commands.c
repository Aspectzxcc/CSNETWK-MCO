#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../headers/commands.h"

// array of available commands with their expected parameter count
const Command commands[] = {
    {COMMAND_JOIN, 2}, // /join <server_ip_add> <port>
    {COMMAND_LEAVE, 0}, // /leave
    {COMMAND_REGISTER, 1}, // /register <handle>
    {COMMAND_STORE, 1}, // /store <filename>
    {COMMAND_DIR, 0}, // /dir
    {COMMAND_GET, 1}, // /get <filename>
    {COMMAND_HELP, 0}, // /?
    {COMMAND_BROADCAST, 1}, // /broadcast <message>
    {COMMAND_UNICAST, 2} // /unicast <handle> <message>
};

// commands count
const int commandsCount = sizeof(commands) / sizeof(commands[0]); // number of commands

// prints available commands
void printCommands() {
    printf("Available commands:\n");
    for (int i = 0; i < commandsCount; i++) {
        // checks for specific commands to print with parameters
        if (commands[i].command == COMMAND_JOIN) {
            printf("%s %s\n", commands[i].command, COMMAND_JOIN_PARAMETERS);
        } else if (commands[i].command == COMMAND_REGISTER) {
            printf("%s %s\n", commands[i].command, COMMAND_REGISTER_PARAMETERS);
        } else if (commands[i].command == COMMAND_STORE) {
            printf("%s %s\n", commands[i].command, COMMAND_STORE_PARAMETERS);
        } else if (commands[i].command == COMMAND_GET) {
            printf("%s %s\n", commands[i].command, COMMAND_GET_PARAMETERS);
        } else if (commands[i].command == COMMAND_BROADCAST) {
            printf("%s %s\n", commands[i].command, COMMAND_BROADCAST_PARAMETERS);
        } else if (commands[i].command == COMMAND_UNICAST) {
            printf("%s %s\n", commands[i].command, COMMAND_UNICAST_PARAMETERS);
        } else {
            printf("%s\n", commands[i].command);
        }
    }
}

// identifies the command struct based on user input
const Command *getCommand(char *input) {
    char command[DEFAULT_BUFLEN]; // buffer for the command
    sscanf(input, "%s", command); // extracts the command from input

    // loops through commands to find a match
    for (int i = 0; i < commandsCount; i++) {
        if (strcmp(command, commands[i].command) == 0) {
            return &commands[i]; // returns the matching command struct
        }
    }

    return NULL; // returns null if no match is found
}

// parses and returns parameters from the command input
char **parseCommandParameters(const Command *command, char *input) {
    // copies input to avoid modifying the original string
    char *inputCopy = strdup(input);
    if (inputCopy == NULL) {
        fprintf(stderr, "memory allocation failed for input copy\n");
        return NULL;
    }

    // allocates memory for parameters array
    char **parameters = malloc(command->parameterCount * sizeof(char *));
    if (parameters == NULL) {
        fprintf(stderr, "memory allocation failed for parameters\n");
        free(inputCopy); // frees input copy if allocation fails
        return NULL;
    }

    // Check if the command is 'broadcast' or 'unicast'
    if (strcmp(command->command, COMMAND_BROADCAST) == 0) {
        // For broadcast, everything after the command is considered a single parameter
        char *message = strchr(inputCopy, ' ');
        if (message != NULL) {
            message++; // Move past the space to get the message
            parameters[0] = strdup(message);
            if (parameters[0] == NULL) {
                fprintf(stderr, "memory allocation failed for message\n");
                free(parameters);
                free(inputCopy);
                return NULL;
            }
        } else {
            // No message found after the command
            free(parameters);
            free(inputCopy);
            return NULL;
        }
    } else if (strcmp(command->command, COMMAND_UNICAST) == 0) {
        // For unicast, the first parameter is the handle or alias, and everything after is the message
        char *handle = strtok(NULL, " "); // Use strtok to get the first parameter after the command
        if (handle != NULL) {
            parameters[0] = strdup(handle);
            if (parameters[0] == NULL) {
                fprintf(stderr, "memory allocation failed for handle\n");
                free(parameters);
                free(inputCopy);
                return NULL;
            }
            // Now, treat the rest of the input as the message
            char *message = inputCopy + (handle - inputCopy) + strlen(handle) + 1; // Calculate position
            parameters[1] = strdup(message);
            if (parameters[1] == NULL) {
                fprintf(stderr, "memory allocation failed for message\n");
                free(parameters[0]);
                free(parameters);
                free(inputCopy);
                return NULL;
            }
        } else {
            // No handle found after the command
            free(parameters);
            free(inputCopy);
            return NULL;
        }
    } else {
        // Existing logic for tokenizing input and extracting parameters
        char *token = strtok(inputCopy, " ");
        int index = 0;
        while (token != NULL && index < command->parameterCount) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                parameters[index] = strdup(token);
                if (parameters[index] == NULL) {
                    fprintf(stderr, "memory allocation failed for parameter %d\n", index);
                    for (int i = 0; i < index; i++) {
                        free(parameters[i]);
                    }
                    free(parameters);
                    free(inputCopy);
                    return NULL;
                }
                index++;
            }
        }

        if (index != command->parameterCount) {
            for (int i = 0; i < index; i++) {
                free(parameters[i]);
            }
            free(parameters);
            free(inputCopy);
            return NULL;
        }
    }

    free(inputCopy); // frees the input copy after use
    return parameters; // returns the parameters array
}