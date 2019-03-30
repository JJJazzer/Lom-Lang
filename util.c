/*
 * file name: util.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Fri 22 Feb 2019 08:54:43 PM CST
 *
 * Module description: The tools of another modules.
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "util.h"

void *checked_malloc(int len)
{
	void *m;
	m = malloc(len);
	assert(m);
	return m;
}

string String(string s)
{
	string p = checked_malloc(strlen(s)+1);
	strcpy(p,s);
	return p;
}

U_boolList U_BoolList(bool head, U_boolList tail)
{
	U_boolList tb;
	tb = checked_malloc(sizeof(*tb));
	tb->head = head;
	tb->tail = tail;
	return tb;
}
