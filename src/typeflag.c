#include <typeflag.h>
#include <typesystem.h>
#include <vector.h>
#include <log.h>
#include <data.h>
#include <tree.h>
#include <thread.h>
#include <stdio.h>
#include <stack.h>
void* glimpse_typeflag_vector_alloc(void* userdata)
{
	return glimpse_vector_new(sizeof(void*));
}
int glimpse_typeflag_vector_init(void* data, void* userdata)
{
	glimpse_vector_init((GlimpseVector_t*)data);
	return 0;
}
int glimpse_typeflag_vector_finalize(void* data, void* userdata)
{
	int i;
	GlimpseVector_t* vec = (GlimpseVector_t*) data;
	GlimpseTypeHandler_t* handler = (GlimpseTypeHandler_t*) userdata;
	for(i = 0; i < vec->size; i ++)
	{
		void** addr = (void**)glimpse_vector_get(vec, i);
		if(NULL == addr) continue;
		glimpse_typesystem_typehandler_free_instance(*addr);
	}
	return 0;
}
int glimpse_typeflag_vector_free(void* data, void* userdata)  
{
	glimpse_vector_free((GlimpseVector_t*)data);
	return 0;
}
/* TODO: stop-set problem described in parser.c also affects this parser */
const char* glimpse_typeflag_vector_parse(const char* text, void* result, void* user_data, void* thread_data)
{
	GlimpseTypeVectorParserParam_t* param = (GlimpseTypeVectorParserParam_t*)user_data;
	GlimpseTypeHandler_t* handler = param->basetype_handler;
	GlimpseThreadData_t* thread = (GlimpseThreadData_t*) thread_data;
	if(handler->parse == NULL || thread == NULL) return text;
	const char* next;
	void* instance;
	while(*text)
	{
		//memory = handler->alloc(handler->alloc_data);
		instance = glimpse_typesystem_typehandler_new_instance(handler);
		if(NULL == instance)
		{
			GLIMPSE_LOG_ERROR("can not allocate memory");
			glimpse_stack_print(&thread->stack);
			return NULL;
		}
		const char* next = glimpse_stack_get_parser(&thread->stack, handler)(text, instance, handler->parse_data, thread_data); /* thread support */
		glimpse_stack_pop(&thread->stack); /* cleanup the stack */
		if(NULL == next)
		{
			GLIMPSE_LOG_ERROR("failed to parse text");
			glimpse_stack_print(&thread->stack);
			glimpse_typesystem_typehandler_free_instance(instance);
			return text;
		}
		int rc = glimpse_vector_push((GlimpseVector_t*)result, &instance);
		if(ESUCCESS != rc)
		{
			GLIMPSE_LOG_WARNING("failed to insert to vector");
			glimpse_stack_print(&thread->stack);
			glimpse_typesystem_typehandler_free_instance(instance);
		}
		text = next;
#ifdef STRING_SEPERATOR_SUPPORT
		int i;
		for(i = 0; 
			text[i] != 0 && 
			param->sep != 0 && 
			text[i] == param->sep[i];
			i ++);
		if(param->sep[i] == 0) text += i;
		else return text;
#else
		if(text[0] && text[0] == param->sep) text ++;
		else return text;  /* the handler stack already checked */
#endif
	}
}
char* glimpse_typeflag_vector_tostring(GlimpseTypeHandler_t* handler, char* buffer, size_t size)
{
	char* p = buffer;
	p += snprintf(p, size - (p - buffer), "Vector{seperator: "
#ifdef STRING_SEPERATOR_SUPPORT
			"%s"
#else 
			"'%c'"
#endif
			", basetype:", 
			handler->vector_parser_param[0]->sep);
	if(handler->vector_parser_param[0]->basetype_handler)
		p = glimpse_typesystem_typehandler_tostring(handler->vector_parser_param[0]->basetype_handler, p, size - (p - buffer));
	if(NULL == p) return NULL;
	p += snprintf(p, size - (p - buffer), "}");
	return p;
}
void* glimpse_typeflag_sublog_alloc(void* userdata)
{
	return glimpse_data_instance_new((GlimpseDataModel_t*)userdata);
}
int glimpse_typeflag_sublog_free(void* data, void* userdata)
{
	glimpse_data_instance_free((GlimpseDataInstance_t*)data);
	return 0;
}
int glimpse_typeflag_sublog_init(void* data, void* userdata)
{
	return glimpse_data_instance_init((GlimpseDataInstance_t*)data);
}
int glimpse_typeflag_sublog_finalize(void* data, void* userdata)
{
	glimpse_data_instance_finalize((GlimpseDataInstance_t*)data);
	return 0;
}
const char* glimpse_typeflag_sublog_parse(const char* text, void* result, void* user_data, void* thread_data)
{
	GlimpseParseTree_t* tree = (GlimpseParseTree_t*)user_data;
	GlimpseThreadData_t* thread = (GlimpseThreadData_t*)thread_data;
	GlimpseDataInstance_t* storage = (GlimpseDataInstance_t*) result;
	if(NULL == tree || NULL == text || NULL == storage) return NULL;
	GlimpseParserStatus_t status = NULL;
	const char* p;
	for(p = text; p && *p;)
	{
		if(NULL == status) status = glimpse_tree_scan_start(tree);
		if(NULL == status)
		{
			GLIMPSE_LOG_FATAL("can not initialize scanner, abort");
			return NULL;
		}
		status = glimpse_tree_scan(status, *(p++));
		if(status)
		{
			if(status->term) /* got a key */
			{
				int id = status->s.terminus.idx;
				GlimpseTypeHandler_t* handler = status->s.terminus.handler;
				//const char* next_p = handler->parse(p, storage->data[id], handler->parse_data); /* parse the value */
				const char* next_p = glimpse_stack_get_parser(&thread->stack, handler)(p, storage->data[id], handler->parse_data, thread_data);
				if(NULL == next_p)
				{
					GLIMPSE_LOG_FATAL("value parser returns a error status, abort");
					return NULL;
				}
#ifdef STRING_SEPERATOR_SUPPORT
				int i;
				for(i = 0; tree->sep_f[i] && next_p[i] && tree->sep_f[i] == next_p[i]; i ++);
				if(0 == tree->sep_f[i]) /* a seprator */
					p = next_p + i;
				else /* something unexcepted */
					return next_p; /* scan terminated */
#else
				if(next_p[0] == tree->sep_f) p = next_p + 1;
				else return next_p;  /* handler stack checked */
#endif
				status = NULL;  /* ready to scan next field */
			}
		} 
		/* 
		 * else the key does not match anything, so rematch 
		 */
	}
	return p;
}
static char* _glimpse_typeflag_sublog_tostring_imp(GlimpseTrieNode_t* node, int level, char* buffer, size_t size)
{
	if(node == NULL) return buffer + snprintf(buffer, size, "(nullnode),");
	char* p = buffer;
	static char key[1024];
	if(node->term)
	{
		int s = 1024;
		char *q;
		if(s > level) s = level;
		key[s] = 0;
		if(s) key[s-1] = 0;
		p += snprintf(p, size - (p - buffer), "%s:", key);
		if(node->s.terminus.handler)
			 q = glimpse_typesystem_typehandler_tostring(node->s.terminus.handler, p, size - (p - buffer));
		if(NULL == q) 
			p += snprintf(p, size - (p - buffer), "(undefined)");
		else
			p = q;
		p += snprintf(p, size - (p - buffer), ",");
		return p;
	}
	else
	{
		GlimpseCharHashNode_t* n;
		int s = 1024;
		if(s > level) s = level;
		for(n = node->s.child->first; n; n = n->list)
		{
			key[s] = n->key;
			char* q = _glimpse_typeflag_sublog_tostring_imp((GlimpseTrieNode_t*)n->value, level + 1, p, size - (p - buffer));
			if(NULL == q) 
				p += snprintf(p, size - (p - buffer), "(undefined),");
			else
				p = q;
		}
		return p;
	}
}
char* glimpse_typeflag_sublog_tostring(GlimpseTypeHandler_t* type, char* buffer, size_t size)
{
	if(NULL == type || NULL == buffer) return NULL;
	char *p = buffer;
	p += snprintf(buffer, size - (p - buffer), "Log{");
	p = _glimpse_typeflag_sublog_tostring_imp(type->type->param.sublog.tree->root, 0, p, size - (p - buffer));
	p += snprintf(p, size - (p - buffer), "sep_kv:'%c',sep_f:'%c'}", 
				  type->type->param.sublog.tree->sep_kv,
				  type->type->param.sublog.tree->sep_f);
	return p;
}
