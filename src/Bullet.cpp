#include "Bullet.h"

#include <cmath>
#define M_PI   3.14159265358979323846264338327950288
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)
#include <cstddef>
#include <cstring>

#include "ArmsItem.h"
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
			
			if (gBul[i].code_bullet >= 49 && gBul[i].code_bullet <= 51 || gBul[i].code_bullet == 73)
			{
				long long xDist = gMC.x - gBul[i].x;
				long long yDist = gMC.y - gBul[i].y;
				int chainWidth = PixelToScreenCoord(8);
				long long subpixelLength = abs(sqrt(xDist * xDist + yDist * yDist));
				int chainLength = SubpixelToScreenCoord(subpixelLength);
				double chainAngle = degToRad(pointDirection(gMC.x, gMC.y, x, y));
				Rect chainRect = {223, 83, 236, 292};
				for (int i = 0; i < chainLength; i += chainWidth)
				{
					int c_x = SubpixelToScreenCoord(gMC.x) + (cos(chainAngle) * i) + (cos(chainAngle) * (chainWidth / 2)) - SubpixelToScreenCoord(fx);
					int c_y = (SubpixelToScreenCoord(gMC.y) - (sin(chainAngle) * i)) + (sin(chainAngle) * (chainWidth / 2)) - SubpixelToScreenCoord(fy);
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

BULLET_TABLE gBulTbl[] = {
	// Null
	{0, 0, 0, 0, 0, 0, 0, 0, {0, 0, 0, 0}, 1, 1},
	// Snake
	{4, 1, 9, 36, 5, 5, 2, 2, {8, 8, 8, 8}, 1, 1},
	{5, 2, 12, 36, 5, 5, 2, 2, {8, 8, 8, 8}, 1, 1},
	{5, 3, 15, 36, 5, 5, 2, 2, {8, 8, 8, 8}, 1, 1},
	// Polar Star
	{6, 2, 11, 32, 6, 6, 2, 2, {8, 8, 8, 8}, 0.6, 0.6},
	{2, 1, 4, 32, 6, 6, 2, 2, {8, 8, 8, 8}, 1, 1},
	{4, 1, 15, 32, 6, 6, 2, 2, {8, 8, 8, 8}, 0.4, 0.4},
	// Fireball
	{2, 2, 120, 8, 8, 16, 4, 2, {8, 8, 8, 8}, 0.8, 1},
	{3, 2, 120, 8, 4, 4, 4, 2, {8, 8, 8, 8}, 0.9, 1},
	{4, 2, 120, 8, 4, 4, 4, 2, {8, 8, 8, 8}, 0.9, 1},
	// Machine Gun
	{3, 3, 22, 32, 4, 2, 2, 2, {8, 8, 8, 8}, 0.14, 0.14},
	{5, 3, 22, 32, 4, 2, 2, 2, {8, 8, 8, 8}, 0.18, 0.18},
	{7, 3, 22, 32, 4, 2, 2, 2, {8, 8, 8, 8}, 0.29, 0.29},
	// Missile Launcher
	{30, 127, 50, 40, 2, 2, 2, 2, {8, 8, 8, 8}, 1, 1},
	{1, 127, 70, 40, 10, 10, 4, 4, {8, 8, 8, 8}, 1, 1},
	{30, 127, 90, 40, 4, 4, 0, 0, {8, 8, 8, 8}, 1, 1},
	// Missile Launcher explosion
	{1, 100, 100, 20, 16, 16, 0, 0, {0, 0, 0, 0}, 0, 0},
	{1, 100, 100, 20, 16, 16, 0, 0, {0, 0, 0, 0}, 0, 0},
	{1, 100, 100, 20, 16, 16, 0, 0, {0, 0, 0, 0}, 0, 0},
	// Bubbler
	{0, 1, 30, 8, 8, 8, 2, 2, {4, 4, 4, 4}, 2, 2},
	{2, 1, 15, 8, 2, 2, 2, 2, {4, 4, 4, 4}, 0.3, 0.3},
	{1, 1, 22, 8, 4, 4, 4, 4, {4, 4, 4, 4}, 0.25, 0.25},
	// Bubbler level 3 thorns
	{3, 1, 32, 32, 2, 2, 2, 2, {4, 4, 4, 4}, 0.33, 0.33},
	// Blade slashes
	{0, 100, 0, 36, 8, 8, 8, 8, {12, 12, 12, 12}, 0, 0},
	// Falling spike that deals 127 damage
	{127, 1, 2, 4, 8, 4, 8, 4, {0, 0, 0, 0}, 1, 1},
	// Blade
	{1, 100, 32, 36, 6, 6, 4, 4, {12, 12, 12, 12}, 1.2, 1.2},
	{10, 3, 1, 32, 10, 10, 4, 2, {12, 12, 12, 12}, 0.1, 0.1},
	{2, 100, 37, 36, 6, 6, 4, 4, {12, 12, 12, 12}, 1.66, 1.66},
	// Rifle
	{10, 2, 27, 32, 6, 6, 2, 2, {8, 8, 8, 8}, 0.3, 0.4},
	{14, 3, 36, 32, 6, 6, 2, 2, {8, 8, 8, 8}, 0.3, 0.4},
	{22, 4, 52, 32, 6, 6, 2, 2, {8, 8, 8, 8}, 0.3, 0.4},
	// Super Missile Launcher explosion
	{2, 100, 100, 20, 12, 12, 0, 0, {0, 0, 0, 0}, 1, 1},
	{2, 100, 100, 20, 20, 20, 0, 0, {0, 0, 0, 0}, 1, 1},
	{2, 100, 100, 20, 12, 12, 0, 0, {0, 0, 0, 0}, 1, 1},
	// Nemesis
	{3, 3, 22, 32, 4, 4, 3, 3, {8, 8, 8, 8}, 0.75, 0.75},
	{4, 2, 22, 32, 2, 2, 2, 2, {8, 8, 8, 8}, 1, 1},
	{2, 1, 22, 32, 4, 4, 4, 4, {8, 8, 8, 8}, 0.25, 0.25},
	// Spur
	{4, 4, 45, 64, 6, 6, 3, 3, {8, 8, 8, 8}, 1, 1},
	{8, 8, 60, 64, 6, 6, 3, 3, {8, 8, 8, 8}, 1, 1},
	{12, 12, 90, 64, 6, 6, 3, 3, {8, 8, 8, 8}, 1, 1},
	// Spur trail
	{3, 100, 45, 32, 6, 6, 3, 3, {4, 4, 4, 4}, 1, 1},
	{6, 100, 45, 32, 6, 6, 3, 3, {4, 4, 4, 4}, 1, 1},
	{11, 100, 45, 32, 6, 6, 3, 3, {4, 4, 4, 4}, 1, 1},
	// Curly's Nemesis
	{4, 4, 20, 32, 4, 4, 3, 3, {8, 8, 24, 8}, 1.4, 1.4},
	// Screen-nuke that kills all enemies
	{0, 4, 4, 4, 0, 0, 0, 0, {0, 0, 0, 0}, 1, 1},
	// Whimsical Star
	{1, 1, 100, 8, 8, 8, 8, 8, {4, 2, 5, 2}, 0.5, 0.5},
	// Little Man
	{1, 1, 1, 36, 4, 4, 4, 4, {1, 1, 1, 1}, 0.22, 0.22},
	// Homing missile
	{2, 1, 30, 36, 4, 4, 2, 2, {8, 8, 8, 8}, 0, 0},
	{2, 1, 30, 36, 4, 4, 2, 2, {8, 8, 8, 8}, 0, 0},
	// Hook
	{10, 100, 15, 105, 6, 6, 4, 4, {4, 4, 4, 4}, 0.33, 0.33},
	{13, 100, 20, 105, 8, 8, 4, 4, {4, 4, 4, 4}, 0.33, 0.33},
	{15, 100, 26, 105, 12, 12, 4, 4, {4, 4, 4, 4}, 0.33, 0.33},
	// Boomerang blade
	{4, 100, 50, 44, 8, 8, 4, 2, {8, 8, 8, 8}, 0.75, 0.75}, //level 1
	{8, 100, 13, 44, 10, 10, 4, 2, {12, 12, 12, 12}, 0.75, 0.75}, //level 2
	{7, 100, 30, 36, 10, 10, 4, 2, {12, 12, 12, 12}, 0.75, 0.75}, //level 3
	{15, 100, 19, 44, 8, 8, 4, 2, {8, 8, 8, 8}, 0.75, 0.75}, //level 3 splits
	// Lvl2 boomerang blade flame
	{3, 1, 40, 8, 8, 8, 8, 8, {4, 2, 5, 2}, 0.5, 0.5},
	// Homing sword
	{4, 100, 0, 52, 10, 10, 8, 8, {12, 12, 12, 12}, 0.1, 0.1},
	// Swing sword
	{6, 100, 24, 52, 12, 10, 4, 2, {8, 8, 8, 8}, 1, 1},
	{9, 100, 19, 52, 13, 12, 4, 2, {10, 10, 10, 10}, 1, 1},
	{12, 100, 17, 52, 14, 13, 4, 2, {12, 12, 12, 12}, 1, 1},
	// Swing sword droplet
	{-1, 1, 35, 8, 8, 8, 8, 8, {4, 2, 5, 2}, 0.5, 0.5},
	// Whirlpool
	{3, 100, 200, 8, 8, 8, 5, 5, {5, 5, 5, 5}, 0.25, 0.25},
	// Lol
	{0, 0, 0, 0, 0, 0, 0, 0, {0, 0, 0, 0}, 1, 1},
	// Mining laser
	{9, 100, 30, 32, 2, 2, 1, 1, {8, 8, 8, 8}, 0, 0},
	{9, 100, 40, 40, 4, 4, 4, 4, {8, 8, 8, 8}, 0, 0},
	{9, 100, 40, 40, 4, 4, 0, 0, {8, 8, 8, 8}, 0, 0},
	// Spark
	{1, 1, 45, 32, 4, 4, 4, 4, {4, 4, 4, 4}, 0.25, 0.25},
	// Fireball lv4
	{5, 2, 120, 8, 4, 4, 4, 2, {8, 8, 8, 8}, 0.92, 0.95},
	// Machine gun lv4
	{9, 6, 30, 32, 2, 2, 2, 2, {8, 8, 8, 8}, 0.29, 0.29},
	// Snake lv4
	{5, 4, 19, 36, 5, 5, 2, 2, {8, 8, 8, 8}, 1, 1},
	// Bubbler lv4
	{1, 100, 20, 8, 4, 4, 4, 4, {4, 4, 4, 4}, 0.25, 0.25},
	// Blade lv4
	{3, 100, 35, 36, 6, 6, 4, 4, {12, 12, 12, 12}, 1, 1},
	// Hook lv4
	{1, 100, 6, 105, 12, 12, 4, 4, {4, 4, 4, 4}, 0.33, 0.33},
	// Floodtide lv4
	{15, 100, 14, 52, 12, 15, 4, 2, {12, 12, 12, 12}, 1, 1},
	// Boomerang lv4
	{6, 100, 30, 44, 10, 10, 4, 2, {12, 12, 12, 12}, 0.75, 0.75},
	// Spur lv4
	{13, 13, 120, 64, 6, 6, 3, 3, {8, 8, 8, 8}, 1, 1},
	// omega selfhurt
	{3, 1, 1, 36, 4, 4, 4, 4, {1, 1, 1, 1}, 0.22, 0.22},
	// red crystal blast
	{4, 1, 20, 32, 2, 2, 2, 2, {8, 8, 8, 8}, 0.14, 0.14},
	// decorative slash
	{0, 100, 0, 36, 8, 8, 8, 8, {12, 12, 12, 12}, 0, 0},
	// helltide sword
	{5, 100, 26, 52, 10, 12, 5, 3, {8, 8, 8, 8}, 1, 1},
	{7, 100, 20, 52, 12, 14, 5, 3, {10, 10, 10, 10}, 1, 1},
	{10, 100, 18, 52, 14, 17, 5, 3, {12, 12, 12, 12}, 1, 1},
	// hellsweep mine
	{-1, 2, 100, 8, 8, 16, 4, 2, {8, 8, 8, 8}, 0.8, 1},
	// lvl3 hellsweep flamings
	{-1, 100, 100, 8, 8, 8, 8, 8, {4, 2, 5, 2}, 0.5, 0.5},
	// lvl4 Hellsweep
	{11, 100, 16, 52, 14, 18, 5, 3, {12, 12, 12, 12}, 1, 1},
	// visuals for Nanites Absorber
	{-1, 2, 100, 12, 8, 16, 4, 2, {8, 8, 8, 8}, 0.8, 1},
	// Frontie-buster lv1
	{2, 1, 100, 32, 8, 16, 4, 2, {8, 8, 8, 8}, 0.8, 1},
	// Frontie-buster lv1 homing missile
	{7, 1, 100, 32, 10, 10, 4, 4, {12, 12, 12, 12}, 0.1, 0.1},
	// Frontie-buster lv2
	{1, 3, 120, 32, 4, 4, 2, 2, {8, 8, 8, 8}, 0.5, 0.5},
	// Frontie-buster lv3
	{1, 100, 30, 36, 8, 16, 4, 2, {8, 8, 8, 8}, 0.8, 1},
	// Frontie-buster lv4
	{1, 15, 40, 32, 10, 10, 1, 1, {12, 12, 12, 12}, 0.1, 0.1}
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
	gBul[i].damage = gBulTbl[no].damage;
	if (((no > 0 && no < 4) || no == 70) && gMC.equip & EQUIP_PROPANE_TANK){
		gBul[i].damage /= 2;
	}
	gBul[i].life = gBulTbl[no].life;
	gBul[i].life_count = gBulTbl[no].life_count;
	if (gBul[i].life_count != 1 && gBul[i].code_bullet != 58 && gBul[i].code_bullet != 59 && gBul[i].code_bullet != 60){
		if (GetNPCFlag(705)){
			if (GetNPCFlag(872)) gBul[i].life_count *= 1.33;
			else gBul[i].life_count *= 1.25;
		}
	}
	gBul[i].bbits = gBulTbl[no].bbits;
	gBul[i].enemyXL = gBulTbl[no].enemyXL * 0x200;
	gBul[i].enemyYL = gBulTbl[no].enemyYL * 0x200;
	gBul[i].blockXL = gBulTbl[no].blockXL * 0x200;
	gBul[i].blockYL = gBulTbl[no].blockYL * 0x200;
	gBul[i].view.back = gBulTbl[no].view.back * 0x200;
	gBul[i].view.front = gBulTbl[no].view.front * 0x200;
	gBul[i].view.top = gBulTbl[no].view.top * 0x200;
	gBul[i].view.bottom = gBulTbl[no].view.bottom * 0x200;
	gBul[i].x = x;
	gBul[i].y = y;
	gBul[i].knockX = gBulTbl[no].knockX;
	gBul[i].knockY = gBulTbl[no].knockY;
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
	gBul[i].damage = gBulTbl[no].damage;
	gBul[i].life = gBulTbl[no].life;
	gBul[i].life_count = gBulTbl[no].life_count;
	if (gBul[i].life_count != 1 && gBul[i].code_bullet != 58 && gBul[i].code_bullet != 59 && gBul[i].code_bullet != 60){
		if (GetNPCFlag(705)){
			if (GetNPCFlag(872)) gBul[i].life_count *= 1.33;
			else gBul[i].life_count *= 1.25;
		}
	}
	gBul[i].bbits = gBulTbl[no].bbits;
	gBul[i].enemyXL = gBulTbl[no].enemyXL * 0x200;
	gBul[i].enemyYL = gBulTbl[no].enemyYL * 0x200;
	gBul[i].blockXL = gBulTbl[no].blockXL * 0x200;
	gBul[i].blockYL = gBulTbl[no].blockYL * 0x200;
	gBul[i].view.back = gBulTbl[no].view.back * 0x200;
	gBul[i].view.front = gBulTbl[no].view.front * 0x200;
	gBul[i].view.top = gBulTbl[no].view.top * 0x200;
	gBul[i].view.bottom = gBulTbl[no].view.bottom * 0x200;
	gBul[i].x = x;
	gBul[i].y = y;
	gBul[i].knockX = gBulTbl[no].knockX;
	gBul[i].knockY = gBulTbl[no].knockY;
	gBul[i].iframes = 0;
	gBul[i].opacity = 1.0f;
	return &gBul[i];
}

bool IsBreakingBullet(BULLET &bul){
	return (bul.code_bullet == 64 || bul.code_bullet == 65 || bul.code_bullet == 34 || bul.code_bullet == 35 ||
	 ((GetNPCFlag(4400) || GetNPCFlag(7999)) && (bul.code_bullet == 37 || bul.code_bullet == 38 || bul.code_bullet == 39)));
}

void ActBullet_Frontia1(BULLET *bul)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->ani_no = Random(0, 2);
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0xF00;
				break;
			case 1:
				bul->ym = -0xF00;
				break;
			case 2:
				bul->xm = 0xF00;
				break;
			case 3:
				bul->ym = 0xF00;
				break;
		}
	}
	else
	{
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	if (++bul->ani_wait > 0)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 0;

	Rect rcLeft[4] = {
		{136, 80, 152, 80},
		{120, 80, 136, 96},
		{136, 64, 152, 80},
		{120, 64, 136, 80},
	};

	Rect rcRight[4] = {
		{120, 64, 136, 80},
		{136, 64, 152, 80},
		{120, 80, 136, 96},
		{136, 80, 152, 80},
	};

	if (bul->direct == 0)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void ActBullet_Duck(BULLET *bul)
{
	if (bul->act_no == 0)
	{
		bul->ani_no = Random(0, 2);
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = Random(-0x800, -0x200);
				bul->ym = Random(-0x200, -0x80);
				break;
			case 1:
				bul->ym = Random(-0x800, -0x200);
				bul->xm = Random(-0x200, -0x202);
				break;
			case 2:
				bul->xm = Random(0x200, 0x800);
				bul->ym = Random(-0x200, -0x80);
				break;
			case 3:
				bul->ym = Random(0x100, 0x500);
				bul->xm = Random(-0x200, -0x202);
				break;
		}
	}
	else
	{
		bul->ym += 50;
		if (bul->ym >= 0x6FF)
			bul->ym = 0x6FF;
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	if (++bul->ani_wait > 10)
	{
		bul->ani_wait = 0;
		if (bul->ani_no == 0)
			bul->ani_no = 1;
		else
			bul->ani_no = 0;
	}

	Rect rcs[][2] = {
		{{30, 147, 42, 157}, {30, 162, 42, 172}},
		{{66, 147, 76, 159}, {51, 147, 61, 159}},
		{{78, 147, 90, 157}, {78, 162, 90, 172}},
		{{50, 161, 60, 173}, {65, 161, 75, 173}}
	};

	bul->rect = rcs[bul->direct][bul->ani_no];
}

void ActBullet_Homing(BULLET *bul)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->ani_no = Random(0, 2);
		bul->act_no = 1;

		int deg = GetArktan(bul->x - bul->tgt_x, bul->y - bul->tgt_y);
		if (bul->direct != 0)
			deg = GetArktan(bul->tgt_x - bul->x, bul->tgt_y - bul->y);
		deg += (unsigned char)Random(-6, 6);
		bul->ym = GetSin(deg) * 8;
		bul->xm = GetCos(deg) * 8;
	}
	else
	{
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	if (++bul->ani_wait > 0)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}
	if (bul->count1 % 2 == 0)
	SetCaret(bul->x, bul->y, 3, 0);

	if (bul->ani_no > 2)
		bul->ani_no = 0;

	Rect rcLeft[3] = {
		{130, 144, 142, 158},
		{147, 147, 157, 157},
		{162, 146, 174, 159}
	};
		
	bul->rect = rcLeft[bul->ani_no];
}

