#include <typesystem.h>
#include <log.h>
#include <retval.h>
#include <string.h>

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
	for(i = 0; i < _glimpse_typesystem_typegroup_count; i ++)
		if(strcmp(_glimpse_typesystem_typegroup_list[i]->name, type->group) == 0)
		{
			handler->type = type;
			int ret = _glimpse_typesystem_typegroup_list[i]->resolve(type, handler);
			return ret;
		}
	return ENOTFOUND;
}

