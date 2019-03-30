/*
 * file name: semantic.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Tue 26 Feb 2019 10:58:01 PM CST
 *
 * Module's function: Semantic analyse.
 */

#include "include/semantic.h"
#include "include/ast.h"
#include "include/symbol.h"
#include "include/errmsg.h"
#include "include/env.h"
#include "include/terms_color.h"
#include "include/util.h"
#include "include/IR-tree.h"
#include "include/translate.h"
#include "include/errtyp.h"
#include "include/escape.h"
#include "include/codegen.h"
#include "include/util.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

extern S_table tenv, venv;
static char *basic_type[4] = {"int", "string", "real", "void"};
static Ty_type actual_ty(Ty_type type);
static int streq(string left, string right);
static int isReturn = FALSE;
static Ty_typeList Create_ParamList(S_table tenv, Ast_paramList plist, Ast_dec dec);
static bool Sem_IsBasicType(S_symbol type);
static S_symbol SearchTenvForUsing(S_table tenv, S_symbol tmp, Ast_dec dec);
extern bool AnyError;
extern FILE *out;

Tr_level parent;
Tr_access acc;
Sem_expty Sem_Expty(Tr_exp exp, Ty_type type)
{
	Sem_expty tet;
	tet = checked_malloc(sizeof(*tet));
	tet->exp = exp;
	tet->type = type;
	return tet;
}

void Sem_TransProg(Ast_decList decList)
{
	Ast_decList p = decList;
	/* Create two initialize symbol tables */
	while (p != NULL) {
		Sem_TransDec(tenv, venv, p->head);
		p = p->tail;
	}

	/* Finding "start" symbol 
	 * if not found, that will report errors
	 */
	Env_enventry x = S_LookUp(venv, S_Symbol("start"));
	if (x == NULL) {
		fprintf(stderr, WHITE(LIGHT) _NO_FOUND_MAIN_ERR() RESET);
		putchar('\n');
	}
}

