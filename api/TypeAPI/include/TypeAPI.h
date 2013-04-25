#ifndef __GLIMPSE_TYPE_API_H__
#define __GLIMPSE_TYPE_API_H__
#include <glimpse/api.h>
#include <glimpse/typesystem.h>
#include <glimpse/log.h>
GlimpseAPIData(TypeAPI)
	//called by plugin
	GlimpseAPIFunctions
		int (*RegisterTypeGroup)(GlimpseTypeGroup_t* typegroup);
		void (*WriteLog)(ErrorLevel level, const char* file, const char* function,int line, const char* fmt,...);
		int  (*ExportSymbol)(const char* symbol, void* address);
		void* (*ImportSymbol)(const char* symbol);
		void* (*DataObjAlloc)(size_t size);
		void  (*DataObjFree)(void* mem);
		int   (*DataObjLock)(void* data);
		int   (*DataObjUnlock)(void* data);
		int   (*DataObjTrylock)(void* data);
		char* (*StringDuplicate)(const char*);
		int	  (*TypeAlias)(const char* type, const char* name);
	GlimpseAPIFunctionsEnd
	//called by API
	GlimpsePluginFunctions
		int (*OnInitialized)(void);
	GlimpsePluginFunctionsEnd
GlimpseAPIDataEnd
void Glimpse_TypeAPI_init(void);
#define RegisterTypeGroup(tg) GlimpseAPICall(TypeAPI, RegisterTypeGroup, tg)
#define WriteLog(args...) GlimpseAPICall(TypeAPI, WriteLog, args)
#define Export(function) GlimpseAPICall(TypeAPI, ExportSymbol, #function, function)
#define Import(function) ((typeof(&function)) GlimpseAPICall(TypeAPI, ImportSymbol, #function))
#define ExternalFunction(func) typeof(&func) _glimpse_external_##func
#define ImportFunction(func) do{ _glimpse_external_##func = Import(func); } while(0)
#define ExternalCall(func, args...) (_glimpse_external_##func(##args))
#define ObjAlloc(size) GlimpseAPICall(TypeAPI, DataObjAlloc, size)
#define ObjFree(mem) GlimpseAPICall(TypeAPI, DataObjFree, mem)
#define StrDup(str) GlimpseAPICall(TypeAPI, StringDuplicate, str)
#define Alias(t,n) GlimpseAPICall(TypeAPI, TypeAlias, t, n)

#ifdef THREAD_SAFE
#define DataLock(data) GlimpseAPICall(TypeAPI, DataObjLock, data)
#define DataUnlock(data) GlimpseAPICall(TypeAPI, DataObjUnlock, data)
#define DataTrylock(data) GlimpseAPICall(TypeAPI, DataObjTrylock, data)
#endif

#define PLUGIN_LOG(level,fmt,arg...) do{\
	WriteLog(level,__FILE__,__FUNCTION__,__LINE__,fmt, ##arg);\
}while(0);

#ifndef LOG_LEVEL
	#define LOG_LEVEL 6
#endif
#if LOG_LEVEL >= 0
#	define PLUGIN_LOG_FATAL(fmt,arg...) PLUGIN_LOG(FATAL,fmt,##arg)
#else
#	define PLUGIN_LOG_FATAL(...)
#endif

#if LOG_LEVEL >= 1
#	define PLUGIN_LOG_ERROR(fmt,arg...) PLUGIN_LOG(ERROR,fmt,##arg)
#else
#	define PLUGIN_LOG_ERROR(...)
#endif

#if LOG_LEVEL >= 2
#	define PLUGIN_LOG_WARNING(fmt,arg...) PLUGIN_LOG(WARNING,fmt,##arg)
#else
#	define PLUGIN_LOG_WARNING(...)
#endif

#if LOG_LEVEL >= 3
#	define PLUGIN_LOG_NOTICE(fmt,arg...) PLUGIN_LOG(NOTICE,fmt,##arg)
#else
#	define PLUGIN_LOG_NOTICE(...)
#endif

#if LOG_LEVEL >= 4
#	define PLUGIN_LOG_INFO(fmt,arg...) PLUGIN_LOG(INFO,fmt,##arg)
#else
#	define PLUGIN_LOG_INFO(...)
#endif

#if LOG_LEVEL >= 5
#	define PLUGIN_LOG_TRACE(fmt,arg...) PLUGIN_LOG(TRACE,fmt,##arg)
#else
#	define PLUGIN_LOG_TRACE(...)
#endif

#if LOG_LEVEL >= 6
#	define PLUGIN_LOG_DEBUG(fmt,arg...) PLUGIN_LOG(DEBUG,fmt,##arg)
#else
#	define PLUGIN_LOG_DEBUG(...)
#endif
#endif
