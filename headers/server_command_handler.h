#ifndef SERVER_COMMAND_HANDLER_H
#define SERVER_COMMAND_HANDLER_H

#include <windows.h> // windows library for threading
#include "commands.h" // commands header file

// Function prototypes
DWORD WINAPI client_handler(void* data);
void handleCommand(SOCKET clientSocket, const char *command, char **parameters, char **clientAlias);
void uploadFileFromClient(SOCKET clientSocket, const char *clientAlias, const char *filename);
void sendFileToClient(SOCKET clientSocket, const char *filename);

#define MAX_CLIENTS 10 // maximum number of clients that can connect to the server
#define MAX_ALIAS_LENGTH 20 // maximum length of the client alias

extern char* clientAliases[MAX_CLIENTS];
extern int clientAliasCount;

#endif // SERVER_COMMAND_HANDLER_H