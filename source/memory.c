#include "defines.h"
#include "ps4.h"
#include "memory.h"
#include "rpc.h"
#include "networking.h"

struct inputPeek {
	uint32_t processId;
	uint64_t address;
	uint32_t length;
} __attribute__((packed));

struct inputPoke {
	uint32_t processId;
	uint64_t address;
	uint32_t length;
} __attribute__((packed));

struct inputRegions {
	uint32_t processId;
} __attribute__((packed));

uint8_t peek(struct clientArgs* client, uint8_t** outputBuffer, uint32_t* outputLength, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputPeek input = *(struct inputPeek*)(inputBuffer + 1);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@peek] Process Id: %d, Address: 0x%08x, Length: %d\n", client->ip, input.processId, input.address, input.length);
	#endif

	// Ensure memory address is valid
	if (!isAddressRangeValid(client, input.processId, input.address, input.address + input.length))
		return INVALID_ADDRESS;

	// Reallocate memory buffer
	*outputBuffer = realloc(*outputBuffer, input.length);

	// Set output length
	*outputLength = input.length;

	// Read memory into buffer
	struct ptrace_io_desc ptDesc;
	ptDesc.piod_offs = (void*)(input.address);
	ptDesc.piod_len = input.length;
	ptDesc.piod_addr = *outputBuffer;
	ptDesc.piod_op = PIOD_READ_D;
	ptrace(PT_IO, input.processId, &ptDesc, 0);
	
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@peek] Read %d bytes from process %d\n", client->ip, input.length, input.processId);

		int bytesToShow = (input.length > 0x10 ? 0x10 : input.length); // Limit 16

		// Create buffer for message
		char* msg = malloc(19 + 4 + INET_ADDRSTRLEN + (bytesToShow * 3));
		char* msgAppend = msg;

		// Prepend
		msgAppend += sprintf(msgAppend, "			[%s@peek] Output: ", client->ip);

		// Loop bytes and format
		for (int i = 0; i < bytesToShow; i++)
			msgAppend += sprintf(msgAppend, "%02x ", (*outputBuffer)[i]);

		// Append with ... if can't display all bytes
		if (bytesToShow < input.length)
			msgAppend += sprintf(msgAppend, "...");

		// New line
		msgAppend += sprintf(msgAppend, "\n");

		networkSendDebugMessage(msg);

		// Free Message
		free(msg);
	#endif

	return NO_ERROR;
}

uint8_t poke(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputPoke input = *(struct inputPoke*)(inputBuffer + 1);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@poke] Process Id: %d, Address: 0x%08x, Length: %d\n", client->ip, input.processId, input.address, input.length);
	#endif

	// Ensure memory address is valid
	if (!isAddressRangeValid(client, input.processId, input.address, input.address + input.length))
		return INVALID_ADDRESS;

	// Write buffer into memory into
	struct ptrace_io_desc ptDesc;
	ptDesc.piod_offs = (void*)(input.address);
	ptDesc.piod_len = input.length;
	ptDesc.piod_addr = &inputBuffer[1 + sizeof(struct inputPoke)];
	ptDesc.piod_op = PIOD_WRITE_D;
	ptrace(PT_IO, input.processId, &ptDesc, 0);

	#ifdef DEBUG
		networkSendDebugMessage("			[%s@poke] Wrote %d bytes to process %d\n", client->ip, input.length, input.processId);

		int bytesToShow = (input.length > 0x10 ? 0x10 : input.length); // Limit 16

		// Create buffer for message
		char* msg = malloc(18 + 4 + INET_ADDRSTRLEN + (bytesToShow * 3));
		char* msgAppend = msg;

		// Prepend
		msgAppend += sprintf(msgAppend, "			[%s@poke] Input: ", client->ip);

		// Loop bytes and format
		for (int i = 0; i < bytesToShow; i++)
			msgAppend += sprintf(msgAppend, "%02x ", inputBuffer[1 + sizeof(struct inputPoke) + i]);

		// Append with ... if can't display all bytes
		if (bytesToShow < input.length)
			msgAppend += sprintf(msgAppend, "...");

		// New line
		msgAppend += sprintf(msgAppend, "\n");

		networkSendDebugMessage(msg);

		// Free Message
		free(msg);
	#endif
	
	return NO_ERROR;
}

