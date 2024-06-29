#include "../../headers/server.h"

// array to store client information
Client *clients[MAX_CLIENTS];
int clientCount = 0; // counter for the number of connected clients

HWND g_hConsoleOutput; // handle to the console output window (if GUI is enabled)
void executeCommand(const char *command, char **parameters, char *userInput);


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
            // Dynamically allocate memory for a new client
            Client *newClient = (Client *)malloc(sizeof(Client));
            if (newClient == NULL) {
                printf("Failed to allocate memory for new client.\n");
                closesocket(clientSocket);
                continue;
            }

            // Initialize the new client's data
            newClient->clientSocket = clientSocket;
            memcpy(&newClient->clientAddress, &client, sizeof(SOCKADDR_IN));
            newClient->clientAddress.sin_port = htons(8888);

            char portBuffer[sizeof(u_short)];
            int bytesRead = recv(clientSocket, portBuffer, sizeof(u_short), 0);

            if (bytesRead > 0) {
                u_short port;
                memcpy(&port, portBuffer, sizeof(u_short));
                newClient->clientAddress.sin_port = htons(port);
            }

            sendMessage(&newClient->clientSocket, MESSAGE_SUCCESSFUL_CONNECTION, -1);
            initUdpSenderSocket(&newClient->senderSocket);

            // Add the new client to the array of clients
            clients[clientCount] = newClient;

            // Create a new thread to handle the client
            HANDLE thread = CreateThread(NULL, 0, client_handler, (void*)newClient, 0, NULL);
            if (thread == NULL) {
                printf("server CreateThread failed with error code: %d\n", GetLastError());
                // Cleanup if thread creation fails
                closesocket(newClient->clientSocket);
                free(newClient);
            } else {
                CloseHandle(thread); // Close the thread handle as it is not needed anymore
                clientCount++; // Increment the client count only on successful thread creation
            }
        } else {
            // if the maximum number of clients has been reached, reject the connection
            printf("Maximum number of clients reached. Connection rejected.\n");
            closesocket(clientSocket);
        }
    }

    // if accepting a client fails, print an error message
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed with error code : %d\n", WSAGetLastError());
    }

    // perform cleanup: close the server socket and deinitialize the Windows Sockets API
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}

void executeCommand(const char *command, char **parameters, char *userInput) {
    // placeholder function only
}