#include "MyChar.h"

#include <cmath>
#include <cstddef>
#include <cstring>

#include <ranges>

#include <vector>

#include "CommonDefines.h"

#include "ArmsItem.h"
#include "BossLife.h"
#include "Bullet.h"
#include "Caret.h"
#include "Cooldown.h"
#include "Draw.h"
#include "Flags.h"
#include "Flash.h"
#include "Frame.h"
#include "Game.h"
#include "Generic.h"
#include "KeyControl.h"
#include "Log.h"
#include "Main.h"
#include "Map.h"
#include "MycParam.h"
#include "NpChar.h"
#include "NpcHit.h"
#include "NpcTbl.h"
#include "Organya.h"
#include "Sound.h"
#include "Shoot.h"
#include "Stage.h"
#include "Star.h"
#include "TextScr.h"
#include "ValueView.h"

#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))

MYCHAR gMC;
std::vector<int> gAvailableIDs;
std::vector<Cooldown> gCooldowns = {};
std::map<std::pair<short, short>, unsigned char> mapModifications;

nlohmann::ordered_map<std::string, MYCHAR_DEFINITION> availableCharacters;

std::vector<MYCHAR_EQUIP_DEFINITION> equipVisuals;

bool IsDashEquipped(){
	return CheckFlagTrait(FlagTraits::SUPER_DASH) || CheckFlagTrait(FlagTraits::BUFFED_DASH) || CheckFlagTrait(FlagTraits::CAT_DASH) || (gMC.flag & 0x100 && CheckFlagTrait(FlagTraits::UNDERWATER_DASH))
	|| (CheckFlagTrait(FlagTraits::BLADE_DASH) && gArmsData[gSelectedArms].code == 9);
}

int DashDuration(){
	if (gMC.ameDashState) return 2000000;
	float base = 16.0f;
	for (const auto&[flag, modifier] : GetActualFlagTraitData<float>(FlagTraits::DASH_MODIFIER, "duration")){
		base *= std::max(0.0f, modifier);
	}
	if (CheckFlagTrait(FlagTraits::UNDERWATER_DASH) && gMC.flag & 0x100)
		base *= std::max(0.0f, GetFlagTraitData<float>(FlagTraits::UNDERWATER_DASH, "duration"));
	return std::round(base);
}

float DashSpeed(){
	float base = 1.4;
	for (const auto&[flag, modifier] : GetActualFlagTraitData<float>(FlagTraits::DASH_MODIFIER, "speed")){
		base *= std::max(0.0f, modifier);
	}
	if (gMC.ameDashState)
		base *= std::max(0.0f, GetFlagTraitData<float>(FlagTraits::CAT_DASH, "speed"));
	if (CheckFlagTrait(FlagTraits::UNDERWATER_DASH) && gMC.flag & 0x100)
		base *= std::max(0.0f, GetFlagTraitData<float>(FlagTraits::UNDERWATER_DASH, "speed"));
	if (CheckFlagTrait(FlagTraits::BLADE_DASH) && gArmsData[gSelectedArms].code == 9)
		base *= std::max(0.0f, GetFlagTraitData<float>(FlagTraits::BLADE_DASH, "speed"));
	return base;
}

int DashCooldown(){
	float base = 45;
	for (const auto&[flag, modifier] : GetActualFlagTraitData<float>(FlagTraits::DASH_MODIFIER, "cooldown")){
		base *= std::max(0.0f, modifier);
	}
	if (CheckFlagTrait(FlagTraits::UNDERWATER_DASH) && gMC.flag & 0x100)
		base *= std::max(0.0f, GetFlagTraitData<float>(FlagTraits::UNDERWATER_DASH, "cooldown"));
	if (CheckFlagTrait(FlagTraits::BLADE_DASH) && gArmsData[gSelectedArms].code == 9)
		base *= std::max(0.0f, GetFlagTraitData<float>(FlagTraits::BLADE_DASH, "cooldown"));
	if (CheckFlagTrait(FlagTraits::CAT_DASH))
		base *= std::max(0.0f, GetFlagTraitData<float>(FlagTraits::CAT_DASH, "cooldown"));
	return std::round(base);
}

bool IsASheep(){
	return CheckForCooldown(gCooldowns, SHEEP_CURSE_ID) != -1 || ChallengeEnabled("sheepCharacter");
}

void InitMyChar(void)
{
	memset(&gMC, 0, sizeof(MYCHAR));
	gMC.cond = 0x80;
	gMC.direct = 2;

	gMC.view.back = 8 * 0x200;
	gMC.view.top = 8 * 0x200;
	gMC.view.front = 8 * 0x200;
	gMC.view.bottom = 8 * 0x200;

	gMC.hit.back = 5 * 0x200;
	gMC.hit.top = 8 * 0x200;
	gMC.hit.front = 5 * 0x200;
	gMC.hit.bottom = 8 * 0x200;

	gMC.life = 6;
	gMC.max_life = 6;
	gMC.unit = 0;
	if (ChallengeEnabled("debug"))
		gMC.unit = 1;
	gMC.air = 1000;

	gMC.depth = 0;
	gMC.score = 0;

	gMC.dashDuration = 0;
	gMC.dashLock = -1;
	gMC.lifeCapsuleLimit = 0;
	gMC.difficulty = gJConfig.value("difficulty", 0);
	mapModifications.clear();

	gMIMCurrentNum = 0;

	// This is initialized with the values the game uses in vanilla
	gMC.physics_normal.max_dash = 0x32C;
	gMC.physics_normal.max_move = 0x5FF;
	gMC.physics_normal.gravity1 = 0x50;
	gMC.physics_normal.gravity2 = 0x20;
	gMC.physics_normal.dash1 = 0x55;
	gMC.physics_normal.dash2 = 0x20;
	gMC.physics_normal.resist = 0x33;
	gMC.physics_normal.jump = 0x500;

	gMC.physics_underwater.max_dash = gMC.physics_normal.max_dash / 1.9;
	gMC.physics_underwater.max_move = gMC.physics_normal.max_move / 1.75;
	gMC.physics_underwater.gravity1 = gMC.physics_normal.gravity1 / 2;
	gMC.physics_underwater.gravity2 = gMC.physics_normal.gravity2 / 2;
	gMC.physics_underwater.dash1 = gMC.physics_normal.dash1 / 1.9;
	gMC.physics_underwater.dash2 = gMC.physics_normal.dash2 / 1.9;
	gMC.physics_underwater.resist = gMC.physics_normal.resist / 1.9;
	gMC.physics_underwater.jump = gMC.physics_normal.jump / 1.9;

	gMC.no_splash_or_air_limit_underwater = 0;
}

DO_SOMETHING(
	flagSetCallbacks.push_back([](int flag)->void{
		for (const auto&[fl, npcIDs] : GetActualFlagTraitData<std::vector<int>>(FlagTraits::MEDAL_SPAWN, "ids")){
			if (fl == flag){
				for (const auto& npcID: npcIDs){
					if (CountNpCharCode(npcID) == 0)
						SetNpChar(npcID, gMC.x, gMC.y, 0, 0, 0, NULL, 0x100, 0, NPC_IGNORE_SOLIDITY, 0);
				}
			}
		}
	});
	flagUnsetCallbacks.push_back([](int flag)->void{
		for (const auto&[fl, npcIDs] : GetActualFlagTraitData<std::vector<int>>(FlagTraits::MEDAL_SPAWN, "ids")){
			if (fl == flag){
				for (const auto& npcID: npcIDs){
					if (CountNpCharCode(npcID) > 0)
						DeleteNpCharCode(npcID, false);
				}
			}
		}
	});
)

DO_SOMETHING(
	flagSetCallbacks.push_back([](int flag)->void{
		if (CheckFlagTrait(FlagTraits::LITTLE_MAN_SPAWN)) {
			int amount = GetFlagTraitData<int>(FlagTraits::LITTLE_MAN_SPAWN, "amount");
			for (int i = 0; i < amount; i++)
				SetNpChar(180, 0, 0, 0, 0, 2, NULL, 0x100);
		}
		if (CheckFlagTrait(FlagTraits::CURLY_SPAWN) && !GetNPCFlag(GetFlagTraitData<int>(FlagTraits::REVIVAL_FROM_CURLY, "mark_flag", Flags::BADGE_OF_LOVE_REVIVE_PROC)))
			SetNpChar(320, gMC.x, gMC.y, 0, 0, 0, NULL, 0x100, 0, NPC_IGNORE_SOLIDITY);
	});
	flagUnsetCallbacks.push_back([](int flag)->void{
		for (const auto& fl: GetFlagTraitData(FlagTraits::LITTLE_MAN_SPAWN) | std::views::keys){
			if (fl == flag){
				if (CountNpCharCode(180) > 0)
					DeleteNpCharCode(180, false);
			}
		}
		for (const auto& fl: GetFlagTraitData(FlagTraits::CURLY_SPAWN) | std::views::keys){
			if (fl == flag){
				if (CountNpCharCode(320) > 0)
					DeleteNpCharCode(320, false);
			}
		}
	});
)

