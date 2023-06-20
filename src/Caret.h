#ifndef __CARET_H__
#define __CARET_H__

#include "CommonDefines.h"

#define CARET_MAX 0x2000

struct CARET_TABLE
{
	int view_left;
	int view_top;
};

struct CARET
{
	int cond;
	int code;
	int direct;
	int x;
	int y;
	int xm;
	int ym;
	int act_no;
	int act_wait;
	int ani_no;
	int ani_wait;
	int view_left;
	int view_top;
	Rect rect;
	float opacity;
};

extern CARET gCrt[CARET_MAX];
extern CARET_TABLE gCaretTable[];
extern bool chainWrap;

void InitCaret(void);
void ActCaret(void);
void PutCaret(int fx, int fy);
void SetCaret(int x, int y, int code, int dir);
CARET* SetCaret(int x, int y, int code);

#endif // __CARET_H__