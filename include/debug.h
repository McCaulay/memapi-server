#pragma once

#include "client.h"

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define FLAG_CARRY 0
#define FLAG_PARITY 2
#define FLAG_AUX_CARRY 4
#define FLAG_ZERO 6
#define FLAG_SIGN 7
#define FLAG_TRACE 8
#define FLAG_INTERRUPT 9
#define FLAG_DIRECTION 10
#define FLAG_OVERFLOW 11

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

/*
 * Function:  debugStep
 * --------------------
 * Step one instruction for a process.
 *
 *  client:       	The client data.
 *	inputBuffer:	The input buffer containing the arguments. (ProcessId)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t debugStep(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  debugGetRegisters
 * --------------------
 * Get the process registers.
 *
 *  client:       	The client data.
 *	outputBuffer:	The buffer to put the registers into.
 *	outputLength:	The length variable to put the length of the registers into.
 *	inputBuffer:	The input buffer containing the arguments. (ProcessId)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t debugGetRegisters(struct clientArgs* client, uint8_t** outputBuffer, uint32_t* outputLength, uint8_t* inputBuffer, uint32_t inputLength);
