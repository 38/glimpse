#include <tree.h>
#include <string.h>
#include <malloc.h>
#include <retval.h>
GlimpseTrieNode_t* glimpse_tree_trienode_new(int terminus)
{
	GlimpseTrieNode_t* ret = (GlimpseTrieNode_t*)malloc(sizeof(GlimpseTrieNode_t));
	if(NULL == ret) return NULL;
	memset(ret, 0, sizeof(GlimpseTrieNode_t));
	ret->term = terminus;
	if(!terminus)
	{
#ifdef CHAR_HASH_TABLE
		/* allocate chartable for child */
		ret->s.child = glimpse_chartable_new();
#endif
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
#ifdef CHAR_HASH_TABLE
	if (NULL != ret->s.child) glimpse_chartable_free(ret->s.child);
#endif
	free(ret);
	return NULL;
}
void glimpse_tree_trienode_free(GlimpseTrieNode_t* node)
{
	if(NULL == node) return;
	if(node->term)
	{
		/* bacause the handler is managed by the Type System, we do not need to free them here */
	}
#ifdef CHAR_HASH_TABLE
	else if(NULL != node->s.child)	/* child table should be desposed */
	{
		GlimpseCharHashNode_t* p;
		/* free the child node recursively */
		for(p = node->s.child->first; NULL != p; p = p->list)
			glimpse_tree_trienode_free((GlimpseTrieNode_t*)p->value);
		glimpse_chartable_free(node->s.child);
	}
#else
	else
	{
		int i;
		for(i = 0; i < 256; i ++)
			if(node->s.child[i])
				glimpse_tree_trienode_free(node->s.child[i]);
	}
#endif
	free(node);
}
#ifdef STRING_SEPERATOR_SUPPORT
GlimpseParseTree_t* glimpse_tree_new(const char* sep_f, const char* sep_kv) /* string seperator support */
#else
GlimpseParseTree_t* glimpse_tree_new(char sep_f, char sep_kv) 
#endif
{
#ifdef STRING_SEPERATOR_SUPPORT
	if(NULL == sep_f || NULL== sep_kv) return NULL;
	if(strlen(sep_f) == 0 || strlen(sep_kv) == 0)
	{
		GLIMPSE_LOG_ERROR("glimpse can not parse log with no key-value/field seperator");
		return NULL;
	}
#endif
	GlimpseParseTree_t* ret = (GlimpseParseTree_t*)malloc(sizeof(GlimpseParseTree_t));
	if(NULL == ret) return NULL;
	ret->model = glimpse_data_model_new();
	if(NULL == ret->model)
	{
		free(ret);
		return NULL;
	}
	ret->sep_f = sep_f;
	ret->sep_kv = sep_kv;
	ret->root = NULL;
	return ret;
}
void glimpse_tree_free(GlimpseParseTree_t* tree)
{
	if(NULL == tree) return;
	if(NULL != tree->root) glimpse_tree_trienode_free(tree->root);
	if(NULL != tree->model) glimpse_data_model_free(tree->model);
}
#ifdef STRING_SEPERATOR_SUPPORT
int glimpse_tree_set_sperator(GlimpseParseTree_t* tree, const char* f, const char* kv)
#else
int glimpse_tree_set_sperator(GlimpseParseTree_t* tree, char f, char kv)
#endif
{
	if(NULL == tree) return EINVAILDARG;
#ifdef STRING_SEPERATOR_SUPPORT
	if(NULL != f && strlen(f) != 0) tree->sep_f = f;
	if(NULL != kv && strlen(kv) != 0) tree->sep_kv = kv;
#else
	tree->sep_f = f;
	tree->sep_kv = kv;
#endif
	return ESUCCESS;
}
int glimpse_tree_insert(GlimpseParseTree_t* tree, const char* field, GlimpseTypeDesc_t* type)
{
	if(NULL == tree || NULL == field || NULL == type) return EINVAILDARG;
	if(NULL == tree->root) tree->root = glimpse_tree_trienode_new(0); /* empty string not be term */
	const char* p;
	GlimpseTrieNode_t* cur = tree->root;
	/* 
	 * insert field to the trie, we assume the kv sep is a nonempty string 
	 * so there will be no terminus in `field'
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
#ifdef CHAR_HASH_TABLE
		GlimpseTrieNode_t* next = (GlimpseTrieNode_t*)glimpse_chartable_find(cur->s.child, val);
#else
		GlimpseTrieNode_t* next = cur->s.child[val];
#endif
		if(NULL == next) /* child does not exist, allocate it */
		{
			GlimpseTrieNode_t* new = glimpse_tree_trienode_new(0); /* Because the kv sep is nonempty string, the node must be an internal node */
			if(NULL == new)
			{
				GLIMPSE_LOG_ERROR("can not create new trienode");
				return EUNKNOWN;
			}
#ifdef CHAR_HASH_TABLE
			int rc = glimpse_chartable_insert(cur->s.child, val, new);
			if(rc != ESUCCESS)
			{
				glimpse_tree_trienode_free(new);
				GLIMPSE_LOG_ERROR("can not insert new trienode to chartable");
				return rc;
			}
#else
			cur->s.child[val] = new;
#endif
			next = new;
		}
		cur = next; /* go down through the edge */
	}
#ifdef STRING_SEPERATOR_SUPPORT
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
#ifdef CHAR_HASH_TABLE
		GlimpseTrieNode_t* next = (GlimpseTrieNode_t*)glimpse_chartable_find(cur->s.child, val);
#else
		GlimpseTrieNode_t* next = cur->cur.child[val];
#endif
		if(NULL == next)
		{
			GlimpseTrieNode_t* new = glimpse_tree_trienode_new(0 == p[1]); /* p[1] == 0 <==> the last char of sep, terminus */
			if(NULL == new)
			{
				GLIMPSE_LOG_ERROR("can not create new trienode");
				return EUNKNOWN;
			}
#ifdef CHAR_HASH_TABLE
			int rc = glimpse_chartable_insert(cur->s.child, val, new);
			if(rc != ESUCCESS)
			{
				glimpse_tree_trienode_free(new);
				GLIMPSE_LOG_ERROR("can not insert new trienode to chartable");
				return rc;
			}
#else
			cur->s.child[val] = new;
#endif
			next = new;
		}
		cur = next;
	}