void ActBullet_Hook(BULLET *bul, int level)
{
	bul->iframes = 25;

	if (GetNPCFlag(4450) || GetNPCFlag(7999))
	{
		bul->blockXL = 0x200 * 4;
		bul->blockYL = 0x200 * 4;
		if ((++bul->count1 > bul->life_count) && bul->act_no == 1)
		{
			bul->act_no = 2;
		}
		if (bul->flag & 0xF && bul->act_no == 1){
			bul->act_no = 3;
		}
	}
	else if ((++bul->count1 > bul->life_count || bul->flag & 0xF) && bul->act_no == 1)
	{
		bul->act_no = 2;
	}
	// if (bul->flag & 0xF){
	// 	bul->act_no = 3;
	// 	chainWrap = true;
	// }

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		int deg = GetArktan(bul->x - bul->tgt_x, bul->y - bul->tgt_y);
		if (!GetNPCFlag(4450))
			deg += Random(-1, 1);
		int speed = 11;
		if (level == 3){
			speed = 8.75;
			bul->life_count *= 2;
		}
		if (level == 4){
			speed = 12;
		}
		bul->ym = GetSin(deg) * speed;
		bul->xm = GetCos(deg) * speed;
	}
	else
	{
		if (bul->act_no == 3){
			if (bul->count2 == 0){
				bul->count2 = 1;
			for (int i = 0; i < BULLET_MAX; ++i)
				if (gBul[i].cond & 0x80 && (gBul[i].code_bullet + 2) / 3 == 17 && &gBul[i] != bul)
				{
					gBul[i].cond = 0;
					break;
				}
			}
			if (bul->x > gMC.x - 0x200*12 && bul->x < gMC.x + 0x200*12 &&
					 bul->y > gMC.y - 0x200*12 && bul->y < gMC.y + 0x200*12){
				gMC.xm = 0;
				gMC.ym = 0;
				bul->xm = 0;
				bul->ym = 0;
			} else {
				gMC.xm = 0;
				gMC.ym = 0;
			int speed = 6 + level*3;
			int deg = GetArktan(gMC.x - bul->x, gMC.y - bul->y);
			bul->ym = GetSin(deg) * speed;
			bul->xm = GetCos(deg) * speed;
			gMC.x += bul->xm;
			gMC.y += bul->ym;
			}
		}
		if (bul->act_no == 2){
			if (bul->x > gMC.x - 0x200*8 && bul->x < gMC.x + 0x200*8 &&
					 bul->y > gMC.y - 0x200*8 && bul->y < gMC.y + 0x200*8){
				bul->cond = 0;
				chainWrap = false;
				return;
			}
			int speed = 22;
			if (level == 3){
				speed = 17;
			}
			if (level == 4){
				speed = 30;
			}
			int deg = GetArktan(bul->x - gMC.x, bul->y - gMC.y);
			deg += Random(-2, 2);
			bul->ym = GetSin(deg) * speed;
			bul->xm = GetCos(deg) * speed;
		}
		if (bul->act_no != 3){
		if (bul->life != 100 && level == 4)
		{
			bul->life = 100;
			int x, y;
			do
			{
				x = Random(0, gMap.width);
				y = Random(0, gMap.length);
			} while (!(Distance(bul->x / 0x2000, bul->y / 0x2000, x, y) < 9 &&
					   Distance(bul->x / 0x2000, bul->y / 0x2000, x, y) > 3));
			BULLET *bully = SetBullet(49, x * 0x2000, y * 0x2000);
			if (bully != NULL)
			{
				bully->tgt_x = bul->x;
				bully->tgt_y = bul->y;
			}
		}
		bul->x += bul->xm;
		bul->y += bul->ym;
		if (level == 3){
			if (bul->count1 % 5 == 0){
				SetBullet(23, bul->x, bul->y, 0);
			}
			else if (bul->count1 % 5 == 1){
				SetBullet(23, bul->x, bul->y, 2);
			}
		}
		}
		if (bul->act_no == 1 || bul->act_no == 3 && gMouseHold){
		if (bul->xm > 0){
			gMC.direct = 2;
		} else {
			gMC.direct = 0;
		}
		if (bul->ym < 0){
			gMC.up = true;
		} else if (bul->ym > 0 && gMC.ym > 0){
			gMC.down = true;
		}
		}
	}

	// CARET* crt = SetCaret(bul->x, bul->y, 18);
	// crt->xm = bul->x;
	// crt->ym = bul->y;
	// crt->act_no = bul->count1 % 50;
		
	bul->rect = {130, 144, 142, 158};
}

void ActBullet_Frontia2(BULLET *bul, int level)
{
	static unsigned int inc;

	bul->local_iframes = 15;

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->ani_no = Random(0, 2);
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x500;
				break;
			case 1:
				bul->ym = -0x500;
				break;
			case 2:
				bul->xm = 0x500;
				break;
			case 3:
				bul->ym = 0x500;
				break;
		}

		++inc;

		switch (bul->direct)
		{
			case 0:
			case 2:
				if (inc % 2)
					bul->ym = 0x600;
				else
					bul->ym = -0x600;

				break;

			case 1:
			case 3:
				if (inc % 2)
					bul->xm = 0x600;
				else
					bul->xm = -0x600;

				break;
		}
	}
	else
	{
		switch (bul->direct)
		{
			case 0:
				bul->xm -= 0x140;
				break;
			case 1:
				bul->ym -= 0x140;
				break;
			case 2:
				bul->xm += 0x140;
				break;
			case 3:
				bul->ym += 0x140;
				break;
		}

		switch (bul->direct)
		{
			case 0:
			case 2:
				if (bul->count1 % 5 == 2)
				{
					if (bul->ym < 0)
						bul->ym = 0x600;
					else
						bul->ym = -0x600;
				}

				break;

			case 1u:
			case 3u:
				if (bul->count1 % 5 == 2)
				{
					if (bul->xm < 0)
						bul->xm = 0x600;
					else
						bul->xm = -0x600;
				}

				break;
		}
		if (bul->count1 % 4 == 0 && level == 4){
		int lol = Random(0, 2);
		for (int i = 0; i < lol; i++){
			BULLET* bully = SetBullet(56, bul->x, bul->y);
			if (bully != NULL){
				bully->direct = 0;
				bully->xm = Random(-1700, 1700);
				bully->ym = Random(-1000, 200);
				}
			}
		}

		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	if (++bul->ani_wait > 0)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 2)
		bul->ani_no = 0;

	Rect rect[3] = {
		{192, 16, 208, 32},
		{208, 16, 224, 32},
		{224, 16, 240, 32},
	};

	bul->rect = rect[bul->ani_no];

	if (level == 2)
		SetNpChar(129, bul->x, bul->y, 0, -0x200, bul->ani_no, NULL, 0x100);
	else
		SetNpChar(129, bul->x, bul->y, 0, -0x200, bul->ani_no + 3, NULL, 0x100);
}

void ActBullet_Rifle(BULLET *bul, int level)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		// Set speed
		switch (bul->direct)
			{
			case 0:
				bul->xm = -0xE80;
				break;
			case 1:
				bul->ym = -0xE80;
				break;
			case 2:
				bul->xm = 0xE80;
				break;
			case 3:
				bul->ym = 0xE80;
				break;
			}

		// Set hitbox
		switch (level)
		{
			case 1:
				switch (bul->direct)
				{
					case 0:
						bul->enemyYL = 0x400;
						break;
					case 1:
						bul->enemyXL = 0x400;
						break;
					case 2:
						bul->enemyYL = 0x400;
						break;
					case 3:
						bul->enemyXL = 0x400;
						break;
				}
				break;
			case 2:
				switch (bul->direct)
				{
					case 0:
						bul->enemyYL = 0x800;
						break;
					case 1:
						bul->enemyXL = 0x800;
						break;
					case 2:
						bul->enemyYL = 0x800;
						break;
					case 3:
						bul->enemyXL = 0x800;
						break;
				}
				break;
		}
	}
	else
	{
		// Move
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	Rect rect1[2] = {
		{128, 32, 144, 48},
		{144, 32, 160, 48},
	};

	Rect rect2[2] = {
		{160, 32, 176, 48},
		{176, 32, 192, 48},
	};

	Rect rect3[2] = {
		{128, 48, 144, 64},
		{144, 48, 160, 64},
	};

	//Set framerect
	switch (level)
	{
		case 1:
			if (bul->direct == 1 || bul->direct == 3)
				bul->rect = rect1[1];
			else
				bul->rect = rect1[0];

			break;

		case 2:
			if (bul->direct == 1 || bul->direct == 3)
				bul->rect = rect2[1];
			else
				bul->rect = rect2[0];

			break;

		case 3:
			if (bul->direct == 1 || bul->direct == 3)
				bul->rect = rect3[1];
			else
				bul->rect = rect3[0];

			break;
	}
}

void ActBullet_PoleStar(BULLET *bul, int level)
{
	if (++bul->count1 > bul->life_count && level != 3)
	{
		bul->cond = 0;
		if (level == 3)
			SetCaret(bul->x, bul->y, 3, 0);
		else
			SetCaret(bul->x, bul->y, 2, 2);
	}

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		// Set speed
		if (level == 3)
		{
			switch (bul->direct)
			{
			case 0:
				bul->xm = -0xE00;
				break;
			case 1:
				bul->ym = -0xE00;
				break;
			case 2:
				bul->xm = 0xE00;
				break;
			case 3:
				bul->ym = 0xE00;
				break;
			}
		}

		// Set hitbox
		switch (level)
		{
			case 1:
				switch (bul->direct)
				{
					case 0:
						bul->enemyYL = 0x400;
						break;
					case 1:
						bul->enemyXL = 0x400;
						break;
					case 2:
						bul->enemyYL = 0x400;
						break;
					case 3:
						bul->enemyXL = 0x400;
						break;
				}
				break;
			case 2:
				switch (bul->direct)
				{
					case 0:
						bul->enemyYL = 0x800;
						break;
					case 1:
						bul->enemyXL = 0x800;
						break;
					case 2:
						bul->enemyYL = 0x800;
						break;
					case 3:
						bul->enemyXL = 0x800;
						break;
				}
				break;
		}
	}
	else
	{
		// Move
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	Rect rect1[2] = {
		{128, 32, 144, 48},
		{144, 32, 160, 48},
	};

	Rect rect2[2] = {
		{160, 32, 176, 48},
		{176, 32, 192, 48},
	};

	Rect rect3[2] = {
		{128, 48, 144, 64},
		{144, 48, 160, 64},
	};

	//Set framerect
	switch (level)
	{
		case 1:
			if (bul->direct == 1 || bul->direct == 3)
				bul->rect = rect1[1];
			else
				bul->rect = rect1[0];

			break;

		case 2:
			if (bul->direct == 1 || bul->direct == 3)
				bul->rect = rect2[1];
			else
				bul->rect = rect2[0];

			break;

		case 3:
			if (bul->direct == 1 || bul->direct == 3)
				bul->rect = rect3[1];
			else
				bul->rect = rect3[0];

			break;
	}
}

void ActBullet_FireBall(BULLET *bul, int level)
{
	bool bBreak;
	if (GetNPCFlag(4405) || GetNPCFlag(7999)){
		bul->life_count = 60;
	}

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		if (GetNPCFlag(4405) || GetNPCFlag(7999)){
		int lol = Random(4, 8);
		for (int i = 0; i < lol; i++){
			BULLET* bully = SetBullet(56, bul->x, bul->y);
			if (bully != NULL){
				bully->direct = 0;
				bully->xm = Random(-1700, 1700);
				bully->ym = Random(-1000, 200);
				}
			}
		}
		return;
	}

	bBreak = false;
	if (bul->flag & 2 && bul->flag & 8)
		bBreak = true;
	if (bul->flag & 1 && bul->flag & 4)
		bBreak = true;

	if (bul->direct == 0 && bul->flag & 1){
		bul->direct = 2;
		if (level == 4){
			SetBullet(8, bul->x, bul->y, 0);
		}
	}
	if (bul->direct == 2 && bul->flag & 4){
		bul->direct = 0;
		if (level == 4){
			SetBullet(8, bul->x, bul->y, 0);
		}
	}

	if (bBreak)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 2, 0);
		PlaySoundObject(28, SOUND_MODE_PLAY);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x500;
				break;

			case 1:
				bul->xm = gMC.xm;

				if (gMC.xm < 0)
					bul->direct = 0;
				else
					bul->direct = 2;

				if (gMC.direct == 0)
					bul->xm -= 0x100;
				else
					bul->xm += 0x100;

				bul->ym = -0x5FF;
				break;

			case 2:
				bul->xm = 0x500;
				break;

			case 3:
				bul->xm = gMC.xm;

				if (gMC.xm < 0)
					bul->direct = 0;
				else
					bul->direct = 2;

				bul->ym = 0x5FF;

				break;
		}
		if (GetNPCFlag(4405) || GetNPCFlag(7999)){
			bul->xm *= 1.7;
			bul->ym *= 1.3;
		}
	}
	else
	{
		if (bul->count1 % 7 == 0 && (gMC.equip & EQUIP_PROPANE_TANK || GetNPCFlag(873) || GetNPCFlag(871))){
			int xCoord, yCoord;
			xCoord = bul->x - 0x200 * 5;
			yCoord = bul->y - 0x200 * 5;
			SetBullet(45, xCoord, yCoord, 0);
		}
		if (bul->flag & 8){
			bul->ym = -0x400;
			if (level == 4){
				SetBullet(8, bul->x, bul->y, 1);
			}
			if (GetNPCFlag(4405) || GetNPCFlag(7999)){
				bul->ym *= 1.5;
			}
		}
		else if (bul->flag & 1){
			bul->xm = 0x500;
			if (GetNPCFlag(4405) || GetNPCFlag(7999)){
				bul->xm *= 1.5;
			}
		}
		else if (bul->flag & 4){
			bul->xm = -0x500;
			if (GetNPCFlag(4405) || GetNPCFlag(7999)){
				bul->xm *= 1.5;
			}
		}

		bul->ym += 40;
		if (bul->ym > 0x3FF)
			bul->ym = 0x3FF;
		if (GetNPCFlag(4405) || GetNPCFlag(7999)){
			if (bul->ym > 0x100)
				bul->ym /= 2;
		}

		bul->x += bul->xm;
		bul->y += bul->ym;

		if (bul->flag & 0xD)
			PlaySoundObject(34, SOUND_MODE_PLAY);
	}

	Rect rect_left1[4] = {
		{128, 0, 144, 16},
		{144, 0, 160, 16},
		{160, 0, 176, 16},
		{176, 0, 192, 16},
	};

	Rect rect_right1[4] = {
		{128, 16, 144, 32},
		{144, 16, 160, 32},
		{160, 16, 176, 32},
		{176, 16, 192, 32},
	};

	Rect rect_left2[3] = {
		{192, 16, 208, 32},
		{208, 16, 224, 32},
		{224, 16, 240, 32},
	};

	Rect rect_right2[3] = {
		{224, 16, 240, 32},
		{208, 16, 224, 32},
		{192, 16, 208, 32},
	};

	++bul->ani_no;

	if (level == 1)
	{
		if (bul->ani_no > 3)
			bul->ani_no = 0;

		if (bul->direct == 0)
			bul->rect = rect_left1[bul->ani_no];
		else
			bul->rect = rect_right1[bul->ani_no];
	}
	else
	{
		if (bul->ani_no > 2)
			bul->ani_no = 0;

		if (bul->direct == 0)
			bul->rect = rect_left2[bul->ani_no];
		else
			bul->rect = rect_right2[bul->ani_no];

		if (level == 2)
			SetNpChar(129, bul->x, bul->y, 0, -0x200, bul->ani_no, NULL, 0x100);
		else
			SetNpChar(129, bul->x, bul->y, 0, -0x200, bul->ani_no + 3, NULL, 0x100);
	}
}

