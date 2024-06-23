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
    SOCKET client, receiver; // socket descriptor for the client and receiver that is used for broadcast and unicast receiving
    SOCKADDR_IN server; // structure holding server address information

    char serverReply[DEFAULT_BUFLEN]; // buffer for server replies
    int replyLength; // length of the reply received from the server
    u_long mode; // variable to store the mode for setting the socket to non-blocking
    int broadcastPermission; // variable to store the permission for enabling broadcast on the socket

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Failed to initialize Winsock. Error code : %d", WSAGetLastError());
        return 1;
    }

    // Create the socket
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == INVALID_SOCKET) {
        fprintf(stderr, "Could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // set the mode to non-blocking
    mode = 1;

    // set the client socket to be non-blocking
    if (ioctlsocket(client, FIONBIO, &mode) != NO_ERROR) {
        fprintf(stderr, "Failed to set client socket to non-blocking mode. Error code: %d", WSAGetLastError());
        return 1;
    }

    // Create the UDP receiver socket for broadcast and unicast messages
    receiver = socket(AF_INET, SOCK_DGRAM, 0); // SOCK_DGRAM for UDP
    if (receiver == INVALID_SOCKET) {
        fprintf(stderr, "Could not create UDP receiver socket : %d", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // allow broadcast messages to be sent on the socket
    broadcastPermission = 1;
    if (setsockopt(receiver, SOL_SOCKET, SO_BROADCAST, (char *)&broadcastPermission, sizeof(broadcastPermission)) < 0) {
        fprintf(stderr, "Could not enable broadcast on socket: %d\n", WSAGetLastError());
        closesocket(client);
        WSACleanup();
        return 1;
    }

    // set the receiver socket to be non-blocking
    if (ioctlsocket(receiver, FIONBIO, &mode) != NO_ERROR) {
        fprintf(stderr, "Failed to set UDP receiver socket to non-blocking mode. Error code: %d", WSAGetLastError());
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

        connectionStatus = checkConnectionStatus(client); // check current connection status

        // execute the command with provided parameters
        breakLoop = executeCommand(&client, &server, command->command, parameters, userInput);

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