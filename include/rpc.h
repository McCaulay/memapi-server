#pragma once

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

#define NO_ERROR 0
#define NOT_CONNECTED 1
#define NO_DATA 2
#define FAILED_ATTACH 3
#define INVALID_RESPONSE 4
#define INVALID_CALL_TYPE 5
#define INVALID_PARAMETER 6
#define INVALID_ADDRESS 7
#define CTL_ERROR 8
#define REQUEST_FAILED 9

/*
 * Function:  handleRpc
 * --------------------
 * Handle the incoming request then call the appropriate function.
 *
 *	socket:	The client socket.
 *	ip:		The client IP.
 *	buffer:	The received buffer.
 *	length:	The length of the received data.
 *
 *  returns: void
 */
void handleRpc(int socket, char* ip, char* buffer, int length);