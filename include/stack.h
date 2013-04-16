#ifndef __STACK_H__
#define __STACK_H__
#include <future.h>
#include <chartable.h>
#include <typesystem.h>
#ifndef GLIMPSE_MAX_STACK_DEPTH 1024
typedef struct _glimpse_handler_stack_node{
	GlimpseTypeHandler_t* handler;  /* the handler used for parse in this level */
	uint8_t	ret_req:1;    /* if ret_req == 1, the parser will terminate unconditionally */
} GlimpseHandlerStackNode_t;
typedef struct _glimpse_handler_stack{
	uint16_t ptr;
	GlimpseHandlerStackNode_t data[GLIMPSE_MAX_STACK_DEPTH];
} GlimpseHandlerStack_t;

#endif
