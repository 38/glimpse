#ifndef __GLIMPSE_BUILTINTYPE_H__
#define __GLIMPSE_BUILTINTYPE_H__
#include <glimpse/future.h>
#include <glimpse/typesystem.h>
#include <glimpse/def.h>
/* interfaces for process log,vector,map as normal type */

/* vector interface */
void* glimpse_builtintype_vector_alloc(void* userdata);
int glimpse_builtintype_vector_init(void* data, void* userdata);
int glimpse_builtintype_vector_finalize(void* data, void* userdata);
int glimpse_builtintype_vector_free(void* data, void* userdata);
const char* glimpse_builtintype_vector_parse(const char* text, void* result, void* user_data, void* thread_data);
char* glimpse_builtintype_vector_tostring(GlimpseTypeHandler_t* type, char* buffer, size_t size);

/*sub log interface */
void* glimpse_builtintype_sublog_alloc(void* userdata);
int glimpse_builtintype_sublog_init(void* data, void* userdata);
int glimpse_builtintype_sublog_finalize(void* data, void* userdata);
int glimpse_builtintype_sublog_free(void* data, void* userdata);
const char* glimpse_builtintype_sublog_parse(const char* text, void* result, void* user_data, void* thread_data);
char* glimpse_builtintype_sublog_tostring(GlimpseTypeHandler_t* type, char* buffer, size_t size);
#endif
