#include <TypeAPI.h>
#include <pluginloader.h>
int main(int argc, char** argv)
{
	Glimpse_TypeAPI_init();
	glimpse_pluginloader_path[0] = ".";
	glimpse_pluginloader_path[1] = NULL;
	glimpse_pluginloader_load_plugin(argv[1]);
	return 0;
}
