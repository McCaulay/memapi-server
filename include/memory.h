#pragma once

#include "client.h"

#define VMMAP_GAP 0x1000000

/*
 * Struct:  inputPeek
 * --------------------
 *
 *	processId:	4 Bytes | The process id.
 *	address:	  8 Bytes | The memory address you want to read.
 *	length:		  4 Bytes | The number of bytes you want to read from the address.
 */
struct inputPeek;

/*
 * Struct:  inputPoke
 * --------------------
 *
 *	processId:	4 Bytes | The process id.
 *	address:	  8 Bytes | The memory address you want to read.
 *	length:		  4 Bytes | The number of bytes you want to read from the address.
 */
struct inputPoke;

/*
 * Struct:  inputRegions
 * --------------------
 *
 *	processId:	4 Bytes | The process id.
 */
struct inputRegions;

/*
 * Function:  peek
 * --------------------
 * Read data from memory for a given process.
 *
 *	client:			The client data.
 *	outputBuffer:	The buffer to put the peeked data into.
 *	outputLength:	The length variable to put the length of the peeked data into.
 *	inputBuffer:	The input buffer containing the arguments. (Process, Address, Length)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t peek(struct clientArgs* client, uint8_t** outputBuffer, uint32_t* outputLength, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  poke
 * --------------------
 * Write data to memory for a given process.
 *
 *  client:       	The client data.
 *	inputBuffer:	The input buffer containing the arguments. (Process, Address, Length, Data)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t poke(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  getRegions
 * --------------------
 * Get the memory regions for a given process.
 *
 *  client:       	The client data.
 *	outputBuffer:	The buffer to put the regions into.
 *	outputLength:	The length variable to put the length of the regions into.
 *	inputBuffer:	The input buffer containing the arguments. (Process)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t getRegions(struct clientArgs* client, uint8_t** outputBuffer, uint32_t* outputLength, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  isAddressRangeValid
 * --------------------
 * Check if the given memory address range is valid for this process.
 *
 *  client:      The client data.
 *	processId:	The process id.
 *	start:		The start address of the range.
 *	end:		The end address of the range.
 *
 *  returns: true/false
 */
bool isAddressRangeValid(struct clientArgs* client, uint32_t processId, uint64_t start, uint64_t end);

/*
 * Function:  getVirtualMemoryMaps
 * --------------------
 * Get the virtual memory regions for a given process.
 * Note: You must free the returned pointer after use.
 *
 *	processId:	The process id.
 *	entries:	The pointer to array of virtual memory maps
 *	length:		The pointer to put the buffer size into.
 *
 *  returns: 0 On success, -1 on error
 */
int32_t getVirtualMemoryMaps(uint32_t processId, kinfo_vmentry** entries, size_t* length);
