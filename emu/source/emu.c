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
