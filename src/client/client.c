#include "../../headers/client.h"

int connectionStatus = 0; // connection status flag
int registrationStatus = 0; // registration status flag

int main() {
    const Command *command; // command structure
    char **parameters; // command parameters
    char userInput[DEFAULT_BUFLEN]; // user input buffer
    int breakLoop; // flag to break the loop

    WSADATA wsaData; // holds Winsock data
    SOCKET client; // client socket descriptor
    SOCKADDR_IN server; // server address structure

    char serverReply[DEFAULT_BUFLEN]; // server reply buffer
    int replyLength; // size of received data

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
        breakLoop = executeCommand(&client, &wsaData, &server, command->command, parameters, userInput);

        // handle server response
        handleServerResponse(&client, command->command);

        if (breakLoop) {
            break;
        }
    }

    // cleanup
    WSACleanup(); // cleanup Winsock
    closesocket(client); // close the socket

    // free allocated memory
    for (int i = 0; i < command->parameterCount; i++) {
        free(parameters[i]);
    }

    free(parameters);

    return 0;
}