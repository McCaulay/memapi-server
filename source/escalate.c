#include "defines.h"
#include "ps4.h"
#include "escalate.h"
#include "networking.h"

unsigned int long long __readmsr(unsigned long __register) {
	// Loads the contents of a 64-bit model specific register (MSR) specified in
	// the ECX register into registers EDX:EAX. The EDX register is loaded with
	// the high-order 32 bits of the MSR and the EAX register is loaded with the
	// low-order 32 bits. If less than 64 bits are implemented in the MSR being
	// read, the values returned to EDX:EAX in unimplemented bit locations are
	// undefined.
	unsigned long __edx;
	unsigned long __eax;
	__asm__ ("rdmsr" : "=d"(__edx), "=a"(__eax) : "c"(__register));
	return (((unsigned int long long)__edx) << 32) | (unsigned int long long)__eax;
}

int getRoot(struct thread* td, void* args)
{
	// Resolve creds
	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-KERN_BASE_PTR];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 =   (void**)&kernel_ptr[KERN_PRISON0];
	void** got_rootvnode = (void**)&kernel_ptr[KERN_ROOTVNODE];

	// Escalate process to root
	cred->cr_uid = 0;
	cred->cr_ruid = 0;
	cred->cr_rgid = 0;
	cred->cr_groups[0] = 0;

	cred->cr_prison = *got_prison0;
	fd->fd_rdir = fd->fd_jdir = *got_rootvnode;

	void *td_ucred = *(void **)(((char *)td) + 304); // p_ucred == td_ucred

	// sceSblACMgrIsSystemUcred
	uint64_t *sonyCred = (uint64_t *)(((char *)td_ucred) + 96);
	*sonyCred = 0xffffffffffffffff;
	
	// sceSblACMgrGetDeviceAccessType
	uint64_t *sceProcType = (uint64_t *)(((char *)td_ucred) + 88);
	*sceProcType = 0x3801000000000013; // Max access
	
	// sceSblACMgrHasSceProcessCapability
	uint64_t *sceProcCap = (uint64_t *)(((char *)td_ucred) + 104);
	*sceProcCap = 0xffffffffffffffff; // Sce Process

	uint64_t cr0 = readCr0();
	writeCr0(cr0 & ~X86_CR0_WP);
	
	// Patch memcpy first
    *(uint8_t *)(kernel_base + 0x1EA53D) = 0xEB;

    // Patch sceSblACMgrIsAllowedSystemLevelDebugging
    memcpy((void *)(kernel_base + 0x11730), "\x48\xC7\xC0\x01\x00\x00\x00\xC3", 8);

    // Patch sceSblACMgrHasMmapSelfCapability
    memcpy((void *)(kernel_base + 0x117B0), "\x48\xC7\xC0\x01\x00\x00\x00\xC3", 8);

    // Patch sceSblACMgrIsAllowedToMmapSelf
    memcpy((void *)(kernel_base + 0x117C0), "\x48\xC7\xC0\x01\x00\x00\x00\xC3", 8);

    // Self patches
    memcpy((void *)(kernel_base + 0x13F03F), "\x31\xC0\x90\x90\x90", 5);

    // Patch vm_map_protect check
    memcpy((void *)(kernel_base + 0x1A3C08), "\x90\x90\x90\x90\x90\x90", 6);

    // Patch ptrace, thanks 2much4u
    *(uint8_t *)(kernel_base + 0x30D9AA) = 0xEB;

    // Patch ptrace check, thanks golden
    memcpy((void *)(kernel_base + 0x30DE01), "\xE9\xD0\x00\x00\x00", 5);

    // Patch ASLR, thanks 2much4u
    *(uint16_t *)(kernel_base + 0x194875) = 0x9090;

	// Restore write protection
	writeCr0(cr0);

	return 0;
}