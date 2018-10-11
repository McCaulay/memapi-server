#pragma once

#include "client.h"

//From https://github.com/freebsd/freebsd/blob/master/sys/sys/sysctl.h#L813
/*
 * Top-level identifiers
 */
#define	CTL_UNSPEC	0		/* unused */
#define	CTL_KERN	1		/* "high kernel": proc, limits */
#define	CTL_VM		2		/* virtual memory */
#define	CTL_VFS		3		/* filesystem, mount type is next */
#define	CTL_NET		4		/* network, see socket.h */
#define	CTL_DEBUG	5		/* debugging parameters */
#define	CTL_HW		6		/* generic cpu/io */
#define	CTL_MACHDEP	7		/* machine dependent */
#define	CTL_USER	8		/* user-level */
#define	CTL_P1003_1B	9		/* POSIX 1003.1B */

/*
 * CTL_KERN identifiers
 */
#define	KERN_OSTYPE		 1	/* string: system version */
#define	KERN_OSRELEASE		 2	/* string: system release */
#define	KERN_OSREV		 3	/* int: system revision */
#define	KERN_VERSION		 4	/* string: compile time info */
#define	KERN_MAXVNODES		 5	/* int: max vnodes */
#define	KERN_MAXPROC		 6	/* int: max processes */
#define	KERN_MAXFILES		 7	/* int: max open files */
#define	KERN_ARGMAX		 8	/* int: max arguments to exec */
#define	KERN_SECURELVL		 9	/* int: system security level */
#define	KERN_HOSTNAME		10	/* string: hostname */
#define	KERN_HOSTID		11	/* int: host identifier */
#define	KERN_CLOCKRATE		12	/* struct: struct clockrate */
#define	KERN_VNODE		13	/* struct: vnode structures */
#define	KERN_PROC		14	/* struct: process entries */
#define	KERN_FILE		15	/* struct: file entries */
#define	KERN_PROF		16	/* node: kernel profiling info */
#define	KERN_POSIX1		17	/* int: POSIX.1 version */
#define	KERN_NGROUPS		18	/* int: # of supplemental group ids */
#define	KERN_JOB_CONTROL	19	/* int: is job control available */
#define	KERN_SAVED_IDS		20	/* int: saved set-user/group-ID */
#define	KERN_BOOTTIME		21	/* struct: time kernel was booted */
#define	KERN_NISDOMAINNAME	22	/* string: YP domain name */
#define	KERN_UPDATEINTERVAL	23	/* int: update process sleep time */
#define	KERN_OSRELDATE		24	/* int: kernel release date */
#define	KERN_NTP_PLL		25	/* node: NTP PLL control */
#define	KERN_BOOTFILE		26	/* string: name of booted kernel */
#define	KERN_MAXFILESPERPROC	27	/* int: max open files per proc */
#define	KERN_MAXPROCPERUID	28	/* int: max processes per uid */
#define	KERN_DUMPDEV		29	/* struct cdev *: device to dump on */
#define	KERN_IPC		30	/* node: anything related to IPC */
#define	KERN_DUMMY		31	/* unused */
#define	KERN_PS_STRINGS		32	/* int: address of PS_STRINGS */
#define	KERN_USRSTACK		33	/* int: address of USRSTACK */
#define	KERN_LOGSIGEXIT		34	/* int: do we log sigexit procs? */
#define	KERN_IOV_MAX		35	/* int: value of UIO_MAXIOV */
#define	KERN_HOSTUUID		36	/* string: host UUID identifier */
#define	KERN_ARND		37	/* int: from arc4rand() */
#define SDK_VERSION         38
#define DEVNAME             355
#define INIT_SAFE_MODE      419

/*
 * KERN_PROC subtypes
 */
#define	KERN_PROC_ALL		0	/* everything */
#define	KERN_PROC_PID		1	/* by process id */
#define	KERN_PROC_PGRP		2	/* by process group id */
#define	KERN_PROC_SESSION	3	/* by session of pid */
#define	KERN_PROC_TTY		4	/* by controlling tty */
#define	KERN_PROC_UID		5	/* by effective uid */
#define	KERN_PROC_RUID		6	/* by real uid */
#define	KERN_PROC_ARGS		7	/* get/set arguments/proctitle */
#define	KERN_PROC_PROC		8	/* only return procs */
#define	KERN_PROC_SV_NAME	9	/* get syscall vector name */
#define	KERN_PROC_RGID		10	/* by real group id */
#define	KERN_PROC_GID		11	/* by effective group id */
#define	KERN_PROC_PATHNAME	12	/* path to executable */
#define	KERN_PROC_OVMMAP	13	/* Old VM map entries for process */
#define	KERN_PROC_OFILEDESC	14	/* Old file descriptors for process */
#define	KERN_PROC_KSTACK	15	/* Kernel stacks for process */
#define	KERN_PROC_INC_THREAD	0x10	/*
					 * modifier for pid, pgrp, tty,
					 * uid, ruid, gid, rgid and proc
					 * This effectively uses 16-31
					 */
