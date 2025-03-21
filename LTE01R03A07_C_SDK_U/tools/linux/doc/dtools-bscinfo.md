# dtools bscinfo

This will parse *blue screen core* dump file, and generate `.cmm` file
for debugging. Also, the bin file can be passed to `dtools gdbserver` for
launching gdb.

When target enters blue screen, it will write some information into reserved
memory. There are several user cases:

* Dump the reserved memory online.
* At next boot without power loss, the memory will be kept. Application can
  write the memory into file system, or send to server after network is ready.

During parsing, it will:

* Show software version, if encountered.
* Show exception information, if encountered.
* Show heap basic information, if encountered.
* Show heap memory block error information, if encountered.
* Show memory allocate and free information, if encountered and `-d v` option
  is specified.
* Show SVC stack usage, if encountered.
* Show thread stack usage, if encountered.
* Write profile data into `profile.bin`, if encountered.
* Write memory blocks into `nnnnnnnn.bin`. The file name is the address of
  memory block.
* Write `ap_regs.txt` for 8910 or 8850 AP registers, if encountered.
* Write `ap.cmm`, if 8910 or 8850 AP register block or 8811 register block is
  encountered.
* Write `cp.cmm`, if 8910 or 8850 CP register block is encountered.

`profile.bin` can be converted to `.prf` file by `dtools bin2prf`. And `.prf`
can be opened by coolprofile tool.

## Dump after `halClearBscoreBuf` is called

Inside `halClearBscoreBuf`, only the magic word will be cleared and other
contents aren't touched. So, after `halClearBscoreBuf` is called,
`halGetBscoreBuf` will return an invalid buffer, and the memory can still
be parsed by `dtools bscinfo`.

At parsing, and it is found that the magic word is missing, this utility will
try to verify the content without the magic word. When no other errors are
found, it will treat it as a valid dump, and the following line will be shown:

    bscore magic in header is cleared, continue anyway

## Launch GDB

Example to launch gdb based on bscore.bin:

```{.shell}
$ dtools gdbserver --bscore bscore.bin --mode 8910AP \
    --launch arm-none-eabi-gdb ap.elf
```

It is possible that stack is not fully dump inside bscore.bin, and then it
is possible the backtrace is incomplete.

Example to get backtrace in batch mode:

```{.shell}
$ dtools gdbserver --bscore bscore.bin --mode 8910AP \
    --launch arm-none-eabi-gdb \
    --gdbcmd "set confirm off" \
    --gdbcmd "set pagination off" \
    --gdbcmd "bt f" \
    --gdbcmd "quit" \
    ap.elf
```

## Blue Screen Core Data Format

The *blue screen core* data are separated into multiple blocks, with a 12
bytes header. The header format is:

```
4B: magic, 'BSCR'
4B: total size
4B: crc, except the 12 bytes header
(blocks)
```

The format of the first 8 bytes of each block is the same:

```
4B: type
4B: block size. When block size is not 4 bytes aligned, next block
    will be started at 4 bytes aligned location
```

### Blue Screen Core End Block

```
4B: type, value is 0
4B: block size
8B: "COREEND"
```

### SWVER Block

```
4B: type, value is 1
4B: block size
(variable size): software version string
```

### Memory Block

```
4B: type, value is 2
4B: block size
4B: flags, reserved now
4B: memory address
(variable size): memory content
```

### Profile Data

```
4B: type, value is 3
4B: block size
(variable size): profile data
```

### Exception Information

```
4B: type, value is 4
4B: block size
(variable size): exception information string
```

### CP Exception Information

```
4B: type, value is 5
4B: block size
(variable size): CP exception information string
```

### Heap Memory Block Error Information

```
4B: type, value is 6
4B: block size
(variable size): 12 bytes per record, (address, caller, flags)
```

### Memory Allocate/Free Information

```
4B: type, value is 7
4B: block size
(variable size): 8 bytes per record, (caller, address)
```

### Heap Information

```
4B: type, value is 8
4B: block size
(variable size): 20 bytes per record, (start, size, index, avail, max_block)
```

### SVC Stack Remained Information

```
4B: type, value is 9
4B: block size
4B: remained bytes
```

### Thread Stack Remained Information

```
4B: type, value is 10
4B: block size
(variable size): 12 bytes per record, (name[8], remained)
```

### ARM v7a AP Registers

```
4B: type, value is 0x40
4B: block size
(registers, refer to hal_blue_screen_imp.h for definition)
```

### ARM v7a CP Registers

```
4B: type, value is 0x41
4B: block size
(registers, refer to hal_blue_screen_imp.h for definition)
```

### ARM v8m AP Registers

```
4B: type, value is 0x42
4B: block size
(registers, refer to hal_blue_screen_imp.h for definition)
```
