#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/commands.h"
#include "../../headers/client_command_handler.h"
#include "../../headers/helpers.h"

#define DEFAULT_BUFLEN 1024 // default buffer size for command input

// executes the specified command with the provided parameters.
int executeCommand(const char *command, char **parameters, char *message) {
    client.connectionStatus = checkConnectionStatus(client.clientSocket); // check current connection status

    // check if the client is not connected and the command requires a connection.
    if (commandRequiresConnection(command) && client.connectionStatus == DISCONNECTED) {
        // log error if trying to execute a command without being connected.
        if (strcmp(command, COMMAND_LEAVE) == 0) {
            if (g_isGUI) {
                MessageBoxW(NULL, ERROR_DISCONNECT_FAILED_W, L"Error", MB_OK | MB_ICONERROR);
                AppendTextToConsoleOutput(g_hConsoleOutput, ERROR_DISCONNECT_FAILED_W);
                AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
            } else {
                fprintf(stderr, ERROR_DISCONNECT_FAILED "\n");
            }
        } else {
            if (g_isGUI) {
                MessageBoxW(NULL, ERROR_CONNECTION_FAILED_W, L"Error", MB_OK | MB_ICONERROR);
                AppendTextToConsoleOutput(g_hConsoleOutput, ERROR_CONNECTION_FAILED_W);
                AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
            } else {
                fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
            }
        }
        return 0;
    }

    // check if the client is not registered and tries to execute commands that require registration.
    if (commandRequiresRegistration(command) && client.registrationStatus == REGISTRATION_NOT_REGISTERED) {
        if (g_isGUI) {
            MessageBoxW(NULL, ERROR_REGISTRATION_FAILED_W, L"Error", MB_OK | MB_ICONERROR);
            AppendTextToConsoleOutput(g_hConsoleOutput, ERROR_REGISTRATION_FAILED_W);
            AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
        } else {
            fprintf(stderr, ERROR_REGISTRATION_FAILED "\n");
        }
        return 0;
    }

    // execute the command based on its identifier.
    if (strcmp(command, COMMAND_JOIN) == 0) {
        // initialize socket connection and send join message.
        initSocketConnection(&client.clientSocket, parameters[0], atoi(parameters[1]));
    } else if (strcmp(command, COMMAND_LEAVE) == 0) {
        // send leave message and close the socket.
        disconnectFromServer(&client.clientSocket);
        return 1; // indicate disconnection to the main loop.
    } else if (strcmp(command, COMMAND_REGISTER) == 0) {
        // send registration message to the server.
        registerAlias(&client.clientSocket, parameters[0]);
    } else if (strcmp(command, COMMAND_STORE) == 0) {
        // send a file to the server.
        sendFileToServer(&client.clientSocket, parameters[0]);
    } else if (strcmp(command, COMMAND_GET) == 0) {
        // request a file from the server.
        receiveFileFromServer(&client.clientSocket, parameters[0]);
    } else if (strcmp(command, COMMAND_DIR) == 0) {
        // send a directory listing request to the server.
        getServerDirectory(&client.clientSocket);
    } else if (strcmp(command, COMMAND_HELP) == 0) {
        // print available commands.
        if (g_isGUI) {
            printCommandsW();
        } else {
            printCommands();
        }
    } else if (strcmp(command, COMMAND_BROADCAST) == 0) {
        // send a broadcast message to the server.
        handleBroadcastAndUnicast(&client.clientSocket, message);
    } else if (strcmp(command, COMMAND_UNICAST) == 0) {
        // send a unicast message to the server.
        handleBroadcastAndUnicast(&client.clientSocket, message);
    } else {
        // handle unknown command.
        if (g_isGUI) {
            MessageBoxW(NULL, ERROR_COMMAND_NOT_FOUND_W, L"Error", MB_OK | MB_ICONERROR);
        } else {
            fprintf(stderr, ERROR_COMMAND_NOT_FOUND "\n");
        }
    }
    return 0; // indicate no disconnection to the main loop.
}   

