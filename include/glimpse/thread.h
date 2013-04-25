#ifndef __GLIMPSE_THREAD_H__
#define __GLIMPSE_THREAD_H__

#include <stdint.h>

#include <glimpse/future.h>
#include <glimpse/stack.h>
#include <glimpse/def.h>
typedef struct _glimpse_thread_data{
#ifdef HANDLER_STACK
	GlimpseStack_t stack;
#else
	uint32_t is_term[256];
	uint32_t __true__;
#endif
	/* add some other data here */
} GlimpseThreadData_t;
GlimpseThreadData_t* glimpse_thread_data_new();
void glimpse_thread_data_free(GlimpseThreadData_t* data);
void glimpse_thread_data_init(GlimpseThreadData_t* data);
#endif
