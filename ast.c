/*
 * file name: ast.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Sun 24 Feb 2019 09:30:27 AM CST
 *
 * Module's function: The declare of abstract syntax tree
 */

#define AST_DEBUG
#ifdef AST_DEBUG
#include "include/ast.h"
#include "include/util.h"
#else
#include "ast.h"
#include "util.h"
#endif

Ast_exp Ast_VarExp(Ast_pos pos, Ast_var var)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_varExp;
	te->pos = pos;
	te->u.var = var;
	return te;
}

Ast_exp Ast_IntExp(Ast_pos pos, int i)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_intExp;
	te->pos = pos;
	te->u.i = i;
	return te;
}

Ast_exp Ast_StrExp(Ast_pos pos, string s)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_strExp;
	te->pos = pos;
	te->u.s = s;
	return te;
}

Ast_exp Ast_NilExp(Ast_pos pos)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_nilExp;
	te->pos = pos;
	return te;
}

Ast_exp Ast_RealExp(Ast_pos pos, float r)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_realExp;
	te->pos = pos;
	te->u.r = r;
	return te;
}
Ast_exp Ast_CallExp(Ast_pos pos, S_symbol func, Ast_fieldList args)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_callExp;
	te->pos = pos;
	te->u.callExp.func = func;
	te->u.callExp.args = args;
	return te;
}

Ast_exp Ast_OpExp(Ast_pos pos, Ast_exp left, Ast_oper oper, Ast_exp right)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_opExp;
	te->pos = pos;
	te->u.opExp.left = left;
	te->u.opExp.oper = oper;
	te->u.opExp.right = right;
	return te;
}

Ast_exp Ast_AssignExp(Ast_pos pos, Ast_var var, Ast_exp exp)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_assignExp;
	te->pos = pos;
	te->u.assignExp.var = var;
	te->u.assignExp.exp = exp;
	return te;
}
Ast_exp Ast_IfExp(Ast_pos pos, Ast_exp cond, Ast_expList then,
		Ast_exp elif, Ast_exp elsee)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_ifExp;
	te->pos = pos;
	te->u.ifExp.cond = cond;
	te->u.ifExp.then = then;
	te->u.ifExp.elif = elif;
	te->u.ifExp.elsee = elsee;
	return te;
}

Ast_exp Ast_CondExp(Ast_pos pos, Ast_exp left, Ast_relop relop,
		Ast_exp right)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_condExp;
	te->pos = pos;
	te->u.condExp.left = left;
	te->u.condExp.relop = relop;
	te->u.condExp.right = right;
	return te;
}
Ast_exp Ast_WhileExp(Ast_pos pos, Ast_exp cond, Ast_expList body)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_whileExp;
	te->pos = pos;
	te->u.whileExp.cond = cond;
	te->u.whileExp.body = body;
	return te;
}

Ast_exp Ast_ForExp(Ast_pos pos, Ast_exp low, Ast_exp high,
		Ast_expList body)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_forExp;
	te->pos = pos;
	te->u.forExp.low = low;
	te->u.forExp.high = high;
	te->u.forExp.body = body;
	return te;
}

Ast_exp Ast_ArrayExp(Ast_pos pos, S_symbol arr, Ast_exp size, Ast_exp init) 
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_arrayExp;
	te->pos = pos;
	te->u.arrayExp.arr = arr;
	te->u.arrayExp.size = size;
	te->u.arrayExp.init = init;
	return te;
}

Ast_exp Ast_VarDecExp(Ast_pos pos, Ast_dec varDec)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_varDecExp;
	te->pos = pos;
	te->u.varDec = varDec;
	return te;
}

Ast_exp Ast_StructDecExp(Ast_pos pos, Ast_dec structDec)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_structDecExp;
	te->pos = pos;
	te->u.structDec = structDec;
	return te;
}

Ast_exp Ast_FunctionDecExp(Ast_pos pos, Ast_dec funcDec)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_functionDecExp;
	te->pos = pos;
	te->u.functionDec = funcDec;
	return te;
}
Ast_exp Ast_TypedefExp(Ast_pos pos, S_symbol type, S_symbol name)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_typedefExp;
	te->pos = pos;
	te->u.typedefExp.name = type;
	te->u.typedefExp.type = name;
	return te;
}

Ast_exp Ast_ReturnExp(Ast_pos pos, Ast_exp returnExp)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_returnExp;
	te->pos = pos;
	te->u.returnExp = returnExp;
	return te;
}

