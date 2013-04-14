#include <data.h>
#include <log.h>
#include <vector.h>
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
	//TODO	
}
void glimpse_data_instance_free(GlimpseDataModel_t* model, GlimpseDataInstance_t* instance)
{
	//TODO
}
