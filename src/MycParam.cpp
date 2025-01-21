#include "MycParam.h"

#include <cstddef>
#include <cstdio>
#include <string>

#include "ArmsItem.h"
#include "Bullet.h"
#include "Caret.h"
#include "Cooldown.h"
#include "Draw.h"
#include "File.h"
#include "Flags.h"
#include "Game.h"
#include "Main.h"
#include "MyChar.h"
#include "NpChar.h"
#include "NpcHit.h"
#include "ResourceCode.h"
#include "Score.h"
#include "Stage.h"
#include "Sound.h"
#include "TextScr.h"
#include "ValueView.h"

ARMS_LEVEL gArmsLevelTable[] =
{
	{{0,  0,  100, 100}}, //nothing
	{{25, 55, 20, 20}}, //snake
	{{1000, 1000, 1000, 1000}}, //polestar
	{{16, 30, 20, 20}}, //fireball
	{{30, 30, 30, 30}}, //machine gun
	{{20, 100000, 100000, 1000}}, //launcher
	{{10, 20, 30, 1000}}, //missiles
	{{25, 40, 40, 40}}, //bubbler
	{{30, 60, 25, 5}}, //whip
	{{30, 60, 5, 35}}, //blade
	{{30, 60, 10, 100}}, //
	{{40, 50, 20, 60}}, //
	{{1000,  1000,  1000, 1000}}, //nemesis
	{{60, 90, 260, 420}}, //spur
	{{0, 0, 300, 0}}, //water gun
	{{30, 60, 10, 70}}, //boomerang blades
	{{30, 50, 20, 70}}, //rifle
	{{40, 60, 20, 80}}, //tideblade
	{{20, 20, 20, 20}},
	{{30, 50, 20, 40}},
	{{55, 70, 30, 90}}, //hellswing
	{{30, 60, 90, 120}}, //gatling gun
	{{35, 60, 35, 40}} //frontie-buster
};

void AddScore(int x){
	if (GetNPCFlag(Flags::MAHIN_CHARACTER)) x /= 10;
	switch(gMC.difficulty){
		case DIFFICULTY_HARDENED: x *= 1.25; break;
		case DIFFICULTY_REINFORCED: x *= 1.75; break;
		case DIFFICULTY_RESONANT: x *= 2.5; break;
	}
	gMC.score += x;
}

void AddExpMyChar(int x)
{
	if (GetNPCFlag(Flags::EXCERPT_CHARACTER)) x *= 2;
	if (GetNPCFlag(Flags::MAHIN_CHARACTER)) x *= 1.5;

	int lv = gArmsData[gSelectedArms].level - 1;
	int arms_code = gArmsData[gSelectedArms].code;
	bool isMiningLaser = arms_code == 18;

	gArmsData[gSelectedArms].exp += x;

	bool zeroXPArm = arms_code == 13 || arms_code == 2 || arms_code == 12 || arms_code == 14;

	int max_lv = 2;
	if (CheckFlagTrait(FlagTraits::LEVEL_4_WEAPONS)) max_lv = 3;
	if (isMiningLaser) max_lv = lv;

	if (lv == max_lv)
	{
		int max_xp = gArmsLevelTable[arms_code].exp[lv];
		if (isMiningLaser) max_xp = 100;
		if (gArmsData[gSelectedArms].exp >= max_xp)
		{
			gArmsData[gSelectedArms].exp = max_xp;
		}
	}
	else
	{
		while (gArmsData[gSelectedArms].exp >= gArmsLevelTable[arms_code].exp[lv] && lv < max_lv && !isMiningLaser)
		{
			gArmsData[gSelectedArms].exp -= gArmsLevelTable[arms_code].exp[lv];
			if (arms_code == 5){
				gArmsData[gSelectedArms].num = std::min(gArmsData[gSelectedArms].num+1, gArmsData[gSelectedArms].max_num);
				PlaySoundObject(42);
			} else {
				++gArmsData[gSelectedArms].level;
				lv++;
			}
			if (lv == max_lv)
				gArmsData[gSelectedArms].exp = std::min(gArmsData[gSelectedArms].exp, gArmsLevelTable[arms_code].exp[lv]);

			if (!zeroXPArm)
			{
				PlaySoundObject(27, SOUND_MODE_PLAY);
				SetCaret(gMC.x, gMC.y, 10, 0);
			}
		}
	}

	if (!zeroXPArm)
	{
		gMC.exp_count += x;
		gMC.exp_wait = 30;
	}
	else
	{
		gMC.exp_wait = 10;
	}
}

