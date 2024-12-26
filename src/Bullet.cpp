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
	{3, 3, 22, 32, 4, 4, 3, 3, {8, 8, 8, 8}, 0.65, 0.65},
	{4, 2, 22, 32, 2, 2, 2, 2, {8, 8, 8, 8}, 1, 1},
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
	{8, 1, 17, 32, 4, 4, 3, 3, {8, 8, 24, 8}, 0.75, 0.75},
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
	{12, 100, 25, 105, 10, 10, 6, 6, {4, 4, 4, 4}, 0.33, 0.33},
	{15, 100, 30, 105, 11, 11, 6, 6, {4, 4, 4, 4}, 0.33, 0.33},
	{10, 100, 35, 105, 12, 12, 6, 6, {4, 4, 4, 4}, 0.33, 0.33},
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
	{4, 100, 30, 105, 12, 12, 4, 4, {4, 4, 4, 4}, 0.33, 0.33},
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
	{12, 100, 25, 105, 10, 10, 6, 6, {4, 4, 4, 4}, 0.33, 0.33},
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

void ActBullet_Hook(BULLET *bul, int level)
{
	if (bul->code_bullet == 95)
		bul->local_iframes = 4;
	else 
		bul->iframes = 15;

	if (level == 4) bul->iframes /= 4;

	if (GetNPCFlag(Flags::BRACKET_CHARACTER) || GetNPCFlag(Flags::MAHIN_CHARACTER))
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
		if (bul->code_bullet != 95)
			bul->damage /= 4;
	}
	// if (bul->flag & 0xF){
	// 	bul->act_no = 3;
	// 	chainWrap = true;
	// }

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		int deg = GetArktan(bul->x - bul->tgt_x, bul->y - bul->tgt_y);
		if (!GetNPCFlag(Flags::BRACKET_CHARACTER))
			deg += Random(-1, 1);
		int speed = 7;
		if (level == 3){
			speed = 5.75;
			bul->life_count *= 2;
		}
		if (level == 4){
			speed = 8;
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
			if (bul->code_bullet == 95)
				bul->local_iframes = 2;
			else
				bul->iframes = 6;

			if (level == 4) bul->iframes /= 3;
			
			if ((!(gKey & gKeyShot && !((level == 4 || bul->code_bullet == 95))) || (GetNPCFlag(Flags::BRACKET_CHARACTER) || GetNPCFlag(Flags::MAHIN_CHARACTER)))  && (bul->x > gMC.x - 0x200*8 && bul->x < gMC.x + 0x200*8 &&
					 bul->y > gMC.y - 0x200*8 && bul->y < gMC.y + 0x200*8)){
				bul->cond = 0;
				chainWrap = false;
				return;
			}
			int speed = 12;
			if (level == 3){
				speed = 8;
			}
			if (level == 4){
				speed = 16;
			}
			int deg = GetArktan(bul->x - gMC.x, bul->y - gMC.y);
			deg += Random(-1, 1);
			if (gKey & gKeyShot && !(level == 4 || bul->code_bullet == 95) && !(GetNPCFlag(Flags::BRACKET_CHARACTER) || GetNPCFlag(Flags::MAHIN_CHARACTER)) && Distance(gMC.x, gMC.y, bul->x, bul->y) < 8 * 16_pix){
				// if (gMC.x > bul->x)
				// 	bul->xm = 0x50;
				// else
				// 	bul->xm = -0x50;
				bul->xm = gMC.xm / 2;

				bul->ym += 0x100;
				if (bul->ym > 0x6FF)
					bul->ym = 0x6FF;

				if (bul->flag & 8)
					bul->ym = -0x100;

				if (bul->flag & 1)
					bul->xm = 0x200;
				else if (bul->flag & 4)
					bul->xm = -0x200;
			} else {
				bul->ym = GetSin(deg) * speed;
				bul->xm = GetCos(deg) * speed;
			}
		}
		if (bul->act_no != 3){
			if (bul->life != 100 && (level == 4))
			{
				bul->life = 100;
				int x, y;
				x = bul->x + Random(-24_pix, 24_pix);
				y = bul->y + Random(-24_pix, 24_pix);
				BULLET *bully = SetBullet(95, x, y);
				if (bully != NULL)
				{
					bully->tgt_x = bul->x;
					bully->tgt_y = bul->y;
					bully->damage = 4;
				}
			}
			bul->x += bul->xm;
			bul->y += bul->ym;
			if (bul->act_no == 2 && level == 2 && bul->count1 % 4 == 0 && gKey & gKeyShot && !(GetNPCFlag(Flags::BRACKET_CHARACTER) || GetNPCFlag(Flags::MAHIN_CHARACTER))){
				int x, y;
				do
				{
					x = Random(0, gMap.width*0x2000);
					y = Random(0, gMap.length*0x2000);
				} while (!(Distance(bul->x, bul->y, x, y) < 90_pix  && 
							Distance(bul->x, bul->y, x, y) > 8_pix));
				for (int i = 0; i < Random(1, 2); i++){
					BULLET *quiverProj = SetBullet(47, bul->x, bul->y);
					if (quiverProj != NULL){
						quiverProj->tgt_x = x;
						quiverProj->tgt_y = y;
						quiverProj->count1 = quiverProj->life_count/1.5f;
					}
				}
			}
			if (level == 3){
				if (bul->count1 % 8 == 0){
					SetBullet(23, bul->x, bul->y, 0);
				}
				else if (bul->count1 % 8 == 2){
					SetBullet(23, bul->x, bul->y, 2);
				}
			}
		}
	}

	// CARET* crt = SetCaret(bul->x, bul->y, 18);
	// crt->xm = bul->x;
	// crt->ym = bul->y;
	// crt->act_no = bul->count1 % 50;
		
	bul->rect = {130, 144, 142, 158};

	if (++bul->ani_wait > 2)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 0;

	Rect rect[4] = {
		{128, 176, 144, 192},
		{144, 176, 160, 192},
		{160, 176, 176, 192},
		{176, 176, 192, 192}
	};

	bul->rect = rect[bul->ani_no];
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
				} else
				switch (gBul[i].code_bullet)
				{
					// Nemesis
					case 34:	// Identical to case 43
						ActBullet_Nemesis(&gBul[i], 1);
						break;
					case 35:
						ActBullet_Nemesis(&gBul[i], 2);
						break;

					// Curly's Nemesis
					case 43:	// Identical to case 34
						ActBullet_Nemesis(&gBul[i], 1);
						break;

					case 49: case 95:
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