// initializes a socket connection using the provided ip address and port
void initSocketConnection(SOCKET *sock, const char *ip, int port) {
    // set up the server address structure
    SOCKADDR_IN server;
    SOCKADDR_IN receiver;

    if (*sock != INVALID_SOCKET) {
        closesocket(*sock); // close the socket if it is already open
    }

    *sock = socket(AF_INET, SOCK_STREAM, 0); // create a TCP socket for the client

    if (*sock == INVALID_SOCKET) {
        if (g_isGUI) {
            wchar_t error[256];
            wsprintfW(error, L"Could not create socket : %d", WSAGetLastError());
            MessageBoxW(NULL, error, L"Error", MB_OK | MB_ICONERROR);
            AppendTextToConsoleOutput(g_hConsoleOutput, error);
            AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
        } else {
            fprintf(stderr, "Could not create socket : %d\n", WSAGetLastError());
        }
        return;
    }

    server.sin_family = AF_INET; // set address family to internet
    server.sin_addr.s_addr = inet_addr(ip); // set ip address
    server.sin_port = htons(port); // set port, converting to network byte order

    initUdpReceiverSocket(&client.receiverSocket, &receiver, ip); // initialize the UDP receiver socket
    udpThread = CreateThread(NULL, 0, listenForMessages, (void*)&client.receiverSocket, 0, NULL); // create a thread to listen for messages

    if (udpThread == NULL) {
        // if thread creation fails, print an error message
        if (g_isGUI) {
            wchar_t error[256];
            wsprintfW(error, L"CreateThread failed with error code: %d", GetLastError());
            MessageBoxW(NULL, error, L"Error", MB_OK | MB_ICONERROR);
            AppendTextToConsoleOutput(g_hConsoleOutput, error);
            AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
        } else {
            fprintf(stderr, "CreateThread failed with error code: %d", GetLastError());
        }
    } 

    // attempt to connect to the server
    if (connect(*sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        if (g_isGUI) {
            MessageBoxW(NULL, ERROR_CONNECTION_FAILED_W, L"Error", MB_OK | MB_ICONERROR);
            AppendTextToConsoleOutput(g_hConsoleOutput, ERROR_CONNECTION_FAILED_W);
            AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
        } else {
            fprintf(stderr, ERROR_CONNECTION_FAILED "\n");
        }
        *sock = INVALID_SOCKET; // set socket to invalid
        return;
    }

    u_short portNetOrder = htons(receiver.sin_port); // convert port to network byte order
    sendMessage(sock, (char*)&portNetOrder, sizeof(portNetOrder)); // send the UDP port to the server

    char serverReply[DEFAULT_BUFLEN]; // buffer for server reply
    strcpy(serverReply, receiveResponse(sock, serverReply, DEFAULT_BUFLEN)); // receive server reply

    if (strcmp(serverReply, MESSAGE_SUCCESSFUL_CONNECTION) == 0) {
        if (g_isGUI) {
            MessageBoxW(NULL, MESSAGE_SUCCESSFUL_CONNECTION_W, L"Success", MB_OK | MB_ICONINFORMATION);
            AppendTextToConsoleOutput(g_hConsoleOutput, MESSAGE_SUCCESSFUL_CONNECTION_W);
            AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
        } else {
            printf(MESSAGE_SUCCESSFUL_CONNECTION "\n");
        }
    }

    client.connectionStatus = CONNECTED; // set connection status flag to connected
}

void disconnectFromServer(SOCKET *sock) {
    sendMessage(sock, COMMAND_LEAVE, -1);
    closesocket(*sock); // close the socket
    CloseHandle(udpThread); // close the UDP listener thread
    client.connectionStatus = DISCONNECTED; // set connection status to disconnected

    if (g_isGUI) {
        MessageBoxW(NULL, MESSAGE_SUCCESSFUL_DISCONNECTION_W, L"Success", MB_OK | MB_ICONINFORMATION);
        AppendTextToConsoleOutput(g_hConsoleOutput, MESSAGE_SUCCESSFUL_DISCONNECTION_W);
        AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
    } else {
        printf(MESSAGE_SUCCESSFUL_DISCONNECTION "\n");
    }
}

void registerAlias(SOCKET *sock, const char *alias) {
    char registrationMessage[DEFAULT_BUFLEN]; // buffer for registration message
    sprintf(registrationMessage, "%s %s", COMMAND_REGISTER, alias); // format registration message
    sendMessage(sock, registrationMessage, -1); // send registration message

    char serverReply[DEFAULT_BUFLEN]; // buffer for server reply
    strcpy(serverReply, receiveResponse(sock, serverReply, DEFAULT_BUFLEN)); // receive server reply

    char successMessage[DEFAULT_BUFLEN]; // buffer for success message
    sprintf(successMessage, MESSAGE_SUCCESSFUL_REGISTRATION, alias); // format success message

    if (strcmp(serverReply, successMessage) == 0) {
        client.registrationStatus = REGISTRATION_REGISTERED; // set registration status to registered
    }
    
    if (g_isGUI) {
        wchar_t serverReplyW[DEFAULT_BUFLEN];
        MultiByteToWideChar(CP_ACP, 0, serverReply, -1, serverReplyW, DEFAULT_BUFLEN);
        MessageBoxW(NULL, serverReplyW, L"Server Reply", MB_OK | MB_ICONINFORMATION);
        AppendTextToConsoleOutput(g_hConsoleOutput, serverReplyW);
        AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
    } else {
        printf("%s\n", serverReply); // print server reply
    }
}

// sends a file to the server
void sendFileToServer(SOCKET *sock, const char *filename) {
    FILE *file; // file pointer
    char filePath[256] = "files/"; // base file path
    strncat(filePath, filename, sizeof(filePath) - strlen(filePath) - 1); // append filename to path

    file = fopen(filePath, "rb"); // open file in binary read mode
    if (file == NULL) {
        if (g_isGUI) {
            MessageBoxW(NULL, ERROR_FILE_NOT_FOUND_CLIENT_W, L"Error", MB_OK | MB_ICONERROR);
            AppendTextToConsoleOutput(g_hConsoleOutput, ERROR_FILE_NOT_FOUND_CLIENT_W);
            AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
        } else {
            fprintf(stderr, ERROR_FILE_NOT_FOUND_CLIENT "\n"); // print error message on file open failure
        }
        return;
    }

    char fileSendRequest[DEFAULT_BUFLEN]; // buffer for file send request
    sprintf(fileSendRequest, "%s %s", COMMAND_STORE, filename); // format file send request
    sendMessage(sock, fileSendRequest, -1); // send file send request

    // determine the file size
    fseek(file, 0, SEEK_END); // move to end of file
    long fileSize = ftell(file); // get current position (file size)
    fseek(file, 0, SEEK_SET); // move back to start of file

    // convert fileSize to network byte order and send it
    long fileSizeNetOrder = htonl(fileSize); // Convert to network byte order
    sendMessage(sock, (char*)&fileSizeNetOrder, sizeof(fileSizeNetOrder));

    char buffer[DEFAULT_BUFLEN]; // buffer for file data
    int bytesRead; // bytes read from file
    
    // send the file data
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(*sock, buffer, bytesRead, 0) == SOCKET_ERROR) {
            if (g_isGUI) {
                MessageBoxW(NULL, ERROR_CONNECTION_FAILED_W, L"Error", MB_OK | MB_ICONERROR);
                AppendTextToConsoleOutput(g_hConsoleOutput, ERROR_CONNECTION_FAILED_W);
                AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
            } else {
                fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message on send failure
            }
            break; // exit loop on send failure
        }
    }

    char serverReply[DEFAULT_BUFLEN]; // buffer for server reply
    strcpy(serverReply, receiveResponse(sock, serverReply, DEFAULT_BUFLEN)); // receive server reply

    if (g_isGUI) {
        wchar_t serverReplyW[DEFAULT_BUFLEN];
        MultiByteToWideChar(CP_ACP, 0, serverReply, -1, serverReplyW, DEFAULT_BUFLEN);
        MessageBoxW(NULL, serverReplyW, L"Server Reply", MB_OK | MB_ICONINFORMATION);
        AppendTextToConsoleOutput(g_hConsoleOutput, serverReplyW);
        AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
    } else {
        printf("%s\n", serverReply); // print server reply
    }

    fclose(file); // close the file
}

