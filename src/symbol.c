#include <symbol.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <log.h>
/* structure that used to store symbols */
typedef struct _sym{
	/* a group of hash functions, which reduces the string compare for find */
	uint32_t hash[4];
	char name[GLIMPSE_SYMBOL_MAX_SYMBOL_LENGTH];
	void* address;
	struct _sym* next;
} _sym; /* private struct can not be used outside of the file */
/* hashtable */
_sym* _glimpse_symbol_symtable[GLIMPSE_SYMBOL_MAX_SYMBOL_NUMBER];
static void _glimpse_symbol_hash(const char* str, uint32_t* h)
{
	const uint32_t m = 0xc6a4a793;
	const unsigned char* data = (unsigned char*) str;
	h[0] = 0x5f2c2345^(strlen(str)*m);	//Murmur
	h[1] = 0;					//sum
	h[2] = 0x23f4537;   	    //muliply
	h[3] = 0;					//ELF Hash
	int i = 0;
	for(i = 0; str[i]; i ++)
	{
		if((i&3) == 0 && i != 0)
		{
			uint32_t k = *(uint32_t*)(data + i - 4);
			h[0] += k;
			h[0] *= m;
			h[0] ^= h[0]>>16;
		}
		h[1] = (h[1] + data[i] + (data[i]<<17) + (data[i]>>15))%0xfffe7985UL;
		h[2] = (data[i] * h[2])%0xee6b27ebUL;
		h[3] = (h[3] << 4) + data[i];
		uint32_t g = h[3] & 0xf0000000ul;
		if(g) h[3]^=g>>24;
		h[3] &= ~g;
	}
	switch(i&3)
	{
		case 0:
			h[0] += data[i-4] << 24;
		case 3:
			h[0] += data[i-3] << 16;
		case 2:
			h[0] += data[i-2] << 8;
		case 1: 
			h[0] += data[i-1];
			h[0] *= m;
			h[0] ^= h[0] >> 16;
	}
	h[0] *= m;
	h[0] ^= h[0]>>10;
	h[0] *= m;
	h[0] ^= h[0]>>17;
}
static _sym* _glimpse_symbol_findsym(const char* symname, uint32_t* h)
{
	_glimpse_symbol_hash(symname, h);
	int idx = h[0]%GLIMPSE_SYMBOL_MAX_SYMBOL_NUMBER;
	_sym* p;
	for(p = _glimpse_symbol_symtable[idx]; p; p = p->next)
		if(p->hash[1] == h[1] &&
		   p->hash[2] == h[2] &&
		   p->hash[3] == h[3] && 
		   p->hash[0] == h[0] &&
		   strcmp(symname, p->name) == 0) 
			return p;
	return NULL;
}
static int _glimpse_symbol_insertsym(const char* symname, void* address)
{
	uint32_t h[4];
	if(_glimpse_symbol_findsym(symname, h) != NULL) return EDUPSYMBOL;
	int idx = h[0]%GLIMPSE_SYMBOL_MAX_SYMBOL_NUMBER;
	_sym *tmp = (_sym*)malloc(sizeof(_sym));
	if(NULL == tmp) return EUNKNOWN;
	tmp->hash[0] = h[0];
	tmp->hash[1] = h[1];
	tmp->hash[2] = h[2];
	tmp->hash[3] = h[3];
	strncpy(tmp->name, symname, GLIMPSE_SYMBOL_MAX_SYMBOL_LENGTH);
	tmp->address = address;
	tmp->next = _glimpse_symbol_symtable[idx];
	_glimpse_symbol_symtable[idx] = tmp;
	return ESUCCESS;
}
int glimpse_symbol_exportsymbol(const char* symname, void* address)
{
	int ret = _glimpse_symbol_insertsym(symname, address);
	GLIMPSE_LOG_DEBUG("memory address <0x%x> was exported to shared symbol list as %s, retcode = %d", address, symname, ret);
	return ret;
}
void* glimpse_symbol_importsymbol(const char* symname)
{
	uint32_t h[4];
	_sym* rc = _glimpse_symbol_findsym(symname, h);
	void* ret = NULL;
	if(rc) ret = rc->address;
	GLIMPSE_LOG_DEBUG("imported shared symbol %s memory at <0x%x>", symname, ret);
	return ret;
}
void glimpse_symbol_cleaup()
{
	int i;
	for(i = 0; i < GLIMPSE_SYMBOL_MAX_SYMBOL_NUMBER; i ++)
	{
		_sym* p;
		for(p = _glimpse_symbol_symtable[i]; p;)
		{
			_sym* tmp = p;
			p = p->next;
			free(tmp);
		}
	}
}
