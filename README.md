# CSNETWK-MCO File Exchange System

## Overview

The CSNETWK-MCO File Exchange System is designed as the final project for our CSNETWK course. It facilitates the storage, sharing, and retrieval of files among clients through a single server, utilizing either TCP or UDP protocols. The system is divided into two main applications: the server application and the client application.

## Current State of the Project

As of now, the project has successfully implemented the foundational aspects of both the server and client applications using TCP protocol. The server can initialize sockets, bind to an address and port, and listen for connections. The server still lacks handling of the commands and no commands are being sent to the server from the client yet as the client command execution is still being developed.

### Project Structure and Dependencies

The project is organized into several key directories to ensure modularity and ease of navigation:

- `src/`: Contains the source code for the project, further organized into subdirectories for clarity:
  - `client/`: Houses the client application source code (`client.c`), responsible for interfacing with the user and communicating with the server.
  - `server/`: Contains the server application source code (`server.c`), which listens for and handles requests from clients.
  - `utils/`: Includes utility functions and common code used by both the client and server applications, such as file handling and network utilities.
- `headers/`: Includes header files such as `server.h` and `client.h`, which define necessary structures and prototypes for the server and client applications, respectively.
- `.gitignore`: Specifies intentionally untracked files to ignore.

Dependencies include:

- **Winsock2 Library**: For socket programming on Windows.
- **C Standard Library**: Utilized for basic input/output operations, string manipulation, and memory management.

### Development and Testing

#### Development Tools

- **Visual Studio Code**: Editor used for code editing and version control.
- **GCC Compiler**: For compiling the C code. [MinGW](https://sourceforge.net/projects/mingw/).

### Running the Applications

To set up the applications, open multiple terminals - one for the server and additional ones for each client. Navigate to the server directory in one terminal and to the client directory in others. Run the designated batch files for compilation and execution in each terminal individually:

- For the server: Execute `compile_server.bat` to compile and then run the server application.
- For each client: Execute `compile_client.bat` to compile and then run the client application.

## Resources

- **Winsock2 Documentation**: Essential for understanding Windows socket programming. [Winsock2 Documentation](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/).
- **C Standard Library**: For a comprehensive overview of available functions and their uses.

This README provides a snapshot of the project's current state, dependencies, and how to run the applications. Future updates will include more detailed instructions on using the system and additional features as they are developed.
