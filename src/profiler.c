#include <profiler.h>
#include <log.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <inttypes.h>
static volatile GlimpseProfilerWatcher_t * _glimpse_profiler_watchers[GLIMPSE_PROFILER_MAX_WATCHER];
static uint32_t _glimpse_profiler_watcher_count = 0;
static inline const char* _glimpse_profiler_print_table(const char* str, int width)
{
    int len = strlen(str);
    int pl = len;
	int i;
    if(len > width) len = len - width;
    else len = 0;
    for(i = 0 ; i < pl - len ; i ++)
    {
        fputc(*str,stderr);
        str ++;
    }
    for(i = pl - len ; i < width ; i ++)
        fputc(' ',stderr);
    fputc('|',stderr);
    return str;
}
static inline void _glimpse_profiler_print_header(int width)
{
	int i;
	for(i = 0; i < width; i ++)
		fputc('-',stderr);
	fputc('+',stderr);
}
static inline void _glimpse_profiler_print_row(int N, int* widths, const char ** strs)
{
	int i;
    for(;;)
    {
        int flag = 0;
        for(i = 0 ; i < N && !flag ; i ++)
            if(*strs[i] != 0) flag = 1;
        if(flag == 0) break;
        for(i = 0 ; i < N ; i ++)
            strs[i] = _glimpse_profiler_print_table(strs[i],widths[i]);
        fputc('\n',stderr);
    }
    for(i = 0 ; i < N ; i ++)
        _glimpse_profiler_print_header(widths[i]);
    fputc('\n',stderr);
}
void glimpse_profiler_print_summary()
{
    int N = 6;
	int i;
    char buffer[10][10000] = {"        file        ",
							  "   line   ",
							  " function ",
							  "               statement                ",
							  "        count       ",
							  "        time        "};
    const char* ptr[10] = {buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]};
    int widths[] = {20,10,10,40,20,20};
    for(i = 0 ; i < N ; i ++)
        _glimpse_profiler_print_header(widths[i]);
    fputc('\n',stderr);
    _glimpse_profiler_print_row(N,widths,ptr);
    for(i = 0 ; i < _glimpse_profiler_watcher_count ; i ++)
    {
		volatile GlimpseProfilerWatcher_t* watcher = _glimpse_profiler_watchers[i];
        snprintf(buffer[0],10000,"%s",watcher->file);
        snprintf(buffer[1],10000,"%10d",watcher->line);
        snprintf(buffer[2],10000,"%s",watcher->function);
        snprintf(buffer[3],10000,"%s",watcher->statement);
        snprintf(buffer[4],10000,"%20"PRIu64,watcher->count);
        snprintf(buffer[5],10000,"%20"PRIu64,watcher->elapsed);
        const char* ptr[10] = {buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]};
        _glimpse_profiler_print_row(N,widths,ptr);
    }
}
uint64_t glimpse_profiler_rdtsc()
{
	uint32_t hi, lo;
	asm volatile("rdtsc": "=a"(lo), "=d"(hi));
	return (((uint64_t)hi)<<32)|lo;
}
void glimpse_profiler_watcher_init(volatile GlimpseProfilerWatcher_t* watcher, const char* file, const char* statement, const char* function, int line)
{
	if(_glimpse_profiler_watcher_count == 0)
		atexit(glimpse_profiler_print_summary);
	watcher->initialized = 1;
	watcher->file = file;
	watcher->line = line;
	watcher->statement = statement;
	watcher->function = function;
	watcher->count = 0;
	watcher->elapsed = 0;
	if(_glimpse_profiler_watcher_count < GLIMPSE_PROFILER_MAX_WATCHER) 
		_glimpse_profiler_watchers[_glimpse_profiler_watcher_count++] = watcher;
	else
		GLIMPSE_LOG_WARNING("the limit of number of profile watcher reached");
}
