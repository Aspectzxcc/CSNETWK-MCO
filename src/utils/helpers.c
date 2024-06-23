#include <stdio.h>
#include <winsock2.h>
#include "../../headers/commands.h"
#include "../../headers/client_command_handler.h"
#include "../../headers/helpers.h"

void sendMessage(SOCKET *sock, const char *message, int length) {
    if (length < 0) {
        length = strlen(message); // Calculate length for strings
    }

    int sendResult = send(*sock, message, length, 0); // Send the message

    if (sendResult == SOCKET_ERROR) {
        fprintf(stderr, "Send failed : %d", WSAGetLastError()); // Print error message
        closesocket(*sock); // Close the socket
        WSACleanup(); // Clean up Winsock
        *sock = INVALID_SOCKET; // Set socket to invalid
    } else if (sendResult == 0) {
        fprintf(stderr, "Connection closed by server\n"); // Print error message
    }
}

char *receiveResponse(SOCKET *sock, char *buffer, int bufferLength) {
    int recvResult = recv(*sock, buffer, bufferLength, 0); // receive the response from the server

    if (recvResult > 0) {
        buffer[recvResult] = '\0'; // null-terminate the buffer
        return buffer; // return the buffer
    } else if (recvResult == 0) {
        fprintf(stderr, "Connection closed by server\n"); // print error message
        return NULL; // return NULL
    } else {
        fprintf(stderr, "Receive failed : %d", WSAGetLastError()); // print error message
        closesocket(*sock); // close the socket
        WSACleanup(); // clean up Winsock
        *sock = INVALID_SOCKET; // set socket to invalid
        return NULL; // return NULL
    }
}

int checkConnectionStatus(SOCKET sock) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int selectResult = select(0, &readfds, NULL, NULL, &tv);

    if (selectResult > 0) {
        char buffer[1];
        int recvResult = recv(sock, buffer, 1, MSG_PEEK);
        if (recvResult > 0) {
            return CONNECTED;
        } else {
            return DISCONNECTED;
        }
    } else if (selectResult == 0) {
        // Check for socket errors when select times out
        int error = 0;
        int len = sizeof(error);
        int retval = getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
        if (retval == 0 && error == 0) {
            return CONNECTED; // No errors, assume connection is alive
        } else {
            return DISCONNECTED; // Socket error detected
        }
    } else {
        return DISCONNECTED;
    }
}

int commandRequiresConnection(const char *command) {
    return strcmp(command, COMMAND_JOIN) != 0 && strcmp(command, COMMAND_HELP) != 0;
}

int commandRequiresRegistration(const char *command) {
    return strcmp(command, COMMAND_GET) == 0 || strcmp(command, COMMAND_STORE) == 0 
    || strcmp(command, COMMAND_BROADCAST) == 0 || strcmp(command, COMMAND_UNICAST) == 0;
}