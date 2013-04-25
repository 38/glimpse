#ifndef __GLIMPSE_STRPOOL_H__
#define __GLIMPSE_STRPOOL_H__
#include <glimpse/retval.h>
#include <glimpse/def.h>
int glimpse_strpool_init();
int glimpse_strpool_cleanup();
char* glimpse_strpool_new(const char* str);
#endif
