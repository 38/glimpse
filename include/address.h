#ifndef __ADDRESSING_H__
#define __ADDRESSING_H__
#include <typesystem.h>
#include <data.h>
#include <vector.h>
#include <stdarg.h>
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
typedef struct _glimpse_address_t{
	int builtin_type;			 /* the builtin-type for this data */
	GlimpseDataOffset_t offset;  /* the offset inside a data instance */
	int index;  /* if the builtin_type = vector, it indicates which data you what. if index = -1, it's a variant index,
				   index = -2 means the pointer to the vector's size
				   which is determined when user fetch the data */
	struct _glimpse_address_t* child;  /* if the data contains a sub-data-instance, the addressing should be recursively */
} GlimpseAddress_t;

int glimpse_address_init();
int glimpse_address_cleanup();

GlimpseAddress_t* glimpse_address_resolve(const char* text);  /* resolve human-readable address into index based address */
static inline void* glimpse_address_fetch(void** data, GlimpseAddress_t* addr, ...);

static inline void* glimpse_address_fetch(void** data, GlimpseAddress_t* addr, ...)
{
	GlimpseAddress_t* p;
	void** d = data;
	void* ret = NULL;
	GlimpseVector_t* vector;
	va_list ap;
	va_start(ap, addr);
	for(p = addr; p; p = p->child)
	{
		switch(p->builtin_type)
		{
			case GLIMPSE_TYPE_BUILTIN_MAP: 
				//TODO
				break;
			case GLIMPSE_TYPE_BUILTIN_VECTOR:
				vector = (GlimpseVector_t*)(d[p->offset]);
				if(NULL == vector) return NULL;
				if(p->index >= 0)
				{
					ret = glimpse_vector_get(vector, p->index);
				}
				else if(p->index == -1)
				{
					int idx = va_arg(ap,int);
					if(idx < 0) ret = NULL;
					else ret = glimpse_vector_get(vector, idx);
				}
				else if(p->index == -2)
				{
					ret = &vector->size;
				}
				data = (void**) ret;
				break;
			case GLIMPSE_TYPE_BUILTIN_SUBLOG:
			case GLIMPSE_TYPE_BUILTIN_NONE:
				data = (void**) (ret = data[p->offset]);
				break;
		}
	}
	if(va_arg(ap,int) != -1) GLIMPSE_LOG_ERROR("invalid argument for address fetch function");
	va_end(ap);
	return ret;
}
#endif
