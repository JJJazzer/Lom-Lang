%{
#include <stdio.h>
#include "ast.h"
#include "symbol.h"
#include "errmsg.h"

extern int Em_tokPos;
extern int yylex();
Ast_decList root;
void yyerror(const char *msg)
{
	/* Error handling */
	/* Not finished */
	Em_Error(Em_tokPos, "Syntax error");
}


%}

%union {
	int num;
	float real;
	string id;
	string ty;	
	string str;
	
	Ast_pos pos;
	Ast_decList root; 
	Ast_dec dec;
	Ast_expList exp_list;
	Ast_exp exp;
	Ast_paramList param_list;
	Ast_param param;
	Ast_fieldList field_list;
	Ast_field field;
	bool	escape;
	bool    isBss;
}

%token  T_VAR T_INT T_REAL T_STRING T_VOID NEWLINE 
	T_TUPLE T_STRUCT USING UMINUS T_IPTR	
	INCLUDE DEF CALL END_OF_FILE RETURN			/* Import keyword */
	BSS
	C_IF C_ELSE C_ELIF C_WHILE C_FOR C_TO		/* Condition keyword */
	
	LBRACE RBRACE LPAREN RPAREN DQUOTA		/* Useful symbol */
	LBRACKET RBRACKET SEMI COLON COMMA 
	BBEGIN END  
	EQ NEQ GT LT GE LE FASSIGN VASSIGN  		/* Logical keyword */
	PLUS MINUS TIMES DIV MOD 
	AND OR NOT 
%token <num> INT
%token <real> REAL
%token <str> STRING 
%token NIL
%type <root> stmtList 
%type <dec>  prog stmt func_dec var_dec main_dec 
	     structure_dec tuple_dec typedef_dec

%type <exp>  var_exps var_exp 
	     op_exp int_exp str_exp nil_exp real_exp
	     call_exp callArg_exps assign_exp exp
	     if_exp cond_exp else_exp for_exp 
	     elif_exp low high while_exp
%type <exp_list> expList 
%type <param_list> param_list 
%type <param> param 
%type <field_list> field_list 
%type <field> field 
%type <pos>  pos 
%type <ty>   type_attach type_constriant T_INT T_REAL 
	     T_STRING T_VAR T_VOID T_IPTR var_type
%token <id> ID MAIN
%left COMMA
%left EQ NEQ LE LT GE GT
%left PLUS MINUS
%left TIMES DIV

%left UMINUS 
%left DQUOTA 
%left SEMI
/* The definition of abstract syntax tree.
 */

%start prog
%%

prog: stmtList  		{ root = $1; if (root == NULL) printf("NULL");}
stmtList: stmt NEWLINE stmtList { $$ = Ast_DecList($1, $3); }	
stmtList:			{ $$ = NULL;}
stmt: func_dec			{ $$ = $1;}
stmt: var_dec 			{ $$ = $1;}
stmt: main_dec 			{ $$ = $1;}
stmt: structure_dec 		{ $$ = $1;}
stmt: tuple_dec 		{ $$ = $1;}
stmt: typedef_dec 		{ $$ = $1;}
pos :   			{ $$ = Em_tokPos;}

func_dec: pos DEF ID LPAREN param_list RPAREN COLON type_attach FASSIGN LBRACE NEWLINE expList  RBRACE 
	{
		$$ = Ast_FunctionDec($1, S_Symbol($3), $5, S_Symbol($8), $12);
	}
func_dec: pos DEF ID LPAREN param_list RPAREN FASSIGN LBRACE expList RBRACE
	{ 
		$$ = Ast_FunctionDec($1, S_Symbol($3), $5, NULL, $9); 
	}
main_dec: pos DEF MAIN LPAREN RPAREN COLON type_attach FASSIGN LBRACE NEWLINE expList RBRACE
	{
		$$ = Ast_MainDec($1, S_Symbol($3), S_Symbol($7), $11);
	}
structure_dec: pos   T_STRUCT ID FASSIGN LBRACE param_list RBRACE
	{
		$$ = Ast_StructDec($1, S_Symbol($3), $6, TRUE);	
	}
tuple_dec: pos   T_TUPLE ID FASSIGN LPAREN field_list RPAREN
	{
		$$ = Ast_TupleDec($1, S_Symbol($3), $6, TRUE); 
	}
var_dec : pos  T_VAR ID COLON var_type VASSIGN var_exps 
	{
	 	$$ = Ast_VarDec($1, S_Symbol($3), S_Symbol($5), $7, FALSE);
	}
var_dec : pos  T_VAR ID VASSIGN var_exps 
	{ $$ = Ast_VarDec($1, S_Symbol($3), NULL, $5, FALSE);}
var_dec : pos BSS T_VAR ID COLON var_type VASSIGN var_exps 
	{
		$$ = Ast_VarDec($1, S_Symbol($4), S_Symbol($6), $8, TRUE);
	}
var_dec : pos BSS T_VAR ID VASSIGN var_exps
	{
		$$ = Ast_VarDec($1, S_Symbol($4), NULL, $6, TRUE);
	}

typedef_dec: pos USING var_type ID 
	{ $$ = Ast_UsingDec($1, S_Symbol($3), S_Symbol($4));}

param_list:
	{ $$ = NULL;}
param_list: param COMMA param_list  
	{ $$ = Ast_ParamNVoidList($1, $3);}
param_list: param 
	{ $$ = Ast_ParamNVoidList($1, NULL);}

param: pos ID COLON var_type  
	{ 
	  $$ = Ast_Param($1, S_Symbol($2), S_Symbol($4));
	}
var_type: T_INT | T_REAL | T_STRING | ID
	{ $$ = $1;}
