/* chartable.c -   
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
	
#ifdef CHAR_HASH_TABLE
/* this file is deprecated, and may be removed in the future */
#include <chartable.h>
#include <malloc.h>
#include <string.h>
int _glimpse_chartable_hashnum[] = {1, 2, 5, 11, 23, 47, 97, 197, 257, 0}; /* prime numbers */
GlimpseCharTable_t* glimpse_chartable_new()
{
	GlimpseCharTable_t* ret = (GlimpseCharTable_t*)malloc(sizeof(GlimpseCharTable_t));
	if(NULL == ret) goto ERR;
	ret->level = 0;
	ret->count = 0;
	ret->first = NULL;
	ret->last = NULL;
	ret->max_chain_len = 0;
	ret->slots = (GlimpseCharHashNode_t**)malloc(sizeof(GlimpseCharHashNode_t*) * _glimpse_chartable_hashnum[ret->level]);
	if(NULL == ret->slots) goto ERR;
	memset(ret->slots, 0,sizeof(GlimpseCharHashNode_t*) * _glimpse_chartable_hashnum[ret->level]);
	return ret;
ERR:
	if(ret->slots) free(ret->slots);
	if(ret) free(ret);
	return NULL;
}
void glimpse_chartable_free(GlimpseCharTable_t* table)
{
	if(NULL == table) return;
	if(table->slots) free(table->slots);
	GlimpseCharHashNode_t* p;
	for(p = table->first; p; )
	{
		GlimpseCharHashNode_t* tmp = p;
		p = p->list;
		free(tmp);
	}
	free(table);
}
static int _glimpse_chartable_resize(GlimpseCharTable_t* table)
{
	if(NULL == table) return EINVAILDARG;
	size_t size = _glimpse_chartable_hashnum[table->level + 1];
	if(0 == size) 
	{
		GLIMPSE_LOG_ERROR("can't increase chartable, too many elements");
		return ELISTFULL;
	}
	GlimpseCharHashNode_t** new = (GlimpseCharHashNode_t**)malloc(sizeof(GlimpseCharHashNode_t*) * size);
	if(NULL == new) 
	{
		GLIMPSE_LOG_ERROR("Can not malloc");
		return EUNKNOWN;
	}
	free(table->slots);
	table->slots = new;
	table->level ++;
	table->max_chain_len = 0;
	memset(table->slots, 0, sizeof(GlimpseCharHashNode_t*) * size);
	/* rehash the table */
	GlimpseCharHashNode_t* node;
	for(node = table->first; NULL != node; node = node->list)
	{
		node->next = table->slots[node->key%size];
		if(NULL == node->next) node->size = 1;
		else node->size = node->next->size + 1;
		table->slots[node->key%size] = node;
		if(node->size > table->max_chain_len) 
			table->max_chain_len = node->size;
	}
	GLIMPSE_LOG_DEBUG("Resized hashtable from %d to %d, max_chain_len = %d", 
			_glimpse_chartable_hashnum[table->level-1],
			_glimpse_chartable_hashnum[table->level],
			table->max_chain_len);
	return ESUCCESS;
}
static inline GlimpseCharHashNode_t* _glimpse_chartable_find(GlimpseCharTable_t* table, uint8_t key)
{
	if(NULL == table) return NULL;
	int h = key%_glimpse_chartable_hashnum[table->level];
	GlimpseCharHashNode_t* p;
	for(p = table->slots[h]; p; p = p->next)
		if(p->key == key) return p;
	return NULL;
}
int  glimpse_chartable_insert(GlimpseCharTable_t* table, uint8_t key ,void* value)
{
	if(NULL != _glimpse_chartable_find(table, key))
	{
		GLIMPSE_LOG_TRACE("Duplicated element, ignored");
		return ESUCCESS;
	}
	if(_glimpse_chartable_hashnum[table->level] == table->count)
	{
		GLIMPSE_LOG_DEBUG("Because the hash table is full, resize the table");
		int ret;
		if(ESUCCESS != (ret = _glimpse_chartable_resize(table)))
		{
			GLIMPSE_LOG_WARNING("Failed to resize the chartable, insertion failed");
			return ret;
		}
	}
	int h = key%_glimpse_chartable_hashnum[table->level];
	GlimpseCharHashNode_t* new = (GlimpseCharHashNode_t*)malloc(sizeof(GlimpseCharHashNode_t));
	new->key = key;
	new->value = value;
	/* maintain transverse pointer */
	new->list = NULL;
	if(NULL != table->last) table->last->list = new;
	table->last = new;
	if(NULL == table->first) table->first = new;
	/* maintain hash chain pointer */
	new->next = table->slots[h];
	table->slots[h] = new;
	if(NULL == new->next) new->size = 1;
	else new->size = new->next->size + 1;
	if(table->max_chain_len < new->size) 
		table->max_chain_len = new->size;
	/* if max_chain_len > GLIMPSE_CHARTABLE_CONFLICT_TOLERANCE, resize the table */
	while(table->max_chain_len > GLIMPSE_CHARTABLE_CONFLICT_TOLERANCE)
	{
		GLIMPSE_LOG_DEBUG("Because max_chain_len = %d, resize the table", table->max_chain_len);
		int ret = _glimpse_chartable_resize(table);
		if(ESUCCESS != ret) return ret;
	}
	return ESUCCESS;
}
void*  glimpse_chartable_find(GlimpseCharTable_t* table, uint8_t key)
{
	GlimpseCharHashNode_t* rc = _glimpse_chartable_find(table, key);
	if(NULL == rc) return NULL;
	return rc->value;
}
#endif
