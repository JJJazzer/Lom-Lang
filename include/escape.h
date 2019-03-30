/*
 * file name: include/escape.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Mon 11 Mar 2019 05:53:43 PM CST
 */

#ifndef _ESCAPE_H
#define _ESCAPE_H

#include "ast.h"

/* Search for escape variable, and add it in "escape" scope */
/* Traverse all of abstract syntax tree */

extern void Esc_SearchEscape(Ast_decList decList);


#endif
