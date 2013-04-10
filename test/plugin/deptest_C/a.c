#include <testapi.h>
int print()
{
	puts("C has been loaded");
}
GlimpsePluginMetaData(testapi)
	GlimpsePluginName("deptest_C");
	GlimpsePluginVersion(1,0,0);
	GlimpsePluginAPICallBack(testapi, done) = print;
GlimpsePluginEndMetaData
