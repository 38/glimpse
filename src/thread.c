#include <thread.h>
#include <malloc.h>
#include <string.h>
GlimpseThreadData_t* glimpse_thread_data_new()
{
	GlimpseThreadData_t* ret = (GlimpseThreadData_t*)malloc(sizeof(GlimpseThreadData_t));
	if(NULL == ret) return NULL;
	memset(ret, 0, sizeof(GlimpseThreadData_t));
	return ret;
}
void glimpse_thread_data_free(GlimpseThreadData_t* data)
{
	if(data) free(data);
}

