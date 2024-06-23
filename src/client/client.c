#include "../../headers/client.h"

// global flags for connection and registration status
ConnectionStatus connectionStatus = DISCONNECTED; 
RegistrationStatus registrationStatus = REGISTRATION_NOT_REGISTERED; 

int main() {
    const Command *command; // holds the command structure after parsing user input
    char **parameters; // holds parameters for the command
    char userInput[DEFAULT_BUFLEN]; // buffer for storing user input
    int breakLoop; // flag to control the main loop

    WSADATA wsaData; // structure for Winsock data
    SOCKET client; // socket descriptor for the client
    SOCKADDR_IN server; // structure holding server address information

    char serverReply[DEFAULT_BUFLEN]; // buffer for server replies
    int replyLength; // length of the reply received from the server

    // main loop for command input and processing
    while (1) {
        fgets(userInput, sizeof(userInput), stdin); // read user input from stdin
        userInput[strcspn(userInput, "\n")] = 0; // remove newline character from input

        command = getCommand(userInput); // parse the command from user input

        // if command is not recognized, print error and continue
        if (command == NULL) {
            fprintf(stderr, ERROR_COMMAND_NOT_FOUND "\n");
            continue;
        }
        
        parameters = parseCommandParameters(command, userInput); // parse parameters for the command

        // if parameters are invalid, print error and continue
        if (parameters == NULL) {
            fprintf(stderr, ERROR_INVALID_PARAMETERS "\n");
            continue;
        }

        connectionStatus = checkConnectionStatus(client); // check current connection status

        // execute the command with provided parameters
        breakLoop = executeCommand(&client, &wsaData, &server, command->command, parameters, userInput);

        // process server response to the executed command
        handleServerResponse(&client, command->command, parameters);

        // if breakLoop is true, exit the loop
        if (breakLoop) {
            break;
        }
    }

    // cleanup operations
    WSACleanup(); // clean up Winsock
    closesocket(client); // close the client socket

    // free allocated memory for parameters
    for (int i = 0; i < command->parameterCount; i++) {
        free(parameters[i]);
    }

    free(parameters); // free the parameters array

    return 0;
}