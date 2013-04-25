/* version.c -   
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
	
#include <glimpse/future.h>
#define Y "=yes"
#define N "=no"
const char* glimpse_name = "glimpse";
const char* glimpse_version = VERSION;
const char* glimpse_configure = 
"multi-char-seperator"
#ifdef STRING_SEPERATOR_SUPPORT
	Y
#else
	N
#endif
",parser-stack"
#ifdef HANDLER_STACK
	Y
#else
	N
#endif
",multi-threading"
#ifdef THREAD_SAFE
	Y
#else
	N
#endif
",lazy-instance"
#ifdef LAZY_INSTANCE
	Y
#else
	N
#endif
",char-table"
#ifdef CHAR_HASH_TABLE
	Y
#else
	N
#endif
;
