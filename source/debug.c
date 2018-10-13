#include "defines.h"
#include "ps4.h"
#include "networking.h"
#include "debug.h"
#include "rpc.h"

struct inputDebug {
	uint32_t processId;
} __attribute__((packed));

struct inputDebugAddWatchpoint {
	uint32_t processId;
	uint32_t registerIndex;
	uint64_t address;
	uint32_t length;
	uint32_t type;
} __attribute__((packed));

struct inputDebugRemoveWatchpoint {
	uint32_t processId;
	uint32_t registerIndex;
} __attribute__((packed));

uint8_t debugContinue(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputDebug input = *(struct inputDebug*)(inputBuffer + 1);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@debugContinue] Process Id: %d\n", client->ip, input.processId);
	#endif

	ptrace(PT_CONTINUE, input.processId, (void*)1, NULL);

	return NO_ERROR;
}

uint8_t debugStop(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputDebug input = *(struct inputDebug*)(inputBuffer + 1);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@debugStop] Process Id: %d\n", client->ip, input.processId);
	#endif

	ptrace(PT_CONTINUE, input.processId, (void*)1, SIGSTOP);

	return NO_ERROR;
}

uint8_t debugKill(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputDebug input = *(struct inputDebug*)(inputBuffer + 1);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@debugKill] Process Id: %d\n", client->ip, input.processId);
	#endif

	ptrace(PT_KILL, input.processId, (void*)1, 0);

	return NO_ERROR;
}

uint8_t debugStep(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputDebug input = *(struct inputDebug*)(inputBuffer + 1);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@debugStep] Process Id: %d\n", client->ip, input.processId);
	#endif

	ptrace(PT_STEP, input.processId, (void*)1, 0);

	return NO_ERROR;
}

uint8_t debugGetRegisters(struct clientArgs* client, uint8_t** outputBuffer, uint32_t* outputLength, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputDebug input = *(struct inputDebug*)(inputBuffer + 1);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@debugGetRegisters] Process Id: %d\n", client->ip, input.processId);
	#endif

	struct reg registers;

	// Reallocate memory buffer
	*outputBuffer = realloc(*outputBuffer, sizeof(struct reg));
	*outputLength = sizeof(struct reg);

	// Get registers
	ptrace(PT_GETREGS, input.processId, &registers, 0);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("\n			[%s@debugGetRegisters] Registers\n\n", client->ip);

		networkSendDebugMessage("			[%s@debugGetRegisters] RAX: %016lX	RBX: %016lX\n", client->ip, registers.r_rax, registers.r_rbx);
		networkSendDebugMessage("			[%s@debugGetRegisters] RCX: %016lX	RDX: %016lX\n", client->ip, registers.r_rcx, registers.r_rdx);
		networkSendDebugMessage("			[%s@debugGetRegisters] RSI: %016lX	RDI: %016lX\n", client->ip, registers.r_rsi, registers.r_rdi);
		networkSendDebugMessage("			[%s@debugGetRegisters] RBP: %016lX	RSP: %016lX\n", client->ip, registers.r_rbp, registers.r_rsp);
		networkSendDebugMessage("			[%s@debugGetRegisters] R08: %016lX	R09: %016lX\n", client->ip, registers.r_r8, registers.r_r9);
		networkSendDebugMessage("			[%s@debugGetRegisters] R10: %016lX	R11: %016lX\n", client->ip, registers.r_r10, registers.r_r11);
		networkSendDebugMessage("			[%s@debugGetRegisters] R12: %016lX	R13: %016lX\n", client->ip, registers.r_r12, registers.r_r13);
		networkSendDebugMessage("			[%s@debugGetRegisters] R14: %016lX	R15: %016lx\n\n", client->ip, registers.r_r14, registers.r_r15);

		networkSendDebugMessage("			[%s@debugGetRegisters] RIP: %016lX\n\n", client->ip, registers.r_rip);

		networkSendDebugMessage("			[%s@debugGetRegisters] Flags: %#x [ %c %c %c %c %c %c %c %c %c ]\n\n", client->ip, registers.r_rflags,
			CHECK_BIT(registers.r_rflags, FLAG_CARRY) ? 'C' : '-',
			CHECK_BIT(registers.r_rflags, FLAG_PARITY) ? 'P' : '-',
			CHECK_BIT(registers.r_rflags, FLAG_AUX_CARRY) ? 'A' : '-',
			CHECK_BIT(registers.r_rflags, FLAG_ZERO) ? 'Z' : '-',
			CHECK_BIT(registers.r_rflags, FLAG_SIGN) ? 'S' : '-',
			CHECK_BIT(registers.r_rflags, FLAG_TRACE) ? 'T' : '-',
			CHECK_BIT(registers.r_rflags, FLAG_INTERRUPT) ? 'I' : '-',
			CHECK_BIT(registers.r_rflags, FLAG_DIRECTION) ? 'D' : '-',
			CHECK_BIT(registers.r_rflags, FLAG_OVERFLOW) ? 'O' : '-'
		);

		networkSendDebugMessage("			[%s@debugGetRegisters] CS: %04X	DS: %04X	SS: %04X\n", client->ip, registers.r_cs, registers.r_ds, registers.r_ss);
		networkSendDebugMessage("			[%s@debugGetRegisters] ES: %04X	FS: %04X	GS: %04X\n\n", client->ip, registers.r_es, registers.r_fs, registers.r_gs);

		networkSendDebugMessage("			[%s@debugGetRegisters] trapno: %08X	err: %08X\n\n", client->ip, registers.r_trapno, registers.r_err);
	#endif

	// Store registers in output buffer
	*(struct reg*)(*outputBuffer) = registers;

	return NO_ERROR;
}

