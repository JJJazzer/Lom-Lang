/*
 * file name: include/frame.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Wed 06 Mar 2019 06:58:12 PM CST
 */

#ifndef _FRAME_H
#define _FRAME_H

#include "tmplabel.h"
#include "util.h"
#include "IR-tree.h"

#define FRAME_BASE	8
/* The standard stack frame of LOM */
typedef struct F_frame_ *F_frame;
typedef struct F_access_ *F_access;

typedef struct F_accessList_ *F_accessList;
struct F_accessList_ {
	F_access head;
	F_accessList tail;
};


/* Create a new frame */
F_frame F_NewFrame(Tmp_label name, U_boolList formals);

Tmp_label F_Name(F_frame f);
F_accessList F_Formals(F_frame f);
F_access F_AllocLocal(F_frame f, bool escape);

Tmp_temp F_Fp(void);
Tmp_temp F_Sp(void);
Tmp_temp F_Rv(void);
extern const int F_wordSize;
IR_exp F_Exp(F_access access, IR_exp framePtr);

IR_stm F_ProcEntryExit1(F_frame frame, IR_stm stm);

typedef struct F_frag_ *F_frag;
struct F_frag_ {
	enum {F_stringFrag, F_procFrag} kind;
	union {
		struct {Tmp_label label; string str;} stringFrag;
		struct {IR_stm body; F_frame frame;} procFrag;
	} u;
};

F_frag F_StringFrag(Tmp_label label, string str);
F_frag F_ProcFrag(IR_stm body, F_frame frame);

typedef struct F_fragList_ *F_fragList;
struct F_fragList_ {
	F_frag head;
	F_fragList tail;
};

F_fragList F_FragList(F_frag head, F_fragList tail);

#endif 
