#pragma once

/*
 * The port the server should listen on.
 */
extern int listenPort;

/*
 * Function:  server
 * --------------------
 * The server thread entry point.
 *
 *	args:	Args that are passed to the server.
 *
 *  returns: null
 */
void *server(void* args);