uint8_t debugGetDebugRegisters(struct clientArgs* client, uint8_t** outputBuffer, uint32_t* outputLength, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputDebug input = *(struct inputDebug*)(inputBuffer + 1);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@debugGetDebugRegisters] Process Id: %d\n", client->ip, input.processId);
	#endif

	struct dbreg registers;

	// Reallocate memory buffer
	*outputBuffer = realloc(*outputBuffer, sizeof(struct dbreg));
	*outputLength = sizeof(struct dbreg);

	// Get registers
	ptrace(PT_GETDBREGS, input.processId, &registers, 0);

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("\n			[%s@debugGetDebugRegisters] Debug Registers\n\n", client->ip);

		networkSendDebugMessage("			[%s@debugGetRegisters] DR0: %016lX	L: %c  G: %c  C: %c%c  S: %c%c\n", client->ip, DBREG_DRXX(registers, 0),
			CHECK_BIT(DBREG_DRXX(registers, 7), 0) ? 'Y' : 'N',
			CHECK_BIT(DBREG_DRXX(registers, 7), 1) ? 'Y' : 'N',
			CHECK_BIT(DBREG_DRXX(registers, 7), 16) ? '1' : '0',
			CHECK_BIT(DBREG_DRXX(registers, 7), 17) ? '1' : '0',
			CHECK_BIT(DBREG_DRXX(registers, 7), 18) ? '1' : '0',
			CHECK_BIT(DBREG_DRXX(registers, 7), 19) ? '1' : '0'
		);
		networkSendDebugMessage("			[%s@debugGetRegisters] DR1: %016lX	L: %c  G: %c  C: %c%c  S: %c%c\n", client->ip, DBREG_DRXX(registers, 1),
			CHECK_BIT(DBREG_DRXX(registers, 7), 2) ? 'Y' : 'N',
			CHECK_BIT(DBREG_DRXX(registers, 7), 3) ? 'Y' : 'N',
			CHECK_BIT(DBREG_DRXX(registers, 7), 20) ? '1' : '0',
			CHECK_BIT(DBREG_DRXX(registers, 7), 21) ? '1' : '0',
			CHECK_BIT(DBREG_DRXX(registers, 7), 22) ? '1' : '0',
			CHECK_BIT(DBREG_DRXX(registers, 7), 23) ? '1' : '0'
		);
		networkSendDebugMessage("			[%s@debugGetRegisters] DR2: %016lX	L: %c  G: %c  C: %c%c  S: %c%c\n", client->ip, DBREG_DRXX(registers, 2),
			CHECK_BIT(DBREG_DRXX(registers, 7), 4) ? 'Y' : 'N',
			CHECK_BIT(DBREG_DRXX(registers, 7), 5) ? 'Y' : 'N',
			CHECK_BIT(DBREG_DRXX(registers, 7), 24) ? '1' : '0',
			CHECK_BIT(DBREG_DRXX(registers, 7), 25) ? '1' : '0',
			CHECK_BIT(DBREG_DRXX(registers, 7), 26) ? '1' : '0',
			CHECK_BIT(DBREG_DRXX(registers, 7), 27) ? '1' : '0'
		);
		networkSendDebugMessage("			[%s@debugGetRegisters] DR3: %016lX	L: %c  G: %c  C: %c%c  S: %c%c\n\n", client->ip, DBREG_DRXX(registers, 3),
			CHECK_BIT(DBREG_DRXX(registers, 7), 6) ? 'Y' : 'N',
			CHECK_BIT(DBREG_DRXX(registers, 7), 7) ? 'Y' : 'N',
			CHECK_BIT(DBREG_DRXX(registers, 7), 28) ? '1' : '0',
			CHECK_BIT(DBREG_DRXX(registers, 7), 29) ? '1' : '0',
			CHECK_BIT(DBREG_DRXX(registers, 7), 30) ? '1' : '0',
			CHECK_BIT(DBREG_DRXX(registers, 7), 31) ? '1' : '0'
		);

		networkSendDebugMessage("			[%s@debugGetRegisters] DR6: %016lX	DR7: %016lX\n\n", client->ip, DBREG_DRXX(registers, 6), DBREG_DRXX(registers, 7));
	#endif

	// Store registers in output buffer
	*(struct dbreg*)(*outputBuffer) = registers;

	return NO_ERROR;
}

