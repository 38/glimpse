#ifndef __TYPEFLAG_H__
#define __TYPEFLAG_H__
void* glimpse_typeflag_vector_alloc(void* userdata);
void glimpse_typeflag_vector_init(void* data, void* userdata);
void glimpse_typeflag_vector_free(void* data, void* userdata);
const char* glimpse_typeflag_vector_parse(const char* text, void* result, void* user_data);

void* glimpse_typeflag_sublog_alloc(void* userdata);
void glimpse_typeflag_sublog_init(void* data, void* userdata);
void glimpse_typeflag_sublog_free(void* data, void* userdata);
const char* glimpse_typeflag_sublog_parse(const char* text, void* result, void* user_data);

void* glimpse_typeflag_map_alloc(void* userdata);
void glimpse_typeflag_map_init(void* data, void* userdata);
void glimpse_typeflag_map_free(void* data, void* userdata);
const char* glimpse_typeflag_map_parse(const char* text, void* result, void* user_data);
#endif
