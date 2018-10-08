#include "defines.h"
#include "ps4.h"
#include "modules.h"
#include "networking.h"
#include "sysctl.h"
#include "rpc.h"

int getModules(char* ip, unsigned char** buffer, int* length)
{
	//Re-allocate buffer
	*buffer = realloc(*buffer, 16384);

	*length = 0;

	int modules[MAX_MODULES];
	int moduleCount = 0;

	getLoadedModules(&modules[0], MAX_MODULES, &moduleCount);

	if (DEBUG)
		networkSendDebugMessage("			[%s@getModules] Found %d modules\n", ip, moduleCount);

	for (int i = 0; i < moduleCount; i++)
	{
		struct moduleInfo info;
		getModuleInfo(modules[i], &info);

		*(int*)(*buffer + *length) = modules[i];
		*length += sizeof(int);

		sprintf((char*)(*buffer + *length), "%s\0", info.name);
		*length += strlen(info.name) + 1;

		*(long*)(*buffer + *length) = info.codeSize;
		*length += sizeof(long);

		*(long*)(*buffer + *length) = info.dataSize;
		*length += sizeof(long);

		if (DEBUG)
			networkSendDebugMessage("			[%s@getModules] Module - Id: %d, Name: %s, Code Size: 0x%x, Data Size: 0x%x\n", ip, modules[i], info.name, info.codeSize, info.dataSize);
	}

	return NO_ERROR;
}