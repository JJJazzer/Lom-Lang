/*
 * file name: errmsg.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Thu 28 Feb 2019 10:31:30 AM CST
 * Module's function: Track the position of lexical unit.
 */

#include <stdio.h>
#include <stdarg.h>
#include "errmsg.h"
#include "util.h"
#include "terms_color.h"

int Em_tokPos = 0;
int lineno = 1;

bool anyError = FALSE;
typedef struct Em_posList_ *Em_posList;
struct Em_posList_ {
	int pos;
	Em_posList next;
};

static Em_posList linePos;
static Em_posList Em_PosList(int pos, Em_posList next)
{
	Em_posList ep;
	ep = checked_malloc(sizeof(*ep));
	ep->pos = pos;
	ep->next = next;
	return ep;
}

void Em_NewLine()
{
	lineno++;
	linePos = Em_PosList(Em_tokPos, linePos);
}

void Em_Error(int pos, char *msg, ...)
{
	Em_posList line = linePos;
	va_list ap;
	int num = lineno;
	anyError = TRUE;
	while (line && line->pos >= pos) {
		line = line->next;
		num--;
	}
	if (line)
		fprintf(stderr, WHITE(LIGHT) "lines: %d, position: %d: " RESET, num, pos - line->pos);
	va_start(ap, msg);
	vfprintf(stderr, msg, ap);
	va_end(ap);
	fprintf(stderr, "\n");
}

void Em_Reset()
{
	Em_PosList(0, NULL);
}