void receiveFileFromServer(SOCKET *sock, const char *filename) {
    char filePath[256] = "files/"; // base file path
    strncat(filePath, filename, sizeof(filePath) - strlen(filePath) - 1); // append filename to path

    char fileRequest[DEFAULT_BUFLEN]; // buffer for file request
    sprintf(fileRequest, "%s %s", COMMAND_GET, filename); // format file request
    sendMessage(sock, fileRequest, -1); // send file request

    // receive initial message from server to check for errors or file size
    char serverReply[DEFAULT_BUFLEN]; // buffer for server reply
    strcpy(serverReply, receiveResponse(sock, serverReply, DEFAULT_BUFLEN)); // receive server reply

    // check if the received message is an error message
    if (strcmp(serverReply, ERROR_FILE_NOT_FOUND_SERVER) == 0) {
        if (g_isGUI) {
            wchar_t serverReplyW[DEFAULT_BUFLEN];
            MultiByteToWideChar(CP_ACP, 0, serverReply, -1, serverReplyW, DEFAULT_BUFLEN);
            MessageBoxW(NULL, serverReplyW, L"Error", MB_OK | MB_ICONERROR);
            AppendTextToConsoleOutput(g_hConsoleOutput, serverReplyW);
            AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
        } else {
            printf("%s\n", serverReply); // print error message
        }
        return;
    }

    // open the file for writing in binary mode
    FILE *file = fopen(filePath, "wb"); // open file in binary write mode
    if (file == NULL) {
        if (g_isGUI) {
            wchar_t error[256];
            wsprintfW(error, L"Could not open file %s", filename);
            MessageBoxW(NULL, error, L"Error", MB_OK | MB_ICONERROR);
            AppendTextToConsoleOutput(g_hConsoleOutput, error);
            AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
        } else {
            fprintf(stderr, "Could not open file %s\n", filename); // print error message on file open failure
        }
        return;
    }

    // receive the file size
    long fileSizeNetOrder; // variable for file size in network byte order
    if (recv(*sock, (char*)&fileSizeNetOrder, sizeof(fileSizeNetOrder), 0) <= 0) {
        if (g_isGUI) {
            MessageBoxW(NULL, ERROR_CONNECTION_FAILED_W, L"Error", MB_OK | MB_ICONERROR);
            AppendTextToConsoleOutput(g_hConsoleOutput, ERROR_CONNECTION_FAILED_W);
            AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
        } else {
            fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message on receive failure
        }
        fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message on receive failure
        fclose(file); // close file
        return;
    }

    long fileSize = ntohl(fileSizeNetOrder); // convert from network byte order to host byte order

    // receive the file data
    char buffer[DEFAULT_BUFLEN]; // buffer for file data
    int bytesReceived; // bytes received from server
    long totalBytesReceived = 0; // total bytes received
    while (totalBytesReceived < fileSize) {
        bytesReceived = recv(*sock, buffer, sizeof(buffer), 0); // receive file data
        if (bytesReceived > 0) {
            fwrite(buffer, 1, bytesReceived, file); // write data to file
            totalBytesReceived += bytesReceived; // update total bytes received
        } else if (bytesReceived == 0) {
            break; // break loop if connection closed
        } else {
            if (g_isGUI) {
                MessageBoxW(NULL, ERROR_CONNECTION_FAILED_W, L"Error", MB_OK | MB_ICONERROR);
                AppendTextToConsoleOutput(g_hConsoleOutput, ERROR_CONNECTION_FAILED_W);
                AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
            } else {
                fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message on receive failure
            }
            break; // break loop on receive failure
        }
    }

    fclose(file); // close the file

    if (totalBytesReceived != fileSize) {
        if (g_isGUI) {
                MessageBoxW(NULL, ERROR_CONNECTION_FAILED_W, L"Error", MB_OK | MB_ICONERROR);
                AppendTextToConsoleOutput(g_hConsoleOutput, ERROR_CONNECTION_FAILED_W);
                AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
            } else {
                fprintf(stderr, ERROR_CONNECTION_FAILED "\n"); // print error message on receive failure
            }
    } else {
        if (g_isGUI) {
            wchar_t filenameW[256], message[256];
            MultiByteToWideChar(CP_ACP, 0, filename, -1, filenameW, 256);
            wsprintfW(message, MESSAGE_SUCCESSFUL_FILE_DOWNLOAD_W, filenameW);
            MessageBoxW(NULL, message, L"Success", MB_OK | MB_ICONINFORMATION);
            AppendTextToConsoleOutput(g_hConsoleOutput, message);
            AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
        } else {
            printf(MESSAGE_SUCCESSFUL_FILE_DOWNLOAD "\n", filename); // print success message
        }
    }
}

