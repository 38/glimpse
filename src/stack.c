#include <stack.h>
#include <log.h>
#ifdef HANDLER_STACK
void glimpse_stack_print(GlimpseStack_t* stack)
{
	if(NULL == stack) return;
	GLIMPSE_LOG_DEBUG("======================Parser Stack======================");
	int i;
	for(i = 0; i < stack->ptr; i ++)
	{
		char buffer[1024];
		buffer[0] = 0;
		glimpse_typesystem_typehandler_tostring(stack->data[i].handler, buffer, sizeof(buffer));
		GLIMPSE_LOG_DEBUG("%4d: %s",i, buffer);
	}
	GLIMPSE_LOG_DEBUG("========================================================");
}
#endif
