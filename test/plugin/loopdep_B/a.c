#include <testapi.h>
int print()
{
	puts("B has been loaded");
}
GlimpsePluginMetaData(testapi)
	GlimpsePluginName("loopdep_B");
	GlimpsePluginVersion(1,0,0);
	GlimpsePluginDependence
		"loopdep_A@1.0.0",
	GlimpsePluginEndDependence;
	GlimpsePluginAPICallBack(testapi, done) = print;
GlimpsePluginEndMetaData