void AnimationMyChar(bool bKey)
{
	Rect rcLeft[13] = {
		{0, 0, 16, 16},
		{16, 0, 32, 16},
		{0, 0, 16, 16},
		{32, 0, 48, 16},
		{0, 0, 16, 16},
		{48, 0, 64, 16},
		{64, 0, 80, 16},
		{48, 0, 64, 16},
		{80, 0, 96, 16},
		{48, 0, 64, 16},
		{96, 0, 112, 16},
		{112, 0, 128, 16},
		{160, 0, 176, 16}
	};

	Rect rcRight[13] = {
		{0, 16, 16, 32},
		{16, 16, 32, 32},
		{0, 16, 16, 32},
		{32, 16, 48, 32},
		{0, 16, 16, 32},
		{48, 16, 64, 32},
		{64, 16, 80, 32},
		{48, 16, 64, 32},
		{80, 16, 96, 32},
		{48, 16, 64, 32},
		{96, 16, 112, 32},
		{112, 16, 128, 32},
		{160, 16, 176, 32}
	};

	if (gMC.cond & 2)
		return;

	int walkSound = 24;
	if (GetNPCFlag(Flags::PROCGEN_CHARACTER)) walkSound = 34;

	if (gMC.flag & 8)
	{
		if (gMC.cond & 1)
		{
			gMC.ani_no = 11;
		}
		else if (gKey & gKeyUp && gKey & (gKeyLeft | gKeyRight) && bKey)
		{
			gMC.cond |= 4;
			int aniDelay = 4;
			if (GetNPCFlag(Flags::QUATE_CHARACTER)) aniDelay = Random(1, 6);

			if (++gMC.ani_wait > aniDelay)
			{
				gMC.ani_wait = 0;

				if (++gMC.ani_no == 7 || gMC.ani_no == 9)
					PlaySoundObject(walkSound, SOUND_MODE_PLAY);
			}

			if (gMC.ani_no > 9 || gMC.ani_no < 6)
				gMC.ani_no = 6;
		}
		else if (gKey & (gKeyLeft | gKeyRight) && bKey)
		{
			gMC.cond |= 4;
			int aniDelay = 4;
			if (GetNPCFlag(Flags::QUATE_CHARACTER)) aniDelay = Random(1, 6);

			if (++gMC.ani_wait > aniDelay)
			{
				gMC.ani_wait = 0;

				if (++gMC.ani_no == 2 || gMC.ani_no == 4)
					PlaySoundObject(walkSound, SOUND_MODE_PLAY);
			}

			if (gMC.ani_no > 4 || gMC.ani_no < 1)
				gMC.ani_no = 1;
		}
		else if (gKey & gKeyUp && bKey)
		{
			if (gMC.cond & 4)
				PlaySoundObject(24, SOUND_MODE_PLAY);

			gMC.cond &= ~4;
			gMC.ani_no = 5;
		}
		else
		{
			if (gMC.cond & 4)
				PlaySoundObject(walkSound, SOUND_MODE_PLAY);

			gMC.cond &= ~4;
			gMC.ani_no = 0;
		}
	}
	else if (gMC.up)
	{
		gMC.ani_no = 6;
	}
	else if (gMC.down)
	{
		gMC.ani_no = 10;
	}
	else
	{
		if (gMC.ym > 0)
			gMC.ani_no = 1;
		else
			gMC.ani_no = 3;
	}
	if (CheckFlagTrait(FlagTraits::DASH_SPRINT) && (gMC.xm != 0 || gMC.ym != 0) && gKey & gKeyAbility){
		gMC.saraDash = 1;
	}
	if (!(gKey & gKeyAbility) && CheckFlagTrait(FlagTraits::DASH_SPRINT)){
		if (gMC.saraDash > 0)
		{
			gMC.saraDash--;
			if (gMC.saraDash == 0)
			{
				SetCaret(gMC.x, gMC.y, 2, 2);
				SetCaret(gMC.x-0x1000, gMC.y, 2, 2);
				SetCaret(gMC.x+0x1000, gMC.y, 2, 2);
				SetCaret(gMC.x, gMC.y+0x1000, 2, 2);
				SetCaret(gMC.x, gMC.y-0x1000, 2, 2);
				if (gMC.xm > 1300)
					gMC.xm /= 2;
			}
		}
	}
	if (gMC.saraDash && (gMC.xm != 0 || gMC.ym != 0)){
		gMC.ani_no = 12;
		gMC.visualDashDuration = 12;
	}
	if (gMC.ameDashState){
		gMC.ani_no = 12;
		gMC.visualDashDuration = 24;
	}

	if (gMC.direct == 0)
		gMC.rect = rcLeft[gMC.ani_no];
	else
		gMC.rect = rcRight[gMC.ani_no];
}

std::pair<int, int> MyCharCoords(){
	std::pair<int, int> coords {gMC.x, gMC.y};

	if (gMC.cond & 2){
		for (auto& npc: gNPC){
			if (npc.cond & 0x80){
				if ((npc.code_char == 355 && (npc.direct == 0 || npc.direct == 2)) || npc.code_char == 150){
					coords.first = npc.x;
					coords.second = npc.y;
					break;
				}
			}
		}
	}

	return coords;
}

void ShowMyChar(bool bShow)
{
	if (bShow)
		gMC.cond &= ~2;
	else
		gMC.cond |= 2;
}

void PutMyChar(int fx, int fy)
{
	if (IsASheep() && (gMC.cond & 0x80)){
		Rect sheepRect = {48, 0, 64, 16};
		if (gMC.direct == 2) sheepRect = {48, 16, 64, 32};
		PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y - gMC.view.top + 0x200 *3) - SubpixelToScreenCoord(fy), &sheepRect, SURFACE_ID_LEVEL_SPRITESET_1);
	}
	else if (gMC.air <= 0 && gMC.life <= 0){
		Rect rcLeft = {0, 0, 16, 16};
	Rect rcRight = {0, 16, 16, 32};
	int color = 0xaca6ff;
	Rect rect = gMC.direct == 0 ? rcLeft : rcRight;
	rect.top += 32 * gMIMCurrentNum;
	rect.bottom += 32 * gMIMCurrentNum;
	PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y - gMC.view.top) - SubpixelToScreenCoord(fy), &rect, color, SURFACE_ID_DASH_VISUAL);
	for (const auto& equip : equipVisuals)
	{
		if (GetNPCFlag(equip.flag))
		{
			bool skip = false;
			for (const auto&[flag, oof]: GetActualFlagTraitData<std::vector<int>>(FlagTraits::HIDDEN_EQUIPS, "flags")){
				for (const auto& flag : oof)
					if (equip.flag == flag){
						skip = true;
						break;
					}
			}
			if (skip) continue;
			Rect accRect = gMC.rect;
			accRect.top += 32 * equip.accs_id;
			accRect.bottom += 32 * equip.accs_id;
			PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y - gMC.view.top) - SubpixelToScreenCoord(fy), &accRect, color, SURFACE_ID_MY_ACCESSORIES_DROWNED);
		}
	}
	}
	else {

	int arms_offset_y;

	if (!(gMC.cond & 0x80) || gMC.cond & 2)
		return;

	// Draw weapon
	gMC.rect_arms.left = (gArmsData[gSelectedArms].code % 13) * 24;
	gMC.rect_arms.right = gMC.rect_arms.left + 24;
	gMC.rect_arms.top = (gArmsData[gSelectedArms].code / 13) * 96;
	gMC.rect_arms.bottom = gMC.rect_arms.top + 16;
	bool skipWeapon = false;
	for (const auto&[flag, oof]: GetActualFlagTraitData<std::vector<int>>(FlagTraits::HIDDEN_WEAPONS, "ids")){
		for (const auto& id : oof)
			if (gArmsData[gSelectedArms].code == id){
				skipWeapon = true;
				break;
		}
	}
	if (skipWeapon || CountBulletNum(Bullets::MOVING_MISSILE)){
		gMC.rect_arms = {0, 0, 0, 0};
	}

	if (gMC.direct == 2)
	{
		gMC.rect_arms.top += 16;
		gMC.rect_arms.bottom += 16;
	}

	if (gMC.up)
	{
		arms_offset_y = -4;
		gMC.rect_arms.top += 32;
		gMC.rect_arms.bottom += 32;
	}
	else if (gMC.down)
	{
		arms_offset_y = 4;
		gMC.rect_arms.top += 64;
		gMC.rect_arms.bottom += 64;
	}
	else
	{
		arms_offset_y = 0;
	}

	if (gMC.ani_no == 1 || gMC.ani_no == 3 || gMC.ani_no == 6 || gMC.ani_no == 8)
		++gMC.rect_arms.top;

	if (gMC.direct == 0)
		PutBitmap3_Flip(
			&grcGame,
			SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx) - PixelToScreenCoord(8),
			SubpixelToScreenCoord(gMC.y - gMC.view.top) - SubpixelToScreenCoord(fy) + 
			PixelToScreenCoord(arms_offset_y * (gMC.flip ? -1 : 1)),
			&gMC.rect_arms,
			SURFACE_ID_ARMS);
	else
		PutBitmap3_Flip(
			&grcGame,
			SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx),
			SubpixelToScreenCoord(gMC.y - gMC.view.top) - SubpixelToScreenCoord(fy) + PixelToScreenCoord(arms_offset_y * (gMC.flip ? -1 : 1)),
			&gMC.rect_arms,
			SURFACE_ID_ARMS);

	Rect rect = gMC.rect;
	rect.top += 32 * gMIMCurrentNum;
	rect.bottom += 32 * gMIMCurrentNum;

	if (CheckFlagTrait(FlagTraits::RANGED_BLOCK_PLACE) && (gArmsData[gSelectedArms].code == 2 
		|| (gArmsData[gSelectedArms].code == 12 && (CheckFlagTrait(FlagTraits::RANGED_BLOCK_BREAK) || CheckFlagTrait(FlagTraits::MINING_FREEZE_AURA))))){
		Rect limitRect = {160, 112, 176, 128};
		int dist = GetFlagTraitData<int>(FlagTraits::RANGED_BLOCK_PLACE, "distance");
		for (int x = 0; x < gMap.width; x++){
			for (int y = 0; y < gMap.length; y++){
				if (Distance(gMC.x/0x2000, gMC.y/0x2000, x, y) == dist){
					PutBitmap3(&grcGame, PixelToScreenCoord(x * 16 - 8) - SubpixelToScreenCoord(fx),
					PixelToScreenCoord(y * 16 - 8) - SubpixelToScreenCoord(fy), &limitRect, SURFACE_ID_TEXT_BOX);
				}
			}
		}
	}
	
	for (int i = 0; i < gMC.visualDashDuration/4; i++){
		PutBitmap3_Flip(&grcGame, 
		SubpixelToScreenCoord(gMC.x - gMC.view.front - gMC.xm*(1+i*3))
		 - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y - gMC.view.top - gMC.ym*(1+i*3)) - SubpixelToScreenCoord(fy), &rect, (SurfaceID)((int)SURFACE_ID_DASH_VISUAL), 0.33f - 0.32f * (float(i) / (gMC.visualDashDuration / (float)4)));
	auto hiddenItems = GetFlagTraitData(FlagTraits::HIDDEN_EQUIPS);
	for (const auto& equip : equipVisuals)
	{
		if (GetNPCFlag(equip.flag))
		{
			bool skip = false;
			for (const auto&[flag, oof]: GetActualFlagTraitData<std::vector<int>>(FlagTraits::HIDDEN_EQUIPS, "flags")){
				for (const auto& flag : oof)
					if (equip.flag == flag){
						skip = true;
						break;
					}
			}
			if (skip) continue;
			Rect accRect = gMC.rect;
			accRect.top += 32 * equip.accs_id;
			accRect.bottom += 32 * equip.accs_id;
			PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y - gMC.view.top) - SubpixelToScreenCoord(fy), &accRect, SURFACE_ID_MY_ACCESSORIES_DASH, 0.33f - 0.32f * (float(i) / (gMC.visualDashDuration / (float)4)));
		}
	}
	}

	if (gMC.shock / 2 % 2 && !CountBulletNum(Bullets::MISSILE_EXPLOSION))
		return;

	if (CountBulletNum(Bullets::MOVING_MISSILE))
		return;

	// Draw player
	rect = gMC.rect;
	rect.top += 32 * gMIMCurrentNum;
	rect.bottom += 32 * gMIMCurrentNum;
