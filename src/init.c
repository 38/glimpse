#include <init.h>
#include <symbol.h>
#include <typesystem.h>
#include <def.h>
#include <log.h>
/* check types */
#include <plugin.h>
#include <typesystem.h>
#include <data.h>
#define _GLIMPSE_CHECK_LAST(type,last) do{\
	if(sizeof(type) != GLIMPSE_OFFSET_OF(type, last)){\
		GLIMPSE_LOG_WARNING("`" #type "::" #last "' is not the last member, this may cause problem");\
		ret = 1;\
	}else\
		GLIMPSE_LOG_DEBUG("`" #type "' seems well formed");\
}while(0)
#define _GLIMPSE_CHECK_CONT(type, f1, f2) do{\
	if(GLIMPSE_OFFSET_OF(type, f2) - GLIMPSE_OFFSET_OF(type, f1) != sizeof(((type*)NULL)->f1)){\
		GLIMPSE_LOG_WARNING("`" #type "::" #f1 "' and `" #type "::" #f2 "' occupies discontinious memroy address");\
		ret = 1;\
	}else\
		GLIMPSE_LOG_DEBUG("`" #type "' seems well formed");\
}while(0)
static inline int _glimpse_check_types()
{
	int ret = 0;
	_GLIMPSE_CHECK_LAST(GlimpsePluginMetaData_t, data);
	_GLIMPSE_CHECK_LAST(GlimpseTypeDesc_t, properties);
	_GLIMPSE_CHECK_LAST(GlimpseTypeInstanceObject_t, data);
	_GLIMPSE_CHECK_CONT(GlimpseTypeInstanceObject_t, pool_obj, data);
	_GLIMPSE_CHECK_LAST(GlimpseDataInstance_t, data);
	return ret;
}
#undef _GLIMPSE_CHECK_CONT
#undef _GLIMPSE_CHECK_LAST
/* call the init function */
void glimpse_init()
{
	if(_glimpse_check_types())
	{
		GLIMPSE_LOG_FATAL("malformed structures, try to correct the issue above and recompile glimpse");
		abort();
	}
	glimpse_typesystem_init();
}
/* call the cleanup function */
void glimpse_cleanup()
{
	glimpse_typesystem_cleanup();
	glimpse_symbol_cleaup();
	glimpse_pluginloader_cleanup();
}
