#include <log.h>
#include <stdio.h>
#include <stdarg.h>
void glimpse_log_write(ErrorLevel level, const char* file, const char* function,int line, const char* fmt,...)
{
	static const char LevelChar[] = "FEWNITD";
	va_list ap;
	fprintf(stderr,"%c[%s@%s:%d] ",LevelChar[level],function,file,line);
	va_start(ap,fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
}
