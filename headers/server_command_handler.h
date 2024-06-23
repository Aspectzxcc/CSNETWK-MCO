#ifndef SERVER_COMMAND_HANDLER_H
#define SERVER_COMMAND_HANDLER_H

#include <windows.h> // windows library for threading
#include "commands.h" // commands header file

#define MAX_CLIENTS 10 // maximum number of clients that can connect to the server
#define MAX_ALIAS_LENGTH 20 // maximum length of the client alias

typedef struct Client {
    SOCKET clientSocket; // Socket associated with the client
    SOCKET receiverSocket; // Socket used for receiving broadcast and unicast messages
    char clientAlias[MAX_ALIAS_LENGTH]; // Alias of the client
} Client;

extern Client clients[MAX_CLIENTS]; // Array to hold client information
extern int clientCount; // Number of clients connected to the server

// Function prototypes
DWORD WINAPI client_handler(void* data);
void handleCommand(Client *client, const char *command, char **parameters);
void handleRegisterAlias(Client *client, char *alias);
void uploadFileFromClient(Client *client, char *filename);
void sendFileToClient(SOCKET *clientSocket, const char *filename);
void sendDirectoryFileList(SOCKET *clientSocket);
void broadcastMessage(Client *client, char *message);

#endif // SERVER_COMMAND_HANDLER_H