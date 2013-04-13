#include <typeflag.h>
#include <typesystem.h>
#include <vector.h>
void* glimpse_typeflag_vector_alloc(void* userdata)
{
	return glimpse_vector_new(sizeof(void*));
}
void glimpse_typeflag_vector_init(void* data, void* userdata)
{
	glimpse_vector_init((GlimpseVector_t*)data);
}
void glimpse_typeflag_vector_free(void* data, void* userdata)
{
	glimpse_vector_free((GlimpseVector_t*)data);
}
const char* glimpse_typeflag_vector_parse(const char* text, void* result, void* user_data)
{
	GlimpseTypeHandler_t* handler = (GlimpseTypeHandler_t*)user_data;
	if(handler->init == NULL || handler->free == NULL || handler->init == NULL || handler->parse == NULL)
		return text;
	const char* next;
	void* memory; 
	while(*text)
	{
		memory = handler->alloc(handler->alloc_data);
		if(NULL == memory)
		{
			GLIMPSE_LOG_ERROR("can not allocate memory");
			return NULL;
		}
		handler->init(memory, handler->init_data);
		const char* next = handler->parse(text, memory, handler->parse_data);
		if(NULL == next)
		{
			GLIMPSE_LOG_ERROR("failed to parse text");
			handler->free(memory, handler->free_data);
			return text;
		}
		int rc = glimpse_vector_push((GlimpseVector_t*)result, &memory);
		if(ESUCCESS != rc)
		{
			GLIMPSE_LOG_WARNING("failed to insert to vector");
			handler->free(memory, handler->free_data);
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
