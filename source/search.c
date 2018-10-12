#include "defines.h"
#include "ps4.h"
#include "search.h"
#include "rpc.h"
#include "networking.h"
#include "client.h"

struct inputSearchStart {
	uint32_t processId;
	uint64_t startAddress;
	uint64_t endAddress;
	uint32_t length;
} __attribute__((packed));

struct inputSearchRescan {
	uint32_t processId;
	uint32_t length;
} __attribute__((packed));

struct searchResults {
  uint64_t* items;
  size_t count;
  size_t size;
};

enum SearchState {
	STARTED,
	ENDED,
};
enum SearchState state;

struct searchResults results;

void allocateResults(struct searchResults* sResults, size_t initialSize)
{
	// If state has started, return as it's already allocated
	if (state == STARTED)
		return;

	sResults->items = (uint64_t*)malloc(initialSize * sizeof(uint64_t));
	sResults->count = 0;
	sResults->size = initialSize;
	state = STARTED;
}

void addResult(struct searchResults* sResults, uint64_t result)
{
	// If state has ended, return as it's not allocated
	if (state == ENDED)
		return;

	// If we've hit the limit, double the allocate size
	if (sResults->count == sResults->size)
	{
		sResults->size *= 2;
		sResults->items = (uint64_t*)realloc(sResults->items, sResults->size * sizeof(uint64_t));
	}
	sResults->items[sResults->count++] = result;
}

void removeResult(struct searchResults* sResults, uint32_t index)
{
	// If state has ended, return as it's not allocated
	if (state == ENDED)
		return;

	sResults->items[index] = NULL;
	sResults->count--;
}

void cleanResults(struct searchResults* sResults)
{
	// If state has ended, return as it's not allocated
	if (state == ENDED)
		return;

	uint32_t validResults = 0;
	uint32_t index = 0;
	while (validResults < sResults->count && index < sResults->size)
	{
		if (sResults->items[index] != NULL)
		{
			sResults->items[validResults] = sResults->items[index];
			validResults++;
		}
		index++;
	}
}

void freeResults(struct searchResults* sResults)
{
	// If state has ended, return as it's not allocated
	if (state == ENDED)
		return;

	free(sResults->items);
	sResults->items = NULL;
	sResults->count = 0;
	sResults->size = 0;
	state = ENDED;
}

uint8_t searchStart(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputSearchStart input = *(struct inputSearchStart*)(inputBuffer + 1);
	void* inputData = (void*)(inputBuffer + 1 + sizeof(struct inputSearchStart));

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@searchStart] Process Id: %d, Start Address: %#16lx, End Address: %#16lx, Length: %d\n", client->ip, input.processId, input.startAddress, input.endAddress, input.length);
	#endif

	// If memory is already allocated (ie Started searching previously without ending) then free previous results
	if (state == STARTED)
		freeResults(&results);

	// Allocate results memory
	allocateResults(&results, 100);

	size_t length = 0;
	kinfo_vmentry* maps = malloc(sizeof(kinfo_vmentry*));
	if (getVirtualMemoryMaps(input.processId, &maps, &length) == -1)
	{
		#ifdef DEBUG
			networkSendDebugMessage("			[%s@searchStart] Failed to get virtual memory maps\n", client->ip);
		#endif
	}

	uint64_t entryAddress = (uint64_t)(maps);
	uint64_t offset = 0;
	bool inSearchRegion = false;

	// Loop each memory region
	while (offset < length)
	{
		kinfo_vmentry entry = *(kinfo_vmentry*)(entryAddress + offset);

		offset += entry.kve_structsize;

		if (entry.kve_structsize == 0)
			break;

		// Start search region
		if (input.startAddress >= entry.kve_start && input.startAddress <= entry.kve_end)
			inSearchRegion = true;

		#ifdef DEBUG
			networkSendDebugMessage("			[%s@searchStart] %#16lx to %#16lx (In: %c)\n", client->ip, entry.kve_start, entry.kve_end, inSearchRegion ? 'y' : 'n');
		#endif

		// In this memory region
		if (inSearchRegion)
		{
			// Default start/length is full region
			uint64_t dumpStart = entry.kve_start;
			uint64_t dumpLength = entry.kve_end - entry.kve_start;

			// If we only need to dump part of the region, only dump the part we need
			if (input.startAddress >= entry.kve_start && input.startAddress <= entry.kve_end && input.endAddress >= entry.kve_start && input.endAddress <= entry.kve_end)
			{
				dumpStart = input.startAddress;
				dumpLength = input.endAddress - input.startAddress;
			}
			else if (input.startAddress >= entry.kve_start && input.startAddress <= entry.kve_end)
			{
				dumpStart = input.startAddress;
				dumpLength = entry.kve_end - input.startAddress;
			}
			else if (input.endAddress >= entry.kve_start && input.endAddress <= entry.kve_end)
			{
				dumpStart = entry.kve_start;
				dumpLength = input.endAddress - entry.kve_start;	
			}

			#ifdef DEBUG
				networkSendDebugMessage("			[%s@searchStart] Dumping %#16lx to %#16lx\n", client->ip, dumpStart, dumpStart + dumpLength);
			#endif

			// Dump Memory
			uint8_t* buffer = malloc(dumpLength);
			struct ptrace_io_desc ptDesc;
			ptDesc.piod_offs = (void*)(dumpStart);
			ptDesc.piod_len = dumpLength;
			ptDesc.piod_addr = buffer;
			ptDesc.piod_op = PIOD_READ_D;
			ptrace(PT_IO, input.processId, &ptDesc, 0);

			// Loop each byte in buffer
			bool found = false;
			for (uint64_t index = 0; index < dumpLength; index++)
			{
				// See if sequence matches
				found = true;
				for (int i = 0; i < input.length; i++)
				{
					if (*(uint8_t*)(buffer + index + i) != *(uint8_t*)(inputData + i))
					{
						found = false;
						break;
					}
				}

				if (found)
					addResult(&results, dumpStart + index);
			}

			free(buffer);
		}

		// End search region
		if (input.endAddress >= entry.kve_start && input.endAddress <= entry.kve_end)
			inSearchRegion = false;
	}

	#ifdef DEBUG
		networkSendDebugMessage("			[%s@searchStart] Found %d results\n", client->ip, results.count);
	#endif

	free(maps);

	return NO_ERROR;
}

