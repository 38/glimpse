#include <readline/readline.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <init.h>
#include <typesystem.h>
#include <pluginloader.h>
#include <stdarg.h>
#include <typeparser.h>
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
char** glimpse_cli_split(const char* str, int* argc)
{
	static char  strbuf[MAX_LINEBUF*2];
	static char* buf[MAX_LINEBUF];
	char* p;
	int count = 0;
	for(p = strbuf;*str; str++)
	{
		char prev;
		for(;*str && glimpse_cli_char_in(*str,"\n\t\r ");str++); /*ignore the leading white space*/
		if(0 == *str) break;  /* reach the end of the string */
		buf[count++] = p;
		if(glimpse_cli_char_in(*str, "\'\""))  /* a string */
		{
			prev = *str;
			int escape = 0;
			for(str++;*str; str++)
			{
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
					if(prev == *str) break;
					else *(p++) = *str;
				}
			}
			*(p++) = 0;
		}
		else 
		{
			for(;*str && !glimpse_cli_char_in(*str,"\n\t\r "); str ++)
				*(p++) = *str;
			*(p++) = 0;
		}
		if(0 == *str) break;
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
	printf("%d\n",argc);
	if(argc == 0)
	{
		glimpse_cli_error("which command do you want ?");
		glimpse_cli_error("Possible command:"); //TODO
		glimpse_cli_error("help, import, list, quit, set");
	}
}
extern GlimpseAPIMetaData_t* _glimpse_pluginloader_api_list[MAX_API_VERSION];
extern GlimpsePluginHandler_t* _glimpse_pluginloader_plugin_list[MAX_PLUGINS];
extern int _glimpse_pluginloader_api_count ;
extern int _glimpse_pluginloader_plugin_count;
extern GlimpseTypeAlias_t _glimpse_typeparser_alias_table[GLIMPSE_MAX_TYPE_ALIAS];
extern int _glimpse_typeparser_alias_count;
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
		//TODO
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
	if(argc != 3) glimpse_cli_error("Usage: define [log|type] description name");
	IFCMD(type){
		GlimpseTypeDesc_t* desc = glimpse_typeparser_parse_type(argv[1]);
		if(NULL != desc) glimpse_typeparser_alias(desc, argv[2]);
		else glimpse_cli_error("failed to alias type");
	}
	IFCMD(log){
		//TODO
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
		char** argv = glimpse_cli_split(line, &argc);
		if(glimpse_cli_do(argc,argv) < 0) return;
	}
}
void api_init();
int main(int argc, char** argv)
{
	glimpse_init();
	Glimpse_TypeAPI_init();
	api_init();
	glimpse_pluginloader_path[0] = ".";
	glimpse_pluginloader_path[1] = NULL;
	glimpse_cli_interactive();  /* start interactive mode */
	glimpse_cleanup();
	return 0;
}
