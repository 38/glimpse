#ifndef __SCANNER_H__
#define __SCANNER_H__
#include <tree.h>
#ifndef GLIMPSE_SCANNER_MAX_LOG_NUM
#	define GLIMPSE_SCANNER_MAX_LOG_NUM 1024 /* the max number user can register log */
#endif
typedef struct _glimpse_scanner_t{
	uint16_t count;
	GlimpseParseTree_t* log[GLIMPSE_SCANNER_MAX_LOG_NUM];   /* all log the parser use */
	const char* name[GLIMPSE_SCANNER_MAX_LOG_NUM];          /* the name of tree */
	/* future feature: you can add more handler here, and use an condition to 
	 * determin which handler should be used */
	GlimpseTypeHandler_t* default_handler;					/* default_handler->parse is the upper most level parser */
} GlimpseScanner_t;  /* scanner is an singleton in the system */
const char* glimpse_scanner_parse(const char* text);    /* parse a log the upper most function, it will create the threaddata */
GlimpseParseTree_t* glimpse_scanner_register_tree(const char* name, char sep_f, char sep_v);		/* add a new log to scanner */
GlimpseParseTree_t* glimpse_scanner_find_tree(const char* name);
int glimpse_scanner_set_defualt_tree(const char* name);  /* set the log you want to parse */
/* you can use the function defined in typesystem.h and tree.h to add some key to parse 
 * and define its type */

/* functions to init and cleanup */
void glimpse_scanner_init();
void glimpse_scanner_cleanup();

#endif
