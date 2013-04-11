#ifndef __TYPE_API_H__
#define __TYPE_API_H__
#include <api.h>
#include <typesystem.h>
#include <log.h>
GlimpseAPIData(TypeAPI)
	//called by plugin
	int (*RegisterTypeGroup)(GlimpseTypeGroup_t* typegroup);
	void (*WriteLog)(ErrorLevel level, const char* file, const char* function,int line, const char* fmt,...);
	//called by API
	int (*OnInitialized)(void);
GlimpseAPIDataEnd
void Glimpse_TypeAPI_init(void);
#define RegisterTypeGroup(tg) GlimpseAPICall(TypeAPI, RegisterTypeGroup, tg)
#define WriteLog(args...) GlimpseAPICall(TypeAPI, WriteLog, ##args)
#endif