void ZeroExpMyChar(void)
{
	gArmsData[gSelectedArms].level = 1;
	gArmsData[gSelectedArms].exp = 0;
}

bool IsMaxExpMyChar(void)
{
	int arms_code;

	int max_lv = 3;
	if (CheckFlagTrait(FlagTraits::LEVEL_4_WEAPONS)) max_lv = 4;

	if (gArmsData[gSelectedArms].level == max_lv)
	{
		arms_code = gArmsData[gSelectedArms].code;

		if (gArmsData[gSelectedArms].exp >= gArmsLevelTable[arms_code].exp[max_lv-1])
			return true;
	}

	return false;
}

int RegenerationDelay(){
	return GetFlagTraitData<int>(FlagTraits::LIFESTEAL, "amount_per_heart");
}

void DamageMyChar(int damage)
{
	if (ChallengeEnabled("debug"))
		return;

#ifdef FIX_BUGS
	if (!(g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED))
#else
	// I'm preeeetty sure this is a typo. The Linux port optimised it out.
	if (!(g_GameFlags | GAME_FLAG_IS_CONTROL_ENABLED))
#endif
		return;

	if (ChallengeEnabled("one_hit")){
		Die();
		return;
	}

	if (CheckFlagTrait(FlagTraits::LOW_DAMAGE_NULLIFY) && damage <= GetFlagTraitData<int>(FlagTraits::LOW_DAMAGE_NULLIFY, "threshold") && Random(0, GetFlagTraitData<int>(FlagTraits::LOW_DAMAGE_NULLIFY, "chance")-1) <= 0){
		damage = 0;
	}

	int oldDamage = damage;

	if (GetNPCFlag(Flags::EXCERPT_CHARACTER)){
		damage *= 1.75;
	}

	if (CheckFlagTrait(FlagTraits::LOW_HP_RESIST) && gMC.life < gMC.max_life*0.5){
		damage *= 0.66;
	}

	if (gMC.saraDash > 0 && damage != 0 && CheckForCooldown(gCooldowns, SARA_DAMAGE_ID) == -1){
		damage = 0;
		InitiateCooldown(gCooldowns, SARA_DAMAGE_ID);
		SetCaret(gMC.x-0x2500, gMC.y, 2, 2);
			SetCaret(gMC.x-0x1800, gMC.y-0x1800, 2, 2);
			SetCaret(gMC.x+0x2500, gMC.y, 2, 2);
			SetCaret(gMC.x+0x1800, gMC.y-0x1800, 2, 2);
			SetCaret(gMC.x, gMC.y+0x2500, 2, 2);
			SetCaret(gMC.x+0x1800, gMC.y+0x1800, 2, 2);
			SetCaret(gMC.x, gMC.y-0x2500, 2, 2);
			SetCaret(gMC.x-0x1800, gMC.y+0x1800, 2, 2);
	}

	if (GetNPCFlag(Flags::SHIELD_OF_RESILIENCE) && damage > 1){
		damage -= 1;
		PlaySoundObject(54, SOUND_MODE_PLAY);
	}
	if (CheckFlagTrait(FlagTraits::DUCK_HEALTH) && damage != 0){
		if (damage > 9) damage = 3;
		else if (damage > 4) damage = 2;
		else damage = 1;
	}
	if (GetNPCFlag(Flags::QUATE_CHARACTER)){
		damage *= float(Random(50, 175)) / 100.0f;
	}

	if (gMC.shock)
		return;

	// Damage player
	if (damage == 0){
		SetCaret(gMC.x, gMC.y, 2, 2);
		SetCaret(gMC.x-0x1000, gMC.y, 2, 2);
		SetCaret(gMC.x+0x1000, gMC.y, 2, 2);
		SetCaret(gMC.x, gMC.y+0x1000, 2, 2);
		SetCaret(gMC.x, gMC.y-0x1000, 2, 2);
		PlaySoundObject(31, SOUND_MODE_PLAY);
		PlaySoundObject(31, SOUND_MODE_PLAY);
		PlaySoundObject(31, SOUND_MODE_PLAY);
	}
	else{
		int sound = 16;
		if (GetNPCFlag(Flags::MAHIN_CHARACTER))
			sound = 52;
		if (GetNPCFlag(Flags::SARA_CHARACTER) || GetNPCFlag(Flags::LILY_CHARACTER) || GetNPCFlag(Flags::SERENITY_CHARACTER))
			sound = 57;
		if (GetNPCFlag(Flags::AMY_CHARACTER))
			sound = 50;
		if (GetNPCFlag(Flags::PROCGEN_CHARACTER))
			sound = 54;
		if (GetNPCFlag(Flags::PUPPY_CHARACTER))
			sound = 105;
		PlaySoundObject(sound, SOUND_MODE_PLAY);
	}
	gMC.cond &= ~1;
	gMC.shock = 128;
	gMC.regenCount = 0;
	if (CheckFlagTrait(FlagTraits::DUCKS))
		InitiateCooldown(gCooldowns, DUCK_TIMER_ID);
	gMC.star = 0;
	if (GetNPCFlag(Flags::MAXY_CHARACTER)){
		gMC.shock = 70;
	}
	if (GetNPCFlag(Flags::MAHIN_CHARACTER)){
		gMC.shock = 200;
	}
	if (CheckFlagTrait(FlagTraits::IFRAMES_BOOST)){
		if (damage != 0){
			gMC.shock *= 1.5;
		} else {
			gMC.shock *= 0.75;
		}
	}
	if (GetNPCFlag(Flags::QUATE_CHARACTER)){
		gMC.shock *= float(Random(40, 150)) / 100.0f;
	}
	if (gMC.difficulty > DIFFICULTY_REINFORCED){
		gMC.shock *= 0.75;
	}
	if (gMC.difficulty > DIFFICULTY_REINFORCED && gMC.flag & 0x100){
		gMC.air -= damage * 25;
	}
	if (CheckFlagTrait(FlagTraits::UNIVERSAL_IFRAMES_MODIFIER)){
		gMC.shock *= GetFlagTraitData<float>(FlagTraits::UNIVERSAL_IFRAMES_MODIFIER, "modifier");
	}
	if (damage <= 1){
		gMC.shock *= 0.66f;
	}
	if (CheckFlagTrait(FlagTraits::NANITES) && CheckForCooldown(gCooldowns, NANITES_CD_ID) == -1
						&& CheckForCooldown(gCooldowns, NANITES_TIME_ID) == -1){
		InitiateCooldown(gCooldowns, NANITES_TIME_ID);
		gMC.shock = CooldownDuration(NANITES_TIME_ID);
		SetDestroyNpChar(gMC.x, gMC.y, 0x1400, 0x10);
		damage = 0;
		PlaySoundObject(31, SOUND_MODE_PLAY);
		PlaySoundObject(31, SOUND_MODE_PLAY);
		PlaySoundObject(31, SOUND_MODE_PLAY);				
	}

	if (gMC.unit == 1)
	{
		// Another weird case where there *has* to be an empty 'if' here to produce the same assembly.
		// Chances are there used to be some commented-out code here.
	}
	else
	{
		if (!CheckFlagTrait(FlagTraits::IFRAMES_BOOST))
			gMC.ym = -0x400 * (gMC.flip ? -1 : 1);
	}
	bool clayMedalProc = false;
	if (gStageNo == 65){
		if (Random(0, 2) <= 0){
			AddToCooldown(gCooldowns, DOCTOR_SHOCK_ID, 75);
		}
		if (oldDamage == 1 || oldDamage == 14){
			AddToCooldown(gCooldowns, DOCTOR_SHOCK_ID, 150);
		}
	}

	if (CheckFlagTrait(FlagTraits::KO_PREVENTION) && (gMC.life - damage) <= 0 && gMC.life > 1){
		for (int i = 0; i < 50; i++)
			SetCaret(gMC.x + 0x200 * 8, gMC.y, 11, 0);
		PlaySoundObject(102, SOUND_MODE_PLAY);
		clayMedalProc = true;
	}
	
	if (clayMedalProc){
		gMC.life = 1;
		gMC.regenCount = -420;
	}
	else 
		gMC.life -= (short)damage;

	float damageMod = 1.0f;
	// if (gMC.difficulty > DIFFICULTY_REINFORCED){
	// 	damageMod = 1.5f;
	// }

	// Lose experience
	if (gArmsData[gSelectedArms].code != 18){
	if (CheckFlagTrait(FlagTraits::EXP_SAVING)){
		gArmsData[gSelectedArms].exp -= damage*
			2/std::max(0.1f, GetFlagTraitData<float>(FlagTraits::EXP_SAVING, "modifier"))*
			damageMod;
	}
	else
		gArmsData[gSelectedArms].exp -= damage * 2 * damageMod;

	if (CheckFlagTrait(FlagTraits::MINING_CHARGE_FROM_DAMAGE) && (gArmsData[gSelectedArms].code == 2 || gArmsData[gSelectedArms].code == 12)){
		ChargeArmsEnergy(damage*std::max(0.0f, GetFlagTraitData<float>(FlagTraits::MINING_CHARGE_FROM_DAMAGE, "amount")));
	}

	while (gArmsData[gSelectedArms].exp < 0)
	{
		if (gArmsData[gSelectedArms].level > 1)
		{
			--gArmsData[gSelectedArms].level;

			int lv = gArmsData[gSelectedArms].level - 1;
			int arms_code = gArmsData[gSelectedArms].code;

			gArmsData[gSelectedArms].exp = gArmsLevelTable[arms_code].exp[lv] + gArmsData[gSelectedArms].exp;

			if (gMC.life > 0 && gArmsData[gSelectedArms].code != 13)
				SetCaret(gMC.x, gMC.y, 10, 2);
		}
		else
		{
			gArmsData[gSelectedArms].exp = 0;
		}
	}
	}

	if (CheckFlagTrait(FlagTraits::EXPLODE_ON_HIT)){
		SetBullet(Bullets::BIGGER_EXPLOSION, gMC.x + gMC.xm, gMC.y + gMC.ym, 0);
	}

	// Tell player how much damage was taken
	if (!(gMC.life <= 0 && GetNPCFlag(Flags::BADGE_OF_LOVE)) && !CheckFlagTrait(FlagTraits::DUCK_HEALTH))
		SetValueView(&gMC.x, &gMC.y, -damage);

	// Death
	 if (gMC.life <= 0)
	{
		Die();
	}
}

