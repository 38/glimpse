#include <pluginloader.h>
#include <string.h>
#include <stdio.h>
#include <dlcntl.h>
typedef struct _plugin_handler{
	PluginMetaData_t* MetaData;
	void* dl_handler;
}PluginHandler_t;
char* pluginloader_path[256];
APIMetaData_t* _pluginloader_api_list[MAX_API_VERSION];
PluginHandler_t* _pluginloader_plugin_list[MAX_PLUGINS];
int _pluginloader_api_count = 0;
int _pluginloader_plugin_count = 0;
int pluginloader_register_api(APIMetaData_t* API)
{
	if(NULL == API) return PLUGINLOADER_INVAILD_ARG;
	if(_pluginloader_api_count >= MAX_API_VERSION) return PLUGINLOADER_TOO_MANY_API;
	_pluginloader_api_count[_pluginloader_api_count++] = API;
	return PLUGINLOADER_SUCCESS;
}
APIMetaData_t* _pluginloader_find_api_by_version(const char* version)
{
	int i;
	for(i = 0; i < _pluginloader_api_count; i ++)
		if(strcmp(_pluginloader_api_count[i]->APIVersion, version) == 0) return _pluginloader_api_count[i];
	return NULL;
}
PluginHandler_t* _pluginloader_find_plugin(const char* name)
{
	PluginHandler_t* ret = (PluginHandler_t*)malloc(sizeof(PluginHandler_t));
	char path[1024];
	int i;
	if(NULL == ret) goto ERR;
	for(i = 0; pluginloader_path[i]; i ++)
	{
		snprintf(path, 1024, "%s/lib%s.so", pluginloader_path[i], name);
		if(0 == access(path, R_OK))
		{
			if((ret->handler = dlopen(path, RTLD_NOW))) return ret;
		}
	}
ERR:
	if(ret->dl_handler) dlclose(ret->dl_handler);
	free(ret);
	return(NULL);
}
typedef PluginHandler_t* (*GetMetaData_proc)(void);
int pluginloader_load_plugin(const char* name)
{
	PluginHandler_t* handler = _pluginloader_find_plugin(name);
	GetMetaData_proc proc;
	if(NULL == handler) return PLUGINLOADER_NOT_FOUND;
	proc = (GetMetaData_proc)dlsym(handler, "GetMetaData");
	if(NULL == proc) return PLUGINLOADER_SYM_NOT_FOUND;
	handler->MetaData = proc();
	if(NULL == handler->MetaData)
	{
		dlcose(handler->handler);
		free(handler);
		return PLUGINLOADER_UNKOWN;
	}
	//TODO resolve dependency && init api & user handler

}
