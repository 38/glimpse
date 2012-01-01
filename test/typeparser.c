#include <init.h>
#include <typeparser.h>
#include <TypeAPI.h>
#include <pluginloader.h>
#include <assert.h>
#include <integer/integer.h>
void case0()
{
	GlimpseTypeDesc_t* desc = glimpse_typeparser_parse_type(
			"integer   {\n"
			"				size:32, #int32\n"
			"				signed:true, #signed\n"
			"				format:hex, #hex rep\n"
			"				heading : '0x'\n"
			"}");
	assert(desc);
	assert(desc->builtin_type == GLIMPSE_TYPE_BUILTIN_NONE);
	assert(strcmp(desc->param.normal.group, "integer") == 0);
	assert(((GlimpseIntegerProperties_t*)desc->properties)->Size == GlimpseInteger32);
	assert(((GlimpseIntegerProperties_t*)desc->properties)->Signed);
	assert(((GlimpseIntegerProperties_t*)desc->properties)->Representation = GlimpseIntegerHex);
	assert(strcmp(((GlimpseIntegerProperties_t*)desc->properties)->Leading, "0x") == 0);
	glimpse_typesystem_typedesc_free(desc);
}
void case1()
{
	GlimpseTypeDesc_t* desc = glimpse_typeparser_parse_type(
			"vector {\n"
			"	seperator:',',\n"
			"	basetype:	integer{size:32, signed:true, format:hex}\n"
			"}");
	assert(desc);
	assert(desc->builtin_type = GLIMPSE_TYPE_BUILTIN_VECTOR);
	assert(desc->param.vector.sep == ',');
	assert(desc->param.vector.basetype);
	assert(desc->param.vector.basetype->builtin_type == GLIMPSE_TYPE_BUILTIN_NONE);
	assert(0 == strcmp(desc->param.vector.basetype->param.normal.group,"integer"));
	assert(((GlimpseIntegerProperties_t*)desc->param.vector.basetype->properties)->Size == GlimpseInteger32);
	assert(((GlimpseIntegerProperties_t*)desc->param.vector.basetype->properties)->Signed);
	assert(((GlimpseIntegerProperties_t*)desc->param.vector.basetype->properties)->Representation = GlimpseIntegerHex);
	glimpse_typesystem_typedesc_free(desc);
}
void case2()
{
	GlimpseTypeDesc_t* int_desc = glimpse_typeparser_parse_type("integer{size:8,signed:true,format:hex}");
	assert(int_desc);
	glimpse_typeparser_alias(int_desc, "int8_hex");  /* aliased type does not need to free */
	GlimpseTypeDesc_t* int_desc_ = glimpse_typeparser_parse_type("int8_hex");
	assert(int_desc == int_desc_);
	GlimpseTypeDesc_t* desc = glimpse_typeparser_parse_type("vector{seperator:'#',basetype:int8_hex}");
	assert(desc);
	assert(desc->builtin_type = GLIMPSE_TYPE_BUILTIN_VECTOR);
	assert(desc->param.vector.sep == '#');
	assert(desc->param.vector.basetype);
	assert(desc->param.vector.basetype->builtin_type == GLIMPSE_TYPE_BUILTIN_NONE);
	assert(0 == strcmp(desc->param.vector.basetype->param.normal.group,"integer"));
	assert(((GlimpseIntegerProperties_t*)desc->param.vector.basetype->properties)->Size == GlimpseInteger8);
	assert(((GlimpseIntegerProperties_t*)desc->param.vector.basetype->properties)->Signed);
	assert(((GlimpseIntegerProperties_t*)desc->param.vector.basetype->properties)->Representation = GlimpseIntegerHex);
	glimpse_typesystem_typedesc_free(desc);
}
int main()
{
	glimpse_init();
	Glimpse_TypeAPI_init();
	glimpse_pluginloader_path[0] = ".";
	glimpse_pluginloader_path[1] = "..";
	glimpse_pluginloader_path[2] = NULL;
	glimpse_pluginloader_load_plugin("integer");
	case0();
	case1();
	case2();
	glimpse_cleanup();
	return 0;
}