#define	KERN_PROC_VMMAP		32	/* VM map entries for process */
#define	KERN_PROC_FILEDESC	33	/* File descriptors for process */
#define	KERN_PROC_GROUPS	34	/* process groups */
#define	KERN_PROC_ENV		35	/* get environment */
#define	KERN_PROC_AUXV		36	/* get ELF auxiliary vector */
#define	KERN_PROC_RLIMIT	37	/* process resource limits */
#define	KERN_PROC_PS_STRINGS	38	/* get ps_strings location */
#define	KERN_PROC_UMASK		39	/* process umask */
#define	KERN_PROC_OSREL		40	/* osreldate for process binary */
#define	KERN_PROC_SIGTRAMP	41	/* signal trampoline location */
#define	KERN_PROC_CWD		42	/* process current working directory */
#define	KERN_PROC_NFDS		43	/* number of open file descriptors */

/*
 * KERN_IPC identifiers
 */
#define	KIPC_MAXSOCKBUF		1	/* int: max size of a socket buffer */
#define	KIPC_SOCKBUF_WASTE	2	/* int: wastage factor in sockbuf */
#define	KIPC_SOMAXCONN		3	/* int: max length of connection q */
#define	KIPC_MAX_LINKHDR	4	/* int: max length of link header */
#define	KIPC_MAX_PROTOHDR	5	/* int: max length of network header */
#define	KIPC_MAX_HDR		6	/* int: max total length of headers */
#define	KIPC_MAX_DATALEN	7	/* int: max length of data? */

/*
 * CTL_HW identifiers
 */
#define	HW_MACHINE	 1		/* string: machine class */
#define	HW_MODEL	 2		/* string: specific machine model */
#define	HW_NCPU		 3		/* int: number of cpus */
#define	HW_BYTEORDER	 4		/* int: machine byte order */
#define	HW_PHYSMEM	 5		/* int: total memory */
#define	HW_USERMEM	 6		/* int: non-kernel memory */
#define	HW_PAGESIZE	 7		/* int: software page size */
#define	HW_DISKNAMES	 8		/* strings: disk drive names */
#define	HW_DISKSTATS	 9		/* struct: diskstats[] */
#define	HW_FLOATINGPT	10		/* int: has HW floating point? */
#define	HW_MACHINE_ARCH	11		/* string: machine architecture */
#define	HW_REALMEM	12		/* int: 'real' memory */

/*
 * CTL_MACHDEP definitions
 */
#define CURR_MANUMODE                           1155
#define IDPS                                    1156
#define OPENPSID_FOR_SYS                        1157   
#define OPENPSID                                1158
#define HWFEATURE_FOR_DECID                     1159
/*missing a couple.*/
#define RCMGR_DEBUG_MENU                        1163
#define RCMGR_DEBUG_MENU_MINI                   1164
#define RCMGR_INTDEV                            1160
#define RCMGR_PSM_INTDEV                        1161
#define RCMGR_SL_DEBUGGER                       1162
#define RCMGR_FLAGED_UPDATER                    1165
#define RCMGR_FORCE_UPDATE                      1166
#define RCMGR_SPECIAL_I                         1167
#define RCMGR_FAKE_FINALIZE                     1168
#define RCMGR_UTOKEN_STORE_MODE                 1169
#define RCMGR_UTOKEN_DATA_EXECUTION             1170
#define RCMGR_UTOKEN_WEAKENED_PORT_RESTRICTION  1171
#define RCMGR_UTOKEN_FLAGED_UPDATER             1172
#define RCMGR_UTOKEN_NP_ENV_SWITCHING           1173
#define RCMGR_UTOKEN_SAVE_DATA_REPAIR           1174
#define RCMGR_UTOKEN_USE_SOFTWAGNER             1175
#define RCMGR_UTOKEN_NOTBEFORE                  1176
#define RCMGR_UTOKEN_NOTAFTER                   1177
/*missing a lot..*/
#define TSC_FREQ                                1226