void Die(void){
	if (GetNPCFlag(Flags::BADGE_OF_LOVE)){
		for (int i = 0; i < NPC_MAX; i++){
			if (gNPC[i].cond & 0x80 && gNPC[i].code_char == 320 || gNPC[i].code_char == 321){
				LoseNpChar(&gNPC[i], false);
			}
		}
		SetNPCFlag(Flags::BADGE_OF_LOVE_REVIVE_PROC);
		gMC.life = gMC.max_life;
		PlaySoundObject(71, SOUND_MODE_PLAY);
		CutNPCFlag(Flags::BADGE_OF_LOVE);
		SubItemData(17);
		EquipItem(64, false);
		for (int i = 0; i < 100; i++)
			SetCaret(gMC.x + 0x200 * 8, gMC.y, 11, 0);
	}
	else if (CheckFlagTrait(FlagTraits::LAST_STAND) && !GetNPCFlag(Flags::TOROKO_BLEEDING_OUT)){
			gMC.life = 0;
			SetNPCFlag(Flags::TOROKO_BLEEDING_OUT);
		} else {
		gMC.life = 0;
		int sound = 17;
		if (GetNPCFlag(Flags::MAHIN_CHARACTER))
			sound = 25;
		if (GetNPCFlag(Flags::SARA_CHARACTER) || GetNPCFlag(Flags::LILY_CHARACTER) || GetNPCFlag(Flags::SERENITY_CHARACTER))
			sound = 35;
		if (GetNPCFlag(Flags::AMY_CHARACTER) || GetNPCFlag(Flags::PUPPY_CHARACTER))
			sound = 53;
		PlaySoundObject(sound, SOUND_MODE_PLAY);
		gMC.cond = 0;
		SetDestroyNpChar(gMC.x, gMC.y, 0x1400, 0x40);
		StartTextScript(40);
	}
}

