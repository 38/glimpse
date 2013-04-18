#ifndef __TREE_H__
#define __TREE_H__
#include <future.h>
#include <log.h>
#include <chartable.h>
#include <typesystem.h>
#include <stdint.h>
#include <data.h>

typedef struct _glimpse_typedesc GlimpseTypeDesc_t;
typedef struct _glimpse_type_handler GlimpseTypeHandler_t;
typedef struct _glimpse_data_model_t GlimpseDataModel_t;

typedef struct _glimpse_trie_node{
	uint8_t term;
	union{
		GlimpseCharTable_t* child;
		struct{
			GlimpseTypeHandler_t* handler;
			int idx; 
		} terminus;
	} s;
} GlimpseTrieNode_t;
/*Trie Node manipulation*/
GlimpseTrieNode_t* glimpse_tree_trienode_new(int terminus);   /* terminus indicates if the node a terminal node */
void glimpse_tree_trienode_free(GlimpseTrieNode_t* node);

typedef struct _glimpse_parse_tree{
#ifdef STRING_SEPERATOR_SUPPORT
	const char* sep_f;  /* seperator between feilds */
	const char* sep_kv; /* seperator between key and value */
#else
	char sep_f;
	char sep_kv;
#endif /* STRING_SEPERATOR_SUPPORT */
	GlimpseTrieNode_t* root;
	GlimpseDataModel_t* model; /* data model for log */
} GlimpseParseTree_t;

/* Tree manipulation */
#ifdef STRING_SEPERATOR_SUPPORT
GlimpseParseTree_t* glimpse_tree_new(const char* sep_f, const char* sep_kv);  /* future */
#else
GlimpseParseTree_t* glimpse_tree_new(char sep_f, char sep_kv);
#endif /*STRING_SEPERATOR_SUPPORT*/
void glimpse_tree_free(GlimpseParseTree_t* tree);
#ifdef STRING_SEPERATOR_SUPPORT
int glimpse_tree_set_sperator(GlimpseParseTree_t* tree, const char* f, const char* kv); /* future */
#else 
int glimpse_tree_set_sperator(GlimpseParseTree_t* tree, char f, char kv);
#endif /*STRING_SEPERATOR_SUPPORT*/
int glimpse_tree_insert(GlimpseParseTree_t* tree, const char* field, GlimpseTypeDesc_t* type);
int  glimpse_tree_query(GlimpseParseTree_t* tree, const char* key); /* Ugly name for return type */
/* Operation */
typedef GlimpseTrieNode_t* GlimpseParserStatus_t;
static inline GlimpseParserStatus_t glimpse_tree_scan_start(GlimpseParseTree_t* tree)
{
	if(NULL == tree) return NULL;
	return tree->root;
}
static inline GlimpseParserStatus_t glimpse_tree_scan(GlimpseParserStatus_t status, char ch)
{
	if(1 == status->term) return NULL; /* terminus, can not walk down */
	return (GlimpseParserStatus_t)glimpse_chartable_find(status->s.child, (uint8_t)ch);
}
#endif
