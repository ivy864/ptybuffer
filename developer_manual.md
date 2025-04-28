# Developer Manual

## Contributing
If you would like to contribute code to PTYBuffer, feel free to open a pull request on github.


## Project Structure
The code of PTYBuffer is written entirely in C, relying on the C standard library, along with the Linux and POSIX APIs. The code is split into the client code, the server code, 'util' code, and the main program entrypint.

### ptyb.c
The ptyb.c file contains the main function and handles argument parsing. 

### Client
The client code is split into the client.c and connect.c files. connect.c contains functions for connecting and sending messages to the server over the socket. client.c contains the main client code. 

The entry-point for the client is the `init_client(char *)` function, which takes in a socket domain as an argument. This function first ensures that the server is running and initializes the pseudoterminal. It then forks, calling `init_shell(int)` and `client_main(int)`.

`client_main(int)` is responsible for the client's main loop. This function drives the master end of the pseudoterminal, and sends output to the server via the socket. 

`init_shell(int)` performs some initialization, then executes a shell. 

### Server
The server code is split into server.c and buffer.c. buffer.c contains code relating to the buffer, while server.c contains the main server loop. The server's main loop reads from the socket and either stores data written from the client or performs some other operation corresponding to a message received.

When data is available to be read, the server will first read 4 bytes from the socket. This either corresponds to a client ID or -1 if a message has been sent. In the former case, the server will then read the text written to the socket and save it to the buffer. In the latter case, the server will read 2 more 32 bit integers, corresponding to the specific message and the relevant client ID.

### utils.c
utils.c contains utilities needed for both the client and the server. utils.h provides a 'PtybServer' struct, along with functions for managing it. 
