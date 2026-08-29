# Y8210 video chip
The Y8210 video chip has a 12 bit address bus. Unused space in its memory map is not usable
as memory.

| Address   | Description      |
| --------- | ---------------- |
| C000-C2FF | Character buffer |
| C300-CBFF | Unused           |
| CC00-CFFF | Character set    |

Characters in the character set are 8x8. Each byte in a font's character is a row.

# Y8211 video chip
The Y8211 is a higher end upgrade to the Y8210 which adds 4 bit RGBI (where I is intensity)
colour. It uses this new memory map:

| Address   | Description      |
| --------- | ---------------- |
| C000-C2FF | Character buffer |
| C300-C5FF | Colour buffer    |
| C600-CBFF | Unused           |
| CC00-CFFF | Character set    |

Each cell has a foreground and a background colour stored in the colour buffer, where the
high nibble is the foreground and the low nibble is the background.

## Colour format
| Bit | Value |
| --- | ----- |
| 1   | I     |
| 2   | B     |
| 3   | G     |
| 4   | R     |
