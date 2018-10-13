#include "defines.h"
#include "ps4.h"
#include "networking.h"
#include "server.h"
#include "escalate.h"
#include "notify.h"

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
	#ifdef DEBUG
		networkOpenDebugConnection(DEBUG_IP, DEBUG_PORT);
	#endif

	// Escalate privileges to root
	#ifdef DEBUG
		networkSendDebugMessage("[+] Escalating privileges to root...\n");
	#endif
	kexec(&getRoot, NULL);

	// Initialise and resolve libraries
	initSysUtil();

	// Welcome Notification
	notify("MEMAPI By Ludicrous Beach");

	// Starting server message
	#ifdef DEBUG
		networkSendDebugMessage("[+] Server starting...\n[+] Process id: %d\n", syscall(20));
	#endif

	ScePthread thread;
	scePthreadCreate(&thread, NULL, server, NULL, "memapiServer");

	// Wait for thread to finish
	scePthreadJoin(thread, NULL);

	// Good Bye Message
	#ifdef DEBUG
		networkSendDebugMessage("[+] Server exiting...\n");
	#endif

	// Close Debug Connection
	#ifdef DEBUG
		networkCloseDebugConnection();
	#endif

	return 0;
}