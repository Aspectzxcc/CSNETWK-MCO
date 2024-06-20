#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "client.h" // include client header file

// function prototypes
const Command *getCommand(char *input);
char **parseCommandParameters(const Command *command, char *input);

#endif // COMMAND_PARSER_H