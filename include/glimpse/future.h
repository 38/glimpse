/* future.h -   
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
	
#ifndef __FUTURE_H__
#define __FUTURE_H__
/* 
 * future features contains the future that currently 
 * does not support yet
 */
#ifdef FUTURE_FEATURES
#	define STRING_SEPERATOR_SUPPORT /* TODO: support to a string seperator */ 
#endif
/*
 * experimental features is features that default does
 * not compile to the source code
 */
#ifdef EXPERIMENTAL_FEATURES
#	define THREAD_SAFE			/* make Glimpse thread-safe, but might slow the parser down */
#	define HANDLER_STACK		/* this make Glimpse record the parser stack, may slow the parser down */
#endif

/* default features */

#ifndef NO_DEFAULT_FEATURES
#	define LAZY_INSTANCE	/* makes the data instance object does not free the memery after its death, 
							 * hoping this will reduce the time of calling instance new/free */
#endif
/*
 * deprecated features is features that does not use any more 
 */
#ifdef DEPRECATED_FEATURES
#	define CHAR_HASH_TABLE
#endif

#endif 
