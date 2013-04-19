#include <typeparser.h>
#include <retval.h>
#include <string.h>
#include <log.h>
static GlimpseTypeAlias_t _glimpse_typeparser_alias_table[GLIMPSE_MAX_TYPE_ALIAS];
static int _glimpse_typeparser_alias_count = 0;
static inline GlimpseTypeHandler_t* _glimpse_typeparser_find_alias(const char* name)
{
	int i;
	for(i = 0; i < _glimpse_typeparser_alias_count; i ++)
		if(0 == strcmp(_glimpse_typeparser_alias_table[i].name, name))
			return _glimpse_typeparser_alias_table[i].handler;
	return NULL;
}
int glimpse_typeparser_alias(GlimpseTypeDesc_t* desc, const char* name)
{
	GlimpseTypeHandler_t* handler = glimpse_typesystem_query(desc);
	if(NULL == handler) return EUNKNOWN;
	if(NULL != _glimpse_typeparser_find_alias(name))
	{
		GLIMPSE_LOG_ERROR("failed to alias name with type, because name %s confilicted", name);
		return EINVAILDARG;
	}
	_glimpse_typeparser_alias_table[_glimpse_typeparser_alias_count].handler = handler;
	_glimpse_typeparser_alias_table[_glimpse_typeparser_alias_count].name = name;
	return ESUCCESS;
}
GlimpseTypeDesc_t* glimpse_typeparser_parse_type(const char* text)
{

}
