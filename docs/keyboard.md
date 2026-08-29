# Keyboard

## Layout
First line = column (except for SP and arrow keys, which are in column 7)

```
   1     2     3     4     5      6

1  2  3  4  5  6  7  8  9  =  -  BK
CT Q  W  E  R  T  Y  U  I  O  P  AP
   A  S  D  F  G  H  J  K  L  SC RT    AU
SH Z  X  C  V  B  N  M  CM DT SL SS AL AD AR
            SP
```

- `BK` = Backspace
- `CT` = Control
- `AP` = Apostrophe
- `SC` = Semicolon
- `RT` = Return
- `CM` = Comma
- `DT` = Dot
- `SL` = Slash
- `SH` = Shift
- `SS` = Symbol Shift
- `SP` = Space

## Data layout

Column 1

| Bit | Key |
| --- | --- |
| 1   | 1   |
| 2   | CT  |
| 3   |     |
| 4   | SH  |
| 5   | 2   |
| 6   | Q   |
| 7   | A   |
| 8   | Z   |

Column 2

| Bit | Key |
| --- | --- |
| 1   | 3   |
| 2   | W   |
| 3   | S   |
| 4   | X   |
| 5   | 4   |
| 6   | E   |
| 7   | D   |
| 8   | C   |

Column 3

| Bit | Key |
| --- | --- |
| 1   | 5   |
| 2   | R   |
| 3   | F   |
| 4   | V   |
| 5   | 6   |
| 6   | T   |
| 7   | G   |
| 8   | B   |

Column 4

| Bit | Key |
| --- | --- |
| 1   | 7   |
| 2   | Y   |
| 3   | H   |
| 4   | N   |
| 5   | 8   |
| 6   | U   |
| 7   | J   |
| 8   | M   |

Column 5

| Bit | Key |
| --- | --- |
| 1   | 9   |
| 2   | I   |
| 3   | K   |
| 4   | CM  |
| 5   | =   |
| 6   | O   |
| 7   | L   |
| 8   | DT  |

Column 6

| Bit | Key |
| --- | --- |
| 1   | -   |
| 2   | P   |
| 3   | SC  |
| 4   | SL  |
| 5   | BK  |
| 6   | AP  |
| 7   | RT  |
| 8   | SS  |

Column 7

| Bit | Key |
| --- | --- |
| 1   | AL  |
| 2   | AU  |
| 3   | AD  |
| 4   | AR  |
| 5   | SP  |
| 6   |     |
| 7   |     |
| 8   |     |

## Interface
Write to port B on the IO chip to select the column to read from, using these values:

| Column | Value |
| ------ | ----- |
| 1      | 1     |
| 2      | 2     |
| 3      | 4     |
| 4      | 8     |
| 5      | 16    |
| 6      | 32    |
| 7      | 64    |

Read from port A to read a bitmask of the keys pressed on that column.