void ActBullet_FireMine(BULLET *bul)
{
	bool bBreak;

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		SetBullet(18, bul->x, bul->y, 0);
		return;
	}

	bBreak = false;
	if (bul->flag & 2 && bul->flag & 8)
		bBreak = true;
	if (bul->flag & 1 && bul->flag & 4)
		bBreak = true;

	if (bul->direct == 0 && bul->flag & 1){
		bul->direct = 2;
	}
	if (bul->direct == 2 && bul->flag & 4){
		bul->direct = 0;
	}

	if (bBreak)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 2, 0);
		SetBullet(18, bul->x, bul->y, 0);
		PlaySoundObject(28, SOUND_MODE_PLAY);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x350;
				break;

			case 1:
				bul->xm = gMC.xm;

				if (gMC.xm < 0)
					bul->direct = 0;
				else
					bul->direct = 2;

				if (gMC.direct == 0)
					bul->xm -= 0xA0;
				else
					bul->xm += 0xA0;

				bul->ym = -0x5FF;
				break;

			case 2:
				bul->xm = 0x350;
				break;

			case 3:
				bul->xm = gMC.xm;

				if (gMC.xm < 0)
					bul->direct = 0;
				else
					bul->direct = 2;

				bul->ym = 0x5FF;

				break;
		}
	}
	else
	{
		if (bul->flag & 8){
			bul->ym = -0x400;
		}
		else if (bul->flag & 1){
			bul->xm = 0x350;
		}
		else if (bul->flag & 4){
			bul->xm = -0x350;
		}

		bul->ym += 40;
		if (bul->ym > 0x3FF)
			bul->ym = 0x3FF;

		bul->x += bul->xm;
		bul->y += bul->ym;

		if (bul->flag & 0xD)
			PlaySoundObject(34, SOUND_MODE_PLAY);
	}

	Rect rect_left1[4] = {
		{128, 0, 144, 16},
		{144, 0, 160, 16},
		{160, 0, 176, 16},
		{176, 0, 192, 16},
	};

	Rect rect_right1[4] = {
		{128, 16, 144, 32},
		{144, 16, 160, 32},
		{160, 16, 176, 32},
		{176, 16, 192, 32},
	};

	++bul->ani_no;
	if (bul->ani_no > 5)
		bul->ani_no = 0;

		if (bul->direct == 0)
			bul->rect = rect_left1[(bul->ani_no+1)/2];
		else
			bul->rect = rect_right1[(bul->ani_no+1)/2];
}

void ActBullet_FrontieBust1(BULLET *bul, int level)
{
	int baseSpeed = 0x1000;
	int inaccuracy = 0x200;
	if (level == 3){
		baseSpeed *= 0.85;
		inaccuracy *= 3.5f;
		bul->iframes = 3;
	}

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -baseSpeed;
				bul->ym = Random(-inaccuracy, inaccuracy);
				break;
			case 1:
				bul->ym = -baseSpeed;
				bul->xm = Random(-inaccuracy, inaccuracy);
				break;
			case 2:
				bul->xm = baseSpeed;
				bul->ym = Random(-inaccuracy, inaccuracy);
				break;
			case 3:
				bul->ym = baseSpeed;
				bul->xm = Random(-inaccuracy, inaccuracy);
				break;
		}
	}
	else
	{
		bul->x += bul->xm;
		bul->y += bul->ym;

		if (bul->flag & 0xD)
			PlaySoundObject(34, SOUND_MODE_PLAY);
	}

	Rect rect_left1[4] = {
		{128, 176, 144, 192},
		{144, 176, 160, 192},
		{160, 176, 176, 192},
		{176, 176, 192, 192},
	};

	Rect rect_right1[4] = {
		{128, 192, 144, 208},
		{144, 192, 160, 208},
		{160, 192, 176, 208},
		{176, 192, 192, 208},
	};

	++bul->ani_no;
	if (bul->ani_no > 3)
		bul->ani_no = 0;

	if (bul->direct == 0)
		bul->rect = rect_left1[bul->ani_no];
	else
		bul->rect = rect_right1[bul->ani_no];

	SetNpChar(129, bul->x, bul->y, 0, -0x200, std::max(0, bul->ani_no-1) + 3, NULL, 0x100);
}

void ActBullet_FrontieBust2(BULLET *bul)
{
	Rect rect[3] = {
		{192, 16, 208, 32},
		{208, 16, 224, 32},
		{224, 16, 240, 32}
	};

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}
	bul->iframes = 30;

	if (bul->act_no == 0)
	{
		bul->ani_no = 0;
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x380;
				break;
			case 1:
				bul->ym = -0x380;
				break;
			case 2:
				bul->xm = 0x380;
				break;
			case 3:
				bul->ym = 0x380;
				break;
		}
	}
	else
	{
		switch (bul->direct)
		{
			case 0:
				bul->xm += -0x60;
				if (bul->xm < -0x1400){
					bul->xm = -0x1400;
					SetCaret(bul->x + Random(-bul->view.front, bul->view.back),
						bul->y + Random(-bul->view.top, bul->view.bottom), 13, 0);
				}
				break;
			case 1:
				bul->ym += -0x60;
				if (bul->ym < -0x1400){
					bul->ym = -0x1400;
					SetCaret(bul->x + Random(-bul->view.front, bul->view.back),
						bul->y + Random(-bul->view.top, bul->view.bottom), 13, 0);
				}
				break;
			case 2:
				bul->xm += 0x60;
				if (bul->xm > 0x1400){
					bul->xm = 0x1400;
					SetCaret(bul->x + Random(-bul->view.front, bul->view.back),
						bul->y + Random(-bul->view.top, bul->view.bottom), 13, 0);
				}
				break;
			case 3:
				bul->ym += 0x60;
				if (bul->ym > 0x1400){
					bul->ym = 0x1400;
					SetCaret(bul->x + Random(-bul->view.front, bul->view.back),
						bul->y + Random(-bul->view.top, bul->view.bottom), 13, 0);
				}
				break;
		}

		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	if (++bul->ani_wait > 0)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 2)
		bul->ani_no = 0;

	bul->rect = rect[bul->ani_no];
}

void ActBullet_MachineGun(BULLET *bul, int level)
{
	int move;
	int secondMove;

	Rect rect1[4] = {
		{64, 0, 80, 16},
		{80, 0, 96, 16},
		{96, 0, 112, 16},
		{112, 0, 128, 16},
	};

	Rect rect2[4] = {
		{64, 16, 80, 32},
		{80, 16, 96, 32},
		{96, 16, 112, 32},
		{112, 16, 128, 32},
	};

	Rect rect3[4] = {
		{64, 32, 80, 48},
		{80, 32, 96, 48},
		{96, 32, 112, 48},
		{112, 32, 128, 48},
	};

	bul->local_iframes = 10;

	if (GetNPCFlag(4200) || GetNPCFlag(7999)){
		switch (level){
			case 1: bul->damage = Random(3, 4); break;
			case 2: bul->damage = Random(5, 6); break;
			case 3: bul->damage = Random(7, 8); break;
			case 4: bul->damage = Random(9, 10); break;
		}
	}

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->act_no == 0)
	{
		switch (level)
		{
			case 1:
				move = 0x2000;
				break;
			case 2:
				move = 0x2000;
				break;
			case 3:
				move = 0x2000;
				break;
			case 4:
				move = 0x2400;
				break;
		}
		if (gMC.equip & EQUIP_OVERCHARGE){
			move *= 1.2f;
		}
		secondMove = 0xFF;
		if (gMC.equip & EQUIP_OVERCHARGE)
			secondMove = 390;
		if (gArmsData[gSelectedArms].code == 21)
			secondMove *= 4;

		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -move;
				bul->ym = Random(-secondMove, secondMove);
				bul->enemyXL = 4_pix;
				bul->enemyYL = 2_pix;
				break;
			case 1:
				bul->ym = -move;
				bul->xm = Random(-secondMove, secondMove);
				bul->enemyYL = 4_pix;
				bul->enemyXL = 2_pix;
				break;
			case 2:
				bul->xm = move;
				bul->ym = Random(-secondMove, secondMove);
				bul->enemyXL = 4_pix;
				bul->enemyYL = 2_pix;
				break;
			case 3:
				bul->ym = move;
				bul->xm = Random(-secondMove, secondMove);
				bul->enemyYL = 4_pix;
				bul->enemyXL = 2_pix;
				break;
		}
	}
	else
	{
		bul->x += bul->xm;
		bul->y += bul->ym;

		switch (level)
		{
			case 1:
				bul->rect = rect1[bul->direct];
				break;

			case 2:
				bul->rect = rect2[bul->direct];

				if (bul->direct == 1 || bul->direct == 3)
					SetNpChar(127, bul->x, bul->y, 0, 0, 1, NULL, 0x100);
				else
					SetNpChar(127, bul->x, bul->y, 0, 0, 0, NULL, 0x100);

				break;

			case 3: case 4:
				bul->rect = rect3[bul->direct];
				SetNpChar(128, bul->x, bul->y, 0, 0, bul->direct, NULL, 0x100);
				break;
		}
	}
}

void ActBullet_Missile(BULLET *bul, int level)
{
	bool bHit;

	static unsigned int inc;

	bHit = false;

	// if (bul->life != 10)
	// 	bHit = true;
	if (bul->flag & 0xF)
		bHit = true;
	if (bHit)
	{
		SetBullet(level + 15, bul->x, bul->y, 0);
		bul->cond = 0;
		gMC.direct = 2;
	}

	bul->life = 100;
	if (++bul->count1 % 3 == 0){
		bul->damage = 1;
	} else {
		bul->damage = 0;
	}

	switch (bul->act_no)
	{
	case 0:
		bul->act_no = 1;

		switch (bul->direct)
		{
		case 0:
		case 2:
			bul->tgt_y = bul->y;
			break;
		case 1:
		case 3:
			bul->tgt_x = bul->x;
			break;
		}
		bul->ani_no = 0x120;
		// Fallthrough
	case 1:

		int max_move;

		if (gMC.flag & 0x100 && !(gMC.flag & (0x8000 | 0x4000 | 0x2000 | 0x1000)))
			max_move = gMC.physics_underwater.max_move * 2.1; // Underwater or in wind
		else
			max_move = gMC.physics_normal.max_move * 2.1; // Normal conditions

		if (gMC.cond == 0){
			SetBullet(level + 15, bul->x, bul->y, 0);
		bul->cond = 0;
		gMC.direct = 2;
		}

		if ((GetNPCFlag(4350) || GetNPCFlag(7999)) && gKey & gKeyJump){
			if (gKey & gKeyUp){
				bul->xm = 0;
				if (bul->ym > 0){
					bul->ym = 0;
				}
				bul->ym -= bul->ani_no * 0.8;
				bul->direct = 1;
			} else if (gKey & gKeyDown){
				bul->xm = 0;
				if (bul->ym < 0){
					bul->ym = 0;
				}
				bul->ym += bul->ani_no * 0.8;
				bul->direct = 3;
			} else if (gKey & gKeyLeft){
				if (bul->xm > 0){
					bul->xm = 0;
				}
				bul->xm -= bul->ani_no * 0.8;
				bul->ym = 0;
				bul->direct = 0;
			} else if (gKey & gKeyRight){
				if (bul->xm < 0){
					bul->xm = 0;
				}
				bul->xm += bul->ani_no * 0.8;
				bul->ym = 0;
				bul->direct = 2;
			} else {
				bul->xm = 0;
				bul->ym = 0;
			}
		} else {
		switch (bul->direct)
		{
		case 0:
			bul->xm += -bul->ani_no;
			break;
		case 1:
			bul->ym += -bul->ani_no;
			break;
		case 2:
			bul->xm += bul->ani_no;
			break;
		case 3:
			bul->ym += bul->ani_no;
			break;
		}
		}

		if ((GetNPCFlag(4350) || GetNPCFlag(7999)) && gKey & gKeyJump){
			
		} else {
			gMC.shock = 2;
		}

		if (bul->xm < -max_move)
			bul->xm = -max_move;
		if (bul->ym < -max_move)
			bul->ym = -max_move;

		if (bul->xm > max_move)
			bul->xm = max_move;
		if (bul->ym > max_move)
			bul->ym = max_move;

		bul->x += bul->xm;
		bul->y += bul->ym;

		gMC.unit = 2;
		if ((GetNPCFlag(4350) || GetNPCFlag(7999)) && gKey & gKeyJump){
			gMC.x = bul->x;
			gMC.y = bul->y;
		} else {
			gMC.x += bul->xm;
			gMC.y += bul->ym;
		}
		gMC.direct = bul->direct;

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
		if (gKey & gKeyUp)
		{
			gMC.index_y -= 0x200;
			if (gMC.index_y < -0x8000)
				gMC.index_y = -0x8000;
		}
		else if (gKey & gKeyDown)
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
		SetFrameTargetMyChar(8);

		break;
	}

	switch (bul->direct)
	{
	case 0:
		SetCaret(bul->x + (8 * 0x200), bul->y, 7, 2);
		break;
	case 1:
		SetCaret(bul->x, bul->y + (8 * 0x200), 7, 3);
		break;
	case 2:
		SetCaret(bul->x - (8 * 0x200), bul->y, 7, 0);
		break;
	case 3:
		SetCaret(bul->x, bul->y - (8 * 0x200), 7, 1);
		break;
	}

	if (gKeyTrg & gKeyAbility && gMC.equip & (EQUIP_THROTTLER))
	{
		bul->cond = 0;
		SetBullet(level + 15, bul->x, bul->y, 0);
		return;
	}

	Rect rect1[4] = {
		{0, 0, 16, 16},
		{16, 0, 32, 16},
		{32, 0, 48, 16},
		{48, 0, 64, 16},
	};

	Rect rect2[4] = {
		{0, 16, 16, 32},
		{16, 16, 32, 32},
		{32, 16, 48, 32},
		{48, 16, 64, 32},
	};

	Rect rect3[4] = {
		{0, 32, 16, 48},
		{16, 32, 32, 48},
		{32, 32, 48, 48},
		{48, 32, 64, 48},
	};

	switch (level)
	{
	case 1:
		bul->rect = rect1[bul->direct];
		break;
	case 2:
		bul->rect = rect2[bul->direct];
		break;
	case 3:
		bul->rect = rect3[bul->direct];
		break;
	}
}

