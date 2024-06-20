#include "../../headers/client.h"
#include "../../headers/command_parser.h"

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

// return the command struct from the input
const Command *getCommand(char *input) {
    char command[DEFAULT_BUFLEN]; // command buffer
    sscanf(input, "%s", command); // extract the command from the input

    for (int i = 0; i < commandsCount; i++) {
        if (strcmp(command, commands[i].command) == 0) {
            return &commands[i]; // return the command
        }
    }

    return NULL; // command not found
}

// execute the command
int executeCommand(const Command *command, char **parameters) {
    if (strcmp(command->command, COMMAND_JOIN) == 0) {

    } else if (strcmp(command->command, COMMAND_LEAVE) == 0) {
        printf(MESSAGE_SUCCESSFUL_DISCONNECTION "\n");

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

        breakLoop = executeCommand(command, parameters);

        if (breakLoop) {
            break;
        }
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