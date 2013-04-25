#ifndef __GLIMPSE_PARSER_API_H__
#define __GLIMPSE_PARSER_API_H__
#include <glimpse/future.h>
#include <glimpse/def.h>
#include <glimpse/api.h>
#include <glimpse/log.h>
#include <glimpse/address.h>
#include <glimpse/scanner.h>
GlimpseAPIData(ParserAPI)
	/*APIs*/
	GlimpseAPIFunctions
		/* type manipulations */
		void (*WriteLog)(ErrorLevel level, const char* file, const char* function,int line, const char* fmt,...);
		int  (*ExportSymbol)(const char* symbol, void* address);
		void* (*ImportSymbol)(const char* symbol);
		int   (*DataObjLock)(void* data);
		int   (*DataObjUnlock)(void* data);
		int   (*DataObjTrylock)(void* data);
		char* (*StringDuplicate)(const char*);
		int	  (*TypeAlias)(const char* type, const char* name);
		/* log manipulations */
#ifdef STRING_SEPERATOR_SUPPORT
	//TODO
#else
		int (*CreateLog)(const char* name, char sep_kv, char sep_f,... );   /* name, sep_kv, sep_f, key1,type1,key2,type2,....,NULL */
#endif
		int (*SetDefaultLog)(const char* name);
		void (*SetBeforeScanCallback)(typeof(((GlimpseScanner_t*)NULL)->before_scan) foo, void* userdata);
		void (*SetAfterScanCallback)(typeof(((GlimpseScanner_t*)NULL)->after_scan) foo, void* userdata);
		GlimpseAddress_t* (*CompileAddress)(const char* address);
	GlimpseAPIFunctionsEnd
	/* Callbacks */
	GlimpsePluginFunctions
		int (*OnInitialized)();
		int (*OnPrimaryPluginSelected)();
	GlimpsePluginFunctionsEnd
GlimpseAPIDataEnd

#define WriteLog(args...) GlimpseAPICall(ParserAPI, WriteLog, args)
#define Export(function) GlimpseAPICall(ParserAPI, ExportSymbol, #function, function)
#define Import(function) ((typeof(&function)) GlimpseAPICall(ParserAPI, ImportSymbol, #function))
#define ExternalFunction(func) typeof(&func) _glimpse_external_##func
#define ImportFunction(func) do{ _glimpse_external_##func = Import(func); } while(0)
#define ExternalCall(func, args...) (_glimpse_external_##func(##args))
#define StrDup(str) GlimpseAPICall(ParserAPI, StringDuplicate, str)
#define Alias(t,n) GlimpseAPICall(ParserAPI, TypeAlias, t, n)
#define CreateLog(n,k,f,arg...) GlimpseAPICall(ParserAPI, CreateLog, k, f, ##arg, NULL)
#define SetDefaultLog(n) GlimpseAPICall(ParserAPI, SetDefaultLog, n)
#define SetBeforeScanCallback(f,d) GlimpseAPICall(ParserAPI,SetBeforeScanCallback, f, d)
#define SetAfterScanCallback(f,d) GlimpseAPICall(ParserAPI,SetAfterScanCallback, f, d)
#define CompileAddress(a) GlimpseAPICall(ParserAPI,CompileAddress,a)
#define FetchAddress glimpse_address_fetch

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
