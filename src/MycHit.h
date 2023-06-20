#ifndef __MYCHIT_H__
#define __MYCHIT_H__

#include "NpChar.h"

void ResetMyCharFlag(void);
void HitMyCharMap(void);
void HitMyCharNpChar(void);
void HitMyCharBoss(void);
void DetermineHitReason(int npcID);
unsigned char JudgeHitMyCharNPC3(NPCHAR *npc);

#endif // __MYCHIT_H__