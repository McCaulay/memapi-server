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

size_t getMaxProcesses(char* ip)
{
	// sysctl kern.maxproc not working since 1.76...
	return 1000;

	size_t max = 0;
	int mib[2] = { CTL_KERN, KERN_MAXPROC };
	size_t len = sizeof(max);

	if(sysctl(mib, 2, (void*)&max, &len, NULL, 0) != -1)
	{
		if (DEBUG)
			networkSendDebugMessage("			[%s@getMaxProcesses] Max Processes:  %d\n", ip, max);
		return max;
	}
	else
	{
		if (DEBUG)
			networkSendDebugMessage("			[%s@getMaxProcesses] Error %d:  %s\n", ip, errno, strerror(errno));
		return 0;
	}
}

uint8_t getProcesses(char* ip, uint8_t** buffer, uint32_t* length)
{
	//Re-allocate buffer
	*buffer = realloc(*buffer, 16384);
	*length = 0;

	size_t max = getMaxProcesses(ip);

	int32_t mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, 0 };
	size_t len;

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

		if (DEBUG)
			networkSendDebugMessage("			[%s@getProcesses] Process - Id: %d, Name: %s, Thread: %s\n", ip, pid, name, thread);
	}

	return NO_ERROR;
}

uint8_t attach(char* ip, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputAttach input = *(struct inputAttach*)(inputBuffer + 1);

	// Attach to process
    int result = ptrace(PT_ATTACH, input.processId, NULL, NULL);
	wait(NULL);

	// Failed to attach
	if (result != 0)
	{
		if (DEBUG)
		{
			// TODO: Output process name as well as process id. Reuse processes.c code to gather this information.
			networkSendDebugMessage("			[%s@attach] Failed to attach to process %d\n", ip, input.processId);
			networkSendDebugMessage("			[%s@attach] Error %d: %s\n", ip, errno, strerror(errno));
		}
		return FAILED_ATTACH;
	}

	// Attached
	if (DEBUG)
		networkSendDebugMessage("			[%s@attach] Attached to process: %d\n", ip, input.processId);

	return NO_ERROR;
}

uint8_t detach(char* ip, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputDetach input = *(struct inputDetach*)(inputBuffer + 1);

	// Detach from process
	ptrace(PT_DETACH, input.processId, NULL, NULL);

	if (DEBUG)
		networkSendDebugMessage("			[%s@peek] Detached from process: %d\n", ip, input.processId);

	return NO_ERROR;
}