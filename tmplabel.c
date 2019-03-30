/*
 * file name: tmplabel.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Wed 06 Mar 2019 11:12:32 PM CST
 * 
 * Modules description: Relative of temporary variable.
 */

#include "include/tmplabel.h"
#include <stdio.h>

typedef struct Tmp_map_ *Tmp_map;
struct Tmp_temp_ {
	int num;
};

static int label_number = 0;
Tmp_label Tmp_NewLabel(void)
{
	char s[100];
	sprintf(s, ".L%d", label_number++);
	return Tmp_CustomizeNameLabel(s);
}

Tmp_label Tmp_CustomizeNameLabel(string name)
{
	return S_Symbol(name);
}

string Tmp_LabelString(Tmp_label s)
{
	return S_Name(s);
}

struct Tmp_map_ {
	Tmp_temp temp;
	Tmp_map  tail;
};

Tmp_temp Tmp_NewTemp(void)
{	
	Tmp_temp t;
	t = checked_malloc(sizeof(*t));
	t->num++;
	return t;
}

Tmp_tempList Tmp_TempList(Tmp_temp head, Tmp_tempList tail)
{
	Tmp_tempList tt = checked_malloc(sizeof(*tt));
	tt->head = head;
	tt->tail = tail;
	return tt;
}

Tmp_labelList Tmp_LabelList(Tmp_label head, Tmp_labelList tail)
{
	Tmp_labelList tl = checked_malloc(sizeof(*tl));
	tl->head = head;
	tl->tail = tail;
	return tl;
}
