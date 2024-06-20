#include "client.h"

const Command commands[] = {
    {COMMAND_JOIN, ERROR_CONNECTION_FAILED, 2}, // /join <server_ip_add> <port>
    {COMMAND_LEAVE, ERROR_DISCONNECT_FAILED, 0}, // /leave
    {COMMAND_REGISTER, ERROR_REGISTRATION_FAILED, 1}, // /register <handle>
    {COMMAND_STORE, ERROR_FILE_NOT_FOUND, 1}, // /store <filename>
    {COMMAND_DIR, ERROR_FILE_NOT_FOUND_SERVER, 0}, // /dir
    {COMMAND_GET, ERROR_FILE_NOT_FOUND_SERVER, 1}, // /get <filename>
    {COMMAND_HELP, ERROR_COMMAND_NOT_FOUND, 0} // /?
};

const int commandsCount = sizeof(commands) / sizeof(commands[0]);

char *parseCommandContent (char *input) {
    char *content = strchr(input, ' ');
    if (content != NULL) {
        return content + 1;
    }
    return NULL;
}

int main() {
    char userInput[DEFAULT_BUFLEN]; // user input buffer
    WSADATA wsaData; // holds Winsock data
    SOCKET s; // client socket descriptor
    struct sockaddr_in server; // server address structure
    char *message, server_reply[DEFAULT_BUFLEN]; // message to send and server reply buffer
    int recv_size; // size of received data

    fgets(userInput, sizeof(userInput), stdin);

    parseCommandContent(userInput);
    
    printf("Command content: %s\n", parseCommandContent(userInput));

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