#include "app.h"
#include "emu.h"
#include "util.h"
#include "display.h"

App app;

void App_Init(void) {
	app.running = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Failed to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	Emu_Init();
	Display_Init();
}

void App_Update(void) {
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT: {
				app.running = false;
			}
		}
	}

	Display_Render();

	MicroSleep(1000000 / 60);
}

void App_Free(void) {
	SDL_Quit();

	Display_Free();
}
