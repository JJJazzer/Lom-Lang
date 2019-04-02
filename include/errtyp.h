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
#include "terms_color.h"

#define ERROR()				RED(LIGHT) "error:"
#define WARNING() 			PURPLE(LIGHT) "warning:"

#define _UNDEF_ERR(name) 		"\u2716 " ERROR() RESET "Undefined symbol <%s>, you should declare it.", name
#define _DEFINED_ERR(name) 		"\u2716 " ERROR() RESET "Context have defined symbol <%s>.", name 
#define _RETVAL_ISVOID_ERR() 		"\u2716 " ERROR() RESET "Void type not require a return expression."
#define _RETVAL_NOMATCH_ERR(name, ret) 	"\u2716 " ERROR() RESET "Function <%s> return value require a <%s>", name, ret 
#define _NOFOUND_TYPE_ERR(name) 	"\u2716 " ERROR() RESET "Not found type <%s> defined.", name
#define _EXPRVAL_NOMATCH_ERR(t) 	"\u2716 " ERROR() RESET "The value may be expect an "#t" type"
#define _EXPRRET_NOMATCH_ERR(t) 	"\u2716 " ERROR() RESET "The return value of expression require a "#t""
#define _MULTI_MAIN_ERR()		"\u2716 " ERROR() RESET "Multidefine's symbol <start>."

#define _NO_FOUND_MAIN_WARN()		"\u2715 " WARNING() WHITE(LIGHT) "Not found symbol <start>." RESET 
#define _NORETURN_WARN(name) 		"\u2715 " WARNING() RESET "Function <%s> have no return value.", name 

#endif
