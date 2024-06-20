#include "../../headers/client.h"

int main() {
    const Command *command; // command structure
    char **parameters; // command parameters
    char userInput[DEFAULT_BUFLEN]; // user input buffer
    int breakLoop; // flag to break the loop

    WSADATA wsaData; // holds Winsock data
    SOCKET s; // client socket descriptor
    SOCKADDR_IN server; // server address structure
    char *message, server_reply[DEFAULT_BUFLEN]; // message to send and server reply buffer
    int recv_size; // size of received data

    // unlimited loop to get user input
    while (1) {
        fgets(userInput, sizeof(userInput), stdin); // get user input
        userInput[strcspn(userInput, "\n")] = 0; // remove newline character

        command = getCommand(userInput); // get the command from the user input

        // check if the command is valid
        if (command == NULL) {
            fprintf(stderr, ERROR_COMMAND_NOT_FOUND "\n");
            continue;
        }

        parameters = malloc(command->parameterCount * sizeof(char *));

        if (parameters == NULL) {
            fprintf(stderr, "Failed to allocate memory for parameters\n");
            continue; 
        }

        parameters = parseCommandParameters(command, userInput);

        if (parameters == NULL) {
            fprintf(stderr, ERROR_INVALID_PARAMETERS "\n");
            continue;
        }

        // execute the command and pass in the socket, Winsock data, server address, command, and parameters
        breakLoop = executeCommand(&s, &wsaData, &server, command, parameters);

        if (breakLoop) {
            break;
        }
    }

    // free allocated memory
    for (int i = 0; i < command->parameterCount; i++) {
        free(parameters[i]);
    }

    free(parameters);

    return 0;
}