#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "client.h" // Assuming this includes the definition of Command and other necessary includes

// Function prototypes
const Command *getCommand(char *input);
char **parseCommandParameters(const Command *command, char *input);

#endif // COMMAND_PARSER_H