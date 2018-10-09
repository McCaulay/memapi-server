#pragma once

#define MAX_MODULES 300

/*
 * Function:  getModules
 * --------------------
 * Get the modules
 *
 *	ip:		The client IP.
 *	buffer:	The buffer to put the module data into.
 *	length:	The length variable to put the length of the buffer into.
 *
 *  returns: RPC Error code
 */
int getModules(char* ip, unsigned char** buffer, int* length);