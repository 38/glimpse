#include <testapi.h>
int print()
{
	puts("A has been loaded");
}
GlimpsePluginMetaData(testapi)
	GlimpsePluginName("deptest_A");
	GlimpsePluginVersion(1,0,0);
	GlimpsePluginDependence
		"deptest_B@1.0.0",
		"deptest_C@1.0.0",
	GlimpsePluginEndDependence;
	GlimpsePluginAPICallBack(testapi, done) = print;
GlimpsePluginEndMetaData
