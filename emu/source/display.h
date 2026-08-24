#ifndef Y8_DISPLAY_H
#define Y8_DISPLAY_H

#include <SDL2/SDL.h>

#define CHR_W 32
#define CHR_H 24
#define RES_X ((CHR_W * 8) + 8)
#define RES_Y ((CHR_H * 8) + 8)

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
