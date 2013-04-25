/* tree.h - the parser tree 
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
	
#ifndef __GLIMPSE_TREE_H__
#define __GLIMPSE_TREE_H__

#include <stdint.h>

#include <glimpse/future.h>
#include <glimpse/log.h>
#include <glimpse/typesystem.h>
#include <glimpse/data.h>
#include <glimpse/def.h>
#ifdef CHAR_HASH_TABLE
#	include <glimpse/chartable.h>
#endif

typedef struct _glimpse_typedesc GlimpseTypeDesc_t;
typedef struct _glimpse_type_handler GlimpseTypeHandler_t;
typedef struct _glimpse_data_model_t GlimpseDataModel_t;

typedef struct _glimpse_trie_node{
	uint8_t term;
	union{
#ifdef CHAR_HASH_TABLE
		GlimpseCharTable_t* child;
#else
		struct _glimpse_trie_node* child[256];
#endif
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
#ifdef CHAR_HASH_TABLE
	return glimpse_chartable_find(status->s.child,(uint8_t)ch);
#else
	return status->s.child[(uint8_t)ch];
#endif
}
#endif
