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
 * Struct:  inputDebugSetRegisters
 * --------------------
 *
 *	processId:	4 Bytes   | The process id.
 *	registers:	120 Bytes | The registers.
 */
struct inputDebugSetRegisters;

/*
 * Struct:  inputDebugSetDebugRegisters
 * --------------------
 *
 *	processId:	4 Bytes   | The process id.
 *	registers:	128 Bytes | The registers.
 */
struct inputDebugSetDebugRegisters;

/*
 * Struct:  inputDebugAddWatchpoint
 * --------------------
 *
 *	processId:		4 Bytes | The process id.
 *	registerIndex:	4 Bytes | The register to use. (0 - 3)
 *	address:		8 Bytes | The memory address to watch.
 *	length:			4 Bytes | The number of bytes to watch.
 *	type:			4 Bytes | The watchpoint type (DBREG_DR7_EXEC 0x00, DBREG_DR7_WRONLY 0x01, DBREG_DR7_RDWR 0x03)
 */
struct inputDebugAddWatchpoint;
/*
 * Struct:  inputDebugRemoveWatchpoint
 * --------------------
 *
 *	processId:		4 Bytes | The process id.
 *	registerIndex:	4 Bytes | The register to use. (0 - 3)
 */
struct inputDebugRemoveWatchpoint;

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
 * Get the processes registers.
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

/*
 * Function:  debugSetRegisters
 * --------------------
 * Set the processes registers.
 *
 *  client:       	The client data.
 *	inputBuffer:	The input buffer containing the arguments. (ProcessId, Registers)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t debugSetRegisters(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  debugGetDebugRegisters
 * --------------------
 * Get the processes debug registers.
 *
 *  client:       	The client data.
 *	outputBuffer:	The buffer to put the debug registers into.
 *	outputLength:	The length variable to put the length of the debug registers into.
 *	inputBuffer:	The input buffer containing the arguments. (ProcessId)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t debugGetDebugRegisters(struct clientArgs* client, uint8_t** outputBuffer, uint32_t* outputLength, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  debugSetDebugRegisters
 * --------------------
 * Set the processes debug registers.
 *
 *  client:       	The client data.
 *	inputBuffer:	The input buffer containing the arguments. (ProcessId, Registers)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t debugSetDebugRegisters(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  debugAddWatchpoint
 * --------------------
 * Add a watchpoint to the process.
 *
 *  client:       	The client data.
 *	inputBuffer:	The input buffer containing the arguments. (ProcessId, RegisterIndex, Address, Length, Type)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t debugAddWatchpoint(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  debugRemoveWatchpoint
 * --------------------
 * Remove a watchpoint from the process.
 *
 *  client:       	The client data.
 *	inputBuffer:	The input buffer containing the arguments. (ProcessId, RegisterIndex)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t debugRemoveWatchpoint(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  debugStopThread
 * --------------------
 * Stop a given thread from executing.
 *
 *  client:       	The client data.
 *	inputBuffer:	The input buffer containing the arguments. (ProcessId)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t debugStopThread(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  debugResumeThread
 * --------------------
 * Resume a given stopped thread.
 *
 *  client:       	The client data.
 *	inputBuffer:	The input buffer containing the arguments. (ProcessId)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t debugResumeThread(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  debugCheckInterrupt
 * --------------------
 * Check if an interrupt has occured.
 *
 *  client:       	The client data.
 *	outputBuffer:	The buffer to put the interrupt information into.
 *	outputLength:	The length variable to put the length of the interrupt information into.
 *	inputBuffer:	The input buffer containing the arguments. (ProcessId)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t debugCheckInterrupt(struct clientArgs* client, uint8_t** outputBuffer, uint32_t* outputLength, uint8_t* inputBuffer, uint32_t inputLength);