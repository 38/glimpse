/* typeparser.h - a parser that convert human-readable json-like type description into type descriptor
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
	
#ifndef __GLIMPSE_TYPEPARSER_H__
#define __GLIMPSE_TYPEPARSER_H__
#include <glimpse/typesystem.h>
#include <glimpse/def.h>
#ifndef GLIMPSE_MAX_TYPE_ALIAS
#	define GLIMPSE_MAX_TYPE_ALIAS 10240
#endif

#ifdef __cplusplus
extern "C"{
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
#ifdef __cplusplus
}
#endif
#endif
