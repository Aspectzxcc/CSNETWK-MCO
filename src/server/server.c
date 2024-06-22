#include "../../headers/server.h"

// array to hold client aliases
Client clients[MAX_CLIENTS];
int clientCount = 0;

int main() {
    WSADATA wsaData; // holds Winsock data
    SOCKET serverSocket, clientSocket; // server and client socket descriptors
    SOCKADDR_IN server, client; // server and client address structures
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

    c = sizeof(SOCKADDR_IN);

    // accept a connection from a client
    while((clientSocket = accept(serverSocket, (struct sockaddr *)&client, &c)) != INVALID_SOCKET) {
        puts("Connection accepted");

        if (clientCount < MAX_CLIENTS) {
            // Add the client to the clients array
            clients[clientCount].clientSocket = clientSocket;

            // Create a thread to handle the client, using the current clientCount index
            HANDLE thread = CreateThread(NULL, 0, client_handler, (void*)&clients[clientCount], 0, NULL);
            if (thread == NULL) {
                // If thread creation fails, print the error
                printf("CreateThread failed with error code: %d", GetLastError());
            } else {
                // Close the thread handle as it's not needed anymore
                CloseHandle(thread);
            }

            // Increment clientCount after successfully creating the thread
            clientCount++;
        } else {
            // If the maximum number of clients is reached, print an error message
            printf("Maximum number of clients reached. Connection rejected.\n");
            closesocket(clientSocket);
            continue;
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