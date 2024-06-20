#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

// function prototypes
const Command *getCommand(char *input);
char **parseCommandParameters(const Command *command, char *input);
int executeCommand(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const Command *command, char **parameters);
void initSocketConnection(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const char *ip, int port);

#endif // COMMAND_PARSER_H