void ActBullet_Bom(BULLET *bul, int level)
{
	if (bul->code_bullet == 17){
	gMC.unit = 0;
	gMC.shock = 30;
	}

	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;

			switch (level)
			{
				case 1:
					bul->act_wait = 7;
					break;
				case 2:
					bul->act_wait = 20;
					break;
				case 3:
					bul->act_wait = 10;
					break;
			}

			PlaySoundObject(44, SOUND_MODE_PLAY);
			// Fallthrough
		case 1:
			switch (level)
			{
				case 1:
					if (bul->act_wait % 3 == 0)
						SetDestroyNpCharUp(bul->x + (Random(-24, 24) * 0x200), bul->y + (Random(-24, 24) * 0x200), bul->enemyXL, 2);
					break;

				case 2:
					if (bul->act_wait % 5 == 0)
						SetDestroyNpCharUp(bul->x + (Random(-32, 32) * 0x200), bul->y + (Random(-32, 32) * 0x200), bul->enemyXL, 2);
					break;

				case 3:
					if (bul->act_wait % 3 == 0)
						SetDestroyNpCharUp(bul->x + (Random(-40, 40) * 0x200), bul->y + (Random(-40, 40) * 0x200), bul->enemyXL, 2);
					break;
			}

			if (--bul->act_wait < 0)
				bul->cond = 0;

			break;
	}
}

void ActBullet_Bubblin1(BULLET *bul)
{
	if (bul->flag & 0x2FF)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 2, 0);
		return;
	}

	switch (bul->act_no)
	{
		case 0:
			bul->npc = NULL;
			bul->act_no = 1;

			switch (bul->direct)
			{
				case 0:
					bul->xm = -0x500;
					break;
				case 2:
					bul->xm = 0x500;
					break;
				case 1:
					bul->ym = -0x500;
					break;
				case 3:
					bul->ym = 0x500;
					break;
			}

			break;
	}
	
	if (bul->act_no == 1){
		switch (bul->direct)
		{
			case 0:
				bul->xm += 0x2A;
				if (bul->xm >= 0) {
					bul->xm = 0;
					bul->act_no = 2;
				}
				break;
			case 2:
				bul->xm -= 0x2A;
				if (bul->xm <= 0) {
					bul->xm = 0;
					bul->act_no = 2;
				}
				break;
			case 1:
				bul->ym += 0x2A;
				if (bul->ym >= 0) {
					bul->ym = 0;
					bul->act_no = 2;
				}
				break;
			case 3:
				bul->ym -= 0x2A;
				if (bul->ym <= 0) {
					bul->ym = 0;
					bul->act_no = 2;
				}
				break;
		}
	} else if (bul->act_no == 2) {
		if (bul->npc == NULL || !(bul->npc->cond & 0x80)){
			bul->npc = GetClosest(bul->x, bul->y, 48_pix);
		} else {
			SetCaret(bul->x, bul->y, 1, 0);
			bul->tgt_x = bul->npc->x;
			bul->tgt_y = bul->npc->y;

			int deg = GetArktan(bul->x - bul->tgt_x, bul->y - bul->tgt_y);
			deg += Random(-32, 32);
			bul->ym = 4 * GetSin(deg);
			bul->xm = 4 * GetCos(deg);
		}
	}

	bul->x += bul->xm;
	bul->y += bul->ym;

	if (++bul->act_wait > 250)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 15, 0);
	}

	Rect rect[4] = {
		{192, 0, 200, 8},
		{200, 0, 208, 8},
		{208, 0, 216, 8},
		{216, 0, 224, 8},
	};

	if (++bul->ani_wait > 3)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 3;

	bul->rect = rect[bul->ani_no];
}

void ActBullet_Bubblin2(BULLET *bul)
{
	bool bDelete = false;

	if (bul->direct == 0 && bul->flag & 1)
		bDelete = true;
	if (bul->direct == 2 && bul->flag & 4)
		bDelete = true;
	if (bul->direct == 1 && bul->flag & 2)
		bDelete = true;
	if (bul->direct == 3 && bul->flag & 8)
		bDelete = true;

	if (bDelete)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 2, 0);
		return;
	}

	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;

			switch (bul->direct)
			{
				case 0:
					bul->xm = -0x900;
					bul->ym = Random(-0x180, 0x180);
					if (gMC.equip & EQUIP_OVERCHARGE){
						bul->xm = -0xAF8;
						bul->ym = Random(-0x27C, 0x27C);
					}
					break;
				case 2:
					bul->xm = 0x900;
					bul->ym = Random(-0x180, 0x180);
					if (gMC.equip & EQUIP_OVERCHARGE){
						bul->xm = 0xAF8;
						bul->ym = Random(-0x27C, 0x27C);
					}
					break;
				case 1:
					bul->ym = -0x900;
					bul->xm = Random(-0x180, 0x180);
					if (gMC.equip & EQUIP_OVERCHARGE){
						bul->ym = -0xAF8;
						bul->xm = Random(-0x27C, 0x27C);
					}
					break;
				case 3:
					bul->ym = 0x900;
					bul->xm = Random(-0x180, 0x180);
					if (gMC.equip & EQUIP_OVERCHARGE){
						bul->ym = 0xAF8;
						bul->xm = Random(-0x27C, 0x27C);
					}
					break;
			}

			break;
	}

	switch (bul->direct)
	{
		case 0:
			bul->xm += 0x10;
			break;
		case 2:
			bul->xm -= 0x10;
			break;
		case 1:
			bul->ym += 0x10;
			break;
		case 3:
			bul->ym -= 0x10;
			break;
	}

	bul->x += bul->xm;
	bul->y += bul->ym;

	if (++bul->act_wait > 50)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 15, 0);
	}

	Rect rect[4] = {
		{192, 8, 200, 16},
		{200, 8, 208, 16},
		{208, 8, 216, 16},
		{216, 8, 224, 16},
	};

	if (++bul->ani_wait > 3)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 3;

	bul->rect = rect[bul->ani_no];
}

void ActBullet_Bubblin3(BULLET *bul)
{
	bul->iframes = 2'147'483'647;
	if (!(gKey & gKeyShot))
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 2, 0);
		PlaySoundObject(100, SOUND_MODE_PLAY);

		if (gMC.up)
			SetBullet(22, bul->x, bul->y, 1);
		else if (gMC.down)
			SetBullet(22, bul->x, bul->y, 3);
		else
			SetBullet(22, bul->x, bul->y, gMC.direct);

		return;
	}

	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;

			switch (bul->direct)
			{
				case 0:
					bul->xm = Random(-0x400, -0x200);
					bul->ym = (Random(-4, 4) * 0x200) / 2;
					break;
				case 2:
					bul->xm = Random(0x200, 0x400);
					bul->ym = (Random(-4, 4) * 0x200) / 2;
					break;
				case 1:
					bul->ym = Random(-0x400, -0x200);
					bul->xm = (Random(-4, 4) * 0x200) / 2;
					break;
				case 3:
					bul->ym = Random(0x80, 0x100);
					bul->xm = (Random(-4, 4) * 0x200) / 2;
					break;
			}

			break;
	}

	if (bul->x < gMC.x)
		bul->xm += 0x20;
	if (bul->x > gMC.x)
		bul->xm -= 0x20;

	if (bul->y < gMC.y)
		bul->ym += 0x20;
	if (bul->y > gMC.y)
		bul->ym -= 0x20;

	if (bul->xm < 0 && bul->flag & 1)
		bul->xm = 0x400;
	if (bul->xm > 0 && bul->flag & 4)
		bul->xm = -0x400;

	if (bul->ym < 0 && bul->flag & 2)
		bul->ym = 0x400;
	if (bul->ym > 0 && bul->flag & 8)
		bul->ym = -0x400;

	bul->x += bul->xm;
	bul->y += bul->ym;

	Rect rect[4] = {
		{240, 16, 248, 24},
		{248, 16, 256, 24},
		{240, 24, 248, 32},
		{248, 24, 256, 32},
	};

	if (++bul->ani_wait > 3)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 3;

	bul->rect = rect[bul->ani_no];
}

void ActBullet_Bubblin4(BULLET *bul)
{
	bul->iframes = 2'147'483'647;
	if (!(gKey & gKeyShot))
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 2, 0);
		PlaySoundObject(100, SOUND_MODE_PLAY);

		if (gMC.up)
			SetBullet(22, bul->x, bul->y, 1);
		else if (gMC.down)
			SetBullet(22, bul->x, bul->y, 3);
		else
			SetBullet(22, bul->x, bul->y, gMC.direct);

		return;
	}

	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;

			switch (bul->direct)
			{
				case 0:
					bul->xm = Random(-0x600, -0x400);
					bul->ym = (Random(-6, 6) * 0x200) / 2;
					break;
				case 2:
					bul->xm = Random(0x400, 0x600);
					bul->ym = (Random(-6, 6) * 0x200) / 2;
					break;
				case 1:
					bul->ym = Random(-0x600, -0x400);
					bul->xm = (Random(-6, 6) * 0x200) / 2;
					break;
				case 3:
					bul->ym = Random(0x100, 0x160);
					bul->xm = (Random(-6, 6) * 0x200) / 2;
					break;
			}

			break;
	}

	if (bul->x < gMC.x)
		bul->xm += 0x10;
	if (bul->x > gMC.x)
		bul->xm -= 0x10;

	if (bul->y < gMC.y)
		bul->ym += 0x10;
	if (bul->y > gMC.y)
		bul->ym -= 0x10;

	if (bul->xm < 0 && bul->flag & 1)
		bul->xm = 0x600;
	if (bul->xm > 0 && bul->flag & 4)
		bul->xm = -0x600;

	if (bul->ym < 0 && bul->flag & 2)
		bul->ym = 0x600;
	if (bul->ym > 0 && bul->flag & 8)
		bul->ym = -0x600;

	bul->x += bul->xm;
	bul->y += bul->ym;

	Rect rect[4] = {
		{240, 16, 248, 24},
		{248, 16, 256, 24},
		{240, 24, 248, 32},
		{248, 24, 256, 32},
	};

	if (++bul->ani_wait > 3)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 3;

	bul->rect = rect[bul->ani_no];
}

void ActBullet_Spine(BULLET *bul)
{
	if (++bul->count1 > bul->life_count || bul->flag & 8)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = (-Random(10, 16) * 0x200) / 2;
				break;
			case 1:
				bul->ym = (-Random(10, 16) * 0x200) / 2;
				break;
			case 2:
				bul->xm = (Random(10, 16) * 0x200) / 2;
				break;
			case 3:
				bul->ym = (Random(10, 16) * 0x200) / 2;
				break;
		}
	}
	else
	{
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	if (++bul->ani_wait > 1)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 1)
		bul->ani_no = 0;

	Rect rcLeft[2] = {
		{224, 0, 232, 8},
		{232, 0, 240, 8},
	};

	Rect rcRight[2] = {
		{224, 0, 232, 8},
		{232, 0, 240, 8},
	};

	Rect rcDown[2] = {
		{224, 8, 232, 16},
		{232, 8, 240, 16},
	};

	switch (bul->direct)
	{
		case 0:
			bul->rect = rcLeft[bul->ani_no];
			break;
		case 1:
			bul->rect = rcDown[bul->ani_no];
			break;
		case 2:
			bul->rect = rcRight[bul->ani_no];
			break;
		case 3:
			bul->rect = rcDown[bul->ani_no];
			break;
	}
}

