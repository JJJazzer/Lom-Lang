/*
 * file name: IR-tree.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Thu 14 Mar 2019 09:25:52 AM CST
 *
 * Module's function: The declare of IR.
 */

#include "include/IR-tree.h"

IR_stm IR_Seq(IR_stm left, IR_stm right)
{
	IR_stm ts;
	ts = checked_malloc(sizeof(*ts));
	ts->kind = IR_seq;
	ts->u.seq.left = left;
	ts->u.seq.right = right;
	return ts;
}

IR_stm IR_Label(Tmp_label label)
{
	IR_stm ts;
	ts = checked_malloc(sizeof(*ts));
	ts->kind = IR_label;
	ts->u.label = label;
	return ts;
}

IR_stm IR_Jump(IR_exp exp, Tmp_labelList labels)
{
	IR_stm ts;
	ts = checked_malloc(sizeof(*ts));
	ts->kind = IR_jump;
	ts->u.jump.exp = exp;
	ts->u.jump.labels = labels;
	return ts;
}

IR_stm IR_Cjump(IR_exp left, IR_relop op, IR_exp right,
		Tmp_label trues, Tmp_label falses)
{
	IR_stm ts;
	ts = checked_malloc(sizeof(*ts));
	ts->kind = IR_cjump;
	ts->u.cjump.left = left;
	ts->u.cjump.op = op;
	ts->u.cjump.right = right;
	ts->u.cjump.trues = trues;
	ts->u.cjump.falses = falses;
	return ts;
}

IR_stm IR_Move(IR_exp src, IR_exp dst)
{
	IR_stm ts;
	ts = checked_malloc(sizeof(*ts));
	ts->u.move.src = src;
	ts->u.move.dst = dst;
	return ts;
}

IR_stm IR_Exp(IR_exp exp)
{
	IR_stm ts;
	ts = checked_malloc(sizeof(*ts));
	ts->kind = IR_eexp;
	ts->u.exp = exp;
	return ts;
}

IR_exp IR_Binop(IR_exp left, IR_binop op, IR_exp right)
{
	IR_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = IR_binOp;
	te->u.binop.left = left;
	te->u.binop.op = op;
	te->u.binop.right = right;
	return te;
}

IR_exp IR_Mem(IR_exp mem)
{
	IR_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = IR_mem;
	te->u.mem = mem;
	return te;
}

IR_exp IR_Temp(Tmp_temp temp)
{
	IR_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = IR_mem;
	te->u.temp = temp;
	return te;
}

IR_exp IR_Eseq(IR_stm stm, IR_exp exp)
{
	IR_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = IR_eseq;
	te->u.eseq.stm = stm;
	te->u.eseq.exp = exp;
	return te;
}

IR_exp IR_Name(Tmp_label label)
{
	IR_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = IR_name;
	te->u.label = label;
	return te;
}

IR_exp IR_Const(int constant)
{
	IR_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = IR_const;
	te->u.constant = constant;
	return te;
}

IR_exp IR_Call(IR_exp head, IR_expList tail)
{
	IR_exp te;
	te = checked_malloc(sizeof(*te));
	te->kind = IR_call;
	te->u.call.head = head;
	te->u.call.tail = tail;
	return te;
}
