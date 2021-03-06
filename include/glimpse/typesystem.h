/* typesystem.h - The Glimpse Type System, Types, Handlers, Instance manipulation 
 *
 * Copyright 2013 Hao Hou <ghost89413@gmail.com>
 * 
 * This file is part of Glimpse, a fast, flexible key-value scanner.
 * 
 * Glimpse is free software: you can redistribute it and/or modify it under the terms 
 * of the GNU General Public License as published by the Free Software Foundation, 
 * either version 3 of the License, or (at your option) any later version.
 *
 * Glimpse is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with Glimpse. 
 * If not, see http://www.gnu.org/licenses/.
 *
 */
	
#ifndef __GLIMPSE_TYPESYSTEM_H__
#define __GLIMPSE_TYPESYSTEM_H__
#include <stdlib.h>
#include <stdint.h>

#ifdef THREAD_SAFE
#include <pthread.h>
#endif

#include <glimpse/future.h>
#include <glimpse/profiler.h>
#include <glimpse/def.h>
#include <glimpse/data.h>
#include <glimpse/tree.h>
#include <glimpse/retval.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef TYPEDESC_MAX_TYPEGROGUPS
#	define TYPEDESC_MAX_TYPEGROUPS 1024
#endif

typedef struct _glimpse_parse_tree GlimpseParseTree_t; 
typedef struct _glimpse_data_model_t GlimpseDataModel_t;

/* define builtin types */
typedef enum{
	GLIMPSE_TYPE_BUILTIN_NONE,
	GLIMPSE_TYPE_BUILTIN_VECTOR,
	GLIMPSE_TYPE_BUILTIN_SUBLOG,
	GLIMPSE_TYPE_BUILTIN_MAP
} GlimpseBuiltinTypeFlag_t;
struct _glimpse_parse_tree;

/* Type descriptor */
typedef struct _glimpse_typedesc{
	GlimpseBuiltinTypeFlag_t builtin_type:2; /* which of builtin type are selected */
	uint8_t extrapulated:1;					 /* is the data are extrapulated from other data, ranther than parsed from text? */
	uint8_t registered:1;					 /* the flag indicates if the desc registered in type system */
	/* following union are the parameter for each type */
	union {
		/* this type is a vector */
		struct{
#ifdef STRING_SEPERATOR_SUPPORT
			char* sep;    /* seprator of vector if the type decleared as a vector, valid when Vector Flag is set*/
#else
			char sep; 	  /* in the first version of glimpse we only support seperator of string */
#endif
			struct _glimpse_typedesc* basetype; /* base type of vector */
		} vector;
		/* this type refers to another log */
		struct {
			GlimpseParseTree_t* tree; /* indicate the parse method of sublog, valid when sublog flag is set*/
		} sublog;
		/* this type refers to another field */
		struct {
			const char* target; /* the target of the map */
		} map;
		/* this type is a normall type that defined by type plugin */
		struct {
			const char* group; /* which group dose the type belongs */
		} normal;
	} param;
	size_t properties_size; /* length of type properties */
	char  properties[0]; /* properties might different from group to group */
	/* DO NOT add any defination here */
} GlimpseTypeDesc_t;

/* memory pool for each type */
struct _glimpse_type_pool_node;
struct _glimpse_type_handler;
/* each type pool node manages a instance */
typedef struct _glimpse_type_pool_node{
	void* instance;	 /* address of instance */
	struct _glimpse_type_handler* handler; /* indicates operations */
	
	struct _glimpse_type_pool_node* prev; /* used only occupied */
	struct _glimpse_type_pool_node* next;
	
	uint8_t occupied:1;  /* is the memory currently in use */

#ifdef THREAD_SAFE
	pthread_mutex_t mutex;
#endif

} GlimpseTypePoolNode_t;

/* memery pool */
typedef struct _glimpse_type_pool{
#ifdef THREAD_SAFE
	pthread_mutex_t mutex;
#endif
	GlimpseTypePoolNode_t* occupied_list;	/* list of occupied memory */	
	GlimpseTypePoolNode_t* available_list;   /* list of available memory */
} GlimpseTypePool_t;

/* data object */
#define GLIMPSE_TYPE_INSTANCE_OBJECT_MAGIC 0xfc354786u
typedef struct _glimpse_type_instance_object{
#ifdef THREAD_SAFE
	pthread_mutex_t mutex;    /* mutex that for thread synchronizing */
#endif
	uint32_t magic;  /* the magic number indicates data is managed by type system */
	GlimpseTypePoolNode_t* pool_obj; /* reference to pool object that manage this memory */
	char data[0];	/* data section */
	/* memory for data instance, DO NOT add any defination here */
} GlimpseTypeInstanceObject_t;

/* data type for the parser parameter for vector */
struct _glimpse_type_handler;
typedef struct _glimpse_type_vector_parser_param{
#ifdef STRING_SEPERATOR_SUPPORT
	const char* sep;  /* seperator of the vector */
#else
	char sep;
#endif
	struct _glimpse_type_handler* basetype_handler; /* indicates the type of elements */
} GlimpseTypeVectorParserParam_t;

