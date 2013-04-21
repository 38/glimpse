#include <TypeAPI.h>
void foo()
{
	PLUGIN_LOG_DEBUG("foo called");
}
int init_plugin()
{
	Export(foo);
	return 0;
}
GlimpsePluginMetaData(TypeAPI)
	GlimpsePluginName("export");
	GlimpsePluginVersion(0,0,1);
	GlimpsePluginAPICallBack(TypeAPI, OnInitialized) = init_plugin; 
GlimpsePluginEndMetaData
