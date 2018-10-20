# PS4 MEMAPI Server

## Debugging
Edit the `include/defines.h` file to change the debug IP address and port to your PC.
```c
#define DEBUG 1
#define DEBUG_IP "192.168.0.38"
#define DEBUG_PORT 9023
```

## RPC
Available Remote Functions

| Function                 | Key                       | Description                                                 |
| ------------------------ | ------------------------- | ----------------------------------------------------------- |
| poke()                   | POKE                      | Write a buffer to memory at a given addrss                  |
| peek()                   | PEEK                      | Read a buffer from memory at a given address                |
| getProcesses()           | GET_PROCESSES             | Get a list of running processes                             |
| getProcessThreads()      | GET_PROCESS_THREADS       | Get the threads the process has                             |
| getModules()             | GET_MODULES               | Get a list of running modules                               |
| getRegions()             | GET_REGIONS               | Get regions for a given process                             |
| getFirmware()            | GET_FIRMWARE              | Get the firmware version                                    |
| notify()                 | NOTIFY                    | Send a system notification                                  |
| searchStart()            | SEARCH_START              | Start searching for data                                    |
| searchRescan()           | SEARCH_RESCAN             | Rescan results from a previous search for a new set of data |
| searchGetResults()       | SEARCH_GET_RESULTS        | Get the current list of results                             |
| searchCountResults()     | SEARCH_COUNT_RESULTS      | Get the number of results                                   |
| searchEnd()              | SEARCH_END                | End the search                                              |
| debugContinue()          | DEBUG_CONTINUE            | Continue a stopped process                                  |
| debugStop()              | DEBUG_STOP                | Stop a process                                              |
| debugKill()              | DEBUG_KILL                | Kill a process                                              |
| debugStep()              | DEBUG_STEP                | Step one instruction on a stopped process                   |
| debugGetRegisters()      | DEBUG_GET_REGISTERS       | Get a processes registers                                   |
| debugSetRegisters()      | DEBUG_SET_REGISTERS       | Set a processes registers                                   |
| debugGetFloatRegisters() | DEBUG_GET_FLOAT_REGISTERS | Get a processes floating point registers                    |
| debugSetFloatRegisters() | DEBUG_SET_FLOAT_REGISTERS | Set a processes floating point registers                    |
| debugGetDebugRegisters() | DEBUG_GET_DEBUG_REGISTERS | Get a processes debug registers                             |
| debugSetDebugRegisters() | DEBUG_SET_DEBUG_REGISTERS | Set a processes debug registers                             |
| debugAddBreakpoint()     | DEBUG_ADD_BREAKPOINT      | Add a breakpoint to the process                             |
| debugRemoveBreakpoint()  | DEBUG_REMOVE_BREAKPOINT   | Remove a breakpoint from the process                        |
| debugAddWatchpoint()     | DEBUG_ADD_WATCHPOINT      | Add a watch to the process                                  |
| debugRemoveWatchpoint()  | DEBUG_REMOVE_WATCHPOINT   | Remove a watch from the process                             |
| debugStopThread()        | DEBUG_STOP_THREAD         | Stop a given thread from running                            |
| debugResumeThread()      | DEBUG_RESUME_THREAD       | Resume a given thread that was previously stopped           |
| debugCheckInterrupt()    | DEBUG_CHECK_INTERRUPT     | Check if there is a interrupt                               |


Thanks to 2much4u, CTurt, Kiwidog and Golden for sources / advice.

Compiled using PS4 SDK - https://github.com/McCaulay/ps4-payload-sdk