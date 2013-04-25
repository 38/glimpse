#include <stdarg.h>
#include <ParserAPI.h>
#include <glimpse/pluginloader.h>
#include <glimpse/symbol.h>
#include <glimpse/typeparser.h>
#include <glimpse/typesystem.h>
#include <glimpse/tree.h>
#include <glimpse/scanner.h>
#include <glimpse/address.h>
#include <glimpse/strpool.h>

int Glimpse_ParserAPI_PthreadMock(void* data){}
int Glimpse_ParserAPI_AliasType(const char* type, const char* name)
{
	GlimpseTypeDesc_t* desc = glimpse_typeparser_parse_type(type);
	if(NULL == desc) return GLIMPSE_EINVAILDARG;
	int rc = glimpse_typeparser_alias(desc, name);
	if(rc != GLIMPSE_ESUCCESS) glimpse_typesystem_typedesc_free(desc);
	return rc;
}
int Glimpse_ParserAPI_CreateLog(const char* name, char sep_kv, char sep_f, ...)
{
	if(NULL == name) return GLIMPSE_EINVAILDARG;
	GlimpseParseTree_t* tree = glimpse_scanner_register_tree(name,sep_f,sep_kv);
	if(NULL == tree) return GLIMPSE_EUNKNOWN;
	va_list ap;
	va_start(ap, sep_f);
	char* key;
	while(key = va_arg(ap, char*))
	{
		char* type = va_arg(ap,char*);
		if(NULL == type) return GLIMPSE_EINVAILDARG;
		GlimpseTypeDesc_t* desc = glimpse_typeparser_parse_type(type);
		if(NULL == desc)
		{
			GLIMPSE_LOG_ERROR("can not parse type %s", type);
			return GLIMPSE_EINVAILDARG;
		}
		int rc = glimpse_tree_insert(tree, key, desc);
		if(rc != GLIMPSE_ESUCCESS)
			GLIMPSE_LOG_WARNING("failed to insert <key=`%s',type=`%s>'", key, type);
	}
	return GLIMPSE_ESUCCESS;
} 
int Glimpse_ParserAPI_PluginInit(void* data)
{
	GlimpseAPIDataType(ParserAPI)* meta = (GlimpseAPIDataType(ParserAPI)*) data;
	meta->api_functions.WriteLog = glimpse_log_write;
	meta->api_functions.ExportSymbol = glimpse_symbol_exportsymbol;
	meta->api_functions.ImportSymbol = glimpse_symbol_importsymbol;
#ifdef THREAD_SAFE
	meta->api_functions.DataObjLock = glimpse_typesystem_instance_object_lock;
	meta->api_functions.DataObjUnlock = glimpse_typesystem_instance_object_unlock;
	meta->api_functions.DataObjTrylock = glimpse_typesystem_instance_object_trylock;
#else
	meta->api_functions.DataObjLock = Glimpse_ParserAPI_PthreadMock;
	meta->api_functions.DataObjUnlock = Glimpse_ParserAPI_PthreadMock;
	meta->api_functions.DataObjTrylock = Glimpse_ParserAPI_PthreadMock;
#endif
	meta->api_functions.StringDuplicate = glimpse_strpool_new;
	meta->api_functions.TypeAlias = Glimpse_ParserAPI_AliasType;

	meta->api_functions.CreateLog = Glimpse_ParserAPI_CreateLog;
	meta->api_functions.SetDefaultLog = glimpse_scanner_set_defualt_tree;
	meta->api_functions.SetBeforeScanCallback = glimpse_scanner_set_before_scan_callback;
	meta->api_functions.SetAfterScanCallback = glimpse_scanner_set_after_scan_callback;
	meta->api_functions.CompileAddress = glimpse_address_resolve;
	if(meta->plugin_functions.OnInitialized) return meta->plugin_functions.OnInitialized();
	return 0;
}
int Glimpse_ParserAPI_OnPrimaryPluginSelected(void* data) 
{
	GlimpseAPIDataType(ParserAPI)* meta = (GlimpseAPIDataType(ParserAPI)*) data;
	if(meta->plugin_functions.OnPrimaryPluginSelected) return meta->plugin_functions.OnPrimaryPluginSelected();
	return 0;
}
GlimpseAPIMetaData_t _ParserAPI_Meta = {
	.APIVersion = "ParserAPI",
	.PluginInitialize = Glimpse_ParserAPI_PluginInit,
	.OnPrimaryPluginSelected = Glimpse_ParserAPI_OnPrimaryPluginSelected
};
void Glimpse_ParserAPI_init(void)
{
	glimpse_pluginloader_register_api(&_ParserAPI_Meta);
}