void ZeroArmsEnergy_All(void)
{
	int a;

	for (a = 0; a < ARMS_MAX; a++)
	{
		gArmsData[a].level = 1;
		gArmsData[a].exp = 0;
	}
}

void AddBulletMyChar(int no, int val)
{
	gMC.a += 1;
	AddScore(500);
	SetNPCFlag(no);
}

void AddLifeMyChar(int x)
{
	if (ChallengeEnabled("one_hit") && x < 0){
		Die();
		return;
	}
	if (CheckFlagTrait(FlagTraits::DUCK_HEALTH) && x > 0 && x < 1000) x = 1;

	if (x == 1000)
		gMC.air = 1000;

	gMC.life += (short)x;
	if (gMC.life > gMC.max_life)
		gMC.life = gMC.max_life;
	gMC.lifeBr = gMC.life;
	if (gMC.life <= 0)
	{
		if (CheckFlagTrait(FlagTraits::LAST_STAND) && !GetNPCFlag(Flags::TOROKO_BLEEDING_OUT)){
			gMC.life = 0;
			SetNPCFlag(Flags::TOROKO_BLEEDING_OUT);
		} else {
		int sound = 17;
		if (GetNPCFlag(Flags::SARA_CHARACTER) || GetNPCFlag(Flags::LILY_CHARACTER))
			sound = 35;
		PlaySoundObject(sound, SOUND_MODE_PLAY);
		gMC.cond = 0;
		SetDestroyNpChar(gMC.x, gMC.y, 0x1400, 0x40);
		StartTextScript(40);
		}
	}
}