Sem_expty Sem_TransDec(S_table tenv, S_table venv, Ast_dec dec)
{
	switch (dec->kind) {
	case Ast_functionDec: {
		Tr_level next;
		next = Tr_NewLevel(parent, dec->u.functionDec.func, U_BoolList(TRUE, NULL));	
		string ret_type = S_Name(dec->u.functionDec.ret);
		if (dec->u.functionDec.ret != NULL) {
			Env_enventry x = S_LookUp(venv, dec->u.functionDec.func);
			if (x && x->kind == Env_funcEnv) {
				Em_Error(dec->pos, RED(LIGHT) "error:" RESET _DEFINED_ERR(
						S_Name(dec->u.functionDec.func)));
			} else {
				/* Process with parameters list 
				 * TO DO:
				 */
				Ty_typeList paramTyList = NULL;
#if 0
				assert(0);
#endif 
				if (dec->u.functionDec.args != NULL)
					paramTyList = Create_ParamList(tenv, dec->u.functionDec.args, dec);
#if 0
				while (paramTyList->tail != NULL) {
					printf("%s ", S_Name(paramTyList->head->u.name.sym));
					paramTyList = paramTyList->tail;
				}
				printf("\n");
#endif 
				/* Process with return value of function */
				string func_name = S_Name(dec->u.functionDec.func);
				if (streq(ret_type, "int"))
					S_Insert(venv, dec->u.functionDec.func,
						Env_FuncEnv(next, Tmp_CustomizeNameLabel(func_name),
							paramTyList, Ty_Int()));
				else if (streq(ret_type, "string"))
					S_Insert(venv, dec->u.functionDec.func,
						Env_FuncEnv(next, Tmp_CustomizeNameLabel(func_name)
							, paramTyList, Ty_String()));
				else if (streq(ret_type, "real"))
					S_Insert(venv, dec->u.functionDec.func,
						Env_FuncEnv(next, Tmp_CustomizeNameLabel(func_name)
							, paramTyList, Ty_Real()));
				else if (streq(ret_type, "void")) 
					S_Insert(venv, dec->u.functionDec.func,
						Env_FuncEnv(next, Tmp_CustomizeNameLabel(func_name)
							, paramTyList, Ty_Void()));
			}
		} else {
			/* No attach return value, automatic check type */
			/* I don't know how to do */
		}
		Ast_expList el = dec->u.functionDec.body;
#if 0	
		assert(0);
#endif 
		
		S_BeginScope(tenv);
		S_BeginScope(venv);

		Sem_expty e = Sem_TransExpList(tenv, venv, el);
		/* Check whether have return expression */
		/* TO DO: */
		if (!isReturn) {
			if (!streq(ret_type, "void"))
				Em_Error(dec->pos, PURPLE(LIGHT) "warning:" RESET
						_NORETURN_ERR(S_Name(dec->u.functionDec.func)));
		} else {
			/* check type of return value */ 
			if (streq(ret_type, "void"))
				Em_Error(dec->pos, PURPLE(LIGHT) "warning:"
						RESET "Void type not require a return expression."); 
			else {
				Env_enventry env = S_LookUp(venv, dec->u.functionDec.func);
				if (env->u.funcEnv.ret->kind != e->type->kind)
					Em_Error(dec->pos, PURPLE(LIGHT) "warning:" 
							RESET _RETVAL_NOMATCH_ERR( 
							S_Name(dec->u.functionDec.func), S_Name(dec->u.functionDec.ret)));
			}
		}
		/* Scope ending */
		S_EndScope(venv);
		S_EndScope(tenv);
		isReturn = FALSE;
		break;
	}
	case Ast_varDec: {
		Sem_expty e = Sem_TransExp(tenv, venv, 
			dec->u.varDec.init);

		if (S_LookUp(venv, dec->u.varDec.var) != NULL) {
			Em_Error(dec->pos, RED(LIGHT) "error:" RESET _DEFINED_ERR(
				S_Name(dec->u.varDec.var)));
			break;
		}
		
		if (dec->u.varDec.type == NULL) {
			/* Variables have no constraint type:
			 * 	var id = exps 
			 */
			S_Insert(venv, dec->u.varDec.var,
				Env_VarEnv(NULL, e->type));
					
		} else {
			/* Variables have constraint type:
			 * 	var id : type-id = exps 
			 */
			
			S_symbol tmp = dec->u.varDec.type;
			S_symbol env = tmp;
			if (!Sem_IsBasicType(tmp)) {
				env = S_LookUp(tenv, tmp);
				while (env != NULL && !Sem_IsBasicType(env)) {
					env = S_LookUp(tenv, env);
				}
				if (env == NULL) {
					Em_Error(dec->pos, RED(LIGHT) "error:" RESET " Not found type <%s> defined.",
						S_Name(tmp));
					break;
				}
			}
			if (streq(S_Name(env), "int")) {
				if (e->type->kind != Ty_int)
					Em_Error(dec->u.varDec.init->pos,
						RED(LIGHT) "error:" RESET 
						_EXPRRET_NOMATCH_ERR(int));
				S_Insert(venv, dec->u.varDec.var,
					Env_VarEnv(NULL, Ty_Int()));
				if (parent == NULL)
					return Sem_Expty(Tr_GlobalVarDec(dec->u.varDec.var,
							dec->u.varDec.type, dec->u.varDec.init), Ty_Int());
				return Sem_Expty(Tr_LocalVarDec(), Ty_Int());

			} else if (streq(S_Name(env), "string")) {
				if (e->type->kind != Ty_string)
					Em_Error(dec->u.varDec.init->pos,
						RED(LIGHT) "error:" RESET 
						_EXPRRET_NOMATCH_ERR(string));
				S_Insert(venv, dec->u.varDec.var,
					Env_VarEnv(NULL, Ty_String()));
				/*
				Cgen_Emit(out, "%s:\n\t.asciz\t%s", S_Name(dec->u.varDec.var),
						dec->u.varDec.init->u.s);
				*/
				if (parent == NULL)
					return Sem_Expty(Tr_GlobalVarDec(dec->u.varDec.var,
							dec->u.varDec.type, dec->u.varDec.init), Ty_String());
				return Sem_Expty(Tr_LocalVarDec(), Ty_Int());
			} else if (streq(S_Name(env), "real")) {
				if (e->type->kind != Ty_real) 
					Em_Error(dec->u.varDec.init->pos,
						RED(LIGHT) "error:" RESET 
						_EXPRRET_NOMATCH_ERR(real));
				S_Insert(venv, dec->u.varDec.var,
					Env_VarEnv(NULL, Ty_Real()));
				if (parent == NULL)
					return Sem_Expty(Tr_GlobalVarDec(dec->u.varDec.var,
							dec->u.varDec.type, dec->u.varDec.init), Ty_Real());
				return Sem_Expty(Tr_LocalVarDec(), Ty_Real());
			
			}
		}
		break;
	}
	case Ast_usingDec: {
		S_symbol type = dec->u.usingDec.type;
		S_symbol name = dec->u.usingDec.name;
		/* Such as: typedef int size_t 
		 * Insert into tenv
		 */
		S_Insert(tenv, name, type);
		return Sem_Expty(Tr_UsingDec(type, name), NULL);
	}
	case Ast_mainDec: {
		if (S_LookUp(venv, dec->u.mainDec.main) != NULL) {
			Em_Error(dec->pos , RED(LIGHT) "error:" RESET _MULTI_MAIN_ERR());
		} else {
			S_Insert(venv, dec->u.mainDec.main,
				Env_FuncEnv(NULL, NULL, NULL, Ty_Int()));
		#if 0
			if (dec->u.mainDec.body == NULL)
				printf("Body is null\n");
		#endif 
			/* Manage to scope
			 * TO DO:
			 */
			S_BeginScope(venv);
			S_BeginScope(tenv);
			Sem_TransExpList(tenv, venv, dec->u.mainDec.body);
			if (!isReturn)
				Em_Error(dec->pos, PURPLE(LIGHT) "warning:" RESET
						_NORETURN_ERR(S_Name(dec->u.mainDec.main)));
			/* EndScope: pop local varibale in function scope */
			S_EndScope(tenv);
			S_EndScope(venv);
		}
		isReturn = FALSE;
		break;
	}
	case Ast_structureDec: {
		/* Semantic analysis for structure 
		 * TO DO: 
		 */
		S_symbol tstruct = dec->u.structureDec.structt;
		if (S_LookUp(venv, tstruct) != NULL) {
			Em_Error(dec->pos, RED(LIGHT) "error:" RESET 
					_UNDEF_ERR(tstruct));
		} else {
			/* Process with param list */

		}
		S_Insert(venv, tstruct, Env_VarEnv(NULL, Ty_Structure()));
		break;
	}
	case Ast_tupleDec: {
		/* tuple t = {Ast_exp(), Ast_exp(), ...} */
		S_symbol ttuple = dec->u.tupleDec.tuple;
		if (S_LookUp(venv, ttuple) != NULL) 
			Em_Error(dec->pos, RED(LIGHT) "error:" RESET
					_UNDEF_ERR(ttuple));
		S_Insert(venv, ttuple,
				Env_VarEnv(NULL, Ty_Tuple()));
		break;		   
	}
	}
	return Sem_Expty(NULL, NULL);
}

