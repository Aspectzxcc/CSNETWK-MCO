#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

typedef enum {
    DISCONNECTED,
    CONNECTED
} ConnectionStatus;

typedef enum {
    REGISTRATION_NOT_REGISTERED,
    REGISTRATION_REGISTERED
} RegistrationStatus;


extern ConnectionStatus connectionStatus;
extern RegistrationStatus registrationStatus;

// function prototypes
int executeCommand(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const char *command, char **parameters, char *message);
void handleServerResponse(SOCKET *sock, const char *command);
void initSocketConnection(SOCKET *sock, WSADATA *wsaData, SOCKADDR_IN *server, const char *ip, int port);
void sendMessageToServer(SOCKET *sock, char *message);
void sendFileToServer(SOCKET *sock, const char *filename);
void receiveFileFromServer(SOCKET *sock, const char *filename);

#endif // COMMAND_PARSER_H