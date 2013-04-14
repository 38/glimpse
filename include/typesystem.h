#ifndef __TYPESYSTEM_H__
#define __TYPESYSTEM_H__
#include <stdlib.h>
#include <stdint.h>
#ifndef TYPEDESC_MAX_TYPEGROGUPS
#	define TYPEDESC_MAX_TYPEGROUPS 1024
#endif
#define GLIMPSE_TYPEFLAG_SUBLOG 0x1
#define GLIMPSE_TYPEFLAG_VECTOR 0x2
#define GLIMPSE_TYPEFLAG_EXTRAPULATE 0x4
#define GLIMPSE_TYPEFLAG_MAP 0x8
/* SUBLOG, VECTOR, MAP are exclusive */
/* Previous decleration for GlimpseParseTree_t 
 * which defined in file tree.h
 */
struct _glimpse_parse_tree;
typedef struct _glimpse_parse_tree GlimpseParseTree_t;
/* Type descriptor */
typedef struct _glimpse_typedesc{
	uint32_t flags; /*type flags*/
	union {
		struct{
			char* sep;    /* seprator of vector if the type decleared as a vector, valid when Vector Flag is set*/
			struct _glimpse_typedesc* basetype; /* base type of vector */
		} vector;
		struct {
			GlimpseParseTree_t* tree; /* indicate the parse method of sublog, valid when sublog flag is set*/
		} sublog;
		struct {
			const char* target; /* the target of the map */
		} map;
		struct {
			const char* group; /* which group dose the type belongs */
		} normal;
	} param;
	size_t properties_size;
	char  properties[0]; /* properties might different from group to group */
	/* DO NOT add any defination here */
} GlimpseTypeDesc_t;

/* memory pool for each type */
struct _glimpse_type_pool_node;
struct _glimpse_type_handler;
typedef void* GlimpseTypeInstance_t;
typedef struct _glimpse_type_pool_node{
	/* DO NOT add any declearation here */
	GlimpseTypeInstance_t instance;
	struct _glimpse_type_handler* handler;
	struct _glimpse_type_pool_node* prev; /* used only occupied */
	struct _glimpse_type_pool_node* next;
	uint8_t occupied:1;
} GlimpseTypePoolNode_t;
typedef struct _glimpse_type_pool{
	GlimpseTypePoolNode_t* occupied;
	GlimpseTypePoolNode_t* available;
} GlimpseTypePool_t;

/* contains handlers for each type, used for parse */
struct _glimpse_type_handler;
typedef struct _glimpse_type_vector_parser_param{
	const char* sep;
	struct _glimpse_type_handler* basetype_handler;
} GlimpseTypeVectorParserParam_t;
typedef struct _glimpse_type_handler{
	GlimpseTypeDesc_t* type; /* type information */
	
	/* the parse function process text into specified data type, the procudt store in result */
	GlimpseTypeVectorParserParam_t* vector_param[0];   /* vector reuse the parse_data as the element handler */
	GlimpseTypeDesc_t* sublog_tree[0];	/*reuse for sublog */
	void* parse_data; /* pass the additional data used by parse function */
	const char* (*parse)(const char* text, void* result, void* user_data);
	
	/* alloc memory for data storage */
	void* alloc_data;
	void* (*alloc)(void* user_data);

	/* free memory used by data */
	void* free_data;
	int (*free)(void* data, void* user_data);

	/* initialize data before use */
	void* init_data;
	int (*init)(void* data, void* user_data);

	/* finalize data after use */
	void* finalize_data;
	int (*finalize)(void* data, void* user_data);
	
	/* Add some new interface here to extend the framework */
	GlimpseTypePool_t pool;

} GlimpseTypeHandler_t;

/* type group */
typedef struct _glimpse_type_group{
	/* name of the type group */
	char* name;
	int (*resolve)(const GlimpseTypeDesc_t*, GlimpseTypeHandler_t*);
}GlimpseTypeGroup_t;

/* type descriptor manipulation */
GlimpseTypeDesc_t* glimpse_typesystem_typedesc_new(size_t sz_properties);
void glimpse_typesystem_typedesc_free(GlimpseTypeDesc_t* typedesc)
#ifndef __TYPESYSTEM_C__
	__attribute__((warning("this function may cause problem if typedesc has been already queried")))
#endif
;

/* type group manipulation */
int glimpse_typesystem_register_typegroup(GlimpseTypeGroup_t* typegroup);

/* handler operations */
//GlimpseTypeHandler_t* glimpse_typesystem_typehandler_new();  /* managed by vector, not needed any more */
void glimpse_typesystem_typehandler_free(GlimpseTypeHandler_t* handler);
GlimpseTypeInstance_t* glimpse_typesystem_typehandler_new_instance(GlimpseTypeHandler_t* handler);
void glimpse_typesystem_typehandler_free_instance(GlimpseTypeInstance_t* instance);


/* type system */
GlimpseTypeHandler_t* glimpse_typesystem_query(GlimpseTypeDesc_t* type);  
int glimpse_typesystem_init();
int glimpse_typesystem_cleanup();

#define GLIMPSE_TYPE_INSTANCE_GET_MEMORY(var) (*var)
#endif