#ifndef ENABLE_MIM_DISABLE_EQUIP_40_GRAPHICS
#endif
	PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y - gMC.view.top) - SubpixelToScreenCoord(fy), &rect, SURFACE_ID_MY_CHAR);

	for (const auto& equip : equipVisuals)
	{
		if (GetNPCFlag(equip.flag))
		{
			Rect accRect = gMC.rect;
			accRect.top += 32 * equip.accs_id;
			accRect.bottom += 32 * equip.accs_id;
			PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y - gMC.view.top) - SubpixelToScreenCoord(fy), &accRect, SURFACE_ID_MY_ACCESSORIES);
		}
	}

	++gMC.bubble;

	// Draw jump charge
	Rect rcJumpCharge[4] = {
		{176, 128, 192, 144},
		{192, 128, 208, 144},
		{208, 128, 224, 144},
		{224, 128, 240, 144},
	};
	
	if (gMC.chargeCD >= 30)
		PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x) - PixelToScreenCoord(8) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y) - PixelToScreenCoord(8) - SubpixelToScreenCoord(fy), &rcJumpCharge[gMC.bubble % 4], SURFACE_ID_CARET);

	// Draw air tank
	Rect rcBubble[2] = {
		{56, 96, 80, 120},
		{80, 96, 104, 120},
	};
	
	if (CheckFlagTrait(FlagTraits::UNDERWATER_BUBBLE_VFX) && gMC.flag & 0x100)
		PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x) - PixelToScreenCoord(12) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y) - PixelToScreenCoord(12) - SubpixelToScreenCoord(fy), &rcBubble[(gMC.bubble / 2) % 2], SURFACE_ID_CARET);
	else if (gMC.unit == 1)
		PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x) - PixelToScreenCoord(12) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y) - PixelToScreenCoord(12) - SubpixelToScreenCoord(fy), &rcBubble[(gMC.bubble / 2) % 2], SURFACE_ID_CARET);
	}
	if (ChallengeEnabled("hitboxes"))
	{
		int size = 0x200 * 3;
		if (CheckFlagTrait(FlagTraits::WITCH_HITBOX))
		{
			size = 0x200;
		}
		int hitX = gMC.x - size - fx;
		int hitY = gMC.y - size - fy;

		Rect hitbox = {
			hitX / 0x200,
			hitY / 0x200,
			(hitX + size * 2) / 0x200,
			(hitY + size * 2) / 0x200,
		};
		CortBox2(&hitbox, 0x7F0000FF);
	}
}

static int bloodTimer = 0;
static bool amePressedBefore = false;
static int ameKeyPressed;

