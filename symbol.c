/*
 * file name: symbol.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Tue 26 Feb 2019 06:24:52 PM CST
 *
 * Modules description: Symbol table, but it not used to another modules expect "table" module.
 */

#define DEBUG
#ifndef DEBUG
#include "symbol.h"
#else
#include "include/symbol.h"
#endif
#include "table.h"

#include <string.h>

struct S_symbol_ {
	string   id;
	S_symbol next;
};

static S_symbol mksymbol(string id, S_symbol next)
{
	S_symbol s;
	s = checked_malloc(sizeof(*s));
	s->id   = id;
	s->next = next;
	return s;
}

#define SIZE 	109
static S_symbol hashtable[SIZE];
static unsigned int hash(string str)
{
	unsigned int hval = 0;
	while (*str != '\0') 
		hval = (hval << 5) | *str++;
	return hval;
}

S_symbol S_Symbol(string id)
{
	
	int i = hash(id) % SIZE;
	S_symbol syms = hashtable[i];
	S_symbol p;
	for (p = syms; p; p = p->next)
		if (strcmp(p->id, id) == 0) return p;
	p = mksymbol(id, syms);
	hashtable[i] = p;
	return p;
}

string S_Name(S_symbol sym)
{
	return sym->id;
}


S_table S_Empty(void)
{
	return Tab_Empty();
}

void S_Insert(S_table t, S_symbol sym, void *value)
{
	Tab_Insert(t, sym, value);
}

void *S_LookUp(S_table t, S_symbol sym)
{
	return Tab_LookUp(t, sym);
}

static struct S_symbol_ marksym = {"<BeginScope>", NULL};
void S_BeginScope(S_table t)
{
	S_Insert(t, &marksym, NULL);
}

void S_EndScope(S_table t)
{
	S_symbol s;
	do {
		s = Tab_PopUp(t);
	} while (s != &marksym);
}
