#include "Bullet.h"

#include <cmath>
#define M_PI   3.14159265358979323846264338327950288
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)
#include <cstddef>
#include <cstring>
#include <functional>
#include <map>

#include "ArmsItem.h"
#include "Boss.h"
#include "BossLife.h"
#include "Draw.h"
#include "Caret.h"
#include "Game.h"
#include "Flags.h"
#include "Frame.h"
#include "Main.h"
#include "Map.h"
#include "KeyControl.h"
#include "MyChar.h"
#include "MycParam.h"
#include "NpChar.h"
#include "Sound.h"
#include "Stage.h"
#include "Triangle.h"
#include "TextScr.h"

BULLET gBul[BULLET_MAX];

double pointDirection(int x1, int y1, int x2, int y2){
	return -radToDeg(atan2(y1 - y2, x1 - x2)) + 180;
}

void InitBullet(void)
{
	// Identical to ClearBullet
	int i;

	for (i = 0; i < BULLET_MAX; ++i)
		gBul[i].cond = 0;
}

int CountArmsBullet(int arms_code)
{
	int i;
	int count = 0;

	for (i = 0; i < BULLET_MAX; ++i)
		if (gBul[i].cond & 0x80 && (gBul[i].code_bullet + 2) / 3 == arms_code)
			++count;

	return count;
}

int CountBulletNum(int bullet_code)
{
	int i;
	int count = 0;

	for (i = 0; i < BULLET_MAX; ++i)
		if (gBul[i].cond & 0x80 && gBul[i].code_bullet == bullet_code)
			++count;

	return count;
}

void DeleteBullet(int code)
{
	int i;
	int count = 0;	// Guessed name. This is unused, and was optimised out of the Linux port.

	for (i = 0; i < BULLET_MAX; ++i)
		if (gBul[i].cond & 0x80 && (gBul[i].code_bullet + 2) / 3 == code)
				gBul[i].cond = 0;
}

void DeleteSpecificBullet(int code)
{
	for (int i = 0; i < BULLET_MAX; ++i)
		if (gBul[i].cond & 0x80 && gBul[i].code_bullet == code)
			gBul[i].cond = 0;
}

void ClearBullet(void)
{
	// Identical to InitBullet
	int i;

	for (i = 0; i < BULLET_MAX; ++i)
		gBul[i].cond = 0;
}

