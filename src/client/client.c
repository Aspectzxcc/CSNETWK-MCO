#include "client.h"

const Command commands[] = {
    {COMMAND_JOIN, ERROR_CONNECTION_FAILED},
    {COMMAND_LEAVE, ERROR_DISCONNECT_FAILED},
    {COMMAND_REGISTER, ERROR_REGISTRATION_FAILED},
    {COMMAND_STORE, ERROR_FILE_NOT_FOUND},
    {COMMAND_DIR, ERROR_FILE_NOT_FOUND_SERVER},
    {COMMAND_GET, ERROR_FILE_NOT_FOUND_SERVER},
    {COMMAND_HELP, ERROR_COMMAND_NOT_FOUND}
};

const int commandsCount = sizeof(commands) / sizeof(commands[0]);

int main() {
    char userInput[DEFAULT_BUFLEN]; // user input buffer
    WSADATA wsaData; // holds Winsock data
    SOCKET s; // client socket descriptor
    struct sockaddr_in server; // server address structure
    char *message, server_reply[DEFAULT_BUFLEN]; // message to send and server reply buffer
    int recv_size; // size of received data

    fgets(userInput, sizeof(userInput), stdin);

    if (strncmp(userInput, "/join", 5) != 0) {
        puts("invalid command");
        return 1;
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

    puts("connected"); // print success message

    // receive a reply from the server
    if((recv_size = recv(s, server_reply, 2000, 0)) == SOCKET_ERROR) {
        puts("recv failed"); // if receiving fails, print error
    }

    puts("reply received\n"); // print success message
    server_reply[recv_size] = '\0'; // null-terminate the received data
    puts(server_reply); // print the server reply

    // cleanup
    closesocket(s); // close the socket
    WSACleanup(); // clean up winsock

    return 0;
}