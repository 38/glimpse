#ifndef __PLUGIN_H__
#define __PLUGIN_H__
#include <future.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
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
#define GlimpsePluginMetaData(APIName) GlimpsePluginMetaData_t* _glimpse_plugin_metadata = NULL;\
GlimpsePluginMetaData_t* GetMetaData(void){\
	if(_glimpse_plugin_metadata) return _glimpse_plugin_metadata;\
	GlimpsePluginMetaData_t* ret = \
		(GlimpsePluginMetaData_t*)malloc(sizeof(GlimpsePluginMetaData_t) + sizeof(GlimpseAPIProc##APIName));\
		memset(ret, 0, sizeof(GlimpsePluginMetaData_t) + sizeof(GlimpseAPIProc##APIName));\
	if(NULL == ret) return NULL;\
	ret->APIVersion = #APIName;
/* Indicates the end of Metadata Section */
#define GlimpsePluginEndMetaData return _glimpse_plugin_metadata = ret;}
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
