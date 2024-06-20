#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h> // windows socket library
#include <stdio.h>
#include <string.h>

// link with winsock library
#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 1024 // default buffer length

extern const char *validCommands[];
extern const int validCommandsCount;

// function prototypes
int isValidCommand(char *input);

#endif // CLIENT_H