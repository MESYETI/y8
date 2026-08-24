#include "app.h"

int main(int argc, char** argv) {
	App_Init(argc, argv);

	while (app.running) {
		App_Update();
	}

	App_Free();
	return 0;
}
