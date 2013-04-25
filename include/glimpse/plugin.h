/* plugin.h - plugin utils 
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
	
#ifndef __GLIMPSE_PLUGIN_H__
#define __GLIMPSE_PLUGIN_H__
#include <malloc.h>
#include <string.h>
#include <stdint.h>

#include <glimpse/future.h>
#include <glimpse/def.h>
typedef uint8_t GlimpsePluginVersion[3];
/* define the plugin metadata*/
typedef struct _glimpse_plugin_metadata_t{
	const char* Name;
	const char* APIVersion;	//which groups of API dose the plugin request
	GlimpsePluginVersion Version;	    //version of the plugin major.minor.revisionin
	char ** Dependency;		//define the dependency of the plugin in 
							//e.g.: plugin A.Dependency = {"B@3.1.2","C@3.0.5",NULL}
	char data[0];			/*data used for specified version of API*/
	/* DO NOT add any defination here */
}GlimpsePluginMetaData_t;
/* Indicates the begining of of the Glimpse Plugin Metadata Section */
#ifdef __cplusplus
#	define GLIMPSE_META_HEADER extern "C" {
#	define GLIMPSE_META_FOOTER }
#else
#	define GLIMPSE_META_HEADER
#	define GLIMPSE_META_FOOTER
#endif 
#define GlimpsePluginMetaData(APIName) GlimpsePluginMetaData_t* _glimpse_plugin_metadata = NULL;\
GLIMPSE_META_HEADER \
GlimpsePluginMetaData_t* GetMetaData(void){\
	if(_glimpse_plugin_metadata) return _glimpse_plugin_metadata;\
	GlimpsePluginMetaData_t* ret = \
		(GlimpsePluginMetaData_t*)malloc(sizeof(GlimpsePluginMetaData_t) + sizeof(GlimpseAPIProc##APIName));\
		memset(ret, 0, sizeof(GlimpsePluginMetaData_t) + sizeof(GlimpseAPIProc##APIName));\
	if(NULL == ret) return NULL;\
	ret->APIVersion = #APIName;
/* Indicates the end of Metadata Section */
#define GlimpsePluginEndMetaData return _glimpse_plugin_metadata = ret;} GLIMPSE_META_FOOTER
#define GlimpsePluginName(name) ret->Name = name
#define GlimpsePluginVersion(major,minor,rev) ret->Version[0] = major;\
	ret->Version[1] = minor;\
	ret->Version[2] = rev;
#define GlimpsePluginDependence static char* dependence[] = {
#define GlimpsePluginEndDependence NULL};ret->Dependency = dependence;
#define GlimpsePluginAPICallBack(APIName,Name) ((GlimpseAPIProc##APIName*)ret->data)->plugin_functions.Name 
#define GlimpseAPICall(APIName,Name, args...) (((GlimpseAPIProc##APIName*)_glimpse_plugin_metadata->data)->api_functions.Name(args))
extern GlimpsePluginMetaData_t* _glimpse_plugin_metadata;
#endif