void PutBullet(int fx, int fy)
{
	int i;
	int x, y;

	for (i = 0; i < BULLET_MAX; ++i)
	{
		if (gBul[i].cond & 0x80)
		{
			switch (gBul[i].direct)
			{
				case 0: case 4:
					x = gBul[i].x - gBul[i].view.front;
					y = gBul[i].y - gBul[i].view.top;
					break;
				case 1: case 5:
					x = gBul[i].x - gBul[i].view.top;
					y = gBul[i].y - gBul[i].view.front;
					break;
				case 2: case 6:
					x = gBul[i].x - gBul[i].view.back;
					y = gBul[i].y - gBul[i].view.top;
					break;
				case 3: case 7:
					x = gBul[i].x - gBul[i].view.top;
					y = gBul[i].y - gBul[i].view.back;
					break;
			}
			if (gBul[i].code_bullet == Bullets::FRONTIER_BUSTER_LVL_2)
			{
				Rect neededRect[3][3] = {
						{{128, 160, 144, 176}, {144, 160, 160, 176}, {160, 160, 176, 176}},
						{{176, 144, 192, 160}, {192, 144, 208, 160}, {208, 144, 224, 160}},
						{{176, 160, 192, 176}, {192, 160, 208, 176}, {208, 160, 224, 176}}
				};
				for (int m = 3; m != 0; m--){
					int direction;
					switch (gBul[i].direct){
						case 0: case 1: direction = -1; break;
						case 2: case 3: direction = +1; break;
					}
					Rect necessaryRect = neededRect[m-1][gBul[i].ani_no];
					PutBitmap3(&grcGame, SubpixelToScreenCoord(x - gBul[i].xm * m) - SubpixelToScreenCoord(fx), 
					SubpixelToScreenCoord(y - gBul[i].ym * m) - SubpixelToScreenCoord(fy), &necessaryRect, SURFACE_ID_BULLET);
				}
			}
			PutBitmap3Ex(&grcGame, SubpixelToScreenCoord(x) - SubpixelToScreenCoord(fx), SubpixelToScreenCoord(y) - SubpixelToScreenCoord(fy), &gBul[i].rect, SURFACE_ID_BULLET, gBul[i].angle, gBul[i].centerX, gBul[i].centerY, gBul[i].opacity);
			
			if (gBul[i].code_bullet >= Bullets::ENERGY_FLAIL_LVL_1 && gBul[i].code_bullet <= Bullets::ENERGY_FLAIL_LVL_3 || gBul[i].code_bullet == Bullets::ENERGY_FLAIL_LVL_4 || gBul[i].code_bullet == Bullets::ENERGY_FLAIL_LVL_4_SPAWN)
			{
				int targetX = gBul[i].code_bullet == Bullets::ENERGY_FLAIL_LVL_4_SPAWN ? gBul[i].tgt_x : gMC.x;
				int targetY = gBul[i].code_bullet == Bullets::ENERGY_FLAIL_LVL_4_SPAWN ? gBul[i].tgt_y : gMC.y;
				long long xDist = targetX - gBul[i].x;
				long long yDist = targetY - gBul[i].y;
				int chainWidth = PixelToScreenCoord(8);
				long long subpixelLength = abs(sqrt(xDist * xDist + yDist * yDist));
				int chainLength = SubpixelToScreenCoord(subpixelLength);
				double chainAngle = degToRad(pointDirection(targetX, targetY, x, y));
				Rect chainRect = {223, 83, 236, 92};
				for (int i = 0; i < chainLength; i += chainWidth)
				{
					int c_x = SubpixelToScreenCoord(targetX) + (cos(chainAngle) * i) + (cos(chainAngle) * (chainWidth / 2)) - SubpixelToScreenCoord(fx);
					int c_y = (SubpixelToScreenCoord(targetY) - (sin(chainAngle) * i)) + (sin(chainAngle) * (chainWidth / 2)) - SubpixelToScreenCoord(fy);
					PutBitmap3(&grcGame, c_x, c_y, &chainRect, SURFACE_ID_CARET);
				}
			}

			if (ChallengeEnabled("hitboxes"))
			{
				int hitX = gBul[i].x + gBul[i].enemyXL - fx;
				int hitY = gBul[i].y + gBul[i].enemyYL - fy;

				Rect hitbox = {
					(hitX - gBul[i].enemyXL * 2) / 0x200,
					(hitY - gBul[i].enemyYL * 2) / 0x200,
					hitX / 0x200,
					hitY / 0x200
				};
				CortBox2(&hitbox, 0x7F00FF00);
			}
		}
	}
}

std::map<decltype(Bullets::NOTHING), BULLET_DEF> gBulletTable;

void SetBullet(int no, int x, int y, int dir)
{
	int i = 0;
	while (i < BULLET_MAX && gBul[i].cond & 0x80)
		++i;

	if (i >= BULLET_MAX)
		return;

	memset(&gBul[i], 0, sizeof(BULLET));
	gBul[i].code_bullet = no;
	gBul[i].first_frame = true;
	gBul[i].cond = 0x80;
	gBul[i].direct = dir;
	gBul[i].damage = gBulletTable[no].table.damage;
	gBul[i].life = gBulletTable[no].table.life;
	gBul[i].life_count = gBulletTable[no].table.life_count;
	gBul[i].bbits = gBulletTable[no].table.bbits;
	gBul[i].enemyXL = gBulletTable[no].table.enemyXL * 0x200;
	gBul[i].enemyYL = gBulletTable[no].table.enemyYL * 0x200;
	gBul[i].blockXL = gBulletTable[no].table.blockXL * 0x200;
	gBul[i].blockYL = gBulletTable[no].table.blockYL * 0x200;
	gBul[i].view.back = gBulletTable[no].table.view.back * 0x200;
	gBul[i].view.front = gBulletTable[no].table.view.front * 0x200;
	gBul[i].view.top = gBulletTable[no].table.view.top * 0x200;
	gBul[i].view.bottom = gBulletTable[no].table.view.bottom * 0x200;
	gBul[i].knockX = gBulletTable[no].table.knockX;
	gBul[i].knockY = gBulletTable[no].table.knockY;
	if (((no > 0 && no < 4) || no == 70) && CheckFlagTrait(FlagTraits::SNAKE_EMPOWER)){
		gBul[i].damage /= 2;
	}
	if (gBul[i].life_count != Bullets::SNAKE_LVL_1 && gBul[i].code_bullet != Bullets::FLOODTIDE_LVL_1 && gBul[i].code_bullet != Bullets::FLOODTIDE_LVL_2 && gBul[i].code_bullet != Bullets::FLOODTIDE_LVL_3){
		if (CheckFlagTrait(FlagTraits::BULLET_LIFE_BOOST)){
			gBul[i].life_count *= GetFlagTraitData<float>(FlagTraits::BULLET_LIFE_BOOST, "boost");
		}
	}
	gBul[i].x = x;
	gBul[i].y = y;
	gBul[i].iframes = 0;
	gBul[i].opacity = 1.0f;
}