Sem_expty Sem_TransExpList(S_table tenv, S_table venv, Ast_expList expList)
{
	Sem_expty e;
	Ast_expList p = expList;
	while (p != NULL) {
		e = Sem_TransExp(tenv, venv, p->head);
		p = p->tail;
	}
	return e;
}

Sem_expty Sem_TransExp(S_table tenv, S_table venv, Ast_exp exp)
{
	if (exp == NULL)
		return NULL;

	switch (exp->kind) {
	case Ast_intExp:
		/* IR_Const(i) Tr_Expr( */
		/* Future: return Sem_Expty(Tr_Expr(IR_Const(exp->u.i)), Ty_Int()); */
		return Sem_Expty(NULL, Ty_Int());
	case Ast_strExp:
		return Sem_Expty(NULL, Ty_String());
	case Ast_realExp:
		return Sem_Expty(NULL, Ty_Real());
	case Ast_nilExp:
		return Sem_Expty(NULL, Ty_Nil());
	case Ast_callExp: {
		/* Process with function(function(Nonvoid ret type) is diffirent to procedure(Void ret type) */
		Env_enventry x = S_LookUp(venv, exp->u.callExp.func);
		if (x == NULL) {
			Em_Error(exp->pos, RED(LIGHT) "error:" RESET _UNDEF_ERR(S_Name(exp->u.callExp.func))); 
		} else if (x && x->kind == Env_funcEnv) {
			if (x->u.funcEnv.ret == Ty_Void()) {
				Em_Error(exp->pos, RED(LIGHT) "error:" RESET "Symbol <%s> is a procedure, have no return value");
				return Sem_Expty(NULL, Ty_Void());
			} else 
				return Sem_Expty(NULL, actual_ty(x->u.funcEnv.ret));
		}
		return Sem_Expty(NULL, Ty_Int());
	}
	case Ast_opExp: {
		/* Operator expression */
		Ast_oper op = exp->u.opExp.oper;
		Sem_expty left = Sem_TransExp(tenv, venv, exp->u.opExp.left);
		Sem_expty right = Sem_TransExp(tenv, venv, exp->u.opExp.right);
		if (op == Ast_plus) {
			switch (left->type->kind) {
			case Ty_int:
				if (right->type->kind != Ty_int)
					Em_Error(exp->u.opExp.right->pos,
						RED(LIGHT) "error:" RESET
						_EXPRVAL_NOMATCH_ERR(int));
				return Sem_Expty(NULL, Ty_Int());
			case Ty_string:
				if (right->type->kind != Ty_string)
					Em_Error(exp->u.opExp.right->pos,
						RED(LIGHT) "error:" RESET
						_EXPRVAL_NOMATCH_ERR(string));
				return Sem_Expty(NULL, Ty_String());
			case Ty_real:
				if (right->type->kind != Ty_real)
					Em_Error(exp->u.opExp.right->pos,
						RED(LIGHT) "error:" RESET 
						_EXPRVAL_NOMATCH_ERR(real));
				return Sem_Expty(NULL, Ty_Real());
			default:
				Em_Error(exp->u.opExp.left->pos, RED(LIGHT) "error:" RESET " The value of type can't require do arith.");
				return Sem_Expty(NULL, Ty_Int());
			}
		} else if (op == Ast_minus || op == Ast_times || op == Ast_div) {
			switch (left->type->kind) {
			case Ty_int:
				if (right->type->kind != Ty_int)
					Em_Error(exp->u.opExp.right->pos,
						RED(LIGHT) "error:" RESET 
						_EXPRVAL_NOMATCH_ERR(int));
				return Sem_Expty(NULL, Ty_Int());
			case Ty_string:
				Em_Error(exp->u.opExp.right->pos,
						RED(LIGHT) "error:" RESET " The string can't do arithmetic, except operator of plus.");
				return Sem_Expty(NULL, Ty_String());
			case Ty_real:
				if (op == Ast_div)
					Em_Error(exp->u.opExp.right->pos,
						RED(LIGHT) "error:" RESET " The real number can't divided.");
				if (right->type->kind != Ty_real)
					Em_Error(exp->u.opExp.right->pos,
						RED(LIGHT) "error:" RESET " The value may be expect a real.");
				return Sem_Expty(NULL, Ty_Real());
			default:
				Em_Error(exp->u.opExp.left->pos, RED(LIGHT) "error:" RESET " The value of type can't require do arith");
				return Sem_Expty(NULL, Ty_Int());
			}
		}
		break;
	}
	case Ast_assignExp: {
		/* Assign expression */	
		Env_enventry x = S_LookUp(venv, exp->u.assignExp.var->u.simpleVar.name);
		if (x != NULL) {
			Sem_expty e = Sem_TransExp(tenv, venv, exp->u.assignExp.exp);
			
			if (e->type->kind != x->u.varEnv.type->kind) {
				if (x->u.varEnv.type->kind == Ty_int) 
					Em_Error(exp->u.assignExp.var->pos, RED(LIGHT) "error:" RESET 
							_EXPRVAL_NOMATCH_ERR(int));
				else if (x->u.varEnv.type->kind == Ty_string)
					Em_Error(exp->u.assignExp.var->pos, RED(LIGHT) "error:" RESET 
							_EXPRVAL_NOMATCH_ERR(string));
				else if (x->u.varEnv.type->kind == Ty_real)
					Em_Error(exp->u.assignExp.var->pos, RED(LIGHT) "error:" RESET 
							_EXPRVAL_NOMATCH_ERR(real));
			}
			return Sem_Expty(NULL, actual_ty(x->u.varEnv.type)); 
		} else { 
			Em_Error(exp->u.assignExp.var->pos, RED(LIGHT) "error:" RESET _UNDEF_ERR(
						S_Name(exp->u.assignExp.var->u.simpleVar.name)));
			return Sem_Expty(NULL, Ty_Int());
		}
		break;
	}
	case Ast_varExp: {
		return Sem_TransVar(tenv, venv, exp->u.var);
	}
	case Ast_varDecExp: {
		/* Process var declare in scope */
		Sem_TransDec(tenv, venv, exp->u.varDec);
		break;
	}
	case Ast_functionDecExp: {
		Sem_TransDec(tenv, venv, exp->u.functionDec);
		break;
	}
	case Ast_returnExp: {
		isReturn = TRUE;
		return Sem_TransExp(tenv, venv, exp->u.returnExp);
	}
	case Ast_ifExp: {
		Sem_TransExp(tenv, venv, exp->u.ifExp.cond);
		Sem_TransExpList(tenv, venv, exp->u.ifExp.then);
		return Sem_Expty(NULL, NULL);
	}
	case Ast_condExp: {
		Sem_expty left = Sem_TransExp(tenv, venv, exp->u.condExp.left);
		Sem_expty right = Sem_TransExp(tenv, venv, exp->u.condExp.right);
	
	}
	case Ast_whileExp: {
		Sem_TransExp(tenv, venv, exp->u.whileExp.cond);
		Sem_TransExpList(tenv, venv, exp->u.whileExp.body);
	}
	case Ast_forExp: {

	}
	default:
		break;
	}	
	return NULL;
}

