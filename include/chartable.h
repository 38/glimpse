#ifndef __CHARTABLE_H__
#define __CHARTABLE_H__
#include <retval.h>
#include <log.h>
#include <stdint.h>
#include <stdlib.h>
#ifndef GLIMPSE_CHARTABLE_CONFLICT_TOLERANCE
#	define GLIMPSE_CHARTABLE_CONFLICT_TOLERANCE 2 /* the max chain length allowed */
#endif
typedef struct _glimpse_char_hash_node_t{
	uint8_t key;
	uint8_t size;  /* how many element does the chain have */
	struct _glimpse_char_hash_node_t* next;
	struct _glimpse_char_hash_node_t* list; /* transverse all element as a linked list */
	void* value;	
} GlimpseCharHashNode_t;
/* Variant length char hash table */
typedef struct _glimpse_char_table_t{
	size_t level;   /* number of slot is different between levels, _glimpse_chartable_hashnum[level] is the capacity */
	size_t count;
	GlimpseCharHashNode_t** slots;
	/* following member are used for transverse the table */
	GlimpseCharHashNode_t*  first;
	GlimpseCharHashNode_t*  last;
	/* following member is for optimization */
	int max_chain_len;
} GlimpseCharTable_t;
GlimpseCharTable_t* glimpse_chartable_new();
void glimpse_chartable_free(GlimpseCharTable_t* table);
int  glimpse_chartable_insert(GlimpseCharTable_t* table, uint8_t key ,void* value);
void*  glimpse_chartable_find(GlimpseCharTable_t* table, uint8_t key);
#endif
