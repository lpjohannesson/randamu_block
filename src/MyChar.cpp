#include "MyChar.h"

#include <cmath>
#include <cstddef>
#include <cstring>

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

int DashDuration(){
	if (gMC.ameDashState) return 2000000;
	if (gMC.equip & EQUIP_WATER_THURST && gMC.flag & 0x100) return 20;
	if ((gMC.equip & EQUIP_SUPER_DASH || GetNPCFlag(876)) && gMC.equip & EQUIP_SUNGLASSES) return 44;
	if (gMC.equip & EQUIP_SUPER_DASH || GetNPCFlag(876)) return 14;
	if (gMC.equip & EQUIP_SUNGLASSES) return 25;
	return 19;
}

float DashSpeed(){
	if (gMC.ameDashState) return 2.75;
	if (gMC.equip & EQUIP_WATER_THURST && gMC.flag & 0x100) return 2.75f;
	if (GetNPCFlag(866)) return 1.8;
	if ((GetNPCFlag(4100) || GetNPCFlag(7999)) && gArmsData[gSelectedArms].code == 9) return 3;
	if ((gMC.equip & EQUIP_SUPER_DASH || GetNPCFlag(876)) && gMC.equip & EQUIP_SPEED_SHOES && gMC.equip & EQUIP_SUNGLASSES) return 2.25;
	if ((gMC.equip & EQUIP_SUPER_DASH || GetNPCFlag(876)) && gMC.equip & EQUIP_SUNGLASSES) return 2.6;
	if ((gMC.equip & EQUIP_SUPER_DASH || GetNPCFlag(876)) && gMC.equip & EQUIP_SPEED_SHOES) return 2.35;
	if (gMC.equip & EQUIP_SUPER_DASH || GetNPCFlag(876)) return 2.25;
	if (gMC.equip & EQUIP_SUNGLASSES) return 1.85;
	if (gMC.equip & EQUIP_SPEED_SHOES) return 1.7;
	return 1.6;
}

int DashCooldown(){
	if (GetNPCFlag(874)) return 88;
	if (GetNPCFlag(866)) return 25;
	if (gMC.equip & EQUIP_WATER_THURST && gMC.flag & 0x100) return 20;
	if (GetNPCFlag(4100) || (GetNPCFlag(7999) && gArmsData[gSelectedArms].code == 9)) return 110;
	if ((gMC.equip & (EQUIP_SUPER_DASH | EQUIP_SUNGLASSES)) || GetNPCFlag(876)) return 45;
	if (gMC.equip & EQUIP_SUPER_DASH || GetNPCFlag(876)) return 35;
	if (gMC.equip & EQUIP_SUNGLASSES) return 50;
	if (!(gMC.equip & EQUIP_DASH_AMULET) && GetNPCFlag(5120)) return 120;
	return 40;
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
	if (GetNPCFlag(1024)) walkSound = 34;

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
			if (GetNPCFlag(4242)) aniDelay = Random(1, 6);

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
			if (GetNPCFlag(4242)) aniDelay = Random(1, 6);

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
	if ((GetNPCFlag(4250) || GetNPCFlag(7999)) && (gMC.xm != 0 || gMC.ym != 0) && gKey & gKeyAbility){
		gMC.saraDash = 1;
	}
	if (!(gKey & gKeyAbility) && (GetNPCFlag(4250) || GetNPCFlag(7999))){
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
	const int accOrder[] = {EQUIP_WATER_THURST, EQUIP_PLATING, EQUIP_SPEED_SHOES, EQUIP_SUNGLASSES, EQUIP_FISHY, EQUIP_PROPANE_TANK, EQUIP_WINGS, EQUIP_SUPER_DASH, EQUIP_GRAVITY_FLIP, EQUIP_SKATES};
	const int accRectNum[] = {6, 5, 2, 1, 4, 7, 3, 0, 8, 9};
	const int accOrderFlag[] = {863, 874, 866, 864, 867, 873, 871, 876};
	const int accRectNumFlag[] = {11, 15, 12, 10, 13, 14, 16, 17};
	if (CheckForCooldown(gCooldowns, SHEEP_CURSE_ID) != -1 && (gMC.cond & 0x80)){
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

	for (int i = 0; i < ARRAY_SIZE(accOrder); i++)
	{
		if (gMC.equip & accOrder[i])
		{
			Rect accRect = gMC.direct == 0 ? rcLeft : rcRight;
			accRect.top += 32 * accRectNum[i];
			accRect.bottom += 32 * accRectNum[i];
			PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y - gMC.view.top) - SubpixelToScreenCoord(fy), &accRect, color, SURFACE_ID_MY_ACCESSORIES_DROWNED);
		}
	}
	for (int i = 0; i < ARRAY_SIZE(accOrderFlag); i++)
	{
		if (GetNPCFlag(accOrderFlag[i]))
		{
			Rect accRect = gMC.direct == 0 ? rcLeft : rcRight;
			accRect.top += 32 * accRectNumFlag[i];
			accRect.bottom += 32 * accRectNumFlag[i];
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
	if (GetNPCFlag(4400) && gArmsData[gSelectedArms].code == 13 || CountBulletNum(14)){
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

	if ((gMC.equip & EQUIP_FAR_PLACE) && (gArmsData[gSelectedArms].code == 2 
		|| (gArmsData[gSelectedArms].code == 12 && (GetNPCFlag(871) || GetNPCFlag(880))))){
		Rect limitRect = {160, 112, 176, 128};
		int dist = 5;
		if (GetNPCFlag(871)) dist = 7;
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
	for (int i = 0; i < ARRAY_SIZE(accOrder); i++)
	{
		if (gMC.equip & accOrder[i])
		{
			if (GetNPCFlag(4096) && accOrder[i] == EQUIP_SKATES) continue;
			Rect accRect = gMC.rect;
			accRect.top += 32 * accRectNum[i];
			accRect.bottom += 32 * accRectNum[i];
			PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y - gMC.view.top) - SubpixelToScreenCoord(fy), &accRect, SURFACE_ID_MY_ACCESSORIES_DASH, 0.33f - 0.32f * (float(i) / (gMC.visualDashDuration / (float)4)));
		}
	}
	for (int i = 0; i < ARRAY_SIZE(accOrderFlag); i++)
	{
		if (GetNPCFlag(accOrderFlag[i]))
		{
			if (GetNPCFlag(4096) && accOrder[i] == EQUIP_SKATES) continue;
			Rect accRect = gMC.rect;
			accRect.top += 32 * accRectNumFlag[i];
			accRect.bottom += 32 * accRectNumFlag[i];
			PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y - gMC.view.top) - SubpixelToScreenCoord(fy), &accRect, SURFACE_ID_MY_ACCESSORIES_DASH, 0.33f - 0.32f * (float(i) / (gMC.visualDashDuration / (float)4)));
		}
	}
	}

	if (gMC.shock / 2 % 2 && !CountBulletNum(17))
		return;

	if (CountBulletNum(14))
		return;

	// Draw player
	rect = gMC.rect;
	rect.top += 32 * gMIMCurrentNum;
	rect.bottom += 32 * gMIMCurrentNum;
