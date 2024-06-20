#include "../../headers/command_parser.h"

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