#include <typeflag.h>
#include <typesystem.h>
#include <vector.h>
#include <log.h>
#include <data.h>
void* glimpse_typeflag_vector_alloc(void* userdata)
{
	return glimpse_vector_new(sizeof(void*));
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
		void** addr = (void**)glimpse_vector_get(vec, i);
		if(NULL == addr) continue;
		glimpse_typesystem_typehandler_free_instance(*addr);
	}
	return 0;
}
int glimpse_typeflag_vector_free(void* data, void* userdata)  
{
	glimpse_vector_free((GlimpseVector_t*)data);
	return 0;
}
const char* glimpse_typeflag_vector_parse(const char* text, void* result, void* user_data)
{
	GlimpseTypeVectorParserParam_t* param = (GlimpseTypeVectorParserParam_t*)user_data;
	GlimpseTypeHandler_t* handler = param->basetype_handler;
	if(handler->parse == NULL)
		return text;
	const char* next;
	void* instance;
	while(*text)
	{
		//memory = handler->alloc(handler->alloc_data);
		instance = glimpse_typesystem_typehandler_new_instance(handler);
		if(NULL == instance)
		{
			GLIMPSE_LOG_ERROR("can not allocate memory");
			return NULL;
		}
		const char* next = handler->parse(text, instance, handler->parse_data);
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
			param->sep != 0 && 
			text[i] == param->sep[i];
			i ++);
		if(param->sep[i] == 0) text += i;
		else return text;
	}
}
void* glimpse_typeflag_sublog_alloc(void* userdata)
{
	return glimpse_data_instance_new((GlimpseDataModel_t*)userdata);
}
int glimpse_typeflag_sublog_free(void* data, void* userdata)
{
	glimpse_data_instance_free((GlimpseDataInstance_t*)data);
	return 0;
}
int glimpse_typeflag_sublog_init(void* data, void* userdata)
{
	return glimpse_data_instance_init((GlimpseDataInstance_t*)data);
}
int glimpse_typeflag_sublog_finalize(void* data, void* userdata)
{
	glimpse_data_instance_finalize((GlimpseDataInstance_t*)data);
	return 0;
}
const char* glimpse_typeflag_sublog_parse(const char* text, void* result, void* user_data)
{
	//TODO: parse the log 
}
