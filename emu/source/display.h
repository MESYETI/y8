#ifndef Y8_DISPLAY_H
#define Y8_DISPLAY_H

#include <SDL2/SDL.h>

#define RES_X 512
#define RES_Y 384
#define CHR_W 64
#define CHR_H 48

typedef struct {
	SDL_Window*   window;
	SDL_Renderer* renderer;
	SDL_Texture*  texture;
	uint32_t*     pixels;
} Display;

extern Display display;

void Display_Init(void);
void Display_Free(void);
void Display_Render(void);

#endif
