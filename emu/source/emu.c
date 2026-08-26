#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "emu.h"
#include "font.h"

#define INC_16(N) ((N) == 65535? 0 : (N) + 1)

Emu emu;

void Emu_Init(void) {
	size_t   size;
	uint8_t* font = GetFont(&size);

	printf("font is %d bytes\n", (int) size);

	memcpy(&emu.vram[3072], font, size);

	emu.halted = false;
}

uint8_t* Emu_GetByte(uint16_t addr) {
	if (addr & 0x8000) {
		// cart
		return &emu.cart[addr & 0x7FFF];
	}
	if ((addr & 0x4000) == 0) {
		return &emu.rom[addr & 0x0FFF];
	}
	if (addr & 0x1000) {
		return &emu.vram[addr & 0x0FFF];
	}

	return &emu.ram[addr & 0x0FFF];
}

uint8_t  Emu_Read8(uint16_t addr) {
	return *Emu_GetByte(addr);
}

uint16_t Emu_Read16(uint16_t addr) {
	uint16_t low  = Emu_Read8(addr);
	uint16_t high = Emu_Read8(INC_16(addr));

	return low | (high << 8);
}

void Emu_Write8(uint16_t addr, uint8_t value) {
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

void Emu_RunInsts(int times) {
	for (int i = 0; i < times; ++ i) {
		if (emu.halted) return;

		uint8_t opc = NextByte();

		switch (opc) {
			case 0x00: { // HALT
				printf("computer halted\n");

				emu.halted = true;
				break;
			}
			case 0x10: { // MOV Rd, Rs
				uint8_t param = NextByte();
				DEC_RD_RS(param);

				Emu_WriteReg8(rd, Emu_ReadReg8(rs));
				break;
			}
			case 0x11: { // MOV Pd, Ps
				uint8_t param = NextByte();
				DEC_PD_PS(param);

				Emu_WriteReg16(pd, Emu_ReadReg16(ps));
				break;
			}
			case 0x12: { // MOV Rd, N8
				DEC_RD(NextByte());

				Emu_WriteReg8(rd, NextByte());
				break;
			}
			case 0x13: { // MOV Pd, N16
				DEC_PD(NextByte());

				Emu_WriteReg16(pd, NextWord());
				break;
			}
			case 0x14: { // MOV Rd, [Ps]
				uint8_t param = NextByte();
				DEC_RD_PS(param);

				Emu_WriteReg8(rd, Emu_Read16(Emu_ReadReg16(ps)));
				break;
			}
			case 0x15: { // MOV Pd, [Ps]
				uint8_t param = NextByte();
				DEC_PD_PS(param);

				Emu_WriteReg16(pd, Emu_Read16(Emu_ReadReg16(ps)));
				break;
			}
			case 0x17: { // MOV Rd, [N16]
				DEC_RD(NextByte());

				Emu_WriteReg8(rd, Emu_Read8(NextWord()));
				break;
			}
			case 0x18: { // MOV Pd, [N16]
				DEC_PD(NextByte());

				Emu_WriteReg16(pd, Emu_Read16(NextWord()));
				break;
			}
			case 0x19: { // MOV [Pd], Rs
				uint8_t param = NextByte();
				DEC_PD_RS(param);

				Emu_Write8(Emu_ReadReg16(pd), Emu_ReadReg8(rs));
				break;
			}
			case 0x1A: { // MOV [Pd], Ps
				uint8_t param = NextByte();
				DEC_PD_PS(param);

				Emu_Write16(Emu_ReadReg16(pd), Emu_ReadReg16(ps));
				break;
			}
			case 0x1b: { // MOV [Pd], N8
				DEC_PD(NextByte());

				Emu_Write8(Emu_ReadReg16(pd), NextByte());
				break;
			}
			case 0x1C: { // MOV [Pd, N16]
				DEC_PD(NextByte());

				Emu_Write16(Emu_ReadReg16(pd), NextWord());
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
				Push16(emu.pc);

				emu.pc = NextWord();
				break;
			}
			case 0x25: { // CALL [N16]
				Push16(emu.pc);

				emu.pc = Emu_Read16(NextWord());
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
				if (
					((opc == 0x28) && !emu.zero)  || ((opc == 0x29) && emu.zero) ||
					((opc == 0x2A) && !emu.sign)  || ((opc == 0x2B) && emu.sign) ||
					((opc == 0x2C) && !emu.carry) || ((opc == 0x2D) && emu.carry)
				) {
					break;
				}

				emu.pc = NextWord();
				break;
			}
			case 0x2E: { // RET
				emu.pc = Pop16();
				break;
			}
			default: {
				fprintf(stderr, "Invalid opcode %.2X", opc);
				emu.halted = true;
			}
		}
	}
}
