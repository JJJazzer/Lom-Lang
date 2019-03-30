/*
 * file name: include/semantic.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Tue 26 Feb 2019 09:56:25 PM CST
 */

#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "ast.h"
#include "types.h"
#include "translate.h"

typedef struct Sem_expty_ *Sem_expty;
struct Sem_expty_ {
	Tr_exp exp;
	Ty_type type;
};

extern Sem_expty Sem_Expty(Tr_exp exp, Ty_type type);
extern void Sem_TransProg(Ast_decList decList);
extern Sem_expty Sem_TransDec(S_table tenv, S_table venv, Ast_dec dec);
extern Sem_expty Sem_TransExpList(S_table tenv, S_table venv, Ast_expList expList);
extern Sem_expty Sem_TransExp(S_table tenv, S_table venv, Ast_exp exp);
extern Sem_expty Sem_TransVar(S_table tenv, S_table venv, Ast_var var);

#endif
