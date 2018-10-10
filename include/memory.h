#pragma once

/*
 * Struct:  inputPeek
 * --------------------
 *
 *	processId:	4 Bytes | The process id.
 *	address:	8 Bytes | The memory address you want to read.
 *	length:		4 Bytes | The number of bytes you want to read from the address.
 */
struct inputPeek;

/*
 * Struct:  inputPoke
 * --------------------
 *
 *	processId:	4 Bytes | The process id.
 *	address:	8 Bytes | The memory address you want to read.
 *	length:		4 Bytes | The number of bytes you want to read from the address.
 */
struct inputPoke;

/*
 * Function:  peek
 * --------------------
 * Read data from memory
 *
 *	ip:				The client IP.
 *	outputBuffer:	The buffer to put the peeked data into.
 *	outputLength:	The length variable to put the length of the peeked data into.
 *	inputBuffer:	The input buffer containing the arguments. (Process, Address, Length)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
int peek(char* ip, unsigned char** outputBuffer, int* outputLength, unsigned char* inputBuffer, int inputLength);

/*
 * Function:  poke
 * --------------------
 * Write data to memory
 *
 *	ip:				The client IP.
 *	inputBuffer:	The input buffer containing the arguments. (Process, Address, Length, Data)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
int poke(char* ip, unsigned char* inputBuffer, int inputLength);
