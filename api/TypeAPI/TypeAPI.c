#include <TypeAPI.h>
#include <pluginloader.h>
#include <symbol.h>
int Glimpse_TypeAPI_PluginInit(void* data)
{
	GlimpseAPIDataType(TypeAPI)* meta = (GlimpseAPIDataType(TypeAPI)*)data;
	meta->api_functions.RegisterTypeGroup = glimpse_typesystem_register_typegroup;
	meta->api_functions.WriteLog = glimpse_log_write;
	meta->api_functions.ExportSymbol = glimpse_symbol_exportsymbol;
	meta->api_functions.ImportSymbol = glimpse_symbol_importsymbol;
	if(meta->plugin_functions.OnInitialized) meta->plugin_functions.OnInitialized();
	return 0;
}

GlimpseAPIMetaData_t TypeAPI_MetaData = {
	.APIVersion = "TypeAPI",
	.PluginInitialize = Glimpse_TypeAPI_PluginInit
};
void Glimpse_TypeAPI_init(void)
{
	glimpse_pluginloader_register_api(&TypeAPI_MetaData);
}
