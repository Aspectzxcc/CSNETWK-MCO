#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "commands.h" // commands header file

#define DEFAULT_BUFLEN 1024 // default buffer length

// function prototypes
const Command *getCommand(char *input);
char **parseCommandParameters(const Command *command, char *input);

#endif // COMMAND_PARSER_H