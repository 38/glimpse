#include <testapi.h>
#include <stdio.h>
int say_hello()
{
	printf("%d\n",1); 
	printf("%d\n", Add(1,1));
}
GlimpsePluginMetaData(testapi)
	GlimpsePluginName("testapi_plugin");
	GlimpsePluginVersion(1,0,0);
	GlimpsePluginAPICallBack(testapi, done) = say_hello;
GlimpsePluginEndMetaData
