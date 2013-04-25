#ifndef __GLIMPSE_PROFILER_H__
#define __GLIMPSE_PROFILER_H__
#include <stdint.h>
#include <glimpse/def.h>
#ifndef GLIMPSE_PROFILER_MAX_WATCHER
#	define GLIMPSE_PROFILER_MAX_WATCHER 1024
#endif
uint64_t glimpse_profiler_rdtsc();
typedef struct _glimpse_profiler_watcher_t{
	uint8_t initialized;
	const char* file;
	const char* statement;
	const char* function;
	int line;
	uint64_t elapsed;
	uint64_t count;
} GlimpseProfilerWatcher_t;
void glimpse_profiler_watcher_init(volatile GlimpseProfilerWatcher_t* watcher, const char* file, const char* statement, const char* function, int line);
#ifdef ENABLE_PROFILER
#	define GLIMPSE_PROFILER_HEAD(s) \
		volatile static GlimpseProfilerWatcher_t __local_watcher__ = {.initialized = 1};\
		if(!__local_watcher__.initialized) glimpse_profiler_watcher_init(&__local_watcher__, __FILE__, s, __FUNCTION__, __LINE__);\
		uint64_t begin = glimpse_profiler_rdtsc();
#	define GLIMPSE_PROFILER_TAIL \
		uint64_t end = glimpse_profiler_rdtsc();\
		__local_watcher__.elapsed += end - begin;\
		__local_watcher__.count ++;
#	define GLIMPSE_EXPRESSION_PROFILER(expression) ({\
		GLIMPSE_PROFILER_HEAD(#expression) \
		typeof(expression) v = (expression);\
		GLIMPSE_PROFILER_TAIL \
		v;\
	})
#	define GLIMPSE_STATEMENT_PROFILER(statement) do{\
	GLIMPSE_PROFILER_HEAD(#statement) \
	statement;\
	GLIMPSE_PROFILER_TAIL\
}while(0)
#else
#	define GLIMPSE_EXPRESSION_PROFILER(exp) exp
#	define GLIMPSE_STATEMENT_PROFILER(stm) stm
#endif

#endif
