#include <testapi.h>
int print()
{
	puts("A has been loaded");
}
GlimpsePluginMetaData(testapi)
	GlimpsePluginName("baddep_A");
	GlimpsePluginVersion(1,0,0);
	GlimpsePluginDependence
		"baddep_B@1.2.0",
	GlimpsePluginEndDependence;
	GlimpsePluginAPICallBack(testapi, done) = print;
GlimpsePluginEndMetaData
