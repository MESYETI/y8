#ifndef Y8_IO_H
#define Y8_IO_H

#include "common.h"

typedef struct {
	uint8_t (*read)(void);
	void    (*write)(uint8_t v, uint8_t mask);
} IOPort;

typedef struct {
	uint8_t  dirA;
	uint8_t  dirB;
	uint16_t timerA;
	uint16_t timerB;
	uint8_t  serialReg;
	uint8_t  serialBits;
	uint8_t  interrupts;

	IOPort portA;
	IOPort portB;
} IOChip;

uint8_t IOChip_Read(IOChip* chip, uint8_t addr);
void    IOChip_Write(IOChip* chip, uint8_t addr, uint8_t value);

#endif
