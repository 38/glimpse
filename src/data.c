#include <data.h>
#include <log.h>
#include <vector.h>
#include <typesystem.h>
GlimpseDataModel_t* glimpse_data_model_new()
{
	GlimpseDataModel_t* ret = (GlimpseDataModel_t*)malloc(sizeof(GlimpseDataModel_t));
	if(NULL == ret) return ret;
	ret->members = NULL;
	return ret;
}
void glimpse_data_model_free(GlimpseDataModel_t* model)
{
	if(NULL == model) return;
	GlimpseDataMember_t* member;
	for(member = model->members; member; )
	{
		GlimpseDataMember_t* next = member->next;
		free(member);
		member = next;
	}
}

int glimpse_data_model_insert(GlimpseDataModel_t* model, GlimpseTypeHandler_t* type)
{
	if(NULL == model || NULL == type) return EINVAILDARG;
	GlimpseDataMember_t* member = (GlimpseDataMember_t*)malloc(sizeof(GlimpseDataMember_t));
	if(NULL == member) return EUNKNOWN;
	member->handler = type;
	if(NULL == model->members) member->idx = 0;
	else member->idx = model->members->idx + 1;
	member->next = model->members;
	model->members = member;
	return ESUCCESS;
}

GlimpseDataInstance_t* glimpse_data_instance_new(GlimpseDataModel_t* model)
{
	if(NULL == model) return NULL;
	/* data instance was managed by typesystem */
	GlimpseDataInstance_t* ret = (GlimpseDataInstance_t*)glimpse_typesystem_instance_object_alloc(
					sizeof(GlimpseDataInstance_t) + GLIMPSE_DATA_MODEL_NUMBER_OF_MEMBERS(model)*sizeof(void*)); 
	if(NULL == ret) return NULL;
	GlimpseDataMember_t* p, *q;
	for(p = model->members; p; p = p->next)
	{
		ret->data[p->idx] = glimpse_typesystem_typehandler_new_instance(p->handler);
		if(NULL == ret->data[p->idx]) goto ERR;
	}
	return ret;
ERR:
	for(q = model->members; q && q != p; q = q->next)
		glimpse_typesystem_typehandler_free_instance(ret->data[p->idx]);
	glimpse_typesystem_instance_object_free(ret);
	return NULL;
}
void glimpse_data_instance_free(GlimpseDataInstance_t* instance)
{
	if(NULL == instance) return;
	GlimpseDataModel_t* model = instance->model;
	GlimpseDataMember_t* p;
	int i;
	for(i = 0; 
		i < GLIMPSE_DATA_MODEL_NUMBER_OF_MEMBERS(model); i ++)
		glimpse_typesystem_typehandler_free_instance(instance->data[i]);
	glimpse_typesystem_instance_object_free(instance);
}
