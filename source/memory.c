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

struct inputPoke {
	int processId;
	unsigned long address;
	unsigned int length;
} __attribute__((packed));

int peek(char* ip, unsigned char** outputBuffer, int* outputLength, unsigned char* inputBuffer, int inputLength)
{
	struct inputPeek input = *(struct inputPeek*)(inputBuffer + 1);

	// Debug Information
	if (DEBUG)
		networkSendDebugMessage("			[%s@peek] Process Id: %d, Address: 0x%08x, Length: %d\n", ip, input.processId, input.address, input.length);

	// Reallocate memory buffer
	*outputBuffer = realloc(*outputBuffer, input.length);

	// Set output length
	*outputLength = input.length;

	// Read memory into buffer
	struct ptrace_io_desc ptDesc;
	ptDesc.piod_offs = (void*)(input.address);
	ptDesc.piod_len = input.length;
	ptDesc.piod_addr = *outputBuffer;
	ptDesc.piod_op = PIOD_READ_D;
	ptrace(PT_IO, input.processId, &ptDesc, 0);
	
	if (DEBUG)
	{
		networkSendDebugMessage("			[%s@peek] Read %d bytes from process %d\n", ip, input.length, input.processId);

		int bytesToShow = (input.length > 0x10 ? 0x10 : input.length); // Limit 16

		// Create buffer for message
		char* msg = malloc(19 + 4 + INET_ADDRSTRLEN + (bytesToShow * 3));
		char* msgAppend = msg;

		// Prepend
		msgAppend += sprintf(msgAppend, "			[%s@peek] Output: ", ip);

		// Loop bytes and format
		for (int i = 0; i < bytesToShow; i++)
			msgAppend += sprintf(msgAppend, "%02x ", (*outputBuffer)[i]);

		// Append with ... if can't display all bytes
		if (bytesToShow < input.length)
			msgAppend += sprintf(msgAppend, "...");

		// New line
		msgAppend += sprintf(msgAppend, "\n");

		networkSendDebugMessage(msg);

		// Free Message
		free(msg);
	}

	return NO_ERROR;
}

int poke(char* ip, unsigned char* inputBuffer, int inputLength)
{
	struct inputPoke input = *(struct inputPoke*)(inputBuffer + 1);

	// Debug Information
	if (DEBUG)
		networkSendDebugMessage("			[%s@poke] Process Id: %d, Address: 0x%08x, Length: %d\n", ip, input.processId, input.address, input.length);

	// Write buffer into memory into
	struct ptrace_io_desc ptDesc;
	ptDesc.piod_offs = (void*)(input.address);
	ptDesc.piod_len = input.length;
	ptDesc.piod_addr = &inputBuffer[1 + sizeof(struct inputPoke)];
	ptDesc.piod_op = PIOD_WRITE_D;
	ptrace(PT_IO, input.processId, &ptDesc, 0);

	if (DEBUG)
	{
		networkSendDebugMessage("			[%s@poke] Wrote %d bytes to process %d\n", ip, input.length, input.processId);

		int bytesToShow = (input.length > 0x10 ? 0x10 : input.length); // Limit 16

		// Create buffer for message
		char* msg = malloc(18 + 4 + INET_ADDRSTRLEN + (bytesToShow * 3));
		char* msgAppend = msg;

		// Prepend
		msgAppend += sprintf(msgAppend, "			[%s@poke] Input: ", ip);

		// Loop bytes and format
		for (int i = 0; i < bytesToShow; i++)
			msgAppend += sprintf(msgAppend, "%02x ", inputBuffer[1 + sizeof(struct inputPoke) + i]);

		// Append with ... if can't display all bytes
		if (bytesToShow < input.length)
			msgAppend += sprintf(msgAppend, "...");

		// New line
		msgAppend += sprintf(msgAppend, "\n");

		networkSendDebugMessage(msg);

		// Free Message
		free(msg);
	}
	
	return NO_ERROR;
}