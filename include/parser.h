#ifndef __PARSER_H__
#define __PARSER_H__
#include <tree.h>
#include <data.h>

/* parse a log */
const char* glimpse_parser_parse(GlimpseParseTree_t* tree, const char* text, GlimpseDataInstance_t* storage);

#endif
