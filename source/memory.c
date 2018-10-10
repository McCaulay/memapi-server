#include "defines.h"
#include "ps4.h"
#include "memory.h"
#include "rpc.h"
#include "networking.h"

struct inputPeek {
	int processId;
	unsigned long address;
	unsigned int length;
} __attribute__((packed));

struct inputPoke {
	int processId;
	unsigned long address;
	unsigned int length;
} __attribute__((packed));

struct inputRegions {
	int processId;
} __attribute__((packed));

int peek(char* ip, unsigned char** outputBuffer, int* outputLength, unsigned char* inputBuffer, int inputLength)
{
	struct inputPeek input = *(struct inputPeek*)(inputBuffer + 1);

	// Debug Information
	if (DEBUG)
		networkSendDebugMessage("			[%s@peek] Process Id: %d, Address: 0x%08x, Length: %d\n", ip, input.processId, input.address, input.length);

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
	
	if (DEBUG)
	{
		networkSendDebugMessage("			[%s@peek] Read %d bytes from process %d\n", ip, input.length, input.processId);

		int bytesToShow = (input.length > 0x10 ? 0x10 : input.length); // Limit 16

		// Create buffer for message
		char* msg = malloc(19 + 4 + INET_ADDRSTRLEN + (bytesToShow * 3));
		char* msgAppend = msg;

		// Prepend
		msgAppend += sprintf(msgAppend, "			[%s@peek] Output: ", ip);

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
	}

	return NO_ERROR;
}

int poke(char* ip, unsigned char* inputBuffer, int inputLength)
{
	struct inputPoke input = *(struct inputPoke*)(inputBuffer + 1);

	// Debug Information
	if (DEBUG)
		networkSendDebugMessage("			[%s@poke] Process Id: %d, Address: 0x%08x, Length: %d\n", ip, input.processId, input.address, input.length);

	// Write buffer into memory into
	struct ptrace_io_desc ptDesc;
	ptDesc.piod_offs = (void*)(input.address);
	ptDesc.piod_len = input.length;
	ptDesc.piod_addr = &inputBuffer[1 + sizeof(struct inputPoke)];
	ptDesc.piod_op = PIOD_WRITE_D;
	ptrace(PT_IO, input.processId, &ptDesc, 0);

	if (DEBUG)
	{
		networkSendDebugMessage("			[%s@poke] Wrote %d bytes to process %d\n", ip, input.length, input.processId);

		int bytesToShow = (input.length > 0x10 ? 0x10 : input.length); // Limit 16

		// Create buffer for message
		char* msg = malloc(18 + 4 + INET_ADDRSTRLEN + (bytesToShow * 3));
		char* msgAppend = msg;

		// Prepend
		msgAppend += sprintf(msgAppend, "			[%s@poke] Input: ", ip);

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
	}
	
	return NO_ERROR;
}

int getRegions(char* ip, unsigned char** outputBuffer, int* outputLength, unsigned char* inputBuffer, int inputLength)
{
	struct inputRegions input = *(struct inputRegions*)(inputBuffer + 1);

	// Debug Information
	if (DEBUG)
		networkSendDebugMessage("			[%s@getRegions] Process Id: %d\n", ip, input.processId);

	size_t length = 0;
	kinfo_vmentry* maps = malloc(sizeof(kinfo_vmentry*));
	if (getVirtualMemoryMaps(input.processId, &maps, &length) == -1)
	{
		if (DEBUG)
			networkSendDebugMessage("			[%s@getRegions] Failed to get virtual memory maps\n", ip);
	}

	networkSendDebugMessage("			[%s@getRegions] Virtual Memory: %d\n", ip, length);

	// Count number of entries
	int entryCount = 0;

	unsigned long entryAddress = (unsigned long)(maps);
	unsigned long endAddress = 0;
	unsigned long offset = 0;
	int loopIndex = 0;

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

	if (DEBUG)
		networkSendDebugMessage("			[%s@getRegions] Found %d memory maps\n", ip, (int)(entryCount / 2));

	// Reallocate memory buffer
	*outputBuffer = realloc(*outputBuffer, entryCount * sizeof(long));

	// Set output length
	*outputLength = entryCount * sizeof(long);

	// Get entries
	endAddress = 0;
	offset = 0;
	loopIndex = 0;
	unsigned long bufferOffset = 0;

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
			*(long*)(*outputBuffer + bufferOffset) = entry.kve_start;
			bufferOffset += sizeof(long);
			if (DEBUG)
				networkSendDebugMessage("			[%s@getRegions] %08x - ", ip, entry.kve_start);

			endAddress = entry.kve_end;
			loopIndex++;
			continue;
		}

		// Loop
		// Ensure the gap is greater than VMMAP_GAP
		if (entry.kve_start > endAddress + VMMAP_GAP)
		{
			// Append end
			*(long*)(*outputBuffer + bufferOffset) = endAddress;
			bufferOffset += sizeof(long);
			if (DEBUG)
				networkSendDebugMessage("%08x\n", endAddress);

			// Append start
			*(long*)(*outputBuffer + bufferOffset) = entry.kve_start;
			bufferOffset += sizeof(long);
			if (DEBUG)
				networkSendDebugMessage("			[%s@getRegions] %08x - ", ip, entry.kve_start);
		}
		endAddress = entry.kve_end;

		// Last Loop
		if (offset >= length)
		{
			// Append end
			*(long*)(*outputBuffer + bufferOffset) = entry.kve_end;
			bufferOffset += sizeof(long);
			if (DEBUG)
				networkSendDebugMessage("%08x\n", entry.kve_end);
		}

		loopIndex++;
	}

	// Clean up allocate memory
	free(maps);

	return NO_ERROR;
}

int getVirtualMemoryMaps(int processId, kinfo_vmentry** entries, size_t* length)
{
	int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_VMMAP, processId };

	// Get data length
	if (sysctl(mib, 4, NULL, length, NULL, 0) == -1)
		return -1;

	// Allocate memory and retrieve data
	*entries = realloc(*entries, *length);
	if (sysctl(mib, 4, (char*)*entries, length, NULL, 0) == -1)
		return -1;
	return 0;
}