void AddMaxLifeMyChar(int val)
{
	if (CheckFlagTrait(FlagTraits::DUCK_HEALTH))
		return;
		
	gMC.max_life += (short)val;
	gMC.life += (short)val;
	gMC.lifeBr = gMC.life;
}

void PutArmsEnergy(bool flash)
{
	static unsigned char add_flash;

	Rect rcPer = {72, 48, 80, 56};
	Rect rcLv = {144, 80, 160, 88};
	rcLv.left += gMC.difficulty * 16;
	rcLv.right += gMC.difficulty * 16;
	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	Rect rcNone = {80, 48, 96, 56};

	if (gArmsEnergyX > 16)
		gArmsEnergyX -= 2;
	if (gArmsEnergyX < 16)
		gArmsEnergyX += 2;

	bool isMiningLaser = gArmsData[gSelectedArms].code == 18;

	int max_lv = 2;
	if (CheckFlagTrait(FlagTraits::LEVEL_4_WEAPONS)) max_lv = 3;
	if (isMiningLaser) max_lv = gArmsData[gSelectedArms].level;

	// Draw max ammo
	if (gArmsData[gSelectedArms].max_num)
	{
		PutNumber4(gArmsEnergyX + 32, 16, gArmsData[gSelectedArms].num, false);
		PutNumber4(gArmsEnergyX + 32, 24, gArmsData[gSelectedArms].max_num, false);
	}
	else
	{
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX + 48), PixelToScreenCoord(16), &rcNone, SURFACE_ID_TEXT_BOX);
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX + 48), PixelToScreenCoord(24), &rcNone, SURFACE_ID_TEXT_BOX);
	}

	// Draw experience and ammo
	if (flash == true && (gMC.shock / 2) % 2)
		return;

	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX + 32), PixelToScreenCoord(24), &rcPer, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX), PixelToScreenCoord(32), &rcLv, SURFACE_ID_TEXT_BOX);
	PutNumber4(gArmsEnergyX - 8, 32, gArmsData[gSelectedArms].level, false);

	Rect rcExpBox = {0, 72, 40, 80};
	Rect rcExpVal = {0, 80, 0, 88};
	Rect rcExpMax = {40, 72, 80, 80};
	Rect rcExpFlash = {40, 80, 80, 88};

	int lv = gArmsData[gSelectedArms].level - 1;

#ifdef FIX_BUGS
	// When the player has no weapons, the default level is 0, which becomes -1.
	// Catch it, and set it to 0 instead, so the following array-accesses aren't
	// out-of-bounds.
	if (lv < 0)
		lv = 0;
#endif

	int arms_code = gArmsData[gSelectedArms].code;
	int exp_now = gArmsData[gSelectedArms].exp;
	int exp_next = gArmsLevelTable[arms_code].exp[lv];
	if (isMiningLaser)
		exp_next = 100;

	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX + 24), PixelToScreenCoord(32), &rcExpBox, SURFACE_ID_TEXT_BOX);

	if ((lv == max_lv) && gArmsData[gSelectedArms].exp == gArmsLevelTable[arms_code].exp[lv])
	{
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX + 24), PixelToScreenCoord(32), &rcExpMax, SURFACE_ID_TEXT_BOX);
	}
	else
	{
		if (exp_next != 0)
			rcExpVal.right += (exp_now * 40) / exp_next;
		else
			rcExpVal.right = 0;

		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX + 24), PixelToScreenCoord(32), &rcExpVal, SURFACE_ID_TEXT_BOX);
	}

	if (gMC.exp_wait && ((add_flash++ / 2) % 2))
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX + 24), PixelToScreenCoord(32), &rcExpFlash, SURFACE_ID_TEXT_BOX);
}

