#ifndef __GLIMPSE_TEST_API__
#define __GLIMPSE_TEST_API__
#include <glimpse/api.h>
GlimpseAPIData(testapi)
	GlimpseAPIFunctions
		int (*add)(int a, int b);
	GlimpseAPIFunctionsEnd
	GlimpsePluginFunctions
		int (*done)();
	GlimpsePluginFunctionsEnd
GlimpseAPIDataEnd
void api_init();
#define Add(a,b) GlimpseAPICall(testapi, add, a, b)
#endif
