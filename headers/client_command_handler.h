#ifndef CLIENT_COMMAND_HANDLER_H
#define CLIENT_COMMAND_HANDLER_H

extern int g_isGUI;
extern HWND g_hConsoleOutput;
extern char g_serverDir[DEFAULT_BUFLEN];

typedef enum {
    DISCONNECTED,
    CONNECTED
} ConnectionStatus;

typedef enum {
    REGISTRATION_NOT_REGISTERED,
    REGISTRATION_REGISTERED
} RegistrationStatus;

typedef struct Client {
    SOCKET clientSocket;
    SOCKET receiverSocket;
    ConnectionStatus connectionStatus;
    RegistrationStatus registrationStatus;
} Client;

extern Client client;
extern HANDLE udpThread;

// function prototypes
int executeCommand(const char *command, char **parameters, char *message);
void initSocketConnection(SOCKET *sock, const char *ip, int port);
void disconnectFromServer(SOCKET *sock);
void registerAlias(SOCKET *sock, const char *alias);
void sendFileToServer(SOCKET *sock, const char *filename);
void receiveFileFromServer(SOCKET *sock, const char *filename);
void getServerDirectory(SOCKET *sock);
DWORD WINAPI listenForMessages(void *data);
void handleBroadcastAndUnicast(SOCKET *sock, const char *message);
void printCommandsW();

#endif // COMMAND_HANDLER_H