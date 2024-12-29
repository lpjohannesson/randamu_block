#ifndef __NPCHIT_H__
#define __NPCHIT_H__

#include <utility>

#include "CommonDefines.h"

#include "Bullet.h"
#include "Game.h"
#include "NpChar.h"

void JadgeHitNpCharBlock(NPCHAR *npc, int x, int y);
void JudgeHitNpCharTriangleA(NPCHAR *npc, int x, int y);
void JudgeHitNpCharTriangleB(NPCHAR *npc, int x, int y);
void JudgeHitNpCharTriangleC(NPCHAR *npc, int x, int y);
void JudgeHitNpCharTriangleD(NPCHAR *npc, int x, int y);
void JudgeHitNpCharTriangleE(NPCHAR *npc, int x, int y);
void JudgeHitNpCharTriangleF(NPCHAR *npc, int x, int y);
void JudgeHitNpCharTriangleG(NPCHAR *npc, int x, int y);
void JudgeHitNpCharTriangleH(NPCHAR *npc, int x, int y);
void JudgeHitNpCharWater(NPCHAR *npc, int x, int y);
void HitNpCharMap(void);
void LoseNpChar(NPCHAR *npc, bool bVanish);
bool IntersectDamageCheck(NPCHAR &npc, int x, int y, int xl = 4_pix, int yl = 4_pix);
bool IsAbleToDamage(std::pair<NPCHAR&, int> npc, std::pair<BULLET&, int> bul);
bool DealDamage(NPCHAR &npc, int dmg, BULLET* bul = NULL);
void HitNpCharBullet(void);

#endif // __NPCHIT_H__