#include <testapi.h>
void say_hello()
{
	printf("%d\n",1,2); 
}
GlimpsePluginMetaData(testapi)
	Name("testapi_plugin")
	GlimpsePluginVersion(1,0,0);
	GlimpsePluginAPICallBack(testapi, done) = say_hello;
GlimpsePluginEndMetaData
