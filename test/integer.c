#include <pluginloader.h>
#include <TypeAPI.h>
int main()
{
	Glimpse_TypeAPI_init();
	glimpse_pluginloader_path[0] = ".";
	glimpse_pluginloader_path[1] = NULL;
	glimpse_pluginloader_load_plugin("integer");
	return 0;

}
