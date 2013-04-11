#include <pluginloader.h>
#include <TypeAPI.h>
#include <integer/integer.h>
int main()
{
	Glimpse_TypeAPI_init();
	glimpse_pluginloader_path[0] = ".";
	glimpse_pluginloader_path[1] = NULL;
	glimpse_pluginloader_load_plugin("integer");
	GlimpseTypeDesc_t* td = glimpse_typesystem_newdesc(sizeof(GlimpseIntegerProperties_t));
	GlimpseIntegerProperties_t* properties = (GlimpseIntegerProperties_t*)td->properties;
	td->group = "integer";
	td->flag = 0;
	properties->Size = GlimpseInteger32;
	properties->Signed = 1;
	properties->Representation = GlimpseIntegerHex;
	properties->Leading = NULL;
	GlimpseTypeHandler_t handler = {};
	glimpse_typesystem_query(td, &handler);
	return 0;

}
