#include "../../headers/client.h"

// Modified function to take WSADATA and SOCKADDR_IN as parameters
void initSocketConnection(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const char *ip, int port) {
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), wsaData) != 0) {
        fprintf(stderr, "Failed to initialize Winsock. Error Code : %d", WSAGetLastError());
        *sock = INVALID_SOCKET;
        return;
    }

    // Create socket
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == INVALID_SOCKET) {
        fprintf(stderr, "Could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return;
    }

    // Setup the server structure
    server->sin_family = AF_INET;
    server->sin_addr.s_addr = inet_addr(ip);
    server->sin_port = htons(port);

    // Connect to remote server
    if (connect(*sock, (struct sockaddr *)server, sizeof(*server)) < 0) {
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
        closesocket(*sock);
        WSACleanup();
        *sock = INVALID_SOCKET;
    }
}

int executeCommand(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const Command *command, char **parameters) {
    if (strcmp(command->command, COMMAND_JOIN) == 0) {
        initSocketConnection(sock, wsaData, server, parameters[0], atoi(parameters[1]));
    } else if (strcmp(command->command, COMMAND_LEAVE) == 0) {
        return 1;
    } else if (strcmp(command->command, COMMAND_REGISTER) == 0) {
        // Implement other commands
    } else if (strcmp(command->command, COMMAND_STORE) == 0) { 
        // Implement other commands
    } else if (strcmp(command->command, COMMAND_DIR) == 0) {
        // Implement other commands
    } else if (strcmp(command->command, COMMAND_GET) == 0) {
        // Implement other commands
    } else if (strcmp(command->command, COMMAND_HELP) == 0) {
        // Implement other commands
    }
    return 0;
}

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