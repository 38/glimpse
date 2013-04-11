#include <typesystem.h>
#include <log.h>
#include <retval.h>
#include <string.h>

TypeGroup_t *_typesystem_typegroup_list[TYPEDESC_MAX_TYPEGROUPS];
int _typesystem_typegroup_count = 0;

TypeDesc_t* typesystem_newdesc(size_t sz_properties)
{
	TypeDesc_t* ret = NULL;
	sz_properties += sizeof(TypeDesc_t);
	ret = (TypeDesc_t*)malloc(sz_properties);
	if(NULL == ret) return NULL;
	memset(ret,0,sz_properties);
	return ret;
}
int typesystem_register_typegroup(TypeGroup_t* typegroup)
{
	int i;
	if(NULL == typegroup || NULL == typegroup->resolve || NULL == typegroup->name) return EINVAILDARG;
	if(_typesystem_typegroup_count >= TYPEDESC_MAX_TYPEGROUPS)
	{
		GLIMPSE_LOG_ERROR("Too many typegourps registered");
		return ETOOMANYTG;
	}
	for(i = 0; i < _typesystem_typegroup_count; i ++)
		if(strcmp(_typesystem_typegroup_list[i]->name, typegroup->name) == 0) 
		{
			GLIMPSE_LOG_ERROR("conflicted typegroup name %s",typegroup->name);
			return EUNKNOWN;
		}
	_typesystem_typegroup_list[_typesystem_typegroup_count++] = typegroup;
	GLIMPSE_LOG_DEBUG("typegroup %s registered", typegroup->name);
	return ESUCCESS;
}
int typesystem_query(TypeDesc_t* type, TypeHandler_t* handler)
{
	int i;
	if(NULL == type || NULL == handler) return EINVAILDARG;
	for(i = 0; i < _typesystem_typegroup_count; i ++)
		if(strcmp(_typesystem_typegroup_list[i]->name, type->group) == 0)
		{
			int ret = _typesystem_typegroup_list[i]->resolve(type, handler);
			return ret;
		}
	return ENOTFOUND;
}

