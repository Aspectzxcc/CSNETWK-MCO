#ifndef CLIENT_COMMAND_HANDLER_H
#define CLIENT_COMMAND_HANDLER_H

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
int executeCommand(SOCKET *sock, SOCKADDR_IN *server, const char *command, char **parameters, char *message);
void handleServerResponse(SOCKET *sock, const char *command, char **parameters);
void initSocketConnection(SOCKET *sock, SOCKADDR_IN *server, const char *ip, int port);
int checkConnectionStatus(SOCKET sock);
void sendMessageToServer(SOCKET *sock, char *message);
void sendFileToServer(SOCKET *sock, const char *filename);
void receiveFileFromServer(SOCKET *sock, const char *filename);

#endif // COMMAND_HANDLER_H