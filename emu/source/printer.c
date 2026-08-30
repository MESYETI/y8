#include <stdio.h>
#include "printer.h"

static FILE* file;

static uint8_t Read(void) {
	return 0;
}

static void Write(uint8_t value) {
	if (file) {
		fwrite(&value, 1, 1, file);
	}

	fwrite(&value, 1, 1, stdout);
	fflush(stdout);
}

SerialDevice PrinterDevice(void) {
	file = fopen("printer.txt", "wb");

	if (!file) {
		fprintf(stderr, "printer: failed to open printer.txt\n");
	}

	printf("printer: initialised\n");

	return (SerialDevice) {&Read, &Write};
}
