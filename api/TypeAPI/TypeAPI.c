/* TypeAPI.c -   
 *
 * Copyright 2013 Hao Hou <ghost89413@gmail.com>
 * 
 * This file is part of Glimpse, a fast, flexible key-value scanner.
 * 
 * Glimpse is free software: you can redistribute it and/or modify it under the terms 
 * of the GNU General Public License as published by the Free Software Foundation, 
 * either version 3 of the License, or (at your option) any later version.
 *
 * Glimpse is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with Glimpse. 
 * If not, see http://www.gnu.org/licenses/.
 *
 */
	
#include <TypeAPI.h>
#include <glimpse/pluginloader.h>
#include <glimpse/symbol.h>
#include <glimpse/strpool.h>
#include <glimpse/typeparser.h>
#include <glimpse/typesystem.h>
int Glimpse_TypeAPI_PthreadMock(void* data){}
int Glimpse_TypeAPI_AliasType(const char* type, const char* name)
{
	GlimpseTypeDesc_t* desc = glimpse_typeparser_parse_type(type);
	if(NULL == desc) return GLIMPSE_EINVAILDARG;
	int rc = glimpse_typeparser_alias(desc, name);
	if(rc != GLIMPSE_ESUCCESS) glimpse_typesystem_typedesc_free(desc);
	return rc;
}
int Glimpse_TypeAPI_PluginInit(void* data)
{
	GlimpseAPIDataType(TypeAPI)* meta = (GlimpseAPIDataType(TypeAPI)*)data;
	meta->api_functions.RegisterTypeGroup = glimpse_typesystem_register_typegroup;
	meta->api_functions.WriteLog = glimpse_log_write;
	meta->api_functions.ExportSymbol = glimpse_symbol_exportsymbol;
	meta->api_functions.ImportSymbol = glimpse_symbol_importsymbol;
	meta->api_functions.DataObjAlloc = glimpse_typesystem_instance_object_alloc;
	meta->api_functions.DataObjFree  = glimpse_typesystem_instance_object_free;
#ifdef THREAD_SAFE
	meta->api_functions.DataObjLock = glimpse_typesystem_instance_object_lock;
	meta->api_functions.DataObjUnlock = glimpse_typesystem_instance_object_unlock;
	meta->api_functions.DataObjTrylock = glimpse_typesystem_instance_object_trylock;
#else
	meta->api_functions.DataObjLock = Glimpse_TypeAPI_PthreadMock;
	meta->api_functions.DataObjUnlock = Glimpse_TypeAPI_PthreadMock;
	meta->api_functions.DataObjTrylock = Glimpse_TypeAPI_PthreadMock;
#endif
	meta->api_functions.StringDuplicate = glimpse_strpool_new;
	meta->api_functions.TypeAlias = Glimpse_TypeAPI_AliasType;
	if(meta->plugin_functions.OnInitialized) return meta->plugin_functions.OnInitialized();
	return GLIMPSE_ESUCCESS;
}

GlimpseAPIMetaData_t TypeAPI_MetaData = {
	.APIVersion = "TypeAPI",
	.PluginInitialize = Glimpse_TypeAPI_PluginInit
};
void Glimpse_TypeAPI_init(void)
{
	glimpse_pluginloader_register_api(&TypeAPI_MetaData);
}
