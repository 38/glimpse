#include <TypeAPI.h>
void foo();
ExternalFunction(foo);
int init()
{
	ImportFunction(foo);
	ExternalCall(foo);
}
GlimpsePluginMetaData(TypeAPI)
	GlimpsePluginName("import");
	GlimpsePluginVersion(0,0,1);
	GlimpsePluginDependence
		"export@0.0.1",
	GlimpsePluginEndDependence
	GlimpsePluginAPICallBack(TypeAPI, OnInitialized) = init; 
GlimpsePluginEndMetaData
