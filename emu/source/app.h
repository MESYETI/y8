#ifndef Y8_APP_H
#define Y8_APP_H

#include "common.h"

typedef struct {
	bool running;
} App;

extern App app;

void App_Init(void);
void App_Update(void);
void App_Free(void);

#endif
