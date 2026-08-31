#ifndef Y8_EMU_H
#define Y8_EMU_H

#include "io.h"
#include "common.h"

typedef struct {
	// memory
	uint8_t rom[4096];
	uint8_t ram[4096];
	uint8_t cart[16384];

	// registers
	uint8_t  a;
	uint8_t  b;
	uint8_t  c;
	uint8_t  d;
	uint8_t  e;
	uint8_t  f;
	uint8_t  g;
	uint8_t  h;
	uint16_t sp;
	uint16_t pc;

	// state
	bool zero;
	bool sign;
	bool carry;
	bool halted;

	// IO
	IOChip io;
} Emu;

extern Emu emu;

void     Emu_Init(void);
void     Emu_LoadCart(const char* path);
uint8_t* Emu_GetByte(uint16_t addr);
uint8_t  Emu_Read8(uint16_t addr);
uint16_t Emu_Read16(uint16_t addr);
void     Emu_Write8(uint16_t addr, uint8_t value);
void     Emu_Write16(uint16_t addr, uint16_t value);
void     Emu_WriteReg8(uint8_t reg, uint8_t value);
void     Emu_WriteReg16(uint8_t reg, uint16_t value);
uint8_t  Emu_ReadReg8(uint8_t reg);
uint16_t Emu_ReadReg16(uint16_t reg);
void     Emu_RunInsts(int times);

#endif
