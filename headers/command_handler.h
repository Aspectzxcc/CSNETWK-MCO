#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

// function prototypes
const Command *getCommand(char *input);
char **parseCommandParameters(const Command *command, char *input);
char **executeCommand(const Command *command, char **parameters);

#endif // COMMAND_PARSER_H