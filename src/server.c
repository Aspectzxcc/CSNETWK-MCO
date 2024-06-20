#include "server.h"

// function to handle client connections
DWORD WINAPI client_handler(void* data) {
    // cast the void pointer back to a socket type
    SOCKET clientSocket = *(SOCKET*)data;

    // message to send to the client
    char* message = "hello client";

    // send the message to the client
    send(clientSocket, message, strlen(message), 0);

    // close the client socket after sending the message
    closesocket(clientSocket);
    return 0;
}

int main() {
    WSADATA wsaData; // holds Winsock data
    SOCKET serverSocket, clientSocket; // server and client socket descriptors
    struct sockaddr_in server, client; // server and client address structures
    int c; // size of the address structure

    // initialize Winsock
    WSAStartup(MAKEWORD(2,2), &wsaData);

    // create a socket for the server
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // configure server address structure
    server.sin_family = AF_INET; // internet protocol v4 addresses
    server.sin_addr.s_addr = INADDR_ANY; // accept connections to all the IPs of the machine
    server.sin_port = htons(12345); // port number

    // bind the socket to the server address
    bind(serverSocket, (struct sockaddr *)&server, sizeof(server));

    // listen for incoming connections
    listen(serverSocket, 3);

    // waiting for incoming connections
    puts("waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);

    // accept a connection from a client
    while((clientSocket = accept(serverSocket, (struct sockaddr *)&client, &c)) != INVALID_SOCKET) {
        puts("connection accepted");

        // create a thread to handle the client
        HANDLE thread = CreateThread(NULL, 0, client_handler, (void*)&clientSocket, 0, NULL);
        if (thread == NULL) {
            // if thread creation fails, print the error
            printf("CreateThread failed with error code : %d", GetLastError());
        } else {
            // close the thread handle as it's not needed anymore
            CloseHandle(thread);
        }
    }

    // if accepting a client fails, print the error
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed with error code : %d", WSAGetLastError());
    }

    // cleanup: close the server socket and deinitialize Winsock
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}