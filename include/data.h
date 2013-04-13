#ifndef __DATA_H__
#define __DATA_H__
#include <typesystem.h>
/* each parse tree has it's own data model */
struct _glimpse_data_instance_t;
typedef struct _glimpse_data_instance_t GlimpseDataInstance_t;
typedef struct _glimpse_data_member_t{
	int idx;
	GlimpseTypeHandler_t* handler;
	struct _glimpse_data_member_t* next;
} GlimpseDataMember_t;
typedef struct _glimpse_data_model_t{
	GlimpseDataMember_t* members;
	/* data instance pool */
	GlimpseDataInstance_t* used;
	GlimpseDataInstance_t* unused;
} GlimpseDataModel_t;
/* data instance of data model */
typedef struct _glimpse_data_instance_t{
	GlimpseDataModel_t* model;
	void** data;
	GlimpseDataModel_t* next;
	GlimpseDataModel_t* prev;
} GlimpseDataInstance_t;

GlimpseDataModel_t* glimpse_data_model_new();
void glimpse_data_model_free(GlimpseDataModel_t* model);

int glimpse_data_model_insert(GlimpseDataModel_t* model, GlimpseTypeHandler_t* type);

GlimpseDataInstance_t* glimpse_data_instance_new(GlimpseDataModel_t* model);
void glimpse_data_instance_free(GlimpseDataModel_t* model, GlimpseDataInstance_t* instance);

#endif
