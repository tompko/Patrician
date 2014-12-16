#include "gameTime.h"

#include <time.h>

void start_timer(Timer* timer)
{
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timer->start);
}

void stop_timer(Timer* timer)
{
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timer->stop);
}

double get_elapsed_time(const Timer* timer)
{
	return (double)(timer->stop.tv_sec - timer->start.tv_sec) + 
	(double)(timer->stop.tv_nsec - timer->start.tv_nsec) / 1000000000.0;
}

const char* parse_time(const char* timeString, int* minutes, int*seconds)
{
	const char * data = timeString;

	*minutes = atoi(data);

	while(*data && *data != ' ' && *data != ':')
	{
		++data;
	}

	if(*data == ':')
	{
		*seconds = atoi(data);
		while(*data && *data != ' ')
		{
			++data;
		}
	}
	return ++data;
}
