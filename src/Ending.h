#ifndef __ENDING_H__
#define __ENDING_H__


#include "CommonDefines.h"

#define MAX_STRIP ((WINDOW_HEIGHT / 16) + 1)*2

void ActionStripper(void);
void PutStripper(void);
void SetStripper(int x, int y, const char *text, int cast);
void RestoreStripper(void);
void ActionIllust(void);
void PutIllust(void);
void ReloadIllust(int a);
void InitCreditScript(void);
void ReleaseCreditScript(void);
bool StartCreditScript(void);
void ActionCredit(void);
void SetCreditIllust(int a);
void CutCreditIllust(void);
int Scene_DownIsland(int mode);

#endif // __ENDING_H__