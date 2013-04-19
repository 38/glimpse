#include <scanner.h>
#include <typesystem.h>
#include <string.h>
#include <malloc.h>
#include <thread.h>
#include <data.h>
#include <retval.h>
static GlimpseScanner_t _glimpse_scanner_instance;  /* internal variable, does not expose to others */
const char* glimpse_scanner_parse(const char* text, GlimpseThreadData_t* thread_data)    /* parse a log the upper most function */
{
	if(NULL == text || NULL == _glimpse_scanner_instance.default_handler) 
		return NULL;
	if(NULL == thread_data)
		return NULL;
	glimpse_thread_data_init(thread_data);
	GlimpseTypeHandler_t* handler = _glimpse_scanner_instance.default_handler;	
	void* data_instance = glimpse_typesystem_typehandler_new_instance(handler);
	if(_glimpse_scanner_instance.before_scan) text = _glimpse_scanner_instance.before_scan(text, 
			_glimpse_scanner_instance.before_scan_data);
#ifdef HANDLER_STACK
	const char* ret = glimpse_stack_get_parser(&thread_data->stack, handler)(text, data_instance, handler->parse_data, thread_data);
	glimpse_stack_pop(&thread_data->stack);
#else
	const char* ret = handler->parse(text, data_instance, handler->parse_data, thread_data);
#endif
	if(_glimpse_scanner_instance.after_scan) _glimpse_scanner_instance.after_scan(((GlimpseDataInstance_t*)data_instance)->data,
			_glimpse_scanner_instance.after_scan_data);
	glimpse_typesystem_typehandler_free_instance(data_instance);
	return ret;
}
inline static int _glimpse_scanner_find_index(const char* name)
{
	int i;
	for(i = 0; i < _glimpse_scanner_instance.count; i ++)
		if(strcmp(_glimpse_scanner_instance.name[i], name) == 0) 
			return i;
	return -1;
}
GlimpseParseTree_t* glimpse_scanner_register_tree(const char* name, char sep_f, char sep_v)		/* add a new log to scanner */
{
	int idx = _glimpse_scanner_find_index(name);
	if(idx != -1)
	{
		GLIMPSE_LOG_WARNING("duplicated tree name %s, return the previously defined one", name);
		return _glimpse_scanner_instance.log[idx];
	}
	idx = _glimpse_scanner_instance.count ++;
	_glimpse_scanner_instance.log[idx] = glimpse_tree_new(sep_f, sep_v);
	if(NULL == _glimpse_scanner_instance.log[idx])
	{
		GLIMPSE_LOG_ERROR("can not create tree object");
		_glimpse_scanner_instance.count --;
		return NULL;
	}
	_glimpse_scanner_instance.name[idx] = name;
	GlimpseParseTree_t* ret = _glimpse_scanner_instance.log[idx];
	if(NULL == ret) return NULL;
	GLIMPSE_LOG_DEBUG("log tree `%s' has been created successfully", name);
	return ret;
}
int glimpse_scanner_set_defualt_tree(const char* name)  /* set the log you want to parse */
{
	int idx = _glimpse_scanner_find_index(name);
	if(-1 == idx)
	{
		GLIMPSE_LOG_ERROR("can not find the tree named %s",name);
		return EINVAILDARG;
	}
	GlimpseTypeDesc_t* desc = glimpse_typesystem_typedesc_new(0);
	desc->builtin_type = GLIMPSE_TYPE_BUILTIN_SUBLOG;
	desc->param.sublog.tree = _glimpse_scanner_instance.log[idx];
	_glimpse_scanner_instance.default_handler = glimpse_typesystem_query(desc);
	if(NULL == _glimpse_scanner_instance.default_handler)
	{
		GLIMPSE_LOG_ERROR("failed to query the type system");
		return EUNKNOWN;
	}
	GLIMPSE_LOG_DEBUG("defualt log parser has been selected");
	return ESUCCESS;
}

GlimpseParseTree_t* glimpse_scanner_find_tree(const char* name)
{
	int idx = _glimpse_scanner_find_index(name);
	if(-1 == idx) return NULL;
	return _glimpse_scanner_instance.log[idx];
}
/* functions to init and cleanup */
void glimpse_scanner_init()
{
	memset(&_glimpse_scanner_instance, 0, sizeof(_glimpse_scanner_instance));
}
void glimpse_scanner_cleanup()
{
	int i;
	for(i = 0; i < _glimpse_scanner_instance.count; i ++)
	{
		glimpse_tree_free(_glimpse_scanner_instance.log[i]);
	}
	/* handler will be freed during the cleanup process of type system */
}
void glimpse_scanner_set_before_scan_callback(typeof(((GlimpseScanner_t*)NULL)->before_scan) callback, void* userdata)
{
	_glimpse_scanner_instance.before_scan = callback;
	_glimpse_scanner_instance.before_scan_data = userdata;
}
void glimpse_scanner_set_after_scan_callback(typeof(((GlimpseScanner_t*)NULL)->after_scan) callback, void* userdata)
{
	_glimpse_scanner_instance.after_scan = callback;
	_glimpse_scanner_instance.after_scan_data = userdata;
}
