#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

// function prototypes
int executeCommand(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const char *command, char **parameters, char *message);
void initSocketConnection(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const char *ip, int port);
void sendMessageToServer(SOCKET *sock, char *message);
void sendFileToServer(SOCKET *sock, const char *filename);

#endif // COMMAND_PARSER_H