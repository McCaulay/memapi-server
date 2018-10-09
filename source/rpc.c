#include "defines.h"
#include "ps4.h"
#include "rpc.h"
#include "networking.h"
#include "modules.h"
#include "processes.h"

void handleRpc(int socket, char* ip, unsigned char* buffer, int length)
{
	int error = NO_ERROR;
	unsigned char* outputBuffer = malloc(1);
	int outputLength = 1;

	int method = (int)buffer[0];
	switch (method)
	{
		case POKE:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call poke() invoked\n", ip);
			// error = poke(ip, buffer, length);
			break;
		case PEEK:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call peek() invoked\n", ip);
			error = peek(ip, &outputBuffer, &outputLength, buffer, length);
			break;
		case GET_HWID:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getHardwareId() invoked\n", ip);
			// error = getHardwareId(ip, &outputBuffer, &outputLength);
			break;
		case GET_FIRMWARE:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getFirmware() invoked\n", ip);
			error = getFirmware(ip, &outputBuffer, &outputLength);
			break;
		case GET_IDPS:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getIdps() invoked\n", ip);
			// error = getIdps(ip, &outputBuffer, &outputLength);
			break;
		case GET_PSID:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getPsid() invoked\n", ip);
			// error = getPsid(ip, &outputBuffer, &outputLength);
			break;
		case GET_PROCESSES:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getProcesses() invoked\n", ip);
			error = getProcesses(ip, &outputBuffer, &outputLength);
			break;
		case GET_MODULES:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getModules() invoked\n", ip);
			error = getModules(ip, &outputBuffer, &outputLength);
			break;
		case GET_REGIONS:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getRegions() invoked\n", ip);
			// error = getRegions(ip, &outputBuffer, &outputLength, buffer, length);
			break;
		case SEARCH_START:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchStart() invoked\n", ip);
			// error = searchStart(ip, &outputBuffer, &outputLength, buffer, length);
			break;
		case SEARCH_RESCAN:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchRescan() invoked\n", ip);
			// error = searchRescan(ip, &outputBuffer, &outputLength, buffer, length);
			break;
		case SEARCH_GET_RESULTS:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchGetResults() invoked\n", ip);
			// error = searchGetResults(ip, &outputBuffer, &outputLength);
			break;
		case SEARCH_COUNT_RESULTS:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchCountResults() invoked\n", ip);
			// error = searchCountResults(ip, &outputBuffer, &outputLength);
			break;
		case SEARCH_END:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchEnd() invoked\n", ip);
			// error = searchEnd();
			break;
		default:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Invalid method call parameter [%d]\n", ip, method);
			error = INVALID_PARAMETER;
			break;
	}

	if (DEBUG && error != NO_ERROR)
		networkSendDebugMessage("		[%s] RPC Error %d\n", ip, error);
	
	// Format response
	unsigned char* outputBufferFull = malloc(outputLength + 1);
	outputBufferFull[0] = error; // First byte is always error byte
	memcpy(&outputBufferFull[1], outputBuffer, outputLength);

	// Send response
	networkSendData(socket, outputBufferFull, outputLength + 1);

	// Free allocated memory
	free(outputBuffer);
	outputBuffer = NULL;
	free(outputBufferFull);
	outputBufferFull = NULL;
}