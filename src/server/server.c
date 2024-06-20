#include "../../headers/server.h"

// function to handle client connections
DWORD WINAPI client_handler(void* data) {
    // cast the void pointer back to a socket type
    SOCKET clientSocket = *(SOCKET*)data;

    // buffer to store the message received from the client
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Receive message from client
    int bytesRead = recv(clientSocket, recvbuf, recvbuflen, 0);
    if (bytesRead > 0) {
        recvbuf[bytesRead] = '\0'; // Null-terminate the received data
        printf("Message received from client: %s\n", recvbuf);

        // Extract command and parameters from the received message
        Command *command = getCommand(recvbuf);

        if (command == NULL) {
            printf(ERROR_COMMAND_NOT_FOUND "\n");
            return 0;
        }

        char **parameters = parseCommandParameters(command, recvbuf);

        if (parameters == NULL) {
            printf(ERROR_INVALID_PARAMETERS "\n");
            return 0;
        }

        // Handle the command based on the extracted command and parameters
        handleCommand(clientSocket, command->command, parameters);
    } else if (bytesRead == 0) {
        printf(ERROR_CONNECTION_FAILED "\n");
    } else {
        printf("recv failed with error: %d\n", WSAGetLastError());
    }

    // close the client socket after sending and receiving the message
    closesocket(clientSocket);
    return 0;
}

void handleCommand(SOCKET clientSocket, const char *command, char **parameters) {
    if (strcmp(command, COMMAND_JOIN) == 0) {
        // Assuming JOIN command requires no parameters for this context
        send(clientSocket, MESSAGE_SUCCESSFUL_CONNECTION, strlen(MESSAGE_SUCCESSFUL_CONNECTION), 0);
    } else if (strcmp(command, COMMAND_LEAVE) == 0) {
        // Assuming LEAVE command signifies disconnection
        send(clientSocket, MESSAGE_SUCCESSFUL_DISCONNECTION, strlen(MESSAGE_SUCCESSFUL_DISCONNECTION), 0);
        closesocket(clientSocket); // Close the client socket to properly disconnect
    } else if (strcmp(command, COMMAND_REGISTER) == 0) {
        // Assuming REGISTER command takes one parameter: the user's handle or alias
        char response[DEFAULT_BUFLEN];
        sprintf(response, MESSAGE_SUCCESSFUL_REGISTRATION, parameters[0]);
        send(clientSocket, response, strlen(response), 0);
    } else if (strcmp(command, COMMAND_STORE) == 0) {
        // Assuming STORE command takes three parameters: user's handle, timestamp, and filename
        char response[DEFAULT_BUFLEN]; 
        sprintf(response, MESSAGE_SUCCESSFUL_FILE_UPLOAD, parameters[0], parameters[1], parameters[2]);
        send(clientSocket, response, strlen(response), 0);
    } else if (strcmp(command, COMMAND_DIR) == 0) {
        // Assuming LIST command requires no parameters and returns a directory listing
        char directoryListing[] = "File1.txt\nFile2.txt\nFile3.txt"; // Example directory listing
        char response[DEFAULT_BUFLEN];
        sprintf(response, MESSAGE_SUCCESSFUL_DIR_LIST, directoryListing);
        send(clientSocket, response, strlen(response), 0);
    } else {
        send(clientSocket, ERROR_COMMAND_NOT_FOUND, strlen(ERROR_COMMAND_NOT_FOUND), 0);
    }
}

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

    // waiting for incoming connections
    puts("waiting for incoming connections...");

    c = sizeof(SOCKADDR_IN);

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