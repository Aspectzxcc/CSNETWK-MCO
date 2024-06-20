# CSNETWK-MCO File Exchange System

## Overview

The CSNETWK-MCO File Exchange System is designed as the final project for our CSNETWK course. It facilitates the storage, sharing, and retrieval of files among clients through a single server, utilizing either TCP or UDP protocols. The system is divided into two main applications: the server application and the client application.

## Project Roadmap

### 1. Understand the Basics

- **C Socket Programming:** Learn to create, bind, listen, accept, and manage sockets for data transmission over TCP/UDP.
- **Client-Server Model:** Understand the fundamentals of client-server architecture with a focus on TCP and UDP protocols.

### 2. Plan Your Project

- **Project Structure:** Decide on directory structure, naming conventions, and code organization for modularity and reusability.
- **Protocol Selection:** Choose between TCP (reliable, connection-oriented) and UDP (unreliable, connectionless) based on project requirements.

### 3. Implement Server Application

- **Socket Initialization:** Use `socket()` to create a socket.
- **Bind Socket:** Bind the socket to an address and port with `bind()`.
- **Listen for Connections:** For TCP, use `listen()` and `accept()` to manage incoming connections.
- **Client Request Handling:** Implement logic to process commands like `/join`, `/leave`, `/register`, `/store`, `/dir`, `/get`, and `/?`.
- **Data Transmission:** Utilize `send()` and `recv()` for TCP, or `sendto()` and `recvfrom()` for UDP.
- **Concurrency:** Employ `fork()`, threads (via pthread library), or select/poll for simultaneous client handling.
- **File Management:** Develop functions for file storage, retrieval, and listing.

### 4. Implement Client Application

- **User Interface:** Create a simple text-based interface for command input and message display.
- **Server Connection:** Use `connect()` for TCP or `sendto()` for UDP to interact with the server.
- **Input Handling:** Parse user commands and communicate with the server accordingly.
- **Server Response Handling:** Display server responses based on command outcomes.

### 5. Testing and Debugging

- **Unit Testing:** Test individual functions, focusing on file operations and protocol behaviors.
- **Integration Testing:** Ensure the client and server applications function together as expected.
- **Debugging:** Utilize debugging tools like `gdb` to identify and resolve issues.

### 6. Documentation and Submission

- **Code Documentation:** Thoroughly comment your code to explain logic and flow.
- **User Guide:** Provide a guide on running the server and client applications.
- **Submission Package:** Prepare your project for submission, including a video demonstration.

### 7. Bonus Features (Optional)

- **GUI:** Add a graphical user interface for the client application using GTK or Qt for C.
- **Group Messaging:** Implement group messaging or file sharing among clients.

## Resources

- **C Socket Programming Tutorials:** Explore online resources for a deeper understanding of socket programming in C.
- **C Standard Library Documentation:** Familiarize with the C standard library, especially socket, thread, and file handling functions.

This roadmap outlines a structured approach to developing the File Exchange System. Adjustments may be necessary as the project progresses and new challenges arise.
