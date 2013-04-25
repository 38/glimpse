#include <testapi.h>
#include <glimpse/pluginloader.h>
int main()
{
	api_init();
	glimpse_pluginloader_path[0] = ".";
	glimpse_pluginloader_path[1] = NULL;
	glimpse_pluginloader_load_plugin("testapi_plugin");
	return 0;
}
