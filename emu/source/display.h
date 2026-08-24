#ifndef Y8_DISPLAY_H
#define Y8_DISPLAY_H

#include <SDL2/SDL.h>

#define RES_X 256
#define RES_Y 192
#define CHR_W 32
#define CHR_H 24

typedef struct {
	SDL_Window*   window;
	SDL_Renderer* renderer;
	SDL_Texture*  texture;
	uint32_t*     pixels;

	uint16_t scroll;
} Display;

extern Display display;

void Display_Init(void);
void Display_Free(void);
void Display_Render(void);

#endif