void getServerDirectory(SOCKET *sock) {
    sendMessage(sock, COMMAND_DIR, -1); // send directory listing request
    
    char serverReply[DEFAULT_BUFLEN]; // buffer for server reply
    strcpy(serverReply, receiveResponse(sock, serverReply, DEFAULT_BUFLEN)); // receive server reply
    strcpy(g_serverDir, serverReply); // store server directory listing (for GUI use)   
    
    if (g_isGUI) {
        wchar_t serverReplyW[DEFAULT_BUFLEN];
        MultiByteToWideChar(CP_ACP, 0, serverReply, -1, serverReplyW, DEFAULT_BUFLEN);
        AppendTextToConsoleOutput(g_hConsoleOutput, serverReplyW);
        AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
    } else {
        printf(MESSAGE_SUCCESSFUL_DIR_LIST "\n", serverReply); // print server reply
    
    }
}

DWORD WINAPI listenForMessages(void *data) {
    SOCKET receiverSocket = *(SOCKET*)data;
    char recvBuffer[DEFAULT_BUFLEN];
    int recvBufferLen = DEFAULT_BUFLEN;
    SOCKADDR_IN senderAddr;
    int senderAddrSize = sizeof(senderAddr);

    while (1) {
        memset(recvBuffer, 0, recvBufferLen); // Clear the buffer
        int bytesReceived = recvfrom(receiverSocket, recvBuffer, recvBufferLen, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);
        if (bytesReceived > 0) {
            // Successfully received a message
            recvBuffer[bytesReceived] = '\0'; // Null-terminate the buffer
            if (g_isGUI) {
                wchar_t recvBufferW[DEFAULT_BUFLEN];
                MultiByteToWideChar(CP_ACP, 0, recvBuffer, -1, recvBufferW, DEFAULT_BUFLEN);
                AppendTextToConsoleOutput(g_hConsoleOutput, recvBufferW);
                AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
            } else {
                printf("%s\n", recvBuffer);
            }
            // Process the received message here
        } else if (bytesReceived == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error != WSAEWOULDBLOCK) {
                // An error occurred, not just the non-blocking mode causing no data to be ready
                break; // Exit the loop on error
            }
        }
        // Implement a mechanism to gracefully exit the loop if needed
    }

    closesocket(receiverSocket); // Close the socket when done
    return 0; // Return value is required for functions of type DWORD WINAPI
}

