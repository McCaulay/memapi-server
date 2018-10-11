#include "defines.h"
#include "ps4.h"
#include "client.h"
#include "rpc.h"
#include "networking.h"

void *client(void* args)
{
	char* ip = ((struct clientArgs*)args)->ip;
	int socket = ((struct clientArgs*)args)->socket;

	if (DEBUG)
		networkSendDebugMessage("		[%s] Thread created\n", ip);

	uint32_t readSize = 0;
	uint8_t buffer[512];
	while ((readSize = networkReceiveData((int)socket, buffer, 512)) > 0)
	{
		// Handle buffer
		handleRpc(socket, ip, buffer, readSize);
		memset(buffer, 0, 512);
	}

	if (DEBUG)
		networkSendDebugMessage("		[%s] Connection disconnected\n", ip);

	networkCloseConnection(socket);
	return NULL;
}