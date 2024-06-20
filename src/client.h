#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h> // windows socket library
#include <stdio.h>
#include <string.h>

// link with Windows Socket Library
#pragma comment(lib, "ws2_32.lib")

// Function declarations
int isValidCommand(char *input);

#endif // CLIENT_H