void ActBullet_BoomerangSword1(BULLET *bul)
{
	bul->local_iframes = 20;

	if (++bul->count1 > bul->life_count && bul->act_no == 1)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}
	else if (bul->life < 100 && bul->act_no == 1)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}
	else if (bul->flag & 0xF && bul->act_no == 1)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}

	if (bul->act_no == 1){
		if (bul->code_bullet == 56)
			bul->damage = 6;
		else
			bul->damage = 4;
	}

	if (bul->count1 == 3)
		bul->bbits &= ~4;

	if (bul->count1 % 25 == 1)
		PlaySoundObject(34, SOUND_MODE_PLAY);

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x700;
				break;
			case 1:
				bul->ym = -0x700;
				break;
			case 2:
				bul->xm = 0x700;
				break;
			case 3:
				bul->ym = 0x700;
				break;
			case 4:
				bul->xm = -0x700;
				bul->ym = -0x700;
				break;
			case 5:
				bul->ym = -0x700;
				bul->xm = 0x700;
				break;
			case 6:
				bul->xm = 0x700;
				bul->ym = 0x700;
				break;
			case 7:
				bul->ym = 0x700;
				bul->xm = -0x700;
				break;
		}
	}
	else
	{
		if (bul->act_no == 2){
			if (bul->x > gMC.x - 0x200*8 && bul->x < gMC.x + 0x200*8 &&
					 bul->y > gMC.y - 0x200*8 && bul->y < gMC.y + 0x200*8){
				bul->cond = 0;
				return;
			}
			int deg = GetArktan(bul->x - gMC.x, bul->y - gMC.y);
			bul->ym = GetSin(deg) * 2.9;
			bul->xm = GetCos(deg) * 2.9;
			if (bul->code_bullet == 55){
				bul->ym = GetSin(deg) * 6.96;
				bul->xm = GetCos(deg) * 6.96;
			}
		}
			bul->x += bul->xm;
			bul->y += bul->ym;
	}

	Rect rcLeft[4] = {
		{0, 48, 16, 64},
		{16, 48, 32, 64},
		{32, 48, 48, 64},
		{48, 48, 64, 64},
	};

	Rect rcRight[4] = {
		{64, 48, 80, 64},
		{80, 48, 96, 64},
		{96, 48, 112, 64},
		{112, 48, 128, 64},
	};

	if (++bul->ani_wait > 1)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 0;

	if (bul->direct == 0 || bul->direct == 4)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void ActBullet_BoomerangSword2(BULLET *bul)
{
	bul->iframes = 20;

	++bul->count1;
	if (bul->count1 > bul->life_count && bul->act_no < 2)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}
	if (bul->life < 100 && bul->act_no < 2)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}
	else if (bul->flag & 0xF && bul->act_no == 1)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}

	bul->damage = 7 + 8 * std::min(1.0f, float(bul->count1)/float(bul->life_count));

	if (bul->count1 == 3)
		bul->bbits &= ~4;

	if (bul->count1 % 7 == 1)
		PlaySoundObject(106, SOUND_MODE_PLAY);

	if (bul->act_no == 0)
	{
		bul->act_no = 1;
		bul->count2 = 0;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x1240;
				break;
			case 1:
				bul->ym = -0x1240;
				break;
			case 2:
				bul->xm = 0x1240;
				break;
			case 3:
				bul->ym = 0x1240;
				break;
		}
	}
	else
	{
		if (bul->act_no == 2){
			if (bul->x > gMC.x - 0x200*8 && bul->x < gMC.x + 0x200*8 &&
					 bul->y > gMC.y - 0x200*8 && bul->y < gMC.y + 0x200*8){
				bul->cond = 0;
				return;
			}
			if (bul->count2 == 1 && bul->count1 % 3 == 0){
				BULLET* bully = SetBullet(56, bul->x, bul->y);
				if (bully != NULL){
					bully->direct = 0;
					bully->xm = Random(-1700, 1700);
					bully->ym = Random(-1000, 200);
				}
			}
			if (bul->count2 == 0 && bul->life == 100 && !(bul->flag & 0xF)){
				bul->count2 = 1;
				int lol = Random(5, 7);
				for (int i = 0; i < lol; i++){
					BULLET* bully = SetBullet(56, bul->x, bul->y);
					if (bully != NULL){
					bully->direct = 0;
					bully->xm = Random(-1700, 1700);
					bully->ym = Random(-1000, 200);
					}
				}
			}
			
			int deg = GetArktan(bul->x - gMC.x, bul->y - gMC.y);
			bul->ym = GetSin(deg) * 12;
			bul->xm = GetCos(deg) * 12;
		}
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	Rect rcLeft[4] = {
		{160, 48, 184, 72},
		{184, 48, 208, 72},
		{208, 48, 232, 72},
		{232, 48, 256, 72},
	};

	Rect rcRight[4] = {
		{160, 72, 184, 96},
		{184, 72, 208, 96},
		{208, 72, 232, 96},
		{232, 72, 256, 96},
	};

	if (++bul->ani_wait > 1)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 0;

	if (bul->direct == 0)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void ActBullet_BoomerangSword3(BULLET *bul)
{
	static unsigned int inc;

	bul->local_iframes = 20;

	++bul->count1;
	if (bul->count1 > bul->life_count && bul->act_no < 2)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}
	if (bul->life < 100 && bul->act_no < 2)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}

	bul->damage = 7;

	if (bul->count1 == 3)
		bul->bbits &= ~4;
	if (bul->count1 % 7 == 1)
		PlaySoundObject(106, SOUND_MODE_PLAY);
	if (bul->count1 % 10 > 6 && bul->count1 % 10 < 9){
			if (bul->count1 % 10 == 7){
				SetBullet(23, bul->x, bul->y, 0);
			}
			else{
				SetBullet(23, bul->x, bul->y, 2);
			}
	}

	if (bul->act_no == 0)
	{
		bul->act_no = 1;
		inc++;

		if (inc % 2){
			bul->count2 = 1;
		} else {
			bul->count2 = 0;
		}

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x999;
				break;
			case 1:
				bul->ym = -0x999;
				break;
			case 2:
				bul->xm = 0x999;
				break;
			case 3:
				bul->ym = 0x999;
				break;
		}
	}
	else
	{
		if (bul->act_no == 2){
			SetBullet(bul->code_bullet + 1, bul->x, bul->y, bul->count2 == 1 ? 4 : 0);
			SetBullet(bul->code_bullet + 1, bul->x, bul->y, bul->count2 == 1 ? 5 : 1);
			SetBullet(bul->code_bullet + 1, bul->x, bul->y, bul->count2 == 1 ? 6 : 2);
			SetBullet(bul->code_bullet + 1, bul->x, bul->y, bul->count2 == 1 ? 7 : 3);
			bul->cond = 0;
			return;
			// if (bul->x > gMC.x - 0x200*8 && bul->x < gMC.x + 0x200*8 &&
			// 		 bul->y > gMC.y - 0x200*8 && bul->y < gMC.y + 0x200*8){
			// 	bul->cond = 0;
			// 	return;
			// }
			// int deg = GetArktan(bul->x - gMC.x, bul->y - gMC.y);
			// bul->ym = GetSin(deg) * 8;
			// bul->xm = GetCos(deg) * 8;
		}
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	Rect rcLeft[4] = {
		{160, 48, 184, 72},
		{184, 48, 208, 72},
		{208, 48, 232, 72},
		{232, 48, 256, 72},
	};

	Rect rcRight[4] = {
		{160, 72, 184, 96},
		{184, 72, 208, 96},
		{208, 72, 232, 96},
		{232, 72, 256, 96},
	};

	if (++bul->ani_wait > 1)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 0;

	if (bul->direct == 0)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void ActBullet_BoomerangSword4(BULLET *bul)
{
	bul->iframes = 20;

	++bul->count1;
	if (bul->count1 > bul->life_count && (bul->act_no == 3 || bul->act_no == 1))
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}

	if (bul->life < 100)
	{
		bul->act_no = 3;
		bul->life = 100;
		bul->count1 = 0;
	}
	else if (bul->flag & 0xF)
	{
		bul->act_no = 3;
		bul->count1 = 0;
	}

	bul->damage = 8;

	if (bul->count1 == 3)
		bul->bbits &= ~4;

	if (bul->count1 % 7 == 1)
		PlaySoundObject(106, SOUND_MODE_PLAY);

	if (bul->act_no == 0)
	{
		bul->act_no = 1;
		bul->count2 = 0;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x1000;
				break;
			case 1:
				bul->ym = -0x1000;
				break;
			case 2:
				bul->xm = 0x1000;
				break;
			case 3:
				bul->ym = 0x1000;
				break;
		}
	}
	else
	{
		if (bul->act_no == 3){
			bul->act_no = 1;
			int deg = GetArktan(bul->x - (bul->x + Random(-0x200 * 4, 0x200 * 4)), 
			bul->y - (bul->y + Random(-0x200 * 4, 0x200 * 4)));
			deg += (unsigned char)Random(-6, 6);
			bul->ym = GetSin(deg) * 10;
			bul->xm = GetCos(deg) * 10;
		}
		if (bul->act_no == 2){
			if ((bul->x > gMC.x - 0x200*8 && bul->x < gMC.x + 0x200*8 &&
					 bul->y > gMC.y - 0x200*8 && bul->y < gMC.y + 0x200*8)){
				bul->cond = 0;
				return;
			}
			int deg = GetArktan(bul->x - gMC.x, bul->y - gMC.y);
			bul->ym = GetSin(deg) * 10;
			bul->xm = GetCos(deg) * 10;
		}
		if (bul->count1 % 4 == 0){
			if ((bul->count1 / 2) % 2 == 0)
				SetBullet(23, bul->x, bul->y, 0);
			else
				SetBullet(23, bul->x, bul->y, 2);
		}
			
		bul->x += bul->xm;
		bul->y += bul->ym;
		if (Distance(bul->x, bul->y, gMC.x, gMC.y) > 12 * 0x2000){
			bul->bbits |= 4;
			bul->act_no = 2;
		}
	}

	Rect rcLeft[4] = {
		{160, 48, 184, 72},
		{184, 48, 208, 72},
		{208, 48, 232, 72},
		{232, 48, 256, 72},
	};

	Rect rcRight[4] = {
		{160, 72, 184, 96},
		{184, 72, 208, 96},
		{208, 72, 232, 96},
		{232, 72, 256, 96},
	};

	if (++bul->ani_wait > 1)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 0;

	if (bul->direct == 0)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void ActBullet_Sword1(BULLET *bul)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->count1 == 3)
		bul->bbits &= ~4;

	if (bul->count1 % 5 == 1)
		PlaySoundObject(34, SOUND_MODE_PLAY);

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x800;
				break;
			case 1:
				bul->ym = -0x800;
				break;
			case 2:
				bul->xm = 0x800;
				break;
			case 3:
				bul->ym = 0x800;
				break;
		}
	}
	else
	{
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	Rect rcLeft[4] = {
		{0, 48, 16, 64},
		{16, 48, 32, 64},
		{32, 48, 48, 64},
		{48, 48, 64, 64},
	};

	Rect rcRight[4] = {
		{64, 48, 80, 64},
		{80, 48, 96, 64},
		{96, 48, 112, 64},
		{112, 48, 128, 64},
	};

	if (++bul->ani_wait > 1)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 0;

	if (bul->direct == 0)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void ActBullet_Sword2(BULLET *bul)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->count1 == 3)
		bul->bbits &= ~4;

	if (bul->count1 % 7 == 1)
		PlaySoundObject(106, SOUND_MODE_PLAY);

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x800;
				break;
			case 1:
				bul->ym = -0x800;
				break;
			case 2:
				bul->xm = 0x800;
				break;
			case 3:
				bul->ym = 0x800;
				break;
		}
	}
	else
	{
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	Rect rcLeft[4] = {
		{160, 48, 184, 72},
		{184, 48, 208, 72},
		{208, 48, 232, 72},
		{232, 48, 256, 72},
	};

	Rect rcRight[4] = {
		{160, 72, 184, 96},
		{184, 72, 208, 96},
		{208, 72, 232, 96},
		{232, 72, 256, 96},
	};

	if (++bul->ani_wait > 1)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 0;

	if (bul->direct == 0)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void ActBullet_Sword3(BULLET *bul, int level)
{
	Rect rcLeft[4] = {
		{160, 48, 184, 72},
		{184, 48, 208, 72},
		{208, 48, 232, 72},
		{232, 48, 256, 72},
	};

	Rect rcRight[4] = {
		{160, 72, 184, 96},
		{184, 72, 208, 96},
		{208, 72, 232, 96},
		{232, 72, 256, 96},
	};

	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;
			bul->xm = 0;
			bul->ym = 0;
			// Fallthrough
		case 1:
			switch (bul->direct)
			{
				case 0:
					bul->xm = -0x900;
					break;
				case 1:
					bul->ym = -0x900;
					break;
				case 2:
					bul->xm = 0x900;
					break;
				case 3:
					bul->ym = 0x900;
					break;
			}
			if (level == 3){
				bul->xm *= 1.55;
				bul->ym *= 1.55;
			}

			if (bul->life < 97)
			{
				bul->act_no = 2;
				bul->ani_no = 1;
				bul->damage = -1;
				bul->act_wait = 0;
			}

			if (++bul->act_wait % 4 == 1)
			{
				PlaySoundObject(106, SOUND_MODE_PLAY);

				if (++bul->count1 % 2){
					SetBullet(23, bul->x, bul->y, 0);
				}
				else
					SetBullet(23, bul->x, bul->y, 2);

				if (level == 2 && Random(0, 3) <= 2){
					if (++bul->count1 % 2){
						SetBullet(23, bul->x, bul->y, 0);
					}
					else
						SetBullet(23, bul->x, bul->y, 2);
				}
			}

			if (++bul->count1 == 5)
				bul->bbits &= ~4;

			if (bul->count1 > bul->life_count)
			{
				bul->cond = 0;
				SetCaret(bul->x, bul->y, 3, 0);
				return;
			}

			break;

		case 2:
			bul->xm = 0;
			bul->ym = 0;
			++bul->act_wait;
			int rngRoll = Random(0, 2);
			if (level >= 2){
				int rngRoll2 = Random(0, 2);
				if (rngRoll2 > rngRoll) rngRoll = rngRoll2;
			}

			if (rngRoll == 2)
			{
				PlaySoundObject(106, SOUND_MODE_PLAY);

				if (Random(0, 1) % 2)
					SetBullet(23, bul->x + (Random(-0x15, 0x15) * 0x200), bul->y + (Random(-0x15, 0x15) * 0x200), 0);
				else
					SetBullet(23, bul->x + (Random(-0x15, 0x15) * 0x200), bul->y + (Random(-0x15, 0x15) * 0x200), 2);
			}

			if (level >= 2 && Random(0, 3) <= 1){
				int rngRoll = Random(-1, 1);

			if (rngRoll == 0)
			{
				PlaySoundObject(106, SOUND_MODE_PLAY);

				if (Random(0, 1) % 2)
					SetBullet(23, bul->x + (Random(-0x15, 0x15) * 0x200), bul->y + (Random(-0x15, 0x15) * 0x200), 0);
				else
					SetBullet(23, bul->x + (Random(-0x15, 0x15) * 0x200), bul->y + (Random(-0x15, 0x15) * 0x200), 2);
			}
			}

			if (bul->act_wait > 95)
				bul->cond = 0;
	}

	bul->x += bul->xm;
	bul->y += bul->ym;

	if (gMC.dashDuration > 0){
		bul->x = gMC.x;
		bul->y = gMC.y;
	}
	
	if (++bul->ani_wait > 1)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 0;

	if (bul->direct == 0)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void ActBullet_MiningLaser(BULLET *bul, int level)
{
	bool bHit;

	static unsigned int inc;

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;

			switch (bul->direct)
			{
				case 0:
				case 2:
					bul->tgt_y = bul->y;
					bul->enemyXL = 0x1000;
					bul->blockXL = 0x1000;
					break;

				case 1:
				case 3:
					bul->tgt_x = bul->x;
					bul->enemyYL = 0x1000;
					bul->blockYL = 0x1000;
					break;
			}

				bul->ani_no = 0x500;
			// Fallthrough
		case 1:
			if (bul->count2 != 1){
			switch (bul->direct)
			{
				case 0:
					bul->xm += -bul->ani_no;
					break;
				case 1:
					bul->ym += -bul->ani_no;
					break;
				case 2:
					bul->xm += bul->ani_no;
					break;
				case 3:
					bul->ym += bul->ani_no;
					break;
			}
			}

			if (bul->xm < -0x1900)
				bul->xm = -0x1900;
			if (bul->xm > 0x1900)
				bul->xm = 0x1900;

			if (bul->ym < -0x1900)
				bul->ym = -0x1900;
			if (bul->ym > 0x1900)
				bul->ym = 0x1900;

			bul->x += bul->xm;
			bul->y += bul->ym;

			break;
	}

	if (++bul->count2 > 1)
	{
		bul->count2 = 0;

		switch (bul->direct)
		{
			case 0:
				SetCaret(bul->x + (8 * 0x200), bul->y, 1, 1);
				break;
			case 1:
				SetCaret(bul->x, bul->y + (8 * 0x200), 1, 1);
				break;
			case 2:
				SetCaret(bul->x - (8 * 0x200), bul->y, 1, 1);
				break;
			case 3:
				SetCaret(bul->x, bul->y - (8 * 0x200), 1, 1);
				break;
		}
	}

	Rect rect1[4] = {
		{120, 96, 136, 112},
		{136, 96, 152, 112},
		{152, 96, 168, 112},
		{168, 96, 184, 112},
	};

	Rect rect2[4] = {
		{184, 96, 200, 112},
		{200, 96, 216, 112},
		{216, 96, 232, 112},
		{232, 96, 248, 112},
	};

	bul->iframes = 8;

	switch (level)
	{
		case 1:
			bul->rect = rect1[bul->direct];
			break;
		case 2:
			bul->rect = rect2[bul->direct];
			break;
		case 3:
			bul->rect = rect1[bul->direct];
			break;
	}
}

void ActBullet_SwordSwing(BULLET *bul, int level)
{
	gMC.direct = bul->direct;

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		return;
	} else {
		bul->iframes = bul->life_count;
	}

	if (bul->act_no == 0){
		if (bul->direct == 2){
			bul->angle = -120;
			bul->centerX = 0;
			bul->centerY = 1;
		}
		else{
			bul->centerX = 1;
			bul->centerY = 1;
			bul->angle = 120;
		}
		if (gMC.flag & 0x100){
			bul->life_count /= 2;
			bul->count2 = 1;
		}
		bul->act_no = 1;
	} else {
		int spinValue = 8 + level*2;
		int swingValue = 13;
		if (bul->count2 == 1){
			spinValue *= 2;
			swingValue /= 2;
		}
		if (bul->direct == 2)
			bul->angle += spinValue;
		else
			bul->angle -= spinValue;

		int whirlLimit = 1;
		if (level == 4) whirlLimit = 2;

		if (level >= 3 && bul->count1 == swingValue && CountBulletNum(62) < whirlLimit){
			int lol = 1;
			for (int i = 0; i < lol; i++)
			{
				BULLET *bully = SetBullet(62, bul->x, bul->y);
				if (bully != NULL)
				{
					PlaySoundObject(102, SOUND_MODE_PLAY);
					bully->direct = 0;
					if (bul->direct == 0)
						bully->xm = -0x200 * 5;
					else
						bully->xm = 0x200 * 5;
					if (gMC.up){
						bully->ym = -0x200 * 5;
						bully->xm = 0;
					}
					else if (gMC.down){
						bully->ym = 0x200 * 5;
						bully->xm = 0;
					} else 
						bully->ym = 0;

					if (bul->count2 == 1){
						bully->xm *= 1.4;
						bully->ym *= 1.4;
						bully->count2 = 1;
					}
				}
			}
		}

		if ((level == 2 || level == 4) && bul->count1 == swingValue){
			if (CheckForCooldown(gCooldowns, FLOODTIDE_ATTACK_ID) == -1){
				int lol = Random(30, 40);
				if (level == 4) lol+=20;
				if (bul->count2 == 1) lol+=24;
				InitiateCooldown(gCooldowns, FLOODTIDE_ATTACK_ID);
				for (int i = 0; i < lol; i++)
				{
					BULLET *bully = SetBullet(61, bul->x, bul->y);
					if (bully != NULL)
					{
						PlaySoundObject(21, SOUND_MODE_PLAY);
						bully->direct = 0;
						if (bul->direct == 0)
							bully->xm = -Random(0, 2200);
						else
							bully->xm = Random(0, 2200);
						if (gMC.up){
							bully->ym = Random(-1600, -700);
							bully->xm /= 5;
						}
						else if (gMC.down){
							bully->ym = -Random(-1600, -700);
							bully->xm /= 5;
						}
						else
							bully->ym = Random(-900, 120);
						if (bul->count2 == 1){
							bully->xm *= 1.4;
							bully->ym *= 1.4;
						}
					}
				}
			} else {
				IncrementCooldown(gCooldowns, FLOODTIDE_ATTACK_ID, -1);
				std::remove_if(gCooldowns.begin(), gCooldowns.end(), [](Cooldown c){
					return c.id == FLOODTIDE_ATTACK_ID && c.duration <= 0;
				});
			}
		}

		if (bul->life != 100 && (level == 1 || level == 4)){
			bul->life = 100;
		}
	}

	bul->x = gMC.x + gMC.xm;
	if (gMC.direct == 0){
		bul->x -= 0x200 * 12 * (float(bul->count1) / float(bul->life_count));
	} else {
		bul->x += 0x200 * 12 * (float(bul->count1) / float(bul->life_count));
	}
	bul->y = gMC.y + gMC.ym - 0x200 * 7 + 0x200 * 5 * (float(bul->count1) / float(bul->life_count));

	Rect rcRight[4] = {
		{224, 144, 236, 156},
		{240, 144, 254, 158},
		{256, 144, 271, 159},
		{271, 144, 286, 160},
	};

	Rect rcLeft[4] = {
		{224, 156, 236, 168},
		{240, 158, 254, 172},
		{256, 159, 271, 173},
		{271, 160, 286, 176},
	};

	if (bul->direct == 0)
		bul->rect = rcLeft[level-1];
	else
		bul->rect = rcRight[level-1];
}

