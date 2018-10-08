#pragma once

#define	KERN_BASE_PTR 		0x00001C0
#define	KERN_BZERO			0x01ea510 
#define	KERN_PRISON0 		0x10986A0
#define	KERN_ROOTVNODE 		0x22C1A70

struct auditinfo_addr {
    /*
    4    ai_auid;
    8    ai_mask;
    24    ai_termid;
    4    ai_asid;
    8    ai_flags;r
    */
    char useless[184];
};

struct ucred {
			uint32_t useless1;
			uint32_t cr_uid;     // effective user id
			uint32_t cr_ruid;    // real user id
 			uint32_t useless2;
    	uint32_t useless3;
    	uint32_t cr_rgid;    // real group id
    	uint32_t useless4;
    	void *useless5;
    	void *useless6;
    	void *cr_prison;     // jail(2)
    	void *useless7;
    	uint32_t useless8;
    	void *useless9[2];
    	void *useless10;
    	struct auditinfo_addr useless11;
    	uint32_t *cr_groups; // groups
    	uint32_t useless12;
};

struct filedesc {
  void *useless1[3];
      void *fd_rdir;
      void *fd_jdir;
};

struct proc {
      char useless[64];
      struct ucred *p_ucred;
      struct filedesc *p_fd;
};

struct thread {
    	void *useless;
    	struct proc *td_proc;
};

/*
 * Function:  getRoot
 * --------------------
 * Escalate privileges to root
 *
 *	td:		The thread
 *	args:	Optional args passed
 *
 *  returns: int
 */
int getRoot(struct thread* td, void* args);

unsigned int long long __readmsr(unsigned long __register);