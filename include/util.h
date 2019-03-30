/*
 * file name: include/util.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Fri 22 Feb 2019 06:25:02 PM CST
 */

#ifndef _UTIL_H
#define _UTIL_H
typedef char *string;
typedef int   bool;
typedef int   position;
typedef struct U_boolList_ *U_boolList;

#define MAXLINE 	4096
enum {
	FALSE, TRUE,
};
extern void *checked_malloc(int len);
extern string String(string s);

#define ARRLENG(x)	(sizeof((x)) / sizeof(*(x)))

struct U_boolList_ {
	bool head;
	U_boolList tail;
};

extern U_boolList U_BoolList(bool head, U_boolList tail);

#endif
