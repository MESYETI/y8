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

| Opcode (hex) | Opcode (bin) | Parameter format         | Name | Description                    | Flags |
| ------------ | ------------ | ------------------------ | ---- | ------------------------------ | ----- |
| `00`         | `00000000`   |                          | HALT | Stops the computer             |       |
| `10`         | `00010000`   | `Rd Rs 00`               | MOV  | `Rd <- Rs`                     | ZS    |
| `11`         | `00010001`   | `Pd Ps 0000`             | MOV  | `Pd <- Ps`                     | ZS    |
| `12`         | `00010010`   | `Rd 00000 N8`            | MOV  | `Rd <- N8`                     | ZS    |
| `13`         | `00010011`   | `Pd 000000 N16`          | MOV  | `Pd <- N16`                    | ZS    |
| `14`         | `00010100`   | `Rd Ps 000`              | MOV  | `Rd <- [Ps]`                   | ZS    |
| `15`         | `00010101`   | `Pd Ps 0000`             | MOV  | `Pd <- [Ps]`                   | ZS    |
| `17`         | `00010111`   | `Rd 00000 N16`           | MOV  | `Rd <- [N16]`                  | ZS    |
| `18`         | `00011000`   | `Pd 000000 N16`          | MOV  | `Pd <- [N16]`                  | ZS    |
| `19`         | `00011001`   | `Pd Rs 000`              | MOV  | `[Pd] <- Rs`                   | ZS    |
| `1A`         | `00011010`   | `Pd Ps 000`              | MOV  | `[Pd] <- Ps`                   | ZS    |
| `1B`         | `00011010`   | `Pd 000000 N8`           | MOV  | `[Pd] <- N8`                   | ZS    |
| `1C`         | `00011011`   | `Pd 000000 N16`          | MOV  | `[Pd] <- N16`                  | ZS    |
| `20`         | `00100000`   | `N16`                    | JMP  | `PC <- N16`                    |       |
| `21`         | `00100001`   | `N16`                    | JMP  | `PC <- [N16]`                  |       |
| `22`         | `00100010`   | `Pd 000000`              | JMP  | `PC <- Pd`                     |       |
| `23`         | `00100011`   | `Pd 000000`              | JMP  | `PC <- [Pd]`                   |       |
| `24`         | `00100100`   | `N16`                    | CALL | `push(PC), PC <- N16`          |       |
| `25`         | `00100101`   | `N16`                    | CALL | `push(PC), PC <- [N16]`        |       |
| `26`         | `00100110`   | `Pd 000000`              | CALL | `push(PC), PC <- Pd`           |       |
| `27`         | `00100111`   | `Pd 000000`              | CALL | `push(PC), PC <- [Pd]`         |       |
| `28`         | `00101000`   | `N16`                    | JZ   | `PC <- N16`                    |       |
| `29`         | `00101001`   | `N16`                    | JNZ  | `PC <- N16`                    |       |
| `2A`         | `00101010`   | `N16`                    | JS   | `PC <- N16`                    |       |
| `2B`         | `00101011`   | `N16`                    | JNS  | `PC <- N16`                    |       |
| `2C`         | `00101100`   | `N16`                    | JC   | `PC <- N16`                    |       |
| `2D`         | `00101101`   | `N16`                    | JNC  | `PC <- N16`                    |       |
| `2E`         | `00101110`   |                          | RET  | `PC <- pop()`                  |       |
| `40`         | `01000000`   | `Rd Rs 00`               | CMP  | `cmp(Rd, Rs)`                  | ZSC   |
| `41`         | `01000001`   | `Rd 00000 N8`            | CMP  | `cmp(Rd, N8)`                  | ZSC   |
| `42`         | `01000010`   | `Pd Ps 0000`             | CMP  | `cmp(Pd, Ps)`                  | ZSC   |
| `43`         | `01000011`   | `Rd Rs 00`               | ADD  | `Rd <- Rd + Rs`                | ZSC   |
| `44`         | `01000100`   | `Rd Rs 00`               | SUB  | `Rd <- Rd - Rs`                | ZS    |
| `45`         | `01000101`   | `Rd Rs 00`               | MUL  | `Rd <- Rd * Rs`                | ZS    |
| `46`         | `01000110`   | `Rd Rs 00`               | DIV  | `Rd <- Rd / Rs`                | ZS    |
| `47`         | `01000111`   | `Pd Rs 000`              | ADD  | `Pd <- Pd + Rs`                | ZSC   |
| `48`         | `01001000`   | `Pd Rs 000`              | SUB  | `Pd <- Pd - Rs`                | ZS    |
| `49`         | `01001001`   | `Rd Rs 00`               | AND  | `Rd <- Rd & Rs`                | ZS    |
| `4A`         | `01001010`   | `Rd Rs 00`               | OR   | `Rd <- Rd | Rs`                | ZS    |
| `4B`         | `01001011`   | `Rd Rs 00`               | XOR  | `Rd <- Rd ^ Rs`                | ZS    |
| `4C`         | `01001100`   | `Rd`                     | NOT  | `Rd <- ~Rd`                    | ZS    |
| `4D`         | `01001101`   | `Rd Rs 00`               | ICMP | `icmp(Rd, Rs)`                 | ZSC   |
| `4E`         | `01001110`   | `Rd 00000 N8`            | ICMP | `icmp(Rd, N8)`                 | ZSC   |
| `4F`         | `01001111`   | `Pd Ps 0000`             | ICMP | `icmp(Pd, Ps)`                 | ZSC   |
| `50`         | `01010000`   |                          | SETZ | `Z = 1`                        | Z     |
| `51`         | `01010001`   |                          | CLZ  | `Z = 0`                        | Z     |
| `52`         | `01010010`   |                          | SETS | `S = 1`                        | S     |
| `53`         | `01010011`   |                          | CLS  | `S = 0`                        | S     |
| `54`         | `01010100`   |                          | SETC | `C = 1`                        | C     |
| `55`         | `01010101`   |                          | CLC  | `C = 0`                        | C     |
| `60`         | `01100000`   | `Rd 00000`               | INC  | `Rd = Rd + 1`                  | ZC    |
| `61`         | `01100001`   | `Pd 000000`              | INC  | `Pd = Pd + 1`                  | ZC    |
| `62`         | `01100010`   | `Rd 00000`               | DEC  | `Rd = Rd - 1`                  | ZC    |
| `63`         | `01100011`   | `Pd 000000`              | DEC  | `Pd = Pd - 1`                  | ZC    |

### Compare instruction
Sets `Z` for equality, `S` for lesser than, and `C` for greater than.