void handleBroadcastAndUnicast(SOCKET *sock, const char *message) {
    sendMessage(sock, message, -1);

    char serverReply[DEFAULT_BUFLEN]; // buffer for server reply
    strcpy(serverReply, receiveResponse(sock, serverReply, DEFAULT_BUFLEN)); // receive server reply

    if (g_isGUI) {
        wchar_t serverReplyW[DEFAULT_BUFLEN];
        MultiByteToWideChar(CP_ACP, 0, serverReply, -1, serverReplyW, DEFAULT_BUFLEN);
        MessageBoxW(NULL, serverReplyW, L"Server Reply", MB_OK | MB_ICONINFORMATION);
        AppendTextToConsoleOutput(g_hConsoleOutput, serverReplyW);
        AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");
    } else {
        printf("%s\n", serverReply); // print server reply
    }
}

#include <windows.h>

void printCommandsW() {
    wchar_t message[DEFAULT_BUFLEN];
    wchar_t wideCommand[256]; // Buffer for the converted command string

    wsprintfW(message, L"Available commands:\n");
    AppendTextToConsoleOutput(g_hConsoleOutput, message);

    for (int i = 0; i < commandsCount; i++) {
        // Convert commands[i].command from char* to wchar_t*
        MultiByteToWideChar(CP_UTF8, 0, commands[i].command, -1, wideCommand, 256);

        if (strcmp(commands[i].command, COMMAND_JOIN) == 0) {
            wsprintfW(message, L"%s %s\n", wideCommand, COMMAND_JOIN_PARAMETERS_W);
        } else if (strcmp(commands[i].command, COMMAND_REGISTER) == 0) {
            wsprintfW(message, L"%s %s\n", wideCommand, COMMAND_REGISTER_PARAMETERS_W);
        } else if (strcmp(commands[i].command, COMMAND_STORE) == 0) {
            wsprintfW(message, L"%s %s\n", wideCommand, COMMAND_STORE_PARAMETERS_W);
        } else if (strcmp(commands[i].command, COMMAND_GET) == 0) {
            wsprintfW(message, L"%s %s\n", wideCommand, COMMAND_GET_PARAMETERS_W);
        } else if (strcmp(commands[i].command, COMMAND_BROADCAST) == 0) {
            wsprintfW(message, L"%s %s\n", wideCommand, COMMAND_BROADCAST_PARAMETERS_W);
        } else if (strcmp(commands[i].command, COMMAND_UNICAST) == 0) {
            wsprintfW(message, L"%s %s\n", wideCommand, COMMAND_UNICAST_PARAMETERS_W);
        } else {
            wsprintfW(message, L"%s\n", wideCommand);
        }

        AppendTextToConsoleOutput(g_hConsoleOutput, message);
    }
}