var_exps:  op_exp
	|  nil_exp
	|  call_exp 
	{ $$ = $1; }
var_exp : pos ID  
	{ $$ = Ast_VarExp($1, Ast_SimpleVar($1, S_Symbol($2), FALSE)); }
op_exp  : op_exp PLUS  pos op_exp	
	{ $$ = Ast_OpExp($3, $1, Ast_plus, $4); }
op_exp  : op_exp MINUS pos op_exp
	{ $$ = Ast_OpExp($3, $1, Ast_minus, $4);}
op_exp  : op_exp TIMES pos op_exp 
	{ $$ = Ast_OpExp($3, $1, Ast_times, $4);}
op_exp  : op_exp DIV   pos op_exp 
	{ $$ = Ast_OpExp($3, $1, Ast_div,   $4);}
op_exp  : pos MINUS INT 	%prec UMINUS
	{ $$ = Ast_IntExp($1, -$3);}
op_exp  : var_exp 
	{ $$ = $1;}
op_exp  : int_exp 
	{ $$ = $1;}
op_exp  : str_exp
	{ $$ = $1;}
op_exp  : real_exp
	{ $$ = $1;}
op_exp  : LPAREN op_exp RPAREN
	{ $$ = $2;}

int_exp : pos INT
	{ $$ = Ast_IntExp($1, $2); }
str_exp : pos STRING 
	{ $$ = Ast_StrExp($1, $2); }
nil_exp : pos NIL 
	{ $$ = Ast_NilExp($1); }
real_exp: pos REAL
	{ $$ = Ast_RealExp($1, $2);}
call_exp: pos ID LPAREN field_list RPAREN
	{ $$ = Ast_CallExp($1, S_Symbol($2), $4); printf("Call exp: %s\n", $2);}
assign_exp: ID VASSIGN pos var_exps
	{ $$ = Ast_AssignExp($3, Ast_SimpleVar($3, S_Symbol($1), FALSE), $4);}

/* If expression */
if_exp:   pos C_IF LPAREN cond_exp RPAREN LBRACE NEWLINE expList RBRACE elif_exp else_exp
	{ $$ = Ast_IfExp($1, $4, $8, $10, $11); }
else_exp: pos C_ELSE LBRACE NEWLINE expList RBRACE
	{ $$ = Ast_IfExp($1, NULL, $5, NULL, NULL); }
elif_exp: pos C_ELIF LPAREN cond_exp RPAREN LBRACE NEWLINE expList RBRACE elif_exp 
	{ $$ = Ast_IfExp($1, $4, $8, $10, NULL); }
else_exp:
	{/* Nothing to do */ $$ = NULL;}
elif_exp: 
	{/* Nothing to do */ $$ = NULL;}
cond_exp: op_exp
	{ $$ = $1; }
cond_exp: cond_exp EQ pos cond_exp 
	{ $$ = Ast_CondExp($3, $1, Ast_eq, $4);}
cond_exp: cond_exp NEQ pos cond_exp 
	{ $$ = Ast_CondExp($3, $1, Ast_neq, $4);}
cond_exp: cond_exp LT pos cond_exp 
	{ $$ = Ast_CondExp($3, $1, Ast_lt, $4);}
cond_exp: cond_exp LE pos cond_exp 
	{ $$ = Ast_CondExp($3, $1, Ast_le, $4);}
cond_exp: cond_exp GT pos cond_exp 
	{ $$ = Ast_CondExp($3, $1, Ast_gt, $4);}
cond_exp: cond_exp GE pos cond_exp 
	{ $$ = Ast_CondExp($3, $1, Ast_ge, $4);}
/* If expression End */

for_exp: pos C_FOR LPAREN low C_TO high RPAREN LBRACE NEWLINE expList RBRACE
	{ $$ = Ast_ForExp($1, $4, $6, $10);}
low: assign_exp
   | int_exp 
	{ $$ = $1;}
high: int_exp 
    | var_exp 
	{ $$ = $1;}

while_exp: pos C_WHILE LPAREN cond_exp RPAREN LBRACE NEWLINE expList RBRACE
	{ $$ = Ast_WhileExp($1, $4, $8);}

field_list: 
	{ $$ = NULL;}
field_list: field COMMA field_list
	{ $$ = Ast_FieldList($1, $3);}
field_list: field
	{ $$ = Ast_FieldList($1, NULL);}
field: pos callArg_exps 
	{ $$ = Ast_Field($1, $2); } 

callArg_exps: op_exp
	    | nil_exp
	    | call_exp
	{ $$ = $1;}
type_attach: type_constriant 
	{ $$ = $1;}
type_constriant: T_INT | T_REAL | T_STRING | T_VOID  | T_IPTR
	{ $$ = $1;}
expList: exp  NEWLINE expList
        { $$ = Ast_ExpList($1, $3); }
expList:{ $$ = NULL;} 
exp    : pos var_dec
        { $$ = Ast_VarDecExp($1, $2); }
exp    : pos structure_dec 
        { $$ = Ast_StructDecExp($1, $2);}
exp    : pos func_dec
   	{ $$ = Ast_FunctionDecExp($1, $2);}
exp    : assign_exp
        { $$ = $1; }
exp    : CALL call_exp
      	{ $$ = $2; }
exp    : RETURN pos op_exp
        { $$ = Ast_ReturnExp($2, $3);}
exp    : if_exp 
        { $$ = $1;}
exp    : for_exp
        { $$ = $1;}
exp    : while_exp
        { $$ = $1;}
exp    : pos BBEGIN NEWLINE expList END
      	{ $$ = Ast_ScopeExp($1, $4); } 
