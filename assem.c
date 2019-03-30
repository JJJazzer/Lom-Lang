/*
 * file name: assem.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Tue 26 Mar 2019 09:21:11 AM CST
 */

#include "include/assem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Assem_targets Assem_Targets(Tmp_labelList labels)
{
	Assem_targets p;
	p = checked_malloc(sizeof(*p));
	p->labels = labels;
	return p;
}

Assem_instr Assem_Oper(string assem, Tmp_tempList src,
		Tmp_tempList dst, Assem_targets jumps)
{
	Assem_instr t;
	t = checked_malloc(sizeof(*t));
	t->kind = Assem_oper;
	t->u.oper.assem = assem;
	t->u.oper.src = src;
	t->u.oper.dst = dst;
	t->u.oper.jumps = jumps;
	return t;
}

Assem_instr Assem_Label(string assem, string type, string data, Tmp_label label)
{
	Assem_instr t;
	t = checked_malloc(sizeof(*t));
	t->kind = Assem_label;
	t->u.label.assem = assem;
	t->u.label.label = label;
	/* It used to advanced instruction in x86 */
	t->u.label.type = type;
	t->u.label.data = data;		
	return t;
}

Assem_instr Assem_Move(string assem, Tmp_tempList src, Tmp_tempList dst)
{
	Assem_instr t;
	t = checked_malloc(sizeof(*t));
	t->kind = Assem_move;
	t->u.move.assem  = assem;
	t->u.move.src = src;
	t->u.move.dst = dst;
	return t;
}

static void out_instr(char *instr, string assem, string type, string data,
		Tmp_tempList src, Tmp_tempList dst, Assem_targets jumps, Tmp_map m)
{
	
	if (type && data) {
		sprintf(instr, "%s:\n\t.%s: %s", assem, type, data);
	}
}

void Assem_Print(FILE *out, Assem_instr i, Tmp_map m)
{
	/* Out to start symbol */
	char instr[256];
	char s[100];
	
	switch (i->kind) {
	case Assem_label:
		out_instr(instr, s, i->u.label.type, i->u.label.data, NULL, NULL, NULL, m);
		fprintf(out, "%s", instr);
		break;
	case Assem_oper:
		break;
	case Assem_move:
		break;
	}
}

void Assem_PrintList(FILE *out, Assem_instrList iList, Tmp_map m)
{

}
