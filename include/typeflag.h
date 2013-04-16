#ifndef __TYPEFLAG_H__
#define __TYPEFLAG_H__
#include <future.h>
/* interfaces for process log,vector,map as normal type */

/* vector interface */
void* glimpse_typeflag_vector_alloc(void* userdata);
int glimpse_typeflag_vector_init(void* data, void* userdata);
int glimpse_typeflag_vector_finalize(void* data, void* userdata);
int glimpse_typeflag_vector_free(void* data, void* userdata);
const char* glimpse_typeflag_vector_parse(const char* text, void* result, void* user_data);

void* glimpse_typeflag_sublog_alloc(void* userdata);
int glimpse_typeflag_sublog_init(void* data, void* userdata);
int glimpse_typeflag_sublog_finalize(void* data, void* userdata);
int glimpse_typeflag_sublog_free(void* data, void* userdata);
const char* glimpse_typeflag_sublog_parse(const char* text, void* result, void* user_data);
#endif
