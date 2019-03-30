/*
 * file name: types.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Sat 02 Mar 2019 05:11:37 PM CST
 *
 * Modules description: Type of this language.
 */

#include "include/types.h"
Ty_type Ty_Nil(void)
{
	Ty_type tt;
	tt = checked_malloc(sizeof(*tt));
	tt->kind = Ty_nil;
	return tt;
}

Ty_type Ty_Int(void)
{
	Ty_type tt;
	tt = checked_malloc(sizeof(*tt));
	tt->kind = Ty_int;
	return tt;
}

Ty_type Ty_String(void)
{
	Ty_type tt;
	tt = checked_malloc(sizeof(*tt));
	tt->kind = Ty_string;
	return tt;
}

Ty_type Ty_Real(void)
{
	Ty_type tt;
	tt = checked_malloc(sizeof(*tt));
	tt->kind = Ty_real;
	return tt;
}

Ty_type Ty_Void(void)
{
	Ty_type tt;
	tt = checked_malloc(sizeof(*tt));
	tt->kind = Ty_void;
	return tt;
}

Ty_type Ty_Iptr(void)
{
	Ty_type tt;
	tt = checked_malloc(sizeof(*tt));
	tt->kind = Ty_iptr;
	return tt;
}

Ty_type Ty_Tuple(void)
{
	Ty_type tt;
	tt = checked_malloc(sizeof(*tt));
	tt->kind = Ty_tuple;
	return tt;
}

Ty_type Ty_Structure(void)
{
	Ty_type tt;
	tt = checked_malloc(sizeof(*tt));
	tt->kind = Ty_structure;
	return tt;
}
