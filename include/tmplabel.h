/*
 * file name: include/tmplabel.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Wed 06 Mar 2019 10:56:22 PM CST
 */

#ifndef _TMPLABEL_H
#define _TMPLABEL_H

#include "symbol.h"
#include "util.h"

typedef struct Tmp_temp_ *Tmp_temp;
extern Tmp_temp Tmp_NewTemp(void);

typedef S_symbol Tmp_label;
extern Tmp_label Tmp_NewLabel(void);
extern Tmp_label Tmp_CustomizeNameLabel(string name);
extern string Tmp_LabelString(Tmp_label s);

typedef struct Tmp_tempList_ *Tmp_tempList;
struct Tmp_tempList_ {
	Tmp_temp head;
	Tmp_tempList tail;
};
extern Tmp_tempList Tmp_TempList(Tmp_temp head, Tmp_tempList tail);

typedef struct Tmp_labelList_ *Tmp_labelList;
struct Tmp_labelList_ {
	Tmp_label head;
	Tmp_labelList tail;
};
extern Tmp_labelList Tmp_LabelList(Tmp_label head, Tmp_labelList tail);

typedef struct Tmp_map_ *Tmp_map;
Tmp_map Tmp_Empty(void);
Tmp_map Tmp_LayerMap(Tmp_map over, Tmp_map under);
void Tmp_Enter(Tmp_map m, Tmp_temp t, string s);
string Tmp_Look(Tmp_map m, Tmp_temp t);

#endif
