#include "defines.h"
#include "ps4.h"
#include "networking.h"
#include "sysctl.h"
#include "rpc.h"

uint8_t getFirmware(char* ip, uint8_t** firmware, uint32_t* length)
{
	size_t len = 0;
	int32_t mib[2] = { CTL_KERN, SDK_VERSION };
	if (sysctl(mib, 2, NULL, &len, NULL, 0) != 0)
		return CTL_ERROR;

	char* version = malloc(len);

	if (sysctl(mib, 2, version, &len, NULL, 0) != 0)
		return CTL_ERROR;

	// Re-allocate memory
	*firmware = realloc(*firmware, 7);

	// Format Version
	sprintf((char*)(*firmware), "%d.%02d", version[2], version[3]);

	*length = strlen((char*)(*firmware));
	
	if (DEBUG)
		networkSendDebugMessage("			[%s@getFirmware] Firmware: %s\n", ip, (char*)(*firmware));

	free(version);
	version = NULL;

	return NO_ERROR;
}
