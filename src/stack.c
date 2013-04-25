/* stack.c -   
 *
 * Copyright 2013 Hao Hou <ghost89413@gmail.com>
 * 
 * This file is part of Glimpse, a fast, flexible key-value scanner.
 * 
 * Glimpse is free software: you can redistribute it and/or modify it under the terms 
 * of the GNU General Public License as published by the Free Software Foundation, 
 * either version 3 of the License, or (at your option) any later version.
 *
 * Glimpse is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with Glimpse. 
 * If not, see http://www.gnu.org/licenses/.
 *
 */
	
#include <glimpse/stack.h>
#include <glimpse/log.h>
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
