#include "defines.h"
#include "ps4.h"
#include "networking.h"
#include "debug.h"
#include "rpc.h"

struct inputDebug {
	uint32_t processId;
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