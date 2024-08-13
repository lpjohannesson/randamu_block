#ifndef __COOLDOWN_H__
#define __COOLDOWN_H__

#include "Json.h"

#include <vector>

struct Cooldown {
	int id;
	int duration;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Cooldown, id, duration)
};

const int CHAOS_KEY_ID = 0;
const int TIME_RING_ID = 1;
const int TIME_STOP_ID = 2;
const int SHEEP_CURSE_ID = 3;
const int SARA_DAMAGE_ID = 4;
const int DUCK_TIMER_ID = 5;
const int DASH_TIMER_ID = 6;
const int NANITES_TIME_ID = 7;
const int NANITES_CD_ID = 8;
const int GRAVITY_FLIP_ID = 9;
const int FLOODTIDE_ATTACK_ID = 10;
const int SHEATHING_TIMER_ID = 11;
const int FLOODTIDE_BOOST_ID = 12;
const int HELLSWEEP_BOOST_ID = 13;
const int DOCTOR_SHOCK_ID = 14;

void InitiateCooldown(std::vector<Cooldown>& list, int id);
void InitiateCooldown(std::vector<Cooldown>& list, int id, int duration);
void IncrementCooldown(std::vector<Cooldown>& list, int id, int duration);
void AddToCooldown(std::vector<Cooldown>& list, int id, int duration);
void OverrideCooldown(std::vector<Cooldown>& list, int id, int duration);
void ClearCooldown(std::vector<Cooldown>& list, int id);
int CheckForCooldown(std::vector<Cooldown>& list, int id);
void PutCooldownList(std::vector<Cooldown>& list);
void ActCooldownList(std::vector<Cooldown>& list);
int CooldownDuration(int id);
#endif // __COOLDOWN_H__