#ifndef __DATA_H__
#define __DATA_H__
#include <future.h>
#include <typesystem.h>
#include <def.h>
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
#define GLIMPSE_DATA_MODEL_NUMBER_OF_MEMBERS(model) (model->members?model->members->idx+1:0)

static inline void* glimpse_data_instance_get(GlimpseDataInstance_t* instance, GlimpseDataOffset_t offset)
{
	if(offset < 0) return NULL;
	if(NULL == instance) return NULL;
	if(GLIMPSE_DATA_MODEL_NUMBER_OF_MEMBERS(instance->model) <= offset) return NULL;
	return instance->data[offset];
}
#endif
