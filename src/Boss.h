#ifndef __BOSS_H__
#define __BOSS_H__

#include "NpChar.h"

#define BOSS_MAX 20

extern NPCHAR gBoss[BOSS_MAX];

typedef void (*BOSSFUNCTION)(void);
extern BOSSFUNCTION gpBossFuncTbl[10];

void InitBossChar(int code);
void PutBossChar(int fx, int fy);
void SetBossCharActNo(int a);
void HitBossBullet(void);
void ActBossChar(void);
void HitBossMap(void);

#endif // __BOSS_H__