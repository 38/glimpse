#ifndef __TEST_API__
#define __TEST_API__
#include <api.h>
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
