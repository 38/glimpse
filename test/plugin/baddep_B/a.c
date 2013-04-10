#include <testapi.h>
int print()
{
	puts("B has been loaded");
}
GlimpsePluginMetaData(testapi)
	GlimpsePluginName("baddep_B");
	GlimpsePluginVersion(1,0,0);
	GlimpsePluginAPICallBack(testapi, done) = print;
GlimpsePluginEndMetaData
