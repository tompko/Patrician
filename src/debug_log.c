#include "debug_log.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static FILE* LogFile = NULL;
static const char * LogPath = "patrician_log.dat";

void initialize_log_lock(void);
void lock_log_lock(void);
void unlock_log_lock(void);
void release_log_lock(void);

void initialize_log(void)
{
	assert(LogFile == NULL);

	LogFile = fopen(LogPath, "w");
	initialize_log_lock();
}

void log_entry(const char* tag, const char* data)
{
	lock_log_lock();
	fprintf(LogFile, "{\"log_type\":\"%s\", \"data\":\"%s\"}\n", tag, data);
	fflush(LogFile);
	unlock_log_lock();
}

void release_log(void)
{
	release_log_lock();

	fclose(LogFile);
	LogFile = NULL;
}

void initialize_log_lock(void)
{

}

void lock_log_lock(void)
{

}

void unlock_log_lock(void)
{

}

void release_log_lock(void)
{

}
