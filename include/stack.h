#ifndef __STACK_H__
#define __STACK_H__
#include <typesystem.h>
#define GLIMPSE_MAX_STACK_DEPTH 1024   /* the max depth of parser recurrent */
typedef struct _glimpse_stack_Frame{
	GlimpseTypeHandler_t* handler;  /* the type handler used for parse in this level */
	uint8_t	ret_req:1;    /* if ret_req == 1, the parser will terminate unconditionally */
} GlimpseStackFrame_t;
typedef struct _glimpse_stack{
	uint16_t ptr;
	uint32_t true_val;
	GlimpseStackFrame_t data[GLIMPSE_MAX_STACK_DEPTH];
	GlimpseTypeHandler_t* frames[256];
	uint8_t flag[256];
} GlimpseStack_t;


/* initialization should be done by creator for thread data */
void glimpse_stack_print(GlimpseStack_t* stack);
static inline void glimpse_stack_push(GlimpseStack_t* stack, GlimpseTypeHandler_t* handler)
{
	if(NULL == stack) return;
	if(stack->ptr >= GLIMPSE_MAX_STACK_DEPTH) return;
	stack->data[stack->ptr].ret_req = 0;
	stack->data[stack->ptr].handler = handler;
	stack->ptr ++;
}
static inline GlimpseTypeHandler_t* glimpse_stack_top(GlimpseStack_t* stack)
{
	if(NULL == stack || 0 == stack->ptr) return NULL;
	return stack->data[stack->ptr-1].handler;
}
static inline int glimpse_stack_retreq(GlimpseStack_t* stack)
{
	if(NULL == stack || 0 == stack->ptr) return 0;
	return stack->data[stack->ptr-1].ret_req;
}
static inline void glimpse_stack_pop(GlimpseStack_t* stack)
{
	if(NULL== stack || 0 == stack->ptr) return;
	stack->ptr --;
	GlimpseTypeHandler_t* handler = stack->data[stack->ptr].handler;
	stack->data[stack->ptr].ret_req = 0;
	switch(handler->type->builtin_type)
	{
		case GLIMPSE_TYPE_BUILTIN_VECTOR:
			stack->frames[(uint8_t)handler->type->param.vector.sep] = NULL;
			break;
		case GLIMPSE_TYPE_BUILTIN_SUBLOG:
			stack->frames[(uint8_t)handler->type->param.sublog.tree->sep_f] = handler;
			break;
		case GLIMPSE_TYPE_BUILTIN_MAP:
		case GLIMPSE_TYPE_BUILTIN_NONE:
			break;
	}
}
static inline typeof(((GlimpseTypeHandler_t*)NULL)->parse) glimpse_stack_get_parser(GlimpseStack_t* stack, GlimpseTypeHandler_t* handler)
{
	if(NULL == stack || NULL == handler) return NULL;
	glimpse_stack_push(stack, handler);
	switch(handler->type->builtin_type)
	{
		case GLIMPSE_TYPE_BUILTIN_VECTOR:
			if(stack->flag[(uint8_t)handler->type->param.vector.sep] == stack->true_val &&
			   stack->frames[(uint8_t)handler->type->param.vector.sep]) 
			{
				GLIMPSE_LOG_ERROR("ambigious sperator %c", handler->type->param.vector.sep);
				glimpse_stack_print(stack);
			}
			stack->frames[(uint8_t)handler->type->param.vector.sep] = handler;
			stack->flag[(uint8_t)handler->type->param.vector.sep] = stack->true_val;
			break;
		case GLIMPSE_TYPE_BUILTIN_SUBLOG:
			if(stack->flag[(uint8_t)handler->type->param.sublog.tree->sep_f] == stack->true_val &&
			   stack->frames[(uint8_t)handler->type->param.sublog.tree->sep_f])
			{
				GLIMPSE_LOG_ERROR("ambigious sperator %c", handler->type->param.sublog.tree->sep_f);
				glimpse_stack_print(stack);
			}
			stack->frames[(uint8_t)handler->type->param.sublog.tree->sep_f] = handler;
			stack->flag[(uint8_t)handler->type->param.sublog.tree->sep_f] = stack->true_val;
			break;
		case GLIMPSE_TYPE_BUILTIN_MAP:
		case GLIMPSE_TYPE_BUILTIN_NONE:
			break;
	}
	if(handler->parse) return handler->parse;
}
#endif
