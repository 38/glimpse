#include <typesystem.h>
#include <log.h>
#include <retval.h>
#include <string.h>
#include <vector.h>
GlimpseTypeGroup_t *_glimpse_typesystem_typegroup_list[TYPEDESC_MAX_TYPEGROUPS];
int _glimpse_typesystem_typegroup_count = 0;

GlimpseTypeDesc_t* glimpse_typesystem_typedesc_new(size_t sz_properties)
{
	GlimpseTypeDesc_t* ret = NULL;
	sz_properties += sizeof(GlimpseTypeDesc_t);
	ret = (GlimpseTypeDesc_t*)malloc(sz_properties);
	if(NULL == ret) return NULL;
	memset(ret,0,sz_properties);
	return ret;
}
void glimpse_typesystem_typedesc_free(GlimpseTypeDesc_t* typedesc)
{
	if(NULL == typedesc) return;
	if(typedesc->flag & GLIMPSE_TYPEFLAG_VECTOR)
	{
		glimpse_typesystem_typedesc_free(typedesc->param.vector.basetype); 
	}
	free(typedesc);
}
int glimpse_typesystem_register_typegroup(GlimpseTypeGroup_t* typegroup)
{
	int i;
	if(NULL == typegroup || NULL == typegroup->resolve || NULL == typegroup->name) return EINVAILDARG;
	if(_glimpse_typesystem_typegroup_count >= TYPEDESC_MAX_TYPEGROUPS)
	{
		GLIMPSE_LOG_ERROR("Too many typegourps registered");
		return ETOOMANYTG;
	}
	for(i = 0; i < _glimpse_typesystem_typegroup_count; i ++)
		if(strcmp(_glimpse_typesystem_typegroup_list[i]->name, typegroup->name) == 0) 
		{
			GLIMPSE_LOG_ERROR("conflicted typegroup name %s",typegroup->name);
			return EUNKNOWN;
		}
	_glimpse_typesystem_typegroup_list[_glimpse_typesystem_typegroup_count++] = typegroup;
	GLIMPSE_LOG_DEBUG("typegroup %s registered", typegroup->name);
	return ESUCCESS;
}
int glimpse_typesystem_query(GlimpseTypeDesc_t* type, GlimpseTypeHandler_t* handler)
{
	int i;
	if(NULL == type || NULL == handler) return EINVAILDARG;
	/*if(type->flags & (GLIMPSE_TYPEFLAG_SUBLOG|
				      GLIMPSE_TYPEFLAG_VECTOR|
					  GLIMPSE_TYPEFLAG_MAP))
		return EINVAILDARG;*/
	if(type->flags & GLIMPSE_TYPEFLAG_VECTOR)
	{
		handler->type = type;
		handler->parse_data = glimpse_typesystem_typehandler_new();
		if(NULL == handler->parse_data) return EUNKNOWN;
		int rc = glimpse_typesystem_query(type->param.vector.basetype, handler->vector_element_handler[0]);
		if(ESUCCESS != rc) 
		{
			free(handler->parse_data);   /* handler_free will cause refree problem */
			return rc;
		}
		handler->init_data = NULL;
		handler->free_data = NULL;
		handler->alloc_data = NULL;
		handler->parse = glimpse_typeflag_vector_parse;
		handler->init = glimpse_typeflag_vector_init;
		handler->free = glimpse_typeflag_vector_free;
		handler->alloc = glimpse_typeflag_vector_alloc;
		return ESUCCESS;
	}
	else if(type->flags & GLIMPSE_TYPEFLAG_SUBLOG)
	{
		handler->type = type;
		handler->parse_data = type->param.sublog.tree;
		handler->init_data = NULL;
		handler->free_data = NULL;
		handler->alloc_data = NULL;
		handler->parse = glimpse_typeflag_sublog_parse;
		handler->init = glimpse_typeflag_sublog_init;
		handler->free = glimpse_typeflag_sublog_free;
		handler->alloc = glimpse_typeflag_sublog_alloc;
		return ESUCCESS;
	}
	else if(type->flags & GLIMPSE_TYPEFLAG_MAP)
	{
		handler->type = type;
		handler->parse_data = NULL;
		handler->init_data = NULL;
		handler->free_data = NULL;
		handler->alloc_data = NULL;
		handler->parse = glimpse_typeflag_map_parse;
		handler->init = glimpse_typeflag_map_init;
		handler->free = glimpse_typeflag_map_free;
		handler->alloc = glimpse_typeflag_map_alloc;
		return ESUCCESS;
	}
	for(i = 0; i < _glimpse_typesystem_typegroup_count; i ++)
		if(strcmp(_glimpse_typesystem_typegroup_list[i]->name, type->param.normal.group) == 0)
		{
			handler->type = type;
			int ret = _glimpse_typesystem_typegroup_list[i]->resolve(type, handler);
			return ret;
		}
	return ENOTFOUND;
}

GlimpseTypeHandler_t* glimpse_typesystem_typehandler_new()
{
	return (GlimpseTypeHandler_t*)malloc(sizeof(GlimpseTypeHandler_t));
}
void glimpse_typesystem_typehandler_free(GlimpseTypeHandler_t* handler)
{
	if(NULL == handler) return;
	if(handler->type->flags & GLIMPSE_TYPEFLAG_VECTOR)
	{
		glimpse_typesystem_typehandler_free(handler->vector_element_handler[0]);
	}
	free(handler);
}
