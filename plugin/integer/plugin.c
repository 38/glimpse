#include <TypeAPI.h>
#include <stdio.h>
int resolve_int(const GlimpseTypeDesc_t* type, GlimpseTypeHandler_t* handler)
{
	return 0;
}
GlimpseTypeGroup_t int_type_group = {
	.name = "integer",
	.resolve = resolve_int
};
int init()
{
	RegisterTypeGroup(&int_type_group);
}
GlimpsePluginMetaData(TypeAPI)
	GlimpsePluginName("integer");
	GlimpsePluginVersion(0,0,1);
	GlimpsePluginAPICallBack(TypeAPI, OnInitialized) = init; 
GlimpsePluginEndMetaData
