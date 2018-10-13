#pragma once

#include "memory.h"
#include "sysctl.h"
#include "client.h"

#define NO_TYPE 0
#define POKE 1
#define PEEK 2
#define GET_HWID 3
#define GET_PROCESSES 4
#define GET_MODULES 5
#define GET_REGIONS 6
#define GET_FIRMWARE 7
#define GET_PSID 8
#define GET_IDPS 9
#define SEARCH_START 10
#define SEARCH_RESCAN 11
#define SEARCH_GET_RESULTS 12
#define SEARCH_COUNT_RESULTS 13
#define SEARCH_END 14
#define ATTACH 15
#define DETACH 16
#define NOTIFY 17

#define NO_ERROR 0
#define INVALID_PARAMETER 1
#define INVALID_ADDRESS 2
#define CTL_ERROR 3
#define SEARCH_NOT_STARTED 4
#define FAILED_ATTACH 5
#define NOT_CONNECTED 6
#define NO_DATA 7
#define INVALID_RESPONSE 8
#define INVALID_CALL_TYPE 9
#define REQUEST_FAILED 10

/*
 * Function:  handleRpc
 * --------------------
 * Handle the incoming request then call the appropriate function.
 *
 *	client:		The client arguments (IP, Socket...).
 *	buffer:	The received buffer.
 *	length:	The length of the received data.
 *
 *  returns: void
 */
void handleRpc(struct clientArgs* client, uint8_t* buffer, uint32_t length);