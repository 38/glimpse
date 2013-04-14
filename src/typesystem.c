#ifndef __TYPESYSTEM_C__
#	define __TYPESYSTEM_C__
#endif
#include <typesystem.h>
#include <log.h>
#include <retval.h>
#include <string.h>
#include <vector.h>
#include <typeflag.h>
GlimpseTypeGroup_t *_glimpse_typesystem_typegroup_list[TYPEDESC_MAX_TYPEGROUPS];
int _glimpse_typesystem_typegroup_count = 0;
GlimpseVector_t* _glimpse_typesystem_known_handler;

GlimpseTypeDesc_t* glimpse_typesystem_typedesc_new(size_t sz_properties)
{
	GlimpseTypeDesc_t* ret = NULL;
	sz_properties += sizeof(GlimpseTypeDesc_t);
	ret = (GlimpseTypeDesc_t*)malloc(sz_properties);
	if(NULL == ret) return NULL;
	memset(ret,0,sz_properties);
	ret->properties_size = sz_properties - sizeof(GlimpseTypeDesc_t);
	return ret;
}
void glimpse_typesystem_typedesc_free(GlimpseTypeDesc_t* typedesc)
{
	if(NULL == typedesc) return;
	if(typedesc->flags & GLIMPSE_TYPEFLAG_VECTOR)
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
static int _glimpse_typesystem_typedesc_equal(GlimpseTypeDesc_t* a, GlimpseTypeDesc_t* b)
{
	if(a == b) return 1;
	if(a == NULL || b == NULL) return 0;
	if(a->flags != b->flags) return 0;
	if(a->flags & GLIMPSE_TYPEFLAG_VECTOR)
	{
		if(strcmp(a->param.vector.sep, b->param.vector.sep)) return 0;
		if(!_glimpse_typesystem_typedesc_equal(a->param.vector.basetype,b->param.vector.basetype)) return 0;
	}
	else if(a->flags & GLIMPSE_TYPEFLAG_SUBLOG)
	{
		if(a->param.sublog.tree != a->param.sublog.tree) return 0;
	}
	else if(a->flags & GLIMPSE_TYPEFLAG_MAP)
	{
		if(strcmp(a->param.map.target, b->param.map.target)) return 0;
	}
	else
	{
		if(strcmp(a->param.normal.group, b->param.normal.group)) return 0;
		if(a->properties_size != b->properties_size) return 0;
		int i;
		for(i = 0; i < a->properties_size; i ++)
			if(a->properties[i] != b->properties[i]) return 0;
	}
	return 1;
}
GlimpseTypeHandler_t* glimpse_typesystem_query(GlimpseTypeDesc_t* type)
{
	int i;
	for(i = 0; i < _glimpse_typesystem_known_handler->size; i ++)
	{
		GlimpseTypeHandler_t* known_handler = (GlimpseTypeHandler_t*)glimpse_vector_get(_glimpse_typesystem_known_handler, i);
		if(NULL == known_handler) continue;
		if(_glimpse_typesystem_typedesc_equal(known_handler->type, type)) 
		{
			glimpse_typesystem_typedesc_free(type);
			return known_handler;
		}
	}
	if(NULL == type) return NULL;
	
	/* make a new handler */
	GlimpseTypeHandler_t handler = {
		.type = type
	};
	if(type->flags & GLIMPSE_TYPEFLAG_VECTOR)
	{
		handler.vector_param[0] = (GlimpseTypeVectorParserParam_t*)malloc(sizeof(GlimpseTypeVectorParserParam_t));
		if(NULL == handler.vector_param[0]) return NULL;
		handler.vector_param[0]->basetype_handler = glimpse_typesystem_query(type->param.vector.basetype);
		handler.vector_param[0]->sep = type->param.vector.sep;
		if(NULL == handler.vector_param[0]->basetype_handler) return NULL; /* we does not need to free type handler any more */
		if(NULL == handler.vector_param[0]->sep) return NULL; /* we does not need to free type handler any more */
		handler.init_data = NULL;
		handler.free_data = type->param.vector.basetype;
		handler.alloc_data = NULL;
		handler.finalize_data = type->param.vector.basetype;
		handler.parse = glimpse_typeflag_vector_parse;
		handler.init = glimpse_typeflag_vector_init;
		handler.free = glimpse_typeflag_vector_free;
		handler.alloc = glimpse_typeflag_vector_alloc;
		handler.finalize = glimpse_typeflag_vector_finalize;
	}
	else if(type->flags & GLIMPSE_TYPEFLAG_SUBLOG)
	{
		/* TODO: sub log handlers */
	}
	else if(type->flags & GLIMPSE_TYPEFLAG_MAP)
	{
		/* TODO: map handlers */
	}
	else
	{
		for(i = 0; i < _glimpse_typesystem_typegroup_count; i ++)
			if(strcmp(_glimpse_typesystem_typegroup_list[i]->name, type->param.normal.group) == 0)
			{
				int ret = _glimpse_typesystem_typegroup_list[i]->resolve(type, &handler);
				if(ESUCCESS != ret) 
				{
					glimpse_typesystem_typedesc_free(type);
					return NULL;
				}
				break;
			}
		if(i == _glimpse_typesystem_typegroup_count) return NULL;
	}

	int rc = glimpse_vector_push(_glimpse_typesystem_known_handler, &handler);
	if(ESUCCESS != rc) return NULL;
	return (GlimpseTypeHandler_t*)glimpse_vector_get(_glimpse_typesystem_known_handler, 
			_glimpse_typesystem_known_handler->size - 1);
}

/*GlimpseTypeHandler_t* glimpse_typesystem_typehandler_new()
{
	GlimpseTypeHandler_t* ret = (GlimpseTypeHandler_t*)malloc(sizeof(GlimpseTypeHandler_t));
	if(NULL == ret) return NULL;
	memset(ret, 0, sizeof(GlimpseTypeHandler_t));
	return ret;
}*/
void glimpse_typesystem_typehandler_free(GlimpseTypeHandler_t* handler)
{
	if(NULL == handler) return;
	GlimpseTypePoolNode_t* p;
	for(p = handler->pool.occupied; p;)
	{
		GlimpseTypePoolNode_t* next = p->next;
		if(handler->finalize) handler->finalize(p->instance, handler->finalize_data);
		if(handler->free) handler->free(p->instance, handler->free_data);
		free(p);
		p = next;
	}	
	for(p = handler->pool.available; p;)
	{
		GlimpseTypePoolNode_t* next = p->next;
		if(handler->free) handler->free(p->instance, handler->free_data);
		free(p);
		p = next;
	}	
	//for vector we does not need to free all element, because it will be done with other handler
	//because all handler are allocated in _glimpse_typesystem_known_handler, we does not need free them
}
GlimpseTypeInstance_t* glimpse_typesystem_typehandler_new_instance(GlimpseTypeHandler_t* handler)
{
	GlimpseTypePoolNode_t* ret;
	if(NULL == handler) return NULL;
	if(handler->pool.available) /* if there's some available data instance */
	{
		ret = handler->pool.available;
		handler->pool.available = handler->pool.available->next;
		ret->occupied = 1;
		GLIMPSE_LOG_DEBUG("reuse pooled memory at <0x%x>", ret->instance);
	}
	else
	{
		if(!handler->alloc) return NULL;
		ret = (GlimpseTypePoolNode_t*)malloc(sizeof(GlimpseTypePoolNode_t));
		if(NULL == ret) return NULL;
		ret->handler = handler;
		ret->prev = NULL;
		ret->next = NULL;
		ret->instance = handler->alloc(handler->alloc_data);
		ret->occupied = 1;
		if(NULL == ret->instance) 
		{
			free(ret);
			return NULL;
		}
		GLIMPSE_LOG_DEBUG("allocated new memory at <0x%x>", ret->instance);
	}
	int rc = 0;
	if(handler->init) rc = handler->init(ret->instance, handler->init_data);  /* init it */
	if(ESUCCESS != rc)  /*if init failed insert the node into available list*/
	{
		ret->next = ret->handler->pool.available;
		ret->handler->pool.available = ret;
		return NULL;
	}
	/* insert the new instance into occupied list */
	ret->next = ret->handler->pool.occupied;
	ret->prev = NULL;
	if(ret->handler->pool.occupied) ret->handler->pool.occupied->prev = ret;
	ret->handler->pool.occupied = ret;
	return &ret->instance;
}
void glimpse_typesystem_typehandler_free_instance(GlimpseTypeInstance_t* instance)
{
	if(NULL == instance) return;
	GlimpseTypePoolNode_t* node = (GlimpseTypePoolNode_t*)instance;
	if(!node->occupied) return;
	if(node->handler->finalize) node->handler->finalize(*instance, node->handler->finalize_data);
	node->occupied = 0;
	/* remove from occupied list */
	if(NULL == node->prev) /*first node*/
	{
		node->handler->pool.occupied = node->next;
		if(node->next) node->next->prev = NULL;
	}
	else if(NULL == node->next) /*last node*/
	{
		node->prev->next = NULL; 
	}
	else
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	/* add it to available list */
	node->prev = NULL;
	node->next = node->handler->pool.available;
	node->handler->pool.available = node;
}

int glimpse_typesystem_init()
{
	_glimpse_typesystem_known_handler = glimpse_vector_new(sizeof(GlimpseTypeHandler_t));
	if(NULL == _glimpse_typesystem_known_handler) 
		return EUNKNOWN;
	return ESUCCESS;
}
int glimpse_typesystem_cleanup()
{
	int i;
	for(i = 0; i < _glimpse_typesystem_known_handler->size; i ++)
	{
		GlimpseTypeHandler_t* handler = (GlimpseTypeHandler_t*)glimpse_vector_get(_glimpse_typesystem_known_handler,i);
		GlimpseTypeDesc_t* type = handler->type;
		if(handler->type->flags & GLIMPSE_TYPEFLAG_VECTOR)  /* free memroy for vector's parser parameter */
			if(handler->parse_data) free(handler->parse_data);
		glimpse_typesystem_typehandler_free(handler);
		glimpse_typesystem_typedesc_free(type);
	}
	glimpse_vector_free(_glimpse_typesystem_known_handler);
	return ESUCCESS;
}
