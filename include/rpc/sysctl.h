#pragma once

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
 * Function:  getFirmware
 * --------------------
 * Get the firmware version
 *
 *  returns: RPC Error code
 */
int getFirmware(char* firmware, int* length);