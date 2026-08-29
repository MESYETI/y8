#include <SDL2/SDL.h>
#include "keyboard.h"

static uint8_t row;

static uint8_t ReadA(void) {
	SDL_Scancode keys[8];

	switch (row) {
		case 0: {
			keys[0] = SDL_SCANCODE_1;
			keys[1] = SDL_SCANCODE_LCTRL;
			keys[2] = 0;
			keys[3] = SDL_SCANCODE_LSHIFT;
			keys[4] = SDL_SCANCODE_2;
			keys[5] = SDL_SCANCODE_Q;
			keys[6] = SDL_SCANCODE_A;
			keys[7] = SDL_SCANCODE_Z;
			break;
		}
		case 1: {
			keys[0] = SDL_SCANCODE_3;
			keys[1] = SDL_SCANCODE_W;
			keys[2] = SDL_SCANCODE_S;
			keys[3] = SDL_SCANCODE_X;
			keys[4] = SDL_SCANCODE_4;
			keys[5] = SDL_SCANCODE_E;
			keys[6] = SDL_SCANCODE_D;
			keys[7] = SDL_SCANCODE_C;
			break;
		}
		case 2: {
			keys[0] = SDL_SCANCODE_5;
			keys[1] = SDL_SCANCODE_R;
			keys[2] = SDL_SCANCODE_F;
			keys[3] = SDL_SCANCODE_V;
			keys[4] = SDL_SCANCODE_6;
			keys[5] = SDL_SCANCODE_T;
			keys[6] = SDL_SCANCODE_G;
			keys[7] = SDL_SCANCODE_B;
			break;
		}
		case 3: {
			keys[0] = SDL_SCANCODE_7;
			keys[1] = SDL_SCANCODE_Y;
			keys[2] = SDL_SCANCODE_H;
			keys[3] = SDL_SCANCODE_N;
			keys[4] = SDL_SCANCODE_8;
			keys[5] = SDL_SCANCODE_U;
			keys[6] = SDL_SCANCODE_J;
			keys[7] = SDL_SCANCODE_M;
			break;
		}
		case 4: {
			keys[0] = SDL_SCANCODE_9;
			keys[1] = SDL_SCANCODE_I;
			keys[2] = SDL_SCANCODE_K;
			keys[3] = SDL_SCANCODE_COMMA;
			keys[4] = SDL_SCANCODE_EQUALS;
			keys[5] = SDL_SCANCODE_O;
			keys[6] = SDL_SCANCODE_L;
			keys[7] = SDL_SCANCODE_PERIOD;
			break;
		}
		case 5: {
			keys[0] = SDL_SCANCODE_MINUS;
			keys[1] = SDL_SCANCODE_P;
			keys[2] = SDL_SCANCODE_SEMICOLON;
			keys[3] = SDL_SCANCODE_SLASH;
			keys[4] = SDL_SCANCODE_BACKSPACE;
			keys[5] = SDL_SCANCODE_APOSTROPHE;
			keys[6] = SDL_SCANCODE_RETURN;
			keys[7] = SDL_SCANCODE_RSHIFT;
			break;
		}
		case 6: {
			keys[0] = SDL_SCANCODE_LEFT;
			keys[1] = SDL_SCANCODE_UP;
			keys[2] = SDL_SCANCODE_DOWN;
			keys[3] = SDL_SCANCODE_RIGHT;
			keys[4] = SDL_SCANCODE_SPACE;
			keys[5] = 0;
			keys[6] = 0;
			keys[7] = 0;
			break;
		}
		default: return 0;
	}

	const uint8_t* state = SDL_GetKeyboardState(NULL);

	uint16_t ret = 0;

	for (int i = 0; i < 8; ++ i) {
		ret <<= 1;
		ret  |= (keys[i] && state[keys[i]])? 1 : 0;
	}

	return ret;
}

static void WriteB(uint8_t v, uint8_t mask) {
	uint8_t bits = v & 127 & mask;

	if      (bits & 1)  row = 0;
	else if (bits & 2)  row = 1;
	else if (bits & 4)  row = 2;
	else if (bits & 8)  row = 3;
	else if (bits & 16) row = 4;
	else if (bits & 32) row = 5;
	else if (bits & 64) row = 6;
}

void Keyboard_Init(IOChip* chip) {
	chip->portA = (IOPort) {&ReadA, NULL};
	chip->portB = (IOPort) {NULL,   &WriteB};
}
