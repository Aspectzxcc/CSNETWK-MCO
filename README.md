# CSNETWK-MCO
This is the final output for our CSNETWK course which is a File Exchange System, enabling clients to store, share, and fetch files from a single server using either TCP or UDP protocol. The File Exchange System will consist of a server application and a client application.

Project Roadmap
1. Understand the Basics
Learn C Socket Programming: Understand how to create sockets, bind them, listen for connections, accept connections, and send/receive data over TCP/UDP.
Study Client-Server Model: Grasp the basics of how a client-server architecture works, focusing on TCP and UDP protocols.
2. Plan Your Project
Define Project Structure: Decide on the directory structure, naming conventions, and how you'll organize your code (modularize for reusability).
Choose TCP or UDP: Based on the requirements, decide whether to use TCP (reliable, connection-oriented) or UDP (unreliable, connectionless) for your application.
3. Implement Server Application
Socket Initialization: Create a socket using socket().
Bind Socket: Bind the socket to a server address and port using bind().
Listen for Connections (TCP): If using TCP, listen for incoming connections with listen() and accept them with accept().
Handle Client Requests: Implement logic to handle different client requests (/join, /leave, /register, /store, /dir, /get, /?) based on the input command.
Send and Receive Data: Use send() and recv() for TCP or sendto() and recvfrom() for UDP to exchange data with clients.
Concurrency: Consider using fork(), threads (with pthread library), or select/poll for handling multiple clients simultaneously.
File Management: Implement functions to manage file storage, retrieval, and listing.
4. Implement Client Application
User Interface: Design a simple text-based interface for input commands and displaying messages.
Connect to Server: Use connect() for TCP or just sendto() for UDP to communicate with the server.
Input Handling: Parse user input and send appropriate commands to the server.
Receive Server Responses: Display messages from the server based on the responses to commands.
5. Testing and Debugging
Unit Testing: Write tests for individual functions, especially those handling file operations and protocol-specific behaviors.
Integration Testing: Test the client and server applications together to ensure they work as expected.
Debugging: Use tools like gdb for debugging and fixing issues.
6. Documentation and Submission
Code Documentation: Comment your code thoroughly to explain the logic and flow.
User Guide: Write a simple user guide on how to run the server and client applications.
Submission Package: Prepare your submission package as per the project specifications, including a video demonstration.
7. Bonus Features (Optional)
GUI: Consider adding a graphical user interface for the client application using libraries like GTK or Qt for C.
Group Messaging: Implement additional features like group messaging or file sharing among multiple clients.
Resources
C Socket Programming Tutorials: Look into online resources and tutorials to get a deeper understanding of socket programming in C.
C Standard Library Documentation: Familiarize yourself with the functions available in the C standard library, especially those related to sockets, threads, and file handling.
This roadmap provides a structured approach to tackling the project. Adjustments may be necessary as you progress and encounter specific challenges.