void ActMyChar_Normal(bool bKey)
{
	// Get speeds and accelerations
	int max_move;
	int max_dash;
	int gravity1;
	int gravity2;
	int jump;
	int dash1;
	int dash2;
	int resist;

	int a, x;

	MYCHAR_PHYSICS *physics;

	if (gMC.cond & 2)
		return;

	physics = (gMC.flag & 0x100) ? &gMC.physics_underwater : &gMC.physics_normal;
	if (CheckFlagTrait(FlagTraits::UNHINDERED_DIVING)){
		physics = &gMC.physics_normal;
	}
	
	max_dash = physics->max_dash;
	gravity1 = physics->gravity1;
	gravity2 = physics->gravity2;
	dash1 = physics->dash1;
	dash2 = physics->dash2;
	resist = physics->resist;
	jump = physics->jump;
	// Limit speed
	if (gMC.flag & 0x100 && !(gMC.flag & (0x8000 | 0x4000 | 0x2000 | 0x1000)))
		max_move = gMC.physics_underwater.max_move;	// Underwater or in wind
	else
		max_move = gMC.physics_normal.max_move;	// Normal conditions
	if (CheckFlagTrait(FlagTraits::UNHINDERED_DIVING))
		max_move = gMC.physics_normal.max_move;
	if ((gMC.flag & 0x100) && CheckFlagTrait(FlagTraits::SWIM_BOOST)){
		jump = gMC.physics_normal.jump*std::max(0.0f, GetFlagTraitData<float>(FlagTraits::SWIM_BOOST, "modifier"));
	} else {
		if (CheckFlagTrait(FlagTraits::JUMP_BOOST)){
			jump = gMC.physics_normal.jump*std::max(0.0f, GetFlagTraitData<float>(FlagTraits::JUMP_BOOST, "modifier"));
		}
	}
	if (CheckFlagTrait(FlagTraits::UNLIMITED_SWIM)){
		if (gMC.flag & 0x100)
			gravity2 = 0;
	}
	if (CheckFlagTrait(FlagTraits::LAST_STAND) && gMC.life == 0){
		float speedBoost = GetFlagTraitData<float>(FlagTraits::LAST_STAND, "speed_boost");
		dash1 *= speedBoost;
		dash2 *= speedBoost;
		max_dash *= speedBoost;
	}
	if (GetNPCFlag(Flags::QUATE_CHARACTER)){
		float jumpHeight = float(Random(125, 300)) / 100.0f;
		max_dash *= float(Random(30, 300)) / 100.0f;
		gravity1 *= float(Random(70, 160)) / 100.0f;
		gravity2 /= jumpHeight * 3;
		dash1 *= float(Random(10, 400)) / 100.0f;
		dash2 *= float(Random(10, 400)) / 100.0f;
		resist *= float(Random(0, 300)) / 100.0f;
		jump *= jumpHeight;
		max_move *= float(Random(20, 350)) / 100.0f;
	}
	if (GetNPCFlag(Flags::MAHIN_CHARACTER)){
		dash1 *= 1.5;
		dash2 *= 1.5;
		max_dash *= 1.5;
		max_move *= 1.2;
		resist *= 1.5;
		jump *= 1.25;
		gravity2 *= 0.85;
		gravity1 *= 1.6;
		if (gKey & gKeyAbility)
			max_dash *= 2;
	}
	if (GetNPCFlag(Flags::PROCGEN_CHARACTER)){
		jump *= 1.1;
		dash1 *= 1.6;
		dash2 *= 0.75;
		resist *= 0.5;
		gravity1 *= 2;
		gravity2 *= 0.8;
	}
	if (gMC.tileset == TYPE_ICE){
		if (!CheckFlagTrait(FlagTraits::NO_SLIPPERY)){
			resist /= 2;
			dash1 *= 0.9;
		}
		if (gMC.flag & 0x100){
			max_dash *= 0.75;
			dash1 *= 0.75;
			dash2 *= 0.75;
			jump *= 0.8;
			max_move *= 0.9;
		}
	}
	if (gMC.tileset == TYPE_SPACESTATION){
		gravity1 *= 0.66;
		gravity2 *= 0.66;
	}
	if (CheckFlagTrait(FlagTraits::DASH_SPRINT) && (gKey & gKeyAbility))
		max_dash *= 2;
	if (CheckFlagTrait(FlagTraits::AMPHIBIOUS))
		gMC.no_splash_or_air_limit_underwater = 1;
	bool miningLaserCharge = gKey & gKeyDown && gArmsData[gSelectedArms].code == 18 && gArmsData[gSelectedArms].exp > 0;
	if (miningLaserCharge){
		max_dash /= 1.5;
		dash1 /= 1.5;
		dash2 /= 1.5;
		jump /= 1.5;
		resist /= 1.5;
		gravity2 /= 1.5;
		gravity1 /= 1.5;
	}
	double mod = 1;
	ModifyWithChallengeValueReal(&mod, "movement_speed");
	double mod2 = 1;
	ModifyWithChallengeValueReal(&mod2, "jump_speed");
	dash1 *= mod;
	dash2 *= mod;
	resist *= mod;
	max_dash *= mod;
	max_move *= mod;
	jump *= mod2;
	if (gMC.flip){
		gravity1 *= -1;
		gravity2 *= -1;
		jump *= -1;
	}

	if (GetNPCFlag(Flags::TOROKO_BLEEDING_OUT)){
		if (bloodTimer == 0){
			bloodTimer = Random(3, 8);
			SetCaret(gMC.x, gMC.y, 11, 0);
		}
		else bloodTimer--;
	}

	if (CheckFlagTrait(FlagTraits::ICE_SKATES)){
		if (gKey & gKeyAbility){
			dash1 = dash2;
		}
	}

	if (CheckFlagTrait(FlagTraits::MINING_FREEZE_AURA) && (gArmsData[gSelectedArms].code == 2 || gArmsData[gSelectedArms].code == 12)){
		for (int i = 0; i < NPC_MAX; i++){
			if (Random(0, 10) == 0 && std::find(etherealEntities.begin(), etherealEntities.end(), gNPC[i].code_char) == etherealEntities.end() && gNPC[i].damage > 0 && 
			Distance(gMC.x, gMC.y, gNPC[i].x, gNPC[i].y) <= GetFlagTraitData<int>(FlagTraits::MINING_FREEZE_AURA, "distance") * 1_pix){
				gNPC[i].freeze += GetFlagTraitData<int>(FlagTraits::MINING_FREEZE_AURA, "rate");
			}
		}
	}

	// Don't create "?" effect
	gMC.ques = false;

	// If can't control player, stop boosting
	if (!bKey)
		gMC.boost_sw = 0;

	// Movement on the ground
	if (gMC.flag & 8 || gMC.flag & 0x10 || gMC.flag & 0x20)
	{
		// Stop boosting and refuel
		gMC.boost_sw = 0;
		gMC.pogo = true;
		gMC.chargeActive = false;
		for (const auto&[_, amount]: GetActualFlagTraitData<int>(FlagTraits::CAN_BOOST, "amount")){
			if (amount > gMC.boost_cnt) gMC.boost_cnt = amount;
		}
		float boostAmount = 1;
		for (const auto&[_, amount]: GetActualFlagTraitData<float>(FlagTraits::BOOSTER_FUEL_BOOST, "amount")){
			auto activeFlags = GetTraitActiveFlags(FlagTraits::BOOSTER_FUEL_BOOST);
			if (std::ranges::find(activeFlags, amount) == activeFlags.end()) boostAmount *= amount;
		}
		gMC.boost_cnt *= boostAmount;
		if (CheckFlagTrait(FlagTraits::GRAVITY_FLIP))
			ClearCooldown(gCooldowns, GRAVITY_FLIP_ID);

		// Move in direction held
		if (bKey)
		{
			if (gKeyTrg == gKeyDown && gKey == gKeyDown && !(gMC.cond & 1) && !(g_GameFlags & GAME_FLAG_IS_GAMEPLAY_NOT_POSSIBLE))
			{
				gMC.cond |= 1;
				gMC.ques = true;
			}
			else if (gKey == gKeyDown)
			{
				// There probably used to be commented-out code here
			}
			else
			{
				int maximal_dash = max_dash;
				int dashy = dash1;
				if (CheckFlagTrait(FlagTraits::ICE_SKATES) && gKey & gKeyAbility){
					maximal_dash = 1500;
				}
				if (CheckFlagTrait(FlagTraits::SPEED_BOOTS)){
					maximal_dash *= std::max(0.0f, GetFlagTraitData<float>(FlagTraits::SPEED_BOOTS, "speed_multiplier"));
					dashy *= std::max(0.0f, GetFlagTraitData<float>(FlagTraits::SPEED_BOOTS, "acceleration"));
				}
				if (gKey & gKeyLeft && gMC.xm > -maximal_dash){
					if (gMC.xm > 0){
						gMC.xm -= dashy*GetFlagTraitData<int>(FlagTraits::TIGHTER_TURNING, "effect");
					}
					gMC.xm -= dashy;
					if (gMC.saraDash > 0 && gMC.xm > -500){
						gMC.xm = -500;
					}
				}
				if (gKey & gKeyRight && gMC.xm < maximal_dash){
					if (gMC.xm < 0) gMC.xm += dashy*GetFlagTraitData<int>(FlagTraits::TIGHTER_TURNING, "effect");
					gMC.xm += dashy;
					if (gMC.saraDash > 0 && gMC.xm < 500){
						gMC.xm = 500;
					}
				}

				if(!(gKey & gKeyAbility && CheckFlagTrait(FlagTraits::STRAFE))){
					if (gKey & gKeyLeft){
						gMC.direct = 0;
					}
						
					if (gKey & gKeyRight){
						gMC.direct = 2;
					}
				}

				if (CheckFlagTrait(FlagTraits::SPEED_BOOTS)){
					float threshold = std::max(1.0f, GetFlagTraitData<float>(FlagTraits::SPEED_BOOTS, "particle_threshold"));
					if (gMC.xm <= -max_dash*threshold || gMC.xm >= max_dash*threshold)
					{
						static int speedEffects;
						if (++speedEffects % 4 == 1)
						{
							if (gMC.direct == 0)
								SetCaret(gMC.x + (1 * 0x200), gMC.y + (6 * 0x200), 7, 2);
							if (gMC.direct == 2)
								SetCaret(gMC.x - (1 * 0x200), gMC.y + (6 * 0x200), 7, 0);

							PlaySoundObject(113, SOUND_MODE_PLAY);
						}
					}
				}

			}
		}

		// Friction
		if (!(gMC.cond & 0x20))
		{
			if (!(CheckFlagTrait(FlagTraits::ICE_SKATES) && gKey & gKeyAbility))
			{
				if (gMC.xm < 0)
				{
					if (gMC.xm > -resist)
						gMC.xm = 0;
					else
						gMC.xm += resist;
				}
				if (gMC.xm > 0)
				{
					if (gMC.xm < resist)
						gMC.xm = 0;
					else
						gMC.xm -= resist;
				}
			}
		}
	}
	else
	{
		// Start boosting
		if (bKey)
		{
			if (CheckFlagTrait(FlagTraits::CAN_BOOST) && gKeyTrg & gKeyJump &&
			 (gMC.boost_cnt != 0) && !(gMC.flag & 0x100) && (!gMC.pogo || !CheckFlagTrait(FlagTraits::DOUBLE_JUMP)))
			{
				if (CheckFlagTrait(FlagTraits::BOOSTER_0_8))
				{
					gMC.boost_sw = 1;

					if (gMC.ym > 0x30)
						gMC.ym /= 2;
				} else if (CheckFlagTrait(FlagTraits::BOOSTER_2_0))
				{
					if (gKey & gKeyUp)
					{
						gMC.boost_sw = 2;
						gMC.xm = 0;
						gMC.ym = -0x5FF;
					}
					else if (gKey & gKeyLeft)
					{
						gMC.boost_sw = 1;
						gMC.ym = 0;
						gMC.xm = -0x5FF;
					}
					else if (gKey & gKeyRight)
					{
						gMC.boost_sw = 1;
						gMC.ym = 0;
						gMC.xm = 0x5FF;
					}
					else if (gKey & gKeyDown)
					{
						gMC.boost_sw = 3;
						gMC.xm = 0;
						gMC.ym = 0x5FF;
					}
					else
					{
						gMC.boost_sw = 2;
						gMC.xm = 0;
						gMC.ym = -0x5FF;
					}
				}
			}

			// Move left and right
			int maximal_dash = max_dash;
			int dashy = dash2;
			if (gKey & gKeyJump && GetNPCFlag(Flags::PROCGEN_CHARACTER) && gArmsData[gSelectedArms].code == 19 && gArmsData[gSelectedArms].num > 0){
				dash2 *= 4;
			}
			if (bKey && gKey & gKeyAbility && CheckFlagTrait(FlagTraits::GLIDING) && gMC.ym > 0x080){
				maximal_dash *= GetFlagTraitData<float>(FlagTraits::GLIDING, "max_speed");
				dashy *= GetFlagTraitData<float>(FlagTraits::GLIDING, "acceleration");
			}
			if (CheckFlagTrait(FlagTraits::SPEED_BOOTS)){
				maximal_dash *= std::max(0.0f, GetFlagTraitData<float>(FlagTraits::SPEED_BOOTS, "air_speed_multiplier", 1.0f));
				dashy *= std::max(0.0f, GetFlagTraitData<float>(FlagTraits::SPEED_BOOTS, "air_acceleration", 1.0f));
			}
			if (gKey & gKeyLeft && gMC.xm > -maximal_dash){
				if (gMC.xm > 0) gMC.xm -= dash2*GetFlagTraitData<int>(FlagTraits::TIGHTER_TURNING, "effect");
				gMC.xm -= dash2;
				if (gMC.saraDash > 0 && gMC.xm > -500){
					gMC.xm = -500;
				}
			}
			if (gKey & gKeyRight && gMC.xm < maximal_dash){
				if (gMC.xm < 0) gMC.xm += dash2*GetFlagTraitData<int>(FlagTraits::TIGHTER_TURNING, "effect");
				gMC.xm += dash2;
				if (gMC.saraDash > 0 && gMC.xm < 500){
					gMC.xm = 500;
				}
			}

			if(!(gKey & gKeyAbility && CheckFlagTrait(FlagTraits::STRAFE))){
					if (gKey & gKeyLeft){
						gMC.direct = 0;
					}
						
					if (gKey & gKeyRight){
						gMC.direct = 2;
					}
				}
		}

		// Slow down when stopped boosting (Booster 2.0)
		if (CheckFlagTrait(FlagTraits::CAN_BOOST) && CheckFlagTrait(FlagTraits::BOOSTER_2_0) && gMC.boost_sw != 0 && !(gKey & gKeyJump))
		{
			if (gMC.boost_sw == 1)
				gMC.xm /= 2;
			else if (gMC.boost_sw == 2)
				gMC.ym /= 2;
		}
		if (CheckFlagTrait(FlagTraits::CAN_BOOST) && CheckFlagTrait(FlagTraits::BOOSTER_2_0) && gMC.boost_sw != 0 && (gMC.boost_cnt == 0) && !CheckFlagTrait(FlagTraits::DONT_SLOW_ON_BOOST_END))
		{
			if (gMC.boost_sw == 1)
				gMC.xm /= 2;
			else if (gMC.boost_sw == 2)
				gMC.ym /= 2;
		}

		// Stop boosting
		if ((gMC.boost_cnt == 0) || !(gKey & gKeyJump))
			gMC.boost_sw = 0;
		
		if (gKey & gKeyJump && gMC.boost_sw == 0 && gArmsData[gSelectedArms].code == 19 && gArmsData[gSelectedArms].num > 0)
			gMC.boost_sw = 1;
	}

	// Jumping
	if (bKey)
	{
		// Look up and down
		if (gKey & gKeyUp)
			gMC.up = true;
		else
			gMC.up = false;

		if (gKey & gKeyDown && !(gMC.flag & 8))
			gMC.down = true;
		else
			gMC.down = false;

		if (gKeyTrg & gKeyJump && (gMC.flag & 8 || gMC.flag & 0x10 || gMC.flag & 0x20 || 
					(gMC.flag & 0x100)
					|| (CheckFlagTrait(FlagTraits::JUMP_ON_SPIKES) && gMC.pogo && GetAttribute(round(((float)gMC.x) / 0x2000), round(((float)gMC.y + 0x200 * 15) / 0x2000)) == 66)
					|| (CheckFlagTrait(FlagTraits::WALL_JUMP) && (GetAttribute(round(((float)gMC.x) / 0x2000) + 1, round(((float)gMC.y) / 0x2000)) == 65)
					|| GetAttribute(round(((float)gMC.x) / 0x2000) - 1, round(((float)gMC.y) / 0x2000)) == 65)
					 || ((CheckFlagTrait(FlagTraits::DOUBLE_JUMP) || gMC.saraDash) && gMC.pogo)))
		{
			if (gMC.flag & 0x2000)
			{
				// Another weird empty case needed for accurate assembly.
				// There probably used to be some commented-out code here.
			}
			else
			{
				PlaySoundObject(15, SOUND_MODE_PLAY);
				if ((CheckFlagTrait(FlagTraits::JUMP_ON_SPIKES) && GetAttribute(round(((float)gMC.x) / 0x2000), round(((float)gMC.y + 0x200 * 15) / 0x2000)) == 66)){
					gMC.pogo = false;
					gMC.ym -= jump / 2;
				}
				else if (CheckFlagTrait(FlagTraits::WALL_JUMP) && GetAttribute(round(((float)gMC.x) / 0x2000) + 1, round(((float)gMC.y) / 0x2000)) == 65 &&
				 !(gMC.flag & 8 || gMC.flag & 0x10 || gMC.flag & 0x20 || gMC.flag & 0x100)){
					gMC.xm -= max_dash;
					gMC.direct = 0;
					gMC.pogo = true;
				} else if (CheckFlagTrait(FlagTraits::WALL_JUMP) && GetAttribute(round(((float)gMC.x) / 0x2000) - 1, round(((float)gMC.y) / 0x2000)) == 65 &&
				 !(gMC.flag & 8 || gMC.flag & 0x10 || gMC.flag & 0x20 || gMC.flag & 0x100)){
					gMC.xm += max_dash;
					gMC.direct = 2;
					gMC.pogo = true;
				}
				else if ((CheckFlagTrait(FlagTraits::DOUBLE_JUMP) || gMC.saraDash) && gMC.pogo == true && !(gMC.flag & 8 || gMC.flag & 0x10 || gMC.flag & 0x20 || gMC.flag & 0x100) ){
					gMC.pogo = false;
					SetDestroyNpChar(gMC.x, gMC.y, gMC.view.back, 5);
				}
				gMC.ym = -jump;
				if (gKey & gKeyUp && gMC.chargeCD >= 25){
					gMC.ym -= jump;
					gMC.chargeCD = 0;
					gMC.chargeActive = true;
					PlaySoundObject(30, SOUND_MODE_PLAY);
				}
			}
		}
	}

	// Stop interacting when moved
	if (bKey && gKey & (gKeyLeft | gKeyRight | gKeyUp | gKeyJump | gKeyShot))
		gMC.cond &= ~1;

	// Booster losing fuel
	if (gMC.boost_sw != 0 && gMC.boost_cnt != 0)
		--gMC.boost_cnt;

	// Wind / current forces
	if (gMC.flag & 0x1000)
		gMC.xm -= 0x88;
	if (gMC.flag & 0x2000)
		gMC.ym -= 0x80;
	if (gMC.flag & 0x4000)
		gMC.xm += 0x88;
	if (gMC.flag & 0x8000)
		gMC.ym += 0x55;

	// Booster 2.0 forces and effects
	if (CheckFlagTrait(FlagTraits::BOOSTER_2_0) && gMC.boost_sw != 0 && !(gMC.flag & 0x100))
	{
		if (gMC.boost_sw == 1)
		{
			// Go up when going into a wall
			if (gMC.flag & 5)
				gMC.ym = -0x100;

			// Move in direction facing
			if (gMC.direct == 0)
				gMC.xm -= 0x20;
			if (gMC.direct == 2)
				gMC.xm += 0x20;

			// Boost particles (and sound)
			if (gKeyTrg & gKeyJump || gMC.boost_cnt % 3 == 1)
			{
				const int boostPart = CheckFlagTrait(FlagTraits::BOOSTER_DAMAGE) ? 19 : 7;
				if (gMC.direct == 0){
					SetCaret(gMC.x + (2 * 0x200), gMC.y + (2 * 0x200), boostPart, 2);
				}
				if (gMC.direct == 2){
					SetCaret(gMC.x - (2 * 0x200), gMC.y + (2 * 0x200), boostPart, 0);
				}

				PlaySoundObject(113, SOUND_MODE_PLAY);
			}
		}
		else if (gMC.boost_sw == 2)
		{
			// Move upwards
			gMC.ym -= 0x20;

			// Boost particles (and sound)
			if (gKeyTrg & gKeyJump || gMC.boost_cnt % 3 == 1)
			{
				const int boostPart = CheckFlagTrait(FlagTraits::BOOSTER_DAMAGE) ? 19 : 7;
				SetCaret(gMC.x, gMC.y + (6 * 0x200), boostPart, 3);
				PlaySoundObject(113, SOUND_MODE_PLAY);
			}
		}
		else if (gMC.boost_sw == 3 && (gKeyTrg & gKeyJump || gMC.boost_cnt % 3 == 1))
		{
			// Boost particles (and sound)
			const int boostPart = CheckFlagTrait(FlagTraits::BOOSTER_DAMAGE) ? 19 : 7;
			SetCaret(gMC.x, gMC.y - (6 * 0x200), boostPart, 1);
			PlaySoundObject(113, SOUND_MODE_PLAY);
		}
	}
	// Upwards wind/current
	else if (gMC.flag & 0x2000)
	{
		gMC.ym += gravity1;
	}
	// Booster 0.8
	else if ((CheckFlagTrait(FlagTraits::BOOSTER_0_8) && !CheckFlagTrait(FlagTraits::BOOSTER_2_0) && gMC.boost_sw != 0 && !(gMC.flag & 0x100)))
	{
		// Booster particles
		if (gMC.boost_cnt % 3 == 0)
		{
			SetCaret(gMC.x, gMC.y + (gMC.hit.bottom / 2), 7, 3);
			PlaySoundObject(113, SOUND_MODE_PLAY);
		}

		if (gMC.ym > -0x600){
			// Upwards force
			gMC.ym -= 0x60;
		}

		// Bounce off of ceiling
		if (gMC.flag & 2)
			gMC.ym = 0x270;
	}
	// Gravity while jump is held
	else if (((gMC.flip && gMC.ym > (CheckFlagTrait(FlagTraits::WITCH_ASCENDING) ? 0x400 : 0)) || gMC.ym < (CheckFlagTrait(FlagTraits::WITCH_ASCENDING) ? 0x400 : 0)) && bKey && gKey & gKeyJump)
	{
		if (gMC.saraDash > 0 && abs(gMC.xm) > 900) gravity2 /= 3;
		if (gKey & gKeyAbility && CheckFlagTrait(FlagTraits::ICE_SKATES) && abs(gMC.xm) >= max_dash + 275){
			gravity2 /= 2;
		}
		if (gKey & gKeyUp && gMC.chargeActive) {
			gravity2 *= 0.66;
			if (CheckFlagTrait(FlagTraits::JUMP_BOOST)){
				gravity2 *= 1.0f / std::max(0.0f, GetFlagTraitData<float>(FlagTraits::JUMP_BOOST, "charge_modifier"));
			}
		}
		gMC.ym += gravity2;
		if (CheckFlagTrait(FlagTraits::VELOCITY_BASED_JUMP)){
			gMC.ym -= GetFlagTraitData<float>(FlagTraits::VELOCITY_BASED_JUMP, "modifier") * abs(gMC.xm);
		}
	}
	// Normal gravity
	else
	{
		if (CheckFlagTrait(FlagTraits::WATER_FLOATING) && !(bKey && gKey & gKeyAbility) && gMC.flag & 0x100){
			gMC.ym -= gravity1*2;
		}
		else{
			gMC.ym += gravity1;
		}
	}
	// if (bKey && gKey & gKeyJump && gMC.equip & EQUIP_WINGS && gMC.ym > 0){
	// 	gMC.ym -= gravity1 * 0.645;
	// }
	// if ((gMC.equip & EQUIP_HEAVY_WEIGHT) && (bKey && gKey & gKeyAbility)){
	// 	gMC.ym = max_dash*3;
	// }
	if (CheckFlagTrait(FlagTraits::HOOK_FLAIL) && (CountBulletNum(Bullets::ENERGY_FLAIL_LVL_1) || CountBulletNum(Bullets::ENERGY_FLAIL_LVL_2) || CountBulletNum(Bullets::ENERGY_FLAIL_LVL_3))){
		for (int i = 0; i < BULLET_MAX; ++i)
		if (gBul[i].cond & 0x80 && (gBul[i].code_bullet + 2) / 3 == 17 && gBul[i].act_no == 3){
			 if (gKeyTrg & gKeyJump){
				gBul[i].cond = 0;
				if (!(gMC.flag & 0xF)){
				gMC.xm += gBul[i].xm;
				gMC.ym += gBul[i].ym;
				if (gBul[i].xm < 0x200)
					gMC.ym -= jump;
				}
			 } else {
				 gMC.xm = 0;
				 gMC.ym = 0;
			 }
		}
	}

	// Keep player on slopes
	if (!bKey || !(gKeyTrg & gKeyJump))
	{
		if (gMC.flag & 0x10 && gMC.xm < 0)
			gMC.ym = -gMC.xm;
		if (gMC.flag & 0x20 && gMC.xm > 0)
			gMC.ym = gMC.xm;
		if (gMC.flag & 8 && gMC.flag & 0x80000 && gMC.xm < 0)
			gMC.ym = 0x400;
		if (gMC.flag & 8 && gMC.flag & 0x10000 && gMC.xm > 0)
			gMC.ym = 0x400;
		if (gMC.flag & 8 && gMC.flag & 0x20000 && gMC.flag & 0x40000)
			gMC.ym = 0x400;
	}

	if (gKeyTrg & gKeyAbility && CheckFlagTrait(FlagTraits::GRAVITY_FLIP) && 
			CheckForCooldown(gCooldowns, GRAVITY_FLIP_ID) == -1){
		PlaySoundObject(112, SOUND_MODE_PLAY);
		SetCaret(gMC.x, gMC.y, 2, 2);
		if (!gMC.flip)
			gMC.flip = true;
		else
			gMC.flip = false;
		InitiateCooldown(gCooldowns, GRAVITY_FLIP_ID);
	}

	if (0)
	{
		// There used to be an if-statement here that didn't do anything, but the compiler optimised it out.
		// We only know this was here because empty if-statements affect the register usage.
		// Since there's no code, we have no idea what the original condition actually was.
	}

	if (bKey && gKey & gKeyAbility && CheckFlagTrait(FlagTraits::GLIDING) && gMC.ym > 0 && !(gMC.flag & 8 || gMC.flag & 0x10 || gMC.flag & 0x20)){
		SetCaret(gMC.x, gMC.y + 0x200*4, 13, 0);
	}
	if (gMC.ym > max_move * 0.75 && CheckFlagTrait(FlagTraits::CAT_FLOAT)){
		gMC.ym = max_move * 0.75;
	}
	if (gMC.tileset == TYPE_SPACESTATION && gMC.ym > max_move*0.7){
		gMC.ym = max_move*0.7;
	}
	if (gMC.saraDash > 0 && gMC.ym * (gMC.flip ? -1 : 1) > max_move / 3 && gKey & gKeyJump){
		gMC.ym = max_move / 3 * (gMC.flip ? -1 : 1);
	}
	if (bKey && gKey & gKeyAbility && CheckFlagTrait(FlagTraits::GLIDING) && gMC.ym  * (gMC.flip ? -1 : 1) > max_move / GetFlagTraitData<float>(FlagTraits::GLIDING, "fall_slowdown")){
		gMC.ym = max_move / GetFlagTraitData<float>(FlagTraits::GLIDING, "fall_slowdown") * (gMC.flip ? -1 : 1);
	}
	if (CheckFlagTrait(FlagTraits::GLIDING_AFTER_DASH) && gMC.ym > max_move * GetFlagTraitData<float>(FlagTraits::GLIDING_AFTER_DASH, "speed_multiplier") && CheckForCooldown(gCooldowns, DASH_TIMER_ID) >= DashCooldown() * GetFlagTraitData<float>(FlagTraits::GLIDING_AFTER_DASH, "duration")){
		gMC.ym = max_move * GetFlagTraitData<float>(FlagTraits::GLIDING_AFTER_DASH, "speed_multiplier");
	}
	if (gMC.ym > max_move && !(CheckFlagTrait(FlagTraits::ACCELERATE_FALL) && gKey & gKeyAbility)){
		gMC.ym = max_move;
	}
	if (CheckFlagTrait(FlagTraits::ACCELERATE_FALL) && (bKey && gKey & gKeyAbility) && !(gMC.flag & 8)){
		gMC.ym = max_move*1.75 * (gMC.flip ? -1 : 1);
		if (gMC.ym > 0x100* (gMC.flip ? -1 : 1)){
			gMC.visualDashDuration+=3;
			if (gMC.visualDashDuration > 12){
				gMC.visualDashDuration = 12;
			}
		}
	}

	if (bKey && ((gKeyTrg & gKeyDash)) && 
		CheckForCooldown(gCooldowns, DASH_TIMER_ID) == -1
		 && gMC.dashDuration == 0){
		InitiateCooldown(gCooldowns, DASH_TIMER_ID);
		gMC.dashDuration = 1;
		gMC.visualDashDuration = 1;
		gMC.dashLock = gMC.direct;
		if (CheckFlagTrait(FlagTraits::CAT_DASH)){
			gMC.ameDashState = true;
		}
		if (CheckFlagTrait(FlagTraits::BLADE_DASH) && gArmsData[gSelectedArms].code == 9)
		{
			int bul_no;
			int level = gArmsData[gSelectedArms].level;

			if (CountArmsBullet(9) > (level == 3 ? 1 : 0))
				return;

			switch (level)
			{
			case 1:
				bul_no = 25;
				break;

			case 2:
				bul_no = 27;
				break;

			case 3:
				bul_no = 27;
				break;
			}
				if (gMC.dashLock == 0)
					SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y - (3 * 0x200), 0);
				else
					SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y - (3 * 0x200), 2);
			PlaySoundObject(34, SOUND_MODE_PLAY);
		}
	}
	if (gMC.dashDuration > 0 && gMC.dashDuration < DashDuration()){
		if (!CheckFlagTrait(FlagTraits::TELEPORT_DASH) || gBL.flag)
		{
			gMC.dashDuration++;
			gMC.visualDashDuration++;
			if (CheckFlagTrait(FlagTraits::DASH_FLAME_TRAIL) && gMC.dashDuration % GetFlagTraitData<int>(FlagTraits::DASH_FLAME_TRAIL, "rate", 3) == 0){
				int xCoord, yCoord;
				xCoord = gMC.x - 0x200 * 7;
				yCoord = gMC.y - 0x200 * 7;
				for (int i = 0; i < 3; i++)
					SetBullet(Bullets::FIREBALL_FLAME, xCoord, yCoord, 0);
			}

			if (gMC.flag & 1 || gMC.flag & 4)
			{
				gMC.dashLock = -1;
				gMC.dashDuration = 0;
				gMC.xm /= 2;
				if (IsDashEquipped())
					gMC.shock += 10;
				if (gMC.ameDashState){
					gMC.ameDashState = false;
					if (gMC.flag & 1){
						gMC.xm = -0x500;
					} else {
						gMC.xm = 0x500;
					}
				}
				if (CheckFlagTrait(FlagTraits::DASH_REFRESHES_JUMP))
				{
					gMC.pogo = true;
				}
			}
			else
			{
				if (gMC.dashLock == 0)
				{
					gMC.xm -= max_move * DashSpeed();
					if (miningLaserCharge){
						gMC.xm /= 2;
					}
				}
				else if (gMC.dashLock == 2)
				{
					gMC.xm += max_move * DashSpeed();
					if (miningLaserCharge){
						gMC.xm /= 2;
					}
				}
				if (CheckFlagTrait(FlagTraits::DASH_FLOAT))
				{
					gMC.ym = 0;
				}
				if (gMC.ameDashState){
					gMC.ym = 0;
				}
			}
		} else {
			int distance = 0x2000 * GetFlagTraitData<float>(FlagTraits::TELEPORT_DASH, "distance");
			if (gMC.direct == 0) distance *= -1;
			while (GetAttribute(round((gMC.x + distance) / (0x2000 * 1.0f)), round((gMC.y) / (0x2000 * 1.0f))) == 65 ||
			GetAttribute(round((gMC.x + distance) / (0x2000 * 1.0f)), round((gMC.y) / (0x2000 * 1.0f))) == 66){
				if (gMC.direct == 0)
					distance += 0x200;
				else
					distance -= 0x200;
			}
			if (abs(distance) > 0x2000 * 1){
			for (int l = gMC.x; 
					gMC.direct == 0 ? (l > gMC.x + distance) : (l < gMC.x + distance); 
					gMC.direct == 0 ? l -= 4_pix : l += 4_pix)
			{
				for (int i = 0; i < 2; i++){
					SetCaret(l, gMC.y, 11, 1);
				}
			}
			for (int l = gMC.x; 
					gMC.direct == 0 ? (l > gMC.x + distance) : (l < gMC.x + distance); 
					gMC.direct == 0 ? l -= 2_pix : l += 2_pix){
				NPCHAR* npc = FindNpCharHere(l, gMC.y);
				
				if (npc != NULL)
					DealDamage(*npc, 1);
			}

			MoveMyChar(gMC.x + distance, gMC.y);
			gMC.xm /= 2;
			gMC.shock += 35;
			NPCHAR* npc = FindNpCharHere(gMC.x, gMC.y);
			if (npc != NULL)
				DealDamage(*npc, GetFlagTraitData<float>(FlagTraits::TELEPORT_DASH, "damage"));
			gMC.dashDuration = 0;
			} else {
				gMC.dashDuration = 0;
			}
			if (CheckFlagTrait(FlagTraits::DASH_REFRESHES_JUMP)){
				gMC.pogo = true;
			}
		}
	}
	else if (gMC.dashDuration == DashDuration()){
		gMC.dashLock = -1;
		gMC.dashDuration = 0;
		gMC.xm /= 2;
		if (IsDashEquipped())
			gMC.shock += 10;
		if (gMC.ameDashState){
					gMC.ameDashState = false;
		}
		if (CheckFlagTrait(FlagTraits::DASH_REFRESHES_JUMP)){
			gMC.pogo = true;
		}
	}
	if (gMC.visualDashDuration > 0 && gMC.dashDuration == 0){
		gMC.visualDashDuration-=2;
		if (gMC.visualDashDuration < 0) gMC.visualDashDuration = 0;
	}

	if (bKey && gKey & gKeyAbility && CheckFlagTrait(FlagTraits::GLIDING) && gMC.ym * (gMC.flip ? -1 : 1) > 0x080){
		float max_speed = GetFlagTraitData<float>(FlagTraits::GLIDING, "max_speed");
		if (gMC.xm > max_move*max_speed)
			gMC.xm = max_move*max_speed;
		if (gMC.xm < -max_move*max_speed)
			gMC.xm = -max_move*max_speed;
		if (miningLaserCharge){
			gMC.xm /= 2;
		}
	}
	else {
		if (gMC.dashDuration > 0){
			if (gMC.xm > max_move*DashSpeed())
				gMC.xm = max_move*DashSpeed();
			if (gMC.xm < -max_move*DashSpeed())
				gMC.xm = -max_move*DashSpeed();
			if (miningLaserCharge){
				gMC.xm /= 2;
			}
		}
		else {
		float mod = GetFlagTraitData<float>(FlagTraits::MAX_XSPEED_LIMIT, "modifier", 1.0f);
		if (gMC.xm > max_move*mod)
			gMC.xm = max_move*mod;
		if (gMC.xm < -max_move*mod)
			gMC.xm = -max_move*mod;
		}
	}

	if (gMC.dashDuration > 0 && CheckFlagTrait(FlagTraits::STAR_DASH_VFX)){
		for (int i = 0; i < 10; i++)
		SetCaret(gMC.x, gMC.y, 13, 0);
	}
	if (gMC.dashDuration > 0 && CheckFlagTrait(FlagTraits::WITCH_DASH_VFX)){
		for (int i = 0; i < 4; i++)
		SetCaret(gMC.x, gMC.y, 11, 1);
	}

	if (gKey & gKeyAbility && CheckFlagTrait(FlagTraits::WALL_STICKY)){
		int wall1 = GetAttribute(round((((gMC.x) / 0x200) - 2)/16), round(((float)gMC.y) / 0x2000));
		int wall2 = GetAttribute(round((((gMC.x) / 0x200) + 15)/16), round(((float)gMC.y) / 0x2000));
		if (wall1 == 65){
			gMC.ym = 0;
			gMC.xm = 0;
			SetCaret(gMC.x - 4*0x200, gMC.y, 15, 0);
		} else if (wall2 == 65){
			gMC.ym = 0;
			gMC.xm = 0;
			SetCaret(gMC.x + 5*0x200, gMC.y, 15, 2);
		}
	}
	if (gMC.ym < -max_move)
		gMC.ym = -max_move;

	// Water splashing
	if (!gMC.no_splash_or_air_limit_underwater && !gMC.sprash && gMC.flag & 0x100)
	{
		int dir;

		if (gMC.flag & 0x800)
			dir = 2;
		else
			dir = 0;

		if (!(gMC.flag & 8) && gMC.ym > 0x200)
		{
			for (a = 0; a < 8; ++a)
			{
				x = gMC.x + (Random(-8, 8) * 0x200);
				SetNpChar(73, x, gMC.y, gMC.xm + Random(-0x200, 0x200), Random(-0x200, 0x80) - (gMC.ym / 2), dir, NULL, 0);
			}

			PlaySoundObject(56, SOUND_MODE_PLAY);
		}
		else
		{
			if (gMC.xm > 0x200 || gMC.xm < -0x200)
			{
				for (a = 0; a < 8; ++a)
				{
					x = gMC.x + (Random(-8, 8) * 0x200);
					SetNpChar(73, x, gMC.y, gMC.xm + Random(-0x200, 0x200), Random(-0x200, 0x80), dir, NULL, 0);
				}

				PlaySoundObject(56, SOUND_MODE_PLAY);
			}
		}

		gMC.sprash = true;
	}

	if (!(gMC.flag & 0x100))
		gMC.sprash = false;

	// Spike damage
	if (gMC.flag & 0x400){
		int damage = 5;
		if (gMC.max_life > 10 && !CheckFlagTrait(FlagTraits::SPIKE_DAMAGE_RESIST)){
			damage += gMC.max_life / 9;
		}
		gMC.deathReason = DeathReason::SPIKE;
		if (ChallengeEnabled("spike_kill")){
			Die();
		} else {
			DamageMyChar(damage);
		}
		if (gMC.life > 0)
			gMC.deathReason = 0;
	}

	if (bKey){
		if (!ChallengeEnabled("charge_jump")){
			gMC.chargeCD = 0;
		}
		else if (!(gMC.flag & 8 || (CheckFlagTrait(FlagTraits::CHARGED_SWIMS) && gMC.flag & 0x100))){
			gMC.chargeCD = 0;
		} else if (!(gKey & gKeyUp) && gMC.chargeCD > 0){
			gMC.chargeCD = 0;
		} else if ((gKey & gKeyUp)) {
			if (++gMC.chargeCD > 30){
				gMC.chargeCD = 30;
			}
		}
	}

	// Camera
	if (gMC.direct == 0)
	{
		gMC.index_x -= 0x200;
		if (gMC.index_x < -0x8000)
			gMC.index_x = -0x8000;
	}
	else
	{
		gMC.index_x += 0x200;
		if (gMC.index_x > 0x8000)
			gMC.index_x = 0x8000;
	}
	if (gKey & (gKeyUp) && bKey)
	{
		gMC.index_y -= 0x200;
		if (gMC.index_y < -0x8000)
			gMC.index_y = -0x8000;
	}
	else if (gKey & (gKeyDown) && bKey)
	{
		gMC.index_y += 0x200;
		if (gMC.index_y > 0x8000)
			gMC.index_y = 0x8000;
	}
	else
	{
		if (gMC.index_y > 0x200)
			gMC.index_y -= 0x200;
		if (gMC.index_y < -0x200)
			gMC.index_y += 0x200;
	}

	gMC.tgt_x = gMC.x + gMC.index_x;
	gMC.tgt_y = gMC.y + gMC.index_y;

	// Change position
	if ((CheckFlagTrait(FlagTraits::ICE_SKATES) && gKey & gKeyAbility)){
		gMC.x += gMC.xm;
	}
	else if (gMC.xm <= resist && gMC.xm >= -resist)
	{
		// This case is completely empty. This is most likely the result of commented-out code or some other change (so this is most likely inaccurate to the original source code)
	}
	else
	{
		gMC.x += gMC.xm;
	}

	gMC.y += gMC.ym;
}

