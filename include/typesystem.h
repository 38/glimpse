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
/* Previous decleration for GlimpseParseTree_t 
 * which defined in file tree.h
 */
struct _glimpse_parse_tree;
typedef struct _glimpse_parse_tree GlimpseParseTree_t;
/* Type descriptor */
typedef struct _glimpse_typedesc{
	uint32_t flag; /*type flags*/
	char* vector_sep;    /* seprator of vector if the type decleared as a vector, valid when Vector Flag is set*/
	GlimpseParseTree_t* sub_tree; /* indicate the parse method of sublog, valid when sublog flag is set*/
	/*TODO: put data reference here*/
	char* group; /* which group dose the type belongs */
	char  properties[0]; /* properties might different from group to group */
	/* DO NOT add any defination here */
} GlimpseTypeDesc_t;

/* contains handlers for each type, used for parse */
typedef struct _glimpse_type_handler{
	GlimpseTypeDesc_t* type; /* type information */
	
	/* the parse function process text into specified data type, the procudt store in result */
	void* parse_data; /* pass the additional data used by parse function */
	const char* (*parse)(const char* text, void* result, void* user_data);
	
	/* alloc memory for data storage */
	void* alloc_data;
	void* (*alloc)(void* user_data);

	/* free memory used by data */
	void* free_data;
	void (*free)(void* data, void* user_data);
	/* Add some new interface here to extend the framework */
} GlimpseTypeHandler_t;

/* type group */
typedef struct _glimpse_type_group{
	/* name of the type group */
	char* name;
	int (*resolve)(const GlimpseTypeDesc_t*, GlimpseTypeHandler_t*);
}GlimpseTypeGroup_t;

GlimpseTypeDesc_t* glimpse_typesystem_typedesc_new(size_t sz_properties);
void glimpse_typesystem_typedesc_free(GlimpseTypeDesc_t* typedesc);

int glimpse_typesystem_register_typegroup(GlimpseTypeGroup_t* typegroup);
int glimpse_typesystem_query(GlimpseTypeDesc_t* type, GlimpseTypeHandler_t* handler);  
#endif