uint8_t searchRescan(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	// If there isn't a started search, then return an error.
	if (state == ENDED)
		return SEARCH_NOT_STARTED;

	struct inputSearchRescan input = *(struct inputSearchRescan*)(inputBuffer + 1);
	void* inputData = (void*)(inputBuffer + 1 + sizeof(struct inputSearchRescan));

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@searchRescan] Process Id: %d, Length: %d\n", client->ip, input.processId, input.length);
	#endif

	//TODO: If results.count > (some value eg 500000) then dump entire memory again like searchStart, and 
	// check each results from that, which may be quicker than dumping each result individually...

	// Allocate buffer
	uint8_t* buffer = malloc(input.length);

	// Loop each result
	int resultCount = results.count;
	for (uint32_t index = 0; index < resultCount; index++)
	{
		// Dump memory at address
		struct ptrace_io_desc ptDesc;
		ptDesc.piod_offs = (void*)(results.items[index]);
		ptDesc.piod_len = input.length;
		ptDesc.piod_addr = buffer;
		ptDesc.piod_op = PIOD_READ_D;
		ptrace(PT_IO, input.processId, &ptDesc, 0);

		// Loop our sequence bytes and compare
		for (uint32_t i = 0; i < input.length; i++)
		{
			// Byte does not match, so remove it from results
			if (*(uint8_t*)(buffer + i) != *(uint8_t*)(inputData + i))
			{
				removeResult(&results, index);
				break;
			}
		}
	}

	// Cleanup removed results
	cleanResults(&results);

	#ifdef DEBUG
		networkSendDebugMessage("			[%s@searchRescan] Found %d results\n", client->ip, results.count);
	#endif

	// Free buffer
	free(buffer);

	return NO_ERROR;
}

uint8_t searchGetResults(struct clientArgs* client, uint8_t** outputBuffer, uint32_t* outputLength)
{
	// If the search has ended, there is no results so return
	if (state == ENDED)
		return NO_ERROR;

	// Reallocate memory buffer
	*outputBuffer = realloc(*outputBuffer, sizeof(uint64_t) * results.count);

	// Set output length
	*outputLength = sizeof(uint64_t) * results.count;

	for (uint32_t i = 0; i < results.count; i++)
	{
		*(uint64_t*)((*outputBuffer) + (i * sizeof(uint64_t))) = results.items[i];

		// Debug Information
		#ifdef DEBUG
			networkSendDebugMessage("			[%s@searchGetResults] [%d/%d] %#16lx\n", client->ip, i + 1, results.count, results.items[i]);
		#endif
	}

	return NO_ERROR;
}

uint8_t searchCountResults(struct clientArgs* client, uint8_t** outputBuffer, uint32_t* outputLength)
{
	size_t count = state == STARTED ? results.count : 0;

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@searchCountResults] Found %d results\n", client->ip, count);
	#endif

	// Reallocate memory buffer
	*outputBuffer = realloc(*outputBuffer, sizeof(size_t));

	// Set output length
	*outputLength = sizeof(size_t);

	*(size_t*)(*outputBuffer) = count;

	return NO_ERROR;
}

uint8_t searchEnd(struct clientArgs* client)
{
	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@searchEnd] Freeing search results\n", client->ip);
	#endif

	// Free results
	freeResults(&results);

	return NO_ERROR;
}