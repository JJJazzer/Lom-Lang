%{
#include "y.tab.h"
#include "ast.h"
#include <string.h>

extern int Em_tokPos;
extern int lineno;
int charPos = 1;
int perline = 0;
#define adjust() (Em_tokPos = charPos, charPos += yyleng, perline += yyleng)

extern void Em_NewLine();
int yywrap(void)
{
	return 1;
}
%}

ws 	[ \t]
digit 	[0-9]
int 	{digit}+
string  [a-zA-Z_]
ext_string [a-zA-Z_ ]
%%

{ws}+ 	   	{Em_tokPos = charPos, charPos += yyleng ; continue;}
\n		{
			adjust();
			Em_NewLine();
			if (perline == 1) {
				perline = 0;
				continue;
			}
			perline = 0;
			return NEWLINE;
		}
var 		{adjust(); return T_VAR;}
int 		{adjust(); yylval.ty = String(yytext); return T_INT;}
real 		{adjust(); yylval.ty = String(yytext); return T_REAL;}
string 		{adjust(); yylval.ty = String(yytext); return T_STRING;}
void 		{adjust(); yylval.ty = String(yytext); return T_VOID;}
tuple 		{adjust(); yylval.ty = String(yytext); return T_TUPLE;}
struct		{adjust(); yylval.ty = String(yytext); return T_STRUCT;}
iptr		{adjust(); return T_IPTR;}
def  		{adjust(); return DEF; }
using	 	{adjust(); return USING;}
start 		{adjust(); yylval.id = String(yytext); return MAIN;}
if		{adjust(); return C_IF;}
else 		{adjust(); return C_ELSE;}
elif 		{adjust(); return C_ELIF;}
while 		{adjust(); return C_WHILE;}
for 		{adjust(); return C_FOR;}
to 		{adjust(); return C_TO;}
-include 	{adjust(); return INCLUDE;}
call 		{adjust(); return CALL;}
return 		{adjust(); return RETURN;}
begin 		{adjust(); return BBEGIN;}
nigeb 		{adjust(); return END;}
bss		{adjust(); return BSS;}
"{"		{adjust(); return LBRACE;}
"}"		{adjust(); return RBRACE;}
"("		{adjust(); return LPAREN;}
")"		{adjust(); return RPAREN;}
"["		{adjust(); return LBRACKET;}
"]"		{adjust(); return RBRACKET;}
"="		{adjust(); return FASSIGN;}
":="		{adjust(); return VASSIGN;}
"=="		{adjust(); return EQ;}
"!="		{adjust(); return NEQ;}
">"		{adjust(); return GT;}
"<"		{adjust(); return LT;}
">="		{adjust(); return GE;}
"<="		{adjust(); return LE;}
"+"		{adjust(); return PLUS;}
"-"		{adjust(); return MINUS;}
"*"		{adjust(); return TIMES;}
"/"		{adjust(); return DIV;}
"%"		{adjust(); return MOD;}
";"		{adjust(); return SEMI;}
":"		{adjust(); return COLON;}
","		{adjust(); return COMMA;}
\"		{adjust(); return DQUOTA;}

plus 		{adjust(); return PLUS;}
minus 		{adjust(); return MINUS;}
times 		{adjust(); return TIMES;}
div 		{adjust(); return DIV;}
mod 		{adjust(); return MOD;}
eq 		{adjust(); return EQ;}
neq 		{adjust(); return NEQ;}
gt 		{adjust(); return GT;}
lt 		{adjust(); return LT;}
ge 		{adjust(); return GE;}
le 		{adjust(); return LE;}

"&&"		{adjust(); return AND;}
"||"		{adjust(); return OR;}
"!"		{adjust(); return NOT;}

[a][n][d] 	{adjust(); return AND;}
[o][r] 		{adjust(); return OR;}
[n][o][t] 	{adjust(); return NOT;}

AND 		{adjust(); return AND;}
OR 		{adjust(); return OR;}
NOT 		{adjust(); return NOT;}

nil 		{adjust(); return NIL;}
{digit}+   	{adjust(); yylval.num = atoi(yytext); return INT;}
{int}*\.{int}[e|E]?-?{int}* 		{adjust(); yylval.real = atof(yytext); return REAL;}
\"{ext_string}+\"				{adjust(); yylval.str = String(yytext); return STRING;}
{string}+{string}*{digit}*		{adjust(); yylval.id = String(yytext); return ID;}
.		{adjust(); continue;}
