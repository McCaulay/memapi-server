#pragma once

/*
 * Struct:  clientArg
 * --------------------
 * The client arguments to be passed to the client function.
 *
 *	ip:	The clients IP address.
 *	socket:	The client socket.
 */
struct clientArgs {
    char* ip;
    int socket;
};

/*
 * Function:  client
 * --------------------
 * The client thread entry point.
 *
 *	socket:	The client socket.
 *
 *  returns: null
 */
void *client(void* args);