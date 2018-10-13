#pragma once

#include "client.h"

/*
 * Struct:  inputNotify
 * --------------------
 *
 *	messageTypeId:	4 Bytes | The message type id.
 *	messageLength:	4 Bytes | The length of the message.
 */
struct inputNotify;

/*
 * Function:  notification
 * --------------------
 * Show a system notification from a RPC request.
 *
 *  client:       	The client data.
 *	inputBuffer:	The input buffer containing the arguments. (Message Type Id, Message Length, Message)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
uint8_t notification(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength);

/*
 * Function:  notify
 * --------------------
 * Show a system notification
 *
 *  message: The message to show.
 */
void notify(char* message);
