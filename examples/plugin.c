#include <TypeAPI.h>
int myinit()
{
	PLUGIN_LOG_FATAL("Hello, Glimpse!");
	return 0;
}
GlimpsePluginMetaData(TypeAPI)
	GlimpsePluginName("plugin");
	GlimpsePluginVersion(0,0,0);
	GlimpsePluginAPICallBack(TypeAPI,OnInitialized) = myinit;
GlimpsePluginEndMetaData
