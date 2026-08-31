#include "app.h"
#include "emu.h"
#include "util.h"
#include "display.h"
#include "printer.h"

App app;

#define APP_USAGE \
	"Usage: %s FLAGS\n\n" \
	"Flags:\n" \
	"    --serial=DEVICE, where DEVICE = `printer`\n" \
	"    --cart FILE\n"

void App_Init(int argc, char** argv) {
	if ((argc > 1) && strcmp(argv[1], "--help") == 0) {
		printf(APP_USAGE, argv[0]);
		exit(0);
	}

	app.running = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Failed to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	Emu_Init();
	Display_Init();

	FILE* rom = fopen("rom.bin", "rb");

	if (rom) {
		size_t res = fread(emu.rom, 1, sizeof(emu.rom), rom);

		printf("rom: loaded %d bytes\n", (int) res);

		fclose(rom);
	}
	else {
		fprintf(stderr, "warning: no rom loaded\n");
	}

	for (int i = 1; i < argc; ++ i) {
		if (strcmp(argv[i], "--serial=printer") == 0) {
			emu.io.serial = PrinterDevice();
		}
		else if (strcmp(argv[i], "--cart") == 0) {
			++ i;

			if (i >= argc) {
				fprintf(stderr, "--cart requires FILE flag\n");
				exit(1);
			}

			Emu_LoadCart(argv[i]);
		}
		else {
			fprintf(stderr, "Unknown flag '%s'\n", argv[i]);
		}
	}
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

	Emu_RunInsts(1000);

	Display_Render();

	MicroSleep(1000000 / 60);
}

void App_Free(void) {
	SDL_Quit();

	Display_Free();
}
