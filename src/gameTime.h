#ifndef GAMETIME_H
#define GAMETIME_H

#include <time.h>

typedef struct Timer
{
	struct timespec start;
	struct timespec stop;
} Timer;

void start_timer(Timer* timer);
void stop_timer(Timer* timer);
double get_elapsed_time(const Timer* timer);

const char* parse_time(const char* timeString, int* minutes, int*seconds);
#endif
