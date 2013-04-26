/* pluginloader.h - A plugin loader 
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
	
#ifndef __GLIMPSE_PLUGINLOADER_H__
#define __GLIMPSE_PLUGINLOADER_H__
#include <stdint.h>
#include <stdio.h>

#include <glimpse/future.h>
#include <glimpse/plugin.h>
#include <glimpse/api.h>
#include <glimpse/def.h>
/* define how many versions of API dose the framework support */
#ifndef MAX_API_VERSION
#	define MAX_API_VERSION 16
#endif
#ifndef MAX_PLUGINS
#	define MAX_PLUGINS 256
#endif

#ifdef __cplusplus
extern "C"{
#endif

typedef struct _glimpse_plugin_handler{
	int32_t index;
	int32_t  initialized;
	GlimpsePluginMetaData_t* MetaData;
	GlimpseAPIMetaData_t* API;
	FILE* conf;
	void* dl_handler;
}GlimpsePluginHandler_t;

extern char* glimpse_pluginloader_path[256];
int glimpse_pluginloader_register_api(GlimpseAPIMetaData_t* API);
int glimpse_pluginloader_load_plugin(const char* name);
int glimpse_pluginloader_set_primary_plugin(const char* name);
int glimpse_pluginloader_cleanup();
#ifdef __cplusplus
}
#endif
#endif
