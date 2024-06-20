#ifndef SERVER_COMMAND_HANDLER_H
#define SERVER_COMMAND_HANDLER_H

#include "commands.h" // commands header file
#include <windows.h> // windows library for threading

// Function prototypes
DWORD WINAPI client_handler(void* data);
void handleCommand(SOCKET clientSocket, const char *command, char **parameters, char **clientAlias);

#define MAX_CLIENTS 10 // maximum number of clients that can connect to the server
#define MAX_ALIAS_LENGTH 20 // maximum length of the client alias

extern char* clientAliases[MAX_CLIENTS];
extern int clientAliasCount;

#endif // SERVER_COMMAND_HANDLER_H