void PutActiveArmsList(void)
{
	int x;
	int a;
	Rect rect = {0, 0, 0, 16};

	int arms_num = 0;
	while (gArmsData[arms_num].code != 0)
		++arms_num;

	if (arms_num == 0)
		return;

	for (a = 0; a < arms_num; ++a)
	{
		// Get X position to draw at
		x = ((a - gSelectedArms) * 16) + gArmsEnergyX;

		if (x < 8)
			x += 48 + (arms_num * 16);
		else if (x >= 24)
			x += 48;

		if (x >= 72 + ((arms_num - 1) * 16))
			x -= 48 + (arms_num * 16);
		if (x < 72 && x >= 24)
			x -= 48;

		// Draw icon
		rect.left = gArmsData[a].code * 16;
		rect.right = rect.left + 16;
		if (CheckFlagTrait(FlagTraits::INVIDIA) && gArmsData[a].code == 13){
			rect.left = 10 * 16;
			rect.right = rect.left + 16;
		}
		if (CheckFlagTrait(FlagTraits::HOOK_FLAIL) && gArmsData[a].code == 8){
			rect.left = 6 * 16;
			rect.right = rect.left + 16;
		}
		PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(x), PixelToScreenCoord(16), &rect, SURFACE_ID_ARMS_IMAGE);
	}
}

void PutActiveArm(void)
{
	int x;
	int a;
	Rect rect = {0, 0, 0, 16};
		// Get X position to draw at
		x = gArmsEnergyX;

		if (x < 8)
			x += 48 + (gSelectedArms * 16);
		else if (x >= 24)
			x += 48;

		if (x >= 72 + ((gSelectedArms - 1) * 16))
			x -= 48 + (gSelectedArms * 16);
		if (x < 72 && x >= 24)
			x -= 48;

		// Draw icon
		rect.left = gArmsData[gSelectedArms].code * 16;
		rect.right = rect.left + 16;
		if (CheckFlagTrait(FlagTraits::INVIDIA) && gArmsData[gSelectedArms].code == 13){
			rect.left = 10 * 16;
			rect.right = rect.left + 16;
		}
		if (CheckFlagTrait(FlagTraits::HOOK_FLAIL) && gArmsData[gSelectedArms].code == 8){
			rect.left = 6 * 16;
			rect.right = rect.left + 16;
		}
		PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(x), PixelToScreenCoord(16), &rect, SURFACE_ID_ARMS_IMAGE);
}

void PutMyLife(bool flash)
{
	if (ChallengeEnabled("one_hit")){
		return;
	}

	if (CheckFlagTrait(FlagTraits::DUCK_HEALTH)){
		Rect rcDuck = {78, 162, 90, 172};
		for (int i = 0; i < gMC.life-1; i++){
			PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(25 + 13*i), PixelToScreenCoord(40), &rcDuck, SURFACE_ID_BULLET);
		}
	} else {

	Rect rcCase = {24, 40, 232, 48};
	Rect rcDifficulty = {184, 96, 208, 104};
	rcDifficulty.top += gMC.difficulty*8;
	rcDifficulty.bottom += gMC.difficulty*8;
	Rect rcLife = {193, 24, 193, 32};
	Rect rcBr = {193, 32, 193, 40};
	if (CheckFlagTrait(FlagTraits::HUNGER_SYSTEM)){
		rcCase.top += 80;
		rcCase.bottom += 80;
		rcLife.top += 8;
		rcLife.bottom += 8;
	}

	if (flash == true && gMC.shock / 2 % 2)
		return;

	if (gMC.lifeBr < gMC.life)
		gMC.lifeBr = gMC.life;

	if (gMC.lifeBr > gMC.life)
	{
		if (++gMC.lifeBr_count > 30)
			--gMC.lifeBr;
	}
	else
	{
		gMC.lifeBr_count = 0;
	}

	// Draw bar
	rcCase.right = 64;
	float healingRate = 6;
	rcLife.right += (((gMC.life * 40)/*  + (gMC.equip & EQUIP_MIMIGA_MASK && 
		gMC.life < gMC.max_life && gMC.shock == 0 ? (gMC.regenCount / healingRate) : 0) */) / gMC.max_life);
	rcBr.right += (((gMC.lifeBr * 40)) / gMC.max_life);

	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(16), PixelToScreenCoord(40), &rcDifficulty, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(40), PixelToScreenCoord(40), &rcCase, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(40), PixelToScreenCoord(40), &rcBr, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(40), PixelToScreenCoord(40), &rcLife, SURFACE_ID_TEXT_BOX);
	PutNumber4(8, 40, gMC.lifeBr, false);
	}
}

