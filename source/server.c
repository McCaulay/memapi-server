#include "defines.h"
#include "ps4.h"
#include "networking.h"
#include "client.h"

int listenPort = 9023;

void *server(void* args)
{
	if (DEBUG)
		networkSendDebugMessage("	[+] Server thread created...\n");

	// Listen on port
	int serverSocket = networkListen("server", listenPort);
	if (DEBUG)
		networkSendDebugMessage("	[+] Server listening on port %d...\n", listenPort);
	
	// Keep accepting client connections
	while (true)
	{
		if (DEBUG)
			networkSendDebugMessage("	[+] Server waiting for incoming connection...\n");

		struct sockaddr_in clientAddress;

		// Accept a client connection
		int clientSocket = networkAccept(serverSocket, (struct sockaddr *)&clientAddress);

		// Get connected client IP address
		char ip[INET_ADDRSTRLEN];
		sceNetInetNtop(AF_INET, &(clientAddress.sin_addr.s_addr), ip, INET_ADDRSTRLEN);

		if (DEBUG)
			networkSendDebugMessage("		[%s] Connection accepted...\n", ip);

		// Build data to send to thread
		struct clientArgs *clientParams = (struct clientArgs*)malloc(sizeof(struct clientArgs));
		clientParams->ip = ip;
		clientParams->socket = clientSocket;

		// Handle client on a thread
		ScePthread thread;
		scePthreadCreate(&thread, NULL, client, (void*)clientParams, "memapiClient");
	}

	// Close Server Socket
	networkCloseConnection(serverSocket);
	return NULL;
}