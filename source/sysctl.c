#include "defines.h"
#include "ps4.h"
#include "sysctl.h"
#include "rpc.h"

int getFirmware(unsigned char* firmware, int* length)
{
	size_t len = 0;
	int mib[2];

	mib[0] = CTL_KERN;
	mib[1] = SDK_VERSION;
	if (sysctl(mib, 2, NULL, &len, NULL, 0) != 0)
		return CTL_ERROR;

	char* version = malloc(len);

	if (sysctl(mib, 2, version, &len, NULL, 0) != 0)
		return CTL_ERROR;

	// Re-allocate memory
	free(firmware);
	firmware = malloc(7);

	// Format Version
	sprintf((char*)firmware, "%d.%02d", version[2], version[3]);

	*length = strlen((char*)firmware);

	free(version);

	return NO_ERROR;
}
