#include "emu.h"
#include "mem.h"
#include "display.h"

Display display;

void Display_Init(void) {
	display.window = SDL_CreateWindow(
		"Y8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RES_X * 4, RES_Y * 4,
		SDL_WINDOW_RESIZABLE
	);

	if (display.window == NULL) {
		fprintf(stderr, "Failed to create window: %s", SDL_GetError());
		exit(1);
	}

	display.renderer = SDL_CreateRenderer(display.window, -1, SDL_RENDERER_ACCELERATED);

	if (display.renderer == NULL) {
		fprintf(stderr, "Failed to create renderer: %s", SDL_GetError());
		exit(1);
	}

	SDL_RenderSetLogicalSize(display.renderer, RES_X, RES_Y);

	display.texture = SDL_CreateTexture(
		display.renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, RES_X, RES_Y
	);

	if (display.texture == NULL) {
		fprintf(stderr, "Failed to create texture: %s", SDL_GetError());
		exit(1);
	}

	display.pixels = SafeMalloc(RES_X * RES_Y * 4);
	display.scroll = 0;

	for (int i = 0; i < RES_X * RES_Y; ++ i) {
		display.pixels[i] = 0xFFCCCCCC;
	}
}

void Display_Free(void) {
	if (display.texture)  SDL_DestroyTexture(display.texture);
	if (display.renderer) SDL_DestroyRenderer(display.renderer);
	if (display.window)   SDL_DestroyWindow(display.window);
}

void Display_Render(void) {
	for (int y = 0; y < CHR_H; ++ y) {
		for (int x = 0; x < CHR_W; ++ x) {
			uint16_t start = 0xC000 + (display.scroll * CHR_W);
			uint8_t ch     = Emu_Read8((uint16_t) (0xC000 + x + (y * CHR_W)));

			bool invert = ch & 0x80? 1 : 0;
			ch = ch & 0x7F;

			uint16_t chAddr = (((uint16_t) ch) * 8) + 0xCC00;

			for (int cy = 0; cy < 8; ++ cy) {
				for (int cx = 0; cx < 8; ++ cx) {
					int px = (x * 8) + cx + 4;
					int py = (y * 8) + cy + 4;

					uint8_t fontLine = Emu_Read8(chAddr + ((uint16_t) cy));

					if (invert) {
						fontLine = ~fontLine;
					}

					if (fontLine & (1 << (7 - cx))) {
						display.pixels[(py * RES_X) + px] = 0x00000000;
					}
					else {
						display.pixels[(py * RES_X) + px] = 0xFFCCCCCC;
					}
				}
			}
		}
	}

	SDL_UpdateTexture(display.texture, NULL, display.pixels, RES_X * 4);
	SDL_RenderCopy(display.renderer, display.texture, NULL, NULL);
	SDL_RenderPresent(display.renderer);
}
