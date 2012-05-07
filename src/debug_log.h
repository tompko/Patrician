#ifndef DEBUG_LOG
#define DEBUG_LOG

void initialize_log(void);
void log_entry(const char* tag, const char* data);
void release_log(void);

#ifdef _NDEBUG
#define INIT_LOG()
#define LOG(...)
#define RELEASE_LOG()
#else
#define INIT_LOG() initialize_log()
#define LOG(tag, data) log_entry(tag, data)
#define RELEASE_LOG() release_log()
#endif

#endif