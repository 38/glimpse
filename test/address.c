#include <address.h>
#include <scanner.h>
#include <pluginloader.h>
#include <vector.h>
#include <integer/integer.h>
#include <init.h>
#include <TypeAPI.h>
#include <typeparser.h>
#include <assert.h>
#ifdef GPROF
#include <google/profiler.h>
#endif
int check0(void** res, void* data)
{
	GlimpseAddress_t** exp = (GlimpseAddress_t**) data;
	GlimpseVector_t* a = res[0];
	assert(a == glimpse_address_fetch(res, exp[0]));
	GlimpseVector_t* b = res[1];
	assert(b == glimpse_address_fetch(res, exp[1]));
	assert(3 == *(int*)glimpse_address_fetch(res, exp[2]));
	assert(2 == *(int*)glimpse_address_fetch(res, exp[3]));
	assert(1 == *(int*)glimpse_address_fetch(res, exp[4]));

	assert(1 == *(int*)glimpse_address_fetch(res, exp[4],0));
	assert(4 == *(int*)glimpse_address_fetch(res, exp[5],1));
	assert(7 == *(int*)glimpse_address_fetch(res, exp[5],2));
	
	assert(3 == *(int*)glimpse_address_fetch(res, exp[6]));
	return 0;
}
void case0()
{
	GlimpseAddress_t* exp[10];
	GlimpseTypeDesc_t* td = glimpse_typeparser_parse_type("uint32d");
	GlimpseParseTree_t* tree = glimpse_scanner_register_tree("mylog",' ','=');
	glimpse_tree_insert(tree,"a",td);
	glimpse_tree_insert(tree,"b",td);
	glimpse_tree_insert(tree,"c",td);
	glimpse_tree_insert(tree,"d",td);
	td = glimpse_typeparser_parse_type("vector{seperator:'#',basetype:sublog{name:mylog}}");
	tree = glimpse_scanner_register_tree("testlog",';',':');
	glimpse_tree_insert(tree,"a",td);
	glimpse_tree_insert(tree,"b",td);
	glimpse_tree_insert(tree,"c",glimpse_typeparser_parse_type("vector{seperator:'#',basetype:vector{seperator:'.',basetype:uint32d}}"));
	glimpse_scanner_set_defualt_tree("testlog");
	glimpse_scanner_set_before_scan_callback(NULL, NULL);
	glimpse_scanner_set_after_scan_callback(check0, exp);
	GlimpseThreadData_t* thread_data = glimpse_thread_data_new();
	exp[0] = glimpse_address_resolve("a");
	exp[1] = glimpse_address_resolve("b");
	exp[2] = glimpse_address_resolve("a[#]");
	exp[3] = glimpse_address_resolve("b[#]");
	exp[4] = glimpse_address_resolve("a[0].a");
	exp[5] = glimpse_address_resolve("a[?].a");
	exp[6] = glimpse_address_resolve("c[#]");
	exp[7] = glimpse_address_resolve("c[?][#]");
	exp[8] = glimpse_address_resolve("c[?][?]");
	int i;
	glimpse_scanner_parse("a:a=1 b=2 c=3#a=4 b=5 c=6#a=7 b=8 c=9 d=10;"
						  "b:a=0 b=2 c=4 d=1 e=2#a=1 b=2 c=3;"
						  "c:1.2.3#4.5.6.8#9.10.11.12.13", thread_data);
	glimpse_thread_data_free(thread_data);
}
int main()
{
#ifdef GPROF
	ProfilerStart("/tmp/scanner.prof");
#endif
	glimpse_init();
	Glimpse_TypeAPI_init();
	glimpse_pluginloader_path[0] = ".";
	glimpse_pluginloader_path[1] = "..";
	glimpse_pluginloader_path[2] = NULL;
	glimpse_pluginloader_load_plugin("integer");
	case0();
	glimpse_cleanup();
#ifdef GPROF
	ProfilerStop();
#endif
}