void ActMyChar_Stream(bool bKey)
{
	gMC.up = false;
	gMC.down = false;

	if (bKey)
	{
		if (gKey & (gKeyLeft | gKeyRight))
		{
			if (gKey & gKeyLeft){
				gMC.xm -= 0x200;
				gMC.direct = 0;
			}

			if (gKey & gKeyRight){
				gMC.xm += 0x200;
				gMC.direct = 2;
			}
		}
		else if (gMC.xm < 0x80 && gMC.xm > -0x80)
		{
			gMC.xm = 0;
		}
		else if (gMC.xm > 0)
		{
			gMC.xm -= 0x160;
		}
		else if (gMC.xm < 0)
		{
			gMC.xm += 0x160;
		}

		if (gKey & (gKeyUp | gKeyDown))
		{
			if (gKey & gKeyUp){
				gMC.ym -= 0x200;
				gMC.up = true;
			}

			if (gKey & gKeyDown){
				gMC.ym += 0x200;
				gMC.down = true;
			}
		}
		else if (gMC.ym < 0x80 && gMC.ym > -0x80)
		{
			gMC.ym = 0;
		}
		else if (gMC.ym > 0)
		{
			gMC.ym -= 0x160;
		}
		else if (gMC.ym < 0)
		{
			gMC.ym += 0x160;
		}
	}
	else
	{
		if (gMC.xm < 0x160 && gMC.xm > -0x90)
			gMC.xm = 0;
		else if (gMC.xm > 0)
			gMC.xm -= 0x90;
		else if (gMC.xm < 0)
			gMC.xm += 0x90;

		if (gMC.ym < 0x160 && gMC.ym > -0x90)
			gMC.ym = 0;
		else if (gMC.ym > 0)
			gMC.ym -= 0x80;
		else if (gMC.ym < 0)
			gMC.ym += 0x80;
	}

	if (gMC.ym < -0x200 && gMC.flag & 2)
		SetCaret(gMC.x, gMC.y - gMC.hit.top, 13, 5);
	if (gMC.ym > 0x200 && gMC.flag & 8)
		SetCaret(gMC.x, gMC.y + gMC.hit.bottom, 13, 5);

	if (gMC.xm > 0x1000)
		gMC.xm = 0x1000;
	if (gMC.xm < -0x1000)
		gMC.xm = -0x1000;

	if (gMC.ym > 0x1000)
		gMC.ym = 0x1000;
	if (gMC.ym < -0x1000)
		gMC.ym = -0x1000;

	if ((gKey & (gKeyLeft | gKeyUp)) == (gKeyLeft | gKeyUp))
	{
		if (gMC.xm < -780*2)
			gMC.xm = -780*2;
		if (gMC.ym < -780*2)
			gMC.ym = -780*2;
	}

	if ((gKey & (gKeyRight | gKeyUp)) == (gKeyRight | gKeyUp))
	{
		if (gMC.xm > 780*2)
			gMC.xm = 780*2;
		if (gMC.ym < -780*2)
			gMC.ym = -780*2;
	}

	if ((gKey & (gKeyLeft | gKeyDown)) == (gKeyLeft | gKeyDown))
	{
		if (gMC.xm < -780*2)
			gMC.xm = -780*2;
		if (gMC.ym > 780*2)
			gMC.ym = 780*2;
	}

	if ((gKey & (gKeyRight | gKeyDown)) == (gKeyRight | gKeyDown))
	{
		if (gMC.xm > 780*2)
			gMC.xm = 780*2;
		if (gMC.ym > 780*2)
			gMC.ym = 780*2;
	}

	// Camera
	if (gMC.direct == 0)
	{
		gMC.index_x -= 0x400;
		if (gMC.index_x < -0x8000)
			gMC.index_x = -0x8000;
	}
	else
	{
		gMC.index_x += 0x400;
		if (gMC.index_x > 0x8000)
			gMC.index_x = 0x8000;
	}
	if (gKey & (gKeyUp) && bKey)
	{
		gMC.index_y -= 0x400;
		if (gMC.index_y < -0x8000)
			gMC.index_y = -0x8000;
	}
	else if (gKey & (gKeyDown) && bKey)
	{
		gMC.index_y += 0x400;
		if (gMC.index_y > 0x8000)
			gMC.index_y = 0x8000;
	}
	else
	{
		if (gMC.index_y > 0x400)
			gMC.index_y -= 0x400;
		if (gMC.index_y < -0x400)
			gMC.index_y += 0x400;
	}

	gMC.tgt_x = gMC.x + gMC.index_x;
	gMC.tgt_y = gMC.y + gMC.index_y;

	gMC.x += gMC.xm;
	gMC.y += gMC.ym;
}

