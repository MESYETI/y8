#ifndef Y8_EMU_H
#define Y8_EMU_H

#include "common.h"

typedef struct {
	uint8_t rom[4096];
	uint8_t ram[4096];
	uint8_t vram[4096];
	uint8_t cart[32768];
} Emu;

extern Emu emu;

void     Emu_Init(void);
uint8_t* Emu_GetByte(uint16_t addr);
uint8_t  Emu_Read8(uint16_t addr);
uint16_t Emu_Read16(uint16_t addr);

#endif
