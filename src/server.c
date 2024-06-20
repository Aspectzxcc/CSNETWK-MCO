#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // link with ws2_32.lib

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in server, client;
    int c;

    // init winsock library
    WSAStartup(MAKEWORD(2,2), &wsaData);

    // create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(12345);

    // bind the socket
    bind(serverSocket, (struct sockaddr *)&server, sizeof(server));

    // listen for incoming connections
    listen(serverSocket, 3);

    // accept incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    clientSocket = accept(serverSocket, (struct sockaddr *)&client, &c);
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed with error code : %d", WSAGetLastError());
    }

    puts("Connection accepted");

    // communicate with client
    char *message = "Hello Client";
    send(clientSocket, message, strlen(message), 0);

    // disconnect and cleanup
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}