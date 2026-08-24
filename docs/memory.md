# Memory map

| Address   | Description               |
| --------- | ------------------------- |
| 0000-4000 | ROM                       |
| 4000-5000 | RAM                       |
| 5000-6000 | VRAM                      |
| 6000-7000 | RAM - mirrored            |
| 7000-8000 | VRAM - mirrored           |
| 8000-FFFF | Cartridge                 |

## Ranges in binary
```
ROM:  0000 0000 0000 0000 to 0100 0000 0000 0000

RAM:  0100 0000 0000 0000 to 0101 0000 0000 0000

VRAM: 0101 0000 0000 0000 to 0110 0000 0000 0000

RAM:  0110 0000 0000 0000 to 0111 0000 0000 0000

VRAM: 0111 0000 0000 0000 to 1000 0000 0000 0000

Cart: 1000 0000 0000 0000 to end
```

# Video memory
The first 3 kilobytes store what characters are stored on screen. The final kilobyte stores
the font.

So, characters are stored from `5000` to `5C00`. And the font is stored from `5C00` to `6000`
