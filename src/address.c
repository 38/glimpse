#include <address.h>
static GlimpseVector_t *_glimpse_address_list = NULL;
int glimpse_address_init()
{
	_glimpse_address_list = glimpse_vector_new(sizeof(GlimpseAddress_t*));
}
int glimpse_address_cleanup()
{
	int i;
	if(NULL == _glimpse_address_list) return 0;
	for(i = 0; i < _glimpse_address_list->size; i ++)
	{
		GlimpseAddress_t* address = *(GlimpseAddress_t**)glimpse_vector_get(_glimpse_address_list, i);
		if(address) free(address);
	}
	return 0;
}

static GlimpseAddress_t* _glimpse_address_new()
{
	GlimpseAddress_t* ret = (GlimpseAddress_t*)malloc(sizeof(GlimpseAddress_t));
	if(ret) ret->child = NULL;
	return ret;
}
GlimpseAddress_t* glimpse_address_resolve(const char* text)  /* resolve human-readable address into index based address */
{
	GlimpseAddress_t* ret = NULL;
	if(NULL == text) return NULL;
	while(*text)
	{
		//TODO parse text
	}
ERR:
	for(;ret;)
	{
		free(ret);
		ret = ret->child;
	}
	return NULL;
}
