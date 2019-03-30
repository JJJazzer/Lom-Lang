/*
 * file name: include/symbol.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Fri 22 Feb 2019 08:56:18 PM CST
 */

#ifndef _SYMBOL_H
#define _SYMBOL_H

#include "util.h"
#include "table.h"

typedef struct S_symbol_ *S_symbol;
typedef Tab_table S_table;

extern S_symbol S_Symbol(string name);
extern string   S_Name  (S_symbol sym);
extern S_table  S_Empty (void);
extern void     S_Insert(S_table t, S_symbol sym, void *value);
extern void    *S_LookUp(S_table t, S_symbol sym);
extern void     S_BeginScope(S_table t);
extern void     S_EndScope(S_table t);

#endif
