/*
 * file name: frame_x86.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Wed 06 Mar 2019 10:14:28 PM CST
 * 
 * Module's function: The frame of x86. 
 */

#ifndef ARCH_x86
#error Your machine is not x86 architecture
#endif 

#include "include/frame.h"
#include "include/tmplabel.h"
#include "include/IR-tree.h"
#include <stdio.h>

const int wordSize = 2;
typedef struct F_formals_ *F_formals;
static F_access F_InFrame(int offset);
static F_access F_InReg(Tmp_temp reg);

struct F_access_ {
	enum {F_inFrame, F_inReg} kind;
	union {
		int offset;
		Tmp_temp reg;
	} u;
};

struct F_ViewShift {
	string entry;
	string arguments;
};

struct F_frame_ {
	F_accessList accList;
	struct F_ViewShift view_shift;
	int frame_size;		/* allocate frame size */
	Tmp_label name;		/* Machine label */
};                                


F_frame F_NewFrame(Tmp_label name, U_boolList formals)
{
	F_frame f;
	int cnt = 0;
	f = checked_malloc(sizeof(*f));
#ifndef _OS_WINDOWS
	f->view_shift.entry = "pushl \%ebp\nmovl \%esp, \%ebp";
#else
	f->view_shift.entry = "push ebp\nmov ebp, esp";
#endif
	f->name = name;
	while (formals->tail != NULL) {
		f->frame_size += 4;
		if (formals->head == TRUE)
			f->accList->head = F_InFrame(8 + cnt * 4);
		else
			f->accList->head = F_InReg(Tmp_NewTemp());
		formals = formals->tail;
	}
	return f;
}

Tmp_label F_Name(F_frame f)
{
	return f->name;
}

F_access F_AllocLocal(F_frame f, bool escape)
{
	static int nVar = 0;
	/* Unescape variable allocate in register,
	 * otherwise allocate in frame 
	 */
	f->frame_size += 4;
	f->name = Tmp_NewLabel();
	f->view_shift.arguments = "subl $4, \%esp";
	if (!escape)
		return F_InReg(Tmp_NewTemp());
	else
		return F_InFrame(FRAME_BASE + wordSize * nVar++);
}

IR_exp F_Exp(F_access acc, IR_exp framePtr)
{
	if (acc->kind == F_inFrame)
		return IR_Mem(IR_Binop(IR_plus, framePtr, IR_Const(acc->u.offset)), 0);
	else
		return NULL;
}

#ifndef ARCH_x86 
Tmp_temp F_Fp(void)
{
	Tmp_temp t = Tmp_NewTemp();
	return t;
}
#else 
Tmp_temp F_Bp(void)
{
	Tmp_temp t = Tmp_NewTemp();
	return t;
}
#endif 

/* Offset of frame pointer */

F_access F_InFrame(int offset)
{
	F_access f;
	f = checked_malloc(sizeof(*f));
	f->kind = F_inFrame;
	f->u.offset = offset;	
	return f;
}

F_access F_InReg(Tmp_temp reg)
{
	F_access f;
	f = checked_malloc(sizeof(*f));
	f->kind = F_inReg;
	f->u.reg = reg;
	return f;
}