Sem_expty Sem_TransVar(S_table tenv, S_table venv, Ast_var var)
{
	/* Assume variable is all of escape */
	switch (var->kind) {
	case Ast_simpleVar: {
		Env_enventry x = S_LookUp(venv, var->u.simpleVar.name);
		if (x && x->kind == Env_varEnv) {
			return Sem_Expty(NULL, actual_ty(x->u.varEnv.type));
		} else {
			Em_Error(var->pos, RED(LIGHT) "error:" RESET _UNDEF_ERR(S_Name(var->u.simpleVar.name)));
			return Sem_Expty(NULL, Ty_Int());
		}
		break;		    
	}
	}
}

static Ty_type actual_ty(Ty_type type)
{
	switch (type->kind) {
	case Ty_int:
		return Ty_Int();
	case Ty_real:
		return Ty_Real();
	case Ty_string:
		return Ty_String();
	case Ty_nil:
		return Ty_Nil();
	case Ty_void:
		return Ty_Void();
	case Ty_iptr:
		return Ty_Iptr();
	case Ty_tuple:
		return Ty_Tuple();
	case Ty_structure:
		return Ty_Structure();
	default:
		return NULL;
	} 
}

static int streq(string left, string right)
{
	if (strcmp(left, right) == 0)
		return 1;
	return 0;
}

