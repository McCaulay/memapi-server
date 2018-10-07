#pragma once

/*
 * The network socket to send debug data back to the computer.
 */
extern int debugSocket;

/*
 * Function:  networkAdress
 * --------------------
 * Gets the sockaddr_in struct for the given port. Note this does not set the IP address.
 *
 *	port:	The port number you are communicating over.
 *
 *  returns: socket address
 */
struct sockaddr_in networkAdress(uint16_t port);

/*
 * Function:  networkListen
 * --------------------
 * Listen to incoming connections.
 *
 *  name:	The network socket name.
 *	port:	The port number you are listening on.
 *
 *  returns: socket
 */
int networkListen(const char * name, uint16_t port);

/*
 * Function:  networkAccept
 * --------------------
 * Accept an incoming connection.
 *
 *  socket: 	The network socket the message should be accepted on.
 *	address:	The address pointer that will be filled with the accepted client data.
 *
 *  returns: client socket
 */
int networkAccept(int socket, struct sockaddr *address);

/*
 * Function:  networkOpenConnection
 * --------------------
 * Opens a connection over the network to the socket.
 *
 *  name:	The network socket name.
 *	ip:		The ip address of the device you are connecting to.
 *	port:	The port number you are communicating over.
 *
 *  returns: socket
 */
int networkOpenConnection(const char * name, const char *ip, uint16_t port);

/*
 * Function:  networkOpenDebugConnection
 * --------------------
 * Opens a connection over the network to the debug socket.
 *
 *	ip:		The ip address of the device you are connecting to.
 *	port:	The port number you are communicating over.
 *
 *  returns: void
 */
void networkOpenDebugConnection(const char *ip, uint16_t port);

/*
 * Function:  networkSendMessage
 * --------------------
 * Sends a string message over the network to the socket.
 *
 *  socket: The network socket the message should be sent through.
 *	format: The string to send, which can be formatted the same as printf.
 *	...:	Additional arguments to be formatted, see printf for more information.
 *
 *  returns: void
 */
#define networkSendMessage(socket, format, ...)\
do {\
	char buffer[512];\
	int size = sprintf(buffer, format, ##__VA_ARGS__);\
	sceNetSend(socket, buffer, size, 0);\
} while(0)

/*
 * Function:  networkSendDebugMessage
 * --------------------
 * Sends a string message over the network to the debug socket.
 *
 *	format: The string to send, which can be formatted the same as printf.
 *	...:	Additional arguments to be formatted, see printf for more information.
 *
 *  returns: void
 */
#define networkSendDebugMessage(format, ...)\
do {\
	char buffer[512];\
	int size = sprintf(buffer, format, ##__VA_ARGS__);\
	sceNetSend(debugSocket, buffer, size, 0);\
} while(0)

/*
 * Function:  networkReceiveData
 * --------------------
 * Receive data over the network to the socket.
 *
 *  socket: The network socket the data should be received through.
 *	buffer: The buffer pointer where the received data should be dumped to.
 *	size:	The total size of the buffer in bytes.
 *
 *  returns: Error status
 */
int networkReceiveData(int socket, char* buffer, int size);

/*
 * Function:  networkSendData
 * --------------------
 * Sends data over the network to the socket.
 *
 *  socket: The network socket the data should be sent through.
 *	buffer: The buffer pointer to the data that should be sent.
 *	size:	The number of bytes of the buffer to send.
 *
 *  returns: Error status
 */
int networkSendData(int socket, char* buffer, int size);

/*
 * Function:  networkSendDebugData
 * --------------------
 * Sends data over the network to the debug socket.
 *
 *	buffer: The buffer pointer to the data that should be sent.
 *	size:	The number of bytes of the buffer to send.
 *
 *  returns: Error status
 */
int networkSendDebugData(char* buffer, int size);

/*
 * Function:  networkCloseConnection
 * --------------------
 * Closes the network connection for the given socket.
 *
 *  socket: The network socket that should be closed.
 *
 *  returns: Error status.
 */
int networkCloseConnection(int socket);

/*
 * Function:  networkCloseDebugConnection
 * --------------------
 * Closes the network connection for the debug socket.
 *
 *  returns: Error status.
 */
int networkCloseDebugConnection();