void PutMyScore(int y){
	Rect rcStars = {16, 80, 32, 96};
	Rect rcStairs = {113, 112, 141, 121};
	Rect rcPoints = {81, 73, 109, 80};
	Rect rcMiracle = {228, 0, 252, 16};
	Rect rcTurboText[2] = {{112, 104, 120, 112}, {120, 104, 128, 112}};
	int depthy = 0;
	if (CheckFlagTrait(FlagTraits::DUCK_HEALTH)) depthy = 3;

	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(16), PixelToScreenCoord(0), &rcStars, SURFACE_ID_NPC_SYM);
	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(16), PixelToScreenCoord(48 + depthy), &rcStairs, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(83 + 230), PixelToScreenCoord(5 + y), &rcPoints, SURFACE_ID_TEXT_BOX);
	if (turboTextEnabled)
		PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(0), PixelToScreenCoord(5 + y), &rcTurboText[gCounter % 2], SURFACE_ID_TEXT_BOX);

	PutNumber4(16, 7, gMC.a, true);
	PutNumber4(31, 48 + depthy, gMC.depth, true);
	PutNumber9(116 + 230, 5, gMC.score, true);


	Rect rcIcon;
	int lol = 0;
	std::map<std::string, int> challengeIcons = {{"no_exp", 0}, {"fast_run", 1}, {"no_health", 2}, {"spike_kill", 3},
	{"one_hit", 4}, {"no_placing", 5}, {"no_breaking", 6}, {"timer", 7}, {"no_map", 8}, {"darkness", 9}};
	for (auto icon : challengeIcons){
		if (ChallengeEnabled(icon.first)){
			rcIcon.left = icon.second * 8;
			rcIcon.right = rcIcon.left + 8;
			rcIcon.top = 152;
			rcIcon.bottom = rcIcon.top + 8;

			PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(WINDOW_WIDTH - 12 - (lol * 12)), PixelToScreenCoord(((lol / 8) * 10) + 15), &rcIcon, SURFACE_ID_TEXT_BOX);
			lol++;
		}
	}
	if (gMC.miracle > 0){
		for (int l = 0; l < gMC.miracle; l++)
			PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(WINDOW_WIDTH - 25), PixelToScreenCoord(((lol / 8) * 10) + (lol > 0 ? 33 : 17) + l * 17), &rcMiracle, SURFACE_ID_ITEM_IMAGE);
	}
}

void PutMyAir(int x, int y)
{
	Rect rcAir[2] = {
		{112, 72, 144, 80},
		{112, 80, 144, 88},
	};

	// if (gMC.equip & EQUIP_AIR_TANK && !gMC.hell)
	// 	return;

	if (gMC.air_get != 0)
	{
		// Draw how much air is left
		if (gMC.air_get % 6 < 4)
			PutNumber4(x + 32, y, gMC.air / 10, false);

		// Draw "AIR" text
		if (gMC.air % 30 > 10)
			PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(x), PixelToScreenCoord(y), &rcAir[0], SURFACE_ID_TEXT_BOX);
		else
			PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(x), PixelToScreenCoord(y), &rcAir[1], SURFACE_ID_TEXT_BOX);
	}
}

int time_count;
int global_time_count;

void PutTimeCounter(int x, int y){
	PutTimeCounter(x, y, &time_count);
}

void PutTimeCounter(int x, int y, int* timerVar)
{
	Rect rcTime[3] = {
		{112, 104, 120, 112},
		{120, 104, 128, 112},
		{128, 104, 160, 112},
	};
		// Draw clock and increase time
		if (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED)
		{
			if (*timerVar < 100 * 60 * (gb60fps ? 60 : 50))	// 100 minutes
				++*timerVar;

			if (*timerVar % 30 > 10)
				PutBitmap3(&grcGame, PixelToScreenCoord(x), PixelToScreenCoord(y), &rcTime[0], SURFACE_ID_TEXT_BOX);
			else
				PutBitmap3(&grcGame, PixelToScreenCoord(x), PixelToScreenCoord(y), &rcTime[1], SURFACE_ID_TEXT_BOX);
		}
		else
		{
			PutBitmap3(&grcGame, PixelToScreenCoord(x), PixelToScreenCoord(y), &rcTime[0], SURFACE_ID_TEXT_BOX);
		}

		// Draw time
		if (gb60fps)
		{
			PutNumber4(x,		y, *timerVar / (60 * 60),	false);
			PutNumber4(x + 20,	y, *timerVar / 60 % 60,	true);
			PutNumber4(x + 32,	y, *timerVar / 6 % 10,		false);
		}
		else
		{
			PutNumber4(x,		y, *timerVar / (60 * 50),	false);
			PutNumber4(x + 20,	y, *timerVar / 50 % 60,	true);
			PutNumber4(x + 32,	y, *timerVar / 5 % 10,		false);
		}
		PutBitmap3(&grcGame, PixelToScreenCoord(x + 30), PixelToScreenCoord(y), &rcTime[2], SURFACE_ID_TEXT_BOX);
}

