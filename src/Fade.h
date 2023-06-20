#ifndef __FADE_H__
#define __FADE_H__


#include "CommonDefines.h"

#define FADE_WIDTH	(((WINDOW_WIDTH - 1) / 16) + 1)
#define FADE_HEIGHT	(((WINDOW_HEIGHT - 1) / 16) + 1)

struct FADE
{
	int mode;
	bool bMask;
	int count;
	signed char ani_no[FADE_HEIGHT][FADE_WIDTH];
	signed char flag[FADE_HEIGHT][FADE_WIDTH];	// Not a boolEAN (those are unsigned)
	signed char dir;
};

extern FADE gFade;

void InitFade(void);
void SetFadeMask(void);
void ClearFade(void);
void StartFadeOut(signed char dir);
void StartFadeIn(signed char dir);
void ProcFade(void);
void PutFade(void);
bool GetFadeActive(void);

#endif // __FADE_H__