#include "io.h"

uint8_t IOChip_Read(IOChip* chip, uint8_t addr) {
	switch (addr) {
		case 0x00:
		case 0x01: {
			IOPort* port = addr? &chip->portB : &chip->portA;
			uint8_t mask = addr?  chip->dirB  :  chip->dirA;

			if (!port->read) return 0;

			return port->read() & ~mask;
		}
		case 0x04: return (uint8_t) (chip->timerA & 0xFF);
		case 0x05: return (uint8_t) ((chip->timerA & 0xFF00) >> 8);
		case 0x06: return (uint8_t) (chip->timerB & 0xFF);
		case 0x07: return (uint8_t) ((chip->timerB & 0xFF00) >> 8);
		default: break;
	}

	return 0;
}

void IOChip_Write(IOChip* chip, uint8_t addr, uint8_t value) {
	switch (addr) {
		case 0x00:
		case 0x01: {
			IOPort* port = addr? &chip->portB : &chip->portA;
			uint8_t mask = addr?  chip->dirB  :  chip->dirA;

			if (!port->write) return;

			port->write(value, mask);
			break;
		}
		case 0x04: {
			chip->timerA &= 0xFF00;
			chip->timerA |= value;
			break;
		}
		case 0x05: {
			chip->timerA &= 0x00FF;
			chip->timerA |= ((uint16_t) value) << 8;
			break;
		}
		case 0x06: {
			chip->timerB &= 0xFF00;
			chip->timerB |= value;
			break;
		}
		case 0x07: {
			chip->timerB &= 0x00FF;
			chip->timerB |= ((uint16_t) value) << 8;
			break;
		}
		default: break;
	}
}
