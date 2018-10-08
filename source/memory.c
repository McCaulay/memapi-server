#include "defines.h"
#include "ps4.h"
#include "memory.h"
#include "rpc.h"
#include "networking.h"

struct inputPeek {
	int processId;
	unsigned long address;
	unsigned int length;
} __attribute__((packed));

int peek(char* ip, unsigned char** outputBuffer, int* outputLength, unsigned char* inputBuffer, int inputLength)
{
	//Re-allocate buffer
	*outputBuffer = realloc(*outputBuffer, inputLength);
	*outputLength = inputLength;

	struct inputPeek input = *(struct inputPeek*)(inputBuffer + 1);

	if (DEBUG)
		networkSendDebugMessage("			[%s@peek] Process Id: %d, Address: 0x%08x, Length: %d\n", ip, input.processId, input.address, input.length);

	return NO_ERROR;
}