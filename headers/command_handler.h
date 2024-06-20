#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

// function prototypes
const Command *getCommand(char *input);
char **parseCommandParameters(const Command *command, char *input);

#endif // COMMAND_PARSER_H