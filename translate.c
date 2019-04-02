/*
 * file name: translate.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Thu 07 Mar 2019 10:39:09 AM CST
 *
 * Modules description: Translate the fragment of abstract syntax tree to IR tree.
 */

#include "include/translate.h"
#include "include/IR-tree.h"
#include "include/tmplabel.h"
#include "include/util.h"
#include "include/frame.h"
#include "include/ast.h"
#include "include/semantic.h"
#include <stdio.h>
#include <assert.h>

typedef int Level;
typedef struct Tr_patchTab_ *Tr_patchTab;
Level _global_lev = 0;

struct Tr_access_ {
	Tr_level level;
	F_access access;
};

struct Tr_level_ {
	Level level;
	Tr_level next;
	F_frame frm;
};

struct Cond {
	Tr_patchTab trues;
	Tr_patchTab falses;
	IR_stm stm;
};

struct Tr_exp_ {
	enum {
		Tr_expr,	/* expression */ 
		Tr_nonRet,	/* none result*/
		Tr_cond,	/* cond stmt  */
	} kind;
	union {
		IR_exp expr;
		IR_stm nonRet;
		struct Cond cond;
	} u;
};

static Tr_exp Tr_Expr(IR_exp exp);
static Tr_exp Tr_NonRet(IR_stm nret);
static Tr_exp Tr_Cond(Tr_patchTab trues, Tr_patchTab falses,
		IR_stm stm);
static IR_exp UnEx(Tr_exp e);
static IR_stm UnNonRet(Tr_exp e);
static struct Cond UnCond(Tr_exp e);

Tr_exp Tr_Expr(IR_exp exp)
{
	/* Normal expression */
	Tr_exp t;
	t = checked_malloc(sizeof(*t));
	t->kind = Tr_expr; 
	t->u.expr = exp;
	return t;
}	

Tr_exp Tr_NonRet(IR_stm nret)
{
	/* none result expression */
	Tr_exp t;
	t = checked_malloc(sizeof(*t));
	t->kind = Tr_nonRet;
	t->u.nonRet = nret;
	return t;
}

Tr_exp Tr_Cond(Tr_patchTab trues, Tr_patchTab falses,
		IR_stm stm)
{
	/* Condition expression */
	Tr_exp t;
	t = checked_malloc(sizeof(*t));
	t->kind = Tr_cond;
	t->u.cond.trues = trues;
	t->u.cond.falses = falses;
	t->u.cond.stm = stm;
	return t;
}

struct Tr_patchTab_ {
	Tmp_label *head;
	Tr_patchTab tail;
};

Tr_patchTab Tr_PatchTab(Tmp_label *head, Tr_patchTab tail)
{
	Tr_patchTab t;
	t = checked_malloc(sizeof(*t));
	t->head = head;
	t->tail = tail;
	return t;
}
static void DoPatch(Tr_patchTab pList, Tmp_label label)
{
	while (pList != NULL) {
		*(pList->head) = label;
		pList = pList->tail;
	}
}

static Tr_patchTab JoinPatch(Tr_patchTab first, Tr_patchTab second)
{
	if (!first) return second;
	for (; first->tail; first = first->tail)
		first->tail = second;
	return first;
}

/* take down the construct function */
IR_exp UnEx(Tr_exp e)
{
	switch (e->kind) {
	case Tr_expr:
		return e->u.expr;
	case Tr_cond: {
		Tmp_temp  tv = Tmp_NewTemp();
		Tmp_label t = Tmp_NewLabel(), f = Tmp_NewLabel();
		DoPatch(e->u.cond.trues, t);
		DoPatch(e->u.cond.falses, f);
		return IR_Eseq(IR_Move(IR_Const(1), IR_Temp(tv)),
			IR_Eseq(e->u.cond.stm,
			IR_Eseq(IR_Label(f),
			IR_Eseq(IR_Move(IR_Const(0), IR_Temp(tv)),
			IR_Eseq(IR_Label(t),
				IR_Temp(tv))))));
	}
	case Tr_nonRet:		
		return IR_Eseq(e->u.nonRet, IR_Const(0));
	default:
		break;
	}

}

IR_stm UnNx(Tr_exp e)
{
}