Ast_exp Ast_ScopeExp(Ast_pos pos, Ast_expList scopeBody)
{
	Ast_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = Ast_scopeExp;
	te->pos = pos;
	te->u.scopeBody = scopeBody;
	return te;
}
Ast_dec Ast_FunctionDec(Ast_pos pos, S_symbol func, Ast_paramList args,
		S_symbol ret, Ast_expList body)
{
	Ast_dec td;
	td = checked_malloc(sizeof(*td));
	td->kind = Ast_functionDec;
	td->pos = pos;
	td->u.functionDec.func = func;
	td->u.functionDec.args = args;
	td->u.functionDec.ret  = ret;
	td->u.functionDec.body = body;
	return td;
}

Ast_dec Ast_VarDec(Ast_pos pos, S_symbol var, S_symbol type, Ast_exp init, bool isBss)
{
	Ast_dec td;
	td = checked_malloc(sizeof(*td));
	td->kind = Ast_varDec;
	td->pos = pos;
	td->u.varDec.var = var;
	td->u.varDec.type = type;
	td->u.varDec.init = init;
	td->u.varDec.escape = TRUE;
	td->u.varDec.isBss = isBss;
	return td;
}

Ast_dec Ast_StructDec(Ast_pos pos, S_symbol structt, Ast_paramList init, bool isBss)
{
	Ast_dec td;
	td = checked_malloc(sizeof(*td));
	td->kind = Ast_structureDec;
	td->pos = pos;
	td->u.structureDec.structt = structt;
	td->u.structureDec.init = init;
	td->u.structureDec.isBss = isBss;
	return td;
}

Ast_dec Ast_TupleDec(Ast_pos pos, S_symbol tuple, Ast_fieldList init, bool isBss)
{
	Ast_dec td;
	td = checked_malloc(sizeof(*td));
	td->kind = Ast_tupleDec;
	td->pos = pos;
	td->u.tupleDec.tuple = tuple;
	td->u.tupleDec.init = init;
	td->u.tupleDec.isBss = isBss;
	return td;
}

Ast_dec Ast_UsingDec(Ast_pos pos, S_symbol type, S_symbol name)
{
	Ast_dec td;
	td = checked_malloc(sizeof(*td));
	td->kind = Ast_usingDec;
	td->pos = pos;
	td->u.usingDec.type = type;
	td->u.usingDec.name = name;
	return td;
}
Ast_dec Ast_MainDec(Ast_pos pos, S_symbol main, S_symbol ret, Ast_expList body)
{
	Ast_dec td;
	td = checked_malloc(sizeof(*td));
	td->kind = Ast_mainDec;
	td->pos = pos;
	td->u.mainDec.main = main;
	td->u.mainDec.ret = ret;
	td->u.mainDec.body = body;
	return td;
}

Ast_decList Ast_DecList(Ast_dec head, Ast_decList tail)
{
	Ast_decList tdl;
	tdl = checked_malloc(sizeof(*tdl));
	tdl->head = head;
	tdl->tail = tail;
	return tdl;
}
Ast_param Ast_Param(Ast_pos pos, S_symbol name, S_symbol type)
{
	Ast_param tp;
	tp = checked_malloc(sizeof(*tp));
	tp->name = name;
	tp->pos = pos;
	tp->type = type;
	tp->escape = TRUE;
	return tp;
}

Ast_paramList Ast_ParamNVoidList(Ast_param head,
		Ast_paramList tail)
{
	Ast_paramList tpl;
	tpl = checked_malloc(sizeof(*tpl));
	tpl->head = head;
	tpl->tail = tail;
	return tpl;
}

Ast_expList Ast_ExpList(Ast_exp head, Ast_expList tail)
{
	Ast_expList tel;
	tel = checked_malloc(sizeof(*tel));
	tel->head = head;
	tel->tail = tail;
	return tel;
}

Ast_field Ast_Field(Ast_pos pos, Ast_exp exp)
{
	Ast_field tf;
	tf = checked_malloc(sizeof(*tf));
	tf->exp = exp;
	tf->pos = pos;
	return tf;
}

Ast_fieldList Ast_FieldList(Ast_field head, Ast_fieldList tail)
{
	Ast_fieldList tfl;
	tfl = checked_malloc(sizeof(*tfl));
	tfl->head = head;
	tfl->tail = tail;
	return tfl;
}

Ast_var Ast_SimpleVar(Ast_pos pos, S_symbol name, bool escape)
{
	Ast_var tv;
	tv = checked_malloc(sizeof(*tv));
	tv->kind = Ast_simpleVar;
	tv->pos = pos;
	tv->u.simpleVar.name = name;
	tv->u.simpleVar.escape = escape;
	return tv;
}

