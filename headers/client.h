#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h> // windows socket library
#include <stdio.h>
#include <string.h>
#include "commands.h" // commands header file
#include "command_handler.h" // command parser header file

// link with winsock library
#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 1024 // default buffer length

#endif // CLIENT_H