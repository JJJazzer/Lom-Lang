/*
 * file name: include/types.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Tue 26 Feb 2019 09:35:27 PM CST
 */

#ifndef _TYPE_H
#define _TYPE_H

#include "symbol.h"
typedef struct Ty_type_      *Ty_type;
typedef struct Ty_typeList_  *Ty_typeList;
typedef struct Ty_field_     *Ty_field;
typedef struct Ty_fieldList_ *Ty_fieldList;
struct Ty_type_ {
	enum {Ty_nil, Ty_int, Ty_string, Ty_real,
	      Ty_name, Ty_void, Ty_iptr, Ty_tuple,
	      Ty_structure, } kind;
	union {
		struct {S_symbol sym; Ty_type ty;} name;
	} u;
};

extern Ty_type Ty_Nil(void);
extern Ty_type Ty_Int(void);
extern Ty_type Ty_String(void);
extern Ty_type Ty_Real(void);
extern Ty_type Ty_Void(void);
extern Ty_type Ty_Iptr(void);
extern Ty_type Ty_Name(S_symbol sym, Ty_type ty);
extern Ty_type Ty_Tuple(void);
extern Ty_type Ty_Structure(void);

struct Ty_typeList_ {
	Ty_type head;
	Ty_typeList tail;
};
extern Ty_typeList Ty_TypeList(Ty_type head, Ty_typeList tail);

struct Ty_field_ {
	S_symbol name;
	Ty_type ty;
};
extern Ty_field Ty_Field(S_symbol name, Ty_type ty);

struct Ty_fieldList {
	Ty_field head;
	Ty_fieldList tail;
};
extern Ty_fieldList Ty_FieldList(Ty_field head, Ty_fieldList tail);

#endif
