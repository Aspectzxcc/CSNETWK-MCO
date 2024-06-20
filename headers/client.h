#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h> // windows socket library
#include <stdio.h>
#include <string.h>
#include "commands.h" // commands header file
#include "client_command_handler.h" // command parser header file

// link with winsock library
#pragma comment(lib, "ws2_32.lib")

#endif // CLIENT_H