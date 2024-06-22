#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../../headers/server_command_handler.h"

// function to handle client connections
DWORD WINAPI client_handler(void* data) {
    Client* client = (Client*)data; // cast the data to a Client pointer
    
    // initialize client alias with empty string
    client->clientAlias[0] = '\0';

    char clientMessage[DEFAULT_BUFLEN]; // buffer to store client messages
    int messageLength = DEFAULT_BUFLEN; // set message length to default buffer length

    // continuously receive messages from the client
    while (1) {
        int bytesRead = recv(client->clientSocket, clientMessage, messageLength, 0); // read data from socket

        // check if client alias is not null and print it
        if (client->clientAlias[0] != '\0') {
            printf("Client: %s\n", client->clientAlias);
        } else {
            printf("Client: \n");
        }

        // handle recv errors
        if (bytesRead == SOCKET_ERROR) {
            fprintf(stderr, "recv failed with error code : %d", WSAGetLastError()); // log recv error
            break; // exit loop on error
        } else if (bytesRead == 0) {
            printf("Client disconnected\n"); // log client disconnection
            break; // exit loop if client disconnected
        } else {
            clientMessage[bytesRead] = '\0'; // null-terminate the received message
            printf("Message: %s\n\n", clientMessage); // print received message

            const Command *command = getCommand(clientMessage); // get command from message

            // check if command is valid
            if (command == NULL) {
                printf(ERROR_COMMAND_NOT_FOUND "\n"); // print error if command not found
                continue; // skip to next iteration on error
            }

            char **parameters = parseCommandParameters(command, clientMessage); // parse command parameters

            // check if parameters are valid
            if (parameters == NULL) {
                printf(ERROR_INVALID_PARAMETERS "\n"); // print error if parameters are invalid
                continue; // skip to next iteration on error
            }

            handleCommand(client, command->command, parameters); // handle the command
        }
    }

    // cleanup before exiting the thread
    closesocket(client->clientSocket); // close the client socket
    free(client); // free allocated memory for client alias
    return 0; // return success code
}

void handleCommand(Client *client, const char *command, char **parameters) {
    if ((strcmp(command, COMMAND_STORE) == 0 || strcmp(command, COMMAND_GET) == 0 || strcmp(command, COMMAND_DIR) == 0) && client->clientAlias[0] == '\0') {
        printf("Client not registered\n");
        send(client->clientSocket, ERROR_REGISTRATION_FAILED, strlen(ERROR_REGISTRATION_FAILED), 0); // send error message if client not registered
        return;
    }

    // handle JOIN command
    if (strcmp(command, COMMAND_JOIN) == 0) {
        send(client->clientSocket, MESSAGE_SUCCESSFUL_CONNECTION, strlen(MESSAGE_SUCCESSFUL_CONNECTION), 0); // send success message

    } else if (strcmp(command, COMMAND_LEAVE) == 0) {
        printf(client->clientAlias ? "Client %s disconnected\n" : "Client disconnected\n", client->clientAlias); // print client disconnection message

    } else if (strcmp(command, COMMAND_REGISTER) == 0) {
        // handle REGISTER command
        handleRegisterAlias(client, parameters[0]); // register client alias

    } else if (strcmp(command, COMMAND_STORE) == 0) {
        // handle STORE command
        uploadFileFromClient(client, parameters[0]); // upload file from client and send confirmation

    } else if (strcmp(command, COMMAND_GET) == 0) {
        // handle GET command
        sendFileToClient(client->clientSocket, parameters[0]); // send file to client

    } else if (strcmp(command, COMMAND_DIR) == 0) {
        // handle DIR command
        sendDirectoryFileList(client->clientSocket); // list files in directory
    } else {
        // command not found
        send(client->clientSocket, ERROR_COMMAND_NOT_FOUND, strlen(ERROR_COMMAND_NOT_FOUND), 0); // send error message if command not found
    }
}

