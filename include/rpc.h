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
#define DEBUG_CONTINUE 18
#define DEBUG_STOP 19
#define DEBUG_KILL 20
#define DEBUG_STEP 21
#define DEBUG_GET_REGISTERS 22
#define DEBUG_SET_REGISTERS 23
#define DEBUG_GET_FLOAT_REGISTERS 24
#define DEBUG_SET_FLOAT_REGISTERS 25
#define DEBUG_GET_DEBUG_REGISTERS 26
#define DEBUG_SET_DEBUG_REGISTERS 27
#define DEBUG_ADD_BREAKPOINT 28
#define DEBUG_REMOVE_BREAKPOINT 29
#define DEBUG_ADD_WATCHPOINT 30
#define DEBUG_REMOVE_WATCHPOINT 31
#define GET_PROCESS_THREADS 32
#define DEBUG_STOP_THREAD 33
#define DEBUG_RESUME_THREAD 34
#define DEBUG_CHECK_INTERRUPT 35

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
#define INVALID_REGISTER_INDEX 11
#define NO_INTERRUPT 12

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