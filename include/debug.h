#pragma once

#include "client.h"

/*
 * Struct:  inputDebug
 * --------------------
 *
 *	processId:	4 Bytes | The process id.
 */
struct inputDebug;

/*
 * Function:  debugContinue
 * --------------------
 * Continue the process.
 *
 *  client:       	The client data.
 *	inputBuffer:	The input buffer containing the arguments. (ProcessId)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t debugContinue(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  debugStop
 * --------------------
 * Stop the process.
 *
 *  client:       	The client data.
 *	inputBuffer:	The input buffer containing the arguments. (ProcessId)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t debugStop(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  debugKill
 * --------------------
 * Kill the process.
 *
 *  client:       	The client data.
 *	inputBuffer:	The input buffer containing the arguments. (ProcessId)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t debugKill(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);
