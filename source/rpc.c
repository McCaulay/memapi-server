#include "defines.h"
#include "ps4.h"
#include "rpc.h"
#include "networking.h"
#include "modules.h"
#include "processes.h"
#include "search.h"
#include "notify.h"
#include "debug.h"

void handleRpc(struct clientArgs* client, uint8_t* buffer, uint32_t length)
{
	uint8_t error = NO_ERROR;
	uint8_t* outputBuffer = malloc(sizeof(uint8_t*));
	uint32_t outputLength = 0;

	uint8_t method = buffer[0];
	switch (method)
	{
		/*
		- Does not require attaching to peek/poke.
		case ATTACH:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call attach() invoked\n", client->ip);
			#endif
			error = attach(client, buffer, length);
			break;
		case DETACH:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call detach() invoked\n", client->ip);
			#endif
			error = detach(client, buffer, length);
			break;
		*/
		case POKE:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call poke() invoked\n", client->ip);
			#endif
			error = poke(client, buffer, length);
			break;
		case PEEK:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call peek() invoked\n", client->ip);
			#endif
			error = peek(client, &outputBuffer, &outputLength, buffer, length);
			break;
		case SEARCH_START:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call searchStart() invoked\n", client->ip);
			#endif
			error = searchStart(client, buffer, length);
			break;
		case SEARCH_RESCAN:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call searchRescan() invoked\n", client->ip);
			#endif
			error = searchRescan(client, buffer, length);
			break;
		case SEARCH_GET_RESULTS:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call searchGetResults() invoked\n", client->ip);
			#endif
			error = searchGetResults(client, &outputBuffer, &outputLength);
			break;
		case SEARCH_COUNT_RESULTS:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call searchCountResults() invoked\n", client->ip);
			#endif
			error = searchCountResults(client, &outputBuffer, &outputLength);
			break;
		case SEARCH_END:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call searchEnd() invoked\n", client->ip);
			#endif
			error = searchEnd(client);
			break;
		case GET_HWID:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call getHardwareId() invoked\n", client->ip);
			#endif
			// error = getHardwareId(client, &outputBuffer, &outputLength);
			break;
		case GET_FIRMWARE:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call getFirmware() invoked\n", client->ip);
			#endif
			error = getFirmware(client, &outputBuffer, &outputLength);
			break;
		case GET_IDPS:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call getIdps() invoked\n", client->ip);
			#endif
			// error = getIdps(client, &outputBuffer, &outputLength);
			break;
		case GET_PSID:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call getPsid() invoked\n", client->ip);
			#endif
			// error = getPsid(client, &outputBuffer, &outputLength);
			break;
		case GET_PROCESSES:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call getProcesses() invoked\n", client->ip);
			#endif
			error = getProcesses(client, &outputBuffer, &outputLength);
			break;
		case GET_MODULES:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call getModules() invoked\n", client->ip);
			#endif
			error = getModules(client, &outputBuffer, &outputLength);
			break;
		case GET_REGIONS:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call getRegions() invoked\n", client->ip);
			#endif
			error = getRegions(client, &outputBuffer, &outputLength, buffer, length);
			break;
		case NOTIFY:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call notify() invoked\n", client->ip);
			#endif
			error = notification(client, buffer, length);
			break;
		case DEBUG_CONTINUE:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugContinue() invoked\n", client->ip);
			#endif
			error = debugContinue(client, buffer, length);
			break;
		case DEBUG_STOP:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugStop() invoked\n", client->ip);
			#endif
			error = debugStop(client, buffer, length);
			break;
		case DEBUG_KILL:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugKill() invoked\n", client->ip);
			#endif
			error = debugKill(client, buffer, length);
			break;
		case DEBUG_STEP:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugStep() invoked\n", client->ip);
			#endif
			error = debugStep(client, buffer, length);
			break;
		case DEBUG_GET_REGISTERS:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugGetRegisters() invoked\n", client->ip);
			#endif
			error = debugGetRegisters(client, &outputBuffer, &outputLength, buffer, length);
			break;
		case DEBUG_SET_REGISTERS:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugSetRegisters() invoked\n", client->ip);
			#endif
			// error = debugSetRegisters(client, buffer, length);
			break;
		case DEBUG_GET_FLOAT_REGISTERS:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugGetFloatRegisters() invoked\n", client->ip);
			#endif
			// error = debugGetFloatRegisters(client, buffer, length);
			break;
		case DEBUG_SET_FLOAT_REGISTERS:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugSetFloatRegisters() invoked\n", client->ip);
			#endif
			// error = debugSetFloatRegisters(client, buffer, length);
			break;
		case DEBUG_GET_DEBUG_REGISTERS:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugGetDebugRegisters() invoked\n", client->ip);
			#endif
			error = debugGetDebugRegisters(client, &outputBuffer, &outputLength, buffer, length);
			break;
		case DEBUG_SET_DEBUG_REGISTERS:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugSetDebugRegisters() invoked\n", client->ip);
			#endif
			// error = debugSetDebugRegisters(client, buffer, length);
			break;
		case DEBUG_ADD_BREAKPOINT:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugAddBreakpoint() invoked\n", client->ip);
			#endif
			// error = debugAddBreakpoint(client, buffer, length);
			break;
		case DEBUG_REMOVE_BREAKPOINT:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugRemoveBreakpoint() invoked\n", client->ip);
			#endif
			// error = debugRemoveBreakpoint(client, buffer, length);
			break;
		case DEBUG_ADD_WATCHPOINT:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugAddWatchpoint() invoked\n", client->ip);
			#endif
			error = debugAddWatchpoint(client, buffer, length);
			break;
		case DEBUG_REMOVE_WATCHPOINT:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Method call debugRemoveWatchpoint() invoked\n", client->ip);
			#endif
			error = debugRemoveWatchpoint(client, buffer, length);
			break;
		default:
			#ifdef DEBUG
				networkSendDebugMessage("		[%s] Invalid method call parameter [%d]\n", client->ip, method);
			#endif
			error = INVALID_PARAMETER;
			break;
	}

	#ifdef DEBUG
		if (error != NO_ERROR)
			networkSendDebugMessage("		[%s] RPC Error %d\n", client->ip, error);
	#endif
	
	// Format response
	uint8_t* outputBufferFull = malloc(outputLength + 1);
	outputBufferFull[0] = error; // First byte is always error byte
	memcpy(&outputBufferFull[1], outputBuffer, outputLength);

	// Send response
	networkSendData(client->socket, outputBufferFull, outputLength + 1);

	// Free allocated memory
	free(outputBuffer);
	outputBuffer = NULL;
	free(outputBufferFull);
	outputBufferFull = NULL;
}