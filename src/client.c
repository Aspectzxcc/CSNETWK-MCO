#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    SOCKET s;
    struct sockaddr_in server;
    char *message, server_reply[2000];
    int recv_size;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2,2), &wsaData);

    // Create socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    // Setup the server structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(12345);

    // Connect to remote server
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        puts("connect error");
        return 1;
    }

    puts("Connected");

    // Receive a reply from the server
    if((recv_size = recv(s, server_reply, 2000, 0)) == SOCKET_ERROR) {
        puts("recv failed");
    }

    puts("Reply received\n");
    server_reply[recv_size] = '\0';
    puts(server_reply);

    // Cleanup
    closesocket(s);
    WSACleanup();

    return 0;
}