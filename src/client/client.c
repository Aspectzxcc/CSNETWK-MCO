#include "client.h"

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
        token = strtok(NULL, " "); // get the next token
        if (token != NULL) {
            parameters[index] = strdup(token); // duplicate the token to store in the array
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

    if (index != command->parameterCount) {
        fprintf(stderr, ERROR_INVALID_PARAMETERS "\n");
        // free allocated memory before returning
        for (int i = 0; i < index; i++) {
            free(parameters[i]);
        }
        free(parameters);
        return NULL;
    }

    return parameters;
}

int main() {
    char userInput[DEFAULT_BUFLEN]; // user input buffer
    WSADATA wsaData; // holds Winsock data
    SOCKET s; // client socket descriptor
    struct sockaddr_in server; // server address structure
    char *message, server_reply[DEFAULT_BUFLEN]; // message to send and server reply buffer
    int recv_size; // size of received data

    while (1) {
        fgets(userInput, sizeof(userInput), stdin); // get user input
        userInput[strcspn(userInput, "\n")] = 0; // remove newline character

        const Command *command = getCommand(userInput); // get the command from the user input

        if (command == NULL) {
            fprintf(stderr, ERROR_COMMAND_NOT_FOUND "\n");
            continue;
        }

        char **parameters = malloc(command->parameterCount * sizeof(char *));

        if (parameters == NULL) {
            fprintf(stderr, "Failed to allocate memory for parameters\n");
            continue; 
        }

        parameters = parseCommandParameters(command, userInput);

        if (parameters == NULL) {
            continue;
        }

        if (strcmp(command->command, COMMAND_LEAVE) == 0) {
            printf(MESSAGE_SUCCESSFUL_DISCONNECTION "\n");
            
            // cleanup before breaking the loop
            for (int i = 0; i < command->parameterCount; i++) {
                free(parameters[i]);
            }
            free(parameters);
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

    return 0;
}