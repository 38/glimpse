/* address.h -   
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
	
#ifndef __GLIMPSE_ADDRESSING_H__
#define __GLIMPSE_ADDRESSING_H__
#include <stdarg.h>

#include <glimpse/future.h>
#include <glimpse/def.h>
#include <glimpse/typesystem.h>
#include <glimpse/data.h>
#include <glimpse/vector.h>

#ifndef GLIMPSE_MAX_ADDRESS_OPS
#	define GLIMPSE_MAX_ADDRESS_OPS 64
#endif
/*
 * the Glimpse addressing system is a interface for user
 * to fetch the data stored in data instance easily.
 * you can convert a human-readable address , with which 
 * machine can not address the data quickly, into a 
 * offset-based address, which can be adressed very fast
 *
 * The syntax for human-readable address is quite simple:
 * 	for example, a log named mylog:
 * 	Log{
 * 		a:vector{
 * 			seperator:'#',
 * 			sublog{
 * 				a:uint32d,
 * 				b:uint32d,
 * 				c:uint32d
 * 			}
 * 		},
 * 		b:uint32d,
 *		c:uint32d
 * 	}
 *	
 *	a ==> a pointer to vector a 
 *	a[0] ==> a pointer to the fisrt element in vector a
 *	a[?]  ==> a pointer to some element in vector a, the index will determined when fetch function called 
 *	a[#] ==> a pointer to size of vector a
 *	a[0].a ==> a pointer to member a in the first element in vector a 
 */
#define GLIMPSE_ADDRESSING_VECTOR_SIZE -1
#define GLIMPSE_ADDRESSING_VECTOR_VARIABLE_INDEX -2
typedef struct _glimpse_address_operations{
	enum {
		GLIMPSE_ADDRESSING_TYPE_LOG,
		GLIMPSE_ADDRESSING_TYPE_VEC
	} type:1;
	union{
		struct{
			GlimpseDataOffset_t offset;
		} log;
		struct{
			int index;  //special addressing index : GLIMPSE_ADDRESSING_VECTOR_SIZE <== '[#]' ; GLIMPSE_ADDRESSING_VECTOR_VARIABLE_INDEX <=='[?]'
		} vec;
	} oper;
} GlimpseAddressOperations_t;

typedef struct _glimpse_address{
	uint8_t count;
	GlimpseAddressOperations_t op[GLIMPSE_MAX_ADDRESS_OPS];
} GlimpseAddress_t;

int glimpse_address_init();
int glimpse_address_cleanup();

GlimpseAddress_t* glimpse_address_resolve(const char* text);  /* resolve human-readable address into index based address */

/* WARNING: for performance reason, we do not check the vailidity of adress and data
 * 			user SHOULD GUAREENTEE THE VAILIDITY BY NOT MODIFYING ADDRESS MANUALLY
 */
#ifdef CHECKED_FETCH
#	define glimpse_address_fetch(args...) __glimpse_address_fetch_chk__(args,-1)
#else
#	define glimpse_address_fetch __glimpse_address_fetch__
#endif
static inline void* __glimpse_address_fetch_chk__(void** data, GlimpseAddress_t* addr, ...)
{
	void* current_data = ((char*)data) - sizeof(GlimpseDataInstance_t);
	int i, id;
	va_list ap;
	va_start(ap, addr);
	for(i = 0; i < addr->count && current_data; i ++)
	{
		if(addr->op[i].type == GLIMPSE_ADDRESSING_TYPE_LOG) /* addressing inside a log */
		{
			current_data = ((GlimpseDataInstance_t*)current_data)->data[addr->op[i].oper.log.offset];
		}
		else
			switch(addr->op[i].oper.vec.index)
			{
				case GLIMPSE_ADDRESSING_VECTOR_SIZE:
					current_data = &((GlimpseVector_t*)current_data)->size;
					break;
				case GLIMPSE_ADDRESSING_VECTOR_VARIABLE_INDEX:
					id = va_arg(ap,int);
					if(id == -1) goto ERR;
					current_data = *(void**)glimpse_vector_get((GlimpseVector_t*)current_data, id);
					break;
				default:
					current_data = *(void**)glimpse_vector_get((GlimpseVector_t*)current_data,addr->op[i].oper.vec.index);
			}
	}
	if(va_arg(ap,int) != -1) 
	{
ERR:
		GLIMPSE_LOG_WARNING("you might have given fetch function some wrong parameter");
		return NULL;
	}
	va_end(ap);
	return current_data;
}
static inline void* __glimpse_address_fetch__(void** data, GlimpseAddress_t* addr, ...)
{
	void* current_data = ((char*)data) - sizeof(GlimpseDataInstance_t);
	int i;
	va_list ap;
	va_start(ap, addr);
	for(i = 0; i < addr->count && current_data; i ++)
	{
		if(addr->op[i].type == GLIMPSE_ADDRESSING_TYPE_LOG) /* addressing inside a log */
		{
			current_data = ((GlimpseDataInstance_t*)current_data)->data[addr->op[i].oper.log.offset];
		}
		else
			switch(addr->op[i].oper.vec.index)
			{
				case GLIMPSE_ADDRESSING_VECTOR_SIZE:
					current_data = &((GlimpseVector_t*)current_data)->size;
					break;
				case GLIMPSE_ADDRESSING_VECTOR_VARIABLE_INDEX:
					current_data = *(void**)glimpse_vector_get((GlimpseVector_t*)current_data, va_arg(ap,int));
					break;
				default:
					current_data = *(void**)glimpse_vector_get((GlimpseVector_t*)current_data,addr->op[i].oper.vec.index);
			}
	}
	va_end(ap);
	return current_data;
}
#endif
