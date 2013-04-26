/* api.h - tools for API developers 
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
	
#ifndef __GLIMPSE_API_H__
#define __GLIMPSE_API_H__

#include <stdint.h>

#include <glimpse/future.h>
#include <glimpse/def.h>


#ifdef __cplusplus
extern "C"{
#endif


/* metadata of APIs */
typedef struct _glimpse_api_metadata_t{
	const char* APIVersion;  /*Version of API*/
	int (*Initialize)();	 /* Called once after Glimpse Started */
	int (*Finalize)();		 /* Called once before Glimpse exited */
	int (*PluginInitialize)(void* metadata);  /*Called after plugin loaded, passing handlers to plugin*/
	int (*PluginFinalize)(void* metadata);	  /*Called befor plugin unloaded, clean up*/
	int (*OnPrimaryPluginSelected)(void* metadata); /* Called after user select a primary plugin */
}GlimpseAPIMetaData_t;
#define GlimpseAPIData(name) typedef struct _glimpse_api_proc_##name GlimpseAPIProc##name;\
	struct _glimpse_api_proc_##name{
#define GlimpseAPIDataEnd };
#define GlimpseAPIDataType(name) GlimpseAPIProc##name
#define GlimpseAPIInit(name)
#define GlimpseAPIFunctions struct {
#define GlimpseAPIFunctionsEnd }api_functions;
#define GlimpsePluginFunctions struct {
#define GlimpsePluginFunctionsEnd }plugin_functions;

#ifdef __cplusplus
}
#endif

#include <glimpse/plugin.h>

#endif
