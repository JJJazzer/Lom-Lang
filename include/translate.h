/*
 * file name: include/translate.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Wed 06 Mar 2019 06:35:25 PM CST
 */

/* Deal with nesting scope */
#ifndef _TRANSLATE_H
#define _TRANSLATE_H

#include "tmplabel.h"
#include "util.h"
#include "frame.h"
#include "ast.h"
#include "semantic.h"

/* *_ex: expression
 * *_nx: none result
 * *_cx: condition statement
 */

typedef struct Tr_exp_ *Tr_exp;
typedef struct Tr_level_ *Tr_level;
/* Manage to local variable, and nesting function */
typedef struct Tr_access_ *Tr_access;
typedef struct Tr_accessList_ *Tr_accessList;

extern Tr_accessList Tr_AccessList(Tr_access head, Tr_accessList tail);

extern Tr_level Tr_Level();
extern int Tr_GetLevel(Tr_level level);
extern Tr_level Tr_OuterMost(void);
extern Tr_level Tr_NewLevel(Tr_level parent, Tmp_label name, 
		U_boolList args);
extern Tr_accessList Tr_Args(Tr_level level);
extern Tr_access Tr_AllocLocal(Tr_level level, bool escape);

extern Tr_exp Tr_Int(int i);
extern Tr_exp Tr_String(string str);
extern Tr_exp Tr_Real(float r);
extern Tr_exp Tr_SimpleVar(Tr_access access, Tr_level level);
extern Tr_exp Tr_GlobalVarDec(Tr_access access, Tr_level level, Tr_exp exps);
extern Tr_exp Tr_LocalVarDec(Tr_access acc, Tr_level lev);
extern Tr_exp Tr_UsingDec(S_symbol type, S_symbol name);
extern Tr_exp Tr_OpExp(Ast_oper op, struct Sem_expty_ *left, struct Sem_expty_ *right);

extern void Tr_ProcEntryExit(Tr_level level, Tr_exp body, Tr_accessList formals);
extern F_fragList Tr_GetResult(void);
#endif
