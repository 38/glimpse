/* def.h - helper functions and macroes
 *
 * Copyright 2013 Hao Hou <ghost89413@gmail.com>
 * 
 * This file is part of Glimpse, a fast, flexible key-value scanner.
 * 
 * Glimpse is free software: you can redistribute it and/or modify it under the terms 
 * of the GNU General Public License as published by the Free Software Foundation, 
 * either version 3 of the License, or (at your option) any later version.
 *
 * Glimpse is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with Glimpse. 
 * If not, see http://www.gnu.org/licenses/.
 *
 */
	
#ifndef __GLIMPSE_DEF_H__
#define __GLIMPSE_DEF_H__
#include <stdio.h>
#include <stdarg.h>

#include <glimpse/future.h>

/* some macro defined in this file */
#define GLIMPSE_OFFSET_OF(type,member) ((size_t)&(((type*)0)->member))
static inline int glimpse_snprintf(char* buffer, size_t size, const char* fmt, ...)
{
	va_list ap;
	va_start(ap,fmt);
	int ret = vsnprintf(buffer, size, fmt, ap);
	va_end(ap);
	if(ret > size) return size;
	else return ret;
}
#endif
