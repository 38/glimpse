/* address.c -   
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
	
#include <glimpse/address.h>
#include <glimpse/tree.h>
#include <glimpse/scanner.h>
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
	glimpse_vector_free(_glimpse_address_list);
	return 0;
}

static inline GlimpseAddress_t* _glimpse_address_new()
{
	GlimpseAddress_t* ret = (GlimpseAddress_t*)malloc(sizeof(GlimpseAddress_t));
	if(NULL == ret) return NULL;
	ret->count = 0;
	return ret;
}
GlimpseAddress_t* glimpse_address_resolve(const char* text)  /* resolve human-readable address into index based address */
{
	GlimpseAddress_t* ret = _glimpse_address_new();
	GlimpseParseTree_t* tree = glimpse_scanner_get_default_tree();
	GlimpseTypeDesc_t* current_type = NULL;
	int size_flag = 0;
	if(NULL == text || NULL == ret || NULL == tree) goto ERR;
	int status = 0;
	for(;*text;)   /* LA1[VA1][VA2][VA3].LA2[VA1][VA2][VA3] */
	{
		if(' ' == *text ||
		   '\t' == *text ||
		   '\n' == *text ||
		   '\r' == *text )
		{
			text ++;
			continue;  /* we ignore white spaces */
		}
		switch(status)
		{
			case 0: /*LA*/
				{
					if((NULL != current_type && current_type->builtin_type != GLIMPSE_TYPE_BUILTIN_SUBLOG) || size_flag)
					{
						GLIMPSE_LOG_ERROR("attempt apply member name to a non-log type");
						goto ERR;
					}
					GlimpseParserStatus_t parser_status = glimpse_tree_scan_start(tree);
					for(;*text && *text != '[' && *text != '.'; text ++)
					{
						parser_status = glimpse_tree_scan(parser_status, *text);
						if(NULL == parser_status) 
						{
							GLIMPSE_LOG_ERROR("can not find keyname");
							goto ERR;
						}
					}
					parser_status = glimpse_tree_scan(parser_status, tree->sep_kv);
					if(parser_status && parser_status->term)
					{
						current_type = parser_status->s.terminus.handler->type;
						if(NULL == current_type) goto ERR;
						ret->op[ret->count].type = GLIMPSE_ADDRESSING_TYPE_LOG;
						ret->op[ret->count].oper.log.offset = parser_status->s.terminus.idx;
						ret->count ++;
					}
					else
					{
						GLIMPSE_LOG_ERROR("can not find keyname");
						goto ERR;
					}
				}
				status = 1;
				break;
			case 1: /*VA*/
				for(;*text == '['; text ++) /* for each vector address */
				{
					if(0 == *++text) 
					{
						GLIMPSE_LOG_ERROR("invaild syntax");
						goto ERR;
					}
					if(current_type->builtin_type != GLIMPSE_TYPE_BUILTIN_VECTOR || size_flag) 
					{
						GLIMPSE_LOG_ERROR("attempt to apply subscript on a scalar");
						goto ERR;
					}
					for(;' ' == *text || '\t' == *text || '\n' == *text || '\r' == *text; text ++);
					int val = 0;
					switch(*text){
						case '?': 
							val = GLIMPSE_ADDRESSING_VECTOR_VARIABLE_INDEX;
							text ++;
							break;
						case '#':
							size_flag = 1;
							val = GLIMPSE_ADDRESSING_VECTOR_SIZE;
							text ++;
							break;
						default:
							for(;*text >= '0' && *text <= '9'; text ++)
								val = val * 10 + (*text - '0');
					}
					for(;' ' == *text || '\t' == *text || '\n' == *text || '\r' == *text; text ++);
					if(*text != ']') 
					{
						GLIMPSE_LOG_ERROR("invaild vector address");
						goto ERR;
					}
					current_type = current_type->param.vector.basetype; 
					ret->op[ret->count].type = GLIMPSE_ADDRESSING_TYPE_VEC;
					ret->op[ret->count].oper.vec.index = val;
					ret->count ++;
				}
				status = 2;
				break;
			case 2: /* expecting another LA */
				if(*text == '.') 
				{
					status = 0;
					text ++;
				}
				else 
				{
					GLIMPSE_LOG_ERROR("invaild address syntax");
					goto ERR;
				}
		}
	}
	glimpse_vector_push(_glimpse_address_list, &ret);
	return ret;
ERR:
	GLIMPSE_LOG_DEBUG("failed to resolve address `%s'", text);
	if(NULL != ret) free(ret);
	return NULL;
}