void AirDeplete(){
	int rate = 1;
	if (!gMC.hell){
		if (CheckFlagTrait(FlagTraits::BREATH_EXTENSION)){
			rate = std::max(1, GetFlagTraitData<int>(FlagTraits::BREATH_EXTENSION, "modifier"));
		}
	}
	if (gCounter % rate == 0){
		gMC.air = std::max(gMC.air - 1, 0);
	}
}

void AirProcess(void)
{
	if (!(gMC.flag & 0x100) || gMC.no_splash_or_air_limit_underwater)
		{
			int limit = 1000;
			if (gMC.hell){
				limit = std::max(0, GetFlagTraitData<int>(FlagTraits::LAVA_PROTECTION, "amount"));
			}
			int airBoost = 25;
			if (gMC.difficulty >= DIFFICULTY_RESONANT && !gMC.hell){
				airBoost = 8;
			}
			gMC.air = std::min(gMC.air + airBoost, limit);
			if (gMC.hell){
				gMC.air = std::min(gMC.air + airBoost/2, limit);
			}
			if (gMC.air < limit && gMC.air_get <= 0) gMC.air_get = airBoost;
		}
		else
		{
			if ((gMC.tileset == TYPE_LIVING_WATERWAY || gMC.tileset == TYPE_IKACHAN) && gMC.depth > genContext.doctorStage)
				AirDeplete();
			AirDeplete();
			if (gMC.air <= 0)
			{
				if (gMC.hell) return;
				//start drowning
				gMC.air = 0;
				if (gMC.drowningTimer <= 0){
					gMC.life--;
					gMC.drowningTimer = 60;
					if (gMC.difficulty > DIFFICULTY_REINFORCED)
						gMC.drowningTimer = 30;
					if (CheckFlagTrait(FlagTraits::DROWNING_SLOWDOWN)){
						gMC.drowningTimer *= std::max(0.0f, GetFlagTraitData<float>(FlagTraits::DROWNING_SLOWDOWN, "modifier"));
					}
					for (int a = 0; a < 6; ++a)
					{
						int x = gMC.x + (Random(-8, 8) * 0x200);
						SetCaret(x, gMC.y + Random(-0x200, 0x80), 3, 0);
					}

					PlaySoundObject(21, SOUND_MODE_PLAY);
				}
				if (gMC.life <= 0){
					// Drown
					gMC.deathReason = 0;
					StartTextScript(41);

					gMC.cond &= ~0x80;
				}
			}
		}

		if (gMC.flag & 0x100)
		{
			gMC.air_get = 60;
		}
		else
		{
			if (gMC.air_get != 0)
				--gMC.air_get;
		}
}

