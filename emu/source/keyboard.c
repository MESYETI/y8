#include <SDL2/SDL.h>
#include "keyboard.h"

static uint8_t col;

static uint8_t Read(void) {
	SDL_Scancode keys[8];

	switch (col) {
		case 0: {
			keys[0] = SDL_SCANCODE_1;
			keys[1] = SDL_SCANCODE_2;
			keys[2] = SDL_SCANCODE_3;
			keys[3] = SDL_SCANCODE_4;
			keys[4] = SDL_SCANCODE_5;
			keys[5] = SDL_SCANCODE_6;
			keys[6] = SDL_SCANCODE_7;
			keys[7] = SDL_SCANCODE_8;
			break;
		}
		case 1: {
			keys[0] = SDL_SCANCODE_9;
			keys[1] = SDL_SCANCODE_EQUALS;
			keys[2] = SDL_SCANCODE_MINUS;
			keys[3] = SDL_SCANCODE_BACKSPACE;
			keys[4] = SDL_SCANCODE_LCTRL;
			keys[5] = SDL_SCANCODE_Q;
			keys[6] = SDL_SCANCODE_W;
			keys[7] = SDL_SCANCODE_E;
			break;
		}
		case 2: {
			keys[0] = SDL_SCANCODE_R;
			keys[1] = SDL_SCANCODE_T;
			keys[2] = SDL_SCANCODE_Y;
			keys[3] = SDL_SCANCODE_U;
			keys[4] = SDL_SCANCODE_I;
			keys[5] = SDL_SCANCODE_O;
			keys[6] = SDL_SCANCODE_P;
			keys[7] = SDL_SCANCODE_APOSTROPHE;
			break;
		}
		case 3: {
			keys[0] = SDL_SCANCODE_A;
			keys[1] = SDL_SCANCODE_S;
			keys[2] = SDL_SCANCODE_D;
			keys[3] = SDL_SCANCODE_F;
			keys[4] = SDL_SCANCODE_G;
			keys[5] = SDL_SCANCODE_H;
			keys[6] = SDL_SCANCODE_J;
			keys[7] = SDL_SCANCODE_K;
			break;
		}
		case 4: {
			keys[0] = SDL_SCANCODE_L;
			keys[1] = SDL_SCANCODE_SEMICOLON;
			keys[2] = SDL_SCANCODE_Z;
			keys[3] = SDL_SCANCODE_X;
			keys[4] = SDL_SCANCODE_C;
			keys[5] = SDL_SCANCODE_V;
			keys[6] = SDL_SCANCODE_B;
			keys[7] = SDL_SCANCODE_N;
			break;
		}
		case 5: {
			keys[0] = SDL_SCANCODE_M;
			keys[1] = SDL_SCANCODE_COMMA;
			keys[2] = SDL_SCANCODE_PERIOD;
			keys[3] = SDL_SCANCODE_SLASH;
			keys[4] = SDL_SCANCODE_RSHIFT;
			keys[5] = SDL_SCANCODE_RETURN;
			keys[6] = SDL_SCANCODE_SPACE;
			keys[7] = SDL_SCANCODE_SPACE;
			break;
		}
		default: {
			keys[0] = SDL_SCANCODE_LEFT;
			keys[1] = SDL_SCANCODE_RIGHT;
			keys[2] = SDL_SCANCODE_UP;
			keys[3] = SDL_SCANCODE_DOWN;
			keys[4] = SDL_SCANCODE_LEFT;
			keys[5] = SDL_SCANCODE_RIGHT;
			keys[6] = SDL_SCANCODE_UP;
			keys[7] = SDL_SCANCODE_DOWN;
		}
	}

	const uint8_t* state = SDL_GetKeyboardState(NULL);

	uint16_t ret = 0;

	for (int i = 0; i < 8; ++ i) {
		ret <<= 1;
		ret  |= state[keys[i]]? 1 : 0;
	}

	return ret;
}

static void Write(uint8_t v, uint8_t mask) {
	col = v & 7 & mask;
}

IOPort CreateKeyboardPort(void) {
	return (IOPort) {&Read, &Write};
}
