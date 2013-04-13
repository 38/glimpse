#include <tree.h>
#include <string.h>
#include <malloc.h>
GlimpseTrieNode_t* glimpse_tree_trienode_new(int terminus)
{
	GlimpseTrieNode_t* ret = (GlimpseTrieNode_t*)malloc(sizeof(GlimpseTrieNode_t));
	if(NULL == ret) return NULL;
	ret->term = terminus;
	memset(ret, 0, sizeof(GlimpseTrieNode_t));
	if(!terminus)
	{
		/* allocate chartable for child */
		ret->s.child = glimpse_chartable_new();
		if(NULL == ret->s.child) goto ERR;
	}
	/* else: we do not handle parse handler here */
	return ret;
ERR:
	/* assumes ret is not NULL */
	/* because we does not process terminus handler here,
	 * so it's ok that we does not free the memory for 
	 * terminus handler
	 */
	if (NULL != ret->s.child) glimpse_chartable_free(ret->s.child);
	free(ret);
	return NULL;
}
void glimpse_tree_trienode_free(GlimpseTrieNode_t* node)
{
	if(NULL == node) return;
	if(node->term)
	{
		
		if(NULL != node->s.terminus.handler) 
		{
			if(node->s.terminus.handler->type->flags&(GLIMPSE_TYPEFLAG_SUBLOG|GLIMPSE_TYPEFLAG_VECTOR|GLIMPSE_TYPEFLAG_MAP))
			{
				//TODO: free Vector,Map,Sublog
			}
			else
			{
				if(node->s.terminus.data) 
					node->s.terminus.handler->free(node->s.terminus.data, node->s.terminus.handler->free_data);
			}
			free(node->s.terminus.handler); /*handler is allocated by glimpse_tree_insert */
		}
	}
	else if(NULL != node->s.child)	/* child table should be desposed */
	{
		GlimpseCharHashNode_t* p;
		for(p = node->s.child->first; NULL != p; p = p->list)
			glimpse_tree_trienode_free((GlimpseTrieNode_t*)p->value);
		glimpse_chartable_free(node->s.child);
	}
	free(node);
}
GlimpseParseTree_t* glimpse_tree_new(const char* sep_f, const char* sep_kv)
{
	if(NULL == sep_f || NULL== sep_kv) return NULL;
	if(strlen(sep_f) == 0 || strlen(sep_kv) == 0)
	{
		GLIMPSE_LOG_ERROR("glimpse can not parse log with no key-value/field seperator");
		return NULL;
	}
	GlimpseParseTree_t* ret = (GlimpseParseTree_t*)malloc(sizeof(GlimpseParseTree_t));
	if(NULL == ret) return NULL;
	memset(ret, 0, sizeof(GlimpseParseTree_t));
	ret->sep_f = sep_f;
	ret->sep_kv = sep_kv;
	return ret;
}
void glimpse_tree_free(GlimpseParseTree_t* tree)
{
	if(NULL == tree) return;
	if(NULL != tree->root) glimpse_tree_trienode_free(tree->root);
}
int glimpse_tree_set_sperator(GlimpseParseTree_t* tree, const char* f, const char* kv)
{
	if(NULL == tree) return EINVAILDARG;
	if(NULL != f && strlen(f) != 0) tree->sep_f = f;
	if(NULL != kv && strlen(kv) != 0) tree->sep_kv = kv;
	return ESUCCESS;
}
int glimpse_tree_insert(GlimpseParseTree_t* tree, const char* field, GlimpseTypeDesc_t* type)
{
	if(NULL == tree || NULL == field || NULL == type) return EINVAILDARG;
	if(NULL == tree->root) tree->root = glimpse_tree_trienode_new(0);
	const char* p;
	GlimpseTrieNode_t* cur = tree->root;
	/* 
	 * insert field to the trie, we assume the kv sep is a nonempty string 
	 * so there will be no terminus
	 */
	for(p = field; *p ; p ++)
	{
		uint8_t val = (uint8_t)*p;
		if(cur->term == 1)  /* we meet some terminus */
		{
			/* this condition is ambigious 
			 * for example:
			 * kv sep is '='
			 * parser tree previously got a field named `field'
			 * then the parser got a field named `field='
			 * the field will be ambious, because a string
			 * field==3 can be parse as `field=' = `3'
			 * or `field' = `=3'
			 * So it's an ambigious condition which should be forbidden
			 */
			char buffer[32];
			size_t  trunc = sizeof(buffer);
			if(trunc <  p - field) trunc = p - field;
			strncpy(buffer, field, trunc);
			GLIMPSE_LOG_ERROR("ambigous key name %s(new) and %s(old)", field, buffer);
			return EINVAILDARG;
		}
		GlimpseTrieNode_t* next = (GlimpseTrieNode_t*)glimpse_chartable_find(cur->s.child, val);
		if(NULL == next) /* child does not exist, allocate it */
		{
			GlimpseTrieNode_t* new = glimpse_tree_trienode_new(0); /* Because the kv sep is nonempty string, the node must be an internal node */
			if(NULL == new)
			{
				GLIMPSE_LOG_ERROR("can not create new trienode");
				return EUNKNOWN;
			}
			int rc = glimpse_chartable_insert(cur->s.child, val, new);
			if(rc != ESUCCESS)
			{
				glimpse_tree_trienode_free(new);
				GLIMPSE_LOG_ERROR("can not insert new trienode to chartable");
				return rc;
			}
			next = new;
		}
		cur = next; /* go down through the edge */
	}
	/*
	 * insert k-v sep into tree
	 */
	for(p = tree->sep_kv; *p; p ++)
	{
		uint8_t val = (uint8_t)*p;
		if(cur->term == 1) /* ambigious like sp=`==' field f1, f1=, f1===2 */
		{
			char buffer[32];
			size_t  trunc = sizeof(buffer);
			size_t  len = strlen(field) - strlen(p);
			if(trunc < len) trunc = len;
			strncpy(buffer, field, trunc);
			GLIMPSE_LOG_ERROR("ambigous key name %s(new) and %s(old)", field, buffer);
			return EINVAILDARG;
		}
		GlimpseTrieNode_t* next = (GlimpseTrieNode_t*)glimpse_chartable_find(cur->s.child, val);
		if(NULL == next)
		{
			GlimpseTrieNode_t* new = glimpse_tree_trienode_new(0 == p[1]); /* p[1] == 0 <==> the last char of sep, terminus */
			if(NULL == new)
			{
				GLIMPSE_LOG_ERROR("can not create new trienode");
				return EUNKNOWN;
			}
			int rc = glimpse_chartable_insert(cur->s.child, val, new);
			if(rc != ESUCCESS)
			{
				glimpse_tree_trienode_free(new);
				GLIMPSE_LOG_ERROR("can not insert new trienode to chartable");
				return rc;
			}
			next = new;
		}
		cur = next;
	}
	if(!cur->term) /* check if cur->term == 1 */
	{
		GLIMPSE_LOG_ERROR("cur->term unexceptedly be 0");
		return EUNKNOWN;
	}
	cur->s.terminus.handler = (GlimpseTypeHandler_t*)malloc(sizeof(GlimpseTypeHandler_t));
	int rc = glimpse_typesystem_query(type, cur->s.terminus.handler);
	if(ESUCCESS != rc) return rc;
	if(type->flags&(GLIMPSE_TYPEFLAG_SUBLOG|GLIMPSE_TYPEFLAG_VECTOR|GLIMPSE_TYPEFLAG_MAP))
	{
		// TODO: allocate vector,sublog, map
	}
	else
	{
		cur->s.terminus.data = cur->s.terminus.handler->alloc(cur->s.terminus.handler->alloc_data);
		if(NULL == cur->s.terminus.data)
		{
			GLIMPSE_LOG_ERROR("can not allocate memory for data %s", field);
			return EUNKNOWN;
		}
	}
	return ESUCCESS;
}

