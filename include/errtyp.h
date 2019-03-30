/*
 * file name: include/errtyp.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Wed 20 Mar 2019 07:39:14 PM CST
 */

#ifndef _ERRTYP_H
#define _ERRTYP_H

/* Occured error type of source code */

#include "util.h"

#define _UNDEF_ERR(name) 		"Undefined symbol <%s>, you should declare it.", name
#define _DEFINED_ERR(name) 		"Context have defined symbol <%s>.", name 
#define _NO_FOUND_MAIN_ERR() 		"Not found symbol <main>."
#define _NORETURN_ERR(name) 		"Function <%s> have no return value.", name 
#define _RETVAL_NOMATCH_ERR(name, ret) 	"Function <%s> return value require a <%s>", name, ret 
#define _EXPRVAL_NOMATCH_ERR(t) 	"The value may be expect an "#t" type"
#define _EXPRRET_NOMATCH_ERR(t) 	"The return value of expression require a "#t""

#define _MULTI_MAIN_ERR()		"Multidefine's symbol <start>."


#endif
