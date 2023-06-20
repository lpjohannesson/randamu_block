#ifndef __STAR_H__
#define __STAR_H__


#include "NpChar.h"

struct STAR
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
};

extern NPCHAR* starTarget;
extern STAR star[3];

void InitStar(void);
void ActStar(void);
void PutStar(int fx, int fy);

#endif // __STAR_H__