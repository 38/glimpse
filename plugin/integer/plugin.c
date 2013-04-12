#include <TypeAPI.h>
#include <stdio.h>
#include <integer/integer.h>
#include <integer/fixlength.h>
int resolve_int(const GlimpseTypeDesc_t* type, GlimpseTypeHandler_t* handler)
{
	if(NULL == type || NULL == handler) return -1;
	GlimpseIntegerProperties_t* properties = (GlimpseIntegerProperties_t*)type->properties;
	handler->parse_data = properties;
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
			//TODO
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
GlimpseTypeGroup_t int_type_group = {
	.name = "integer",
	.resolve = resolve_int
};
int init()
{
	//TODO: return value ?
	RegisterTypeGroup(&int_type_group);
}
GlimpsePluginMetaData(TypeAPI)
	GlimpsePluginName("integer");
	GlimpsePluginVersion(0,0,1);
	GlimpsePluginAPICallBack(TypeAPI, OnInitialized) = init; 
GlimpsePluginEndMetaData
