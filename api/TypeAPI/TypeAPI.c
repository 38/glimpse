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
	meta->api_functions.DataObjAlloc = glimpse_typesystem_instance_object_alloc;
	meta->api_functions.DataObjFree  = glimpse_typesystem_instance_object_free;
#ifdef THREAD_SAFE
	meta->api_functions.DataObjLock = glimpse_typesystem_instance_object_lock;
	meta->api_functions.DataObjUnlock = glimpse_typesystem_instance_object_unlock;
	meta->api_functions.DataObjTrylock = glimpse_typesystem_instance_object_trylock;
#endif
	if(meta->plugin_functions.OnInitialized) return meta->plugin_functions.OnInitialized();
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
