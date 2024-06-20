#ifndef SERVER_COMMAND_HANDLER_H
#define SERVER_COMMAND_HANDLER_H

#include "commands.h" // commands header file
#include <windows.h> // windows library for threading

// Function prototypes
DWORD WINAPI client_handler(void* data);
void handleCommand(SOCKET clientSocket, const char *command, char **parameters);

#endif // SERVER_COMMAND_HANDLER_H