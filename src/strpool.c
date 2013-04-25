/* strpool.c -   
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
	
#include <malloc.h>
#include <string.h>
#include <glimpse/strpool.h>
#include <glimpse/vector.h>
static GlimpseVector_t* _glimpse_strpool_table;
int glimpse_strpool_init()
{
	_glimpse_strpool_table = (GlimpseVector_t*)glimpse_vector_new(sizeof(char*));
	if(NULL == _glimpse_strpool_table) return GLIMPSE_EUNKNOWN;
	return GLIMPSE_ESUCCESS;
}
int glimpse_strpool_cleanup()
{
	int i = 0;
	for(i = 0; i < _glimpse_strpool_table->size; i ++)
	{
		void* str = *(void**)glimpse_vector_get(_glimpse_strpool_table, i);
		free(str);
	}
	glimpse_vector_free(_glimpse_strpool_table);
	return GLIMPSE_ESUCCESS;
}
char* glimpse_strpool_new(const char* str)
{
	if(NULL == str) return NULL;
	size_t len = strlen(str);
	void* mem = malloc(len + 1);
	memcpy(mem, str, len + 1);
	if(glimpse_vector_push(_glimpse_strpool_table, &mem) != GLIMPSE_ESUCCESS)
	{
		free(mem);
		return NULL;
	}
	return (char*)mem;
}
