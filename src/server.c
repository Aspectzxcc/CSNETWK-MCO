#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // link with Windows Socket Library

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in server, client;
    int c;

    // initialize winsock library for use
    WSAStartup(MAKEWORD(2,2), &wsaData); // request version 2.2 of Winsock on the system

    // create a new socket for communication
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); // specify the IPv4 address family, TCP type socket

    // set up the sockaddr_in structure for the server
    server.sin_family = AF_INET; // use IPv4 addresses
    server.sin_addr.s_addr = INADDR_ANY; // accept connections to all the IPs of the machine
    server.sin_port = htons(12345); // specify port number, converting host byte order to network byte order

    // bind the socket to the local IP address and port
    bind(serverSocket, (struct sockaddr *)&server, sizeof(server));

    // listen on the socket for incoming connections
    listen(serverSocket, 3); // set the maximum number of queued connections

    // wait for incoming connections
    puts("waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    clientSocket = accept(serverSocket, (struct sockaddr *)&client, &c); // accept a connection
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed with error code : %d", WSAGetLastError()); // if failed, print error
    }

    puts("connection accepted");

    // send a welcome message to the client
    char *message = "hello client";
    send(clientSocket, message, strlen(message), 0); // send the message to the client

    // close the socket and clean up
    closesocket(serverSocket);
    WSACleanup(); // clean up Winsock

    return 0;
}