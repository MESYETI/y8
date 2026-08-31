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

	uint8_t charBuf[0x300];
	uint8_t charSet[2048];
	uint8_t setRaster;
	uint8_t rasterCount;
	bool    enableInterrupt;
	bool    interruptFlag;
} Display;

extern Display display;

void    Display_Init(void);
void    Display_Free(void);
void    Display_Write(uint16_t addr, uint8_t value);
uint8_t Display_Read(uint16_t addr);
void    Display_Render(void);

#endif