static struct Cond Cx_init(struct Cond *cx, Tr_patchTab falses, IR_stm stm, Tr_patchTab trues)
{
	cx->falses = falses;
	cx->stm    = stm;
	cx->trues  = trues;
	return *cx;
}
struct Cond UnCx(Tr_exp e)
{
	struct Cond cx;
	Cx_init(&cx, NULL, NULL, NULL);
	switch (e->kind) {
	case Tr_cond:
		return e->u.cond;
	case Tr_expr: {
		switch (e->u.expr->kind) {
		case IR_const: {
			/* 1 -> cond .L1 exp .L2 exp */

			Tmp_label t = Tmp_NewLabel();
			Tmp_label f = Tmp_NewLabel();
			DoPatch(cx.trues, t);
			DoPatch(cx.falses, f);
			cx.stm = IR_Cjump(IR_Const(e->u.expr->u.constant),
					IR_neq, IR_Const(0), t, f);			
			return cx;	
		}
		default:
			return cx;
		}
	
	}
	break;
	case Tr_nonRet:
	default:
		/* Fall through */
		fprintf(stderr, "Condition expression have no return value\n");
		return cx;
	}
}

Tr_level Tr_Level(Level level, Tr_level next)
{
	Tr_level t;
	t = checked_malloc(sizeof(*t));
	t->level = level;
	t->next = next;
	t->frm = NULL;
	return t;
}

Tr_level Tr_NewLevel(Tr_level parent, Tmp_label name, U_boolList formals)
{
	/* Create a new frame and nest level */
	Tr_level t = Tr_Level(_global_lev++, parent);
	F_NewFrame(name, formals);
#if 0
	assert(0);
#endif	

	return t;
}

Tr_access Tr_AllocLocal(Tr_level lev, bool esc)
{
	Tr_access ta;
	F_access fa = F_AllocLocal(lev->frm, esc);
	ta = checked_malloc(sizeof(*ta));
	ta->access = fa;
	ta->level = lev;
	return ta;
}

Tr_exp Tr_SimpleVar(Tr_access acc, Tr_level lev)
{
	/* The variable is store in frame */ 
	return Tr_Expr(F_Exp(acc->access, 
#ifndef ARCH_x86
	IR_Temp(F_Fp())
#else 
	IR_Temp(F_Bp())
#endif 
	));
}

Tr_exp Tr_AssignExp()
{

}

Tr_exp Tr_VarDec(Tr_access acc, Tr_level lev, Ast_exp init)
{
	/*
	return Tr_NonRet(IR_Move(IR_Binop(IR_Temp(F_Sp()), IR_minus, IR_Const(F_wordSize)),
				IR_Binop(init->u.opExp.left, init->u.opExp.oper, init->u.opExp.right); 
	*/

}

Tr_exp Tr_GlobalVarDec(Tr_access accesss, Tr_level level, Tr_exp exps)
{
	/* */
	return exps;
}

Tr_exp Tr_LocalVarDec(Tr_access acc, Tr_level lev)
{
	return Tr_SimpleVar(acc, lev);
}

Tr_exp Tr_UsingDec(S_symbol type, S_symbol replace)
{
	return Tr_NonRet(IR_Seq(IR_Label(Tmp_CustomizeNameLabel(S_Name(type))),
				IR_Label(Tmp_CustomizeNameLabel(S_Name(replace)))));
}

Tr_exp Tr_Int(int i)
{
	return Tr_Expr(IR_Const(i)); 
}

Tr_exp Tr_String(string str)
{
	return NULL;
}

Tr_exp Tr_Real(float f)
{
	Tr_exp nil = Tr_Expr(NULL);
	return nil;	
}


Tr_exp Tr_OpExp(Ast_oper op, Sem_expty left, Sem_expty right)
{
	switch (op) {
	case Ast_plus:
		return Tr_Expr(IR_Binop(IR_plus, UnEx(left->exp), UnEx(right->exp)));
	case Ast_minus:
		return Tr_Expr(IR_Binop(IR_minus, UnEx(left->exp), UnEx(right->exp)));
	case Ast_times:
		return Tr_Expr(IR_Binop(IR_times, UnEx(left->exp), UnEx(right->exp)));
	case Ast_div:
		return Tr_Expr(IR_Binop(IR_div, UnEx(left->exp), UnEx(right->exp)));
	case Ast_mod:
		return Tr_Expr(IR_Binop(IR_div, UnEx(left->exp), UnEx(right->exp)));
	default:
		return NULL;
	}
}
