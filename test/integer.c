#include <pluginloader.h>
#include <TypeAPI.h>
#include <integer/integer.h>
#include <assert.h>
#include <time.h>
void case0()
{
	GlimpseTypeDesc_t* td = glimpse_typesystem_typedesc_new(sizeof(GlimpseIntegerProperties_t));
	GlimpseIntegerProperties_t* properties = (GlimpseIntegerProperties_t*)td->properties;
	td->param.normal.group = "integer";
	td->builtin_type = GLIMPSE_TYPE_BUILTIN_NONE;
	td->extrapulated = 0;
	properties->Size = GlimpseInteger32;
	properties->Signed = 1;
	properties->Representation = GlimpseIntegerHex;
	properties->Leading = "0x";
	GlimpseTypeHandler_t *handler = glimpse_typesystem_query(td);
	assert(handler);
	int tmp = -1;
	const char* ret = NULL;
	const char* inp;
	ret = handler->parse(inp = "ffff", &tmp, handler->parse_data);
	assert(tmp == -1);
	assert(ret == inp);
	ret = handler->parse("0x10000", &tmp, handler->parse_data);
	assert(tmp ==  65536);
	assert(ret != NULL);
	assert(*ret == 0);
	properties->Leading = NULL;
	ret = handler->parse("-7cb8642c", &tmp, handler->parse_data);
	assert(tmp == -0x7cb8642c);
	assert(ret != NULL);
	assert(*ret == 0);
	properties->LeadingAfterSign = "0x";
	ret = handler->parse("0x10000", &tmp, handler->parse_data);
	assert(tmp ==  65536);
	assert(ret != NULL);
	assert(*ret == 0);
	ret = handler->parse("-0x7cb8642c", &tmp, handler->parse_data);
	assert(tmp == -0x7cb8642c);
	assert(ret != NULL);
	assert(*ret == 0);
}
char D[] = "0123456789abcdef";
const char* print(long long *n, int sign, int size, int rep, const char* l, const char* las)
{
	static char buffer[100];
	long long range = 0;
	int exp = 0;
	switch(rep){
		case GlimpseIntegerBin: exp = 2; break;
		case GlimpseIntegerOct: exp = 8; break;
		case GlimpseIntegerDec: exp = 10;break;
		case GlimpseIntegerHex: exp = 16; break;
	}
	switch(size){
		case GlimpseInteger8: range = 0xff;break;
		case GlimpseInteger16: range = 0xffff;break;
		case GlimpseInteger32: range = 0xffffffff;break;
		case GlimpseInteger64: range = 0xffffffffffffffffll;break;
	}
	*n &= range;
	int len = 0;
	int i;
	long long val;
	for(val = *n; val != 0; val /= exp)
	{
		int d = val%exp;
		buffer[len++] = D[d];
	}
	if(las)
	for(i = strlen(las) - 1; i >= 0; i --)
		buffer[len++] = las[i];
	if(sign){
		sign = 1;
		for(i=rand()&0xf; i ; i --)
			buffer[len++] = '-', *n = -*n;
	}
	if(l)
	for(i = strlen(l) - 1; i >= 0; i --)
		buffer[len++] = l[i];
	*n &= range;
	int p,q;
	for(p = 0, q = len -1; p < q; p++, q--)
	{
		char t = buffer[p];
		buffer[p] = buffer[q];
		buffer[q] = t;
	}
	buffer[len] = 0;
	return buffer;
}
void random_case()
{
	GlimpseTypeDesc_t* td = glimpse_typesystem_typedesc_new(sizeof(GlimpseIntegerProperties_t));
	GlimpseIntegerProperties_t* prop = (GlimpseIntegerProperties_t*)td->properties;
	td->param.normal.group = "integer";
	td->builtin_type = GLIMPSE_TYPE_BUILTIN_NONE;
	td->extrapulated = 0;
	prop->Size = rand()%GlimpseIntegerSizeCount;
	prop->Signed = rand()&1;
	prop->Representation = rand()%GlimpseIntegerRepresentationCount;
	GlimpseTypeHandler_t *handler = glimpse_typesystem_query(td);
	if(NULL == handler) return;
	long long tmp = 0;
	handler->parse("1", &tmp, handler->parse_data);
	assert(tmp == 1);
	const char* inp;
	const char* ret = handler->parse(inp = "3", &tmp, handler->parse_data);
	if(prop->Representation == GlimpseIntegerBin) assert(inp == ret);
	else 
	{
		assert(3 == tmp);
		assert(NULL != ret);
		assert(0 == *ret);
	}
	int i;
	for(i = 0; i < 100; i ++)
	{
		long long expect = (((long long)(rand()&0x7fffffff))<<32)|rand();
		const char* input = print(&expect, prop->Signed, prop->Size, prop->Representation, prop->Leading, prop->LeadingAfterSign);
		long long returned = 0;
		const char* rc = handler->parse(input, &returned, handler->parse_data);
		assert(returned == expect);
	}
}
int main()
{
	int i = 0;
	srand((unsigned)time(NULL));
	glimpse_typesystem_init();
	Glimpse_TypeAPI_init();
	glimpse_pluginloader_path[0] = ".";
	glimpse_pluginloader_path[1] = "..";
	glimpse_pluginloader_path[2] = NULL;
	assert(0 == glimpse_pluginloader_load_plugin("integer"));
	case0();
	for(i = 0; i < 10000; i ++)
	{
		random_case();
		GLIMPSE_LOG_INFO("random case #%d passed", i);
	}
	glimpse_pluginloader_cleanup();
	glimpse_typesystem_cleanup();
	return 0;
}
