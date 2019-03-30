/*
 * file name: include/ast.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Fri 22 Feb 2019 08:53:44 PM CST
 */

#ifndef _AST_H
#define _AST_H

#include "symbol.h"

typedef int                       Ast_pos;
typedef struct Ast_exp_ 	 *Ast_exp;
typedef struct Ast_expList_      *Ast_expList;
typedef struct Ast_dec_ 	 *Ast_dec;
typedef struct Ast_decList_	 *Ast_decList;
typedef struct Ast_param_ 	 *Ast_param;
typedef struct Ast_paramList_ 	 *Ast_paramList;
typedef struct Ast_var_ 	 *Ast_var;
typedef struct Ast_field_ 	 *Ast_field;
typedef struct Ast_fieldList_ 	 *Ast_fieldList;
typedef enum {Ast_plus, Ast_minus, Ast_times, Ast_div, Ast_mod} Ast_oper;
typedef enum {Ast_eq, Ast_neq, Ast_lt, Ast_le, Ast_gt, Ast_ge} Ast_relop;

struct Ast_exp_ {
	enum {Ast_varExp, Ast_intExp, Ast_strExp, Ast_realExp, Ast_nilExp,
		Ast_callExp, Ast_opExp,  Ast_assignExp,
		Ast_ifExp, Ast_condExp, Ast_whileExp, Ast_forExp, Ast_arrayExp,
		Ast_varDecExp, Ast_structDecExp, Ast_functionDecExp, 
		Ast_typedefExp, Ast_returnExp, Ast_scopeExp,
	} kind;
	Ast_pos pos;
	union {
		Ast_var var;
		int     i;
		string  s;
		float   r;
		/* Nil */
		struct {S_symbol func; Ast_fieldList args;} callExp;
		struct {Ast_exp left; Ast_oper oper; Ast_exp right;} opExp;
		struct {Ast_var var; Ast_exp exp;} assignExp;
		struct {Ast_exp cond; Ast_expList then;
			Ast_exp elif; Ast_exp elsee;} ifExp;
		struct {Ast_exp left, right; Ast_relop relop;} condExp;
		struct {Ast_exp cond; Ast_expList body;} whileExp;
		struct {S_symbol var; Ast_exp low;
			Ast_exp high; Ast_expList body;} forExp;
		struct {S_symbol arr; Ast_exp size; 
			Ast_exp init;} arrayExp;
		Ast_dec varDec;
		Ast_dec structDec;
		Ast_dec functionDec;
		struct {S_symbol name; S_symbol type;} typedefExp;
		Ast_exp returnExp;
		Ast_expList scopeBody;
	} u;
};

