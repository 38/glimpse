#ifndef __GLIMPSE_API_H__
#define __GLIMPSE_API_H__

#include <stdint.h>

#include <glimpse/future.h>
#include <glimpse/def.h>
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
#include <glimpse/plugin.h>
#endif
