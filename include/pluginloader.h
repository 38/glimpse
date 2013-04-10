#ifndef __SRC_PLUGIN_H__
#define __SRC_PLUGIN_H__
#include <plugin.h>
#include <api.h>
/* define how many versions of API dose the framework support */
#ifndef MAX_API_VERSION
#	define MAX_API_VERSION 16
#endif
#ifndef MAX_PLUGINS
#	define MAX_PLUGINS 256
#endif

typedef struct _glimpse_plugin_handler{
	int index;
	int  initialized;
	GlimpsePluginMetaData_t* MetaData;
	GlimpseAPIMetaData_t* API;
	void* dl_handler;
}GlimpsePluginHandler_t;

extern char* pluginloader_path[256];
int pluginloader_register_api(GlimpseAPIMetaData_t* API);
int pluginloader_load_plugin(const char* name);
int pluginloader_unload_plugin(GlimpsePluginMetaData_t* plugin);
#endif
