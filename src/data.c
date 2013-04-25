#include <string.h>
#include <glimpse/data.h>
#include <glimpse/log.h>
#include <glimpse/vector.h>
#include <glimpse/typesystem.h>
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
	if(NULL == model || NULL == type) return GLIMPSE_EINVAILDARG;
	GlimpseDataMember_t* member = (GlimpseDataMember_t*)malloc(sizeof(GlimpseDataMember_t));
	if(NULL == member) return GLIMPSE_EUNKNOWN;
	member->handler = type;
	if(NULL == model->members) member->idx = 0;
	else member->idx = model->members->idx + 1;
	member->next = model->members;
	model->members = member;
	return member->idx;
}

GlimpseDataInstance_t* glimpse_data_instance_new(GlimpseDataModel_t* model)
{
	if(NULL == model) return NULL;
	size_t sz = sizeof(GlimpseDataInstance_t) + GLIMPSE_DATA_MODEL_NUMBER_OF_MEMBERS(model)*sizeof(void*);
			  //sizeof(GlimpseDataInstance_t) + GLIMPSE_DATA_MODEL_NUMBER_OF_MEMBERS(model)*sizeof(void*)); 
	/* data instance was managed by typesystem */
	GlimpseDataInstance_t* ret = (GlimpseDataInstance_t*)glimpse_typesystem_instance_object_alloc(sz);
#ifdef LAZY_INSTANCE
	memset(ret, 0, sz);
#endif /*LAZY_INSTANCE*/
	if(NULL == ret) return NULL;
	ret->model = model; 
	return ret;
}
void glimpse_data_instance_free(GlimpseDataInstance_t* instance)
{
	/* because all members are freed while its own type handler desposed, 
	 * so we does not free anything here */
	glimpse_typesystem_instance_object_free(instance);
}
int glimpse_data_instance_init(GlimpseDataInstance_t* instance)
{
	if(NULL == instance) return GLIMPSE_EINVAILDARG;
	GlimpseDataModel_t* model = instance->model;
	if(NULL == model) return GLIMPSE_EINVAILDARG;
	
	GlimpseDataMember_t* p, *q;
	for(p = model->members; p; p = p->next)
	{
#ifdef LAZY_INSTANCE
		if(instance->data[p->idx])
		{
			if(GLIMPSE_ESUCCESS != glimpse_typesystem_typehandler_init_instance(instance->data[p->idx]))
				goto ERR;
		}
		else
			instance->data[p->idx] = glimpse_typesystem_typehandler_new_instance(p->handler);
#else /*LAZY_INSTANCE*/
		instance->data[p->idx] = glimpse_typesystem_typehandler_new_instance(p->handler);
#endif /*LAZY_INSTANCE*/
		if(NULL == instance->data[p->idx]) goto ERR;
	}
	return GLIMPSE_ESUCCESS;
ERR:
	for(q = model->members; q && q != p; q = q->next)
#ifdef LAZY_INSTANCE
		glimpse_typesystem_typehandler_free_instance(instance->data[p->idx]);
#else /*LAZY_INSTANCE*/
		glimpse_typesystem_typehandler_free_instance(instance->data[p->idx]);
#endif /*LAZY_INSTANCE*/
	return GLIMPSE_EUNKNOWN;
}
void glimpse_data_instance_finalize(GlimpseDataInstance_t* instance)
{
	if(NULL == instance) return;
	GlimpseDataModel_t* model = instance->model;
	GlimpseDataMember_t *p;
	for(p = model->members; p; p = p->next)
#ifdef LAZY_INSTANCE
		glimpse_typesystem_typehandler_fianlize_instance(instance->data[p->idx]);
#else /*LAZY_INSTANCE*/
		glimpse_typesystem_typehandler_free_instance(instance->data[p->idx]);
#endif /*LAZY_INSTANCE*/
}
