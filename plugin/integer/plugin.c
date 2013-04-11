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
		case GlimpseInteger8:
			switch(properties->Representation)
			{
				case GlimpseIntegerBin: 
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_bin_i8;
					else handler->parse = glimpse_integer_fixlength_bin_u8;
					break;
				case GlimpseIntegerOct:
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_oct_i8;
					else handler->parse = glimpse_integer_fixlength_oct_u8;
					break;
				case GlimpseIntegerDec:
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_dec_i8;
					else handler->parse = glimpse_integer_fixlength_dec_u8;
					break;
				case GlimpseIntegerHex:
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_hex_i8;
					else handler->parse = glimpse_integer_fixlength_hex_u8;
					break;
			}
			break;
		case GlimpseInteger16:	
			switch(properties->Representation)
			{
				case GlimpseIntegerBin: 
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_bin_i16;
					else handler->parse = glimpse_integer_fixlength_bin_u16;
					break;
				case GlimpseIntegerOct:
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_oct_i16;
					else handler->parse = glimpse_integer_fixlength_oct_u16;
					break;
				case GlimpseIntegerDec:
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_dec_i16;
					else handler->parse = glimpse_integer_fixlength_dec_u16;
					break;
				case GlimpseIntegerHex:
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_hex_i16;
					else handler->parse = glimpse_integer_fixlength_hex_u16;
					break;
			}
			break;
		case GlimpseInteger32:
			switch(properties->Representation)
			{
				case GlimpseIntegerBin: 
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_bin_i32;
					else handler->parse = glimpse_integer_fixlength_bin_u32;
					break;
				case GlimpseIntegerOct:
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_oct_i32;
					else handler->parse = glimpse_integer_fixlength_oct_u32;
					break;
				case GlimpseIntegerDec:
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_dec_i32;
					else handler->parse = glimpse_integer_fixlength_dec_u32;
					break;
				case GlimpseIntegerHex:
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_hex_i32;
					else handler->parse = glimpse_integer_fixlength_hex_u32;
					break;
			}
			break;
		case GlimpseInteger64:
			switch(properties->Representation)
			{
				case GlimpseIntegerBin: 
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_bin_i64;
					else handler->parse = glimpse_integer_fixlength_bin_u64;
					break;
				case GlimpseIntegerOct:
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_oct_i64;
					else handler->parse = glimpse_integer_fixlength_oct_u64;
					break;
				case GlimpseIntegerDec:
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_dec_i64;
					else handler->parse = glimpse_integer_fixlength_dec_u64;
					break;
				case GlimpseIntegerHex:
					if(properties->Signed) handler->parse = glimpse_integer_fixlength_hex_i64;
					else handler->parse = glimpse_integer_fixlength_hex_u64;
					break;
			}
			break;
		case GlimpseIntegerVariant:
			//TODO
			break;
		default:
			return -1;
	}
	return 0;
}
GlimpseTypeGroup_t int_type_group = {
	.name = "integer",
	.resolve = resolve_int
};
int init()
{
	RegisterTypeGroup(&int_type_group);
}
GlimpsePluginMetaData(TypeAPI)
	GlimpsePluginName("integer");
	GlimpsePluginVersion(0,0,1);
	GlimpsePluginAPICallBack(TypeAPI, OnInitialized) = init; 
GlimpsePluginEndMetaData
