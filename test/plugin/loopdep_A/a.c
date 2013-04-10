#include <testapi.h>
int print()
{
	puts("A has been loaded");
}
GlimpsePluginMetaData(testapi)
	GlimpsePluginName("loopdep_A");
	GlimpsePluginVersion(1,0,0);
	GlimpsePluginDependence
		"loopdep_B@1.0.0",
	GlimpsePluginEndDependence;
	GlimpsePluginAPICallBack(testapi, done) = print;
GlimpsePluginEndMetaData
