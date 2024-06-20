#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in server, client;
    int c;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2,2), &wsaData);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(12345);

    // Bind
    bind(serverSocket, (struct sockaddr *)&server, sizeof(server));

    // Listen
    listen(serverSocket, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    clientSocket = accept(serverSocket, (struct sockaddr *)&client, &c);
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed with error code : %d", WSAGetLastError());
    }

    puts("Connection accepted");

    // Communicate with client
    char *message = "Hello Client";
    send(clientSocket, message, strlen(message), 0);

    // Cleanup
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}