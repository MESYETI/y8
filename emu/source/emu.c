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

void Emu_RunInsts(int times) {
	(void) times;
}
