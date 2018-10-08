#include "defines.h"
#include "ps4.h"
#include "networking.h"
#include "server.h"
#include "escalate.h"

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

	// Escalate privileges to root
	if (DEBUG)
		networkSendDebugMessage("[+] Escalating privileges to root...\n");
	kexec(&getRoot, NULL);

	// Open Debug Connection
	if (DEBUG)
	{
		networkOpenDebugConnection(DEBUG_IP, DEBUG_PORT);
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