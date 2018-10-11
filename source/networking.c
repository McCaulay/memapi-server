#include "ps4.h"
#include "networking.h"

int debugSocket = NULL;

struct sockaddr_in networkAdress(uint16_t port)
{
	struct sockaddr_in address;
	address.sin_len = sizeof(address);
	address.sin_family = AF_INET;
	address.sin_port = sceNetHtons(port);
	memset(address.sin_zero, 0, sizeof(address.sin_zero));
	return address;
}

int networkListen(const char * name, uint16_t port)
{
	struct sockaddr_in server = networkAdress(port);
	server.sin_addr.s_addr = IN_ADDR_ANY;

	// Create the socket.
	int socket = sceNetSocket("server", AF_INET, SOCK_STREAM, 0);
	if (socket == -1) {
		return -1;
	}
	
	// Bind the socket
	int error = sceNetBind(socket, (struct sockaddr *)&server, sizeof(server));
	if (error == -1) {
		return -1;
	}

	// Listen to the socket
	error = sceNetListen(socket, 10);
	if (error == -1) {
		return -1;
	}

	return socket;
}

int networkAccept(int socket, struct sockaddr *address)
{
	int addressLength = sizeof(struct sockaddr_in);
	return sceNetAccept(socket, address, (socklen_t*)&addressLength);
}

int networkOpenConnection(const char * name, const char *ip, uint16_t port)
{
	struct sockaddr_in server = networkAdress(port);
	sceNetInetPton(AF_INET, ip, &server.sin_addr.s_addr);

	int socket = sceNetSocket(name, AF_INET, SOCK_STREAM, 0);
	sceNetConnect(socket, (struct sockaddr *)&server, sizeof(server));
	return socket;
}

void networkOpenDebugConnection(const char *ip, uint16_t port)
{
	debugSocket = networkOpenConnection("debug", ip, port);
}

int networkReceiveData(int socket, uint8_t* buffer, int32_t size)
{
	return sceNetRecv(socket, buffer, size, 0);
}

int networkSendData(int socket, uint8_t* buffer, int32_t size)
{
	return sceNetSend(socket, buffer, size, 0);
}

int networkSendDebugData(uint8_t* buffer, int32_t size)
{
	return networkSendData(debugSocket, buffer, size);
}

int networkCloseConnection(int socket)
{
	return sceNetSocketClose(socket);
}

int networkCloseDebugConnection()
{
	return networkCloseConnection(debugSocket);
}