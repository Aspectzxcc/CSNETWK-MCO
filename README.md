# CSNETWK-MCO File Exchange System

## Overview

The CSNETWK-MCO File Exchange System is designed as the final project for our CSNETWK course. It facilitates the storage, sharing, and retrieval of files among clients through a single server, utilizing either TCP or UDP protocols. The system is divided into two main applications: the server application and the client application.

## Current State of the Project

- The project has been successfully completed using the C programming language, fulfilling all specifications outlined in the CSNETWK - Machine Project Demo Kit T3 AY2023-2024 document. It includes all required functionalities and introduces a bonus GUI application that supports both broadcast and unicast messaging features.
- For communication, the client application establishes a stateful connection using a TCP socket, while a separate thread manages a UDP socket to asynchronously receive broadcast and unicast messages.

## Project Structure

This project is organized into several directories to separate the concerns of client, server, and utility functions:

- `src/`: Contains the source code for the project.
  - `client/`: Contains the client application code, including a GUI and command-line interface.
  - `server/`: Contains the server application code.
  - `utils/`: Includes utility functions and common code used by both the client (CLI and GUI) and server applications, such as file handling and network utilities.
- `headers/`: Includes header files for all of the source files used to provide `#define` macros and variable and function prototypes.
- `.gitignore`: Specifies intentionally untracked files to ignore in Git.

Dependencies include:

- **Winsock2 Library**: For socket programming on Windows.
- **C Standard Library**: Utilized for basic input/output operations, string manipulation, and memory management.

#### Development Tools

- **Visual Studio Code**: Editor used for code editing and version control.
- **GCC Compiler**: For compiling the C code. [MinGW](https://sourceforge.net/projects/mingw/).

### Running the Applications

To set up the applications, open multiple terminals - one for the server and additional ones for each client (For the GUI, only one terminal is needed to set up multiple client GUI applications). Navigate to the server directory in one terminal and to the client directory in others. Run the designated batch files for compilation and execution in each terminal individually:

- For the server: Execute `compile_server.bat` to compile and then run the server application.
- For each client: Execute `compile_client.bat` to compile and then run the client application.
- For each client GUI: Execute `compile_and_run_gui.bat` to both compile and run the GUI application.

## Special Libraries and Tools Used

- **Winsock2**: This project makes extensive use of the Winsock2 library for network communication between the client and server applications. This library is essential for socket programming on Windows platforms.
- **Windows API**: The client GUI is built using the Windows API, allowing for a native application experience.
- **Common Controls (commctrl.h)**: Utilized for incorporating advanced user interface elements in the Windows GUI, such as buttons, toolbars, and status bars. This library is part of the Windows API that provides a set of functions to create and manage control windows.
- **Rich Edit Control (richedit.h)**: This library is used to embed rich text editing capabilities within the GUI application. It allows for the display and editing of text with various formatting options, such as different fonts, colors, and sizes, enhancing the user interface for message composition and display.

## Resources

- **Winsock2 Documentation**: Essential for understanding Windows socket programming. [Winsock2 Documentation](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/).
- **C Standard Library**: For a comprehensive overview of available functions and their uses.
- **Common Controls Documentation**: Provides information on using common controls to enhance the application's GUI. [Common Controls](https://learn.microsoft.com/en-us/windows/win32/controls/common-controls-intro).
- **Rich Edit Control Documentation**: Offers guidance on integrating rich text editing features. [Rich Edit Controls](https://learn.microsoft.com/en-us/windows/win32/controls/rich-edit-controls).

This README provides a snapshot of the completed project, including its dependencies, and instructions on how to run the applications. The project features both a command-line interface (CLI) and a graphical user interface (GUI) for the client, as well as comprehensive server functionality.
