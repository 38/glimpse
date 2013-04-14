#include <data.h>
#include <log.h>
#include <vector.h>
GlimpseDataModel_t* glimpse_data_model_new()
{
	GlimpseDataModel_t* ret = (GlimpseDataModel_t*)malloc(sizeof(GlimpseDataModel_t));
	if(NULL == ret) return ret;
	ret->members = NULL;
	ret->used = NULL;
	ret->unused = NULL;
	return ret;
}
/* despose a data member with a instance */
static void _glimpse_data_instance_free_member(void* data, GlimpseTypeHandler_t* handler)
{
	if(NULL == data || NULL == handler) return;
	if(handler->type->flags & GLIMPSE_TYPEFLAG_VECTOR)
	{
		int i;
		GlimpseVector_t* vec = (GlimpseVector_t*)data;
		/* despose the element of vector first */
		for(i = 0; i < vec->object_count; i ++)
		{
			void* element = glimpse_vector_get(vec, i);
			_glimpse_data_instance_free_member(element, handler->vector_element_handler[0]);
		}
		glimpse_vector_free(vec);  /* handler->free must be vector free */
		return;
	}
	else if(handler->type->flags & GLIMPSE_TYPEFLAG_SUBLOG)
	{
		//glimpse_data_instance_free((GlimpseDataInstance_t*)data);
		if(handler->free) handler->free(data, handler->free_data);
		return;
	}
	else if(handler->type->flags & GLIMPSE_TYPEFLAG_MAP) 
	{
		if(handler->free) handler->free(data, handler->free_data);
		return; /* map is the reference to others */
	}
	if(handler->free) handler->free(data, handler->free_data);
}
/* despose data list */
static void _glimpse_data_instance_free_list(GlimpseDataInstance_t* list)
{
	if(NULL == list) return;
	GlimpseDataInstance_t* inst;
	GlimpseDataMember_t* member;
	/* for each instance */
	for(inst = list; inst; inst = inst->next)
	{
		/* despose instance */
		for(member = inst->model->members; member; member = member->next)
		{
			void* member_data = inst->data[member->idx];
			_glimpse_data_instance_free_member(member_data, member->handler);
		}
	}
}
void glimpse_data_model_free(GlimpseDataModel_t* model)
{
	if(NULL == model) return;
	if(NULL != model->used) _glimpse_data_instance_free_list(model->used);
	if(NULL != model->unused) _glimpse_data_instance_free_list(model->unused);
	GlimpseDataMember_t* member;
	for(member = model->members; member; )
	{
		GlimpseDataMember_t* next = member->next;
		glimpse_typesystem_typehandler_free(member->handler);
		free(member);
	}
}

int glimpse_data_model_insert(GlimpseDataModel_t* model, GlimpseTypeHandler_t* type);

GlimpseDataInstance_t* glimpse_data_instance_new(GlimpseDataModel_t* model);
void glimpse_data_instance_free(GlimpseDataModel_t* model, GlimpseDataInstance_t* instance);
