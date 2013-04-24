#ifndef __TYPESYSTEM_C__
#	define __TYPESYSTEM_C__
#endif
#include <typesystem.h>
#include <log.h>
#include <retval.h>
#include <string.h>
#include <vector.h>
#include <typeflag.h>
#include <stdio.h>
static GlimpseTypeGroup_t *_glimpse_typesystem_typegroup_list[TYPEDESC_MAX_TYPEGROUPS];
static int _glimpse_typesystem_typegroup_count = 0;
static GlimpseVector_t* _glimpse_typesystem_known_handler;

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
	/* because all types are record in the _glimpse_typesystem_known_handler, so 
	 * we does not need to free them recursively 
	 * but in some cases, the typedesc might not be managed by typesystem.
	 * and it requires to be freed recursively 
	 * the member type in a registered type MUST be a registered type
	 */
	if(0 == typedesc->registered &&
	   typedesc->builtin_type  == GLIMPSE_TYPE_BUILTIN_VECTOR && 
	   NULL != typedesc->param.vector.basetype &&
	   0 == typedesc->param.vector.basetype->registered)
		glimpse_typesystem_typedesc_free(typedesc->param.vector.basetype);
	free(typedesc);
}
int glimpse_typesystem_typedesc_set_property(GlimpseTypeDesc_t* desc ,const char* key, const char* value)
{
	int i;
	if(NULL == desc || NULL == key || NULL == value) return EINVAILDARG;
	if(desc->builtin_type != GLIMPSE_TYPE_BUILTIN_NONE) return ESUCCESS;
	for(i = 0; i < _glimpse_typesystem_typegroup_count; i ++)
		if(strcmp(_glimpse_typesystem_typegroup_list[i]->name, desc->param.normal.group) == 0)
		{
			int rc = EINVAILDARG;
			if(desc->param.normal.group) 
				rc = _glimpse_typesystem_typegroup_list[i]->set_property(key, value, desc->properties);
			return rc;
		}
	return ENOTFOUND;
}
GlimpseTypeDesc_t* glimpse_typesystem_typedesc_dup(GlimpseTypeDesc_t* type)
{
	if(NULL == type) return NULL;
	size_t size = sizeof(GlimpseTypeDesc_t) + type->properties_size;
	GlimpseTypeDesc_t* ret = (GlimpseTypeDesc_t*)malloc(size);
	if(NULL == ret) return NULL;
	memcpy(ret, type, size);
	return ret;
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
size_t glimpse_typesystem_sizeof_typegroup_prop(const char* name)
{
	int i;
	for(i = 0; i < _glimpse_typesystem_typegroup_count; i ++)
		if(strcmp(_glimpse_typesystem_typegroup_list[i]->name, name) == 0)
			return _glimpse_typesystem_typegroup_list[i]->property_size;
	return 0;
}
static int _glimpse_typesystem_typedesc_equal(GlimpseTypeDesc_t* a, GlimpseTypeDesc_t* b)
{
	int i;
	if(a == b) return 1;
	if(a == NULL || b == NULL) return 0;
	if(a->builtin_type != b->builtin_type) return 0;
	switch(a->builtin_type)
	{
		case GLIMPSE_TYPE_BUILTIN_VECTOR: 
#ifdef STRING_SEPERATOR_SUPPORT
			if(strcmp(a->param.vector.sep, b->param.vector.sep)) return 0;
#else
			if(a->param.vector.sep != b->param.vector.sep) return 0;
#endif
			if(!_glimpse_typesystem_typedesc_equal(a->param.vector.basetype,b->param.vector.basetype)) return 0;
			break;
		case GLIMPSE_TYPE_BUILTIN_SUBLOG:
			if(a->param.sublog.tree != b->param.sublog.tree) return 0;
			break;
		case GLIMPSE_TYPE_BUILTIN_MAP:
			if(strcmp(a->param.map.target, b->param.map.target)) return 0;
			break;
		case GLIMPSE_TYPE_BUILTIN_NONE:
			if(strcmp(a->param.normal.group, b->param.normal.group)) return 0;
			if(a->properties_size != b->properties_size) return 0;
			for(i = 0; i < a->properties_size; i ++)
				if(a->properties[i] != b->properties[i]) return 0;
			break;
		default:
			GLIMPSE_LOG_WARNING("invalid builtin type %d\n", a->builtin_type);
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
			if(!type->registered) glimpse_typesystem_typedesc_free(type);
			char buffer[1024];
			if(glimpse_typesystem_typehandler_tostring(known_handler, buffer, sizeof(buffer)))
				GLIMPSE_LOG_DEBUG("queried an known handler: %s", buffer);
			return known_handler;
		}
	}
	if(NULL == type) return NULL;
	
	/* make a new handler */
	GlimpseTypeHandler_t handler = {
		.type = type
	};
	switch(type->builtin_type)
	{
		case GLIMPSE_TYPE_BUILTIN_VECTOR:
			handler.vector_parser_param[0] = (GlimpseTypeVectorParserParam_t*)malloc(sizeof(GlimpseTypeVectorParserParam_t));
			if(NULL == handler.vector_parser_param[0]) goto ERR_VEC;
			handler.vector_parser_param[0]->basetype_handler = glimpse_typesystem_query(type->param.vector.basetype);
			if(NULL == handler.vector_parser_param[0]->basetype_handler) goto ERR_VEC; /* we does not need to free type handler any more */
			/* because the basetype might be freed after query the type system. (the base type is a known type)
			 * so it's unsafe to use the type->param.vector.basetype. we need maintain the pointer */
			type->param.vector.basetype = handler.vector_parser_param[0]->basetype_handler->type; /* this is safe, because it's owned by the basetype */
			handler.vector_parser_param[0]->sep = type->param.vector.sep;
#ifdef STRING_SEPERATOR_SUPPORT
			if(NULL == handler.vector_parser_param[0]->sep) goto ERR_VEC;
#endif
			handler.init_data = NULL;
			handler.free_data = type->param.vector.basetype;
			handler.alloc_data = NULL;
			handler.finalize_data = type->param.vector.basetype;
			handler.parse = glimpse_typeflag_vector_parse;
			handler.init = glimpse_typeflag_vector_init;
			handler.free = glimpse_typeflag_vector_free;
			handler.alloc = glimpse_typeflag_vector_alloc;
			handler.finalize = glimpse_typeflag_vector_finalize;
			handler.tostring = glimpse_typeflag_vector_tostring;
			break;
ERR_VEC:
			if(handler.vector_parser_param[0]) free(handler.vector_parser_param[0]);
			return NULL;
		case GLIMPSE_TYPE_BUILTIN_SUBLOG:
			if(NULL == type->param.sublog.tree) return NULL;
			if(NULL == type->param.sublog.tree->model) return NULL;
			handler.sublog_parser_param[0] = type->param.sublog.tree;
			handler.sublog_alloc_param[0] = type->param.sublog.tree->model;
			handler.init_data = NULL;
			handler.free_data = NULL;
			handler.finalize_data = NULL;
			handler.parse = glimpse_typeflag_sublog_parse;
			handler.alloc = glimpse_typeflag_sublog_alloc;
			handler.free  = glimpse_typeflag_sublog_free;
			handler.init  = glimpse_typeflag_sublog_init;
			handler.finalize = glimpse_typeflag_sublog_finalize;
			handler.tostring = glimpse_typeflag_sublog_tostring;
			break;
		case GLIMPSE_TYPE_BUILTIN_MAP:
			//TODO: handler for map 
			break;
		case GLIMPSE_TYPE_BUILTIN_NONE:
			for(i = 0; i < _glimpse_typesystem_typegroup_count; i ++)
				if(strcmp(_glimpse_typesystem_typegroup_list[i]->name, type->param.normal.group) == 0)
				{
					int ret = _glimpse_typesystem_typegroup_list[i]->resolve(type, &handler);
					if(ESUCCESS == ret) break;
				}
			if(i == _glimpse_typesystem_typegroup_count) 
			{
				glimpse_typesystem_typedesc_free(type);
				return NULL;
			}
			break;
		default:
			GLIMPSE_LOG_ERROR("invalid builtin type %d\n", type->builtin_type);
	}
	
	int rc = glimpse_vector_push(_glimpse_typesystem_known_handler, &handler);
	if(ESUCCESS != rc) return NULL;
	type->registered = 1;
	/* print log */
	char buffer[1024];
	if(glimpse_typesystem_typehandler_tostring(&handler, buffer, sizeof(buffer)))
		GLIMPSE_LOG_DEBUG("registed new type handler: %s", buffer);
#ifdef THREAD_SAFE
	GlimpseTypeHandler_t* ret = (GlimpseTypeHandler_t*)glimpse_vector_get(_glimpse_typesystem_known_handler, 
																		  _glimpse_typesystem_known_handler->size - 1);
	if(NULL == ret) return NULL;
	pthread_mutex_init(&ret->pool.mutex, NULL);
	return ret;
#else
	return (GlimpseTypeHandler_t*)glimpse_vector_get(_glimpse_typesystem_known_handler, 
			_glimpse_typesystem_known_handler->size - 1);
#endif
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
	for(p = handler->pool.occupied_list; p;)
	{
		GlimpseTypePoolNode_t* next = p->next;
		//if(handler->finalize) handler->finalize(p->instance, handler->finalize_data);
		/* finalize here will cause problems:
		 * for example a vector<int>, handler of int has been freed previously,
		 * then vector's finalize will require int to finalize, this will cause 
		 * problem.
		 * so a memery's life cycle should be
		 * alloc  -->  init --> finalize -->init --> finalize --> init --> free
		 * so the last finalize should be done during free function if needed 
		 */
		if(handler->free) handler->free(p->instance, handler->free_data);
		free(p);
		p = next;
	}	
	for(p = handler->pool.available_list; p;)
	{
		GlimpseTypePoolNode_t* next = p->next;
		if(handler->free) handler->free(p->instance, handler->free_data);
		free(p);
		p = next;
	}
#ifdef THREAD_SAFE
	pthread_mutex_destroy(&handler->pool.mutex);
#endif
	//for vector we does not need to free all element, because it will be done with other handler
	//because all handler are allocated in _glimpse_typesystem_known_handler, we does not need free them
}
void* glimpse_typesystem_typehandler_alloc_instance(GlimpseTypeHandler_t* handler)
{
	GlimpseTypePoolNode_t* ret;
	if(!handler->alloc) return NULL;
	ret = (GlimpseTypePoolNode_t*)malloc(sizeof(GlimpseTypePoolNode_t));
	if(NULL == ret) return NULL;
	ret->handler = handler;
	ret->prev = NULL;
	ret->next = NULL;
	ret->instance = handler->alloc(handler->alloc_data);
	ret->occupied = 1;
#ifdef THREAD_SAFE
	pthread_mutex_init(&ret->mutex, NULL);
#endif
	if(NULL == ret->instance) 
	{
		free(ret);
		return NULL;
	}
	if(ESUCCESS != glimpse_typesystem_instance_object_alias(ret->instance, ret))
	{
		GLIMPSE_LOG_ERROR("memory returned by alloc is not a valid type instance object");
		free(ret);
		return NULL;
	}
	GLIMPSE_LOG_DEBUG("allocated new memory at <0x%x>", ret->instance);
	return ret;
}
char* glimpse_typesystem_typehandler_tostring(GlimpseTypeHandler_t* handler, char* buffer,size_t size)
{
	if(NULL == handler || NULL == buffer) return NULL;
	char* ret = buffer;
	if(handler->tostring) 
		ret = handler->tostring(handler, ret, size);
	else
		ret += glimpse_snprintf(ret, size, "Type{group:%s}", handler->type->param.normal.group);
	return ret;
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
		if(handler->type->builtin_type == GLIMPSE_TYPE_BUILTIN_VECTOR)  /* free memroy for vector's parser parameter */
			if(handler->parse_data) free(handler->parse_data);
		glimpse_typesystem_typehandler_free(handler);
		glimpse_typesystem_typedesc_free(type);
	}
	glimpse_vector_free(_glimpse_typesystem_known_handler);
	return ESUCCESS;
}
void* glimpse_typesystem_instance_object_alloc(size_t size)
{
	GlimpseTypeInstanceObject_t* ret = (GlimpseTypeInstanceObject_t*)malloc(size + sizeof(GlimpseTypeInstanceObject_t)); /* malloc is threadsafe */
	if(NULL == ret) return NULL;
	ret->magic = GLIMPSE_TYPE_INSTANCE_OBJECT_MAGIC;
	return ret->data;
}
void glimpse_typesystem_instance_object_free(void* data)
{
	if(NULL == data) return;
	if(!glimpse_typesystem_instance_object_check(data)) GLIMPSE_LOG_WARNING("memory at <0x%x> is not a instance object");
	GlimpseTypeInstanceObject_t* ret = (GlimpseTypeInstanceObject_t*)((char*)data - sizeof(GlimpseTypeInstanceObject_t));
	free(ret);  /* free is threadsafe */
}
int glimpse_typesystem_instance_object_alias(void* data, GlimpseTypePoolNode_t* pool_obj)
{
	if(!glimpse_typesystem_instance_object_check(data)) return EINVAILDARG;
	if(NULL == pool_obj) return EINVAILDARG;
	GlimpseTypeInstanceObject_t* ret = (GlimpseTypeInstanceObject_t*)((char*)data - sizeof(GlimpseTypeInstanceObject_t));
	ret->pool_obj = pool_obj;
	return ESUCCESS;
}
#ifdef THREAD_SAFE
int glimpse_typesystem_instance_object_lock(void* data)
{
	if(NULL == data) return -1;
	if(!glimpse_typesystem_instance_object_check(data)) 
	{
		GLIMPSE_LOG_WARNING("memory at <0x%x> is not a instance object");
		return -1;
	}
	GlimpseTypeInstanceObject_t* ret = (GlimpseTypeInstanceObject_t*)((char*)data - sizeof(GlimpseTypeInstanceObject_t));
	return pthread_mutex_lock(&ret->mutex);
}
int glimpse_typesystem_instance_object_unlock(void* data)
{
	if(NULL == data) return -1;
	if(!glimpse_typesystem_instance_object_check(data))
	{
		GLIMPSE_LOG_WARNING("memory at <0x%x> is not a instance object");
		return -1;
	}
	GlimpseTypeInstanceObject_t* ret = (GlimpseTypeInstanceObject_t*)((char*)data - sizeof(GlimpseTypeInstanceObject_t));
	return pthread_mutex_unlock(&ret->mutex);
}
int glimpse_typesystem_instance_object_trylock(void* data)
{
	if(NULL == data) return -1;
	if(!glimpse_typesystem_instance_object_check(data))
	{
		GLIMPSE_LOG_WARNING("memory at <0x%x> is not a instance object");
		return -1;
	}
	GlimpseTypeInstanceObject_t* ret = (GlimpseTypeInstanceObject_t*)((char*)data - sizeof(GlimpseTypeInstanceObject_t));
	return pthread_mutex_trylock(&ret->mutex);
}
#endif
char** glimpse_typesystem_list_knowntypes()
{
	int i;
	char** ret = (char**)malloc(sizeof(char*) * (_glimpse_typesystem_known_handler->size + 1));
	if(NULL == ret) goto ERR;
	memset(ret, 0, sizeof(char*) * (_glimpse_typesystem_known_handler->size + 1));
	for(i = 0; i < _glimpse_typesystem_known_handler->size; i ++)
	{
		GlimpseTypeHandler_t* handler = (GlimpseTypeHandler_t*)glimpse_vector_get(_glimpse_typesystem_known_handler, i);
		char buffer[10240];
		if(NULL == glimpse_typesystem_typehandler_tostring(handler, buffer, sizeof(buffer))) goto ERR;
		int len = strlen(buffer);
		ret[i] = (char*)malloc(len + 1);
		if(NULL == ret[i]) goto ERR;
		strcpy(ret[i], buffer);
	}
	return ret;
ERR:
	if(ret)
	{
		for(i = 0; i < _glimpse_typesystem_known_handler->size; i ++)
			if(ret[i]) free(ret[i]);
		free(ret);
	}
	return NULL;
}
