/* data.h - data instance manipulation 
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
	
#ifndef __GLIMPSE_DATA_H__
#define __GLIMPSE_DATA_H__
#include <glimpse/future.h>
#include <glimpse/typesystem.h>
#include <glimpse/def.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct _glimpse_data_instance_t GlimpseDataInstance_t;
typedef struct _glimpse_type_handler GlimpseTypeHandler_t;
/* member of each data instance */
typedef struct _glimpse_data_member_t{
	int idx;
	GlimpseTypeHandler_t* handler;
	struct _glimpse_data_member_t* next;
} GlimpseDataMember_t;
/* each parse tree has it's own data model */
typedef struct _glimpse_data_model_t{
	GlimpseDataMember_t* members;
} GlimpseDataModel_t;
/* data instance of data model */
typedef struct _glimpse_data_instance_t{
	GlimpseDataModel_t* model;
	void* data[0];  /* data address table*/
	/* DO NOT add anything here */
	/* memory for data table */
} GlimpseDataInstance_t;

typedef int GlimpseDataOffset_t;   /* indicates the offset of data within a data instance */

GlimpseDataModel_t* glimpse_data_model_new();
void glimpse_data_model_free(GlimpseDataModel_t* model);

int glimpse_data_model_insert(GlimpseDataModel_t* model, GlimpseTypeHandler_t* type);

GlimpseDataInstance_t* glimpse_data_instance_new(GlimpseDataModel_t* model);
void glimpse_data_instance_free(GlimpseDataInstance_t* instance);
int glimpse_data_instance_init(GlimpseDataInstance_t* instance);
void glimpse_data_instance_finalize(GlimpseDataInstance_t* instance);
#ifdef LAZY_INSTANCE
void glimpse_data_instance_cleanup(GlimpseDataInstance_t* instance);
#endif
#define GLIMPSE_DATA_MODEL_NUMBER_OF_MEMBERS(model) (model->members?model->members->idx+1:0)

static inline void* glimpse_data_instance_get(GlimpseDataInstance_t* instance, GlimpseDataOffset_t offset)
{
	if(offset < 0) return NULL;
	if(NULL == instance) return NULL;
	if(GLIMPSE_DATA_MODEL_NUMBER_OF_MEMBERS(instance->model) <= offset) return NULL;
	return instance->data[offset];
}

#ifdef __cplusplus
}
#endif

#endif
