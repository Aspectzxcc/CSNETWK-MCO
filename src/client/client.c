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
        fprintf(stderr, "Connect error");
        closesocket(*sock);
        WSACleanup();
        *sock = INVALID_SOCKET;
    }
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

        // breakLoop = executeCommand(command, parameters);

        // if (breakLoop) {
        //     break;
        // }
    }

    // initialize winsock
    WSAStartup(MAKEWORD(2,2), &wsaData); // request version 2.2 of winsock

    // create socket
    s = socket(AF_INET, SOCK_STREAM, 0); // create a socket for the IPv4 address family using TCP

    // setup the server structure
    server.sin_family = AF_INET; // set address family to IPv4
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // set server address
    server.sin_port = htons(12345); // set server port, converting from host to network byte order

    // connect to remote server
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        puts("connect error"); // if connection fails, print error
        return 1;
    }

    // cleanup
    closesocket(s); // close the socket
    WSACleanup(); // clean up winsock

    // free allocated memory
    for (int i = 0; i < command->parameterCount; i++) {
        free(parameters[i]);
    }

    free(parameters);

    return 0;
}