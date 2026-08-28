# Memory map

With expanded RAM:
| Address     | Description |
| ----------- | ----------- |
| 0000-3FFF   | ROM         |
| 4000-7FFF   | Cartridge   |
| 8000-BFFF   | RAM         |
| C000-FFFF   | I/O         |

Without expanded RAM:
| Address     | Description    |
| ----------- | -------------- |
| 0000-3FFF   | ROM            |
| 4000-7FFF   | Cartridge      |
| 8000-8FFF   | RAM            |
| 9000-9FFF   | RAM (mirrored) |
| A000-AFFF   | RAM (mirrored) |
| B000-BFFF   | RAM (mirrored) |
| C000-FFFF   | I/O            |

## IO memory map
| Address   | Description |
| --------- | ----------- |
| C000-CFFF | VRAM        |
| D000-DFFF | Unused      |
| E000-EFFF | Unused      |
| F000-FFFF | Unused      |

# Video memory
The first 3 kilobytes store what characters are stored on screen. The final kilobyte stores
the font.

So, characters are stored from `5000` to `5C00`. And the font is stored from `5C00` to `6000`
