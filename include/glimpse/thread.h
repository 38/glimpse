/* thread.h -  thread related data 
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
	
#ifndef __GLIMPSE_THREAD_H__
#define __GLIMPSE_THREAD_H__

#include <stdint.h>

#include <glimpse/future.h>
#include <glimpse/stack.h>
#include <glimpse/def.h>
typedef struct _glimpse_thread_data{
#ifdef HANDLER_STACK
	GlimpseStack_t stack;
#else
	uint32_t is_term[256];
	uint32_t __true__;
#endif
	/* add some other data here */
} GlimpseThreadData_t;
GlimpseThreadData_t* glimpse_thread_data_new();
void glimpse_thread_data_free(GlimpseThreadData_t* data);
void glimpse_thread_data_init(GlimpseThreadData_t* data);
#endif
