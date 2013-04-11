#ifndef __TYPE_API_H__
#define __TYPE_API_H__
#include <api.h>
#include <typesystem.h>
#include <log.h>
GlimpseAPIData(TypeAPI)
	//called by plugin
	GlimpseAPIFunctions
		int (*RegisterTypeGroup)(GlimpseTypeGroup_t* typegroup);
		void (*WriteLog)(ErrorLevel level, const char* file, const char* function,int line, const char* fmt,...);
	GlimpseAPIFunctionsEnd
	//called by API
	GlimpsePluginFunctions
		int (*OnInitialized)(void);
	GlimpsePluginFunctionsEnd
GlimpseAPIDataEnd
void Glimpse_TypeAPI_init(void);
#define RegisterTypeGroup(tg) GlimpseAPICall(TypeAPI, RegisterTypeGroup, tg)
#define WriteLog(args...) GlimpseAPICall(TypeAPI, WriteLog, ##args)
#endif