#else
	/* check if the field is ambigious */
	if(cur->term == 1)
	{
		char buffer[32];
		size_t trunc = sizeof(buffer);
		size_t len = strlen(field) - 1;
		strncpy(buffer, field, trunc);
		GLIMPSE_LOG_ERROR("ambigous key name %s(new) and %s(old)", field, buffer);
		return EINVAILDARG;
	}

	uint8_t val = (uint8_t) tree->sep_kv;
#ifdef CHAR_HASH_TABLE
	GlimpseTrieNode_t* next = (GlimpseTrieNode_t*)glimpse_chartable_find(cur->s.child, val);
#else
	GlimpseTrieNode_t* next = cur->s.child[val];
#endif
	
	if(NULL == next)
	{
		GlimpseTrieNode_t* new = glimpse_tree_trienode_new(1);  /* it's a terminus */
		if(NULL == new)
		{
			GLIMPSE_LOG_ERROR("can not create new trienode");
			return EUNKNOWN;
		}
#ifdef CHAR_HASH_TABLE
		int rc = glimpse_chartable_insert(cur->s.child, val, new);
		if(rc != ESUCCESS)
		{
			glimpse_tree_trienode_free(new);
			GLIMPSE_LOG_ERROR("can not insert new trienode to chartable");
			return rc;
		}
#else
		cur->s.child[val] = new;
#endif
		next = new;
	}
	else if(next->term)
	{
		GLIMPSE_LOG_ERROR("duplicated field");
		return EINVAILDARG;
	}
	cur = next;
#endif
	if(!cur->term) /* check if cur->term == 1 */
	{
		GLIMPSE_LOG_ERROR("cur->term unexceptedly be 0");
		return EUNKNOWN;
	}
	cur->s.terminus.handler = glimpse_typesystem_query(type);
	if(NULL == cur->s.terminus.handler) return EUNKNOWN;
	int rc;
	rc = cur->s.terminus.idx = glimpse_data_model_insert(tree->model, cur->s.terminus.handler);
	if(rc < 0) return rc;
	return ESUCCESS;
}
GlimpseDataOffset_t glimpse_tree_query(GlimpseParseTree_t* tree, const char* key)
{
	if(NULL == tree || NULL == key) return;
	GlimpseTrieNode_t* node;
	for(node = tree->root; node && !node->term && *key; key++)
	{
		uint8_t val = (uint8_t)*key;
#ifdef CHAR_HASH_TABLE
		if(node->s.child) node = (GlimpseTrieNode_t*)glimpse_chartable_find(node->s.child, val);
		else node = NULL;
#else
		node = node->s.child[val];
#endif
	}
	if(NULL == node) 
		return -1;
	if(node->term) /* node->term should not be true, the reason described above */
		return -1;
	/* here, *key must be 0 */
#ifdef STRING_SEPERATOR_SUPPORT
	/* TODO */
#else
#ifdef CHAR_HASH_TABLE
	if(node->s.child) 
		node = (GlimpseTrieNode_t*)glimpse_chartable_find(node->s.child, (uint8_t)tree->sep_kv);
	else node = NULL;
#else
	node = node->s.child[(uint8_t)tree->sep_kv];
#endif
#endif
	if(NULL == node || !node->term) return -1;
	return node->s.terminus.idx; 
}
