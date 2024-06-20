#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h> // windows socket library
#include <windows.h> // windows library
#include <stdio.h>

// link with winsock library
#pragma comment(lib, "ws2_32.lib")

// define message success responses
#define MESSAGE_SUCCESSFUL_CONNECTION "Connection to the File Exchange Server is successful!"
#define MESSAGE_SUCCESSFUL_DISCONNECTION "Connection closed. Thank you!"
#define MESSAGE_SUCCESSFUL_REGISTRATION "Welcome %s!" // %s is the user's handle or alias
#define MESSAGE_SUCCESSFUL_FILE_UPLOAD "%s<%s>: Uploaded %s" // %s and %s is the user's handle or alias, timestamp, and filename respectively
#define MESSAGE_SUCCESSFUL_DIR_LIST "Server Directory\n%s" // %s is the directory listing

// function prototypes
DWORD WINAPI client_handler(void* data);

#endif // SERVER_H