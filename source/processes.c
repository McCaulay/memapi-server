#include "defines.h"
#include "ps4.h"
#include "modules.h"
#include "networking.h"
#include "sysctl.h"
#include "rpc.h"

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

		networkSendDebugMessage("			[%s@getProcesses] Process - Id: %d, Name: %s, Thread: %s\n", ip, pid, name, thread);
	}

	return NO_ERROR;
}