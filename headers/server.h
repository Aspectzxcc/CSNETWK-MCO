#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h> // windows socket library
#include <stdio.h>
#include "commands.h" // commands header file
#include "server_command_handler.h" // server command handler header file
#include "helpers.h"

// link with winsock library
#pragma comment(lib, "ws2_32.lib")

#endif // SERVER_H