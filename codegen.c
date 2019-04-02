/*
 * file name: codegen.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Sun 24 Mar 2019 09:04:03 AM CST
 */

#include "include/codegen.h"
#include "include/translate.h"
#include "include/assem.h"
#include "include/IR-tree.h"
#include "include/semantic.h"

extern Tr_level lev;
void Cgen_Emit(FILE *out, char *instr, ...)
{
	va_list ap;
	va_start(ap, instr);
	vfprintf(out, instr, ap);
	va_end(ap);
	fprintf(out, "\n");
}

void Cgen_GlobalVarEmit(S_symbol name, S_symbol type, Sem_expty e)
{
	char s[200];
	sprintf(s, "%s:\n\t.%s: %s", S_Name(name), S_Name(type), "0");
	Cgen_Emit(out, s);
}
void CodeGen()
{
}
void Cgen_CodeGen(Assem_instr i)
{
	/* Test coding */
	Assem_Print(out, i, NULL);
}

static void Cgen_MunchStm(IR_stm s)
{
	switch (s->kind) {
	case IR_move:
		break;
	default:
		break;
	}
}

static void Cgen_MunchExp(IR_exp e)
{

}