/*
 * CTL_USER definitions
 */
#define	USER_CS_PATH		 1	/* string: _CS_PATH */
#define	USER_BC_BASE_MAX	 2	/* int: BC_BASE_MAX */
#define	USER_BC_DIM_MAX		 3	/* int: BC_DIM_MAX */
#define	USER_BC_SCALE_MAX	 4	/* int: BC_SCALE_MAX */
#define	USER_BC_STRING_MAX	 5	/* int: BC_STRING_MAX */
#define	USER_COLL_WEIGHTS_MAX	 6	/* int: COLL_WEIGHTS_MAX */
#define	USER_EXPR_NEST_MAX	 7	/* int: EXPR_NEST_MAX */
#define	USER_LINE_MAX		 8	/* int: LINE_MAX */
#define	USER_RE_DUP_MAX		 9	/* int: RE_DUP_MAX */
#define	USER_POSIX2_VERSION	10	/* int: POSIX2_VERSION */
#define	USER_POSIX2_C_BIND	11	/* int: POSIX2_C_BIND */
#define	USER_POSIX2_C_DEV	12	/* int: POSIX2_C_DEV */
#define	USER_POSIX2_CHAR_TERM	13	/* int: POSIX2_CHAR_TERM */
#define	USER_POSIX2_FORT_DEV	14	/* int: POSIX2_FORT_DEV */
#define	USER_POSIX2_FORT_RUN	15	/* int: POSIX2_FORT_RUN */
#define	USER_POSIX2_LOCALEDEF	16	/* int: POSIX2_LOCALEDEF */
#define	USER_POSIX2_SW_DEV	17	/* int: POSIX2_SW_DEV */
#define	USER_POSIX2_UPE		18	/* int: POSIX2_UPE */
#define	USER_STREAM_MAX		19	/* int: POSIX2_STREAM_MAX */
#define	USER_TZNAME_MAX		20	/* int: POSIX2_TZNAME_MAX */

#define	CTL_P1003_1B_ASYNCHRONOUS_IO		1	/* boolean */
#define	CTL_P1003_1B_MAPPED_FILES		2	/* boolean */
#define	CTL_P1003_1B_MEMLOCK			3	/* boolean */
#define	CTL_P1003_1B_MEMLOCK_RANGE		4	/* boolean */
#define	CTL_P1003_1B_MEMORY_PROTECTION		5	/* boolean */
#define	CTL_P1003_1B_MESSAGE_PASSING		6	/* boolean */
#define	CTL_P1003_1B_PRIORITIZED_IO		7	/* boolean */
#define	CTL_P1003_1B_PRIORITY_SCHEDULING	8	/* boolean */
#define	CTL_P1003_1B_REALTIME_SIGNALS		9	/* boolean */
#define	CTL_P1003_1B_SEMAPHORES			10	/* boolean */
#define	CTL_P1003_1B_FSYNC			11	/* boolean */
#define	CTL_P1003_1B_SHARED_MEMORY_OBJECTS	12	/* boolean */
#define	CTL_P1003_1B_SYNCHRONIZED_IO		13	/* boolean */
#define	CTL_P1003_1B_TIMERS			14	/* boolean */
#define	CTL_P1003_1B_AIO_LISTIO_MAX		15	/* int */
#define	CTL_P1003_1B_AIO_MAX			16	/* int */
#define	CTL_P1003_1B_AIO_PRIO_DELTA_MAX		17	/* int */
#define	CTL_P1003_1B_DELAYTIMER_MAX		18	/* int */
#define	CTL_P1003_1B_MQ_OPEN_MAX		19	/* int */
#define	CTL_P1003_1B_PAGESIZE			20	/* int */
#define	CTL_P1003_1B_RTSIG_MAX			21	/* int */
#define	CTL_P1003_1B_SEM_NSEMS_MAX		22	/* int */
#define	CTL_P1003_1B_SEM_VALUE_MAX		23	/* int */
#define	CTL_P1003_1B_SIGQUEUE_MAX		24	/* int */
#define	CTL_P1003_1B_TIMER_MAX			25	/* int */

#define	CTL_P1003_1B_MAXID	26

/*
 * Function:  getFirmware
 * --------------------
 * Get the firmware version
 *
 *	client:		The client data.
 *	firmware:	The buffer to put the firmware id into.
 *	length:		The length variable to put the length of the firmware id into.
 *
 *  returns: RPC Error code
 */
uint8_t getFirmware(struct clientArgs* client, uint8_t** firmware, uint32_t* length);
