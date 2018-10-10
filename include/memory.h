#pragma once

#define VMMAP_GAP 0x1000000

/*
 * Struct:  inputPeek
 * --------------------
 *
 *	processId:	4 Bytes | The process id.
 *	address:	8 Bytes | The memory address you want to read.
 *	length:		4 Bytes | The number of bytes you want to read from the address.
 */
struct inputPeek;

/*
 * Struct:  inputPoke
 * --------------------
 *
 *	processId:	4 Bytes | The process id.
 *	address:	8 Bytes | The memory address you want to read.
 *	length:		4 Bytes | The number of bytes you want to read from the address.
 */
struct inputPoke;

/*
 * Struct:  inputRegions
 * --------------------
 *
 *	processId:	4 Bytes | The process id.
 */
struct inputRegions;

/*
 * Struct:  kinfo_vmentry
 */
typedef struct {
  int      kve_structsize;                /* Variable size of record. */
  int      kve_type;                      /* Type of map entry. */
  uint64_t kve_start;                     /* Starting address. */
  uint64_t kve_end;                       /* Finishing address. */
  uint64_t kve_offset;                    /* Mapping offset in object */
  uint64_t kve_vn_fileid;                 /* inode number if vnode */
  uint32_t kve_vn_fsid;                   /* dev_t of vnode location */
  int      kve_flags;                     /* Flags on map entry. */
  int      kve_resident;                  /* Number of resident pages. */
  int      kve_private_resident;          /* Number of private pages. */
  int      kve_protection;                /* Protection bitmask. */
  int      kve_ref_count;                 /* VM obj ref count. */
  int      kve_shadow_count;              /* VM obj shadow count. */
  int      kve_vn_type;                   /* Vnode type. */
  uint64_t kve_vn_size;                   /* File size. */
  uint32_t kve_vn_rdev;                   /* Device id if device. */
  uint16_t kve_vn_mode;                   /* File mode. */
  uint16_t kve_status;                    /* Status flags. */
  int      _kve_ispare[12];               /* Space for more stuff. */
  /* Truncated before copyout in sysctl */
  char     kve_path[256];            /* Path to VM obj, if any. */
} kinfo_vmentry;

/*
 * Function:  peek
 * --------------------
 * Read data from memory for a given process.
 *
 *	ip:				The client IP.
 *	outputBuffer:	The buffer to put the peeked data into.
 *	outputLength:	The length variable to put the length of the peeked data into.
 *	inputBuffer:	The input buffer containing the arguments. (Process, Address, Length)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
int peek(char* ip, unsigned char** outputBuffer, int* outputLength, unsigned char* inputBuffer, int inputLength);

/*
 * Function:  poke
 * --------------------
 * Write data to memory for a given process.
 *
 *	ip:				The client IP.
 *	inputBuffer:	The input buffer containing the arguments. (Process, Address, Length, Data)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
int poke(char* ip, unsigned char* inputBuffer, int inputLength);

/*
 * Function:  getRegions
 * --------------------
 * Get the memory regions for a given process.
 *
 *	ip:				The client IP.
 *	outputBuffer:	The buffer to put the regions into.
 *	outputLength:	The length variable to put the length of the regions into.
 *	inputBuffer:	The input buffer containing the arguments. (Process)
 *	inputLength:	The total length of the input buffer.
 *
 *  returns: RPC Error code
 */
int getRegions(char* ip, unsigned char** outputBuffer, int* outputLength, unsigned char* inputBuffer, int inputLength);

/*
 * Function:  getVirtualMemoryMaps
 * --------------------
 * Get the virtual memory regions for a given process.
 * Note: You must free the returned pointer after use.
 *
 *	processId:	The process id.
 *	entries:	The pointer to array of virtual memory maps
 *	length:		The pointer to put the buffer size into.
 *
 *  returns: 0 On success, -1 on error
 */
int getVirtualMemoryMaps(int processId, kinfo_vmentry** entries, size_t* length);
