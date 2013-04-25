/* thread.c -   
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
	
#include <string.h>
#include <malloc.h>

#include <glimpse/thread.h>
GlimpseThreadData_t* glimpse_thread_data_new()
{
	GlimpseThreadData_t* ret = (GlimpseThreadData_t*)malloc(sizeof(GlimpseThreadData_t));
	if(NULL == ret) return NULL;
	memset(ret, 0, sizeof(GlimpseThreadData_t));
#ifdef HANDLER_STACK
	ret->stack.true_val = 1;
#else
	ret->__true__ = 0;
#endif
	return ret;
}
void glimpse_thread_data_free(GlimpseThreadData_t* data)
{
	if(data) free(data);
}
void glimpse_thread_data_init(GlimpseThreadData_t* data)
{
	if(NULL == data) return;
#ifdef HANDLER_STACK
	data->stack.ptr = 0;
	data->stack.true_val ++;
#else
	data->__true__ ++;
#endif
}
