/* pluginloader.c -   
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
	
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>

#include <malloc.h>
#include <fcntl.h>

#include <glimpse/pluginloader.h>
#include <glimpse/log.h>
#include <glimpse/retval.h>
char* glimpse_pluginloader_path[256];
GlimpseAPIMetaData_t* _glimpse_pluginloader_api_list[MAX_API_VERSION];
GlimpsePluginHandler_t* _glimpse_pluginloader_plugin_list[MAX_PLUGINS];
int _glimpse_pluginloader_api_count = 0;
int _glimpse_pluginloader_plugin_count = 0;
int glimpse_pluginloader_register_api(GlimpseAPIMetaData_t* API)
{
	if(NULL == API) return GLIMPSE_EINVAILDARG;
	if(_glimpse_pluginloader_api_count >= MAX_API_VERSION) 
	{
		GLIMPSE_LOG_WARNING("too many versions of API registed");
		return GLIMPSE_ETOOMANYAPI;
	}
	_glimpse_pluginloader_api_list[_glimpse_pluginloader_api_count++] = API;
	if(API->Initialize) 
	{
		int ret = API->Initialize();
		if(ret < 0) 
		{
			GLIMPSE_LOG_ERROR("API %s Initilaize failed", API->APIVersion);
			_glimpse_pluginloader_api_count--;
			return GLIMPSE_EUNKNOWN;
		}
	}
	GLIMPSE_LOG_DEBUG("API %s Registered.", API->APIVersion);
	return GLIMPSE_ESUCCESS;
}
static GlimpseAPIMetaData_t* _glimpse_pluginloader_find_api_by_version(const char* version)
{
	int i;
	for(i = 0; i < _glimpse_pluginloader_api_count; i ++)
		if(strcmp(_glimpse_pluginloader_api_list[i]->APIVersion, version) == 0) 
			return _glimpse_pluginloader_api_list[i];
	GLIMPSE_LOG_WARNING("can not find API %s", version);
	return NULL;
}
static GlimpsePluginHandler_t* _glimpse_pluginloader_find_plugin(const char* name)
{
	int i;
	for(i = 0; i < _glimpse_pluginloader_plugin_count; i ++)
		if(strcmp(_glimpse_pluginloader_plugin_list[i]->MetaData->Name, name) == 0)
			return _glimpse_pluginloader_plugin_list[i];
	GlimpsePluginHandler_t* ret = (GlimpsePluginHandler_t*)malloc(sizeof(GlimpsePluginHandler_t));
	char path[1024];
	if(NULL == ret) goto ERR;
	ret->MetaData = NULL;
	ret->dl_handler = NULL;
	ret->API = NULL;
	ret->index = -1;
	ret->initialized = 0;
	for(i = 0; glimpse_pluginloader_path[i]; i ++)
	{
		snprintf(path, 1024, "%s/lib%s.so", glimpse_pluginloader_path[i], name);
		if(0 == access(path, R_OK))
		{
			if((ret->dl_handler = dlopen(path, RTLD_NOW)))
			{
				GLIMPSE_LOG_TRACE("using %s as plugin %s", path, name);
				return ret;
			}
			else
				GLIMPSE_LOG_DEBUG("%s", dlerror());
		}
	}
ERR:
	if(ret->dl_handler) dlclose(ret->dl_handler);
	free(ret);
	return NULL;
}
int glimpse_pluginloader_set_primary_plugin(const char* name)
{
	int i;
	for(i = 0; i < _glimpse_pluginloader_plugin_count; i ++)
		if(strcmp(_glimpse_pluginloader_plugin_list[i]->MetaData->Name, name) == 0)
		{
			GlimpseAPIMetaData_t* api =  _glimpse_pluginloader_plugin_list[i]->API;
			GlimpsePluginMetaData_t* plugin = _glimpse_pluginloader_plugin_list[i]->MetaData;
			if(api->OnPrimaryPluginSelected) return api->OnPrimaryPluginSelected(plugin->data);
		}
	return GLIMPSE_ENOTFOUND;
}
typedef GlimpsePluginMetaData_t* (*GetMetaData_proc)(void);
static int _glimpse_pluginloader_initilaize_plugin(GlimpsePluginHandler_t* handler)
{
	int errval = GLIMPSE_EINVAILDARG;
	int idx = -1;
	if(NULL == handler) goto ERR_BEFORE_ENQUEUED;
	
	errval = GLIMPSE_ESYMNOTFOUND;
	GetMetaData_proc proc;
	proc = (GetMetaData_proc)dlsym(handler->dl_handler, "GetMetaData");
	if(NULL == proc) goto ERR_BEFORE_ENQUEUED;
	
	errval = GLIMPSE_EUNKNOWN;
	handler->MetaData = proc();
	if(NULL == handler->MetaData) goto ERR_BEFORE_ENQUEUED;

	errval = GLIMPSE_EINVAILDARG;
	if(NULL == handler->MetaData->Name || 
	   NULL == handler->MetaData->APIVersion) 
	{
		GLIMPSE_LOG_ERROR("Invalid Plugin");
		goto ERR_BEFORE_ENQUEUED;
	}
	handler->API = _glimpse_pluginloader_find_api_by_version(handler->MetaData->APIVersion);
	if(NULL == handler->API)
	{
		GLIMPSE_LOG_ERROR("invalid API Version %s", handler->MetaData->APIVersion);
		goto ERR_BEFORE_ENQUEUED;
	}
	
	handler->initialized = 1;
	
	handler->index = _glimpse_pluginloader_plugin_count;
	_glimpse_pluginloader_plugin_list[idx = _glimpse_pluginloader_plugin_count++] = handler;
	
	int i;
	for(i = 0; handler->MetaData->Dependency && handler->MetaData->Dependency[i]; i ++)
	{
		char temp[128];
		strncpy(temp, handler->MetaData->Dependency[i], sizeof(temp));
		char* vercode = strstr(temp, "@");
		*vercode = 0;
		vercode ++;
		GlimpsePluginVersion min_version;
		int j, index = 0;
		errval = GLIMPSE_EINVALIDVERCODE;
		for(j = 0; j < 3; j ++)
		{
			min_version[j] = 0;
			for(;vercode[index] && vercode[index] != '.'; index ++)
			{
				if(vercode[index] < '0' || vercode[index] > '9') goto ERR;
				min_version[j] = min_version[j] * 10 + (vercode[index] - '0');
			}
			if(vercode[index]) index ++; 
		}
		errval = GLIMPSE_EDEPEND;
		GlimpsePluginHandler_t* dep_handler = _glimpse_pluginloader_find_plugin(temp);
		if(NULL == dep_handler) goto ERR;
		if(dep_handler->initialized == 2) {/*already initialized*/}
		else
		{
			if(dep_handler->initialized == 1)
			{
				GLIMPSE_LOG_ERROR("loop dependence detected");
				goto ERR;
			}
			errval = _glimpse_pluginloader_initilaize_plugin(dep_handler);
			if(GLIMPSE_ESUCCESS != errval) goto ERR;
		}

		errval = GLIMPSE_EVERSION;
		for(j = 0; j < 3; j ++)
			if(dep_handler->MetaData->Version[j] != min_version[j])
			{
				if(dep_handler->MetaData->Version[j] < vercode[j]) 
				{
					GLIMPSE_LOG_ERROR("plugin %s requires plugin %s in version %d.%d.%d, but got %d.%d.%d",
									  handler->MetaData->Name,
									  dep_handler->MetaData->Name,
									  min_version[0],min_version[1],min_version[2],
									  dep_handler->MetaData->Version[0],
									  dep_handler->MetaData->Version[1],
									  dep_handler->MetaData->Version[2]);
					goto ERR;
				}
				break;
			}
	}
	
	if(handler->API->PluginInitialize)
		errval = handler->API->PluginInitialize(handler->MetaData->data);
	else
	{
		errval = GLIMPSE_EMALFORMEDAPI;
		goto ERR;
	}
	
	if(errval != GLIMPSE_ESUCCESS) goto ERR;

	handler->initialized = 2;
	GLIMPSE_LOG_TRACE("Plugin %s@%d.%d.%d has been initialized", handler->MetaData->Name,
			handler->MetaData->Version[0],
			handler->MetaData->Version[1],
			handler->MetaData->Version[2]);
	return GLIMPSE_ESUCCESS;
