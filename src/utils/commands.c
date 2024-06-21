#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../headers/commands.h"

const Command commands[] = {
    {COMMAND_JOIN, 2}, // /join <server_ip_add> <port>
    {COMMAND_LEAVE, 0}, // /leave
    {COMMAND_REGISTER, 1}, // /register <handle>
    {COMMAND_STORE, 1}, // /store <filename>
    {COMMAND_DIR, 0}, // /dir
    {COMMAND_GET, 1}, // /get <filename>
    {COMMAND_HELP, 0} // /?
};

const int commandsCount = sizeof(commands) / sizeof(commands[0]); // number of commands

void printCommands() {
    printf("Available commands:\n");
    for (int i = 0; i < commandsCount; i++) {
        if (commands[i].command == COMMAND_JOIN) {
            printf("%s %s\n", commands[i].command, COMMAND_JOIN_PARAMETERS);
        } else if (commands[i].command == COMMAND_REGISTER) {
            printf("%s %s\n", commands[i].command, COMMAND_REGISTER_PARAMETERS);
        } else if (commands[i].command == COMMAND_STORE) {
            printf("%s %s\n", commands[i].command, COMMAND_STORE_PARAMETERS);
        } else if (commands[i].command == COMMAND_GET) {
            printf("%s %s\n", commands[i].command, COMMAND_GET_PARAMETERS);
        } else {
            printf("%s\n", commands[i].command);
        }
    }
}

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
    // Copy input to avoid modifying the original string
    char *inputCopy = strdup(input);
    if (inputCopy == NULL) {
        fprintf(stderr, "memory allocation failed for input copy\n");
        return NULL;
    }

    // allocate memory for parameters array based on expected parameter count
    char **parameters = malloc(command->parameterCount * sizeof(char *));
    if (parameters == NULL) {
        fprintf(stderr, "memory allocation failed for parameters\n");
        free(inputCopy); // Free the input copy if parameters allocation fails
        return NULL;
    }

    // tokenize inputCopy to skip the command itself and extract parameters
    char *token = strtok(inputCopy, " ");
    int index = 0;
    while (token != NULL && index < command->parameterCount) {
        token = strtok(NULL, " "); // get next parameter
        if (token != NULL) {
            parameters[index] = strdup(token); // duplicate parameter to ensure it's not overwritten
            if (parameters[index] == NULL) {
                fprintf(stderr, "memory allocation failed for parameter %d\n", index);
                // free allocated memory for parameters and inputCopy before error return
                for (int i = 0; i < index; i++) {
                    free(parameters[i]);
                }
                free(parameters);
                free(inputCopy); // Don't forget to free the input copy
                return NULL;
            }
            index++;
        }
    }

    // check if the number of parameters matches the expected count
    if (index != command->parameterCount) {
        // free allocated memory for parameters and inputCopy before returning null
        for (int i = 0; i < index; i++) {
            free(parameters[i]);
        }
        free(parameters);
        free(inputCopy); // Don't forget to free the input copy
        return NULL;
    }

    free(inputCopy); // Free the input copy after use
    return parameters; // return the array of parameters
}