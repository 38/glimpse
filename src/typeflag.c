#include <typeflag.h>
#include <typesystem.h>
#include <vector.h>
#include <log.h>
void* glimpse_typeflag_vector_alloc(void* userdata)
{
	return glimpse_vector_new(sizeof(GlimpseTypeInstance_t*));
}
int glimpse_typeflag_vector_init(void* data, void* userdata)
{
	glimpse_vector_init((GlimpseVector_t*)data);
	return 0;
}
int glimpse_typeflag_vector_finalize(void* data, void* userdata)
{
	int i;
	GlimpseVector_t* vec = (GlimpseVector_t*) data;
	GlimpseTypeHandler_t* handler = (GlimpseTypeHandler_t*) userdata;
	for(i = 0; i < vec->size; i ++)
	{
		GlimpseTypeInstance_t* instance = (GlimpseTypeInstance_t*)glimpse_vector_get(vec, i);
		if(NULL == instance) continue;
		glimpse_typesystem_typehandler_free_instance(instance);
	}
}
int glimpse_typeflag_vector_free(void* data, void* userdata)  
{
	glimpse_vector_free((GlimpseVector_t*)data);
	return 0;
}
const char* glimpse_typeflag_vector_parse(const char* text, void* result, void* user_data)
{
	GlimpseTypeHandler_t* handler = (GlimpseTypeHandler_t*)user_data;
	if(handler->init == NULL || handler->free == NULL || handler->init == NULL || handler->parse == NULL)
		return text;
	const char* next;
	GlimpseTypeInstance_t* instance;
	while(*text)
	{
		//memory = handler->alloc(handler->alloc_data);
		instance = glimpse_typesystem_typehandler_new_instance(handler);
		if(NULL == instance)
		{
			GLIMPSE_LOG_ERROR("can not allocate memory");
			return NULL;
		}
		const char* next = handler->parse(text, *instance, handler->parse_data);
		if(NULL == next)
		{
			GLIMPSE_LOG_ERROR("failed to parse text");
			glimpse_typesystem_typehandler_free_instance(instance);
			return text;
		}
		int rc = glimpse_vector_push((GlimpseVector_t*)result, &instance);
		if(ESUCCESS != rc)
		{
			GLIMPSE_LOG_WARNING("failed to insert to vector");
			glimpse_typesystem_typehandler_free_instance(instance);
		}
		text = next;
		int i;
		for(i = 0; 
			text[i] != 0 && 
			handler->type->param.vector.sep[i] != 0 && 
			text[i] == handler->type->param.vector.sep[i];
			i ++);
		if(handler->type->param.vector.sep[i] == 0) text += i;
		else return text;
	}
}
/*
void* glimpse_typeflag_sublog_alloc(void* userdata)
{
	//TODO
}
void glimpse_typeflag_sublog_init(void* data, void* userdata)
{
	//TODO
}
void glimpse_typeflag_sublog_free(void* data, void* userdata)
{
	//TODO
}
const char* glimpse_typeflag_sublog_parse(const char* text, void* result, void* user_data)
{
	//TODO
}

void* glimpse_typeflag_map_alloc(void* userdata)
{
	//TODO
}
void glimpse_typeflag_map_init(void* data, void* userdata)
{
	//TODO
}
void glimpse_typeflag_map_free(void* data, void* userdata)
{
	//TODO
}

const char* glimpse_typeflag_map_parse(const char* text, void* result, void* user_data)
{
	//TODO
}
*/
