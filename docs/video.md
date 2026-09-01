# Y8210 video chip
The Y8210 video chip has a 12 bit address bus. Unused space in its memory map is not usable
as memory.

| Address   | Description      |
| --------- | ---------------- |
| C000-C0FF | Registers        |
| C100-C3FF | Character buffer |
| C400-CBFF | Unused           |
| CC00-CFFF | Character set    |

Characters in the character set are 8x8. Each byte in a font's character is a row.

## Registers
| Address   | Description      |
| --------- | ---------------- |
| C000      | Border colour    |
| C001      | Raster counter   |
| C002      | Interrupt flag   |

Bit 0 of the border colour register controls the border colour. 0 is black and 1 is white.
This register is also write-only.

# Y8211 video chip
The Y8211 is a higher end upgrade to the Y8210 which adds 4 bit RGBI (where I is intensity)
colour, and 128 more characters in the font. It uses this new memory map:

| Address   | Description              |
| --------- | ------------------------ |
| C000-C0FF | Registers                |
| C100-C3FF | Character buffer         |
| C400-C6FF | Colour buffer            |
| C700-C7FF | Unused                   |
| C800-CBFF | Character set 1st half   |
| CC00-CFFF | Character set 2nd half   |

Each cell has a foreground and a background colour stored in the colour buffer, where the
high nibble is the foreground and the low nibble is the background.

## Colour format
| Bit | Value |
| --- | ----- |
| 1   | I     |
| 2   | B     |
| 3   | G     |
| 4   | R     |

## Registers
| Address   | Description      |
| --------- | ---------------- |
| C000      | Border colour    |
| C001      | Raster counter   |
| C002      | Interrupt flag   |
| C010      | 8211 flags       |

The border colour has been upgraded to an RGBI value, with the same colour format as
used in the colour buffer.

Write to the raster counter to set which line should send an interrupt. Write 1 (enable) or
0 (disable) to the interrupt flag to set whether interrupts should be sent when a certain
line is reached.

### 8211 flags
| Bit | Flag                          |
| --- | ----------------------------- | 
| 0   | Enable colour                 |
| 1   | Enable 2nd character set half |

If the 2nd character set half is enabled, then the character set begins at `C800`. If it
is not enabled, the character set begins at `CC00`.
