#ifndef __THREADDATA_H__
#define __THREADDATA_H__
#include <future.h>
#include <stack.h>
typedef struct _glimpse_thread_data{
#ifdef HANDLER_STACK
	GlimpseStack_t stack;
#endif
	/* add some other data here */
} GlimpseThreadData_t;
GlimpseThreadData_t* glimpse_thread_data_new();
void glimpse_thread_data_free(GlimpseThreadData_t* data);
void glimpse_thread_data_init(GlimpseThreadData_t* data);
#endif