#ifndef ENABLE_MIM_DISABLE_EQUIP_40_GRAPHICS
#endif
	PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y - gMC.view.top) - SubpixelToScreenCoord(fy), &rect, SURFACE_ID_MY_CHAR);

	for (int i = 0; i < ARRAY_SIZE(accOrder); i++)
	{
		if (gMC.equip & accOrder[i])
		{
			Rect accRect = gMC.rect;
			accRect.top += 32 * accRectNum[i];
			accRect.bottom += 32 * accRectNum[i];
			PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y - gMC.view.top) - SubpixelToScreenCoord(fy), &accRect, SURFACE_ID_MY_ACCESSORIES);
		}
	}
	for (int i = 0; i < ARRAY_SIZE(accOrderFlag); i++)
	{
		if (GetNPCFlag(accOrderFlag[i]))
		{
			Rect accRect = gMC.rect;
			accRect.top += 32 * accRectNumFlag[i];
			accRect.bottom += 32 * accRectNumFlag[i];
			PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x - gMC.view.front) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y - gMC.view.top) - SubpixelToScreenCoord(fy), &accRect, SURFACE_ID_MY_ACCESSORIES);
		}
	}

	// Draw air tank
	Rect rcBubble[2] = {
		{56, 96, 80, 120},
		{80, 96, 104, 120},
	};

	++gMC.bubble;
	if (gMC.equip & EQUIP_AIR_TANK && gMC.flag & 0x100)
		PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x) - PixelToScreenCoord(12) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y) - PixelToScreenCoord(12) - SubpixelToScreenCoord(fy), &rcBubble[(gMC.bubble / 2) % 2], SURFACE_ID_CARET);
	else if (gMC.unit == 1)
		PutBitmap3_Flip(&grcGame, SubpixelToScreenCoord(gMC.x) - PixelToScreenCoord(12) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(gMC.y) - PixelToScreenCoord(12) - SubpixelToScreenCoord(fy), &rcBubble[(gMC.bubble / 2) % 2], SURFACE_ID_CARET);
	}
	if (ChallengeEnabled("hitboxes"))
	{
		int size = 0x200 * 3;
		if (GetNPCFlag(4405))
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

bool IsDashEquipped(){
	return (gMC.equip & (EQUIP_SUPER_DASH | EQUIP_DASH_AMULET) || (gMC.flag & 0x100 && gMC.equip & EQUIP_WATER_THURST) || GetNPCFlag(4300)
	|| ((GetNPCFlag(4100) || GetNPCFlag(7999)) && gArmsData[gSelectedArms].code == 9) || GetNPCFlag(866) || GetNPCFlag(5120) || GetNPCFlag(874) || GetNPCFlag(871) || GetNPCFlag(876));
}

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
	if (GetNPCFlag(863)){
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
	if (GetNPCFlag(863))
		max_move = gMC.physics_normal.max_move;
	if ((gMC.flag & 0x100) && (gMC.equip & EQUIP_WATER_THURST)){
		jump = gMC.physics_normal.jump*1.25;
	} else {
		if (gMC.equip & EQUIP_WINGS){
			jump = 1600;
		}
		if (GetNPCFlag(866)){
			jump = 1800;
		}
	}
	if (GetNPCFlag(863)){
		gMC.no_splash_or_air_limit_underwater = 1;
		if (gMC.flag & 0x100)
			gravity2 = 0;
	}
	if (GetNPCFlag(4300)){
		jump *= 1.5;
		dash1 *= 1.15;
		dash2 *= 1.15;
		max_dash *= 1.15;
		gravity2 *= 0.9;
		if (gMC.life == 0){
			dash1 *= 1.15;
			dash2 *= 1.15;
			max_dash *= 1.15;
		}
	}
	if (GetNPCFlag(4242)){
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
	if (GetNPCFlag(4405)){
		gravity2 *= 0.53;
	}
	if (GetNPCFlag(7999)){
		dash1 *= 1.5;
		dash2 *= 1.5;
		max_dash *= 1.5;
		max_move *= 1.2;
		resist *= 1.5;
		jump *= 1.25;
		gravity2 *= 0.85;
		gravity1 *= 1.6;
		gMC.no_splash_or_air_limit_underwater = 1;
		if (gKey & gKeyAbility)
			max_dash *= 2;
	}
	if (GetNPCFlag(4200)){
		dash1 *= 0.82;
		dash2 *= 0.82;
		max_dash *= 0.71;
		jump *= 1.15;
		gravity2 *= 0.88;
		gravity1 *= 1.25;
	}
	if (GetNPCFlag(4100)){
		jump *= 1.25;
		dash1 *= 1.06;
		dash2 *= 1.06;
		max_dash *= 1.06;
		max_move *= 1.025;
	}
	if (GetNPCFlag(4096)){
		jump *= 1.08;
		gravity2 *= 0.7;
		dash1 *= 1.4;
		dash2 *= 1.4;
		max_dash *= 1.2;
		max_move *= 1.1;
	}
	if (GetNPCFlag(1024)){
		jump *= 1.1;
		dash1 *= 1.6;
		dash2 *= 0.33;
		resist *= 0.5;
		gravity1 *= 2;
		gravity2 *= 0.8;
	}
	if (GetNPCFlag(4400)){
		jump *= 1.05;
		dash1 *= 1.075;
		dash2 *= 1.075;
		max_dash *= 1.075;
		gravity2 = 0;
		gMC.no_splash_or_air_limit_underwater = 1;
	}
	if (GetNPCFlag(3333)){
		dash1 *= 1.08;
		dash2 *= 1.08;
		max_dash *= 1.08;
	}
	if (GetNPCFlag(4150)){
		dash1 *= 1.2;
		dash2 *= 1.2;
		max_dash *= 1.2;
		resist *= 1.2;
	}
	if (GetNPCFlag(4475)){
		dash1 *= 1.75;
		dash2 *= 1.75;
		max_dash *= 1.75;
		max_move *= 1.12;
		resist *= 1.75;
		jump *= 1.25;
	}
	if (GetNPCFlag(4250)){
		resist *= 1.09;
		gravity1 *= 0.8;
		gravity2 *= 0.8;
		if (gKey & gKeyAbility)
			max_dash *= 2;
	}
	if (GetNPCFlag(4450)){
		dash1 *= 1.25;
		dash2 *= 1.25;
		max_dash *= 1.25;
		resist *= 1.25;
	}
	if (GetNPCFlag(869)){
		dash1 *= 1.2;
	}
	if (gMC.tileset == TYPE_ICE){
		if (!(gMC.equip & EQUIP_SKATES) && !GetNPCFlag(869)){
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
	if (gKey & gKeyDown && gArmsData[gSelectedArms].code == 18){
		max_dash /= 2;
		dash1 /= 2;
		dash2 /= 2;
		jump /= 2;
		resist /= 3;
		gravity2 /= 2;
		gravity1 /= 2;
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

	if (GetNPCFlag(4301)){
		if (bloodTimer == 0){
			bloodTimer = Random(3, 8);
			SetCaret(gMC.x, gMC.y, 11, 0);
		}
		else bloodTimer--;
	}

	if (gMC.equip & EQUIP_SKATES){
		if (gKey & gKeyAbility){
			dash1 = dash2;
		}
	}

	if (GetNPCFlag(880) && (gArmsData[gSelectedArms].code == 2 || gArmsData[gSelectedArms].code == 12)){
		for (int i = 0; i < NPC_MAX; i++){
			if (Random(0, 10) == 0 && std::find(etherealEntities.begin(), etherealEntities.end(), gNPC[i].code_char) == etherealEntities.end() && gNPC[i].damage > 0 && Distance(gMC.x, gMC.y, gNPC[i].x, gNPC[i].y) <= 88_pix){
				gNPC[i].freeze += 10;
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

		if (gMC.equip & EQUIP_BOOSTER_2_0)
		{
			gMC.boost_cnt = 40;
			if (gMC.equip & EQUIP_BOOSTER_FUEL || GetNPCFlag(862)){
				gMC.boost_cnt = 90;
			}
		}
		else if (GetNPCFlag(869)){
			gMC.boost_cnt = 60;
		}
		else
		{
			gMC.boost_cnt = 0;
			if (gMC.equip & EQUIP_BOOSTER_FUEL || GetNPCFlag(862)){
				gMC.boost_cnt = 15;
			}
		}

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
				if (gMC.equip & EQUIP_SPEED_SHOES){
					maximal_dash *= 1.75;
				}
				if (GetNPCFlag(869)){
					maximal_dash = 1535;
				}
				if (gMC.equip & EQUIP_SKATES && gKey & gKeyAbility){
					maximal_dash = 1500;
				}
				if (GetNPCFlag(879)){
					maximal_dash *= 2;
					dashy *= 3;
				}
				if (gKey & gKeyLeft && gMC.xm > -maximal_dash){
					if (gMC.xm > 0 && gMC.equip & EQUIP_THROTTLER){
						gMC.xm -= dashy;
					}
					gMC.xm -= dashy;
					if (gMC.saraDash > 0 && gMC.xm > -500){
						gMC.xm = -500;
					}
				}
				if (gKey & gKeyRight && gMC.xm < maximal_dash){
					if (gMC.xm < 0 && gMC.equip & EQUIP_THROTTLER) gMC.xm += dashy;
					gMC.xm += dashy;
					if (gMC.saraDash > 0 && gMC.xm < 500){
						gMC.xm = 500;
					}
				}

				if(!(gKey & gKeyAbility && GetNPCFlag(872))){
					if (gKey & gKeyLeft){
						gMC.direct = 0;
					}
						
					if (gKey & gKeyRight){
						gMC.direct = 2;
					}
				}

				if ((gMC.xm <= -max_dash*1.5 || gMC.xm >= max_dash*1.5) && (gMC.equip & EQUIP_SPEED_SHOES || GetNPCFlag(879)))
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

		// Friction
		if (!(gMC.cond & 0x20))
		{
			if (!(gMC.equip & EQUIP_SKATES && gKey & gKeyAbility))
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
			if ((gMC.equip & (EQUIP_BOOSTER_2_0 | EQUIP_BOOSTER_FUEL) || GetNPCFlag(869)) && gKeyTrg & gKeyJump &&
			 (gMC.boost_cnt != 0) && (!gMC.pogo || !(gMC.equip & EQUIP_DOUBLE_JUMP)))
			{
				if (GetNPCFlag(869))
				{
					gMC.boost_sw = 1;

					if (gMC.ym > 0x30)
						gMC.ym /= 2;
				} else if (gMC.equip & (EQUIP_BOOSTER_2_0 | EQUIP_BOOSTER_FUEL))
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
			if (gKey & gKeyJump && GetNPCFlag(1024) && gArmsData[gSelectedArms].code == 19 && gArmsData[gSelectedArms].num > 0){
				dash2 *= 8;
			}
			if (bKey && gKey & gKeyAbility && gMC.equip & EQUIP_WINGS && gMC.ym > 0x080){
				maximal_dash *= 1.75;
				dashy *= 1.35;
			}
			if (GetNPCFlag(879)){
				maximal_dash *= 2;
				dashy *= 3;
			}
			if (gKey & gKeyLeft && gMC.xm > -maximal_dash){
				if (gMC.xm > 0 && gMC.equip & EQUIP_THROTTLER) gMC.xm -= dash2;
				gMC.xm -= dash2;
				if (gMC.saraDash > 0 && gMC.xm > -500){
					gMC.xm = -500;
				}
			}
			if (gKey & gKeyRight && gMC.xm < maximal_dash){
				if (gMC.xm < 0 && gMC.equip & EQUIP_THROTTLER) gMC.xm += dash2;
				gMC.xm += dash2;
				if (gMC.saraDash > 0 && gMC.xm < 500){
					gMC.xm = 500;
				}
			}

			if(!(gKey & gKeyAbility && GetNPCFlag(872))){
					if (gKey & gKeyLeft){
						gMC.direct = 0;
					}
						
					if (gKey & gKeyRight){
						gMC.direct = 2;
					}
				}
		}

		// Slow down when stopped boosting (Booster 2.0)
		if (gMC.equip & (EQUIP_BOOSTER_2_0 | EQUIP_BOOSTER_FUEL) && gMC.boost_sw != 0 && !(gKey & gKeyJump))
		{
			if (gMC.boost_sw == 1)
				gMC.xm /= 2;
			else if (gMC.boost_sw == 2)
				gMC.ym /= 2;
		}
		if (gMC.equip & (EQUIP_BOOSTER_2_0 | EQUIP_BOOSTER_FUEL) && gMC.boost_sw != 0 && (gMC.boost_cnt == 0) && !GetNPCFlag(862))
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
					(gMC.flag & 0x100 && gMC.boost_cnt <= 0)
					|| (gMC.equip & EQUIP_ALIEN_MEDAL && gMC.pogo && GetAttribute(round(((float)gMC.x) / 0x2000), round(((float)gMC.y + 0x200 * 15) / 0x2000)) == 66)
					|| ((gMC.equip & EQUIP_WALL_JUMP || GetNPCFlag(4300)) && GetAttribute(round(((float)gMC.x) / 0x2000) + 1, round(((float)gMC.y) / 0x2000)) == 65)
					|| ((gMC.equip & EQUIP_WALL_JUMP || GetNPCFlag(4300)) && GetAttribute(round(((float)gMC.x) / 0x2000) - 1, round(((float)gMC.y) / 0x2000)) == 65)
					 || ((gMC.equip & EQUIP_DOUBLE_JUMP || GetNPCFlag(4300) || gMC.saraDash || (GetNPCFlag(866))) && gMC.pogo)))
		{
			if (gMC.flag & 0x2000)
			{
				// Another weird empty case needed for accurate assembly.
				// There probably used to be some commented-out code here.
			}
			else
			{
				PlaySoundObject(15, SOUND_MODE_PLAY);
				if ((gMC.equip & EQUIP_ALIEN_MEDAL && GetAttribute(round(((float)gMC.x) / 0x2000), round(((float)gMC.y + 0x200 * 15) / 0x2000)) == 66)){
					gMC.pogo = false;
					gMC.ym -= jump / 2;
				}
				else if ((gMC.equip & EQUIP_WALL_JUMP || GetNPCFlag(4300)) && GetAttribute(round(((float)gMC.x) / 0x2000) + 1, round(((float)gMC.y) / 0x2000)) == 65 &&
				 !(gMC.flag & 8 || gMC.flag & 0x10 || gMC.flag & 0x20 || gMC.flag & 0x100)){
					gMC.xm -= max_dash;
					gMC.direct = 0;
					gMC.pogo = true;
				} else if ((gMC.equip & EQUIP_WALL_JUMP || GetNPCFlag(4300)) && GetAttribute(round(((float)gMC.x) / 0x2000) - 1, round(((float)gMC.y) / 0x2000)) == 65 &&
				 !(gMC.flag & 8 || gMC.flag & 0x10 || gMC.flag & 0x20 || gMC.flag & 0x100)){
					gMC.xm += max_dash;
					gMC.direct = 2;
					gMC.pogo = true;
				}
				else if ((gMC.equip & EQUIP_DOUBLE_JUMP || GetNPCFlag(4300) || (GetNPCFlag(866)) || gMC.saraDash) && gMC.pogo == true && !(gMC.flag & 8 || gMC.flag & 0x10 || gMC.flag & 0x20 || gMC.flag & 0x100) ){
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
	if (gMC.equip &  (EQUIP_BOOSTER_2_0 | EQUIP_BOOSTER_FUEL) && gMC.boost_sw != 0)
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
				if (gMC.direct == 0){
					SetCaret(gMC.x + (2 * 0x200), gMC.y + (2 * 0x200), 7, 2);
				}
				if (gMC.direct == 2){
					SetCaret(gMC.x - (2 * 0x200), gMC.y + (2 * 0x200), 7, 0);
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
				SetCaret(gMC.x, gMC.y + (6 * 0x200), 7, 3);
				PlaySoundObject(113, SOUND_MODE_PLAY);
			}
		}
		else if (gMC.boost_sw == 3 && (gKeyTrg & gKeyJump || gMC.boost_cnt % 3 == 1))
		{
			// Boost particles (and sound)
			SetCaret(gMC.x, gMC.y - (6 * 0x200), 7, 1);
			PlaySoundObject(113, SOUND_MODE_PLAY);
		}
	}
	// Upwards wind/current
	else if (gMC.flag & 0x2000)
	{
		gMC.ym += gravity1;
	}
	// Booster 0.8
	else if ((GetNPCFlag(869) && (!(gMC.equip & EQUIP_BOOSTER_2_0)) && gMC.boost_sw != 0))
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
	else if (((gMC.flip && gMC.ym > (GetNPCFlag(4400) ? 0x400 : 0)) || gMC.ym < (GetNPCFlag(4400) ? 0x400 : 0)) && bKey && gKey & gKeyJump)
	{
		if (GetNPCFlag(4475) && abs(gMC.xm) >= max_move - 0x100){
			gravity2 /= 1.5;
		}
		if (gMC.saraDash > 0 && abs(gMC.xm) > 900) gravity2 /= 3;
		if (gKey & gKeyAbility && gMC.equip & EQUIP_SKATES && abs(gMC.xm) >= max_dash + 275){
			gravity2 /= 2;
		}
		if (gKey & gKeyUp && gMC.chargeActive) {
			gravity2 *= 0.66;
			if (gMC.equip & EQUIP_WINGS){
				gravity2 *= 0.4;
			}
		}
		gMC.ym += gravity2;
		if (GetNPCFlag(879)){
			gMC.ym -= 0.008f * abs(gMC.xm);
		}
	}
	// Normal gravity
	else
	{
		if (gMC.equip & EQUIP_FISHY && !(bKey && gKey & gKeyAbility) && gMC.flag & 0x100){
			gMC.ym -= gravity1*2;
		}
		else{
			if (gMC.equip & EQUIP_FISHY && !(bKey && gKey & gKeyAbility) && gMC.flag & 0x100){
				gMC.ym += gravity1*5;
			} else
			gMC.ym += gravity1;
		}
	}
	// if (bKey && gKey & gKeyJump && gMC.equip & EQUIP_WINGS && gMC.ym > 0){
	// 	gMC.ym -= gravity1 * 0.645;
	// }
	// if ((gMC.equip & EQUIP_HEAVY_WEIGHT) && (bKey && gKey & gKeyAbility)){
	// 	gMC.ym = max_dash*3;
	// }
	if ((GetNPCFlag(4450) || GetNPCFlag(7999)) && (CountBulletNum(49) || CountBulletNum(50) || CountBulletNum(51))){
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

	if (gKeyTrg & gKeyAbility && gMC.equip & EQUIP_GRAVITY_FLIP && 
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

	if (bKey && gKey & gKeyAbility && gMC.equip & EQUIP_WINGS && gMC.ym > 0 && !(gMC.flag & 8 || gMC.flag & 0x10 || gMC.flag & 0x20)){
		SetCaret(gMC.x, gMC.y + 0x200*4, 13, 0);
	}
	if (gMC.ym > max_move * 0.75 && GetNPCFlag(5120)){
		gMC.ym = max_move * 0.75;
	}
	if (gMC.tileset == TYPE_SPACESTATION && gMC.ym > max_move*0.7){
		gMC.ym = max_move*0.7;
	}
	if (gMC.saraDash > 0 && gMC.ym * (gMC.flip ? -1 : 1) > max_move / 3 && gKey & gKeyJump){
		gMC.ym = max_move / 3 * (gMC.flip ? -1 : 1);
	}
	if (bKey && gKey & gKeyAbility && gMC.equip & EQUIP_WINGS && gMC.ym  * (gMC.flip ? -1 : 1) > max_move / 2.8){
		gMC.ym = max_move / 2.8 * (gMC.flip ? -1 : 1);
	}
	if (gMC.ym > max_move / 6 && CheckForCooldown(gCooldowns, DASH_TIMER_ID) >= DashCooldown() / 2 && (GetNPCFlag(866))){
		gMC.ym = max_move / 6;
	}
	if (gMC.ym > max_move && !(gMC.equip & EQUIP_HEAVY_WEIGHT && gKey & gKeyAbility)){
		gMC.ym = max_move;
	}
	if ((gMC.equip & EQUIP_HEAVY_WEIGHT) && (bKey && gKey & gKeyAbility) && !(gMC.flag & 8)){
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
		 && gMC.dashDuration == 0 && IsDashEquipped()){
		InitiateCooldown(gCooldowns, DASH_TIMER_ID);
		gMC.dashDuration = 1;
		gMC.visualDashDuration = 1;
		gMC.dashLock = gMC.direct;
		if (GetNPCFlag(5120)){
			gMC.ameDashState = true;
		}
		if ((GetNPCFlag(4100) || GetNPCFlag(7999)) && gArmsData[gSelectedArms].code == 9)
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
		if (!GetNPCFlag(874) || gBL.flag)
		{
			gMC.dashDuration++;
			gMC.visualDashDuration++;
			if (GetNPCFlag(871) && gMC.dashDuration % 3 == 0){
				int xCoord, yCoord;
				xCoord = gMC.x - 0x200 * 7;
				yCoord = gMC.y - 0x200 * 7;
				for (int i = 0; i < 3; i++)
					SetBullet(45, xCoord, yCoord, 0);
			}

			if (gMC.flag & 1 || gMC.flag & 4)
			{
				gMC.dashLock = -1;
				gMC.dashDuration = 0;
				gMC.xm /= 2;
				gMC.shock += 10;
				if (gMC.ameDashState){
					gMC.ameDashState = false;
					if (gMC.flag & 1){
						gMC.xm = -0x500;
					} else {
						gMC.xm = 0x500;
					}
				}
				if (GetNPCFlag(866))
				{
					gMC.pogo = true;
				}
			}
			else
			{
				if (gMC.dashLock == 0)
				{
					gMC.xm -= max_move * DashSpeed();
					if (gKey & gKeyDown && gArmsData[gSelectedArms].code == 18){
						gMC.xm /= 2;
					}
				}
				else if (gMC.dashLock == 2)
				{
					gMC.xm += max_move * DashSpeed();
					if (gKey & gKeyDown && gArmsData[gSelectedArms].code == 18){
						gMC.xm /= 2;
					}
				}
				if (gMC.equip & EQUIP_SUNGLASSES || GetNPCFlag(4405) || GetNPCFlag(7999))
				{
					gMC.ym = 0;
				}
				if (gMC.ameDashState){
					gMC.ym = 0;
				}
			}
		} else {
			int distance = 0x2000 * 6;
			if (gKey & gKeyDown && gArmsData[gSelectedArms].code == 18){
				distance /= 2;
			}
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
				DealDamage(*npc, 24);
			gMC.dashDuration = 0;
			if (GetNPCFlag(866)){
				gMC.pogo = true;
			}
			} else {
				gMC.dashDuration = 0;
			}
		}
	}
	else if (gMC.dashDuration == DashDuration()){
		gMC.dashLock = -1;
		gMC.dashDuration = 0;
		gMC.xm /= 2;
		gMC.shock += 10;
		if (gMC.ameDashState){
					gMC.ameDashState = false;
		}
		if (GetNPCFlag(866)){
			gMC.pogo = true;
		}
	}
	if (gMC.visualDashDuration > 0 && gMC.dashDuration == 0){
		gMC.visualDashDuration-=2;
		if (gMC.visualDashDuration < 0) gMC.visualDashDuration = 0;
	}

	if (bKey && gKey & gKeyAbility && gMC.equip & EQUIP_WINGS && gMC.ym * (gMC.flip ? -1 : 1) > 0x080){
		if (gMC.xm > max_move*1.8)
			gMC.xm = max_move*1.8;
		if (gMC.xm < -max_move*1.8)
			gMC.xm = -max_move*1.8;
		if (gKey & gKeyDown && gArmsData[gSelectedArms].code == 18){
			gMC.xm /= 2;
		}
	}
	else {
		if (gMC.dashDuration > 0){
			if (gMC.xm > max_move*DashSpeed())
				gMC.xm = max_move*DashSpeed();
			if (gMC.xm < -max_move*DashSpeed())
				gMC.xm = -max_move*DashSpeed();
			if (gKey & gKeyDown && gArmsData[gSelectedArms].code == 18){
				gMC.xm /= 2;
			}
		}
		else {
		float mod = 1.0f;
		if (GetNPCFlag(4475)) mod = 1.75f;
		if (gMC.xm > max_move*mod)
			gMC.xm = max_move*mod;
		if (gMC.xm < -max_move*mod)
			gMC.xm = -max_move*mod;
		}
	}

	if (gMC.dashDuration > 0 && gMC.equip & EQUIP_SUNGLASSES){
		for (int i = 0; i < 10; i++)
		SetCaret(gMC.x, gMC.y, 13, 0);
	}
	if (gMC.dashDuration > 0 && (GetNPCFlag(4405) || GetNPCFlag(7999))){
		for (int i = 0; i < 4; i++)
		SetCaret(gMC.x, gMC.y, 11, 1);
	}

	if (gKey & gKeyAbility && gMC.equip & EQUIP_WALL_JUMP){
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
	if (GetNPCFlag(866) && gMC.ym < -max_move*1.2){
		gMC.ym = -max_move*1.2;
	} else if (gMC.ym < -max_move)
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
		if (gMC.max_life > 10 && !(gMC.equip & EQUIP_ALIEN_MEDAL)){
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
		else if (!(gMC.flag & 8 || (gMC.equip & EQUIP_WATER_THURST && gMC.flag & 0x100))){
			gMC.chargeCD = 0;
		} else if (!(gKey & gKeyUp) && gMC.chargeCD > 0){
			gMC.chargeCD = 0;
		} else if ((gKey & gKeyUp)) {
			if (++gMC.chargeCD > 30){
				gMC.chargeCD = 30;
				SetCaret(gMC.x, gMC.y + gMC.hit.top, 13, 0);
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
	if ((gMC.equip & EQUIP_SKATES && gKey & gKeyAbility)){
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

void AirProcess(void)
{
	if (gMC.equip & EQUIP_AIR_TANK && !gMC.hell)
	{
		gMC.air = 1000;
		gMC.air_get = 0;
	}
	else
	{
		if (!(gMC.flag & 0x100) || gMC.no_splash_or_air_limit_underwater)
		{
			if (gMC.difficulty < DIFFICULTY_RESONANT){
				gMC.air = 1000;
				if (gMC.hell){
					gMC.air = 480;
				}
			} else {
				int limit = 1000;
				if (gMC.hell){
					limit = 480;
				}
				if (++gMC.air > limit){
					gMC.air = limit;
				} else {
					gMC.air_get = 27;
				}
			}
		}
		else
		{
			if ((gMC.tileset == TYPE_LIVING_WATERWAY || gMC.tileset == TYPE_IKACHAN) && gMC.depth > genContext.doctorStage)
				gMC.air--;
			if (--gMC.air <= 0)
			{
				if (gMC.hell) return;
				//start drowning
				gMC.air = 0;
				if (gMC.drowningTimer <= 0){
					gMC.life--;
					gMC.drowningTimer = 60;
					if (gMC.difficulty > DIFFICULTY_REINFORCED)
						gMC.drowningTimer = 30;
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

					// if (gMC.direct == 0)
					// 	SetCaret(gMC.x, gMC.y, 8, 0);
					// else
					// 	SetCaret(gMC.x, gMC.y, 8, 2);

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

	if (CheckForCooldown(gCooldowns, SHEEP_CURSE_ID) != -1){
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

	if (GetNPCFlag(3333)){
		if (--hungerRate < 0){
			gMC.deathReason = DeathReason::HUNGER;
			AddLifeMyChar(-1);
			hungerRate = 450;
			if (gMC.life > 0)
				gMC.deathReason = 0;
		}
	}

	if ((GetNPCFlag(882) || GetNPCFlag(883)) && CountBulletNum(86) == 0){
		SetBullet(86, gMC.x, gMC.y);
	}

	if ((gMC.tileset == 'A' || gMC.tileset == '9' || gMC.tileset == 'E') && (gMC.flag & 0x100 && !GetNPCFlag(881)) && !(gMC.equip & EQUIP_MIMIGA_MASK))
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
	else if (gMC.equip & EQUIP_MIMIGA_MASK && bKey)
	{
		int regenDelay = RegenerationDelay();
		if (++gMC.regenCount > regenDelay && gMC.life < gMC.max_life)
		{
			if (!((gMC.tileset == 'A' || gMC.tileset == '9' || gMC.tileset == 'E') && gMC.flag & 0x100))
			{
				AddLifeMyChar(1);
				gMC.regenMaxCount = std::max(RegenerationCap(), gMC.regenMaxCount + 1);
				PlaySoundObject(20, SOUND_MODE_PLAY);
			}
			gMC.regenCount = 0;
		}
	}

	if (gMC.doctorShock != 0){
		gMC.doctorShock--;
	}

	if (gMC.drowningTimer != 0){
		gMC.drowningTimer--;
	}

	if (gMC.doctorShock > 0){
		for (int i = 0; i < 7; i++)
			SetCaret(gMC.x + 0x200 * 8, gMC.y, 11, 0);
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

	if (gKeyTrg & gMouseClick && gMC.chaosAnim == 0 && CheckForCooldown(gCooldowns, CHAOS_KEY_ID) == -1 && (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED) && GetNPCFlag(703)){
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

	if (gMC.doctorShock > 0 && Random(0, 2) == 0){
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

	if (gKeyTrg & gKeyAbility && (GetNPCFlag(860) || GetNPCFlag(875) || GetNPCFlag(876)) && 
			CheckForCooldown(gCooldowns, TIME_RING_ID) == -1 && 
			CheckForCooldown(gCooldowns, TIME_STOP_ID) == -1){
		SetFlash(gMC.x, gMC.y, 1, RGB(154, 209, 232) | (0x80 << 24));
		if (GetNPCFlag(860) || GetNPCFlag(876)){
			PlaySoundObject(101, SOUND_MODE_PLAY);
			for (int i = 0; i < SE_MAX; i++)
				ChangeSoundFrequency(i, 1200);
			InitiateCooldown(gCooldowns, TIME_STOP_ID);
			PlayOrganyaMusic();
		} else if (GetNPCFlag(875)){
			PlaySoundObject(118, SOUND_MODE_PLAY);
			for (int i = 0; i < NPC_MAX; i++){
				if (gNPC[i].cond & 0x80 &&
					gMC.x < gNPC[i].x + int(WINDOW_WIDTH/1.5f) * 1_pix && gMC.x > gNPC[i].x - int(WINDOW_WIDTH/1.5f) * 1_pix && gMC.y < gNPC[i].y + int(WINDOW_HEIGHT/1.5f) * 1_pix && gMC.y > gNPC[i].y - int(WINDOW_HEIGHT/1.5f) * 1_pix && gNPC[i].code_char != 4){
						gNPC[i].freeze = 450;
						SetDestroyNpChar(gNPC[i].x, gNPC[i].y, gNPC[i].view.back, 10);
					}
			}
			InitiateCooldown(gCooldowns, TIME_RING_ID);
		}
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

void EquipItem(int flag, bool b)
{
	if (b)
		gMC.equip |= flag;
	else
		gMC.equip &= ~flag;
	IncrementCooldown(gCooldowns, DASH_TIMER_ID, -9999);
}

void EquipItem2(int flag, bool b)
{
	if (b)
		gMC.equip |= 2 << (flag-1);
	else
		gMC.equip &= ~(2 << (flag-1));
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
