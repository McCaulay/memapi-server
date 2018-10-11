#include "defines.h"
#include "ps4.h"
#include "client.h"
#include "rpc.h"
#include "networking.h"

void *client(void* args)
{
	struct clientArgs* cArgs = (struct clientArgs*)args;

	#ifdef DEBUG
		networkSendDebugMessage("		[%s] Thread created\n", cArgs->ip);
	#endif

	uint32_t readSize = 0;
	uint8_t buffer[512];
	while ((readSize = networkReceiveData(cArgs->socket, buffer, 512)) > 0)
	{
		// Handle buffer
		handleRpc(cArgs, buffer, readSize);
		memset(buffer, 0, 512);
	}

	#ifdef DEBUG
		networkSendDebugMessage("		[%s] Connection disconnected\n", cArgs->ip);
	#endif

	networkCloseConnection(cArgs->socket);
	free(args);
	return NULL;
}