#include  <typesystem.h>
#include <pluginloader.h>
#include <TypeAPI.h>
#include <assert.h>
#include <integer/integer.h>
#include <vector.h>
GlimpseTypeHandler_t* init_handler()
{
	GlimpseTypeDesc_t* td = glimpse_typesystem_typedesc_new(sizeof(GlimpseIntegerProperties_t));
	GlimpseIntegerProperties_t* properties = (GlimpseIntegerProperties_t*)td->properties;
	td->param.normal.group = "integer";
	td->flags = 0;
	properties->Size = GlimpseInteger32;
	properties->Signed = 1;
	properties->Representation = GlimpseIntegerHex;
	properties->Leading = "0x";
	GlimpseTypeHandler_t *handler = glimpse_typesystem_query(td);
	return handler;
}
GlimpseTypeHandler_t* vec_handler()
{
	GlimpseTypeDesc_t* td = glimpse_typesystem_typedesc_new(0);
	td->flags = GLIMPSE_TYPEFLAG_VECTOR;
	td->param.vector.sep = ",";
	td->param.vector.basetype = glimpse_typesystem_typedesc_new(sizeof(GlimpseIntegerProperties_t));
	GlimpseIntegerProperties_t* properties = (GlimpseIntegerProperties_t*)td->param.vector.basetype->properties;
	td->param.vector.basetype->flags = 0;
	td->param.vector.basetype->param.normal.group = "integer";
	properties->Size = GlimpseInteger32;
	properties->Signed = 1;
	properties->Representation = GlimpseIntegerHex;
	properties->Leading = "0x";
	return glimpse_typesystem_query(td);
}
void case_type_pool()
{
	GlimpseTypeHandler_t *h1, *h2;
	h1 = init_handler();
	h2 = init_handler();
	assert(h1);
	assert(h1 == h2);
	GlimpseTypeInstance_t* inst1 = glimpse_typesystem_typehandler_new_instance(h1);
	glimpse_typesystem_typehandler_free_instance(inst1);
	GlimpseTypeInstance_t* inst2 = glimpse_typesystem_typehandler_new_instance(h2);
	glimpse_typesystem_typehandler_free_instance(inst2);
	int i = 0;
	GlimpseTypeInstance_t* addr[1000];
	for(i = 0; i < 1000; i ++)
	{
		addr[i] = glimpse_typesystem_typehandler_new_instance(h1);
	}
	glimpse_typesystem_typehandler_free_instance(addr[439]);
	glimpse_typesystem_typehandler_free_instance(addr[376]);
	glimpse_typesystem_typehandler_free_instance(addr[213]);
	assert(addr[213] == glimpse_typesystem_typehandler_new_instance(h1));
	assert(addr[376] == glimpse_typesystem_typehandler_new_instance(h1));
	assert(addr[439] == glimpse_typesystem_typehandler_new_instance(h1));
	GlimpseTypeInstance_t* new = glimpse_typesystem_typehandler_new_instance(h1);
	for(i = 0; i < 1000; i ++)
		assert(new != addr[i]);
}
void case_vector()
{
	int i;
	GlimpseTypeHandler_t *hint = init_handler();
	GlimpseTypeHandler_t *hvec = vec_handler();
	GlimpseTypeInstance_t* instance = glimpse_typesystem_typehandler_new_instance(hvec);
	hvec->parse("0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0x10", *instance, hvec->parse_data);
	for(i = 0; i < ((GlimpseVector_t*)*instance)->size; i ++)
	{
		GlimpseTypeInstance_t** inst = (GlimpseTypeInstance_t**)glimpse_vector_get((GlimpseVector_t*)*instance , i);
		int* res = (int*)**inst;
		assert(*res == i + (i/10)*6);
	}
}
int main()
{
	glimpse_init();
	Glimpse_TypeAPI_init();
	glimpse_pluginloader_path[0] = ".";
	glimpse_pluginloader_path[1] = "..";
	glimpse_pluginloader_path[1] = NULL;
	assert(0 == glimpse_pluginloader_load_plugin("integer"));
	case_type_pool();
	case_vector();
	glimpse_cleanup();
	return 0;
}
