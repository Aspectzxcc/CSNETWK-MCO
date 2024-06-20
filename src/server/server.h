#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h> // windows socket library
#include <windows.h> // windows library
#include <stdio.h>

// link with winsock library
#pragma comment(lib, "ws2_32.lib")

// function prototypes
DWORD WINAPI client_handler(void* data);

#endif // SERVER_H