void handleRegisterAlias(Client *client, char *alias) {
    // Check if the alias is already registered
    if (clientCount > 0) {
        for (int i = 0; i < clientCount; i++) {
            if (strcmp(clients[i].clientAlias, alias) == 0) {
                char response[DEFAULT_BUFLEN];
                printf("Client alias %s already registered.\n", clients[i].clientAlias);
                sprintf(response, ERROR_REGISTRATION_FAILED, alias);
                send(client->clientSocket, response, strlen(response), 0);
                return;
            }
        }
    }

    // Check if alias is not null and add it to the client
    if (alias != NULL && strlen(alias) > 0) {
        strcpy(client->clientAlias, alias);
    } else {
        char response[DEFAULT_BUFLEN];
        sprintf(response, ERROR_REGISTRATION_FAILED, alias);
        send(client->clientSocket, response, strlen(response), 0);
        return;
    }

    // Prepare and send the confirmation message
    char response[DEFAULT_BUFLEN];
    sprintf(response, MESSAGE_SUCCESSFUL_REGISTRATION, alias);
    send(client->clientSocket, response, strlen(response), 0);
}

void uploadFileFromClient(Client *client, char *filename) {
    char filePath[256] = "files/"; // Base directory for files
    strcat(filePath, filename); // Append filename to path

    // Open the file for writing in binary mode
    FILE *file = fopen(filePath, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    // Receive the file size first
    long fileSizeNetOrder;
    int bytesReceived = recv(client->clientSocket, (char*)&fileSizeNetOrder, sizeof(fileSizeNetOrder), 0);
    if (bytesReceived <= 0) {
        fprintf(stderr, "Failed to receive file size.\n");
        fclose(file);
        return;
    }
    long fileSize = ntohl(fileSizeNetOrder); // Convert from network byte order to host byte order
    printf("Expected file size: %ld bytes.\n", fileSize);

    char buffer[DEFAULT_BUFLEN];
    long totalBytesReceived = 0; // Track the total bytes received to compare with expected file size

    // Receive file data from client and write to file
    while (totalBytesReceived < fileSize) {
        bytesReceived = recv(client->clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            fwrite(buffer, 1, bytesReceived, file);
            totalBytesReceived += bytesReceived;
            printf("Received %d bytes\n", bytesReceived);
        } else if (bytesReceived == 0) {
            printf("Connection closed by client.\n");
            break;
        } else {
            fprintf(stderr, "recv failed with error: %d\n", WSAGetLastError());
            break;
        }
    }

    // Close the file
    fclose(file);

    // Check if the entire file was received
    if (totalBytesReceived == fileSize) {
        printf("File %s received and saved.\n", filename);
    } else {
        printf("File %s received partially. Expected %ld bytes but got %ld bytes.\n", filename, fileSize, totalBytesReceived);
    }

    // Prepare and send the confirmation message
    char response[DEFAULT_BUFLEN];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timeStr[20]; // For timestamp
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);

    // Check if client alias is not null and add it to the response
    if (client->clientAlias[0] != '\0' && strlen(client->clientAlias) > 0) {
        sprintf(response, "%s<%s>: Uploaded %s\n", client->clientAlias, timeStr, filename);
    } else {
        sprintf(response, "<%s>: Uploaded %s\n", timeStr, filename);
    }

    send(client->clientSocket, response, strlen(response), 0);
}

void sendFileToClient(SOCKET clientSocket, const char *filename) {
    char filePath[256] = "files/"; // Base directory for files
    strcat(filePath, filename); // Append filename to path

    // Open the file for reading in binary mode
    FILE *file = fopen(filePath, "rb");
    if (file == NULL) {
        send(clientSocket, ERROR_FILE_NOT_FOUND_SERVER, strlen(ERROR_FILE_NOT_FOUND_SERVER), 0);
        return;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET); // Reset file pointer to the beginning of the file

    // Send the file size first
    long fileSizeNetOrder = htonl(fileSize); // Convert to network byte order
    send(clientSocket, (char*)&fileSizeNetOrder, sizeof(fileSizeNetOrder), 0);

    // Send the file data
    char buffer[DEFAULT_BUFLEN];
    int bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(clientSocket, buffer, bytesRead, 0);
    }

    fclose(file); // Close the file
    printf("File %s sent to client.\n", filename);
}

void sendDirectoryFileList(SOCKET clientSocket) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("./files/*", &findFileData); // Adjust the path as needed

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        return;
    } 

    // Buffer to store the directory listing
    char directoryListing[1024] = "Server Directory\n";
    do {
        // Skip directories, only list files
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            strcat(directoryListing, findFileData.cFileName);
            strcat(directoryListing, "\n");
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    // Send the directory listing to the client
    send(clientSocket, directoryListing, strlen(directoryListing), 0);
}