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

	int readSize = 0;
	unsigned char buffer[512];
	while ((readSize = networkReceiveData((int)socket, buffer, 512)) > 0)
	{
		// Handle buffer
		handleRpc(socket, ip, buffer, readSize);
		memset(buffer, 0, 512);
	}

	networkCloseConnection(socket);
	return NULL;
}