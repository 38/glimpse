#include <TypeAPI.h>
void foo()
{
	PLUGIN_LOG_DEBUG("foo called");
}
int init()
{
	foo();
	Export(foo);
}
GlimpsePluginMetaData(TypeAPI)
	GlimpsePluginName("export");
	GlimpsePluginVersion(0,0,1);
	GlimpsePluginAPICallBack(TypeAPI, OnInitialized) = init; 
GlimpsePluginEndMetaData
