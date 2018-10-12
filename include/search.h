#pragma once

#include "client.h"

/*
 * Struct:  inputSearchStart
 * --------------------
 *
 *	processId:		4 Bytes | The process id.
 *	startAddress:	8 Bytes | The memory address you want to start searching from.
 *	endAddress:		8 Bytes | The memory address you want to finish searching at.
 *	length:			4 Bytes | The number of bytes you want to search for.
 */
struct inputSearchStart;

/*
 * Struct:  inputSearchRescan
 * --------------------
 *
 *	processId:		4 Bytes | The process id.
 *	length:			4 Bytes | The number of bytes you want to search for.
 */
struct inputSearchRescan;

/*
 * Struct:  searchResults
 * --------------------
 *
 *	items:	The results.
 *	count:	The number of items.
 *	size:	The total number of items that can be in this array.
 */
struct searchResults;

/*
 * enum:  SearchState
 * --------------------
 *
 *	STARTED:	If the search has been started and results are allocated.
 *	ENDED:		If the search has finished and results have been freed.
 */
enum SearchState;

/*
 * Function:  allocateResults
 * --------------------
 * Allocate the results data set.
 *
 *	sResults:		The results struct to be allocated.
 *	initialSize:	The initial number of elements the array should be allocated for.
 */
void allocateResults(struct searchResults* sResults, size_t initialSize);

/*
 * Function:  addResult
 * --------------------
 * Add a result address to the list of results.
 *
 *	sResults:		The results struct.
 *	result:			The result address to be added to the results.
 */
void addResult(struct searchResults* sResults, uint64_t result);

/*
 * Function:  removeResult
 * --------------------
 * Remove a result from the list of results.
 * Note: You must run cleanResults after this function. (Or after the loop of using this funciton) to fix the array.
 *
 *	sResults:		The results struct.
 *	index:			The index of the results array to be removed.
 */
void removeResult(struct searchResults* sResults, uint32_t index);

/*
 * Function:  cleanResults
 * --------------------
 * Re-calibrate the array by removing NULL values.
 *
 *	sResults:		The results struct.
 */
void cleanResults(struct searchResults* sResults);

/*
 * Function:  freeResults
 * --------------------
 * Free the results allocated resources.
 *
 *	sResults:		The results struct to be freed.
 */
void freeResults(struct searchResults* sResults);

/*
 * Function:  searchStart
 * --------------------
 * Begin searching for data between two given addresses.
 *
 *	client:			The client data.
 *	inputBuffer:	The input buffer containing the arguments. (Process, Start Address, End Address, Length, Data)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t searchStart(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  searchRescan
 * --------------------
 * Rescan the previous search results.
 *
 *	client:			The client data.
 *	inputBuffer:	The input buffer containing the arguments. (Process, Length, Data)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t searchRescan(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  searchGetResults
 * --------------------
 * Get the current list of results.
 *
 *	client:			The client data.
 *	outputBuffer:	The buffer to put the results data into.
 *	outputLength:	The length variable to put the length of the results data into.
 *
 *  returns: RPC Error code
 */
uint8_t searchGetResults(struct clientArgs* client, uint8_t** outputBuffer, uint32_t* outputLength);

/*
 * Function:  searchCountResults
 * --------------------
 * Get the number of results currently in the list.
 *
 *	client:			The client data.
 *	outputBuffer:	The buffer to put the number of results into.
 *	outputLength:	The length variable to put the length of the output buffer into.
 *
 *  returns: RPC Error code
 */
uint8_t searchCountResults(struct clientArgs* client, uint8_t** outputBuffer, uint32_t* outputLength);

/*
 * Function:  searchEnd
 * --------------------
 * End a previously started search.
 *
 *	client:			The client data.
 *
 *  returns: RPC Error code
 */
uint8_t searchEnd(struct clientArgs* client);
