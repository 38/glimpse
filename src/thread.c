#include <thread.h>
#include <malloc.h>
#include <string.h>
GlimpseThreadData_t* glimpse_thread_data_new()
{
	GlimpseThreadData_t* ret = (GlimpseThreadData_t*)malloc(sizeof(GlimpseThreadData_t));
	if(NULL == ret) return NULL;
	memset(ret, 0, sizeof(GlimpseThreadData_t));
#ifdef HANDLER_STACK
	ret->stack.true_val = 1;
#endif
	return ret;
}
void glimpse_thread_data_free(GlimpseThreadData_t* data)
{
	if(data) free(data);
}
void glimpse_thread_data_init(GlimpseThreadData_t* data)
{
	if(NULL == data) return;
#ifdef HANDLER_STACK
	data->stack.ptr = 0;
	data->stack.true_val ++;
#endif
}