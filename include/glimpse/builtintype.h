/* builtintype.h - gourp of functions that manipulates builtin types
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
	
#ifndef __GLIMPSE_BUILTINTYPE_H__
#define __GLIMPSE_BUILTINTYPE_H__
#include <glimpse/future.h>
#include <glimpse/typesystem.h>
#include <glimpse/def.h>
/* interfaces for process log,vector,map as normal type */

/* vector interface */
void* glimpse_builtintype_vector_alloc(void* userdata);
int glimpse_builtintype_vector_init(void* data, void* userdata);
int glimpse_builtintype_vector_finalize(void* data, void* userdata);
int glimpse_builtintype_vector_free(void* data, void* userdata);
const char* glimpse_builtintype_vector_parse(const char* text, void* result, void* user_data, void* thread_data);
char* glimpse_builtintype_vector_tostring(GlimpseTypeHandler_t* type, char* buffer, size_t size);


/*sub log interface */
void* glimpse_builtintype_sublog_alloc(void* userdata);
int glimpse_builtintype_sublog_init(void* data, void* userdata);
int glimpse_builtintype_sublog_finalize(void* data, void* userdata);
int glimpse_builtintype_sublog_free(void* data, void* userdata);
const char* glimpse_builtintype_sublog_parse(const char* text, void* result, void* user_data, void* thread_data);
char* glimpse_builtintype_sublog_tostring(GlimpseTypeHandler_t* type, char* buffer, size_t size);

#ifdef LAZY_INSTANCE
int glimpse_builtintype_vector_cleanup(void* data, void* userdata);
int glimpse_builtintype_sublog_cleanup(void* data, void* userdata);
#endif
#endif
