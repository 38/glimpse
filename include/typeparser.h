#ifndef __TYPEPARSER_H__
#define __TYPEPARSER_H__
#include <typesystem.h>
#ifndef GLIMPSE_MAX_TYPE_ALIAS
#	define GLIMPSE_MAX_TYPE_ALIAS 10240
#endif
/*
 * the syntax for type description:
 * $type: $typename|$typename $parameters
 * $typename: Identifier
 * $parameter: {}|{property_list}
 * $property_list: $property|$property,$property_list
 * $property: Propname: $value
 * $value: String|$type
 * 
 * for example, the type T is a vector of 32bit unsigned decimal integer,
 * whoes seperator is '#'
 * T can be described as follow
 * vector{
 * 		sep: '#',
 * 		basetype: integer{
 * 			size: 32,
 * 			signed: true,
 * 			format: decimal,
 * 			heading: '',
 * 			heading_after_sign: ''
 * 		}
 * }
 * there are builtin type name, vector, sublog, map
 * you can also alias a type to a short name
 * if you assigned T with the name `vector_int'
 * you can use the name `vector_int' as the type above
 */
typedef struct _glimpse_type_alias_t{
	const char* name;
	GlimpseTypeDesc_t* type;
} GlimpseTypeAlias_t;
GlimpseTypeDesc_t* glimpse_typeparser_parse_type(const char* text);
int glimpse_typeparser_alias(GlimpseTypeDesc_t* desc, const char* name);
#endif
