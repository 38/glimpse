/* scanner.h -   
 *
 * Copyright 2013 Hao Hou <ghost89413@gmail.com>
 * 
 * This file is part of Glimpse, a fast, flexible key-value scanner.
 * 
 * Glimpse is free software: you can redistribute it and/or modify it under the terms 
 * of the GNU General Public License as published by the Free Software Foundation, 
 * either version 3 of the License, or (at your option) any later version.
 *
 * Glimpse is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with Glimpse. 
 * If not, see http://www.gnu.org/licenses/.
 *
 */
	
#ifndef __GLIMPSE_SCANNER_H__
#define __GLIMPSE_SCANNER_H__
#include <glimpse/tree.h>
#include <glimpse/thread.h>
#include <glimpse/def.h>
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
	void* before_scan_data;
	void* after_scan_data;
	const char* (*before_scan)(const char* text, void* userdata);			/* called before the log has been parsed */
	int (*after_scan)(void** result, void* userdata);						/* called after the log has been parsed */
#ifdef LAZY_INSTANCE
	void* data_instance;
#endif
} GlimpseScanner_t;  /* scanner is an singleton in the system */
const char* glimpse_scanner_parse(const char* text, GlimpseThreadData_t* thread_data);/* parse a log the upper most function*/
GlimpseParseTree_t* glimpse_scanner_register_tree(const char* name, char sep_f, char sep_v);		/* add a new log to scanner */
GlimpseParseTree_t* glimpse_scanner_find_tree(const char* name);
int glimpse_scanner_set_defualt_tree(const char* name);  /* set the log you want to parse */
GlimpseParseTree_t* glimpse_scanner_get_default_tree();
void glimpse_scanner_set_before_scan_callback(typeof(((GlimpseScanner_t*)NULL)->before_scan) callback, void* userdata);
void glimpse_scanner_set_after_scan_callback(typeof(((GlimpseScanner_t*)NULL)->after_scan) callback, void* userdata);
/* you can use the function defined in typesystem.h and tree.h to add some key to parse 
 * and define its type */

/* functions to init and cleanup */
void glimpse_scanner_init();
void glimpse_scanner_cleanup();

#endif
