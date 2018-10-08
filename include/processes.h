#pragma once

/*
 * Function:  getMaxProcesses
 * --------------------
 * Get the maximum number of processes
 *
 *	ip:			The client IP.
 *
 *  returns: The maximum number of processes
 */
int getMaxProcesses(char* ip);


/*
 * Function:  getProcesses
 * --------------------
 * Get the running processes
 *
 *	ip:			The client IP.
 *	firmware:	The buffer to put the buffer into.
 *	length:		The length variable to put the length of the buffer into.
 *
 *  returns: RPC Error code
 */
int getProcesses(char* ip, unsigned char** buffer, int* length);
