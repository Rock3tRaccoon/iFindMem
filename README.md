# iFindMem

iFindMem is a RE/Mem scanner framework. iFindMem comes as an app that is installed to your iDevice and a binary which can be executed by typing 'iFindMem' in a Terminal app or over SSH. SearchKit (SearchKit/SearchKit.h) is also included for developers to use to add memory searching in their own projects. The documentation for SearchKit can be found below along with example code in SearchKit/example.c which shows iFindMem finding instances of MH_MAGIC_64 (feedfacf) in memory.

iFindMem boilerplate was taken from PsychoBird's RevelariOS. RevelariOS is no longer maintained. 

## iFindMem Features:

- Search memory for instances of bytes / strings in memory
- View found addresses
- Write bytes / strings to memory
- Read lines of memory as bytes / characters (CLI only currently)
- Pause / Resume current task

### Signing iFindMem

iFindMem isn't running? Make sure to sign iFindMem with ent.xml! iFindMem can't operate without proper entitlements.

Use `ldid` for signing iFindMem. Exactly as typed and as root type `ldid -Sent.xml /path/to/iFindMem`

## Documentation - SearchKit.h

`kern_return_t get_region_size(mach_port_t task, vm_address_t *baseaddr, vm_address_t *endaddr)`
- Gets memory region for searching
- **mach_port_t task** - task for the process that will be searched
- **vm_address_t \*baseaddr** - base address found by get_region_size (out)
- **vm_address_t \*endaddr** - end address found by get_region_size (out)
- **RETURN** - KERN_SUCCESS / KERN_FAILURE

`search_t search_data(mach_port_t task, bool isString, bool quitOnFirstResult, vm_address_t baseaddr, vm_address_t end, vm_address_t *outaddr, char in[100])`
- Searches for the provided data
- **mach_port_t task** - task for the process that will be searched
- **bool isString** - boolean for wether the input data is bytes or simply a string
- **bool quitOnFirstResult** - SearchKit will end searching after the first result is found
- **vm_address_t baseaddr** - base address for searching. Use `*baseaddr` from `get_region_size`
- **vm_address_t endaddr** - end address for searching. Use `*endaddr` from `get_region_size`
- **vm_address_t  \*outaddr[SEARCH_MAX]** - address where the searched data is found (out)
- **result_t \*resultnum** - amount of results found (out)
- **char in[MAX_INPUT_DATA]** - the data to be searched. if `isString = true`, `search_data` will search for a string. The input string should be `"ABCDE"`. If `isString = false`, `search_data` will search for the provided bytes. The input bytes should be `"6269742e6c792f3368476634696d"`
- **RETURN** - SEARCH_SUCCESS / SEARCH_FAILURE / BYTES_UNEVEN / DATA_TOO_LARGE

`search_t write_data(mach_port_t task, bool isString, vm_address_t addr, char in[100])`
- **mach_port_t task** - task for the process that will be searched
- **bool isString** - boolean for wether the input data is bytes or simply a string
- **vm_address_t addr** - address to write data to
- **char in[MAX_INPUT_DATA]** - the data to be searched. if `isString = true`, `search_data` will search for a string. The input string should be `"ABCDE"`. If `isString = false`, `search_data` will search for the provided bytes. The input bytes should be `"6269742e6c792f3368476634696d"`
- **RETURN** - WRITE_SUCCESS / WRITE_FAILURE / WRITE_BAD_ADDRESS / BYTES_UNEVEN / DATA_TOO_LARGE

### CONSTANTS

`SEARCH_MAX ((result_t) (~0 >> 1)) + 1` - the max amount of search results supported. The size is the max `result_t` + 1 - 256 by default. Change `typedef result_t` to increase max result size.

`READ_PAGE_SIZE getpagesize()` - page size of the device

`MAX_INPUT_DATA 100` - max number of supported characters to input to `write_data` / `search_data`. WARNING: Must be even!

`SEARCH_SUCCESS 0` - search was successful

`SEARCH_FAILURE 1` - search failed

`BYTES_UNEVEN 2` - bytes attempted to be searched were uneven: ex: "41b". Use "410b" instead

`DATA_TOO_LARGE 3` - input data is too large for allocated size (standard - 100)

`WRITE_SUCCESS 4` - write was successful

`WRITE_FAILURE 5` - write failed

`WRITE_BAD_ADDRESS 6` - address to write to is invalid



### Typedefs

`search_t (int)`

`byte_t (unsigned char)`

`result_t (uint8_t)`

![iFindMem](Pics/iFindMem.png)
![iFindMem App](Pics/iFindMemapp.png)

