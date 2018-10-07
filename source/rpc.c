#include "defines.h"
#include "ps4.h"
#include "rpc.h"
#include "networking.h"
#include "rpc/sysctl.h"

void handleRpc(int socket, char* ip, char* buffer, int length)
{
	int error = NO_ERROR;
	char* outputBuffer = malloc(1);
	int outputLength = 1;

	int method = (int)buffer[0];
	switch (method)
	{
		case POKE:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call poke() invoked...\n", ip);
			// error = poke(cSocket, clientBuffer, bufferLength);
			break;
		case PEEK:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call peek() invoked...\n", ip);
			// error = peek(cSocket, clientBuffer, bufferLength);
			break;
		case GET_HWID:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getHwid() invoked...\n", ip);
			// error = sendHWID(cSocket);
			break;
		case GET_FIRMWARE:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getFirmware() invoked...\n", ip);
			error = getFirmware(outputBuffer, &outputLength);
			break;
		case GET_IDPS:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getIdps() invoked...\n", ip);
			// error = sendIDPS(cSocket);
			break;
		case GET_PSID:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getPsid() invoked...\n", ip);
			// error = sendPSID(cSocket);
			break;
		case GET_PROCESSES:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getProcesses() invoked...\n", ip);
			// error = sendProcesses(cSocket);
			break;
		case GET_MODULES:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getModules() invoked...\n", ip);
			// error = sendModules(cSocket);
			break;
		case GET_REGIONS:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call getRegions() invoked...\n", ip);
			// error = sendRegions(cSocket, clientBuffer, bufferLength);
			break;
		case SEARCH_START:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchStart() invoked...\n", ip);
			// error = startSearch(cSocket, clientBuffer, bufferLength);
			break;
		case SEARCH_RESCAN:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchRescan() invoked...\n", ip);
			// error = rescanSearch(cSocket, clientBuffer, bufferLength);
			break;
		case SEARCH_GET_RESULTS:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchGetResults() invoked...\n", ip);
			// error = sendSearchResults(cSocket);
			break;
		case SEARCH_COUNT_RESULTS:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchCountResults() invoked...\n", ip);
			// error = sendCountSearchResults(cSocket);
			break;
		case SEARCH_END:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Method call searchEnd() invoked...\n", ip);
			// error = emptySearchResults(cSocket);
			break;
		default:
			if (DEBUG)
				networkSendDebugMessage("		[%s] Invalid method call parameter [%d]...\n", ip, method);
			error = INVALID_PARAMETER;
			break;
	}

	// Format response
	char* outputBufferFull = malloc(outputLength + 1);
	outputBufferFull[0] = error; // First byte is always error byte
	memcpy(&outputBufferFull[1], outputBuffer, outputLength);

	// Send response
	networkSendData(socket, outputBufferFull, outputLength + 1);

	// Free allocated memory
	free(outputBuffer);
	free(outputBufferFull);
}