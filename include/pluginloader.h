#ifndef __SRC_PLUGIN_H__
#define __SRC_PLUGIN_H__
/* define how many versions of API dose the framework support */
#ifndef MAX_API_VERSION
#	define MAX_API_VERSION 16
#endif
#ifndef MAX_PLUGINS
#	define MAX_PLUGINS 256
#endif
typedef unsigned char PluginVersion[3];
/* define the plugin metadata*/
typedef struct _plugin_metadata_t{
	const char* APIVersion;	//which groups of API dose the plugin request
	const PluginVersion Version[3];	    //version of the plugin major.minor.revisionin
	char ** Dependency;		//define the dependency of the plugin in 
							//e.g.: plugin A.Dependency = {"B@>=3.1.2","C@3.0.5",NULL}
	int  initialized;		// if the plugin initialized
	char data[0];			/*data used for specified version of API*/
	/* DO NOT add any defination here */
}PluginMetaData_t;
/* metadata of APIs */
typedef struct _api_metadata_t{
	const char* APIVersion;  /*Version of API*/
	int (*Initialize)();	 /* Called once after Glimpse Started */
	int (*Finalize)();		 /* Called once before Glimpse exited */
	int (*PluginInitialize)(void* metadata);  /*Called after plugin loaded, passing handlers to plugin*/
	int (*PluginFinalize)(void* metadata);	  /*Called befor plugin unloaded, clean up*/
}APIMetaData_t;
extern char** pluginloader_path;
int pluginloader_register_api(APIMetaData_t* API);
int pluginloader_load_plugin(const char* name);
int pluginloader_unload_plugin(PluginMetaData_t* plugin);

#define PLUGINLOADER_SUCCESS 0
#define PLUGINLOADER_INVAILD_ARG -1
#define PLUGINLOADER_TOO_MANY_API -2
#define PLUGINLOADER_NOT_FOUND -3
#define PLUGINLOADER_SYM_NOT_FOUND -4
#define PLUGINLOADER_UNKOWN -5
#endif
