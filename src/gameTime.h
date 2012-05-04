#ifndef GAMETIME_H
#define GAMETIME_H

#ifdef _MSC_VER
#include <Windows.h>

typedef struct Timer
{
	LARGE_INTEGER start;
	LARGE_INTEGER stop;
} Timer;

#else

#include <time.h>

typedef struct Timer
{
	struct timespec start;
	struct timespec stop;
} Timer;

#endif

void start_timer(Timer* timer);
void stop_timer(Timer* timer);
double get_elapsed_time(const Timer* timer);

#endif