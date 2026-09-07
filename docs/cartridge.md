# Cartridge

Cartridges have 16 kilobytes of addressable memory. Support for banking may be added in
the future.

## Header
Cartridges have a 16 byte header at the start. This means the ROM jumps to `0x4010` to run
a cartridge. The header uses this format

| Offset | Size | Description                                           |
| ------ | ---- | ----------------------------------------------------- |
| 0      | 2    | Magic bytes: 0x59 0x38                                |
| 2      | 14   | Cartridge name, null terminated if length is below 14 |
