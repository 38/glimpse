#include <testapi.h>
int print()
{
	puts("B has been loaded");
}
GlimpsePluginMetaData(testapi)
	GlimpsePluginName("deptest_B");
	GlimpsePluginVersion(1,0,0);
	GlimpsePluginDependence
		"deptest_C@1.0.0",
	GlimpsePluginEndDependence
	GlimpsePluginAPICallBack(testapi, done) = print;
GlimpsePluginEndMetaData
