#include "defines.h"
#include "ps4.h"
#include "networking.h"
#include "debug.h"
#include "rpc.h"

struct inputDebug {
	uint32_t processId;
} __attribute__((packed));

uint8_t debugContinue(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputDebug input = *(struct inputDebug*)(inputBuffer + 1);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@debugContinue] Process Id: %d\n", client->ip, input.processId);
	#endif

	ptrace(PT_CONTINUE, input.processId, (void*)1, NULL);

	return NO_ERROR;
}

uint8_t debugStop(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputDebug input = *(struct inputDebug*)(inputBuffer + 1);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@debugStop] Process Id: %d\n", client->ip, input.processId);
	#endif

	ptrace(PT_CONTINUE, input.processId, (void*)1, SIGSTOP);

	return NO_ERROR;
}

uint8_t debugKill(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputDebug input = *(struct inputDebug*)(inputBuffer + 1);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@debugKill] Process Id: %d\n", client->ip, input.processId);
	#endif

	ptrace(PT_KILL, input.processId, (void*)1, 0);

	return NO_ERROR;
}