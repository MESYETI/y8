# I/O

## IO memory map
| Address   | Description      |
| --------- | ---------------- |
| C000-CFFF | Y8210 video chip |
| D000-DFFF | Y8110 IO chip    |
| E000-EFFF | Y8310 audio chip |
| F000-FFFF | Expansion port   |

## Video memory
The first 3 kilobytes store what characters are stored on screen. The final kilobyte stores
the font.

So, characters are stored from `5000` to `5C00`. And the font is stored from `5C00` to `6000`

The Y8110 chip implements I/O on the yeti-8.

## Y8110 IO chip
The Y8110 has a 4-bit address bus for 16 8-bit registers.

| Register | Description           |
| -------- | --------------------- |
| 0        | Port A                |
| 1        | Port B                |
| 2        | Port A direction      |
| 3        | Port B direction      |
| 4        | Timer A low           |
| 5        | Timer A high          |
| 6        | Timer B low           |
| 7        | Timer B high          |
| 8        |                       |
| 9        |                       |
| A        |                       |
| B        |                       |
| C        | Serial shift register |
| D        | Interrupt control     |
| E        | Control timer A       |
| F        | Control timer B       |

Serial and timers are currently unimplemented.

### Port direction registers
The port direction registers are a bitmask where 0 = read and 1 = write.
