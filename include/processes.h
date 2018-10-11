#pragma once

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
uint8_t getProcesses(char* ip, uint8_t** buffer, uint32_t* length);

/*
 * Function:  attach
 * --------------------
 * Attach to a process
 *
 *	ip:				The client IP.
 *	inputBuffer:	The input buffer containing the arguments. (Process)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t attach(char* ip, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  detach
 * --------------------
 * Detach from a process
 *
 *	ip:				The client IP.
 *	inputBuffer:	The input buffer containing the arguments. (Process)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t detach(char* ip, uint8_t* inputBuffer, uint32_t inputLength);