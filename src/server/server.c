#include "../../headers/server.h"

// array to store client information
Client clients[MAX_CLIENTS];
int clientCount = 0; // counter for the number of connected clients

int main() {
    WSADATA wsaData; // structure to hold Windows Sockets API data
    SOCKET serverSocket, clientSocket; // descriptors for server and client sockets
    SOCKADDR_IN server, client; // structures for server and client addresses
    int c; // variable to store the size of the address structure

    // initialize the Windows Sockets API
    WSAStartup(MAKEWORD(2,2), &wsaData);

    // create a TCP socket for the server
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // set up the server address structure
    server.sin_family = AF_INET; // use IPv4 addresses
    server.sin_addr.s_addr = INADDR_ANY; // accept connections on any IP address of the server
    server.sin_port = htons(12345); // set the server port number

    // bind the server socket to the specified IP address and port
    bind(serverSocket, (struct sockaddr *)&server, sizeof(server));

    // start listening for incoming connections
    listen(serverSocket, 3);

    c = sizeof(SOCKADDR_IN);

    // loop to accept incoming connections
    while((clientSocket = accept(serverSocket, (struct sockaddr *)&client, &c)) != INVALID_SOCKET) {
        puts("Connection accepted");

        // check if the maximum number of clients has not been reached
        if (clientCount < MAX_CLIENTS) {
            // add the new client to the array of clients
            clients[clientCount].clientSocket = clientSocket;

            // take the client address
            memcpy(&clients[clientCount].clientAddress, &client, sizeof(SOCKADDR_IN));
            clients[clientCount].clientAddress.sin_port = htons(8888);

            char portBuffer[sizeof(u_short)];
            int bytesRead = recv(clientSocket, portBuffer, sizeof(u_short), 0);

            if (bytesRead > 0) {
                u_short port;
                memcpy(&port, portBuffer, sizeof(u_short));
                clients[clientCount].clientAddress.sin_port = htons(port);
            }

            sendMessage(&clients[clientCount].clientSocket, MESSAGE_SUCCESSFUL_CONNECTION, -1);
            initUdpSenderSocket(&clients[clientCount].senderSocket);

            // create a new thread to handle the client
            HANDLE thread = CreateThread(NULL, 0, client_handler, (void*)&clients[clientCount], 0, NULL);
            if (thread == NULL) {
                // if thread creation fails, print an error message
                printf("CreateThread failed with error code: %d", GetLastError());
            } else {
                // close the thread handle as it is not needed anymore
                CloseHandle(thread);
            }

            // increment the client count
            clientCount++;
        } else {
            // if the maximum number of clients has been reached, reject the connection
            printf("Maximum number of clients reached. Connection rejected.\n");
            closesocket(clientSocket);
            continue;
        }
    }

    // if accepting a client fails, print an error message
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed with error code : %d", WSAGetLastError());
    }

    // perform cleanup: close the server socket and deinitialize the Windows Sockets API
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}