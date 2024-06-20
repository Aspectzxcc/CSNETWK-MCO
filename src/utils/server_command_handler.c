#include <stdio.h>
#include <string.h>
#include "../../headers/server_command_handler.h"

// function to handle client connections
DWORD WINAPI client_handler(void* data) {
    // cast the void pointer back to a socket type
    SOCKET clientSocket = *(SOCKET*)data;

    // buffer to store the message received from the client
    char clientMessage[DEFAULT_BUFLEN];
    int messageLength = DEFAULT_BUFLEN;

    // Receive message from client
    int bytesRead = recv(clientSocket, clientMessage, messageLength, 0);

    if (bytesRead > 0) {
        clientMessage[bytesRead] = '\0'; // Null-terminate the received data
        printf("Message received from client: %s\n", clientMessage);

        // Extract command and parameters from the received message
        const Command *command = getCommand(clientMessage);

        if (command == NULL) {
            printf(ERROR_COMMAND_NOT_FOUND "\n");
            return 0;
        }

        char **parameters = parseCommandParameters(command, clientMessage);

        if (parameters == NULL) {
            printf(ERROR_INVALID_PARAMETERS "\n");
            return 0;
        }

        // Handle the command based on the extracted command and parameters
        handleCommand(clientSocket, command->command, parameters);
    } else if (bytesRead == 0) {
        printf("Client disconnected\n");
    } else {
        printf("recv failed with error: %d\n", WSAGetLastError());
    }

    // close the client socket after sending and receiving the message
    closesocket(clientSocket);
    return 0;
}

void handleCommand(SOCKET clientSocket, const char *command, char **parameters) {
    if (strcmp(command, COMMAND_JOIN) == 0) {
        send(clientSocket, MESSAGE_SUCCESSFUL_CONNECTION, strlen(MESSAGE_SUCCESSFUL_CONNECTION), 0);
    } else if (strcmp(command, COMMAND_LEAVE) == 0) {
        printf("Client disconnected\n");
        closesocket(clientSocket); // Close the client socket to properly disconnect
    } else if (strcmp(command, COMMAND_REGISTER) == 0) {
        // Assuming REGISTER command takes one parameter: the user's handle or alias
        char response[DEFAULT_BUFLEN];
        sprintf(response, MESSAGE_SUCCESSFUL_REGISTRATION, parameters[0]);
        send(clientSocket, response, strlen(response), 0);
    } else if (strcmp(command, COMMAND_STORE) == 0) {
        // Assuming STORE command takes three parameters: user's handle, timestamp, and filename
        char response[DEFAULT_BUFLEN]; 
        sprintf(response, MESSAGE_SUCCESSFUL_FILE_UPLOAD, parameters[0], parameters[1], parameters[2]);
        send(clientSocket, response, strlen(response), 0);
    } else if (strcmp(command, COMMAND_DIR) == 0) {
        // Assuming LIST command requires no parameters and returns a directory listing
        char directoryListing[] = "File1.txt\nFile2.txt\nFile3.txt"; // Example directory listing
        char response[DEFAULT_BUFLEN];
        sprintf(response, MESSAGE_SUCCESSFUL_DIR_LIST, directoryListing);
        send(clientSocket, response, strlen(response), 0);
    } else {
        send(clientSocket, ERROR_COMMAND_NOT_FOUND, strlen(ERROR_COMMAND_NOT_FOUND), 0);
    }
}