uint8_t debugAddWatchpoint(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputDebugAddWatchpoint input = *(struct inputDebugAddWatchpoint*)(inputBuffer + 1);

	// Ensure the register index is between 0 and 3
	if (input.registerIndex < 0 || input.registerIndex > 3)
		return INVALID_REGISTER_INDEX;

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@debugAddWatchpoint] Process Id: %d, Register: DR%d, Address: %#16lx, Length: %c%c%c%c, Type: %c%c%c\n", client->ip,
			input.processId,
			input.registerIndex,
			input.address,
			input.length == DBREG_DR7_LEN_1 ? '1' : '-',
			input.length == DBREG_DR7_LEN_2 ? '2' : '-',
			input.length == DBREG_DR7_LEN_4 ? '4' : '-',
			input.length == DBREG_DR7_LEN_8 ? '8' : '-',
			input.type == DBREG_DR7_EXEC ? 'e' : '-',
			input.type == DBREG_DR7_WRONLY || DBREG_DR7_RDWR ? 'w' : '-',
			input.type == DBREG_DR7_RDWR ? 'r' : '-'
		);
	#endif

	// Get this processes threads
	size_t threadCount = ptrace(PT_GETNUMLWPS, input.processId, NULL, 0);
	uint32_t* threads = malloc(threadCount * sizeof(uint32_t));
	ptrace(PT_GETLWPLIST, input.processId, (void*)threads, threadCount);

	// Get Registers
	struct dbreg registers;
	ptrace(PT_GETDBREGS, input.processId, &registers, 0);

	// Add watchpoint
	DBREG_DRXX(registers, 7) &= ~DBREG_DR7_MASK(input.registerIndex);
	DBREG_DRXX(registers, input.registerIndex) = input.address; 
	DBREG_DRXX(registers, 7) |= DBREG_DR7_SET(input.registerIndex, input.length, input.type, DBREG_DR7_LOCAL_ENABLE | DBREG_DR7_GLOBAL_ENABLE);

	// Apply debug registers to each thread process id
	for (int i = 0; i < threadCount; i++)
		ptrace(PT_SETDBREGS, threads[i], &registers, NULL);

	// Cleanup memory
	free(threads);

	return NO_ERROR;
}

uint8_t debugRemoveWatchpoint(struct clientArgs* client, uint8_t* inputBuffer, uint32_t inputLength)
{
	struct inputDebugRemoveWatchpoint input = *(struct inputDebugRemoveWatchpoint*)(inputBuffer + 1);

	// Ensure the register index is between 0 and 3
	if (input.registerIndex < 0 || input.registerIndex > 3)
		return INVALID_REGISTER_INDEX;

	// Debug Information
	#ifdef DEBUG
		networkSendDebugMessage("			[%s@debugRemoveWatchpoint] Process Id: %d, Register: DR%d\n", client->ip, input.processId, input.registerIndex);
	#endif

	// Get this processes threads
	size_t threadCount = ptrace(PT_GETNUMLWPS, input.processId, NULL, 0);
	uint32_t* threads = malloc(threadCount * sizeof(uint32_t));
	ptrace(PT_GETLWPLIST, input.processId, (void*)threads, threadCount);

	// Get Registers
	struct dbreg registers;
	ptrace(PT_GETDBREGS, input.processId, &registers, 0);

	// Remove watchpoint
	DBREG_DRXX(registers, 7) &= ~DBREG_DR7_MASK(input.registerIndex);
	DBREG_DRXX(registers, input.registerIndex) = NULL; 
	DBREG_DRXX(registers, 7) |= DBREG_DR7_SET(input.registerIndex, NULL, NULL, NULL);

	// Apply debug registers to each thread process id
	for (int i = 0; i < threadCount; i++)
		ptrace(PT_SETDBREGS, threads[i], &registers, NULL);

	// Cleanup memory
	free(threads);

	return NO_ERROR;
}