BULLET* SetBullet(int no, int x, int y)
{
	int i = 0;
	while (i < BULLET_MAX && gBul[i].cond & 0x80)
		++i;

	if (i >= BULLET_MAX)
		return NULL;

	memset(&gBul[i], 0, sizeof(BULLET));
	gBul[i].code_bullet = no;
	gBul[i].first_frame = true;
	gBul[i].cond = 0x80;
	gBul[i].damage = gBulletTable[no].table.damage;
	gBul[i].life = gBulletTable[no].table.life;
	gBul[i].life_count = gBulletTable[no].table.life_count;
	gBul[i].bbits = gBulletTable[no].table.bbits;
	gBul[i].enemyXL = gBulletTable[no].table.enemyXL * 0x200;
	gBul[i].enemyYL = gBulletTable[no].table.enemyYL * 0x200;
	gBul[i].blockXL = gBulletTable[no].table.blockXL * 0x200;
	gBul[i].blockYL = gBulletTable[no].table.blockYL * 0x200;
	gBul[i].view.back = gBulletTable[no].table.view.back * 0x200;
	gBul[i].view.front = gBulletTable[no].table.view.front * 0x200;
	gBul[i].view.top = gBulletTable[no].table.view.top * 0x200;
	gBul[i].view.bottom = gBulletTable[no].table.view.bottom * 0x200;
	gBul[i].knockX = gBulletTable[no].table.knockX;
	gBul[i].knockY = gBulletTable[no].table.knockY;
	if (((no > 0 && no < 4) || no == 70) && CheckFlagTrait(FlagTraits::SNAKE_EMPOWER)){
		gBul[i].damage /= 2;
	}
	if (gBul[i].life_count != Bullets::SNAKE_LVL_1 && gBul[i].code_bullet != Bullets::FLOODTIDE_LVL_1 && gBul[i].code_bullet != Bullets::FLOODTIDE_LVL_2 && gBul[i].code_bullet != Bullets::FLOODTIDE_LVL_3){
		if (CheckFlagTrait(FlagTraits::BULLET_LIFE_BOOST)){
			gBul[i].life_count *= GetFlagTraitData<float>(FlagTraits::BULLET_LIFE_BOOST, "boost");
		}
	}
	gBul[i].x = x;
	gBul[i].y = y;
	gBul[i].iframes = 0;
	gBul[i].opacity = 1.0f;
	return &gBul[i];
}

bool IsBreakingBullet(BULLET &bul){
	return (bul.code_bullet == Bullets::MINING_LASER || bul.code_bullet == Bullets::MINING_BOOM || bul.code_bullet == Bullets::NEMESIS_MAIN ||
	 (CheckFlagTrait(FlagTraits::INVIDIA) && (bul.code_bullet == Bullets::SPUR_LVL_1 || bul.code_bullet == Bullets::SPUR_LVL_2 || bul.code_bullet == Bullets::SPUR_LVL_3)));
}

void ActBullet(void)
{
	int i;

	for (i = 0; i < BULLET_MAX; ++i)
	{
		if (gBul[i].cond & 0x80)
		{
			if (gBul[i].life < 1)
			{
				gBul[i].cond = 0;
				continue;
			}

			if (gBul[i].first_frame || !TimeStopActive(gBul[i].code_bullet + 700)){
				gBul[i].first_frame = false;
				if (gBulletTable.contains(gBul[i].code_bullet)){
					gBulletTable[gBul[i].code_bullet].act(&gBul[i]);
				}
			}
		}
	}
}

bool IsActiveSomeBullet(void)
{
	int i;

	for (i = 0; i < BULLET_MAX; ++i)
	{
		if (gBul[i].cond & 0x80)
		{
			switch (gBul[i].code_bullet)
			{
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				case 23:
				case 25:
				case 26:
				case 27:
				case 31:
				case 32:
				case 33:
					return true;
			}
		}
	}

	return false;
}
