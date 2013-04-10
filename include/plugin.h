#ifndef __PLUGIN_H__
#define __PLUGIN_H__
typedef unsigned char GlimpsePluginVersion[3];
/* define the plugin metadata*/
typedef struct _glimpse_plugin_metadata_t{
	const char* Name;
	const char* APIVersion;	//which groups of API dose the plugin request
	const GlimpsePluginVersion Version[3];	    //version of the plugin major.minor.revisionin
	char ** Dependency;		//define the dependency of the plugin in 
							//e.g.: plugin A.Dependency = {"B@3.1.2","C@3.0.5",NULL}
	char data[0];			/*data used for specified version of API*/
	/* DO NOT add any defination here */
}GlimpsePluginMetaData_t;
#define GlimpsePluginMetaData(APIName) GlimpsePluginMetaData_t* GetMetaData(void){\
	GlimpsePluginMetaData_t* ret = \
		(GlimpsePluginMetaData_t*)malloc(sizeof(GlimpsePluginMetaData_t) + sizeof(GlimpseAPIProc#APIName));\
	if(NULL == ret) return NULL;\
	ret->APIVersion = ##APIName;
#define GlimpsePluginName(name) ret->Name = name
#define GlimpsePluginVersion(major,minor,rev) ret->Version[0] = major;\
	ret->Version[1] = minor;\
	ret->Version[2] = rev;
#define GlimpsePluginDependence static char* dependence[] = {
#define GlimpsePluginEndDependence };
#define GlimpsePluginEndMetaData ret->Dependency = dependence;\
	return ret;}
#endif
