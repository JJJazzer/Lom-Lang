/*
 * file name: include/table.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Tue 26 Feb 2019 07:04:35 PM CST
 */

#ifndef _TABLE_H
#define _TABLE_H

typedef struct Tab_table_ *Tab_table;
extern Tab_table Tab_Empty(void);
extern void      Tab_Insert(Tab_table t, void *key, void *value);
extern void     *Tab_LookUp(Tab_table t, void *key);
extern void     *Tab_PopUp(Tab_table t);

#endif
