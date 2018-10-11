#include "defines.h"
#include "ps4.h"
#include "modules.h"
#include "networking.h"
#include "sysctl.h"
#include "rpc.h"

uint8_t getModules(struct clientArgs* client, uint8_t** buffer, uint32_t* length)
{
	int32_t modules[MAX_MODULES];
	int32_t moduleCount = 0;

	getLoadedModules(&modules[0], MAX_MODULES, &moduleCount);

	if (DEBUG)
		networkSendDebugMessage("			[%s@getModules] Found %d modules\n", client->ip, moduleCount);

	// Calculate buffer size
	for (int i = 0; i < moduleCount; i++)
	{
		struct moduleInfo info;
		getModuleInfo(modules[i], &info);
		*length += sizeof(int32_t);
		*length += strlen(info.name) + 1;
		*length += sizeof(uint64_t);
		*length += sizeof(uint64_t);
	}

	//Re-allocate buffer
	*buffer = realloc(*buffer, *length);
	*length = 0;

	for (int i = 0; i < moduleCount; i++)
	{
		struct moduleInfo info;
		getModuleInfo(modules[i], &info);

		*(int32_t*)(*buffer + *length) = modules[i];
		*length += sizeof(int32_t);

		sprintf((char*)(*buffer + *length), "%s\0", info.name);
		*length += strlen(info.name) + 1;

		*(uint64_t*)(*buffer + *length) = info.codeSize;
		*length += sizeof(uint64_t);

		*(uint64_t*)(*buffer + *length) = info.dataSize;
		*length += sizeof(uint64_t);

		if (DEBUG)
			networkSendDebugMessage("			[%s@getModules] Module - Id: %d, Name: %s, Code Size: 0x%x, Data Size: 0x%x\n", client->ip, modules[i], info.name, info.codeSize, info.dataSize);
	}

	return NO_ERROR;
}