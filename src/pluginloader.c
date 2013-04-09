#include <pluginloader.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <log.h>
#include <retval.h>
#include <malloc.h>
#include <fcntl.h>
char* glimpse_pluginloader_path[256];
GlimpseAPIMetaData_t* _glimpse_pluginloader_api_list[MAX_API_VERSION];
GlimpsePluginHandler_t* _glimpse_pluginloader_plugin_list[MAX_PLUGINS];
int _glimpse_pluginloader_api_count = 0;
int _glimpse_pluginloader_plugin_count = 0;
int glimpse_pluginloader_register_api(GlimpseAPIMetaData_t* API)
{
	if(NULL == API) return EINVAILDARG;
	if(_glimpse_pluginloader_api_count >= MAX_API_VERSION) 
	{
		GLIMPSE_LOG_WARNING("too many versions of API registed");
		return ETOOMANYAPI;
	}
	_glimpse_pluginloader_api_list[_glimpse_pluginloader_api_count++] = API;
	return ESUCCESS;
}
GlimpseAPIMetaData_t* _glimpse_pluginloader_find_api_by_version(const char* version)
{
	int i;
	for(i = 0; i < _glimpse_pluginloader_api_count; i ++)
		if(strcmp(_glimpse_pluginloader_api_list[i]->APIVersion, version) == 0) 
			return _glimpse_pluginloader_api_list[i];
	GLIMPSE_LOG_DEBUG("can not find API");
	return NULL;
}
GlimpsePluginHandler_t* _glimpse_pluginloader_find_plugin(const char* name)
{
	int i;
	for(i = 0; i < _glimpse_pluginloader_plugin_count; i ++)
		if(strcmp(_glimpse_pluginloader_plugin_list[i]->MetaData->Name, name))
			return _glimpse_pluginloader_plugin_list[i];
	GlimpsePluginHandler_t* ret = (GlimpsePluginHandler_t*)malloc(sizeof(GlimpsePluginHandler_t));
	char path[1024];
	if(NULL == ret) goto ERR;
	for(i = 0; glimpse_pluginloader_path[i]; i ++)
	{
		snprintf(path, 1024, "%s/lib%s.so", glimpse_pluginloader_path[i], name);
		if(0 == access(path, R_OK))
		{
			if((ret->dl_handler = dlopen(path, RTLD_NOW))) 
				return ret;
			else
				GLIMPSE_LOG_DEBUG("%s", dlerror());
		}
	}
ERR:
	if(ret->dl_handler) dlclose(ret->dl_handler);
	free(ret);
	return NULL;
}
typedef GlimpsePluginMetaData_t* (*GetMetaData_proc)(void);
int _glimpse_pluginloader_initilaize_plugin(GlimpsePluginHandler_t* handler)
{
	int errval = EINVAILDARG;
	if(NULL == handler) goto ERR;
	
	errval = ESYMNOTFOUND;
	GetMetaData_proc proc;
	proc = (GetMetaData_proc)dlsym(handler, "GetMetaData");
	if(NULL == proc) goto ERR;
	
	errval = EUNKNOWN;
	handler->MetaData = proc();
	if(NULL == handler->MetaData) goto ERR;

	errval = EINVAILDARG;
	if(NULL == handler->MetaData->Name || 
	   NULL == handler->MetaData->APIVersion || 
	   NULL == handler->MetaData->Dependency)
	{
		GLIMPSE_LOG_ERROR("Invalid Plugin");
		goto ERR;
	}
	handler->API = _glimpse_pluginloader_find_api_by_version(handler->MetaData->APIVersion);
	if(NULL == handler->API)
	{
		GLIMPSE_LOG_ERROR("invalid API Version %s", handler->MetaData->APIVersion);
		goto ERR;
	}
	
	if(handler->API->PluginInitialize)
		errval = handler->API->PluginInitialize(handler->MetaData->data);
	else
	{
		errval = EMALFORMEDAPI;
		goto ERR;
	}
	
	//TODO: resolve dependency
	int i;
	for(i = 0; handler->MetaData->Dependency[i]; i ++)
	{
		
	}
	return ESUCCESS;
ERR:
	if(handler)
	{
		if(handler->MetaData) free(handler->MetaData);
		if(handler->dl_handler) dlclose(handler->dl_handler);
		free(handler);
	}
	return errval;
}
int glimpse_pluginloader_load_plugin(const char* name)
{
	GlimpsePluginHandler_t* handler = _glimpse_pluginloader_find_plugin(name);
	return _glimpse_pluginloader_initilaize_plugin(handler);
}
