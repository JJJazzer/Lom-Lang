/*
 * file name: table.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Tue 26 Feb 2019 07:07:05 PM CST
 * 
 * Module's description: The symbol table.
 */

#define DEBUG
#ifndef DEBUG 
#include "util.h"
#include "table.h"

#else
#include "include/table.h"
#include "include/util.h"
#endif

#define NULL ((void*) 0)
#define TABSIZE 	127
typedef struct binder_ *binder;
struct binder_ {
	void   *key;
	void   *value;
	binder  next;
	void   *prevTopStack;
};

struct Tab_table_ {
	binder table[TABSIZE];
	void  *top;
};

static binder Binder(void *key, void *value, binder next, void *prevTopStack)
{
	binder b;
	b = checked_malloc(sizeof(*b));
	b->key = key;
	b->value = value;
	b->next  = next;
	b->prevTopStack = prevTopStack;
	return b;
}

Tab_table Tab_Empty()
{
	Tab_table t;
	int i;
	t = checked_malloc(sizeof(*t));
	t->top = NULL;

	for (i = 0; i < TABSIZE; i++)
		t->table[i] = NULL;
	return t;
}

void Tab_Insert(Tab_table t, void *key, void *value)
{
	unsigned index;
	index = ((unsigned) key) % TABSIZE;
	t->table[index] = Binder(key, value, t->table[index], t->top);
	t->top = key;
}

void *Tab_LookUp(Tab_table t, void *key)
{
	unsigned index;
	index = ((unsigned) key) % TABSIZE;
	binder p;
	for (p = t->table[index]; p; p = p->next) 
		if (p->key == key)
			return p->value;
	return NULL;
}

void *Tab_PopUp(Tab_table t)
{
	void *k;
	k = t->top;
	binder b;
	int index = ((unsigned)k) % TABSIZE;
	b = t->table[index];
	t->table[index] = b->next;
	t->top = b->prevTopStack;
	return b->key;
}
