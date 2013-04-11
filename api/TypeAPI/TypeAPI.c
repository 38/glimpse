#include <TypeAPI.h>
#include <pluginloader.h>
int Glimpse_TypeAPI_PluginInit(void* data)
{
	GlimpseAPIDataType(TypeAPI)* meta = (GlimpseAPIDataType(TypeAPI)*)data;
	meta->RegisterTypeGroup = glimpse_typesystem_register_typegroup;
	meta->WriteLog = glimpse_log_write;
	if(meta->OnInitialized) meta->OnInitialized();
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
