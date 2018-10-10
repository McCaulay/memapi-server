#include "defines.h"
#include "ps4.h"
#include "modules.h"
#include "networking.h"
#include "sysctl.h"
#include "rpc.h"

struct inputAttach {
	int processId;
} __attribute__((packed));

struct inputDetach {
	int processId;
} __attribute__((packed));

int getMaxProcesses(char* ip)
{
	// sysctl kern.maxproc not working since 1.76...
	return 1000;

	int max = 0;
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

int getProcesses(char* ip, unsigned char** buffer, int* length)
{
	//Re-allocate buffer
	*buffer = realloc(*buffer, 16384);
	*length = 0;

	int max = getMaxProcesses(ip);

	int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, 0 };
	size_t len;

	for (int pid = 0; pid < max; pid++)
	{
		// Get data length
		mib[3] = pid;
		if (sysctl(mib, 4, NULL, &len, NULL, 0) == -1)
			continue;

		// Allocate memory and retrieve data
		void* dump = malloc(len);
		if (sysctl(mib, 4, dump, &len, NULL, 0) == -1)
		{
			free(dump);
			continue;
		}

		// Skip if no data
		if (len <= 0)
			continue;

		char* name = dump + 0x1bf;
		char* thread = dump + 0x18a;

		*(int*)(*buffer + *length) = pid;
		*length += sizeof(int);

		sprintf((char*)(*buffer + *length), "%s\0", name);
		*length += strlen(name) + 1;

		sprintf((char*)(*buffer + *length), "%s\0", thread);
		*length += strlen(thread) + 1;

		if (DEBUG)
			networkSendDebugMessage("			[%s@getProcesses] Process - Id: %d, Name: %s, Thread: %s\n", ip, pid, name, thread);
	}

	return NO_ERROR;
}

int attach(char* ip, unsigned char* inputBuffer, int inputLength)
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

int detach(char* ip, unsigned char* inputBuffer, int inputLength)
{
	struct inputDetach input = *(struct inputDetach*)(inputBuffer + 1);

	// Detach from process
	ptrace(PT_DETACH, input.processId, NULL, NULL);

	if (DEBUG)
		networkSendDebugMessage("			[%s@peek] Detached from process: %d\n", ip, input.processId);

	return NO_ERROR;
}