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
	struct inputPeek input = *(struct inputPeek*)(inputBuffer + 1);

	// Debug Information
	if (DEBUG)
		networkSendDebugMessage("			[%s@peek] Process Id: %d, Address: 0x%08x, Length: %d\n", ip, input.processId, input.address, input.length);

	// Reallocate memory buffer
	*outputBuffer = realloc(*outputBuffer, input.length);

	// Set output length
	*outputLength = input.length;

	// Attach to process
    int result = ptrace(PT_ATTACH, input.processId, NULL, NULL);
	wait(NULL);

	if (result != 0)
	{
		if (DEBUG)
		{
			// TODO: Output process name as well as process id. Reuse processes.c code to gather this information.
			networkSendDebugMessage("			[%s@peek] Failed to attach to process %d\n", ip, input.processId);
			networkSendDebugMessage("			[%s@peek] Error %d: %s\n", ip, errno, strerror(errno));
		}
		return FAILED_ATTACH;
	}
	if (DEBUG)
		networkSendDebugMessage("			[%s@peek] Attached to process %d\n", ip, input.processId);

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
		{
			msgAppend += sprintf(msgAppend, "...");
		}

		// New line
		msgAppend += sprintf(msgAppend, "\n");

		networkSendDebugMessage(msg);

		// Free Message
		free(msg);
	}

	// Detach from process
	ptrace(PT_DETACH, input.processId, NULL, NULL);

	if (DEBUG)
		networkSendDebugMessage("			[%s@peek] Detached from process %d\n", ip, input.processId);

	return NO_ERROR;
}