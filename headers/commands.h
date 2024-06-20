#ifndef COMMANDS_H
#define COMMANDS_H

// commands list
#define COMMAND_JOIN "/join"
#define COMMAND_LEAVE "/leave"
#define COMMAND_REGISTER "/register"
#define COMMAND_STORE "/store"
#define COMMAND_DIR "/dir"
#define COMMAND_GET "/get"
#define COMMAND_HELP "/?"

// define message success responses
#define MESSAGE_SUCCESSFUL_CONNECTION "Connection to the File Exchange Server is successful!"
#define MESSAGE_SUCCESSFUL_DISCONNECTION "Connection closed. Thank you!"
#define MESSAGE_SUCCESSFUL_REGISTRATION "Welcome %s!" // %s is the user's handle or alias
#define MESSAGE_SUCCESSFUL_FILE_UPLOAD "%s<%s>: Uploaded %s" // %s and %s is the user's handle or alias, timestamp, and filename respectively
#define MESSAGE_SUCCESSFUL_DIR_LIST "Server Directory\n%s" // %s is the directory listing

// error messages
#define ERROR_CONNECTION_FAILED "Error: Connection to the Server has failed! Please check IP Address and Port Number."
#define ERROR_DISCONNECT_FAILED "Error: Disconnection failed. Please connect to the server first."
#define ERROR_REGISTRATION_FAILED "Error: Registration failed. Handle or alias already exists."
#define ERROR_FILE_NOT_FOUND "Error: File not found."
#define ERROR_FILE_NOT_FOUND_SERVER "Error: File not found in the server."
#define ERROR_COMMAND_NOT_FOUND "Error: Command not found."
#define ERROR_INVALID_PARAMETERS "Error: Command parameters do not match or is not allowed."

#define DEFAULT_BUFLEN 1024 // default buffer length

// command structure
typedef struct Command {
    const char* command;
    const int parameterCount;
} Command;

// commands list and count
extern const Command commands[];
extern const int commandsCount;

// function prototypes
const Command *getCommand(char *input);
char **parseCommandParameters(const Command *command, char *input);

#endif // COMMANDS_H