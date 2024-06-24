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
    char *inputCopy = strdup(input);
    if (inputCopy == NULL) {
        fprintf(stderr, "memory allocation failed for input copy\n");
        return NULL;
    }

    char **parameters = malloc(command->parameterCount * sizeof(char *));
    if (parameters == NULL) {
        fprintf(stderr, "memory allocation failed for parameters\n");
        free(inputCopy);
        return NULL;
    }

    if (strcmp(command->command, COMMAND_BROADCAST) == 0 || strcmp(command->command, COMMAND_UNICAST) == 0) {
        // For both broadcast and unicast, everything after the command is considered a single parameter
        char *message = strchr(inputCopy, ' ');
        if (message != NULL) {
            message++; // Move past the space to get the message
            if (strcmp(command->command, COMMAND_UNICAST) == 0) {
                // Duplicate the message to avoid modifying the original with strtok
                char *messageDup = strdup(message);
                if (messageDup == NULL) {
                    fprintf(stderr, "memory allocation failed for message duplication\n");
                    free(parameters);
                    free(inputCopy);
                    return NULL;
                }

                // Use strtok on the duplicated string to get the handle
                char *handle = strtok(messageDup, " ");
                if (handle != NULL) {
                    parameters[0] = strdup(handle);
                    if (parameters[0] == NULL) {
                        fprintf(stderr, "memory allocation failed for handle\n");
                        free(messageDup);
                        free(parameters);
                        free(inputCopy);
                        return NULL;
                    }

                    // Attempt to get the next part of the message, which should be the actual message
                    char *actualMessage = strtok(NULL, "");
                    if (actualMessage == NULL || *actualMessage == '\0') {
                        free(parameters[0]);
                        free(messageDup);
                        free(parameters);
                        free(inputCopy);
                        return NULL;
                    }

                    // Set the message parameter
                    message = strdup(actualMessage);
                } else {
                    // No handle found
                    free(messageDup);
                    free(parameters);
                    free(inputCopy);
                    return NULL;
                }

                // Free the duplicated message string
                free(messageDup);
            }

            // This part is common for both broadcast and unicast (for unicast, it's everything after the handle)
            parameters[strcmp(command->command, COMMAND_BROADCAST) == 0 ? 0 : 1] = strdup(message);
            if (parameters[strcmp(command->command, COMMAND_BROADCAST) == 0 ? 0 : 1] == NULL) {
                fprintf(stderr, "memory allocation failed for message\n");
                if (strcmp(command->command, COMMAND_UNICAST) == 0) {
                    free(parameters[0]); // Free the handle if it's unicast
                }
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

    free(inputCopy);
    return parameters;
}