bool SaveTimeCounter(void)
{
	int i;
	unsigned char p[4];
	REC rec;
	FILE *fp;
	std::string path;

	// Get last time
	if (!gMC.randomSeed)
		path = gModulePath + "/score/" + std::to_string(gMC.seed) + ".290.rec";
	else
		path = gModulePath + "/290.rec";

	fp = fopen(path.c_str(), "rb");
	if (fp != NULL)
	{
		// Read data
		rec.counter[0] = File_ReadLE32(fp);
		rec.counter[1] = File_ReadLE32(fp);
		rec.counter[2] = File_ReadLE32(fp);
		rec.counter[3] = File_ReadLE32(fp);
		rec.random[0] = fgetc(fp);
		rec.random[1] = fgetc(fp);
		rec.random[2] = fgetc(fp);
		rec.random[3] = fgetc(fp);
		fclose(fp);

		p[0] = (unsigned char)(rec.counter[0] - rec.random[0]);
		p[1] = (unsigned char)((rec.counter[0] >> 8) - rec.random[0]);
		p[2] = (unsigned char)((rec.counter[0] >> 16) - rec.random[0]);
		p[3] = (unsigned char)((rec.counter[0] >> 24) - rec.random[0] / 2);

		rec.counter[0] = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);

		// If this is faster than our new time, quit
		if (rec.counter[0] < time_count)
			return true;
	}

	// Save new time
	for (i = 0; i < 4; ++i)
	{
		rec.counter[i] = time_count;
		rec.random[i] = Random(0, 250) + i;

		p[0] = (unsigned char)(rec.counter[i] + rec.random[i]);
		p[1] = (unsigned char)((rec.counter[i] >> 8) + rec.random[i]);
		p[2] = (unsigned char)((rec.counter[i] >> 16) + rec.random[i]);
		p[3] = (unsigned char)((rec.counter[i] >> 24) + rec.random[i] / 2);

		rec.counter[i] = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
	}

	fp = fopen(path.c_str(), "wb");
	if (fp == NULL)
		return false;

	File_WriteLE32(rec.counter[0], fp);
	File_WriteLE32(rec.counter[1], fp);
	File_WriteLE32(rec.counter[2], fp);
	File_WriteLE32(rec.counter[3], fp);
	fputc(rec.random[0], fp);
	fputc(rec.random[1], fp);
	fputc(rec.random[2], fp);
	fputc(rec.random[3], fp);

	fclose(fp);
	return true;
}

int LoadTimeCounter(void)
{
	int i;
	unsigned char p[4];
	REC rec;
	FILE *fp;
	std::string path;

	// Open file
	if (!gMC.randomSeed)
		path = gModulePath + "/score/" + std::to_string(gMC.seed) + ".290.rec";
	else
		path = gModulePath + "/290.rec";

	fp = fopen(path.c_str(), "rb");
	if (fp == NULL)
		return 0;

	// Read data
	rec.counter[0] = File_ReadLE32(fp);
	rec.counter[1] = File_ReadLE32(fp);
	rec.counter[2] = File_ReadLE32(fp);
	rec.counter[3] = File_ReadLE32(fp);
	rec.random[0] = fgetc(fp);
	rec.random[1] = fgetc(fp);
	rec.random[2] = fgetc(fp);
	rec.random[3] = fgetc(fp);
	fclose(fp);

	// Decode from checksum
	for (i = 0; i < 4; ++i)
	{
		p[0] = (unsigned char)(rec.counter[i] - rec.random[i]);
		p[1] = (unsigned char)((rec.counter[i] >> 8) - rec.random[i]);
		p[2] = (unsigned char)((rec.counter[i] >> 16) - rec.random[i]);
		p[3] = (unsigned char)((rec.counter[i] >> 24) - rec.random[i] / 2);

		rec.counter[i] = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
	}

	// Verify checksum's result
	if (rec.counter[0] != rec.counter[1] || rec.counter[0] != rec.counter[2])
	{
		global_time_count = 0;
		return 0;
	}

	global_time_count = rec.counter[0];
	return global_time_count;
}
