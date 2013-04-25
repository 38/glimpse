#include <string.h>
#include <malloc.h>
#include <glimpse/vector.h>
#include <glimpse/log.h>
#include <glimpse/typesystem.h>
GlimpseVector_t* glimpse_vector_new(size_t elem_size)
{
	if(0 == elem_size) return NULL;
	GlimpseVector_t* ret = (GlimpseVector_t*)glimpse_typesystem_instance_object_alloc(sizeof(GlimpseVector_t));
	if(NULL == ret) return NULL;
	ret->capacity = GLIMPSE_VECTOR_INIT_CAP;
	ret->size = 0;
	ret->elem_size = elem_size;
#ifdef LAZY_INSTANCE
	ret->max_size = 0;
#endif
	ret->data = (char*)malloc(elem_size * GLIMPSE_VECTOR_INIT_CAP);
	if(NULL == ret->data)
	{
		free(ret);
		return NULL;
	}
	return ret;
}
void glimpse_vector_init(GlimpseVector_t* vector)
{
#ifdef LAZY_INSTANCE
	if(vector->max_size < vector->size) 
		vector->max_size = vector->size;
#endif
	vector->size = 0;
}
void glimpse_vector_free(GlimpseVector_t* vector)
{
	if(NULL == vector) return;
	if(vector->data) free(vector->data);
	glimpse_typesystem_instance_object_free(vector);
}

static int _glimpse_vector_resize(GlimpseVector_t* vector)
{
	GLIMPSE_LOG_DEBUG("resizing vector from %d to %d", vector->capacity, vector->capacity * 2);
	if(NULL == vector) return GLIMPSE_EINVAILDARG;
	if(NULL == vector->data) return GLIMPSE_EINVAILDARG;
	size_t new_capacity = vector->capacity * 2;
	void* data = realloc(vector->data, new_capacity * vector->elem_size);
	if(NULL == data) return GLIMPSE_EUNKNOWN;
	vector->data = data;
	vector->capacity = new_capacity;
	return GLIMPSE_ESUCCESS;
}
int glimpse_vector_push(GlimpseVector_t* vector, void* data)
{
	if(vector->size == vector->capacity)
	{
		int rc = _glimpse_vector_resize(vector);
		if(GLIMPSE_ESUCCESS != rc) return rc;
	}
	void* addr = vector->data + vector->size * vector->elem_size;
	memcpy(addr, data, vector->elem_size);
	vector->size ++;
	return GLIMPSE_ESUCCESS;
}