void ActBullet_SwordSwing2(BULLET *bul, int level)
{
	gMC.direct = bul->direct;

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		return;
	} else {
		bul->iframes = bul->life_count;
	}

	if (bul->act_no == 0){
		if (bul->direct == 2){
			bul->angle = -180;
			bul->centerX = 0;
			bul->centerY = 1;
		}
		else{
			bul->centerX = 1;
			bul->centerY = 1;
			bul->angle = 180;
		}
		if (gMC.life <= gMC.max_life / 2){
			bul->life_count /= 2;
			bul->count2 = 1;
		}
		bul->act_no = 1;
	} else {
		int spinValue = 9 + level;
		int swingValue = 14;
		if (bul->count2 == 1){
			spinValue *= 2;
			swingValue /= 2;
		}
		if (bul->direct == 2)
			bul->angle += spinValue;
		else
			bul->angle -= spinValue;

		int whirlLimit = 3;
		if (level == 4) whirlLimit = 5;

		// if (level >= 3 && bul->count1 == swingValue && CountBulletNum(62) < whirlLimit){
		// 	int lol = 1;
		// 	for (int i = 0; i < lol; i++)
		// 	{
		// 		BULLET *bully = SetBullet(62, bul->x, bul->y);
		// 		if (bully != NULL)
		// 		{
		// 			PlaySoundObject(102, SOUND_MODE_PLAY);
		// 			bully->direct = 0;
		// 			if (bul->direct == 0)
		// 				bully->xm = -0x200 * 5;
		// 			else
		// 				bully->xm = 0x200 * 5;
		// 			if (gMC.up){
		// 				bully->ym = -0x200 * 5;
		// 				bully->xm = 0;
		// 			}
		// 			else if (gMC.down){
		// 				bully->ym = 0x200 * 5;
		// 				bully->xm = 0;
		// 			} else 
		// 				bully->ym = 0;

		// 			if (bul->count2 == 1){
		// 				bully->xm *= 1.4;
		// 				bully->ym *= 1.4;
		// 				bully->count2 = 1;
		// 			}
		// 		}
		// 	}
		// }

		if ((level == 3) && bul->count1 == swingValue){
			int lol = Random(10, 25);
			for (int i = 0; i < lol; i++)
			{
				BULLET *bully = SetBullet(84, bul->x, bul->y);
				if (bully != NULL)
				{
					PlaySoundObject(33, SOUND_MODE_PLAY);
					bully->direct = gMC.direct;
					if (bul->direct == 0)
						bully->xm = -Random(0, 1800);
					else
						bully->xm = Random(0, 1800);
					if (gMC.up){
						bully->ym = Random(-1300, -400);
						bully->xm /= 3;
					}
					else if (gMC.down){
						bully->ym = -Random(-1300, -400);
						bully->xm /= 3;
					}
					else
						bully->ym = Random(-700, 80);
					if (bul->count2 == 1){
						bully->xm *= 1.2;
						bully->ym *= 1.2;
					}
				}
			}
		}

		if ((level == 2 || level == 4) && bul->count1 == swingValue){
			int lol = 1;
			if (level == 4) lol+=1;
			for (int i = 0; i < lol; i++)
			{
				int flameX = bul->x + Random(-0x2000, 0x2000), flameY = bul->y;
				while (IsEmpty(flameX / 0x2000, flameY / 0x2000) && bul->y - flameY < 0x2000 * 4.5){
					flameY -= 0x2000;
				}
				flameY += 0x2000;
				int flameID = 8;
				if (level == 4) flameID = 83;
				BULLET *bully = SetBullet(flameID, flameX, flameY);
				if (bully != NULL)
				{
					SetDestroyNpChar(flameX, flameY, 0x1000, 8);
					PlaySoundObject(71, SOUND_MODE_PLAY);
					bully->direct = gMC.direct;
					if (level == 2)
						bully->damage = 1;
					bully->life = Random(1, 4);
					if (bul->direct == 0)
						bully->xm = -Random(0, 1800);
					else
						bully->xm = Random(0, 1800);
					if (gMC.up){
						bully->ym = Random(-1300, -400);
						bully->xm /= 5;
					}
					else if (gMC.down){
						bully->ym = -Random(-1300, -400);
						bully->xm /= 5;
					}
					else
						bully->ym = Random(-700, 80);
					if (bul->count2 == 1){
						bully->xm *= 1.4;
						bully->ym *= 1.4;
					}
				}
			}
		}

		if (bul->life != 100 && (level == 1 || level == 4)){
			bul->life = 100;
			BULLET *bully = SetBullet(83, bul->x, bul->y);
			if (bully != NULL)
			{
				PlaySoundObject(21, SOUND_MODE_PLAY);
				bully->direct = gMC.direct;
			}
		}
	}

	bul->x = gMC.x + gMC.xm;
	if (gMC.direct == 0){
		bul->x -= 0x200 * 4 + 0x200 * 3 * (float(bul->count1) / float(bul->life_count));
	} else {
		bul->x += 0x200 * 10 + 0x200 * 3 * (float(bul->count1) / float(bul->life_count));
	}
	bul->y = gMC.y + gMC.ym - 0x200 * 7 + 0x200 * 5 * (float(bul->count1) / float(bul->life_count));

	Rect rcRight[4] = {
		{224, 176, 237, 189},
		{240, 176, 255, 191},
		{256, 176, 272, 192},
		{272, 176, 289, 193},
	};

	Rect rcLeft[4] = {
		{224, 189, 237, 202},
		{240, 191, 255, 204},
		{256, 192, 272, 205},
		{272, 193, 289, 210},
	};

	if (bul->direct == 0)
		bul->rect = rcLeft[level-1];
	else
		bul->rect = rcRight[level-1];
}

void ActBullet_Edge(BULLET *bul)
{
	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;
			bul->y -= 12 * 0x200;

			if (bul->direct == 0)
				bul->x += 16 * 0x200;
			else
				bul->x -= 16 * 0x200;
			// Fallthrough
		case 1:
			if (++bul->ani_wait > 2)
			{
				bul->ani_wait = 0;
				++bul->ani_no;
			}

			if (bul->direct == 0)
				bul->x -= 2 * 0x200;
			else
				bul->x += 2 * 0x200;

			bul->y += 2 * 0x200;

			bul->damage = 1;

			if (bul->ani_no > 4)
			{
				bul->cond = 0;
			#ifdef FIX_BUGS
				return;	// The code below will use 'ani_no' to access 'rcLeft' and 'rcRight', even though it's now too high
			#endif
			}

			break;
	}

	Rect rcLeft[5] = {
		{0, 64, 24, 88},
		{24, 64, 48, 88},
		{48, 64, 72, 88},
		{72, 64, 96, 88},
		{96, 64, 120, 88},
	};

	Rect rcRight[5] = {
		{0, 88, 24, 112},
		{24, 88, 48, 112},
		{48, 88, 72, 112},
		{72, 88, 96, 112},
		{96, 88, 120, 112},
	};

	if (bul->direct == 0)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void ActBullet_Edge2(BULLET *bul)
{
	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;
			bul->y -= 12 * 0x200;

			if (bul->direct == 0)
				bul->x += 16 * 0x200;
			else
				bul->x -= 16 * 0x200;
			// Fallthrough
		case 1:
			if (++bul->ani_wait > 6)
			{
				bul->ani_wait = 0;
				++bul->ani_no;
			}

			if (bul->direct == 0)
				bul->x -= 0.75 * 0x200;
			else
				bul->x += 0.75 * 0x200;

			bul->y += 0.75 * 0x200;

			bul->damage = -1;

			if (bul->ani_no > 4)
			{
				bul->cond = 0;
			#ifdef FIX_BUGS
				return;	// The code below will use 'ani_no' to access 'rcLeft' and 'rcRight', even though it's now too high
			#endif
			}

			break;
	}

	Rect rcLeft[5] = {
		{0, 64, 24, 88},
		{24, 64, 48, 88},
		{48, 64, 72, 88},
		{72, 64, 96, 88},
		{96, 64, 120, 88},
	};

	Rect rcRight[5] = {
		{0, 88, 24, 112},
		{24, 88, 48, 112},
		{48, 88, 72, 112},
		{72, 88, 96, 112},
		{96, 88, 120, 112},
	};

	if (bul->direct == 0)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void ActBullet_Drop(BULLET *bul)
{
	Rect rc = {0, 0, 0, 0};

	if (++bul->act_wait > 2)
		bul->cond = 0;

	bul->rect = rc;
}

void ActBullet_SuperMissile(BULLET *bul, int level)
{
	bool bHit;

	static unsigned int inc;

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	bHit = false;

	if (bul->life != 10)
		bHit = true;
	if (bul->direct == 0 && bul->flag & 1)
		bHit = true;
	if (bul->direct == 2 && bul->flag & 4)
		bHit = true;
	if (bul->direct == 1 && bul->flag & 2)
		bHit = true;
	if (bul->direct == 3 && bul->flag & 8)
		bHit = true;
	if (bul->direct == 0 && bul->flag & 0x80)
		bHit = true;
	if (bul->direct == 0 && bul->flag & 0x20)
		bHit = true;
	if (bul->direct == 2 && bul->flag & 0x40)
		bHit = true;
	if (bul->direct == 2 && bul->flag & 0x10)
		bHit = true;

	if (bHit)
	{
		SetBullet(level + 30, bul->x, bul->y, 0);
		bul->cond = 0;
	}

	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;

			switch (bul->direct)
			{
				case 0:
				case 2:
					bul->tgt_y = bul->y;
					bul->enemyXL = 0x1000;
					bul->blockXL = 0x1000;
					break;

				case 1:
				case 3:
					bul->tgt_x = bul->x;
					bul->enemyYL = 0x1000;
					bul->blockYL = 0x1000;
					break;
			}

			if (level == 3)
			{
				switch (bul->direct)
				{
					case 0:
					case 2:
						if (bul->y > gMC.y)
							bul->ym = 0x100;
						else
							bul->ym = -0x100;

						bul->xm = Random(-0x200, 0x200);
						break;

					case 1:
					case 3:
						if (bul->x > gMC.x)
							bul->xm = 0x100;
						else
							bul->xm = -0x100;

						bul->ym = Random(-0x200, 0x200);
						break;
				}

				switch (++inc % 3)
				{
					case 0:
						bul->ani_no = 0x200;
						break;
					case 1:
						bul->ani_no = 0x100;
						break;
					case 2:
						bul->ani_no = 0xAA;
						break;
				}
			}
			else
			{
				bul->ani_no = 0x200;
			}
			// Fallthrough
		case 1:
			switch (bul->direct)
			{
				case 0:
					bul->xm += -bul->ani_no;
					break;
				case 1:
					bul->ym += -bul->ani_no;
					break;
				case 2:
					bul->xm += bul->ani_no;
					break;
				case 3:
					bul->ym += bul->ani_no;
					break;
			}

			if (level == 3)
			{
				switch (bul->direct)
				{
					case 0:
					case 2:
						if (bul->y < bul->tgt_y)
							bul->ym += 0x40;
						else
							bul->ym -= 0x40;

						break;
					case 1:
					case 3:
						if (bul->x < bul->tgt_x)
							bul->xm += 0x40;
						else
							bul->xm -= 0x40;

						break;
				}
			}

			if (bul->xm < -0x1400)
				bul->xm = -0x1400;
			if (bul->xm > 0x1400)
				bul->xm = 0x1400;

			if (bul->ym < -0x1400)
				bul->ym = -0x1400;
			if (bul->ym > 0x1400)
				bul->ym = 0x1400;

			bul->x += bul->xm;
			bul->y += bul->ym;

			break;
	}

	if (++bul->count2 > 2)
	{
		bul->count2 = 0;

		switch (bul->direct)
		{
			case 0:
				SetCaret(bul->x + (8 * 0x200), bul->y, 7, 2);
				break;
			case 1:
				SetCaret(bul->x, bul->y + (8 * 0x200), 7, 3);
				break;
			case 2:
				SetCaret(bul->x - (8 * 0x200), bul->y, 7, 0);
				break;
			case 3:
				SetCaret(bul->x, bul->y - (8 * 0x200), 7, 1);
				break;
		}
	}

	Rect rect1[4] = {
		{120, 96, 136, 112},
		{136, 96, 152, 112},
		{152, 96, 168, 112},
		{168, 96, 184, 112},
	};

	Rect rect2[4] = {
		{184, 96, 200, 112},
		{200, 96, 216, 112},
		{216, 96, 232, 112},
		{232, 96, 248, 112},
	};

	switch (level)
	{
		case 1:
			bul->rect = rect1[bul->direct];
			break;
		case 2:
			bul->rect = rect2[bul->direct];
			break;
		case 3:
			bul->rect = rect1[bul->direct];
			break;
	}
}

void ActBullet_SuperBom(BULLET *bul, int level)
{
	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;

			switch (level)
			{
				case 1:
					bul->act_wait = 10;
					break;
				case 2:
					bul->act_wait = 15;
					break;
				case 3:
					bul->act_wait = 6;
					break;
			}

			PlaySoundObject(44, SOUND_MODE_PLAY);
			// Fallthrough
		case 1:
			switch (level)
			{
				case 1:
					if (bul->act_wait % 3 == 0)
						SetDestroyNpCharUp(bul->x + (Random(-16, 16) * 0x200), bul->y + (Random(-16, 16) * 0x200), bul->enemyXL, 2);
					break;
				case 2:
					if (bul->act_wait % 3 == 0)
						SetDestroyNpCharUp(bul->x + (Random(-48, 48) * 0x200), bul->y + (Random(-48, 48) * 0x200), bul->enemyXL, 7);
					break;
				case 3:
					if (bul->act_wait % 3 == 0)
						SetDestroyNpCharUp(bul->x + (Random(-40, 40) * 0x200), bul->y + (Random(-40, 40) * 0x200), bul->enemyXL, 2);
					break;
			}

			if (--bul->act_wait < 0)
				bul->cond = 0;

			break;
	}
}

void ActBullet_Nemesis(BULLET *bul, int level)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->act_no = 1;
		bul->count1 = 0;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x1000;
				break;
			case 1:
				bul->ym = -0x1000;
				break;
			case 2:
				bul->xm = 0x1000;
				break;
			case 3:
				bul->ym = 0x1000;
				break;
		}

		switch (level)
		{
			case 3:
				bul->xm /= 3;
				bul->ym /= 3;
				break;
		}
	}
	else
	{
		if (level == 1 && bul->count1 % 4 == 1)
		{
			switch (bul->direct)
			{
				case 0:
					SetNpChar(4, bul->x, bul->y, -0x200, Random(-0x200, 0x200), 2, NULL, 0x100);
					break;
				case 1:
					SetNpChar(4, bul->x, bul->y, Random(-0x200, 0x200), -0x200, 2, NULL, 0x100);
					break;
				case 2:
					SetNpChar(4, bul->x, bul->y, 0x200, Random(-0x200, 0x200), 2, NULL, 0x100);
					break;
				case 3:
					SetNpChar(4, bul->x, bul->y, Random(-0x200, 0x200), 0x200, 2, NULL, 0x100);
					break;
			}
		}

		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	if (++bul->ani_no > 1)
		bul->ani_no = 0;

	Rect rcL[2] = {
		{0, 112, 32, 128},
		{0, 128, 32, 144},
	};

	Rect rcU[2] = {
		{32, 112, 48, 144},
		{48, 112, 64, 144},
	};

	Rect rcR[2] = {
		{64, 112, 96, 128},
		{64, 128, 96, 144},
	};

	Rect rcD[2] = {
		{96, 112, 112, 144},
		{112, 112, 128, 144},
	};

	switch (bul->direct)
	{
		case 0:
			bul->rect = rcL[bul->ani_no];
			break;
		case 1:
			bul->rect = rcU[bul->ani_no];
			break;
		case 2:
			bul->rect = rcR[bul->ani_no];
			break;
		case 3:
			bul->rect = rcD[bul->ani_no];
			break;
	}

	bul->rect.top += ((level - 1) / 2) * 32;
	bul->rect.bottom += ((level - 1) / 2) * 32;
	bul->rect.left += ((level - 1) % 2) * 128;
	bul->rect.right += ((level - 1) % 2) * 128;
}

