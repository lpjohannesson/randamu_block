#ifndef __BOSSOHM_H__
#define __BOSSOHM_H__

#include <utility>
#include "Bullet.h"
#include "NpChar.h"

void ActBossChar_Omega(void);
int Omega_HealthValue();
bool Omega_IsAbleToDamage(std::pair<NPCHAR&, int> npc, std::pair<BULLET&, int> bul);

#endif // __BOSSOHM_H__