#include <stdio.h>

#include <glimpse/builtintype.h>
#include <glimpse/typesystem.h>
#include <glimpse/vector.h>
#include <glimpse/log.h>
#include <glimpse/data.h>
#include <glimpse/tree.h>
#include <glimpse/thread.h>
#include <glimpse/stack.h>
void* glimpse_builtintype_vector_alloc(void* userdata)
{
	return glimpse_vector_new(sizeof(void*));
}
int glimpse_builtintype_vector_init(void* data, void* userdata)
{
	glimpse_vector_init((GlimpseVector_t*)data);
	return 0;
}
int glimpse_builtintype_vector_finalize(void* data, void* userdata)
{
	int i;
	GlimpseVector_t* vec = (GlimpseVector_t*) data;
	GlimpseTypeHandler_t* handler = (GlimpseTypeHandler_t*) userdata;
	for(i = 0; i < vec->size; i ++)
	{
		void** addr = (void**)glimpse_vector_get(vec, i);
		if(NULL == addr) continue;
#ifdef LAZY_INSTANCE
		glimpse_typesystem_typehandler_fianlize_instance(*addr);  
		/* we do not actually recycle the memory, just finalize it */
#else
		glimpse_typesystem_typehandler_free_instance(*addr);
#endif
	}
	return 0;
}
int glimpse_builtintype_vector_free(void* data, void* userdata)  
{
	glimpse_vector_free((GlimpseVector_t*)data);
	return 0;
}
const char* glimpse_builtintype_vector_parse(const char* text, void* result, void* user_data, void* thread_data)
{
	GlimpseTypeVectorParserParam_t* param = (GlimpseTypeVectorParserParam_t*)user_data;
	GlimpseTypeHandler_t* handler = param->basetype_handler;
	GlimpseThreadData_t* thread = (GlimpseThreadData_t*) thread_data;
	if(handler->parse == NULL || thread == NULL) return text;
	const char* next;
	void* instance;
	while(*text 
#ifdef HANDLER_STACK

#else
			&& thread->is_term[(uint8_t)*text] != thread->__true__
#endif
		)
	{
		//memory = handler->alloc(handler->alloc_data);
#ifdef LAZY_INSTANCE
		void* free_space = glimpse_vector_next_free_space((GlimpseVector_t*)result);  /* we attempt to reuse the space */
		if(NULL == free_space) instance = glimpse_typesystem_typehandler_new_instance(handler);
		else 
		{
			instance = *(void**)free_space;
			if(GLIMPSE_ESUCCESS != glimpse_typesystem_typehandler_init_instance(instance))  instance = NULL; 
		}
#else /*LAZY_INSTANCE*/
		instance = glimpse_typesystem_typehandler_new_instance(handler);
#endif /*LAZY_INSTANCE*/
		if(NULL == instance)
		{
			GLIMPSE_LOG_ERROR("can not allocate memory");
#ifdef HANDLER_STACK
			glimpse_stack_print(&thread->stack);
#endif  /*HANDLER_STACK*/
			return NULL;
		}
#ifdef HANDLER_STACK
		const char* next = glimpse_stack_get_parser(&thread->stack, handler)(text, instance, handler->parse_data, thread_data); /* thread support */
		glimpse_stack_pop(&thread->stack); /* cleanup the stack */
#else /*HANDLER_STACK*/
		thread->is_term[(uint8_t)param->sep] = thread->__true__;
		const char* next = handler->parse(text, instance, handler->parse_data, thread_data);
		thread->is_term[(uint8_t)param->sep] = 0;
#endif /*HANDLER_STACK*/
		if(NULL == next)
		{
			GLIMPSE_LOG_ERROR("failed to parse text");
#ifdef HANDLER_STACK
			glimpse_stack_print(&thread->stack);
#endif /*HANDLER_STACK*/
			glimpse_typesystem_typehandler_free_instance(instance);
			return text;
		}
		int rc = glimpse_vector_push((GlimpseVector_t*)result, &instance);
		if(GLIMPSE_ESUCCESS != rc)
		{
			GLIMPSE_LOG_WARNING("failed to insert to vector");
#ifdef HANDLER_STACK
			glimpse_stack_print(&thread->stack);
#endif /*HANDLER_STACK*/
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
#else /*STRING_SEPERATOR_SUPPORT*/
		if(text[0] && text[0] == param->sep) text ++;
		else return text;  /* the handler stack already checked */
#endif /*STRING_SEPERATOR_SUPPORT*/
	}
}
char* glimpse_builtintype_vector_tostring(GlimpseTypeHandler_t* handler, char* buffer, size_t size)
{
	char* p = buffer;
	p += glimpse_snprintf(p, size - (p - buffer), "Vector{seperator: "
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
	p += glimpse_snprintf(p, size - (p - buffer), "}");
	return p;
}
void* glimpse_builtintype_sublog_alloc(void* userdata)
{
	return glimpse_data_instance_new((GlimpseDataModel_t*)userdata);
}
int glimpse_builtintype_sublog_free(void* data, void* userdata)
{
	glimpse_data_instance_free((GlimpseDataInstance_t*)data);
	return 0;
}
int glimpse_builtintype_sublog_init(void* data, void* userdata)
{
	return glimpse_data_instance_init((GlimpseDataInstance_t*)data);
}
int glimpse_builtintype_sublog_finalize(void* data, void* userdata)
{
	glimpse_data_instance_finalize((GlimpseDataInstance_t*)data);
	return 0;
}
const char* glimpse_builtintype_sublog_parse(const char* text, void* result, void* user_data, void* thread_data)
{
	GlimpseParseTree_t* tree = (GlimpseParseTree_t*)user_data;
	GlimpseThreadData_t* thread = (GlimpseThreadData_t*)thread_data;
	GlimpseDataInstance_t* storage = (GlimpseDataInstance_t*) result;
	if(NULL == tree || NULL == text || NULL == storage) return NULL;
	GlimpseParserStatus_t status = NULL;
	const char* p;
	int flag = 1;
	for(p = text;*p 
#ifndef HANDLER_STACK
			&& thread->is_term[(uint8_t)*p] != thread->__true__
#endif 
		;)
	{
		if(0 == flag)
			if(*(p++) != tree->sep_f) continue;
		flag = 1;
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
#ifdef HANDLER_STACK
				const char* next_p = glimpse_stack_get_parser(&thread->stack, handler)(p, storage->data[id], handler->parse_data, thread_data);
				glimpse_stack_pop(&thread->stack);
#else
				thread->is_term[(uint32_t)tree->sep_f] = thread->__true__;
				const char* next_p = handler->parse(p, storage->data[id], handler->parse_data, thread_data);
				thread->is_term[(uint32_t)tree->sep_f] = 0;
#endif
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
		else flag = 0;  /*ignore*/
		/* 
		 * else the key does not match anything, so rematch 
		 */
	}
	return p;
}
static char* _glimpse_builtintype_sublog_tostring_imp(GlimpseTrieNode_t* node, int level, char* buffer, size_t size, char* key)
{
	if(node == NULL) return buffer + glimpse_snprintf(buffer, size, "(nullnode),");
	char* p = buffer;
	if(node->term)
	{
		int s = 1024;
		char *q;
		if(s > level) s = level;
		key[s] = 0;
		if(s) key[s-1] = 0;
		p += glimpse_snprintf(p, size - (p - buffer), "%s:", key);
		if(node->s.terminus.handler)
			 q = glimpse_typesystem_typehandler_tostring(node->s.terminus.handler, p, size - (p - buffer));
		if(NULL == q) 
			p += glimpse_snprintf(p, size - (p - buffer), "(undefined)");
		else
			p = q;
		p += glimpse_snprintf(p, size - (p - buffer), ",");
		return p;
	}
	else
	{
		int s = 1024;
		if(s > level) s = level;
#ifdef CHAR_HASH_TABLE
		GlimpseCharHashNode_t* n;
		for(n = node->s.child->first; n; n = n->list)
		{
			key[s] = n->key;
			char* q = _glimpse_builtintype_sublog_tostring_imp((GlimpseTrieNode_t*)n->value, level + 1, p, size - (p - buffer),key);
#else
		int i;
		for(i = 0; i < 256; i ++)
		{
			if(NULL == node->s.child[i]) continue;
			key[s] = i;
			char* q = _glimpse_builtintype_sublog_tostring_imp((GlimpseTrieNode_t*)node->s.child[i], level + 1, p, size - (p - buffer),key);
#endif
			if(NULL == q) 
				p += glimpse_snprintf(p, size - (p - buffer), "(undefined),");
			else
				p = q;
		}
		return p;
	}
}
char* glimpse_builtintype_sublog_tostring(GlimpseTypeHandler_t* type, char* buffer, size_t size)
{
	char key[1024];
	if(NULL == type || NULL == buffer) return NULL;
	char *p = buffer;
	p += glimpse_snprintf(buffer, size - (p - buffer), "Log{");
	p = _glimpse_builtintype_sublog_tostring_imp(type->type->param.sublog.tree->root, 0, p, size - (p - buffer),key);
	p += glimpse_snprintf(p, size - (p - buffer), "sep_kv:'%c',sep_f:'%c'}", 
				  type->type->param.sublog.tree->sep_kv,
				  type->type->param.sublog.tree->sep_f);
	return p;
}