void ActBullet_Spur(BULLET *bul, int level)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (GetNPCFlag(4400) || GetNPCFlag(7999)){
		bul->bbits = 104;
	}

	if (bul->damage && bul->life != 100)
		bul->damage = 0;

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x1000;
				break;
			case 1:
				bul->ym = -0x1000;
				break;
			case 2:
				bul->xm = 0x1000;
				break;
			case 3:
				bul->ym = 0x1000;
				break;
		}

		switch (level)
		{
			case 1:
				switch (bul->direct)
				{
					case 0:
						bul->enemyYL = 0x400;
						break;
					case 1:
						bul->enemyXL = 0x400;
						break;
					case 2:
						bul->enemyYL = 0x400;
						break;
					case 3:
						bul->enemyXL = 0x400;
						break;
				}

				break;

			case 2:
				switch (bul->direct)
				{
					case 0:
						bul->enemyYL = 0x800;
						break;
					case 1:
						bul->enemyXL = 0x800;
						break;
					case 2:
						bul->enemyYL = 0x800;
						break;
					case 3:
						bul->enemyXL = 0x800;
						break;
				}

				break;
		}
	}
	else
	{
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	Rect rect1[2] = {
		{128, 32, 144, 48},
		{144, 32, 160, 48},
	};

	Rect rect2[2] = {
		{160, 32, 176, 48},
		{176, 32, 192, 48},
	};

	Rect rect3[2] = {
		{128, 48, 144, 64},
		{144, 48, 160, 64},
	};

	bul->damage = bul->life;

	if (!GetNPCFlag(4400) && !GetNPCFlag(7999)){
	switch (level)
	{
		case 1:
			if (bul->direct == 1 || bul->direct == 3)
				bul->rect = rect1[1];
			else
				bul->rect = rect1[0];

			break;

		case 2:
			if (bul->direct == 1 || bul->direct == 3)
				bul->rect = rect2[1];
			else
				bul->rect = rect2[0];

			break;

		case 3:
			if (bul->direct == 1 || bul->direct == 3)
				bul->rect = rect3[1];
			else
				bul->rect = rect3[0];

			break;
	}
	}
	else {
		if (bul->count1 % (5 - level) == 0){
			SetNpChar(146, bul->x, bul->y, 0, 0, 2, NULL, 0x100);
		}
	}

	bul->iframes = 2;

	SetBullet(39 + level, bul->x, bul->y, bul->direct);
}

void ActBullet_Spur2(BULLET *bul, int level)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (GetNPCFlag(4400) || GetNPCFlag(7999)){
		bul->bbits = 104;
	}

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x1300;
				break;
			case 1:
				bul->ym = -0x1300;
				break;
			case 2:
				bul->xm = 0x1300;
				break;
			case 3:
				bul->ym = 0x1300;
				break;
		}

		switch (level)
		{
			case 1:
				switch (bul->direct)
				{
					case 0:
						bul->enemyYL = 0x400;
						break;
					case 1:
						bul->enemyXL = 0x400;
						break;
					case 2:
						bul->enemyYL = 0x400;
						break;
					case 3:
						bul->enemyXL = 0x400;
						break;
				}

				break;

			case 2:
				switch (bul->direct)
				{
					case 0:
						bul->enemyYL = 0x800;
						break;
					case 1:
						bul->enemyXL = 0x800;
						break;
					case 2:
						bul->enemyYL = 0x800;
						break;
					case 3:
						bul->enemyXL = 0x800;
						break;
				}

				break;
		}
	}
	else
	{
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	bul->damage = -1;
	bul->iframes = 3;

	SetBullet(32, bul->x, bul->y, bul->direct);
}

void ActBullet_SpurTail(BULLET *bul, int level)
{
	if (++bul->count1 > 20)
		bul->ani_no = bul->count1 - 20;

	if (bul->ani_no > 2)
	{
		bul->cond = 0;
	#ifdef FIX_BUGS
		return;	// Avoid accessing the Rect arrays with an out-of-bounds index
	#endif
	}

	if (bul->damage && bul->life != 100)
		bul->damage = 0;

	Rect rc_h_lv1[3] = {
		{192, 32, 200, 40},
		{200, 32, 208, 40},
		{208, 32, 216, 40},
	};

	Rect rc_v_lv1[3] = {
		{192, 40, 200, 48},
		{200, 40, 208, 48},
		{208, 40, 216, 48},
	};

	Rect rc_h_lv2[3] = {
		{216, 32, 224, 40},
		{224, 32, 232, 40},
		{232, 32, 240, 40},
	};

	Rect rc_v_lv2[3] = {
		{216, 40, 224, 48},
		{224, 40, 232, 48},
		{232, 40, 240, 48},
	};

	Rect rc_h_lv3[3] = {
		{240, 32, 248, 40},
		{248, 32, 256, 40},
		{256, 32, 264, 40},
	};

	Rect rc_v_lv3[3] = {
		{240, 32, 248, 40},
		{248, 32, 256, 40},
		{256, 32, 264, 40},
	};

	bul->iframes = 1;

	if (!GetNPCFlag(4400) && !GetNPCFlag(7999)){
	switch (level)
	{
		case 1:
			if (bul->direct == 0 || bul->direct == 2)
				bul->rect = rc_h_lv1[bul->ani_no];
			else
				bul->rect = rc_v_lv1[bul->ani_no];

			break;

		case 2:
			if (bul->direct == 0 || bul->direct == 2)
				bul->rect = rc_h_lv2[bul->ani_no];
			else
				bul->rect = rc_v_lv2[bul->ani_no];

			break;

		case 3:
			if (bul->direct == 0 || bul->direct == 2)
				bul->rect = rc_h_lv3[bul->ani_no];
			else
				bul->rect = rc_v_lv3[bul->ani_no];

			break;
	}
	}
}

void ActBullet_EnemyClear(BULLET *bul)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		return;
	}

	bul->damage = 10000;
	bul->enemyXL = 0xC8000;
	bul->enemyYL = 0xC8000;
}

void ActBullet_LittleMan(BULLET *bul)
{
	if (++bul->count1 > bul->life_count)
		bul->cond = 0;
}

void ActBullet_HomingSword(BULLET *bul)
{
	Rect rcLeft[] = {
		{160, 48, 184, 72},
		{184, 48, 208, 72},
		{208, 48, 232, 72},
		{232, 48, 256, 72}
	};

	Rect rcRight[] = {
		{232, 72, 256, 96},
		{208, 72, 232, 96},
		{184, 72, 208, 96},
		{160, 72, 184, 96}
	};

	if (bul->npc != NULL && (!(bul->npc->cond & 0x80) || bul->npc->cond & 8 || bul->npc->life <= 0)){
			bul->npc = NULL;
			return;
	}
	bul->life = 100;
	int fx, fy, mx, my;
	if (gKeyTrg & KEY_MOUSE){
		PlaySoundObject(18);
		if (bul->direct == 0)
			bul->direct = 1;
		else
			bul->direct = 0;
	}

	float speedMod = 1.0f;
	if (bul->direct == 1)
		speedMod = 1.5f;

	if (bul->direct == 1){
		mx = gMC.x + gMC.xm + Random(-64_pix, 64_pix);
		my = gMC.y + gMC.ym + Random(-64_pix, 64_pix);
		if (bul->npc != NULL){
			mx = gMC.x + gMC.xm;
			my = gMC.y + gMC.ym;
		}
	} else {
		GetFramePosition(&fx, &fy);
		mx = fx + ScreenToSubpixelCoord(gMouseX);
		my = fy + ScreenToSubpixelCoord(gMouseY);
	}

	

	if (bul->npc == NULL)
	{
		bul->damage = 0;
		switch (bul->act_no)
		{
		default:
			bul->act_no = 1;
			bul->ani_no = 0;
			// Fallthrough
		case 1:
			bul->y += bul->ym;

			if (++bul->act_wait > 2)
				bul->act_no = 10;

			break;

		case 10:

			if (mx < bul->x)
				bul->xm -= 250 * speedMod;
			else
				bul->xm += 250 * speedMod;

			if (my < bul->y)
				bul->ym -= 250 * speedMod;
			else
				bul->ym += 250 * speedMod;
			if (bul->xm > 0x600 * speedMod)
				bul->xm = 0x600 * speedMod;
			if (bul->xm < -0x600 * speedMod)
				bul->xm = -0x600 * speedMod;

			if (bul->ym > 0x600 * speedMod)
				bul->ym = 0x600 * speedMod;
			if (bul->ym < -0x600 * speedMod)
				bul->ym = -0x600 * speedMod;

			bul->x += bul->xm;
			bul->y += bul->ym;

			for (int i = 0; i < NPC_MAX; i++)
			{
				if ((bul->x > gNPC[i].x - gNPC[i].view.front &&
					 bul->x < gNPC[i].x + gNPC[i].view.back &&
					 bul->y > gNPC[i].y - gNPC[i].view.top &&
					 bul->y < gNPC[i].y + gNPC[i].view.bottom) &&
					gNPC[i].damage == 0 && (gNPC[i].cond & 0x80))
				{
					if (gNPC[i].code_char == 1)
					{
						PlaySoundObject(14, SOUND_MODE_PLAY);
						AddExpMyChar(gNPC[i].exp);
						gNPC[i].cond = 0;
					}

					if (gNPC[i].code_char == 86)
					{
						if (gBL.flag == true){
							if (static_iframes[i][0] == 0){
								SetCaret((bul->x + gNPC[i].x) / 2, (bul->y + gNPC[i].y) / 2, 2, 2);
								PlaySoundObject(31, SOUND_MODE_PLAY);
								static_iframes[i][0] = 20;
							}
						} else {
							PlaySoundObject(42, SOUND_MODE_PLAY);
							AddBulletMyChar(gNPC[i].code_event, gNPC[i].exp);
							SetCaret(gNPC[i].x, gNPC[i].y, 3, 0);
							SetCaret(gNPC[i].x, gNPC[i].y, 18, 0);
							if (gMC.a == 20)
							{
								StartTextScript(303);
							}
							gNPC[i].cond = 0;
						}
					}

					if (gNPC[i].code_char == 87)
					{
						PlaySoundObject(20, SOUND_MODE_PLAY);
						AddLifeMyChar(gNPC[i].exp);
						gNPC[i].cond = 0;
					}
				}
			}

			break;
		}
	}
	else {
		int deg;
		if (Distance(mx, my, bul->x, bul->y) > 0x2000 * 8){
			bul->npc = NULL;
			return;
		}
		bul->iframes = 8;
		switch (bul->act_no)
		{
		default:
			bul->act_no = 20;
			//Fallthrough
		case 20:
			bul->act_no = 21;
			bul->act_wait = 0;
			bul->ani_no = 0;
			bul->ani_wait = 0;
			// Fallthrough
		case 21:
			// bul->xm = (bul->xm * 6) / 7;
			// bul->ym = (bul->ym * 6) / 7;

			if (++bul->act_wait > 10)
				bul->act_no = 30;

			break;

		case 30:
			bul->act_no = 31;
			bul->act_wait = 0;
			bul->ani_no = 2;
			// Fallthrough
		case 31:
			if (Distance(bul->x, bul->y, bul->npc->x, bul->npc->y) < 0x2000){
				bul->act_no = 34;
			} else {
				bul->act_no = 32;
			}

			break;

		case 32:
			// bul->act_no = 33;
			bul->act_wait = 0;

			bul->tgt_x = bul->npc->x;
			bul->tgt_y = bul->npc->y;

			if (bul->tgt_x < bul->x)
				bul->xm -= 200 * speedMod;
			else
				bul->xm += 200 * speedMod;

			if (bul->tgt_y < bul->y)
				bul->ym -= 200 * speedMod;
			else
				bul->ym += 200 * speedMod;
			if (bul->xm > 0x600 * speedMod)
				bul->xm = 0x600 * speedMod;
			if (bul->xm < -0x600 * speedMod)
				bul->xm = -0x600 * speedMod;

			if (bul->ym > 0x600 * speedMod)
				bul->ym = 0x600 * speedMod;
			if (bul->ym < -0x600 * speedMod)
				bul->ym = -0x600 * speedMod;
			if (Distance(bul->x, bul->y, bul->npc->x, bul->npc->y) < 0x2000){
				bul->act_no = 34;
			}
			break;

		case 34:
			bul->act_no = 35;
			bul->act_wait = 0;
			bul->damage = 3;

			bul->tgt_x = bul->npc->x;
			bul->tgt_y = bul->npc->y;

			deg = GetArktan(bul->x - bul->tgt_x, bul->y - bul->tgt_y);
			deg += Random(-16, 16);
			bul->ym = 1 * GetSin(deg) * speedMod;
			bul->xm = 1 * GetCos(deg) * speedMod;
			// Fallthrough
		case 35:
			if (++bul->act_wait > 2){
				bul->act_no = 20;
			}

			if (++bul->ani_wait > 1)
			{
				bul->ani_wait = 0;
				++bul->ani_no;
			}

			if (bul->ani_no > 3)
				bul->ani_no = 0;

			break;
		}

		bul->x += bul->xm;
		bul->y += bul->ym;
		if (!(bul->npc->cond & 0x80) || bul->npc->cond & 8 || bul->npc->life <= 0){
			bul->npc = NULL;
			return;
		}
	}

	SetCaret(bul->x, bul->y, 3, 0);
	SetCaret(bul->x, bul->y, 13, 0);
	SetCaret(bul->x, bul->y, 13, 0);
	SetCaret(bul->x, bul->y, 13, 0);
}

void ActBullet_HomingFrontieBust(BULLET *bul)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	bul->npc = GetClosest(bul->x, bul->y, 60_pix);

	if (bul->act_no == 0){
		bul->act_no = 1;

		// Set speed
		switch (bul->direct)
		{
		case 0:
			bul->xm = -0x1000;
			break;
		case 1:
			bul->ym = -0x1000;
			break;
		case 2:
			bul->xm = 0x1000;
			break;
		case 3:
			bul->ym = 0x1000;
			break;
		}
	} else {
		if (bul->npc != NULL){
			unsigned char deg = GetArktan(bul->x - bul->npc->x, bul->y - bul->npc->y);
			bul->ym = GetSin(deg) * 6;
			bul->xm = GetCos(deg) * 6;
		}
	}

	bul->x += bul->xm;
	bul->y += bul->ym;

	SetCaret(bul->x, bul->y, 3, 0);
	SetCaret(bul->x, bul->y, 2, 2);
	for (int i = 0; i < 3; i++)
		SetCaret(bul->x + Random(-bul->view.front, bul->view.back),
						bul->y + Random(-bul->view.top, bul->view.bottom), 13, 0);
}

void ActBullet_HomingFrontieBust2(BULLET *bul)
{
	if (bul->npc == NULL){
		if (++bul->count1 > bul->life_count)
		{
			bul->cond = 0;
			SetCaret(bul->x, bul->y, 3, 0);
			return;
		}
	}

	bul->npc = GetClosest(bul->x, bul->y, 55_pix);
	bul->local_iframes = 15;
	int inaccuracy = 0x260;

	if (bul->act_no == 0){
		bul->act_no = 1;

		// Set speed
		switch (bul->direct)
		{
		case 0:
			bul->xm = -0x1000;
			bul->ym = Random(-inaccuracy, inaccuracy);
			break;
		case 1:
			bul->ym = -0x1000;
			bul->xm = Random(-inaccuracy, inaccuracy);
			break;
		case 2:
			bul->xm = 0x1000;
			bul->ym = Random(-inaccuracy, inaccuracy);
			break;
		case 3:
			bul->ym = 0x1000;
			bul->xm = Random(-inaccuracy, inaccuracy);
			break;
		}
	} else {
		if (bul->npc != NULL){
			unsigned char deg = GetArktan(bul->x - bul->npc->x, bul->y - bul->npc->y);
			bul->ym = GetSin(deg) * 8;
			bul->xm = GetCos(deg) * 8;
			bul->bbits = 4;
		} else {
			bul->bbits = 32;
		}
	}

	bul->x += bul->xm;
	bul->y += bul->ym;

	SetCaret(bul->x, bul->y, 2, 2);
}

