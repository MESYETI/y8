#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "emu.h"
#include "font.h"
#include "display.h"
#include "keyboard.h"

#define INC_16(N) ((N) == 65535? 0 : (N) + 1)

Emu emu;

void Emu_Init(void) {
	size_t   size;
	uint8_t* font = GetFont(&size);

	printf("font is %d bytes\n", (int) size);

	memcpy(display.charSet, font, size);

	emu.halted = false;

	IOChip_Init(&emu.io);

	Keyboard_Init(&emu.io);
}

uint8_t* Emu_GetByte(uint16_t addr) {
	if (addr < 0x4000) {
		return &emu.rom[addr];
	}
	else if (addr < 0x8000) {
		return &emu.cart[addr - 0x4000];
	}
	else if (addr < 0xC000) {
		return &emu.ram[(addr - 0x8000) % 4096];
	}
	else if (addr < 0xD000) {
		return NULL;
	}
	else if (addr < 0xE000) {
		return NULL;
	}

	fprintf(stderr, "warning: accessing out of bounds memory");

	static uint8_t inaccessible;
	return &inaccessible;
}

uint8_t Emu_Read8(uint16_t addr) {
	if ((addr >= 0xD000) && (addr < 0xE000)) {
		uint16_t ioAddr = (addr - 0xD000) % 16;

		return IOChip_Read(&emu.io, (uint8_t) ioAddr);
	}
	if ((addr >= 0xC000) && (addr < 0xD000)) {
		return Display_Read(addr - 0xC000);
	}

	return *Emu_GetByte(addr);
}

uint16_t Emu_Read16(uint16_t addr) {
	uint16_t low  = Emu_Read8(addr);
	uint16_t high = Emu_Read8(INC_16(addr));

	return low | (high << 8);
}

void Emu_Write8(uint16_t addr, uint8_t value) {
	if (addr < 0x8000) {
		fprintf(stderr, "warning: writing to read-only memory at %.4X\n", (int) addr);
		fprintf(stderr, "PC: %.4X\n", (int) emu.pc);
		return;
	}

	if ((addr >= 0xD000) && (addr < 0xE000)) {
		uint16_t ioAddr = (addr - 0xD000) % 16;

		IOChip_Write(&emu.io, (uint8_t) ioAddr, value);
		return;
	}

	if ((addr >= 0xC000) && (addr < 0xD000)) {
		Display_Write(addr - 0xC000, value);
		return;
	}

	#if 0
		printf("Writing %d to %.4X\n", (int) value, (int) addr);
	#endif

	*Emu_GetByte(addr) = value;
}

void Emu_Write16(uint16_t addr, uint16_t value) {
	Emu_Write8(addr,         (uint8_t) (value & 0xFF));
	Emu_Write8(INC_16(addr), (uint8_t) ((value & 0xFF00) >> 8));
}

void Emu_WriteReg8(uint8_t reg, uint8_t value) {
	switch (reg) {
		case 0: emu.a = value; break;
		case 1: emu.b = value; break;
		case 2: emu.c = value; break;
		case 3: emu.d = value; break;
		case 4: emu.e = value; break;
		case 5: emu.f = value; break;
		case 6: emu.g = value; break;
		case 7: emu.h = value; break;
	}

	#if 0
		const char* names = "abcdefgh";

		printf("setting %c to %d\n", names[reg], (int) value);
	#endif
}

void Emu_WriteReg16(uint8_t reg, uint16_t value) {
	uint8_t h;
	uint8_t l;

	switch (reg) {
		case 0: {
			h = 0;
			l = 1;
			break;
		}
		case 1: {
			h = 2;
			l = 3;
			break;
		}
		case 2: {
			h = 4;
			l = 5;
			break;
		}
		case 3: {
			emu.sp = value;
			return;
		}
	}

	Emu_WriteReg8(h, ((uint8_t) ((value & 0xFF00) >> 8)));
	Emu_WriteReg8(l, ((uint8_t) (value & 0xFF)));
}

uint8_t Emu_ReadReg8(uint8_t reg) {
	switch (reg) {
		case 0: return emu.a;
		case 1: return emu.b;
		case 2: return emu.c;
		case 3: return emu.d;
		case 4: return emu.e;
		case 5: return emu.f;
		case 6: return emu.g;
		case 7: return emu.h;
	}

	abort();
}

uint16_t Emu_ReadReg16(uint16_t reg) {
	uint8_t h;
	uint8_t l;

	switch (reg) {
		case 0: {
			h = 0;
			l = 1;
			break;
		}
		case 1: {
			h = 2;
			l = 3;
			break;
		}
		case 2: {
			h = 4;
			l = 5;
			break;
		}
		case 3: {
			return emu.sp;
		}
		default: {
			abort();
		}
	}

	h = Emu_ReadReg8(h);
	l = Emu_ReadReg8(l);

	return ((uint16_t) l) | (((uint16_t) h) << 8);
}

