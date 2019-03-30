/*
 * file name: escape.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Mon 11 Mar 2019 05:54:37 PM CST
 *
 * Module's function: Find "escape" variable.
 */

#include "include/ast.h"
#include "include/env.h"
#include <stdio.h>
#include <assert.h>

int  d = 0;
static void Esc_TraverseExp(S_table env, int depth, Ast_exp exp);
static void Esc_TraverseExpList(S_table env, int depth, Ast_expList expList); 
static void Esc_TraverseDec(S_table env, int depth, Ast_dec dec);
static void Esc_TraverseVar(S_table env, int depth, Ast_var var);

S_table tenv, venv;

void Esc_SearchEscape(Ast_decList decList)
{
	tenv = Env_BaseTEnv();
	venv = Env_BaseVEnv();
	Ast_decList p = decList;
	while (p != NULL) {
		Esc_TraverseDec(venv, d, p->head);
		p = p->tail;
	}
}

void Esc_TraverseDec(S_table env, int depth, Ast_dec dec)
{
	switch (dec->kind) {
	/*
		case Ast_functionDec: {
		Esc_TraverseExpList(env, depth, dec->u.functionDec.body);	
	}*/
	break;
	default:
		break;
	}
}

void Esc_TraverseExpList(S_table env, int depth, Ast_expList expList)
{
	Ast_expList p = expList;
	while (p->tail != NULL) {
		Esc_TraverseExp(env, depth, p->head);
		p = p->tail;
		d = 0;
	}
}

void Esc_TraverseExp(S_table env, int depth, Ast_exp exp)
{
	switch (exp->kind) {
	case Ast_varExp:
		if (S_LookUp(env, exp->u.var->u.simpleVar.name)
			&& d > depth) 
			exp->u.var->u.simpleVar.escape = TRUE;
		else
			exp->u.var->u.simpleVar.escape = FALSE;
		Env_EscapeEnv(depth, &(exp->u.var->u.simpleVar.escape));
		break;
	case Ast_scopeExp:
		d++;
		Esc_TraverseExpList(env, depth, exp->u.scopeBody);
		break;
	default:
		return;
	}	
}