int hungerRate = 600;
int teleportX;
int teleportY;

void ActMyChar(bool bKey)
{
	if (!(gMC.x > -48_pix && gMC.x < (gMap.width+4)*0x2000 && gMC.y < (gMap.length+1)*0x2000) && gMC.outerWallInit && (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED))
	{
		StartTextScript(42);
	}

	if (!(gMC.cond & 0x80)){
		for (int i = 0; i < SE_MAX; i++)
				ChangeSoundFrequency(i, 2195);
		return;
	}

	if (gMC.a == 20){
		SetCaret(0, 0, 18, 0);
	}

	if (gMC.exp_wait != 0)
		--gMC.exp_wait;

	if (IsASheep()){
		bKey = true;
	}

	if (gMC.shock != 0)
	{
		--gMC.shock;
	}
	else if (gMC.exp_count != 0)
	{
		SetValueView(&gMC.x, &gMC.y, gMC.exp_count);
		gMC.exp_count = 0;
	}

	ActCooldownList(gCooldowns);

	if (CheckFlagTrait(FlagTraits::HUNGER_SYSTEM)){
		if (--hungerRate < 0){
			gMC.deathReason = DeathReason::HUNGER;
			AddLifeMyChar(-1);
			hungerRate = 450;
			if (gMC.life > 0)
				gMC.deathReason = 0;
		}
	}

	if ((CheckFlagTrait(FlagTraits::NANITES)) && CountBulletNum(Bullets::NANITES_VFX) == 0){
		SetBullet(Bullets::NANITES_VFX, gMC.x, gMC.y);
	}

	if ((gMC.tileset == 'A' || gMC.tileset == '9' || gMC.tileset == 'E') && (gMC.flag & 0x100 && !GetNPCFlag(Flags::WATER_GUN_EFFECT)) && !CheckFlagTrait(FlagTraits::ACID_IMMUNITY))
	{
		if (++gMC.regenCount % 6 == 0)
		{
			SetCaret(gMC.x, gMC.y, 3, 0);
			SetCaret(gMC.x, gMC.y, 11, 0);
		}
		if (gMC.regenCount > 76 && gMC.life > 0)
		{
			gMC.deathReason = DeathReason::WATER;
			AddLifeMyChar(-1);
			gMC.regenCount = 0;
			if (gMC.life > 0)
				gMC.deathReason = 0;
		}
	}

	if (gMC.drowningTimer != 0){
		gMC.drowningTimer--;
	}

	if (gMC.freeze != 0){
		gMC.freeze--;
	}

	if (gMC.freeze > 0){
		for (int l = 0; l < 3 + std::fmax((gMC.freeze - 100) / 20, 0); l++)
		{
			SetCaret(gMC.x + Random(-gMC.view.front, gMC.view.back),
					 gMC.y + Random(-gMC.view.top, gMC.view.bottom), 13, 0);
		}
	}

	if (gKeyTrg & gMouseClick && gMC.chaosAnim == 0 && CheckForCooldown(gCooldowns, CHAOS_KEY_ID) == -1 && (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED) && CheckFlagTrait(FlagTraits::MOUSE_TELEPORT)){
		int fx, fy, x, y;
		GetFramePosition(&fx, &fy);
		x = fx + ScreenToSubpixelCoord(gMouseX);
		y = fy + ScreenToSubpixelCoord(gMouseY);
		int tile = GetAttribute(round(x * 1.0f / 0x2000), round(y * 1.0f / 0x2000));
		if (!gBL.flag || IntersectCheck(genContext.bossRect, x / 16_pix, y / 16_pix)){
			if (tile != 65 && tile != 66 && InsideMap(round(x * 1.0f / 0x2000), round(y * 1.0f / 0x2000))){
				gMC.chaosAnim = 60;
				teleportX = x;
				teleportY = y;
			}
		}
	}

	if (CheckForCooldown(gCooldowns, DOCTOR_SHOCK_ID) != -1 && Random(0, 2) == 0){
		//do nothing
	}
	else if (gMC.freeze > 0 && Random(0, 3 + std::fmax((gMC.freeze-100)/5, 0)) > 1){
				
	}
	else if (gMC.chaosAnim == 0) {
	switch (gMC.unit)
	{
		case 0:
			if (!(g_GameFlags & GAME_FLAG_IS_GAMEPLAY_NOT_POSSIBLE) && bKey)
				AirProcess();

			ActMyChar_Normal(bKey);
			break;

		case 1:
			ActMyChar_Stream(bKey);
			break;
	}
	} else {
		gMC.chaosAnim--;
		SetCaret(gMC.x, gMC.y, 11, 1);
		if (gMC.chaosAnim <= 0){
			gMC.chaosAnim = 0;
			InitiateCooldown(gCooldowns, CHAOS_KEY_ID);
			MoveMyChar(teleportX, teleportY);
			teleportX = 0;
			teleportY = 0;
			for (int i = 0; i < 50; i++)
				SetCaret(gMC.x, gMC.y, 11, 1);
		}
	}

	if (gKeyTrg & gKeyAbility && (CheckFlagTrait(FlagTraits::TIMESTOP)) && CheckForCooldown(gCooldowns, TIME_STOP_ID) == -1){
		SetFlash(gMC.x, gMC.y, 1, RGB(154, 209, 232) | (0x80 << 24));
		PlaySoundObject(101, SOUND_MODE_PLAY);
		for (int i = 0; i < SE_MAX; i++)
			ChangeSoundFrequency(i, 1200);
		InitiateCooldown(gCooldowns, TIME_STOP_ID);
		PlayOrganyaMusic();
	}

	if (gKeyTrg & gKeyAbility && CheckFlagTrait(FlagTraits::SCREEN_FREEZE) && CheckForCooldown(gCooldowns, TIME_RING_ID) == -1){
		SetFlash(gMC.x, gMC.y, 1, RGB(154, 209, 232) | (0x80 << 24));
		PlaySoundObject(118, SOUND_MODE_PLAY);
		for (int i = 0; i < NPC_MAX; i++){
			if (gNPC[i].cond & 0x80 &&
				gMC.x < gNPC[i].x + int(WINDOW_WIDTH/1.5f) * 1_pix && gMC.x > gNPC[i].x - int(WINDOW_WIDTH/1.5f) * 1_pix && gMC.y < gNPC[i].y + int(WINDOW_HEIGHT/1.5f) * 1_pix && gMC.y > gNPC[i].y - int(WINDOW_HEIGHT/1.5f) * 1_pix && gNPC[i].code_char != 4){
					gNPC[i].freeze = GetFlagTraitData<int>(FlagTraits::SCREEN_FREEZE, "amount");
					SetDestroyNpChar(gNPC[i].x, gNPC[i].y, gNPC[i].view.back, 10);
				}
		}
		InitiateCooldown(gCooldowns, TIME_RING_ID);
	}


	gMC.cond &= ~0x20;
}

