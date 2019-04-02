/*
 * file name: include/codegen.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Sun 24 Mar 2019 09:01:38 AM CST
 */

#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "assem.h"
#include "semantic.h"

#include <stdio.h>
#include <stdarg.h>

FILE *out;

#define Cgen_Init(out_file)	\
{			\
	if ((out = fopen(out_file, "w+")) == NULL) \
		fprintf(stderr, "%s", strerror(errno));	\
}

extern void Cgen_Emit(FILE *out, char *instr, ...);
extern void Cgen_GlobalVarEmit(S_symbol name, S_symbol type, Sem_expty e);
extern void Cgen_CodeGen(Assem_instr i);

#endif
