/*
 * file name: include/env.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Tue 26 Feb 2019 09:46:53 PM CST
 */

#ifndef _ENV_H
#define _ENV_H

#include "symbol.h"
#include "types.h"
#include "translate.h"
typedef struct Env_enventry_ *Env_enventry;

struct Env_enventry_ {
	enum {Env_varEnv, Env_funcEnv, Env_escapeEnv,} kind;
	union {
		struct {Tr_access access; Ty_type type;} varEnv;
		struct {Tr_level level; Tmp_label label;
			Ty_typeList argsType; Ty_type ret;} funcEnv;
		struct {int depth; bool *is_escape;} escapeEnv;
	} u;
};
extern Env_enventry Env_VarEnv(Tr_access access, Ty_type type);
extern Env_enventry Env_FuncEnv(Tr_level level, Tmp_label label,
		Ty_typeList argsType, Ty_type ret);
extern Env_enventry Env_EscapeEnv(int depth, bool *is_escape);
/* Predefined function: such as sum(), floor(), ceiling(), etc.. */
extern S_table Env_BaseTEnv(void);	/* environment of Type */	
extern S_table Env_BaseVEnv(void); 	/* Env of Value */

#endif
