#ifndef __SYMBOL_H__
#define __SYMBOL_H__
#include <future.h>
#include <retval.h>
#include <def.h>
#ifndef GLIMPSE_SYMBOL_MAX_SYMBOL_NUMBER
#	define GLIMPSE_SYMBOL_MAX_SYMBOL_NUMBER 4099  /*a prime number*/
#endif
#ifndef GLIMPSE_SYMBOL_MAX_SYMBOL_LENGTH
#	define GLIMPSE_SYMBOL_MAX_SYMBOL_LENGTH 128
#endif
void glimpse_symbol_init();
int glimpse_symbol_exportsymbol(const char* symname, void* address);
void* glimpse_symbol_importsymbol(const char* symname);
#endif
