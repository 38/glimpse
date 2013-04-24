#ifndef __SRC_PLUGIN_H__
#define __SRC_PLUGIN_H__
#include <future.h>
#include <plugin.h>
#include <api.h>
#include <stdint.h>
#include <def.h>
/* define how many versions of API dose the framework support */
#ifndef MAX_API_VERSION
#	define MAX_API_VERSION 16
#endif
#ifndef MAX_PLUGINS
#	define MAX_PLUGINS 256
#endif

typedef struct _glimpse_plugin_handler{
	int32_t index;
	int32_t  initialized;
	GlimpsePluginMetaData_t* MetaData;
	GlimpseAPIMetaData_t* API;
	void* dl_handler;
}GlimpsePluginHandler_t;

extern char* glimpse_pluginloader_path[256];
int glimpse_pluginloader_register_api(GlimpseAPIMetaData_t* API);
int glimpse_pluginloader_load_plugin(const char* name);
int glimpse_pluginloader_cleanup();
#endif
