/*
 * file name: include/assem.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Thu 21 Mar 2019 04:56:30 PM CST
 */

#ifndef _ASSEM_H
#define _ASSEM_H

#include "tmplabel.h"
#include <stdio.h>

typedef struct Assem_instrList_ *Assem_instrList;
typedef struct Assem_targets_ *Assem_targets;

struct Assem_targets_ {
	Tmp_labelList labels;
};

extern Assem_targets Assem_Targets(Tmp_labelList labels);

typedef struct {
	enum {Assem_oper, Assem_label, Assem_move} kind;
	union {
		/* "src" and "dst" are register table */
		struct {string assem; Tmp_tempList src, dst;
			Assem_targets jumps;} oper;
		struct {string assem, type, data; Tmp_label label;} label;
		struct {string assem; Tmp_tempList src, dst;} move;
	} u;
} *Assem_instr;

struct Assem_instrList_ {
	Assem_instr head;
	Assem_instrList tail;
};

extern Assem_instr Assem_Oper(string assem, Tmp_tempList src, 
		Tmp_tempList dst, Assem_targets jumps);
extern Assem_instr Assem_Label(string assem, string type, string data, Tmp_label label);
extern Assem_instr Assem_Move(string assem, Tmp_tempList src,
		Tmp_tempList dst);
extern void Assem_Print(FILE *out, Assem_instr i, Tmp_map m);

#endif 