static uint8_t NextByte(void) {
	uint8_t ret = Emu_Read8(emu.pc);

	emu.pc = INC_16(emu.pc);

	return ret;
}

static uint16_t NextWord(void) {
	uint16_t ret = NextByte();

	ret |= ((uint16_t) NextByte()) << 8;

	return ret;
}

static void Push16(uint16_t value) {
	emu.sp -= 2;

	Emu_Write16(emu.sp, value);
}

static uint16_t Pop16(void) {
	uint16_t ret = Emu_Read16(emu.sp);
	emu.sp += 2;

	return ret;
}

#define DEC_RD_RS(B) \
	uint8_t rd = (B & 0xE0) >> 5; \
	uint8_t rs = (B & 0x1C) >> 2

#define DEC_PD_PS(B) \
	uint8_t pd = (B & 0xC0) >> 6; \
	uint8_t ps = (B & 0x30) >> 4

#define DEC_RD_PS(B) \
	uint8_t rd = (B & 0xE0) >> 5; \
	uint8_t ps = (B & 0x18) >> 3

#define DEC_PD_RS(B) \
	uint8_t pd = (B & 0xC0) >> 6; \
	uint8_t rs = (B & 0x38) >> 3

#define DEC_RD(B) \
	uint8_t rd = (B & 0xE0) >> 5

#define DEC_PD(B) \
	uint8_t pd = (B & 0xC0) >> 6

#define FLAGS_8(V) do { \
	emu.zero = v == 0; \
	emu.sign = v & 0x80; \
} while (0)

#define FLAGS_16(V) do { \
	emu.zero = v == 0; \
	emu.sign = v & 0x8000; \
} while (0)

