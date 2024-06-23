#ifndef HELPERS_H
#define HELPERS_H

// function prototypes
void sendMessage(SOCKET *sock, const char *message, int length);
char *receiveResponse(SOCKET *sock, char *buffer, int bufferLength);
int checkConnectionStatus(SOCKET clientSocket);
int commandRequiresConnection(const char *command);
int commandRequiresRegistration(const char *command);
void initUdpReceiverSocket(SOCKET *sock, SOCKADDR_IN *receiverAddress, const char *ip);
void initUdpSenderSocket(SOCKET *senderSocket);

#endif // HELPERS_H