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
			if (gBul[i].code_bullet == 89)
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
			
			if (gBul[i].code_bullet >= 49 && gBul[i].code_bullet <= 51 || gBul[i].code_bullet == 73 || gBul[i].code_bullet == 95)
			{
				int targetX = gBul[i].code_bullet == 95 ? gBul[i].tgt_x : gMC.x;
				int targetY = gBul[i].code_bullet == 95 ? gBul[i].tgt_y : gMC.y;
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

BULLET_TABLE gBulTbl[] = {
	// Null
	{},
	// Snake
	{},
	{},
	{},
	// Polar Star
	{},
	{},
	{},
	// Fireball
	{},
	{},
	{},
	// Machine Gun
	{},
	{},
	{},
	// Missile Launcher
	{},
	{},
	{},
	// Missile Launcher explosion
	{},
	{},
	{},
	// Bubbler
	{},
	{},
	{},
	// Bubbler level 3 thorns
	{},
	// Blade slashes
	{},
	// Falling spike that deals 127 damage
	{},
	// Blade
	{},
	{},
	{},
	// Rifle
	{},
	{},
	{},
	// Super Missile Launcher explosion
	{},
	{},
	{},
	// Nemesis
	{},
	{},
	{},
	// Spur
	{},
	{},
	{},
	// Spur trail
	{},
	{},
	{},
	// Curly's Nemesis
	{},
	// Screen-nuke that kills all enemies
	{},
	// Whimsical Star
	{},
	// Little Man
	{},
	// Homing missile
	{},
	{},
	// Hook
	{},
	{},
	{},
	// Boomerang blade
	{}, //level 1
	{}, //level 2
	{}, //level 3
	{}, //level 3 splits
	// Lvl2 boomerang blade flame
	{},
	// Homing sword
	{},
	// Swing sword
	{},
	{},
	{},
	// Swing sword droplet
	{},
	// Whirlpool
	{},
	// Lol
	{},
	// Mining laser
	{},
	{},
	{},
	// Spark
	{},
	// Fireball lv4
	{},
	// Machine gun lv4
	{},
	// Snake lv4
	{},
	// Bubbler lv4
	{},
	// Blade lv4
	{},
	// Hook lv4
	{},
	// Floodtide lv4
	{},
	// Boomerang lv4
	{},
	// Spur lv4
	{},
	// omega selfhurt
	{},
	// red crystal blast
	{},
	// decorative slash
	{},
	// helltide sword
	{},
	{},
	{},
	// hellsweep mine
	{},
	// lvl3 hellsweep flamings
	{},
	// lvl4 Hellsweep
	{},
	// visuals for Nanites Absorber
	{},
	// Frontie-buster lv1
	{},
	// Frontie-buster lv1 homing missile
	{},
	// Frontie-buster lv2
	{},
	// Frontie-buster lv3
	{},
	// Frontie-buster lv4
	{},
	// Gatling Gun
	{},
	// Water Gun projectile
	{},
	// Mannan visuals
	{},
	// Hook lv4 spurts
	{},
};

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
	if (gBulletTable.contains(no)){
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
	} else {
		gBul[i].damage = gBulTbl[no].damage;
		gBul[i].life = gBulTbl[no].life;
		gBul[i].life_count = gBulTbl[no].life_count;
		gBul[i].bbits = gBulTbl[no].bbits;
		gBul[i].enemyXL = gBulTbl[no].enemyXL * 0x200;
		gBul[i].enemyYL = gBulTbl[no].enemyYL * 0x200;
		gBul[i].blockXL = gBulTbl[no].blockXL * 0x200;
		gBul[i].blockYL = gBulTbl[no].blockYL * 0x200;
		gBul[i].view.back = gBulTbl[no].view.back * 0x200;
		gBul[i].view.front = gBulTbl[no].view.front * 0x200;
		gBul[i].view.top = gBulTbl[no].view.top * 0x200;
		gBul[i].view.bottom = gBulTbl[no].view.bottom * 0x200;
		gBul[i].knockX = gBulTbl[no].knockX;
		gBul[i].knockY = gBulTbl[no].knockY;
	}
	if (((no > 0 && no < 4) || no == 70) && gMC.equip & EQUIP_PROPANE_TANK){
		gBul[i].damage /= 2;
	}
	if (gBul[i].life_count != 1 && gBul[i].code_bullet != 58 && gBul[i].code_bullet != 59 && gBul[i].code_bullet != 60){
		if (GetNPCFlag(Flags::BATTERY_PACK)){
			if (GetNPCFlag(Flags::AMPHIBIAN_MEDAL)) gBul[i].life_count *= 1.33;
			else gBul[i].life_count *= 1.25;
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
	if (gBulletTable.contains(no)){
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
	} else {
		gBul[i].damage = gBulTbl[no].damage;
		gBul[i].life = gBulTbl[no].life;
		gBul[i].life_count = gBulTbl[no].life_count;
		gBul[i].bbits = gBulTbl[no].bbits;
		gBul[i].enemyXL = gBulTbl[no].enemyXL * 0x200;
		gBul[i].enemyYL = gBulTbl[no].enemyYL * 0x200;
		gBul[i].blockXL = gBulTbl[no].blockXL * 0x200;
		gBul[i].blockYL = gBulTbl[no].blockYL * 0x200;
		gBul[i].view.back = gBulTbl[no].view.back * 0x200;
		gBul[i].view.front = gBulTbl[no].view.front * 0x200;
		gBul[i].view.top = gBulTbl[no].view.top * 0x200;
		gBul[i].view.bottom = gBulTbl[no].view.bottom * 0x200;
		gBul[i].knockX = gBulTbl[no].knockX;
		gBul[i].knockY = gBulTbl[no].knockY;
	}
	if (((no > 0 && no < 4) || no == 70) && gMC.equip & EQUIP_PROPANE_TANK){
		gBul[i].damage /= 2;
	}
	if (gBul[i].life_count != 1 && gBul[i].code_bullet != 58 && gBul[i].code_bullet != 59 && gBul[i].code_bullet != 60){
		if (GetNPCFlag(Flags::BATTERY_PACK)){
			if (GetNPCFlag(Flags::AMPHIBIAN_MEDAL)) gBul[i].life_count *= 1.33;
			else gBul[i].life_count *= 1.25;
		}
	}
	gBul[i].x = x;
	gBul[i].y = y;
	gBul[i].iframes = 0;
	gBul[i].opacity = 1.0f;
	return &gBul[i];
}

bool IsBreakingBullet(BULLET &bul){
	return (bul.code_bullet == Bullets::MINING_LASER || bul.code_bullet == Bullets::MINING_BOOM || bul.code_bullet == 34 || bul.code_bullet == 35 ||
	 ((GetNPCFlag(Flags::MISERY_CHARACTER) || GetNPCFlag(Flags::MAHIN_CHARACTER)) && (bul.code_bullet == 37 || bul.code_bullet == 38 || bul.code_bullet == 39)));
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
