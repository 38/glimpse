#include <TypeAPI.h>
void foo();
ExternalFunction(foo);
int init_plugin_()
{
	ImportFunction(foo);
	ExternalCall(foo);
	return 0;
}
GlimpsePluginMetaData(TypeAPI)
	GlimpsePluginName("import");
	GlimpsePluginVersion(0,0,1);
	GlimpsePluginDependence
		"export@0.0.1",
	GlimpsePluginEndDependence
	GlimpsePluginAPICallBack(TypeAPI, OnInitialized) = init_plugin_; 
GlimpsePluginEndMetaData