uint8_t getRegions(struct clientArgs* client, uint8_t** outputBuffer, uint32_t* outputLength, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputRegions input = *(struct inputRegions*)(inputBuffer + 1);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@getRegions] Process Id: %d\n", client->ip, input.processId);
	#endif

	size_t length = 0;
	kinfo_vmentry* maps = malloc(sizeof(kinfo_vmentry*));
	if (getVirtualMemoryMaps(input.processId, &maps, &length) == -1)
	{
		#ifdef DEBUG
			networkSendDebugMessage("			[%s@getRegions] Failed to get virtual memory maps\n", client->ip);
		#endif
	}

	// Count number of entries
	unsigned int entryCount = 0;

	uint64_t entryAddress = (uint64_t)(maps);
	uint64_t endAddress = 0;
	uint64_t offset = 0;
	unsigned int loopIndex = 0;

	while (offset < length)
	{
		kinfo_vmentry entry = *(kinfo_vmentry*)(entryAddress + offset);

		offset += entry.kve_structsize;

		if (entry.kve_structsize == 0)
			break;

		// First loop
		if (loopIndex == 0)
		{
			entryCount++;
			endAddress = entry.kve_end;
			loopIndex++;
			continue;
		}

		// Loop
		// Ensure the gap is greater than VMMAP_GAP
		if (entry.kve_start > endAddress + VMMAP_GAP)
			entryCount += 2;

		endAddress = entry.kve_end;

		// Last Loop
		if (offset >= length)
			entryCount++;

		loopIndex++;
	}

	#ifdef DEBUG
		networkSendDebugMessage("			[%s@getRegions] Found %d memory maps\n", client->ip, (uint32_t)(entryCount / 2));
	#endif

	// Reallocate memory buffer
	*outputBuffer = realloc(*outputBuffer, entryCount * sizeof(uint64_t));

	// Set output length
	*outputLength = entryCount * sizeof(uint64_t);

	// Get entries
	endAddress = 0;
	offset = 0;
	loopIndex = 0;
	uint64_t bufferOffset = 0;

	while (offset < length)
	{
		kinfo_vmentry entry = *(kinfo_vmentry*)(entryAddress + offset);
		offset += entry.kve_structsize;

		if (entry.kve_structsize == 0)
			break;

		// First loop
		if (loopIndex == 0)
		{
			// Append start
			*(uint64_t*)(*outputBuffer + bufferOffset) = entry.kve_start;
			bufferOffset += sizeof(uint64_t);
			#ifdef DEBUG
				networkSendDebugMessage("			[%s@getRegions] %16lx - ", client->ip, entry.kve_start);
			#endif

			endAddress = entry.kve_end;
			loopIndex++;
			continue;
		}

		// Loop
		// Ensure the gap is greater than VMMAP_GAP
		if (entry.kve_start > endAddress + VMMAP_GAP)
		{
			// Append end
			*(uint64_t*)(*outputBuffer + bufferOffset) = endAddress;
			bufferOffset += sizeof(uint64_t);
			#ifdef DEBUG
				networkSendDebugMessage("%16lx\n", endAddress);
			#endif

			// Append start
			*(uint64_t*)(*outputBuffer + bufferOffset) = entry.kve_start;
			bufferOffset += sizeof(uint64_t);
			#ifdef DEBUG
				networkSendDebugMessage("			[%s@getRegions] %16lx - ", client->ip, entry.kve_start);
			#endif
		}
		endAddress = entry.kve_end;

		// Last Loop
		if (offset >= length)
		{
			// Append end
			*(uint64_t*)(*outputBuffer + bufferOffset) = entry.kve_end;
			bufferOffset += sizeof(uint64_t);
			#ifdef DEBUG
				networkSendDebugMessage("%16lx\n", entry.kve_end);
			#endif
		}

		loopIndex++;
	}

	// Clean up allocate memory
	free(maps);

	return NO_ERROR;
}

bool isAddressRangeValid(struct clientArgs* client, uint32_t processId, uint64_t start, uint64_t end)
{
	size_t length = 0;
	kinfo_vmentry* maps = malloc(sizeof(kinfo_vmentry*));
	if (getVirtualMemoryMaps(processId, &maps, &length) == -1)
	{
		#ifdef DEBUG
			networkSendDebugMessage("			[%s@isAddressRangeValid] Failed to get virtual memory maps\n", client->ip);
		#endif
	}

	// Loop entries
	uint64_t entryAddress = (uint64_t)(maps);
	uint64_t offset = 0;

	while (offset < length)
	{
		kinfo_vmentry entry = *(kinfo_vmentry*)(entryAddress + offset);

		offset += entry.kve_structsize;

		if (entry.kve_structsize == 0)
			break;

		if (start >= entry.kve_start && end <= entry.kve_end)
			return true;
	}

	// Clean up allocate memory
	free(maps);
	return false;
}

int32_t getVirtualMemoryMaps(uint32_t processId, kinfo_vmentry** entries, size_t* length)
{
	int32_t mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_VMMAP, processId };

	// Get data length
	if (sysctl(mib, 4, NULL, length, NULL, 0) == -1)
		return -1;

	// Allocate memory and retrieve data
	*entries = realloc(*entries, *length);
	if (sysctl(mib, 4, (char*)*entries, length, NULL, 0) == -1)
		return -1;
	return 0;
}