static Ty_typeList Create_ParamList(S_table tenv, Ast_paramList plist, Ast_dec dec)
{
	Ty_typeList t, ret;
	t = checked_malloc(sizeof(*t));
	ret = t;
	S_symbol env;
	while (plist != NULL) {
		t->tail = checked_malloc(sizeof(*t));
		env = plist->head->type;
		if (!Sem_IsBasicType(plist->head->type)) 
			env = SearchTenvForUsing(tenv, plist->head->type, dec); 
		if (env == NULL)
			break;
		if (streq(S_Name(env), "int")) 
			t->head = Ty_Int();
		else if (streq(S_Name(env), "string"))
			t->head = Ty_String();
		else if (streq(S_Name(env), "real"))
			t->head = Ty_Real();
		t->head->u.name.sym = plist->head->name;
		t = t->tail;
		plist = plist->tail;
	}
	t->tail = NULL;
	return ret;
}

static bool Sem_IsBasicType(S_symbol type)
{
	char *t = S_Name(type);
	int i;
	for (i = 0; i < (int) ARRLENG(basic_type); i++) {
		if (streq(t, basic_type[i]))
			return 1;
	}
	return 0;
}

S_symbol SearchTenvForUsing(S_table tenv, S_symbol tmp, Ast_dec dec) 
{
	S_symbol env = S_LookUp(tenv, tmp);
	while (env != NULL && !Sem_IsBasicType(env)) 
		env = S_LookUp(tenv, env);
	if (env == NULL) 
		Em_Error(dec->u.functionDec.args->head->pos, RED(LIGHT) "error:" RESET " Not found type <%s> defined.",
							S_Name(tmp));
	return env;
}
