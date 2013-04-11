#ifndef __TYPESYSTEM_H__
#define __TYPESYSTEM_H__
#include <stdlib.h>
#include <tree.h>
#ifndef TYPEDESC_MAX_TYPEGROGUPS
#	define TYPEDESC_MAX_TYPEGROUPS 1024
#endif
#define TYPEFLAG_SUBLOG 0x1
#define TYPEFLAG_VECTOR 0x2
#define TYPEFLAG_EXTRAPULATE 0x4
#define TYPEFLAG_MAP 0x8
/* Type descriptor */
typedef struct _glimpse_typedesc{
	unsigned flag; /*type flags*/
	char* vector_sep;    /* seprator of vector if the type decleared as a vector, valid when Vector Flag is set*/
	ParseTree_t* sub_tree; /* indicate the parse method of sublog, valid when sublog flag is set*/ 
	char* group; /* which group dose the type belongs */
	char  properties[0]; /* properties might different from group to group */
	/* DO NOT add any defination here */
} GlimpseTypeDesc_t;
GlimpseTypeDesc_t* glimpse_typesystem_newdesc(size_t sz_properties);

/* contains handlers for each type, used for parse */
typedef struct _glimpse_type_handler{
	void* parse_data; /* pass the additional data used by parse function */
	/* the parse function process text into specified data type, the procudt store in result */
	const char* (*parse)(const char* text, void* result, void* additional); 
	/* Add some new interface here to extend the framework */
} GlimpseTypeHandler_t;

/* type group */
typedef struct _glimpse_type_group{
	/* name of the type group */
	char* name;
	int (*resolve)(const GlimpseTypeDesc_t* type, GlimpseTypeHandler_t* handler);
}GlimpseTypeGroup_t;

int glimpse_typesystem_register_typegroup(GlimpseTypeGroup_t* typegroup);
int glimpse_typesystem_query(GlimpseTypeDesc_t* type, GlimpseTypeHandler_t* handler);

#endif
