#include "defines.h"
#include "ps4.h"
#include "rpc.h"
#include "networking.h"
#include "modules.h"
#include "processes.h"

void handleRpc(struct clientArgs* client, uint8_t* buffer, uint32_t length)
{
	uint8_t error = NO_ERROR;
	uint8_t* outputBuffer = malloc(sizeof(uint8_t*));
	uint32_t outputLength = 1;

	uint8_t method = buffer[0];
	switch (method)
	{
		/*
		- Does not require attaching to peek/poke.
		case ATTACH:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call attach() invoked\n", client->ip);
			error = attach(client, buffer, length);
			break;
		case DETACH:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call detach() invoked\n", client->ip);
			error = detach(client, buffer, length);
			break;
		*/
		case POKE:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call poke() invoked\n", client->ip);
			error = poke(client, buffer, length);
			break;
		case PEEK:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call peek() invoked\n", client->ip);
			error = peek(client, &outputBuffer, &outputLength, buffer, length);
			break;
		case SEARCH_START:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchStart() invoked\n", client->ip);
			// error = searchStart(client, &outputBuffer, &outputLength, buffer, length);
			break;
		case SEARCH_RESCAN:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchRescan() invoked\n", client->ip);
			// error = searchRescan(client, &outputBuffer, &outputLength, buffer, length);
			break;
		case SEARCH_GET_RESULTS:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchGetResults() invoked\n", client->ip);
			// error = searchGetResults(client, &outputBuffer, &outputLength);
			break;
		case SEARCH_COUNT_RESULTS:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchCountResults() invoked\n", client->ip);
			// error = searchCountResults(client, &outputBuffer, &outputLength);
			break;
		case SEARCH_END:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchEnd() invoked\n", client->ip);
			// error = searchEnd();
			break;
		case GET_HWID:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getHardwareId() invoked\n", client->ip);
			// error = getHardwareId(client, &outputBuffer, &outputLength);
			break;
		case GET_FIRMWARE:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getFirmware() invoked\n", client->ip);
			error = getFirmware(client, &outputBuffer, &outputLength);
			break;
		case GET_IDPS:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getIdps() invoked\n", client->ip);
			// error = getIdps(client, &outputBuffer, &outputLength);
			break;
		case GET_PSID:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getPsid() invoked\n", client->ip);
			// error = getPsid(client, &outputBuffer, &outputLength);
			break;
		case GET_PROCESSES:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getProcesses() invoked\n", client->ip);
			error = getProcesses(client, &outputBuffer, &outputLength);
			break;
		case GET_MODULES:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getModules() invoked\n", client->ip);
			error = getModules(client, &outputBuffer, &outputLength);
			break;
		case GET_REGIONS:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getRegions() invoked\n", client->ip);
			error = getRegions(client, &outputBuffer, &outputLength, buffer, length);
			break;
		default:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Invalid method call parameter [%d]\n", client->ip, method);
			error = INVALID_PARAMETER;
			break;
	}

	if (DEBUG && error != NO_ERROR)
		networkSendDebugMessage("		[%s] RPC Error %d\n", client->ip, error);
	
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