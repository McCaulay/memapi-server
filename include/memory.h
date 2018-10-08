#pragma once

struct inputPeek;

/*
 * Function:  peek
 * --------------------
 * Peek data from memory
 *
 *	ip:				The client IP.
 *	outputBuffer:	The buffer to put the peeked data into.
 *	outputLength:	The length variable to put the length of the peeked data into.
 *	inputBuffer:	The input buffer containing the arguments. (Process, Address, length)
 *	inputLength:	The length of the input buffer.
 *
 *  returns: RPC Error code
 */
int peek(char* ip, unsigned char** outputBuffer, int* outputLength, unsigned char* inputBuffer, int inputLength);
