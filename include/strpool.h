#ifndef __STRPOOL_H__
#define __STRPOOL_H__
#include <retval.h>
int glimpse_strpool_init();
int glimpse_strpool_cleanup();
char* glimpse_strpool_new(const char* str);
#endif
