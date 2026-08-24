#include <time.h>
#include <unistd.h>
#include "util.h"

void MicroSleep(uint64_t us) {
	struct timespec time;
	time.tv_sec  = 0;
	time.tv_nsec = us * 1000;

	nanosleep(&time, NULL);
}