void Emu_RunInsts(int times) {
	for (int i = 0; i < times; ++ i) {
		if (emu.halted) return;

		uint16_t opcAddr = emu.pc;
		uint8_t  opc     = NextByte();

		switch (opc) {
			case 0x00: { // HALT
				printf("computer halted\n");

				emu.halted = true;
				break;
			}
			case 0x10: { // MOV Rd, Rs
				uint8_t param = NextByte();
				DEC_RD_RS(param);

				uint8_t v = Emu_ReadReg8(rs);
				FLAGS_8(v);

				Emu_WriteReg8(rd, v);
				break;
			}
			case 0x11: { // MOV Pd, Ps
				uint8_t param = NextByte();
				DEC_PD_PS(param);

				uint16_t v = Emu_ReadReg16(ps);
				FLAGS_16(v);

				Emu_WriteReg16(pd, v);
				break;
			}
			case 0x12: { // MOV Rd, N8
				DEC_RD(NextByte());

				uint8_t v = NextByte();
				FLAGS_8(v);

				Emu_WriteReg8(rd, v);
				break;
			}
			case 0x13: { // MOV Pd, N16
				DEC_PD(NextByte());

				uint16_t v = NextWord();
				FLAGS_16(v);

				Emu_WriteReg16(pd, v);
				break;
			}
			case 0x14: { // MOV Rd, [Ps]
				uint8_t param = NextByte();
				DEC_RD_PS(param);

				uint8_t v = Emu_Read16(Emu_ReadReg16(ps));
				FLAGS_16(v);

				Emu_WriteReg8(rd, v);
				break;
			}
			case 0x15: { // MOV Pd, [Ps]
				uint8_t param = NextByte();
				DEC_PD_PS(param);

				uint16_t v = Emu_Read16(Emu_ReadReg16(ps));
				FLAGS_16(v);

				Emu_WriteReg16(pd, v);
				break;
			}
			case 0x17: { // MOV Rd, [N16]
				DEC_RD(NextByte());

				uint8_t v = Emu_Read8(NextWord());
				FLAGS_8(v);

				Emu_WriteReg8(rd, v);
				break;
			}
			case 0x18: { // MOV Pd, [N16]
				DEC_PD(NextByte());

				uint16_t v = Emu_Read16(NextWord());
				FLAGS_16(v);

				Emu_WriteReg16(pd, v);
				break;
			}
			case 0x19: { // MOV [Pd], Rs
				uint8_t param = NextByte();
				DEC_PD_RS(param);

				uint8_t v = Emu_ReadReg8(rs);
				FLAGS_8(v);

				Emu_Write8(Emu_ReadReg16(pd), v);
				break;
			}
			case 0x1A: { // MOV [Pd], Ps
				uint8_t param = NextByte();
				DEC_PD_PS(param);

				uint16_t v = Emu_ReadReg16(ps);
				FLAGS_16(v);

				Emu_Write16(Emu_ReadReg16(pd), v);
				break;
			}
			case 0x1b: { // MOV [Pd], N8
				DEC_PD(NextByte());

				uint8_t v = NextByte();
				FLAGS_8(v);

				Emu_Write8(Emu_ReadReg16(pd), v);
				break;
			}
			case 0x1C: { // MOV [Pd], N16
				DEC_PD(NextByte());

				uint16_t v = NextWord();
				FLAGS_16(v);

				Emu_Write16(Emu_ReadReg16(pd), v);
				break;
			}
			case 0x20: { // JMP N16	
				emu.pc = NextWord();
				break;
			}
			case 0x21: { // JMP [N16]
				emu.pc = Emu_Read16(NextWord());
				break;
			}
			case 0x22: { // JMP Pd
				DEC_PD(NextByte());

				emu.pc = Emu_ReadReg16(pd);
				break;
			}
			case 0x23: { // JMP [Pd]
				DEC_PD(NextByte());

				emu.pc = Emu_Read16(Emu_ReadReg16(pd));
				break;
			}
			case 0x24: { // CALL N16
				uint16_t addr = NextWord();
				Push16(emu.pc);

				emu.pc = addr;
				break;
			}
			case 0x25: { // CALL [N16]
				uint16_t addr = Emu_Read16(NextWord());
				Push16(emu.pc);

				emu.pc = addr;
				break;
			}
			case 0x26: { // CALL Pd
				DEC_PD(NextByte());

				Push16(emu.pc);
				emu.pc = Emu_ReadReg16(pd);
				break;
			}
			case 0x27: { // CALL [Pd]
				DEC_PD(NextByte());

				Push16(emu.pc);
				emu.pc = Emu_Read16(Emu_ReadReg16(pd));
				break;
			}
			case 0x28:   // JZ  N16
			case 0x29:   // JNZ N16
			case 0x2A:   // JS  N16
			case 0x2B:   // JNS N16
			case 0x2C:   // JC  N16
			case 0x2D: { // JNC N16
				uint16_t addr = NextWord();

				if (
					((opc == 0x28) && !emu.zero)  || ((opc == 0x29) && emu.zero) ||
					((opc == 0x2A) && !emu.sign)  || ((opc == 0x2B) && emu.sign) ||
					((opc == 0x2C) && !emu.carry) || ((opc == 0x2D) && emu.carry)
				) {
					break;
				}

				emu.pc = addr;
				break;
			}
			case 0x2E: { // RET
				emu.pc = Pop16();
				break;
			}
			case 0x40:   // CMP Rd, Rs
			case 0x41:   // CMP Rd, N8
			case 0x42: { // CMP Pd, Ps
				uint16_t a;
				uint16_t b;

				uint8_t param = NextByte();

				switch (opc) {
					case 0x40: { // CMP Rd, Rs
						DEC_RD_RS(param);

						a = Emu_ReadReg8(rd);
						b = Emu_ReadReg8(rs);
						break;
					}
					case 0x41: { // CMP Rd, N8
						DEC_RD(param);

						a = Emu_ReadReg8(rd);
						b = NextByte();
						break;
					}
					case 0x42: { // CMP Pd, Ps
						DEC_PD_PS(param);

						a = Emu_ReadReg16(pd);
						b = Emu_ReadReg16(ps);
						break;
					}
				}

				emu.zero  = a == b;
				emu.sign  = a < b;
				emu.carry = a > b;
				break;
			}
			case 0x43:   // ADD Rd, Rs
			case 0x44:   // SUB Rd, Rs
			case 0x45:   // MUL Rd, Rs
			case 0x46:   // DIV Rd, Rs
			case 0x49:   // AND Rd, Rs
			case 0x4A:   // OR  Rd, Rs
			case 0x4B: { // XOR Rd, Rs
				uint8_t param = NextByte();
				DEC_RD_RS(param);

				uint8_t a = Emu_ReadReg8(rd);
				uint8_t b = Emu_ReadReg8(rs);

				switch (opc) {
					case 0x43: a += b; break;
					case 0x44: a -= b; break;
					case 0x45: a *= b; break;
					case 0x46: a /= b; break;
					case 0x49: a &= b; break;
					case 0x4A: a |= b; break;
					case 0x4B: a ^= b; break;
				}

				Emu_WriteReg8(rd, a);
				break;
			}
			case 0x47:   // ADD Pd, Rs
			case 0x48: { // SUB Pd, Rs
				uint8_t param = NextByte();
				DEC_PD_RS(param);

				uint16_t a = Emu_ReadReg16(pd);
				uint8_t  b = Emu_ReadReg8(rs);

				if (opc == 0x47) {
					a += b;
				}
				else {
					a -= b;
				}

				Emu_WriteReg16(pd, a);
				break;
			}
			case 0x4D:   // ICMP Rd, Rs
			case 0x4E: { // ICMP Rd, N8
				uint8_t param = NextByte();

				int8_t a;
				int8_t b;

				if (opc == 0x4E) {
					DEC_RD(param);

					a = (int8_t) Emu_ReadReg8(rd);
					b = (int8_t) NextByte();
				}
				else {
					DEC_RD_RS(param);

					a = (int8_t) Emu_ReadReg8(rd);
					b = (int8_t) Emu_ReadReg8(rs);
				}

				emu.zero  = a == b;
				emu.sign  = a < b;
				emu.carry = a > b;
				break;
			}
			case 0x4F: { // ICMP Pd, Ps
				uint8_t param = NextByte();
				DEC_PD_PS(param);

				uint16_t a = Emu_ReadReg16(pd);
				uint16_t b = Emu_ReadReg16(ps);

				emu.zero  = a == b;
				emu.sign  = a < b;
				emu.carry = a > b;
				break;
			}
			case 0x50: emu.zero = true;   break; // SETZ
			case 0x51: emu.zero = false;  break; // CLZ
			case 0x52: emu.sign = true;   break; // SETS
			case 0x53: emu.sign = false;  break; // CLS
			case 0x54: emu.carry = true;  break; // SETC
			case 0x55: emu.carry = false; break; // CLC
			case 0x60: { // INC Rd
				DEC_RD(NextByte());

				uint8_t v = Emu_ReadReg8(rd);

				emu.zero  = v == 255;
				emu.carry = v == 255;

				Emu_WriteReg8(rd, v + 1);
				break;
			}
			case 0x61: { // INC Pd
				DEC_PD(NextByte());

				uint16_t v = Emu_ReadReg16(pd);

				emu.zero  = v == 65535;
				emu.carry = v == 65535;

				Emu_WriteReg16(pd, v + 1);
				break;
			}
			case 0x62: { // DEC Rd
				DEC_RD(NextByte());

				uint8_t v = Emu_ReadReg8(rd);

				emu.zero  = v == 1;
				emu.carry = v == 0;

				Emu_WriteReg8(rd, v - 1);
				break;
			}
			case 0x63: { // DEC Pd
				DEC_PD(NextByte());

				uint16_t v = Emu_ReadReg16(pd);

				emu.zero  = v == 1;
				emu.carry = v == 0;

				Emu_WriteReg16(pd, v - 1);
				break;
			}
			case 0x64: { // SHL Rd, N4
				uint8_t param = NextByte();
				DEC_RD(param);
				param &= 0xF;

				Emu_WriteReg8(rd, Emu_ReadReg8(rd) << param);
				break;
			}
			case 0x65: { // SHL Pd, N4
				uint8_t param = NextByte();
				DEC_PD(param);
				param &= 0xF;

				Emu_WriteReg16(pd, Emu_ReadReg16(pd) << param);
				break;
			}
			case 0x66: { // SHL Rd, N4
				uint8_t param = NextByte();
				DEC_RD(param);
				param &= 0xF;

				Emu_WriteReg8(rd, Emu_ReadReg8(rd) >> param);
				break;
			}
			case 0x67: { // SHL Pd, N4
				uint8_t param = NextByte();
				DEC_PD(param);
				param &= 0xF;

				Emu_WriteReg16(pd, Emu_ReadReg16(pd) >> param);
				break;
			}
			case 0x68: { // SHL Rd, Rs
				uint8_t param = NextByte();
				DEC_RD_RS(param);

				Emu_WriteReg8(rd, Emu_ReadReg8(rd) << Emu_ReadReg8(rs));
				break;
			}
			case 0x69: { // SHL Pd, Ps
				uint8_t param = NextByte();
				DEC_PD_PS(param);

				Emu_WriteReg8(pd, Emu_ReadReg8(pd) << Emu_ReadReg8(ps));
				break;
			}
			case 0x6A: { // SHR Rd, Rs
				uint8_t param = NextByte();
				DEC_RD_RS(param);

				Emu_WriteReg8(rd, Emu_ReadReg8(rd) >> Emu_ReadReg8(rs));
				break;
			}
			case 0x6B: { // SHR Pd, Ps
				uint8_t param = NextByte();
				DEC_PD_PS(param);

				Emu_WriteReg8(pd, Emu_ReadReg8(pd) >> Emu_ReadReg8(ps));
				break;
			}
			default: {
				fprintf(stderr, "Invalid opcode %.2X at %.4X\n", opc, (int) opcAddr);
				emu.halted = true;
			}
		}
	}
}
