#include "defines.h"
#include "ps4.h"
#include "modules.h"
#include "networking.h"
#include "sysctl.h"
#include "rpc.h"

struct inputAttach {
	uint32_t processId;
} __attribute__((packed));

struct inputDetach {
	uint32_t processId;
} __attribute__((packed));

size_t getMaxProcesses(struct clientArgs* client)
{
	// sysctl kern.maxproc not working since 1.76...
	return 1000;

	size_t max = 0;
	int mib[2] = { CTL_KERN, KERN_MAXPROC };
	size_t len = sizeof(max);

	if(sysctl(mib, 2, (void*)&max, &len, NULL, 0) != -1)
	{
		#ifdef DEBUG
			networkSendDebugMessage("			[%s@getMaxProcesses] Max Processes:  %d\n", client->ip, max);
		#endif
		return max;
	}
	else
	{
		#ifdef DEBUG
			networkSendDebugMessage("			[%s@getMaxProcesses] Error %d:  %s\n", client->ip, errno, strerror(errno));
		#endif
		return 0;
	}
}

uint8_t getProcesses(struct clientArgs* client, uint8_t** buffer, uint32_t* length)
{
	size_t max = getMaxProcesses(client);

	int32_t mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, 0 };
	size_t len;

	// Calculate buffer size
	for (int32_t pid = 0; pid < max; pid++)
	{
		// Get data length
		mib[3] = pid;
		if (sysctl(mib, 4, NULL, &len, NULL, 0) == -1)
			continue;

		// Allocate memory and retrieve data
		void* dump = malloc(len);
		if (sysctl(mib, 4, dump, &len, NULL, 0) == -1 || len <= 0)
		{
			free(dump);
			continue;
		}

		char* name = dump + 0x1bf;
		char* thread = dump + 0x18a;

		*length += sizeof(int32_t);
		*length += strlen(name) + 1;
		*length += strlen(thread) + 1;

		free(dump);
	}

	//Re-allocate buffer
	*buffer = realloc(*buffer, *length);
	*length = 0;

	for (int32_t pid = 0; pid < max; pid++)
	{
		// Get data length
		mib[3] = pid;
		if (sysctl(mib, 4, NULL, &len, NULL, 0) == -1)
			continue;

		// Allocate memory and retrieve data
		void* dump = malloc(len);
		if (sysctl(mib, 4, dump, &len, NULL, 0) == -1 || len <= 0)
		{
			free(dump);
			continue;
		}

		char* name = dump + 0x1bf;
		char* thread = dump + 0x18a;

		*(int32_t*)(*buffer + *length) = pid;
		*length += sizeof(int32_t);

		sprintf((char*)(*buffer + *length), "%s\0", name);
		*length += strlen(name) + 1;

		sprintf((char*)(*buffer + *length), "%s\0", thread);
		*length += strlen(thread) + 1;

		#ifdef DEBUG
			networkSendDebugMessage("			[%s@getProcesses] Process - Id: %d, Name: %s, Thread: %s\n", client->ip, pid, name, thread);
		#endif

		free(dump);
	}

	return NO_ERROR;
}

uint8_t attach(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputAttach input = *(struct inputAttach*)(inputBuffer + 1);

	// Attach to process
    int result = ptrace(PT_ATTACH, input.processId, NULL, NULL);
	wait(NULL);

	// Failed to attach
	if (result != 0)
	{
		#ifdef DEBUG
			// TODO: Output process name as well as process id. Reuse processes.c code to gather this information.
			networkSendDebugMessage("			[%s@attach] Failed to attach to process %d\n", client->ip, input.processId);
			networkSendDebugMessage("			[%s@attach] Error %d: %s\n", client->ip, errno, strerror(errno));
		#endif
		return FAILED_ATTACH;
	}

	// Attached
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@attach] Attached to process: %d\n", client->ip, input.processId);
	#endif

	return NO_ERROR;
}

uint8_t detach(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputDetach input = *(struct inputDetach*)(inputBuffer + 1);

	// Detach from process
	ptrace(PT_DETACH, input.processId, NULL, NULL);

	#ifdef DEBUG
		networkSendDebugMessage("			[%s@peek] Detached from process: %d\n", client->ip, input.processId);
	#endif

	return NO_ERROR;
}