void ActBullet_Flame(BULLET *bul)
{
	Rect rc_h_lv1[3] = {
		{0, 144+16, 8, 160+16},
		{8, 144+16, 16, 160+16},
		{16, 144+16, 24, 160+16},
	};

	if (++bul->ani_no > 2)
		bul->ani_no = 0;

	bul->rect = rc_h_lv1[bul->ani_no];

	if (bul->flag & 8)
		bul->ym = -0x120;
	
	bul->xm *= 0.95;

	if (bul->ym < 0)
		bul->ym += 30;
	else
		bul->ym += 90;
	if (bul->ym > 0x450)
		bul->ym = 0x450;

	bul->y += bul->ym;
	bul->x += bul->xm;

	bul->opacity -= 0.02f;

	if (++bul->count1 > bul->life_count){
		bul->cond = 0;
		// SetCaret(bul->x, bul->y, 3, 0);
	}
}

void ActBullet_RedBurst(BULLET *bul)
{
	if (++bul->ani_no > Random(0, 3)){
		bul->ani_no = 0;
		SetCaret(bul->x, bul->y, 11, 0);
	}

	bul->y += bul->ym;
	bul->x += bul->xm;
}

void ActBullet_Nanites(BULLET *bul)
{
	unsigned char deg, lol, speed;

	if (bul->act_no == 0)
	{
		bul->ani_no = Random(0, 2);
		bul->act_no = 1;

		bul->count1 = 0;
		bul->count2 = 60;
	}
	else
	{
		if (CheckForCooldown(gCooldowns, NANITES_CD_ID) == -1){
			lol = 3;
			speed = (CheckForCooldown(gCooldowns, NANITES_TIME_ID) > 0 ? 16 : 8);
			
			if (GetNPCFlag(883)){
				short rate = (CheckForCooldown(gCooldowns, NANITES_TIME_ID) > 0 ? 5 : 40);
				if (++bul->act_wait % rate == 0){
					short amount = (CheckForCooldown(gCooldowns, NANITES_TIME_ID) > 0 ? 2 : 1);
					for (int i = 0; i < amount; i++){
						BULLET *bullet = SetBullet(47, bul->x, bul->y);
						if (bullet != NULL){
							bullet->direct = 1;
							bullet->tgt_x = gMC.x;
							bullet->tgt_y = gMC.y;
						}
					}
				}
			}
		} else {
			lol = 0;
			speed = 20;
		}
		bul->count1 += speed;
		SetNpChar(129, bul->x, bul->y, 0, 0, bul->ani_no + lol, NULL, 0x100);

		deg = bul->count1 / 4;

		bul->x = gMC.x + gMC.xm + bul->count2 * GetCos(deg) / 4;
		bul->y = gMC.y + gMC.ym + bul->count2 * GetSin(deg) / 4;

		// bul->xm = bul->tgt_x - bul->x;
		// bul->ym = bul->tgt_y - bul->y;
		// if ((bul->count1 / 2 % 2) == 0)
		// 	bul->ym = (bul->tgt_y - bul->y) / 2;
	}

	if (++bul->ani_wait > 0)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 2)
		bul->ani_no = 0;

	// bul->x += bul->xm;
	// bul->y += bul->ym;

	if (!GetNPCFlag(882) && !GetNPCFlag(883)){
		bul->cond = 0;
		return;
	}
		
	bul->rect = {0,0,0,0};
}

void ActBullet_Droplet(BULLET *bul)
{
	if (++bul->ani_no > Random(4, 6)){
		bul->ani_no = 0;
		SetCaret(bul->x, bul->y, 1, 0);
	}

	if (++bul->count1 > 10){
		bul->damage = 1;
	}

	if (bul->flag & 8)
		bul->ym = -0x120;
	
	bul->xm *= 0.99;

	if (bul->ym < 0)
		bul->ym += 10;
	else
		bul->ym += 100;
	if (bul->ym > 0x550)
		bul->ym = 0x550;

	bul->y += bul->ym;
	bul->x += bul->xm;

	if (bul->count1 > bul->life_count){
		bul->cond = 0;
		// SetCaret(bul->x, bul->y, 3, 0);
	}
}

void ActBullet_Spark(BULLET *bul)
{
	Rect rcLeft[4] = {
		{256, 128, 264, 136},
		{264, 128, 272, 136},
		{272, 128, 280, 136},
		{280, 128, 288, 136},
	};

	if (++bul->ani_wait > 9)
	{
		bul->ani_wait = 0;

		if (++bul->ani_no > 3)
		{
			bul->cond = 0;
			return;
		}
	}

	if (bul->act_no == 0){
		bul->act_no = 1;
		int min_speed = 1700;
		int max_speed = 3000;
		switch (bul->direct){
			case 0:
				bul->xm = -Random(min_speed, max_speed);
				bul->ym = Random(-0x160, 0x160);
				break;
			case 1:
				bul->ym = -Random(min_speed, max_speed);
				bul->xm = Random(-0x160, 0x160);
				break;
			case 2:
				bul->xm = Random(min_speed, max_speed);
				bul->ym = Random(-0x160, 0x160);
				break;
			case 3:
				bul->ym = Random(min_speed, max_speed);
				bul->xm = Random(-0x160, 0x160);
				break;
		}
	} else {
	
		bul->xm *= 0.98;
		bul->ym *= 0.99;
		
		bul->y += bul->ym;
		bul->x += bul->xm;

		bul->rect = rcLeft[bul->ani_no];
	}
}

void ActBullet_Flamings(BULLET *bul)
{
	Rect rc_h_lv1[3] = {
		{0, 144, 8, 160},
		{8, 144, 16, 160},
		{16, 144, 24, 160},
	};

	if (++bul->ani_no > 2)
		bul->ani_no = 0;

	bul->rect = rc_h_lv1[bul->ani_no];

	if (bul->flag & 8)
		bul->ym = -0x120;

	bul->ym += 70;
	if (bul->ym > 0x3FF)
		bul->ym = 0x3FF;
	bul->iframes = 15;

	bul->xm *= 0.975;

	bul->x += bul->xm;
	bul->y += bul->ym;

	bul->opacity *= 0.98;


	if (++bul->count1 > bul->life_count)
		bul->cond = 0;
	else if (bul->count1 > 25)
		bul->damage = 2;
}

void ActBullet_Star(BULLET *bul)
{
	Rect rc_h_lv1[3] = {
		{0, 144, 8, 160},
		{8, 144, 16, 160},
		{16, 144, 24, 160},
	};

	if (++bul->ani_no > 2)
		bul->ani_no = 0;

	bul->rect = rc_h_lv1[bul->ani_no];

	if (bul->flag & 8)
		bul->ym = -0x120;

	bul->ym += 70;
	if (bul->ym > 0x3FF)
		bul->ym = 0x3FF;

	bul->y += bul->ym;

	if (++bul->count1 > bul->life_count)
		bul->cond = 0;
}

void ActBullet_Whirlpool(BULLET *bul)
{
	bul->rect = {288, 128, 298, 138};
	bul->life_count = 500;
	bul->iframes = 10;
	if (GetNPCFlag(705)){
			if (GetNPCFlag(872)) bul->life_count *= 1.33;
			else bul->life_count *= 1.25;
		}

	if (++bul->ani_wait > 0)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->flag & 0xF){
		int deg = GetArktan(bul->x - (bul->x + Random(-0x200 * 4, 0x200 * 4)), 
			bul->y - (bul->y + Random(-0x200 * 4, 0x200 * 4)));
		deg += (unsigned char)Random(-18, 18);
		bul->ym = GetSin(deg) * 6;
		bul->xm = GetCos(deg) * 6;
		if (bul->count2 == 1){
			bul->xm += GetCos(deg) * 2;
			bul->ym += GetSin(deg) * 2;
		}
	}

	if (++bul->act_wait > 6 - (bul->count2 == 1 ? 2 : 0) || bul->life != 100){
		bul->act_wait = 0;
		int lol = Random(2, 4);
		if (bul->life != 100)
			lol = Random(0, 3);
		bul->life = 100;
		for (int i = 0; i < lol; i++)
		{
			BULLET *bully = SetBullet(61, bul->x, bul->y);
			if (bully != NULL)
			{
				bully->direct = 0;
				bully->xm = Random(-970, 970);
				bully->ym = Random(-970, 970);
				if (bul->count2 == 1)
				{
					bully->xm *= 1.4;
					bully->ym *= 1.4;
				}
			}
		}
	}

	if (bul->ani_no > 2)
		bul->ani_no = 0;

	SetNpChar(129, bul->x, bul->y, 0, -0x200, bul->ani_no + 3, NULL, 0x100);

	bul->y += bul->ym;
	bul->x += bul->xm;

	if (++bul->count1 > bul->life_count){
		bul->cond = 0;
		int lol = Random(40, 80);
		for (int i = 0; i < lol; i++)
		{
			BULLET *bully = SetBullet(61, bul->x, bul->y);
			if (bully != NULL)
			{
				bully->direct = 0;
				bully->xm = Random(-1200, 1200);
				bully->ym = Random(-1000, 200);
				if (bul->count2 == 1){
					bully->xm *= 1.4;
					bully->ym *= 1.4;
				}
			}
		}
	}
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
				switch (gBul[i].code_bullet)
				{
					// Snake
					case 1:
						ActBullet_Frontia1(&gBul[i]);
						break;
					case 2:
						ActBullet_Frontia2(&gBul[i], 2);
						break;
					case 3:
						ActBullet_Frontia2(&gBul[i], 3);
						break;
					case 70:
						ActBullet_Frontia2(&gBul[i], 4);
						break;

					// Polar Star
					case 4:
						ActBullet_PoleStar(&gBul[i], 1);
						break;
					case 5:
						ActBullet_PoleStar(&gBul[i], 2);
						break;
					case 6:
						ActBullet_PoleStar(&gBul[i], 3);
						break;

					// Fireball
					case 7:
						ActBullet_FireBall(&gBul[i], 1);
						break;
					case 8:
						ActBullet_FireBall(&gBul[i], 2);
						break;
					case 9:
						ActBullet_FireBall(&gBul[i], 3);
						break;
					case 68:
						ActBullet_FireBall(&gBul[i], 4);
						break;

					// Machine Gun
					case 10:
						ActBullet_MachineGun(&gBul[i], 1);
						break;
					case 11:
						ActBullet_MachineGun(&gBul[i], 2);
						break;
					case 12:
						ActBullet_MachineGun(&gBul[i], 3);
						break;
					case 69:
						ActBullet_MachineGun(&gBul[i], 4);
						break;

					// Missile Launcher
					case 13:
						ActBullet_Missile(&gBul[i], 1);
						break;
					case 14:
						ActBullet_Missile(&gBul[i], 2);
						break;
					case 15:
						ActBullet_Missile(&gBul[i], 3);
						break;

					// Missile Launcher explosion
					case 16:
						ActBullet_Bom(&gBul[i], 1);
						break;
					case 17:
						ActBullet_Bom(&gBul[i], 2);
						break;
					case 18:
						ActBullet_Bom(&gBul[i], 3);
						break;

					// Bubbler
					case 19:
						ActBullet_Bubblin1(&gBul[i]);
						break;
					case 20:
						ActBullet_Bubblin2(&gBul[i]);
						break;
					case 21:
						ActBullet_Bubblin3(&gBul[i]);
						break;
					case 71:
						ActBullet_Bubblin4(&gBul[i]);
						break;

					// Bubbler level 3 spines
					case 22:
						ActBullet_Spine(&gBul[i]);
						break;

					// Blade slashes
					case 23:
						ActBullet_Edge(&gBul[i]);
						break;

					// Falling spike that deals 127 damage
					case 24:
						ActBullet_Drop(&gBul[i]);
						break;

					// Blade
					case 25:
						ActBullet_Sword3(&gBul[i], 1);
						break;
					case 26:
						ActBullet_Sword2(&gBul[i]);
						break;
					case 27:
						ActBullet_Sword3(&gBul[i], 2);
						break;
					case 72:
						ActBullet_Sword3(&gBul[i], 3);
						break;

					// Super Missile Launcher
					case 28:
						ActBullet_Rifle(&gBul[i], 1);
						break;
					case 29:
						ActBullet_Rifle(&gBul[i], 2);
						break;
					case 30:
						ActBullet_Rifle(&gBul[i], 3);
						break;

					// Super Missile Launcher explosion
					case 31:
						ActBullet_SuperBom(&gBul[i], 1);
						break;
					case 32:
						ActBullet_SuperBom(&gBul[i], 2);
						break;
					case 33:
						ActBullet_SuperBom(&gBul[i], 3);
						break;

					// Nemesis
					case 34:	// Identical to case 43
						ActBullet_Nemesis(&gBul[i], 1);
						break;
					case 35:
						ActBullet_Nemesis(&gBul[i], 2);
						break;
					case 36:
						ActBullet_Duck(&gBul[i]);
						break;

					// Spur
					case 37:
						ActBullet_Spur(&gBul[i], 1);
						break;
					case 38:
						ActBullet_Spur(&gBul[i], 2);
						break;
					case 39:
						ActBullet_Spur(&gBul[i], 3);
						break;
					case 76:
						ActBullet_Spur2(&gBul[i], 3);
						break;

					// Spur trail
					case 40:
						ActBullet_SpurTail(&gBul[i], 1);
						break;
					case 41:
						ActBullet_SpurTail(&gBul[i], 2);
						break;
					case 42:
						ActBullet_SpurTail(&gBul[i], 3);
						break;

					// Curly's Nemesis
					case 43:	// Identical to case 34
						ActBullet_Nemesis(&gBul[i], 1);
						break;

					// Screen-nuke that kills all enemies
					case 44:
						ActBullet_EnemyClear(&gBul[i]);
						break;

					// Whimsical Star
					case 45:
						ActBullet_Star(&gBul[i]);
						break;

					// Little Man
					case 46:
					case 77:
						ActBullet_LittleMan(&gBul[i]);
						break;

					case 47:
						ActBullet_Homing(&gBul[i]);
						break;

					case 49:
						ActBullet_Hook(&gBul[i], 1);
						break;
					case 50:
						ActBullet_Hook(&gBul[i], 2);
						break;
					case 51:
						ActBullet_Hook(&gBul[i], 3);
						break;
					case 73:
						ActBullet_Hook(&gBul[i], 4);
						break;

					case 52: case 55:
						ActBullet_BoomerangSword1(&gBul[i]);
						break;

					case 53:
						ActBullet_BoomerangSword2(&gBul[i]);
						break;

					case 54:
						ActBullet_BoomerangSword3(&gBul[i]);
						break;

					case 75:
						ActBullet_BoomerangSword4(&gBul[i]);
						break;

					case 56:
						ActBullet_Flame(&gBul[i]);
						break;

					case 57:
						ActBullet_HomingSword(&gBul[i]);
						break;

					case 58:
						ActBullet_SwordSwing(&gBul[i], 1);
						break;
					case 59:
						ActBullet_SwordSwing(&gBul[i], 2);
						break;
					case 60:
						ActBullet_SwordSwing(&gBul[i], 3);
						break;
					case 74:
						ActBullet_SwordSwing(&gBul[i], 4);
						break;
					
					case 61:
						ActBullet_Droplet(&gBul[i]);
						break;

					case 62:
						ActBullet_Whirlpool(&gBul[i]);
						break;
					
					case 64:
						ActBullet_MiningLaser(&gBul[i], 1);
						break;
					case 65:
						ActBullet_MiningLaser(&gBul[i], 2);
						break;
					case 66:
						ActBullet_MiningLaser(&gBul[i], 3);
						break;
					case 67:
						ActBullet_Spark(&gBul[i]);
						break;

					case 78:
						ActBullet_RedBurst(&gBul[i]);
						break;

					case 79:
						ActBullet_Edge2(&gBul[i]);
						break;

					case 80:
						ActBullet_SwordSwing2(&gBul[i], 1);
						break;
					case 81:
						ActBullet_SwordSwing2(&gBul[i], 2);
						break;
					case 82:
						ActBullet_SwordSwing2(&gBul[i], 3);
						break;
					case 83:
						ActBullet_FireMine(&gBul[i]);
						break;
					case 84:
						ActBullet_Flamings(&gBul[i]);
						break;
					case 85:
						ActBullet_SwordSwing2(&gBul[i], 4);
						break;

					case 86:
						ActBullet_Nanites(&gBul[i]);
						break;

					case 87:
						ActBullet_FrontieBust1(&gBul[i], 1);
						break;

					case 88:
						ActBullet_HomingFrontieBust(&gBul[i]);
						break;

					case 89:
						ActBullet_FrontieBust2(&gBul[i]);
						break;

					case 90:
						ActBullet_FrontieBust1(&gBul[i], 3);
						break;

					case 91:
						ActBullet_HomingFrontieBust2(&gBul[i]);
						break;
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
				case 28:
				case 29:
				case 30:
				case 31:
				case 32:
				case 33:
					return true;
			}
		}
	}

	return false;
}
