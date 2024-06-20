#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h> // windows socket library
#include <stdio.h>
#include <string.h>

// link with winsock library
#pragma comment(lib, "ws2_32.lib")

#define COMMAND_JOIN "/join"
#define COMMAND_LEAVE "/leave"
#define COMMAND_REGISTER "/register"
#define COMMAND_STORE "/store"
#define COMMAND_DIR "/dir"
#define COMMAND_GET "/get"
#define COMMAND_HELP "/?"

#define MESSAGE_SUCCESSFUL_CONNECTION "Connection to the File Exchange Server is successful!"
#define MESSAGE_SUCCESSFUL_DISCONNECTION "Connection closed. Thank you!"
#define MESSAGE_SUCCESSFUL_REGISTRATION "Welcome %s!" // %s is the user's handle or alias
#define MESSAGE_SUCCESSFUL_FILE_UPLOAD "%s<%s>: Uploaded %s" // %s and %s is the user's handle or alias, timestamp, and filename respectively
#define MESSAGE_SUCCESSFUL_DIR_LIST "Server Directory\n%s" // %s is the directory listing

#define ERROR_CONNECTION_FAILED "Error: Connection to the Server has failed! Please check IP Address and Port Number."
#define ERROR_DISCONNECT_FAILED "Error: Disconnection failed. Please connect to the server first."
#define ERROR_REGISTRATION_FAILED "Error: Registration failed. Handle or alias already exists."
#define ERROR_FILE_NOT_FOUND "Error: File not found."
#define ERROR_FILE_NOT_FOUND_SERVER "Error: File not found in the server."
#define ERROR_COMMAND_NOT_FOUND "Error: Command not found."
#define ERROR_INVALID_PARAMETERS "Error: Command parameters do not match or is not allowed."

#define DEFAULT_BUFLEN 1024 // default buffer length

typedef struct Command {
    const char* command;
    const int parameterCount;
} Command;

extern const Command commands[];

// function prototypes
int isValidCommand(char *input);
char *getCommand (char *input);
char *parseCommandContent (char *input);

#endif // CLIENT_H