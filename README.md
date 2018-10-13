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

| Function             | Key                  | Description                                                 |
| -------------------- | -------------------- | ----------------------------------------------------------- |
| poke()               | POKE                 | Write a buffer to memory at a given addrss                  |
| peek()               | PEEK                 | Read a buffer from memory at a given address                |
| getProcesses()       | GET_PROCESSES        | Get a list of running processes                             |
| getModules()         | GET_MODULES          | Get a list of running modules                               |
| getRegions()         | GET_REGIONS          | Get regions for a given process                             |
| getFirmware()        | GET_FIRMWARE         | Get the firmware version                                    |
| notify()             | NOTIFY               | Send a system notification                                  |
| searchStart()        | SEARCH_START         | Start searching for data                                    |
| searchRescan()       | SEARCH_RESCAN        | Rescan results from a previous search for a new set of data |
| searchGetResults()   | SEARCH_GET_RESULTS   | Get the current list of results                             |
| searchCountResults() | SEARCH_COUNT_RESULTS | Get the number of results                                   |
| searchEnd()          | SEARCH_END           | End the search                                              |

Thanks to 2much4u, CTurt, Kiwidog and Golden for sources / advice.

Compiled using PS4 SDK - https://github.com/McCaulay/ps4-payload-sdk