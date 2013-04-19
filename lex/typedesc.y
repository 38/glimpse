%{
#include <typeparser.h>
    extern int lex_status;
    extern int line;
%}
%token ID
%token STRING
%token CHAR
%token INT
%token FLOAT

%%
type	: ID
		| ID param
param	: '{' props '}'
props	: prop
	  	| props ',' prop
prop	: ID ':' value
value	: STRING
	    | CHAR
		| INT
		| FLOAT
		| type
%%
extern char* yytext;
void yyerror(char* s)
{
	GLIMPSE_LOG_ERROR("invaild type descriptor: %s\n", s);
}
