#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../../headers/server_command_handler.h"
#include "../../headers/helpers.h"

// function to handle client connections
DWORD WINAPI client_handler(void* data) {
    Client* client = (Client*)data; // cast input data to a client pointer
    
    // initialize client alias as an empty string
    client->clientAlias[0] = '\0';

    char clientMessage[DEFAULT_BUFLEN]; // buffer for storing client messages
    int messageLength = DEFAULT_BUFLEN; // initialize message length to default buffer size

    // loop to continuously receive messages from the client
    while (1) {
        int bytesRead = recv(client->clientSocket, clientMessage, messageLength, 0); // read data from the client socket

        // print client alias if it's not empty
        if (client->clientAlias[0] != '\0') {
            printf("Client: %s\n", client->clientAlias);
        } else {
            printf("Client: \n");
        }

        // handle errors from recv function
        if (bytesRead == SOCKET_ERROR) {
            fprintf(stderr, "recv failed in client_handler : %d\n", WSAGetLastError()); // log recv error
            break; // exit the loop on error
        } else if (bytesRead == 0) {
            printf("Client disconnected\n"); // log client disconnection
            break; // exit loop if client disconnected
        } else {
            clientMessage[bytesRead] = '\0'; // null-terminate the received message
            printf("Message: %s\n\n", clientMessage); // print the received message

            const Command *command = getCommand(clientMessage); // get command from the received message

            // validate the command
            if (command == NULL) {
                printf(ERROR_COMMAND_NOT_FOUND "\n"); // print error if command not found
                continue; // skip to the next iteration on error
            }

            char **parameters = parseCommandParameters(command, clientMessage); // parse parameters from the command

            // validate parameters
            if (parameters == NULL) {
                printf(ERROR_INVALID_PARAMETERS "\n"); // print error if parameters are invalid
                continue; // skip to the next iteration on error
            }

            handleCommand(client, command->command, parameters); // execute the command
        }
    }

    // cleanup before thread exit
    closesocket(client->clientSocket); // close the client socket
    return 0; // return success code
}

void handleCommand(Client *client, const char *command, char **parameters) {
    // check if client is registered for specific commands
    if (commandRequiresRegistration(command) && client->clientAlias[0] == '\0') {
        printf("Client not registered\n");
        send(client->clientSocket, ERROR_REGISTRATION_FAILED, strlen(ERROR_REGISTRATION_FAILED), 0); // send error message for unregistered client
        return;
    }

    // handle specific commands

    if (strcmp(command, COMMAND_LEAVE) == 0) {
        printf(client->clientAlias ? "Client %s disconnected\n" : "Client disconnected\n", client->clientAlias); // log client disconnection

    } else if (strcmp(command, COMMAND_REGISTER) == 0) {
        handleRegisterAlias(client, parameters[0]); // register client alias

    } else if (strcmp(command, COMMAND_STORE) == 0) {
        uploadFileFromClient(client, parameters[0]); // upload file from client

    } else if (strcmp(command, COMMAND_GET) == 0) {
        sendFileToClient(&client->clientSocket, parameters[0]); // send file to client

    } else if (strcmp(command, COMMAND_DIR) == 0) {
        sendDirectoryFileList(&client->clientSocket); // list files in directory

    } else if (strcmp(command, COMMAND_BROADCAST) == 0) {
        broadcastMessage(client, parameters[0]); // broadcast message to all clients

    } else if (strcmp(command, COMMAND_UNICAST) == 0) {
        // unicastMessage(client, parameters[0], parameters[1]); // unicast message to specific client

    } else {
        sendMessage(&client->clientSocket, ERROR_COMMAND_NOT_FOUND, strlen(ERROR_COMMAND_NOT_FOUND)); // send error message for unknown command
    }
}

void handleRegisterAlias(Client *client, char *alias) {
    // check if the alias is already in use
    if (clientCount > 0) {
        for (int i = 0; i < clientCount; i++) {
            if (strcmp(clients[i].clientAlias, alias) == 0) {
                char response[DEFAULT_BUFLEN];
                printf("Client alias %s already registered.\n", clients[i].clientAlias);
                sprintf(response, ERROR_REGISTRATION_FAILED, alias);
                sendMessage(&client->clientSocket, response, strlen(response));
                return;
            }
        }
    }

    // validate alias and add it to the client
    if (alias != NULL && strlen(alias) > 0) {
        strcpy(client->clientAlias, alias);
        printf("Client registered with alias %s\n", client->clientAlias);
    } else {
        char response[DEFAULT_BUFLEN];
        sprintf(response, ERROR_REGISTRATION_FAILED, alias);
        sendMessage(&client->clientSocket, response, strlen(response));
        return;
    }

    // send confirmation message for successful registration
    char response[DEFAULT_BUFLEN];
    sprintf(response, MESSAGE_SUCCESSFUL_REGISTRATION, alias);
    sendMessage(&client->clientSocket, response, strlen(response));
}

