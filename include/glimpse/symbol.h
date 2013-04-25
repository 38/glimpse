#ifndef __GLIMPSE_SYMBOL_H__
#define __GLIMPSE_SYMBOL_H__
#include <glimpse/future.h>
#include <glimpse/retval.h>
#include <glimpse/def.h>
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
