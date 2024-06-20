#include "../../headers/commands.h"

const Command commands[] = {
    {COMMAND_JOIN, 2}, // /join <server_ip_add> <port>
    {COMMAND_LEAVE, 0}, // /leave
    {COMMAND_REGISTER, 1}, // /register <handle>
    {COMMAND_STORE, 1}, // /store <filename>
    {COMMAND_DIR, 0}, // /dir
    {COMMAND_GET, 1}, // /get <filename>
    {COMMAND_HELP, 0} // /?
};

const int commandsCount = sizeof(commands) / sizeof(commands[0]); // number of commands