/* Type handler is a group of operation that use to data manipulation
 * the Glimpse Type System use the handler to manipulate data in various types
 * You can use a Type Descriptor to QUERY the type system in order to obtain a
 * Type handler. 
 */
typedef struct _glimpse_type_handler{

	GlimpseTypeDesc_t* type; /* type information */
	
	/* the parse function process text into specified data type, the procudt store in result */
	GlimpseTypeVectorParserParam_t* vector_parser_param[0];   /* vector reuse the parse_data as the element handler */
	GlimpseParseTree_t* sublog_parser_param[0];	/*reuse for sublog */
	/* DO NOT add any defination here */
	void* parse_data; /* pass the additional data used by parse function */
	const char* (*parse)(const char* text, void* result, void* user_data, void* thread_data); /* function for parse data from text */
	
	/* alloc memory for data storage */
	/* NOTICE: any initialization should not be performed in this function
	 *         you should only allocate memory for this data
	 */
	GlimpseDataModel_t* sublog_alloc_param[0];
	void* alloc_data; 
	void* (*alloc)(void* user_data);

	/* free memory used by data */
	/* NOTICE: release the memory only, DO NOT do any finalization */
	void* free_data;
	int (*free)(void* data, void* user_data);

	/* initialize data before use */
	void* init_data;
	int (*init)(void* data, void* user_data);

	/* finalize data after use */
	void* finalize_data;
	int (*finalize)(void* data, void* user_data);

#ifdef LAZY_INSTANCE
	/* for lazy instance, the instance will hold the memory even
	 * after its death. So a cleanup function is needed to release
	 * the memory, if user want to reduce the memory usage
	 */
	void* cleanup_data;
	int (*cleanup)(void* data, void* user_data); /* the cleanup function only release data instance, but not despose/finalize them */
#endif

	/* human readable type, for debug purpose */
	char* (*tostring)(struct _glimpse_type_handler* type, char* buffer, size_t size);
	
	/* Add some new interface here to extend the framework */


	GlimpseTypePool_t pool; /* memory pool for this data type */

} GlimpseTypeHandler_t;

/* type group */
typedef struct _glimpse_type_group{
	char* name;  /* group name */
	int (*set_property)(const char* name, const char* value, void* buffer);     /* parse the text propery and save them into desc */
	int (*resolve)(const GlimpseTypeDesc_t* desc, GlimpseTypeHandler_t* ret); /* function for resolve type */
	size_t property_size;  /* size of private property */
}GlimpseTypeGroup_t;

/* type descriptor manipulation */
GlimpseTypeDesc_t* glimpse_typesystem_typedesc_new(size_t sz_properties);
GlimpseTypeDesc_t* glimpse_typesystem_typedesc_dup(GlimpseTypeDesc_t* type) 
#ifndef USE_DUP_TYPEDESC
	__attribute__((deprecated))
#endif
;
void glimpse_typesystem_typedesc_free(GlimpseTypeDesc_t* typedesc);
int glimpse_typesystem_typedesc_set_property(GlimpseTypeDesc_t* desc ,const char* key, const char* value);

/* type group manipulation */
int glimpse_typesystem_register_typegroup(GlimpseTypeGroup_t* typegroup);
size_t glimpse_typesystem_sizeof_typegroup_prop(const char* name);

/* handler operations */
void glimpse_typesystem_typehandler_free(GlimpseTypeHandler_t* handler); 
GlimpseTypePoolNode_t* glimpse_typesystem_typehandler_alloc_instance(GlimpseTypeHandler_t* handler);
static inline void* glimpse_typesystem_typehandler_new_instance(GlimpseTypeHandler_t* handler);
static inline void glimpse_typesystem_typehandler_free_instance(void* instance);
#ifdef LAZY_INSTANCE
static inline int glimpse_typesystem_typehandler_init_instance(void* instance);
static inline int glimpse_typesystem_typehandler_fianlize_instance(void* instance);
#endif
char* glimpse_typesystem_typehandler_tostring(GlimpseTypeHandler_t* handler, char* buffer,size_t size);

/* instance object */
void* glimpse_typesystem_instance_object_alloc(size_t size);
void glimpse_typesystem_instance_object_free(void* data);
static inline int glimpse_typesystem_instance_object_check(void* data); /* check if the data is a instance object */
static inline GlimpseTypePoolNode_t* glimpse_typesystem_instance_object_get_pool(void* data); /* get pool node which manage the memory */
int glimpse_typesystem_instance_object_alias(void* data, GlimpseTypePoolNode_t* pool_obj); /*alias data with pool_obj*/
#ifdef THREAD_SAFE
int glimpse_typesystem_instance_object_lock(void* data);
int glimpse_typesystem_instance_object_unlock(void* data);
int glimpse_typesystem_instance_object_trylock(void* data);
#endif

/* type system */
GlimpseTypeHandler_t* glimpse_typesystem_query(GlimpseTypeDesc_t* type);  
int glimpse_typesystem_init();
int glimpse_typesystem_cleanup();