void uploadFileFromClient(Client *client, char *filename) {
    char filePath[256] = "files/"; // base directory for files
    strcat(filePath, filename); // append filename to path

    // open the file for writing in binary mode
    FILE *file = fopen(filePath, "wb");
    if (file == NULL) {
        fprintf(stderr, "error opening file for writing.\n");
        return;
    }

    // receive the file size first
    long fileSizeNetOrder;
    int bytesReceived = recv(client->clientSocket, (char*)&fileSizeNetOrder, sizeof(fileSizeNetOrder), 0);
    if (bytesReceived <= 0) {
        fprintf(stderr, "failed to receive file size.\n");
        fclose(file);
        return;
    }
    long fileSize = ntohl(fileSizeNetOrder); // convert from network byte order to host byte order
    printf("expected file size: %ld bytes.\n", fileSize);

    char buffer[DEFAULT_BUFLEN];
    long totalBytesReceived = 0; // track the total bytes received to compare with expected file size

    // receive file data from client and write to file
    while (totalBytesReceived < fileSize) {
        bytesReceived = recv(client->clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            fwrite(buffer, 1, bytesReceived, file);
            totalBytesReceived += bytesReceived;
            printf("received %d bytes\n", bytesReceived);
        } else if (bytesReceived == 0) {
            printf("connection closed by client.\n");
            break;
        } else {
            fprintf(stderr, "recv failed in uploadFileFromClient: %d\n", WSAGetLastError());
            break;
        }
    }

    // close the file
    fclose(file);

    // check if the entire file was received
    if (totalBytesReceived == fileSize) {
        printf("file %s received and saved.\n", filename);
    } else {
        printf("file %s received partially. expected %ld bytes but got %ld bytes.\n", filename, fileSize, totalBytesReceived);
    }

    // prepare and send the confirmation message
    char response[DEFAULT_BUFLEN];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timeStr[20]; // for timestamp
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);

    // check if client alias is not null and add it to the response
    if (client->clientAlias[0] != '\0' && strlen(client->clientAlias) > 0) {
        sprintf(response, "%s<%s>: uploaded %s", client->clientAlias, timeStr, filename);
    } else {
        sprintf(response, "<%s>: uploaded %s", timeStr, filename);
    }

    sendMessage(&client->clientSocket, response, strlen(response));
}

void sendFileToClient(SOCKET *clientSocket, const char *filename) {
    char filePath[256] = "files/"; // base directory for files
    strcat(filePath, filename); // append filename to path

    // open the file for reading in binary mode
    FILE *file = fopen(filePath, "rb");
    if (file == NULL) {
        sendMessage(clientSocket, ERROR_FILE_NOT_FOUND_CLIENT, -1);
        return;
    }

    // send confirmation message to confirm file transfer else send error message
    const char confirmationMessage[DEFAULT_BUFLEN] = "starting file transfer";
    sendMessage(clientSocket, confirmationMessage, -1);

    // get the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET); // reset file pointer to the beginning of the file

    // send the file size first
    long fileSizeNetOrder = htonl(fileSize); // convert to network byte order
    sendMessage(clientSocket, (char*)&fileSizeNetOrder, sizeof(fileSizeNetOrder));

    // send the file data
    char buffer[DEFAULT_BUFLEN];
    int bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(*clientSocket, buffer, bytesRead, 0);
    }

    fclose(file); // close the file
    printf("file %s sent to client.\n", filename);
    
}

void sendDirectoryFileList(SOCKET *clientSocket) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("./files/*", &findFileData); // adjust the path as needed

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("findfirstfile failed (%d)\n", GetLastError());
        return;
    } 

    // buffer to store the directory listing
    char directoryListing[1024] = "Server directory\n";
    do {
        // skip directories, only list files
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            strcat(directoryListing, findFileData.cFileName);
            strcat(directoryListing, "\n");
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    // send the directory listing to the client
    sendMessage(clientSocket, directoryListing, strlen(directoryListing));
}

void broadcastMessage(Client *client, char *message) {
    char formattedMessage[DEFAULT_BUFLEN]; // Assuming DEFAULT_BUFLEN is defined and large enough
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timeStr[20]; // for timestamp
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);

    // Format the message with the client's alias, timestamp, and the original message
    if (client->clientAlias[0] != '\0' && strlen(client->clientAlias) > 0) {
        sprintf(formattedMessage, SEND_MESSAGE_FORMAT, client->clientAlias, timeStr, message);
    } else {
        sprintf(formattedMessage, "<%s>: %s", timeStr, message); // Handle case where client alias might be empty
    }

    for (int i = 0; i < clientCount; i++) {
        if (clients[i].clientSocket != client->clientSocket) {
            // Use senderSocket for UDP broadcasting
            int sendResult = sendto(clients[i].senderSocket, formattedMessage, strlen(formattedMessage), 0, 
                                    (struct sockaddr *)&clients[i].clientAddress, sizeof(clients[i].clientAddress));
            if (sendResult == SOCKET_ERROR) {
                printf("Failed to send message to client %s, error: %d\n", clients[i].clientAlias, WSAGetLastError());
            } else {
                printf("Broadcast message sent to client %s\n", clients[i].clientAlias);
            }
        }
    }

    // Send confirmation message to the client that sent the broadcast
    sendMessage(&client->clientSocket, MESSAGE_SUCCESSFUL_BROADCAST, -1);
}