void GetMyCharPosition(int *x, int *y)
{
	*x = gMC.x;
	*y = gMC.y;
}

void SetMyCharPosition(int x, int y)
{
	gMC.x = x;
	gMC.y = y;
	gMC.tgt_x = gMC.x;
	gMC.tgt_y = gMC.y;
	gMC.index_x = 0;
	gMC.index_y = 0;
	gMC.xm = 0;
	gMC.ym = 0;
	gMC.cond &= ~1;
	InitStar();
}

void MoveMyChar(int x, int y)
{
	gMC.x = x;
	gMC.y = y;
}

void ZeroMyCharXMove(void)
{
	gMC.xm = 0;
}

int GetUnitMyChar(void)
{
	return gMC.unit;
}

void SetMyCharDirect(unsigned char dir)
{
	int i;

	if (dir == 3)
	{
		gMC.cond |= 1;
	}
	else
	{
		gMC.cond &= ~1;

		if (dir < 10)
		{
			gMC.direct = dir;
		}
		else
		{
			for (i = 0; i < NPC_MAX; ++i)
				if (gNPC[i].code_event == dir)
					break;

			if (i == NPC_MAX)
				return;

			if (gMC.x > gNPC[i].x)
				gMC.direct = 0;
			else
				gMC.direct = 2;
		}
	}

	gMC.xm = 0;
	AnimationMyChar(false);
}

void ChangeMyUnit(unsigned char a)
{
	gMC.unit = a;
}

void PitMyChar(void)
{
	gMC.y += 2 * 0x10 * 0x200;	// Shove player two tiles down. I wonder what this was meant for?
}

// TODO: rework into "automatic" item-flag setter
void EquipItem(int flag, bool b)
{
	if (b){
		if (equipMap.contains(flag)){
			SetNPCFlag(equipMap[flag].flag);
		}
	}
	else {
		if (equipMap.contains(flag)){
			CutNPCFlag(equipMap[flag].flag);
		}
	}
	IncrementCooldown(gCooldowns, DASH_TIMER_ID, -9999);
}

void ResetCheck(void)
{
	gMC.cond &= ~1;
}

static int noise_no;
static unsigned int noise_freq;

void SetNoise(int no, int freq)
{
	noise_freq = freq;
	noise_no = no;

	switch (noise_no)
	{
		case 1:
			ChangeSoundFrequency(40, noise_freq);
			ChangeSoundFrequency(41, noise_freq + 100);
			PlaySoundObject(40, SOUND_MODE_PLAY_LOOP);
			PlaySoundObject(41, SOUND_MODE_PLAY_LOOP);
			break;

		case 2:
			PlaySoundObject(58, SOUND_MODE_PLAY_LOOP);
			break;
	}
}

void CutNoise(void)
{
	noise_no = 0;
	PlaySoundObject(40, SOUND_MODE_STOP);
	PlaySoundObject(41, SOUND_MODE_STOP);
	PlaySoundObject(58, SOUND_MODE_STOP);
}

void ResetNoise(void)
{
	switch (noise_no)
	{
		case 1:
			ChangeSoundFrequency(40, noise_freq);
			ChangeSoundFrequency(41, noise_freq + 100);
			PlaySoundObject(40, SOUND_MODE_PLAY_LOOP);
			PlaySoundObject(41, SOUND_MODE_PLAY_LOOP);
			break;

		case 2:
			PlaySoundObject(58, SOUND_MODE_PLAY_LOOP);
			break;
	}
}

void SleepNoise(void)
{
	PlaySoundObject(40, SOUND_MODE_STOP);
	PlaySoundObject(41, SOUND_MODE_STOP);
	PlaySoundObject(58, SOUND_MODE_STOP);
}
