#pragma once

#include "client.h"

/*
 * Function:  getFirmware
 * --------------------
 * Get the firmware version
 *
 *	client:		The client data.
 *	firmware:	The buffer to put the firmware id into.
 *	length:		The length variable to put the length of the firmware id into.
 *
 *  returns: RPC Error code
 */
uint8_t getFirmware(struct clientArgs* client, uint8_t** firmware, uint32_t* length);
