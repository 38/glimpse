/* cli.c - Command Line Interface for Glimpse  
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
	
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>

#include <readline/readline.h>

#include <glimpse/init.h>
#include <glimpse/typesystem.h>
#include <glimpse/pluginloader.h>
#include <glimpse/typeparser.h>
#include <glimpse/scanner.h>
#include <glimpse/strpool.h>
#include <glimpse/version.h>
#ifndef DEFAULT_PROMPT 
#	define DEFAULT_PROMPT "Glimpse> "
#endif
#ifndef MAX_LINEBUF
#	define MAX_LINEBUF 4096
#endif
#define STREQ(a,b) (0 == strcmp((a),(b)))
FILE* fp_input = NULL;
FILE* fp_output = NULL;;
const char* prompt = DEFAULT_PROMPT;
char* glimpse_cli_readline(const char* prompt)
{
	static char * buffer = NULL;
	if(buffer) free(buffer);
	buffer = readline(prompt);
	if(buffer) add_history(buffer);
	return buffer;
}
static inline int glimpse_cli_char_in(char c, const char* chars)
{
	for(;*chars;chars++)
		if(c == *chars) return 1;
	return 0;
}
char** glimpse_cli_split(const char* str, int* argc, FILE* file, int clc, char** clv)
{
	static char  strbuf[MAX_LINEBUF*2];
	static char* buf[MAX_LINEBUF];
	static char  readbuf[MAX_LINEBUF];
	char* p;
	int count = 0;
	for(p = strbuf;*str; str++)
	{
		char prev;
		for(;*str && glimpse_cli_char_in(*str,"\n\t\r ");str++); /*ignore the leading white space*/
		if('#' == *str) break; /*outside of a string*/ 
		if(0 == *str) break;  /* reach the end of the string */
		if(count == 0 || strlen(buf[count-1]) > 0) count ++; 
		buf[count-1] = p;
		if(glimpse_cli_char_in(*str, "\'\""))  /* a string */
		{
			prev = *str;
			int escape = 0;
			for(str++;*str; str++)
			{
IN_STRING:
				if(escape)
				{
					switch(*str)
					{
						case 'n':
							*(p++) = '\n';
							break;
						case 'r':
							*(p++) = '\r';
						case 't':
							*(p++) = '\t';
						default:
							*(p++) = *str;
					}
					escape = 0;
				}
				else
				{
					if(*str == '\\') escape = 1;
					else if(prev == *str) break;
					else if('$' == *str && '{' == str[1] && clc > 0 && NULL != clv)
					{
						int idx = 0;
						int i;
						for(i = 2;str[i] && glimpse_cli_char_in(str[i],"\n\t\r ");i++);
						for(; str[i] && str[i] >= '0' && str[i] <= '9'; i ++)
							idx = idx * 10 + str[i] - '0';
						for(;str[i] && glimpse_cli_char_in(str[i],"\n\t\r ");i++);
						if('}' == str[i]) 
						{
							if(idx < clc)
							{
								int j;
								for(j = 0; clv[idx][j]; j ++)
									*(p++) = clv[idx][j];
							}
							str += i;
						}
						else *(p++) = '$';
					}
					else *(p++) = *str;
				}
			}
			if(0 == *str) /* a EOL in string */
			{
				*(p++) = '\n';
				if(NULL == file)
				{
					str = glimpse_cli_readline("quote> ");
					if(NULL == str) return NULL;
				}
				else 
				{
					if(fgets(readbuf, sizeof(readbuf), file))
						str = readbuf;
					else 
						return NULL;
					int len = strlen(readbuf);
					for(;glimpse_cli_char_in(readbuf[len-1],"\n\r");readbuf[--len] = 0);  /* strip the \n */
				}
				goto IN_STRING;
			}
			*(p++) = 0;
		}
		else 
		{
			for(;*str && !glimpse_cli_char_in(*str,"#\n\t\r "); str ++)
			{
				if(str[0] == '$' && str[1] == '{' && clc > 0 && clv != NULL)
				{
						int idx = 0;
						int i;
						for(i = 2;str[i] && glimpse_cli_char_in(str[i],"\n\t\r ");i++);
						for(; str[i] && str[i] >= '0' && str[i] <= '9'; i ++)
							idx = idx * 10 + str[i] - '0';
						for(;str[i] && glimpse_cli_char_in(str[i],"\n\t\r ");i++);
						if('}' == str[i]) 
						{
							if(idx < clc)
							{
								int j;
								for(j = 0; clv[idx][j]; j ++)
									*(p++) = clv[idx][j];
							}
							str += i;
						}
						else *(p++) = '$';
				}
				else if(str[0] == '\\' && str[1] == 0)
				{
					if(NULL == file)
					{
						str = glimpse_cli_readline("cont> ");
						if(NULL == str) return NULL;
						str --;
					}
					else 
					{
						if(fgets(readbuf, sizeof(readbuf), file))
							str = readbuf - 1;
						else 
							return NULL;
						int len = strlen(readbuf);
						for(;glimpse_cli_char_in(readbuf[len-1],"\n\r");readbuf[--len] = 0);  /* strip the \n */
					}
				}
				else *(p++) = *str;
			}
			*(p++) = 0;
		}
		if(0 == *str) break;
		if('#' == *str) break; /* a comment next to it */
	}
	buf[count] = NULL;
	*argc = count;
	return buf;
}
void glimpse_cli_error(const char* fmt, ...)
{
	va_list vp;
	va_start(vp,fmt);
	fputs("\033[33m",stderr);
	vfprintf(stderr,fmt,vp);
	fputs("\033[0m\n", stderr);
	va_end(vp);
}
#define IFCMD(name) else if STREQ(argv[0],#name)
void glimpse_cli_help(int argc, char** argv)
{
	if(argc == 0){
		glimpse_cli_error("define, display, help, import, list, parse , quit, set");
	}
	IFCMD(define){
		glimpse_cli_error("define log kv_sep f_sep field type [field type ....] name");
		glimpse_cli_error("define type typedesc name");
	}
	IFCMD(help){
		glimpse_cli_error("help [command]");
	}
	IFCMD(import){
		glimpse_cli_error("import plugin [plugin ...]");
	}
	IFCMD(list){
		glimpse_cli_error("list api");
		glimpse_cli_error("list alias");
		glimpse_cli_error("list log");
		glimpse_cli_error("list path");
		glimpse_cli_error("list plugin");
		glimpse_cli_error("list type");
	}
	IFCMD(display){
		glimpse_cli_error("display log logname");
		glimpse_cli_error("display type typename");
	}
	IFCMD(quit){
		glimpse_cli_error("quit");
	}
	IFCMD(set){
		glimpse_cli_error("set path dirname [dirname ...]");
		glimpse_cli_error("set defualt-log logname");
		glimpse_cli_error("set output path");
		glimpse_cli_error("set input path");
		glimpse_cli_error("set output-format how address [how address ...]");
	}
	IFCMD(parse)
	{
	}
	else{
		glimpse_cli_error("No Such Content");
	}
}
extern GlimpseAPIMetaData_t* _glimpse_pluginloader_api_list[MAX_API_VERSION];
extern GlimpsePluginHandler_t* _glimpse_pluginloader_plugin_list[MAX_PLUGINS];
extern int _glimpse_pluginloader_api_count ;
extern int _glimpse_pluginloader_plugin_count;
extern GlimpseTypeAlias_t _glimpse_typeparser_alias_table[GLIMPSE_MAX_TYPE_ALIAS];
extern int _glimpse_typeparser_alias_count;
extern GlimpseScanner_t _glimpse_scanner_instance;
void glimpse_cli_list(int argc, char** argv)
{
	if(argc == 0) glimpse_cli_error("Usage: list [api|type|alias|log|path|plugin]");
	IFCMD(type){
		char** ret = glimpse_typesystem_list_knowntypes();
		if(NULL == ret)
		{
			glimpse_cli_error("Failed to Obtain Type List");
			return;
		}
		int i;
		puts("List of all Known Types:");
		for(i = 0;ret[i]; i ++)
		{
			printf("[%d]:\t%s\n",i,ret[i]);
			free(ret[i]);
		}
		free(ret);
		puts("");
	}
	IFCMD(path){
		int i;
		puts("List of Plugin Search Pathes:");
		for(i = 0; glimpse_pluginloader_path[i]; i ++)
			printf("[%d]:\t%s\n",i,glimpse_pluginloader_path[i]);
		puts("");
	}
	IFCMD(api){
		int i;
		puts("List of Available APIs:");
		for(i = 0; i < _glimpse_pluginloader_api_count; i ++)
			printf("[%d]:\t%s\n",i,_glimpse_pluginloader_api_list[i]->APIVersion);
		puts("");
	}
	IFCMD(plugin){
		int i;
		puts("List of Plugins:");
		for(i = 0; i < _glimpse_pluginloader_plugin_count; i ++)
		{
			printf("[%d]: %s %s ",i,
					_glimpse_pluginloader_plugin_list[i]->MetaData->Name,
					_glimpse_pluginloader_plugin_list[i]->API->APIVersion);
			int j;
			for(j = 0; _glimpse_pluginloader_plugin_list[i]->MetaData->Dependency
					&& _glimpse_pluginloader_plugin_list[i]->MetaData->Dependency[j]; j ++)
			{
				if(j!=0) printf(",");
				printf("%s",_glimpse_pluginloader_plugin_list[i]->MetaData->Dependency[j]);
			}
			puts("");
		}
		puts("");
	}
	IFCMD(alias){
		int i;
		puts("List of Type Alias:");
		for(i = 0; i < _glimpse_typeparser_alias_count; i ++)
		{
			GlimpseTypeHandler_t* handler = glimpse_typesystem_query(_glimpse_typeparser_alias_table[i].type);
			char buffer[1024];
			glimpse_typesystem_typehandler_tostring(handler, buffer, sizeof(buffer));
			printf("[%d]\t%s = %s\n",i,_glimpse_typeparser_alias_table[i].name,buffer);
		}
		puts("");
	}
	IFCMD(log){
		puts("List of Logs:");
		int i;
		for(i = 0; i < _glimpse_scanner_instance.count; i ++)
		{
			char buffer[1024];
			snprintf(buffer, sizeof(buffer), "sublog{name:%s}", _glimpse_scanner_instance.name[i]);
			GlimpseTypeDesc_t* desc = glimpse_typeparser_parse_type(buffer);
			GlimpseTypeHandler_t* handler = glimpse_typesystem_query(desc);
			if(NULL == handler) continue;
			glimpse_typesystem_typehandler_tostring(handler, buffer, sizeof(buffer));
			printf("[%d]\t%s: %s\n",i,_glimpse_scanner_instance.name[i],buffer);
		}
		puts("");
	}
}
void glimpse_cli_import(int argc, char** argv)
{
	if(argc == 0) glimpse_cli_error("Usage: import plugin [plugin ...]");
	int i;
	for(i = 0; i < argc; i ++)
	{
		if(GLIMPSE_ESUCCESS != glimpse_pluginloader_load_plugin(argv[i]))
			glimpse_cli_error("failed to import plugin `%s'", argv[i]);
	}
}
void glimpse_cli_define(int argc, char** argv)
{
	if(argc == 0) glimpse_cli_error("Usage: define [log|type]");
	IFCMD(type){
		if(argc != 3) 
		{
			glimpse_cli_error("Usage: define type typedesc name");
			return;
		}
		GlimpseTypeDesc_t* desc = glimpse_typeparser_parse_type(argv[1]);
		if(NULL != desc) glimpse_typeparser_alias(desc, glimpse_strpool_new(argv[2]));
		else glimpse_cli_error("Failed to Alias Type");
	}
	IFCMD(log){
		if(argc < 4 || (argc&1))
		{
			glimpse_cli_error("Usage: define log sep_kv sep_f key type [key type ...] name");
			return;
		}
		GlimpseParseTree_t* tree = glimpse_scanner_register_tree(
				glimpse_strpool_new(argv[argc-1]), argv[1][0], argv[2][0]);
		if(tree)
		{
			int i;
			for(i = 3; i < argc - 1; i += 2)
			{
				GlimpseTypeDesc_t* desc = glimpse_typeparser_parse_type(argv[i+1]);
				if(NULL == desc)
				{
					glimpse_cli_error("Failed to Parse Type %s", argv[i+1]);
					continue;
				}
				if(GLIMPSE_ESUCCESS != glimpse_tree_insert(tree, argv[i], desc))
				{
					glimpse_cli_error("Failed to Insert the Field %s into Tree", argv[i]);
					if(!desc->registered)glimpse_typesystem_typedesc_free(desc);
				}
			}
		}
	}
}
void glimpse_cli_display(int argc, char** argv)
{
	if(argc != 2) glimpse_cli_error("Usage: display [log|type] typedesc");
	IFCMD(type){
		GlimpseTypeDesc_t* desc = glimpse_typeparser_parse_type(argv[1]);
		if(NULL != desc)
		{
			char buffer[4096];
			GlimpseTypeHandler_t* handler = glimpse_typesystem_query(desc);
			if(NULL != handler) 
			{
				glimpse_typesystem_typehandler_tostring(handler, buffer, sizeof(buffer));
				puts(buffer);
			}
			else glimpse_cli_error("Failed to Query Type");
		}
		else glimpse_cli_error("Failed to Parse Type Description");
	}
	IFCMD(log){
		char buffer[4096];
		snprintf(buffer, sizeof(buffer), "sublog{name:%s}", argv[1]);
		GlimpseTypeDesc_t* desc = glimpse_typeparser_parse_type(buffer);
		if(desc)
		{
			GlimpseTypeHandler_t* handler = glimpse_typesystem_query(desc);
			if(handler)
			{
				glimpse_typesystem_typehandler_tostring(handler, buffer, sizeof(buffer));
				puts(buffer);
			}
			else glimpse_cli_error("Failed to Query Type");
		}
		else glimpse_cli_error("Fialed to Parse Type Description");
	}
}
void glimpse_cli_set(int argc, char** argv)
{
	if(argc == 0) glimpse_cli_error("Usage: set [path|input|output|output-format|default-log]");
	IFCMD(path){
		int i;
		for(i = 1; i < argc; i ++)
			glimpse_pluginloader_path[i-1] = glimpse_strpool_new(argv[i]);
		glimpse_pluginloader_path[argc-1] = NULL;
	}
	else if STREQ(argv[0], "default-log") {
		if(argc == 2)
			glimpse_scanner_set_defualt_tree(argv[1]);
		else
			glimpse_cli_error("Usage: set default-log name");
	}
	IFCMD(input)
	{
		if(argc == 2)
		{
			if STREQ(argv[1],"-") fp_input = stdin;
			else
			{
				FILE* fp = fopen(argv[1],"r");
				if(NULL == fp) return;
				fp_input = fp;
			}
		}
		else
			glimpse_cli_error("Usage: set input filename //- means stdin");
	}
	IFCMD(output)
	{
		if(argc == 2)
		{
			if STREQ(argv[1],"-") fp_output = stdin;
			else
			{
				FILE* fp = fopen(argv[1],"w");
				if(NULL == fp) return;
				fp_output = fp;
			}
		}
		else
			glimpse_cli_error("Usage: set output filename //- means stdout");
	}
}
void glimpse_cli_parse_arg(int argc, char** argv)
{
	if(argc != 1)
	{
		glimpse_cli_error("Usage: parsearg \"text\"");
		return;
	}
	GlimpseThreadData_t* thread_data = glimpse_thread_data_new();
	glimpse_scanner_parse(argv[1], thread_data);
	glimpse_thread_data_free(thread_data);
}
int glimpse_cli_do(int argc, char** argv)
{
		if(argc == 0) return 0;
		IFCMD(quit) return -1;
		IFCMD(help) glimpse_cli_help(argc-1, argv+1);
		IFCMD(list) glimpse_cli_list(argc-1, argv+1);
		IFCMD(import) glimpse_cli_import(argc-1, argv+1);
		IFCMD(define) glimpse_cli_define(argc-1, argv+1);
		IFCMD(echo) {
			for(argc -- , argv ++ ;argc; argc --, argv ++)
			{
				printf("%s", *argv);
				if(argc > 1) putchar(' ');
			}
			puts("");
		}
		IFCMD(display) glimpse_cli_display(argc-1, argv+1);
		IFCMD(set) glimpse_cli_set(argc-1, argv+1);
		IFCMD(parsearg) glimpse_cli_parse_arg(argc-1, argv+1);
		else glimpse_cli_error("Undefined Command");
		return 0;
}
void glimpse_cli_interactive()
{
	glimpse_cli_error("Glimpse CLI Shell (lib%s %s)\nwith %s",glimpse_name, glimpse_version, glimpse_configure);
	glimpse_cli_error("Type `help' for avaiable commands");
	glimpse_cli_error("");
	for(;;)
	{
		char* line = glimpse_cli_readline(prompt);
		if(NULL == line) return;
		int argc;
		char** argv = glimpse_cli_split(line, &argc, NULL, 0, NULL);
#if 0
		clock_t start = clock();
#else
		unsigned long long start = glimpse_profiler_rdtsc();
#endif
		if(glimpse_cli_do(argc,argv) < 0) return;
#if 0
		clock_t end = clock();
		glimpse_cli_error("execute time :%.2f ms",(end-start)*1000.0/CLOCKS_PER_SEC);
#else
		unsigned long long end = glimpse_profiler_rdtsc(); 
		static unsigned long long _interval , _f=1;
		if(_f)
		{
			_f = 0;
			_interval = glimpse_profiler_rdtsc();
			usleep(1000);
			_interval = glimpse_profiler_rdtsc() - _interval;
		}
		glimpse_cli_error("%g ms",(end-start)*1.0/_interval);
#endif
	}
}
void glimpse_cli_script(const char* filename, int argc, char** argv)
{
	FILE* fp = fopen(filename, "r");
	char line_buffer[4096];
	if(NULL == fp) goto ERR;
	while(fgets(line_buffer, sizeof(line_buffer), fp))
	{
		int len = strlen(line_buffer);
		for(;glimpse_cli_char_in(line_buffer[len-1],"\n\r");line_buffer[--len] = 0);  /* strip the \n */
		int c;
		char** v = glimpse_cli_split(line_buffer, &c, fp, argc, argv);
		if(NULL == v) goto ERR;
		if(glimpse_cli_do(c,v) < 0) break;
	}
ERR:
	if(fp) fclose(fp);
}
#ifdef GLIMPSE_DEBUG
void api_init();
#endif
int main(int argc, char** argv)
{
	glimpse_init();
	Glimpse_TypeAPI_init();
#ifdef GLIMPSE_DEBUG
	api_init();
#endif
	glimpse_pluginloader_path[0] = ".";
	glimpse_pluginloader_path[1] = GLIMPSE_CLI_DEFAULT_PATH;
	glimpse_pluginloader_path[2] = NULL;
	fp_input = stdin;
	fp_output = stdout;
	if(argc >= 3 && STREQ(argv[1],"-f"))
	{
		glimpse_cli_script(argv[2], argc - 2, argv + 2);
	}
	else glimpse_cli_interactive();  /* start interactive mode */
	glimpse_cleanup();
	if(fp_input) fclose(fp_input);
	if(fp_output) fclose(fp_output);
	return 0;
}
