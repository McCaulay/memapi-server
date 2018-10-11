#pragma once

#include "client.h"

#define MAX_MODULES 300

/*
 * Function:  getModules
 * --------------------
 * Get the modules
 *
 *	client:	The client data.
 *	buffer:	The buffer to put the module data into.
 *	length:	The length variable to put the length of the buffer into.
 *
 *  returns: RPC Error code
 */
uint8_t getModules(struct clientArgs* client, uint8_t** buffer, uint32_t* length);
