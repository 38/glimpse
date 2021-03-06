/* init.c -   
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
	
#include <glimpse/init.h>
#include <glimpse/symbol.h>
#include <glimpse/typesystem.h>
#include <glimpse/def.h>
#include <glimpse/log.h>
/* check types */
#include <glimpse/plugin.h>
#include <glimpse/typesystem.h>
#include <glimpse/data.h>
#include <glimpse/address.h>
#define _GLIMPSE_CHECK_LAST(type,last) do{\
	if(sizeof(type) != GLIMPSE_OFFSET_OF(type, last)){\
		GLIMPSE_LOG_WARNING("`" #type "::" #last "' is not the last member, this may cause problem");\
		ret = 1;\
	}else\
		GLIMPSE_LOG_DEBUG("`" #type "::" #last "' is the last member, it seems well-fomed");\
}while(0)
#define _GLIMPSE_CHECK_CONT(type, f1, f2) do{\
	if(GLIMPSE_OFFSET_OF(type, f2) - GLIMPSE_OFFSET_OF(type, f1) != sizeof(((type*)NULL)->f1)){\
		GLIMPSE_LOG_WARNING("`" #type "::" #f1 "' and `" #type "::" #f2 "' occupies discontinious memroy address");\
		ret = 1;\
	}else\
		GLIMPSE_LOG_DEBUG("`" #type "::" #f1 "' and `" #type "::" #f2 "' occupies continious memroy address, it seems well-formed");\
}while(0)
static inline int _glimpse_check_types()
{
	int ret = 0;
	_GLIMPSE_CHECK_LAST(GlimpsePluginMetaData_t, data);
	_GLIMPSE_CHECK_LAST(GlimpseTypeDesc_t, properties);
	_GLIMPSE_CHECK_LAST(GlimpseTypeInstanceObject_t, data);
	_GLIMPSE_CHECK_CONT(GlimpseTypeInstanceObject_t, pool_obj, data);
	_GLIMPSE_CHECK_LAST(GlimpseDataInstance_t, data);
	_GLIMPSE_CHECK_CONT(GlimpseTypeHandler_t, vector_parser_param, parse_data);
	_GLIMPSE_CHECK_CONT(GlimpseTypeHandler_t, sublog_parser_param, parse_data);
	return ret;
}
#undef _GLIMPSE_CHECK_CONT
#undef _GLIMPSE_CHECK_LAST
/* call the init function */
int glimpse_init()
{
	if(_glimpse_check_types())
	{
		GLIMPSE_LOG_FATAL("malformed structures, try to correct the issue above and recompile glimpse");
		return GLIMPSE_EUNKNOWN;
	}
	else
		GLIMPSE_LOG_DEBUG("type check passed, no error found");
	glimpse_strpool_init();
	glimpse_typesystem_init();
	glimpse_scanner_init();
	glimpse_address_init();	
}
/* call the cleanup function */
void glimpse_cleanup()
{
	glimpse_address_cleanup();
	glimpse_scanner_cleanup();
	glimpse_typesystem_cleanup();
	glimpse_symbol_cleaup();
	glimpse_pluginloader_cleanup();
	glimpse_strpool_cleanup();
}
