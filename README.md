# YETI-8
YETI-8 is an incomplete fantasy computer inspired by the ZX81. It features several non-existent custom
chips:

### 8010 CPU
- 8-bit
- Eight 8-bit registers, 6 of which can be put into 16-bit pairs
- 2 MHz
### 8110 IO
- Supposed to be a cheaper clone of the MOS 6526 without the time of day clock
- 2 parallel ports (both used by the yeti-8 keyboard)
- One serial port (going to be used by a tape drive, floppy drive, or a printer)
- 2 timers
### 8210 video chip
- 12 bit address bus
- 32x24 character display with 8x8 cells
- Modifiable character set containing 128 characters
### 8211 video chip
- Used in the higher end YETI-8 Mk2
- Adds 4-bit RGBI (where I is intensity), where each cell can have a foreground and
  background colour
### 8310 audio chip
- Features to be defined in the future
### 8410 16K ROM
- 14 bit address bus
- Used in the YETI-8 to store the operating system, named Monitor
### 8510 4K RAM
- YETI-8 has 4 sockets for these chips, making it possible to have up to 16 kilobytes
  of RAM internally

## Build
#### Emulator
```
cd emu
make
```

Requires SDL2

#### ROM
```
cd rom
make
```

Requires `customasm`

To use the ROM in the emulator, put the `rom.bin` in the `emu` folder
