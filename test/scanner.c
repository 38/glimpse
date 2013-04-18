#include <pluginloader.h>
#include <scanner.h>
#include <init.h>
#include <assert.h>
#include <integer/integer.h>
const char* case0_before_scan(const char* text, void* data)
{
	assert(text == *(char**)data);
	assert(NULL != text);
	return text;
}
int case0_after_scan(void** result, void* userdata)
{
	int* expected = (int*)userdata;
	assert( expected[0] == *(int*)result[0]);
	assert( expected[1] == *(int*)result[1]);
	assert( expected[2] == *(int*)result[2]);
}

void case0()
{
	const char *input;
	int expected[3];
	glimpse_scanner_init();
	GlimpseParseTree_t* tree = glimpse_scanner_register_tree("mylog", ' ', '=');
	assert(NULL != tree);
	GlimpseTypeDesc_t* td = glimpse_typesystem_typedesc_new(sizeof(GlimpseIntegerProperties_t));
	assert(NULL != td);
	GlimpseIntegerProperties_t* prop = (GlimpseIntegerProperties_t*)td->properties;
	td->builtin_type = GLIMPSE_TYPE_BUILTIN_NONE;
	td->param.normal.group = "integer";
	prop->Size = GlimpseInteger32;
	prop->Signed = 1;
	prop->Representation = GlimpseIntegerDec;
	prop->Leading = prop->LeadingAfterSign = NULL;
	glimpse_tree_insert(tree ,"value1", td);
	td = glimpse_typesystem_typedesc_dup(td);
	glimpse_tree_insert(tree ,"value2", td);
	td = glimpse_typesystem_typedesc_dup(td);
	glimpse_tree_insert(tree ,"value3", td);
	assert(0 == glimpse_tree_query(tree,"value1"));
	assert(1 == glimpse_tree_query(tree,"value2"));
	assert(2 == glimpse_tree_query(tree,"value3"));
	glimpse_scanner_set_defualt_tree("mylog");
	glimpse_scanner_set_before_scan_callback(case0_before_scan, &input);
	glimpse_scanner_set_after_scan_callback(case0_after_scan, expected);
	expected[0] = 123;
	expected[1] = 456;
	expected[2] = 789;
	glimpse_scanner_parse(input = "value1=123 value2=456 value3=789");
	expected[0] = 11111;
	expected[1] = 22222;
	expected[2] = 33333;
	glimpse_scanner_parse(input = "value4=0x12345 value1=11111 value2=22222 value3=33333");
	glimpse_scanner_cleanup();
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
	glimpse_cleanup();
}