ERR:
	for(i = idx + 1; i < _glimpse_pluginloader_plugin_count; i ++)
		_glimpse_pluginloader_plugin_list[i-1] = _glimpse_pluginloader_plugin_list[i];
	_glimpse_pluginloader_plugin_count --;
ERR_BEFORE_ENQUEUED:	
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
	if(handler && handler->initialized == 2)
	{
		GLIMPSE_LOG_DEBUG("plugin %s has been loaded previously", name);
		return GLIMPSE_ESUCCESS;
	}
	return _glimpse_pluginloader_initilaize_plugin(handler);
}
int glimpse_pluginloader_cleanup()
{
	int i;
	for(i = 0; i < _glimpse_pluginloader_plugin_count; i ++)
	{
		GlimpsePluginHandler_t* handler = _glimpse_pluginloader_plugin_list[i];
		if(handler->API->PluginFinalize)
			handler->API->PluginFinalize(handler->MetaData->data);
		free(handler->MetaData);
		dlclose(handler->dl_handler);
	}
	for(i = 0; i < _glimpse_pluginloader_api_count; i ++)
	{
		GlimpseAPIMetaData_t* API = _glimpse_pluginloader_api_list[i];
		if(API->Finalize) API->Finalize();
	}
	_glimpse_pluginloader_api_count = _glimpse_pluginloader_plugin_count = 0;
	return 0;
}
