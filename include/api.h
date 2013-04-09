#ifndef __API_H__
#define __API_H__
/* metadata of APIs */
typedef struct _glimpse_api_metadata_t{
	const char* APIVersion;  /*Version of API*/
	int (*Initialize)();	 /* Called once after Glimpse Started */
	int (*Finalize)();		 /* Called once before Glimpse exited */
	int (*PluginInitialize)(void* metadata);  /*Called after plugin loaded, passing handlers to plugin*/
	int (*PluginFinalize)(void* metadata);	  /*Called befor plugin unloaded, clean up*/
}GlimpseAPIMetaData_t;
#endif