extern Ast_exp Ast_VarExp(Ast_pos pos, Ast_var var);
extern Ast_exp Ast_IntExp(Ast_pos pos, int i);
extern Ast_exp Ast_StrExp(Ast_pos pos, string s);
extern Ast_exp Ast_NilExp(Ast_pos pos);
extern Ast_exp Ast_RealExp(Ast_pos pos, float r);
extern Ast_exp Ast_CallExp(Ast_pos pos, S_symbol func, Ast_fieldList args);
extern Ast_exp Ast_OpExp(Ast_pos pos, Ast_exp left, Ast_oper oper, Ast_exp right);
extern Ast_exp Ast_AssignExp(Ast_pos pos, Ast_var var, Ast_exp exp);
extern Ast_exp Ast_IfExp(Ast_pos pos, Ast_exp cond, Ast_expList then, Ast_exp elif, Ast_exp elsee);
extern Ast_exp Ast_WhileExp(Ast_pos pos, Ast_exp cond, Ast_expList body);
extern Ast_exp Ast_ForExp(Ast_pos pos, Ast_exp low, Ast_exp high, Ast_expList body);
extern Ast_exp Ast_ArrayExp(Ast_pos pos, S_symbol arr, Ast_exp size, Ast_exp init);
extern Ast_exp Ast_VarDecExp(Ast_pos pos, Ast_dec varDec);
extern Ast_exp Ast_StructDecExp(Ast_pos pos, Ast_dec structDec);
extern Ast_exp Ast_FunctionDecExp(Ast_pos pos, Ast_dec functionDec);
extern Ast_exp Ast_TypedefExp(Ast_pos pos, S_symbol type, S_symbol name);
extern Ast_exp Ast_ReturnExp(Ast_pos pos, Ast_exp retExp);
extern Ast_exp Ast_CondExp(Ast_pos pos, Ast_exp left, Ast_relop relop, Ast_exp right);
extern Ast_exp Ast_ScopeExp(Ast_pos pos, Ast_expList scopeBody);
/* AST of declare */
struct Ast_dec_ {
	enum {Ast_functionDec, Ast_varDec, Ast_structureDec,
		Ast_tupleDec, Ast_usingDec, Ast_mainDec} kind;
	Ast_pos pos;
	union {
		struct {S_symbol func; Ast_paramList args;
			S_symbol ret;  Ast_expList body;} functionDec;
		struct {S_symbol var; S_symbol type; 
			Ast_exp init; bool escape; bool isBss;}	     varDec;
		struct {S_symbol structt; Ast_paramList init; bool isBss;} structureDec;
		struct {S_symbol tuple; Ast_fieldList init; bool isBss;} tupleDec;
		struct {S_symbol type, name;} usingDec;
		struct {S_symbol main; S_symbol ret; Ast_expList body;} mainDec;
	} u;
};

extern Ast_dec Ast_FunctionDec(Ast_pos pos, S_symbol func, Ast_paramList args, 
			S_symbol ret, Ast_expList body);
extern Ast_dec Ast_VarDec(Ast_pos pos, S_symbol var, S_symbol type, Ast_exp init, bool isBss);
extern Ast_dec Ast_StructDec(Ast_pos pos, S_symbol structt, Ast_paramList init, bool isBss);
extern Ast_dec Ast_TupleDec(Ast_pos pos, S_symbol tuple, Ast_fieldList init, bool isBss);
extern Ast_dec Ast_UsingDec(Ast_pos pos, S_symbol type, S_symbol name);
extern Ast_dec Ast_MainDec(Ast_pos pos, S_symbol main, S_symbol ret, Ast_expList body);

/* AST of declare list */
struct Ast_decList_ {
	Ast_dec head;
	Ast_decList tail;
};

extern Ast_decList Ast_DecList(Ast_dec head, Ast_decList tail);

/* AST of parameters */
struct Ast_paramList_ {
	Ast_param head;
	Ast_paramList tail;
};

extern Ast_paramList Ast_ParamNVoidList(Ast_param head, Ast_paramList next);

/* AST of parameter */
struct Ast_param_ {
	S_symbol name;
	S_symbol type;
	Ast_pos  pos;
	bool     escape;
};

extern Ast_param Ast_Param(Ast_pos pos, S_symbol name, S_symbol type);

/* AST of variable */
struct Ast_var_ {
	enum {Ast_simpleVar, } kind;
	Ast_pos pos;
	union {
		struct {S_symbol name; bool escape;} simpleVar;
	} u;
};

extern Ast_var Ast_SimpleVar(Ast_pos pos, S_symbol name, bool escape);

/* AST of expression list */
struct Ast_expList_ {
	Ast_exp head; 
	Ast_expList tail;
};

extern Ast_expList Ast_ExpList(Ast_exp exp, Ast_expList expList);

/* AST of field list */
struct Ast_fieldList_ {
	Ast_field head;
	Ast_fieldList tail;
};

extern Ast_fieldList Ast_FieldList(Ast_field head, Ast_fieldList tail);

/* AST of field */
struct Ast_field_ {
	Ast_pos pos;
	Ast_exp exp;
};
extern Ast_field Ast_Field(Ast_pos pos, Ast_exp exp);

#endif
