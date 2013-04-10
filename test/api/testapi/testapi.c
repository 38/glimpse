#include <testapi.h>
#include <pluginloader.h>
int add(int a, int b)
{
	return a + b;
}
int init(void* meta)
{
	GlimpseAPIDataType(testapi) *data = (GlimpseAPIDataType(testapi)*) meta;
	data->add = add;
}
GlimpseAPIMetaData_t metadata = {
	.APIVersion = "testapi",
	.PluginInitialize = init 
};
void api_init()
{
	glimpse_pluginloader_register_api(&metadata);
}

