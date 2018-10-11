#pragma once

#include "client.h"

/*
 * Struct:  inputAttach
 * --------------------
 *
 *	processId:	4 Bytes | The process id.
 */
struct inputAttach;

/*
 * Struct:  inputDetach
 * --------------------
 *
 *	processId:	4 Bytes | The process id.
 */
struct inputDetach;

/*
 * Function:  getMaxProcesses
 * --------------------
 * Get the maximum number of processes
 *
 *	client:	The client data.
 *
 *  returns: The maximum number of processes
 */
int getMaxProcesses(struct clientArgs* client);


/*
 * Function:  getProcesses
 * --------------------
 * Get the running processes
 *
 *	client:		The client data.
 *	firmware:	The buffer to put the buffer into.
 *	length:		The length variable to put the length of the buffer into.
 *
 *  returns: RPC Error code
 */
uint8_t getProcesses(struct clientArgs* client, uint8_t** buffer, uint32_t* length);

/*
 * Function:  attach
 * --------------------
 * Attach to a process
 *
 *	client:			The client data.
 *	inputBuffer:	The input buffer containing the arguments. (Process)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t attach(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  detach
 * --------------------
 * Detach from a process
 *
 *	client:			The client data.
 *	inputBuffer:	The input buffer containing the arguments. (Process)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t detach(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);