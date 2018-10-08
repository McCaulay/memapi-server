#include "defines.h"
#include "ps4.h"
#include "networking.h"
#include "server.h"

/*
 * Function:  _main
 * --------------------
 * Application entry point.
 *
 *  returns: Error status.
 */
int _main(void) {

	// Initialise and resolve libraries
	initKernel();
	initLibc();
	initNetwork();
	initPthread();
	initModule();

	// Open Debug Connection
	if (DEBUG)
	{
		networkOpenDebugConnection("192.168.0.38", 9023);
		networkSendDebugMessage("[+] Server starting...\n[+] Process id: %d\n", syscall(20));
	}

	ScePthread thread;
	scePthreadCreate(&thread, NULL, server, NULL, "memapiServer");

	// Wait for thread to finish
	scePthreadJoin(thread, NULL);

	// Good Bye Message
	if (DEBUG)
		networkSendDebugMessage("[+] Server exiting...\n");

	// Close Debug Connection
	if (DEBUG)
		networkCloseDebugConnection();

	return 0;
}