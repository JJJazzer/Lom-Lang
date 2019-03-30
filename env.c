/*
 * file name: env.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Sat 02 Mar 2019 05:01:47 PM CST
 * 
 * Modules function: Environment of symbol table, it used to "semantic" modules.
 */

#include "include/env.h"

Env_enventry Env_VarEnv(Tr_access access, Ty_type type)
{
	Env_enventry te;
	te = checked_malloc(sizeof(*te));
	te->kind = Env_varEnv;
	te->u.varEnv.access = access;
	te->u.varEnv.type = type;
	return te;
}

Env_enventry Env_FuncEnv(Tr_level level, Tmp_label label,
		Ty_typeList argsType, Ty_type ret)
{
	Env_enventry te;
	te = checked_malloc(sizeof(*te));
	te->kind = Env_funcEnv;
	te->u.funcEnv.level = level;
	te->u.funcEnv.label = label;
	te->u.funcEnv.argsType = argsType;
	te->u.funcEnv.ret = ret;
	return te;
}

Env_enventry Env_EscapeEnv(int depth, bool *is_escape)
{
	Env_enventry te;
	te = checked_malloc(sizeof(*te));
	te->kind = Env_escapeEnv;
	te->u.escapeEnv.depth = depth;
	te->u.escapeEnv.is_escape = is_escape;
	return te;
}
S_table Env_BaseTEnv(void)
{
	S_table tenv = S_Empty();
	return tenv;
}

S_table Env_BaseVEnv(void)
{

	S_table venv = S_Empty();
	return venv;
}
