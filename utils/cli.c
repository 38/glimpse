#include <readline/readline.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <init.h>
#include <typesystem.h>
#include <pluginloader.h>
#include <stdarg.h>
#include <typeparser.h>
#include <scanner.h>
#include <unistd.h>
#ifndef DEFAULT_PROMPT 
#	define DEFAULT_PROMPT "Glimpse> "
#endif
#ifndef MAX_LINEBUF
#	define MAX_LINEBUF 4096
#endif
#define STREQ(a,b) (0 == strcmp((a),(b)))
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
	vfprintf(stderr,fmt,vp);
	fputc('\n', stderr);
	va_end(vp);
}
#define IFCMD(name) else if STREQ(argv[0],#name)
void glimpse_cli_help(int argc, char** argv)
{
	if(argc == 0){
		glimpse_cli_error("which command do you want ?");
		glimpse_cli_error("Possible command:"); //TODO
		glimpse_cli_error("define, display, help, import, list, quit, set");
	}
	IFCMD(define){
		glimpse_cli_error("define log kv_sep f_sep field1 type1 field2 type2 .... fieldN typeN name : define a log and alias the log type with name");
		glimpse_cli_error("define type typedesc name : define type and alias it with name");
	}
	IFCMD(help){
		glimpse_cli_error("help <command>: show help for <command>");
	}
	IFCMD(import){
		glimpse_cli_error("import plugin1 plugin2 ... pluginN: import plugin1,plugin2,...,pluginN");
	}
	IFCMD(list){
		glimpse_cli_error("list api: list all APIs glimpse currently support");
		glimpse_cli_error("list alias: list all names that alisa with some types");
		glimpse_cli_error("list log: list all log structures have been defined");
		glimpse_cli_error("list path: list plugin search path");
		glimpse_cli_error("list plugin: list info of plugin that have been loaded");
		glimpse_cli_error("list type: list all known type ");
	}
	IFCMD(display){
		glimpse_cli_error("display log logname: print the structure of the log");
		glimpse_cli_error("display type typename: print the description of typename");
	}
	IFCMD(quit){
		glimpse_cli_error("quit: quit the program");
	}
	IFCMD(set){
		glimpse_cli_error("set path path1 path2 path3 ... pathN : set the glimpse plugin search path to path1 .. pathN");
		glimpse_cli_error("set defualt-log logname: set the scanner defualt log name ");
		glimpse_cli_error("set output-path path: set where to output");
	}
	else{
		glimpse_cli_error("no such content");
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
			glimpse_cli_error("failed to obtain type list");
			return;
		}
		int i;
		puts("List of all known types:");
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
		puts("List of plugin search pathes:");
		for(i = 0; glimpse_pluginloader_path[i]; i ++)
			printf("[%d]:\t%s\n",i,glimpse_pluginloader_path[i]);
		puts("");
	}
	IFCMD(api){
		int i;
		puts("List of available APIs:");
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
	if(argc == 0) glimpse_cli_error("Usage: import <plugin_name>");
	int i;
	for(i = 0; i < argc; i ++)
	{
		if(ESUCCESS != glimpse_pluginloader_load_plugin(argv[i]))
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
		if(NULL != desc) glimpse_typeparser_alias(desc, argv[2]);
		else glimpse_cli_error("failed to alias type");
	}
	IFCMD(log){
		if(argc < 4 || (argc&1))
		{
			glimpse_cli_error("Usage: define log sep_kv sep_f field1 type1 ... fieldN typeN name");
			return;
		}
		GlimpseParseTree_t* tree = glimpse_scanner_register_tree(argv[argc-1], argv[1][0], argv[2][0]);
		if(tree)
		{
			int i;
			for(i = 3; i < argc - 1; i += 2)
			{
				GlimpseTypeDesc_t* desc = glimpse_typeparser_parse_type(argv[i+1]);
				if(NULL == desc)
				{
					glimpse_cli_error("failed to parse type %s", argv[i+1]);
					continue;
				}
				if(ESUCCESS != glimpse_tree_insert(tree, argv[i], desc))
				{
					glimpse_cli_error("failed to insert the field %s into tree", argv[i]);
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
			else glimpse_cli_error("failed to query the type");
		}
		else glimpse_cli_error("failed to parse type desc");
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
			else glimpse_cli_error("failed to query the type");
		}
		else glimpse_cli_error("fialed to parse type desc");
	}
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
		else glimpse_cli_error("undefined command");
		return 0;
}
void glimpse_cli_interactive()
{
	const char* prompt = DEFAULT_PROMPT;
	for(;;)
	{
		char* line = glimpse_cli_readline(prompt);
		if(NULL == line) return;
		int argc;
		char** argv = glimpse_cli_split(line, &argc, NULL, 0, NULL);
		if(glimpse_cli_do(argc,argv) < 0) return;
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
	glimpse_pluginloader_path[1] = NULL;
	if(argc >= 3 && STREQ(argv[1],"-f"))
	{
		glimpse_cli_script(argv[2], argc - 2, argv + 2);
	}
	else glimpse_cli_interactive();  /* start interactive mode */
	glimpse_cleanup();
	return 0;
}
