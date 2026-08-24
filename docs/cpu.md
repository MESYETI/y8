# CPU instruction set architecture

## 8-bit Registers
| Number | Name       | Use             |
| ------ | ---------- | --------------- |
| 0      | A          | General purpose |
| 1      | B          | General purpose |
| 2      | C          | General purpose |
| 3      | D          | General purpose |
| 4      | E          | General purpose |
| 5      | F          | General purpose |
| 6      | G          | General purpose |
| 7      | H          | General purpose |

## 16-bit Registers
| Number | Name       | Use             |
| ------ | ---------- | --------------- |
| 0      | AB         | General purpose |
| 1      | CD         | General purpose |
| 2      | EF         | General purpose |
| 3      | SP         | Stack pointer   |

A Program Counter (PC) is also used.

## Flag register
Flags:
- (Z) Zero
- (S) Sign
- (C) Carry

## Instructions

#### Notation
- `Rd`  - 8-bit register destination
- `Rs`  - 8-bit register source
- `N8`  - 8-bit integer literal
- `N16` - 16-bit integer literal
- `Pd`  - 16-bit register destination
- `Ps`  - 16-bit register source

`Rd` and `Rs` each store 3 bits.

`N8` stores 8 bits.

`N16` stores 16 bits.

`Pd` and `Ps` each store 2 bits.

| Opcode     | Parameter format         | Name | Description                    | Flags |
| ---------- | ------------------------ | ---- | ------------------------------ | ----- |
| `00000000` |                          | HALT | Stops the computer             |       |
| `00010000` | `Rd Rs 00`               | MOV  | `Rd <- Rs`                     | ZS    |
| `00010001` | `Pd Ps 0000`             | MOV  | `Pd <- Ps`                     | ZS    |
| `00010010` | `Rd 00000 N8`            | MOV  | `Rd <- N8`                     | ZS    |
| `00010011` | `Pd 000000 N16`          | MOV  | `Pd <- N16`                    | ZS    |
| `00010100` | `Rd Ps 000`              | MOV  | `Rd <- [Ps]`                   | ZS    |
| `00010101` | `Pd Ps 0000`             | MOV  | `Pd <- [Ps]`                   | ZS    |
| `00010111` | `Rd 00000 N16`           | MOV  | `Rd <- [N16]`                  | ZS    |
| `00011000` | `Pd 000000 N16`          | MOV  | `Pd <- [N16]`                  | ZS    |
| `00100000` | `N16`                    | JMP  | `PC <- N16`                    |       |
| `00100001` | `N16`                    | JMP  | `PC <- [N16]`                  |       |
| `00100010` | `Ps 000000`              | JMP  | `PC <- Ps`                     |       |
| `00100011` | `Ps 000000`              | JMP  | `PC <- [Ps]`                   |       |
| `00100100` | `N16`                    | CALL | `push(PC), PC <- N16`          |       |
| `00100101` | `N16`                    | CALL | `push(PC), PC <- [N16]`        |       |
| `00100110` | `Ps 000000`              | CALL | `push(PC), PC <- Ps`           |       |
| `00100111` | `Ps 000000`              | CALL | `push(PC), PC <- [Ps]`         |       |
| `00101000` | `N16`                    | JZ   | `PC <- N16`                    |       |
| `00101001` | `N16`                    | JNZ  | `PC <- N16`                    |       |
| `00101010` | `N16`                    | JS   | `PC <- N16`                    |       |
| `00101011` | `N16`                    | JNS  | `PC <- N16`                    |       |
| `00101100` | `N16`                    | JC   | `PC <- N16`                    |       |
| `00101101` | `N16`                    | JNC  | `PC <- N16`                    |       |
| `00101110` |                          | RET  | `PC <- pop()`                  |       |
| `01000000` | `Rd Rs 00`               | CMP  | `cmp(Rd, Rs)`                  | ZSC   |
| `01000001` | `Rd 00000 N8`            | CMP  | `cmp(Rd, N8)`                  | ZSC   |
| `01000010` | `Pd Ps 0000`             | CMP  | `cmp(Pd, Ps)`                  | ZSC   |
| `01000011` | `Rd Rs 00`               | ADD  | `Rd <- Rd + Rs`                | ZSC   |
| `01000100` | `Rd Rs 00`               | SUB  | `Rd <- Rd - Rs`                | ZS    |
| `01000101` | `Rd Rs 00`               | MUL  | `Rd <- Rd * Rs`                | ZS    |
| `01000110` | `Rd Rs 00`               | DIV  | `Rd <- Rd / Rs`                | ZS    |
| `01000111` | `Pd Rs 000`              | ADD  | `Pd <- Pd + Rs`                | ZSC   |
| `01001000` | `Pd Rs 000`              | SUB  | `Pd <- Pd - Rs`                | ZS    |
| `01001001` | `Rd Rs 00`               | AND  | `Rd <- Rd & Rs`                | ZS    |
| `01001010` | `Rd Rs 00`               | OR   | `Rd <- Rd | Rs`                | ZS    |
| `01001011` | `Rd Rs 00`               | XOR  | `Rd <- Rd ^ Rs`                | ZS    |
| `01001100` | `Rd`                     | NOT  | `Rd <- ~Rd`                    | ZS    |
| `01010000` |                          | SETZ | `Z = 1`                        | Z     |
| `01010001` |                          | CLZ  | `Z = 0`                        | Z     |
| `01010010` |                          | SETS | `S = 1`                        | S     |
| `01010011` |                          | CLS  | `S = 0`                        | S     |
| `01010100` |                          | SETC | `C = 1`                        | C     |
| `01010101` |                          | CLC  | `C = 0`                        | C     |

### Compare instruction
Sets `Z` for equality, `S` for lesser than, and `C` for greater than.
