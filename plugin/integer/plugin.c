/* plugin.c -   
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
	
#include <TypeAPI.h>
#include <stdio.h>
#include <integer/integer.h>
#include <integer/fixlength.h>
extern void alias_types();

void* alloc_int(void* data)
{
	GlimpseIntegerProperties_t* properties = (GlimpseIntegerProperties_t*)data;
	size_t size = 0;
	switch(properties->Size)
	{
		case GlimpseInteger8:
			size = 1;
			break;
		case GlimpseInteger16:
			size = 2;
			break;
		case GlimpseInteger32:
			size = 4;
			break;
		case GlimpseInteger64:
			size = 5;
			break;
		case GlimpseIntegerVariant:
			PLUGIN_LOG_ERROR("we can not support big integer yet");
			size = 0;
			break;
		default:
			PLUGIN_LOG_ERROR("unsupported integer type");
			size = 0;
	}
	if(0 == size) return NULL;
	return ObjAlloc(size);
}
int free_int(void* data, void* userdata)
{
	ObjFree(data);
	return 0;
}
char* tostring_int(GlimpseTypeHandler_t* handler, char* buffer, size_t size)
{
	if(NULL == handler) return NULL;
	GlimpseIntegerProperties_t* properties = (GlimpseIntegerProperties_t*)handler->type->properties;
	char *p = buffer;
	p += glimpse_snprintf(p, size - (p - buffer), "Integer{");
	p += glimpse_snprintf(p, size - (p - buffer), "size:");
#define PRINT_ENUM(name) \
	case name:\
		p += glimpse_snprintf(p, size - (p - buffer), "%s", #name);\
		break
	switch(properties->Size)
	{
		PRINT_ENUM(GlimpseInteger8);
		PRINT_ENUM(GlimpseInteger16);
		PRINT_ENUM(GlimpseInteger32);
		PRINT_ENUM(GlimpseInteger64);
		PRINT_ENUM(GlimpseIntegerVariant);
	}
	p += glimpse_snprintf(p, size - (p - buffer), ", signed:%s, format:", (properties->Signed?"true":"false") );
	switch(properties->Representation)
	{
		PRINT_ENUM(GlimpseIntegerBin);
		PRINT_ENUM(GlimpseIntegerOct);
		PRINT_ENUM(GlimpseIntegerDec);
		PRINT_ENUM(GlimpseIntegerHex);
	}

	p += glimpse_snprintf(p, size - (p - buffer), ", heading:%s , ", properties->Leading);
	p += glimpse_snprintf(p, size - (p - buffer), "heading_after_sign:%s}", properties->LeadingAfterSign);
	return p;
}
int resolve_int(const GlimpseTypeDesc_t* type, GlimpseTypeHandler_t* handler)
{
	if(NULL == type || NULL == handler) return -1;
	GlimpseIntegerProperties_t* properties = (GlimpseIntegerProperties_t*)type->properties;
	handler->parse_data = properties;
	handler->alloc_data = properties;
	handler->free_data = properties;
	handler->alloc = alloc_int;
	handler->free = free_int; 
	handler->init_data = NULL;
	handler->init = NULL; 		/* we does not need initialize */
	handler->finalize_data = NULL;
	handler->finalize = NULL;
	handler->tostring = tostring_int;
	switch(properties->Size)
	{
/* Local Macro, undefined after used */
#define _CASE_SIZE(len)\
		case GlimpseInteger##len:\
			switch(properties->Representation)\
			{\
				case GlimpseIntegerBin:\
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_bin_i##len;\
					else handler->parse = glimpse_integer_fixlength_bin_u##len;\
					break;\
				case GlimpseIntegerOct:\
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_oct_i##len;\
					else handler->parse = glimpse_integer_fixlength_oct_u##len;\
					break;\
				case GlimpseIntegerDec:\
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_dec_i##len;\
					else handler->parse = glimpse_integer_fixlength_dec_u##len;\
					break;\
				case GlimpseIntegerHex:\
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_hex_i##len;\
					else handler->parse = glimpse_integer_fixlength_hex_u##len;\
					break;\
				default:\
					return -1;\
			}\
			break
		_CASE_SIZE(8);
		_CASE_SIZE(16);
		_CASE_SIZE(32);
		_CASE_SIZE(64);
		case GlimpseIntegerVariant:
			//TODO: big integer
			PLUGIN_LOG_WARNING("Integer Plugin does not support big integer yet");
			return -1;
			break;
		default:
			return -1;
#undef _CASE_SIZE
	}
	PLUGIN_LOG_DEBUG("Integer, Size = %d, Signed = %d, Representation = %d Assigned to parser at <0x%x>", 
	                 properties->Size, 
					 properties->Signed, 
					 properties->Representation,
					 handler->parse);
	return 0;
}
int int_property_parser(const char* name, const char* value, void* buffer)
{
#define IFSTREQ(var,val) if(strcmp(var, val) == 0)
	GlimpseIntegerProperties_t* prop = (GlimpseIntegerProperties_t*)buffer;
	IFSTREQ(name,"size")
	{
		IFSTREQ(value,"8") prop->Size = GlimpseInteger8;
		else IFSTREQ(value,"16") prop->Size = GlimpseInteger16;
		else IFSTREQ(value,"32") prop->Size = GlimpseInteger32;
		else IFSTREQ(value,"64") prop->Size = GlimpseInteger64;
		else IFSTREQ(value,"variant") prop->Size = GlimpseIntegerVariant;
		else PLUGIN_LOG_WARNING("invaild integer size %s", value);
	}
	else IFSTREQ(name,"signed")
	{
		IFSTREQ(value,"true") prop->Signed = 1;
		else IFSTREQ(value,"false") prop->Signed = 0;
		else PLUGIN_LOG_WARNING("integer.signed should be either `true' or `false'");
	}
	else IFSTREQ(name,"format")
	{
		IFSTREQ(value,"dec") prop->Representation = GlimpseIntegerDec;
		else IFSTREQ(value,"hex") prop->Representation = GlimpseIntegerHex;
		else IFSTREQ(value,"oct") prop->Representation = GlimpseIntegerOct;
		else IFSTREQ(value,"bin") prop->Representation = GlimpseIntegerBin;
		else PLUGIN_LOG_WARNING("invalid format %s", value);
	}
	else IFSTREQ(name,"heading")
		prop->Leading = StrDup(value);
	else IFSTREQ(name,"heading_after_sign")
		prop->LeadingAfterSign = StrDup(value);
	else 
		PLUGIN_LOG_WARNING("unknown propery for integer");
	return 0;
}
GlimpseTypeGroup_t int_type_group = {
	.name = "integer",
	.resolve = resolve_int,
	.set_property = int_property_parser,
	.property_size = sizeof(GlimpseIntegerProperties_t)
};
int plugin_int_init()
{
	RegisterTypeGroup(&int_type_group);
	alias_types();
	return 0;
}
GlimpsePluginMetaData(TypeAPI)
	GlimpsePluginName("integer");
	GlimpsePluginVersion(0,0,1);
	GlimpsePluginAPICallBack(TypeAPI, OnInitialized) = plugin_int_init; 
GlimpsePluginEndMetaData
