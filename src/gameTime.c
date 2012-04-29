#include "gameTime.h"

#ifdef _MSC_VER
#include <Windows.h>

void start_timer(Timer* timer)
{
	QueryPerformanceCounter(&timer->start);
}

void stop_timer(Timer* timer)
{
	QueryPerformanceCounter(&timer->stop);
}

double get_elapsed_time(const Timer* timer)
{
	LARGE_INTEGER freq;

	QueryPerformanceFrequency(&freq);
	return (double)(timer->stop.QuadPart - timer->start.QuadPart) / (double)(freq.QuadPart);
}

#endif