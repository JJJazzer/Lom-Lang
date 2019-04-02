/*
 * file name: include/IR-tree.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Tue 26 Feb 2019 09:58:43 PM CST
 */

#ifndef _IR_TREE_H
#define _IR_TREE_H

/* IR tree */
#include "tmplabel.h"

typedef struct IR_stm_ *IR_stm;
typedef struct IR_exp_ *IR_exp;
typedef struct IR_expList_ *IR_expList;
typedef struct IR_stmList_ *IR_stmList;

struct IR_expList_ {
	IR_exp head;
	IR_expList tail;
};

struct IR_stmList_ {
	IR_stm head;
	IR_stmList tail;
};

typedef enum {
	IR_plus, IR_minus, IR_times, IR_div,
	IR_and, IR_or, IR_lshift, IR_rshift,
	IR_xor
} IR_binop;

typedef enum {
	IR_eq, IR_neq, IR_lt, IR_le, IR_gt, IR_ge,
	IR_ult, IR_ule, IR_ugt, IR_uge,
} IR_relop;

struct IR_stm_ {
	enum {IR_seq, IR_label, IR_jump, IR_cjump, IR_move,
		IR_eexp} kind;
	union {
		struct {IR_stm left, right;} seq;
		Tmp_label label;
		struct {IR_exp exp; Tmp_labelList labels;} jump;
		struct {IR_exp left; IR_relop op; IR_exp right;
			Tmp_label trues, falses;} cjump;
		struct {IR_exp src, dst;} move;
		IR_exp exp;
	} u;
};

extern IR_stm IR_Seq(IR_stm left, IR_stm right);
extern IR_stm IR_Label(Tmp_label label);
extern IR_stm IR_Jump(IR_exp exp, Tmp_labelList labels);
extern IR_stm IR_Cjump(IR_exp left, IR_relop op, IR_exp right,
		Tmp_label trues, Tmp_label falses);
extern IR_stm IR_Move(IR_exp src, IR_exp dst);
extern IR_stm IR_Exp(IR_exp exp);

struct IR_exp_ {
	enum {IR_binOp, IR_mem, IR_temp, IR_eseq, IR_name, IR_const,
		IR_call, } kind;
	union {
		struct {IR_exp left; IR_binop op; IR_exp right;} binop;
		IR_exp mem;
		Tmp_temp temp;
		struct {IR_stm stm; IR_exp exp;} eseq;
		Tmp_label label;
		int 	  constant;
		int       size; 		/* variable size */
		struct {IR_exp head; IR_expList tail;} call;
	} u;
};

extern IR_exp IR_Binop(IR_binop op, IR_exp left, IR_exp right);
extern IR_exp IR_Mem(IR_exp mem, int size);
extern IR_exp IR_Temp(Tmp_temp temp);
extern IR_exp IR_Eseq(IR_stm stm, IR_exp exp);
extern IR_exp IR_Name(Tmp_label label);			/* label of constant n */
extern IR_exp IR_Const(int constant);			/* constant */
extern IR_exp IR_Call(IR_exp head, IR_expList tail);

#endif 
