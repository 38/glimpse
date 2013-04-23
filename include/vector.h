#ifndef __VECTOR_H__
#define __VECTOR_H__
#include <future.h>
#include <stdlib.h>
#include <retval.h>
#ifndef GLIMPSE_VECTOR_INIT_CAP
#	define GLIMPSE_VECTOR_INIT_CAP 32
#endif
typedef struct _glimpse_vector_t{
	size_t capacity;
	size_t size;
	size_t elem_size;
	char* data;
} GlimpseVector_t;
GlimpseVector_t* glimpse_vector_new(size_t elem_size);
void glimpse_vector_init(GlimpseVector_t* vector);
void glimpse_vector_free(GlimpseVector_t* vector);

int glimpse_vector_push(GlimpseVector_t* vector, void* data);

static inline void* glimpse_vector_get(GlimpseVector_t* vector, int idx);

static inline void* glimpse_vector_get(GlimpseVector_t* vector, int idx)
{
	if(NULL == vector) return NULL;
	if(idx >= vector->size) return NULL;
	return (void*)(vector->data + vector->elem_size * idx);
}
#endif
