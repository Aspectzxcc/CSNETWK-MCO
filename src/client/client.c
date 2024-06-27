#include "../../headers/client.h"

int g_isGUI = 0; // flag to determine if the client is running in GUI mode
HWND g_hConsoleOutput; // handle to the console output window (if GUI is enabled)

// client structure to hold client socket and status information
Client client = {INVALID_SOCKET, INVALID_SOCKET, DISCONNECTED, REGISTRATION_NOT_REGISTERED};
HANDLE udpThread; // thread handle for UDP communication

int main() {
    WSADATA wsaData; // structure for Winsock data
    const Command *command; // holds the command structure after parsing user input
    char **parameters; // holds parameters for the command
    char userInput[DEFAULT_BUFLEN]; // buffer for storing user input
    int breakLoop; // flag to control the main loop
    char serverReply[DEFAULT_BUFLEN]; // buffer for server replies
    int replyLength; // length of the reply received from the server

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Failed to initialize Winsock. Error code : %d", WSAGetLastError());
        return 1;
    }

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

        // execute the command with provided parameters
        breakLoop = executeCommand(command->command, parameters, userInput);

        // if breakLoop is true, exit the loop
        if (breakLoop) {
            break;
        }
    }

    // cleanup operations
    WSACleanup(); // clean up Winsock
    closesocket(client.clientSocket); // close the client socket

    // free allocated memory for parameters
    for (int i = 0; i < command->parameterCount; i++) {
        free(parameters[i]);
    }

    free(parameters); // free the parameters array

    return 0;
}