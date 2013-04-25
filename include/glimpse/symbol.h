/* symbol.h - export & import a symbol  
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
