#include "defines.h"
#include "ps4.h"
#include "networking.h"
#include "notify.h"
#include "rpc.h"

struct inputNotify {
	uint32_t messageTypeId;
	uint32_t messageLength;
} __attribute__((packed));

uint8_t notification(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputNotify input = *(struct inputNotify*)(inputBuffer + 1);

	// Null terminate the message
	char* message = malloc(input.messageLength + 1);
	memcpy(message, &inputBuffer[1 + sizeof(struct inputNotify)], input.messageLength);
	message[input.messageLength] = '\0';

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@notify] Message Type Id: %d, Length: %d, Message: %s\n", client->ip, input.messageTypeId, input.messageLength, message);
	#endif

	sceSysUtilSendSystemNotificationWithText(input.messageTypeId, message);

	// Cleanup allocated memory
	free(message);

	return NO_ERROR;
}

void notify(char* message)
{
	sceSysUtilSendSystemNotificationWithText(222, message);	
}