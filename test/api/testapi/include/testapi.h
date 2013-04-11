#ifndef __TEST_API__
#define __TEST_API__
#include <api.h>
GlimpseAPIData(testapi)
	int (*add)(int a, int b);
	int (*done)();
GlimpseAPIDataEnd
void api_init();
#define Add(a,b) GlimpseAPICall(testapi, add, a, b)
#endif