/* functions that used for cli interface */
char** glimpse_typesystem_list_knowntypes();   /* get all string description of known handler, do not recycle memory */

/* define inline functions */
static inline void* glimpse_typesystem_typehandler_new_instance(GlimpseTypeHandler_t* handler)
{
	GlimpseTypePoolNode_t* ret;
	if(NULL == handler) return NULL;
#ifdef THREAD_SAFE
	pthread_mutex_lock(&handler->pool.mutex);  /* lock the pool */
#endif
	if(handler->pool.available_list) /* if there's some available data instance */
	{
		ret = handler->pool.available_list;
		handler->pool.available_list = handler->pool.available_list->next;
		ret->occupied = 1;
		GLIMPSE_LOG_DEBUG("reuse pooled memory at <0x%x>", ret->instance);
	}
	else 
		ret = glimpse_typesystem_typehandler_alloc_instance(handler);
	int rc = 0;
	if(handler->init) rc = handler->init(ret->instance, handler->init_data);  /* init it */
	if(GLIMPSE_ESUCCESS != rc)  /*if init failed insert the node into available list*/
	{
		ret->next = ret->handler->pool.available_list;
		ret->handler->pool.available_list = ret;
		return NULL;
	}
	/* insert the new instance into occupied list */
	ret->next = ret->handler->pool.occupied_list;
	ret->prev = NULL;
	if(ret->handler->pool.occupied_list) ret->handler->pool.occupied_list->prev = ret;
	ret->handler->pool.occupied_list = ret;
#ifdef THREAD_SAFE
	pthread_mutex_unlock(&handler->pool.mutex);
#endif
	return ret->instance;
}
/* when lazy instance is on, the free function will be called only after user requested do that 
 * the finalize function do not call free function and return memory to memory pool
 * it simple hold the memory for next cycle.
 * And when user request a cleanup, the free function will be called and use the handler->cleanup
 * function to recycle it's memory
 */
static inline void glimpse_typesystem_typehandler_free_instance(void* instance)
{
	if(NULL == instance) return;
	GlimpseTypePoolNode_t* node = glimpse_typesystem_instance_object_get_pool(instance);
	if(NULL == node) return;
	if(!node->occupied) return;  /* if it's not in use */
#ifdef THREAD_SAFE
	pthread_mutex_lock(&node->handler->pool.mutex);
#endif
	/* if lazy instance is off, the finalize function will return the memory to the pool
	 * so it's ok to not clean up it.
	 * however, when the option is on, the finalize function do not return memory, so
	 * if you want to clean the memory a cleanup function is needed 
	 */
	if(node->handler->finalize) node->handler->finalize(instance, node->handler->finalize_data);
#ifdef LAZY_INSTANCE
	if(node->handler->cleanup) node->handler->cleanup(instance, node->handler->cleanup_data);
#endif
	node->occupied = 0;
#ifdef THREAD_SAFE
	pthread_mutex_unlock(&node->mutex);
#endif
	/* remove from occupied list */
	if(NULL == node->prev) /*first node*/
	{
		node->handler->pool.occupied_list = node->next;
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
	node->next = node->handler->pool.available_list;
	node->handler->pool.available_list = node;
#ifdef THREAD_SAFE
	pthread_mutex_unlock(&node->handler->pool.mutex);
#endif
}

static inline int glimpse_typesystem_instance_object_check(void* data)
{
	if(NULL == data) return 0;
	GlimpseTypeInstanceObject_t* ret = (GlimpseTypeInstanceObject_t*)((char*)data - sizeof(GlimpseTypeInstanceObject_t));
	if(ret->magic == GLIMPSE_TYPE_INSTANCE_OBJECT_MAGIC) return 1;
	return 0;
}

static inline GlimpseTypePoolNode_t* glimpse_typesystem_instance_object_get_pool(void* data)
{
#if 0   /* we do not check the pointer, for performance purpose */
	if(!glimpse_typesystem_instance_object_check(data)) return NULL;
#endif
	GlimpseTypeInstanceObject_t* ret = (GlimpseTypeInstanceObject_t*)((char*)data - sizeof(GlimpseTypeInstanceObject_t));
	return ret->pool_obj;
}
#ifdef LAZY_INSTANCE
static inline int glimpse_typesystem_typehandler_init_instance(void* instance)
{
	GlimpseTypePoolNode_t* node = glimpse_typesystem_instance_object_get_pool(instance);
	if(NULL == node) return GLIMPSE_EINVAILDARG;
	int rc = GLIMPSE_ESUCCESS;
	if(node->handler->init) rc = node->handler->init(instance, node->handler->init_data);
	return rc;
}
static inline int glimpse_typesystem_typehandler_fianlize_instance(void* instance)
{
	GlimpseTypePoolNode_t* node = glimpse_typesystem_instance_object_get_pool(instance);
	if(NULL == node) return GLIMPSE_EINVAILDARG;
	int rc = GLIMPSE_ESUCCESS;
	if(node->handler->finalize) rc = node->handler->finalize(instance, node->handler->finalize_data);
	return rc;
}
#endif
#ifdef __cplusplus
}
#endif
#endif
