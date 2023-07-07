#include "Shoot.h"

#include "cmath"

#include "CommonDefines.h"

#include "ArmsItem.h"
#include "BossLife.h"
#include "Bullet.h"
#include "Caret.h"
#include "Flags.h"
#include "Frame.h"
#include "Game.h"
#include "Generic.h"
#include "KeyControl.h"
#include "Map.h"
#include "MyChar.h"
#include "MycParam.h"
#include "NpChar.h"
#include "ResourceCode.h"
#include "Sound.h"
#include "Stage.h"
#include "Triangle.h"

static int empty;

int GravityDirect(int i){
	if (i == 1){
		return (gMC.flip ? 3 : 1);
	}
	else {
		return (gMC.flip ? 1 : 3);
	}
}

void ShootBullet_Frontia1(int level)
{
	int bul_no;

	switch (level)
	{
		case 1:
			bul_no = 1;
			break;

		case 2:
			bul_no = 2;
			break;

		case 3:
			bul_no = 3;
			break;
		case 4:
			bul_no = 70;
			break;
	}
	if (gMC.equip & EQUIP_PROPANE_TANK || GetNPCFlag(871)){
	if (!(gKey & gKeyShot))
		gMC.rensha = 10;
	}

	if (CountArmsBullet(1) > 3 * ((gMC.equip & EQUIP_PROPANE_TANK || GetNPCFlag(871)) ? 3 : 1))
		return;
	if (CountBulletNum(70) > 3 * ((gMC.equip & EQUIP_PROPANE_TANK || GetNPCFlag(871)) ? 3 : 1))
		return;

	if (((gMC.equip & EQUIP_PROPANE_TANK || GetNPCFlag(871)) && gKey & gKeyShot) || gKeyTrg & gKeyShot)
	{
		if (!UseArmsEnergy(1))
		{
			ChangeToFirstArms();
		}
		else
		{
			if (gMC.equip & EQUIP_PROPANE_TANK){
			if (++gMC.rensha < 11)
				return;

			gMC.rensha = 0;
			}

			if (gMC.up)
			{
				if (gMC.direct == 0)
				{
					if (gMC.equip & EQUIP_PROPANE_TANK || GetNPCFlag(871)){
						SetBullet(bul_no, gMC.x - (4 * 0x200), gMC.y - (10 * 0x200), GravityDirect(1));
						SetBullet(bul_no, gMC.x + (4 * 0x200), gMC.y - (10 * 0x200), GravityDirect(1));
					}
					else
						SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y - (10 * 0x200), GravityDirect(1));
					SetCaret(gMC.x - (3 * 0x200), gMC.y - (10 * 0x200), 3, 0);
				}
				else
				{
					if (gMC.equip & EQUIP_PROPANE_TANK || GetNPCFlag(871)){
						SetBullet(bul_no, gMC.x - (4 * 0x200), gMC.y - (10 * 0x200), GravityDirect(1));
						SetBullet(bul_no, gMC.x + (4 * 0x200), gMC.y - (10 * 0x200), GravityDirect(1));
					}
					else
						SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y - (10 * 0x200), GravityDirect(1));
					SetCaret(gMC.x + (3 * 0x200), gMC.y - (10 * 0x200), 3, 0);
				}
			}
			else if (gMC.down)
			{
				if (gMC.direct == 0)
				{
					if (gMC.equip & EQUIP_PROPANE_TANK || GetNPCFlag(871)){
						SetBullet(bul_no, gMC.x - (4 * 0x200), gMC.y + (10 * 0x200), GravityDirect(3));
						SetBullet(bul_no, gMC.x + (4 * 0x200), gMC.y + (10 * 0x200), GravityDirect(3));
					}
					else
						SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y + (10 * 0x200), GravityDirect(3));
					SetCaret(gMC.x - (3 * 0x200), gMC.y + (10 * 0x200), 3, 0);
				}
				else
				{
					if (gMC.equip & EQUIP_PROPANE_TANK || GetNPCFlag(871)){
						SetBullet(bul_no, gMC.x - (4 * 0x200), gMC.y + (10 * 0x200), GravityDirect(3));
						SetBullet(bul_no, gMC.x + (4 * 0x200), gMC.y + (10 * 0x200), GravityDirect(3));
					}
					else
						SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y + (10 * 0x200), GravityDirect(3));
					SetCaret(gMC.x + (3 * 0x200), gMC.y + (10 * 0x200), 3, 0);
				}
			}
			else
			{
				if (gMC.direct == 0)
				{
					if (gMC.equip & EQUIP_PROPANE_TANK || GetNPCFlag(871)){
						SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y - (4 * 0x200), 0);
						SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y + (4 * 0x200), 0);
					}
					else
						SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y + (2 * 0x200), 0);
					SetCaret(gMC.x - (12 * 0x200), gMC.y + (2 * 0x200), 3, 0);
				}
				else
				{
					if (gMC.equip & EQUIP_PROPANE_TANK || GetNPCFlag(871)){
						SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y - (4 * 0x200), 2);
						SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y + (4 * 0x200), 2);
					}
					else
						SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y + (2 * 0x200), 2);
					SetCaret(gMC.x + (12 * 0x200), gMC.y + (2 * 0x200), 3, 0);
				}
			}

			PlaySoundObject(33, SOUND_MODE_PLAY);
		}
	}
}

void PlaceWater(int x, int y){
	int i;

	FluidFallS(x, y, 0, 30);
	for (int t = 0; t < gMap.width * gMap.length; t++){
            	if (GetTile(XCoord(t), YCoord(t)-1) == 30 && GetTile(XCoord(t), YCoord(t)) == 13){
					EditMapS(t, 30);
				}
        	}
	for (int t = 0; t < gMap.width * gMap.length; t++){
            	if (IsEmpty(XCoord(t), YCoord(t)-1) && GetTile(XCoord(t), YCoord(t)) == 30){
					EditMapS(t, 13);
				}
    }

	for (i = 0; i < 3; ++i)
		SetNpChar(4, x * 0x200 * 0x10, y * 0x200 * 0x10, 0, 0, 0, NULL, 0);
	int a = 0;
	while (a < ARMS_MAX && gArmsData[a].code != 12)
		++a;
}

void PlaceBlock(int x, int y){
	int i;

	ChangeMapS(x, y, 1);

	for (i = 0; i < 3; ++i)
		SetNpChar(4, x * 0x200 * 0x10, y * 0x200 * 0x10, 0, 0, 0, NULL, 0);
	int a = 0;
	while (a < ARMS_MAX && gArmsData[a].code != 12)
		++a;

	if (a != ARMS_MAX)
	{
		gArmsData[a].num += 1;
		if (gArmsData[a].num > gArmsData[a].max_num)
			gArmsData[a].num = gArmsData[a].max_num;
	}
}

bool IsEmptee(int x, int y){
	if (!InsideMap(x, y)) return false;
    return GetTile(x, y) == 0 || GetAttribute(x + y * gMap.length) == 0 || GetAttribute(x + y * gMap.length) == 66 || GetAttribute(x + y * gMap.length) == 30;
}

int rifleAutoCount = 0;

void ShootBullet_Rifle(int level)
{
	int bul_no;

	switch (level)
	{
		case 1:
			bul_no = 28;
			break;

		case 2:
			bul_no = 29;
			break;

		case 3:
			bul_no = 30;
			break;
		
		case 4:
			bul_no = 37;
			break;
	}

	if (CountArmsBullet(2) > 2 + (level == 4 ? 4 : 0))
		return;

	if ((level == 4 && gKey & gKeyShot) || (level < 4 && gKeyTrg & gKeyShot))
	{
		if (level == 4){
			if (++rifleAutoCount > 60)
				rifleAutoCount = 0;
			if (rifleAutoCount != 10 && rifleAutoCount != 30 && rifleAutoCount != 50)
				return;
		}

		if (!UseArmsEnergy(1))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);
		}
		else
		{
			if (gMC.up)
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y - (8 * 0x200), 1);
					SetCaret(gMC.x - (1 * 0x200), gMC.y - (8 * 0x200), 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y - (8 * 0x200), 1);
					SetCaret(gMC.x + (1 * 0x200), gMC.y - (8 * 0x200), 3, 0);
				}
			}
			else if (gMC.down)
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y + (8 * 0x200), 3);
					SetCaret(gMC.x - (1 * 0x200), gMC.y + (8 * 0x200), 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y + (8 * 0x200), 3);
					SetCaret(gMC.x + (1 * 0x200), gMC.y + (8 * 0x200), 3, 0);
				}
			}
			else
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y + (3 * 0x200), 0);
					SetCaret(gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y + (3 * 0x200), 2);
					SetCaret(gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
				}
			}

			if (level == 3)
				PlaySoundObject(49, SOUND_MODE_PLAY);
			else
				PlaySoundObject(32, SOUND_MODE_PLAY);
		}
	}
}


void ShootBullet_PoleStar(int level)
{
	int bul_no;
	static int wait;
	ZeroExpMyChar();

	switch (level)
	{
		case 1:
			bul_no = 4;
			break;

		case 2:
			bul_no = 5;
			break;

		case 3:
			bul_no = 6;
			break;
	}

	if (CountArmsBullet(2) > 1)
		return;

	if (gKeyTrg & gMouseClick && gMC.equip & EQUIP_FAR_PLACE)
	{
		if (gStageNo != gGeneratedStage || gBL.flag || !UseArmsEnergy(3))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);
			return;
		}
		else
		{
			int fx, fy, x, y;
			GetFramePosition(&fx, &fy);
			x = fx + ScreenToSubpixelCoord(gMouseX);
			y = fy + ScreenToSubpixelCoord(gMouseY);
			float dist = 5.5;
			if (GetNPCFlag(871)) dist = 7.5;
			if (Distance(gMC.x, gMC.y, x, y) > dist*0x2000)
			{
				ChargeArmsEnergy(3);
				PlaySoundObject(37, SOUND_MODE_PLAY);
				return;
			}
			else
			{
				PlaceBlock(round(((float)x) / 0x2000), round(((float)y) / 0x2000));
				if (gMC.up)
				{
					SetCaret(gMC.x + (1 * 0x200), gMC.y - (9 * 0x200), 3, 0);
				}
				else if (gMC.down)
				{
					SetCaret(gMC.x + (1 * 0x200), gMC.y + (9 * 0x200), 3, 0);
				}
				else
				{
					if (gMC.direct == 0)
					{
						SetCaret(gMC.x - (16 * 0x200), gMC.y - (5 * 0x200), 3, 0);
					}
					else
					{
						SetCaret(gMC.x + (16 * 0x200), gMC.y - (5 * 0x200), 3, 0);
					}
				}
			}
		}
	}

	if (gKeyTrg & gKeyShot)
	{
		if (gStageNo != gGeneratedStage || gBL.flag || !UseArmsEnergy(1))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);
		}
		else
		{
			if (gMC.up)
			{
				if (gMC.equip & EQUIP_FAR_PLACE){
					PlaceBlock(round(((float)gMC.x + gMC.xm) / 0x2000), round(((float)gMC.y + gMC.ym) / 0x2000) - 1 * (gMC.flip ? -1 : 1));
				} else
					PlaceBlock(round(((float)gMC.x) / 0x2000), round(((float)gMC.y) / 0x2000) - 1 * (gMC.flip ? -1 : 1));
				SetCaret(gMC.x + (1 * 0x200), gMC.y - (9 * 0x200), 3, 0);
			}
			else if (gMC.down)
			{
				if (gMC.equip & EQUIP_FAR_PLACE){
					PlaceBlock(round(((float)gMC.x + gMC.xm) / 0x2000), round(((float)gMC.y + gMC.ym) / 0x2000) + 1 * (gMC.flip ? -1 : 1));
				} else
					PlaceBlock(round(((float)gMC.x) / 0x2000), round(((float)gMC.y) / 0x2000) + 1 * (gMC.flip ? -1 : 1));
				SetCaret(gMC.x + (1 * 0x200), gMC.y + (9 * 0x200), 3, 0);
			}
			else
			{
				if (gMC.direct == 0)
				{
					if (gMC.equip & EQUIP_FAR_PLACE){
						PlaceBlock(round(((float)gMC.x + gMC.xm) / 0x2000) - 1, round(((float)gMC.y + gMC.ym) / 0x2000));
					} else
						PlaceBlock(round(((float)gMC.x) / 0x2000) - 1, round(((float)gMC.y) / 0x2000));
					SetCaret(gMC.x - (16 * 0x200), gMC.y - (5 * 0x200), 3, 0);
				}
				else
				{
					if (gMC.equip & EQUIP_FAR_PLACE){
						PlaceBlock(round(((float)gMC.x + gMC.xm) / 0x2000) + 1, round(((float)gMC.y + gMC.ym) / 0x2000));
					} else
						PlaceBlock(round(((float)gMC.x) / 0x2000) + 1, round(((float)gMC.y) / 0x2000));
					SetCaret(gMC.x + (16 * 0x200), gMC.y - (5 * 0x200), 3, 0);
				}
			}

			if (level == 3)
				PlaySoundObject(49, SOUND_MODE_PLAY);
			else
				PlaySoundObject(32, SOUND_MODE_PLAY);
		}
	}
	else
	{
		++wait;

		if (gMC.equip & EQUIP_TURBOCHARGE)
		{
			if (wait > 120)
			{
				wait = 0;
				ChargeArmsEnergy(1);
			}
		}
	}
}

void ShootBullet_Shotgun(int level)
{
	static int wait;
	int deg, xm, ym, tgt_x, tgt_y, direct;
	int bul_no = 4;

	if (CountBulletNum(bul_no) > 0)
		return;

	if (gKeyTrg & gKeyShot)
	{
		if (!UseArmsEnergy(1))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);
		}
		else
		{
			if (gMC.up)
			{
				if (gMC.direct == 0)
				{
					tgt_x = gMC.x;
					tgt_y = gMC.y - 0x200 * 10;
					direct = 1;
					SetCaret(gMC.x - (1 * 0x200), gMC.y - (8 * 0x200), 3, 0);
				}
				else
				{
					tgt_x = gMC.x;
					tgt_y = gMC.y - 0x200 * 10;
					direct = 1;
					SetCaret(gMC.x + (1 * 0x200), gMC.y - (8 * 0x200), 3, 0);
				}
			}
			else if (gMC.down)
			{
				if (gMC.direct == 0)
				{
					tgt_x = gMC.x;
					tgt_y = gMC.y + 0x200 * 10;
					direct = 3;
					SetCaret(gMC.x - (1 * 0x200), gMC.y + (8 * 0x200), 3, 0);
				}
				else
				{
					tgt_x = gMC.x;
					tgt_y = gMC.y + 0x200 * 10;
					direct = 3;
					SetCaret(gMC.x + (1 * 0x200), gMC.y + (8 * 0x200), 3, 0);
				}
			}
			else
			{
				if (gMC.direct == 0)
				{
					tgt_x = gMC.x - 0x200 * 16;
					tgt_y = gMC.y + 0x200 * 3;
					direct = 0;
					SetCaret(gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
				}
				else
				{
					tgt_x = gMC.x + 0x200 * 16;
					tgt_y = gMC.y + 0x200 * 3;
					direct = 2;
					SetCaret(gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
				}
			}

			int shots, spread;
			switch (level)
			{
				case 1:
					shots = Random(6, 14);
					spread = 15;
					break;
				case 2:
					shots = Random(8, 20);
					spread = 20;
					break;
				case 3:
					shots = Random(13, 26);
					spread = 26;
					break;
				case 4:
					shots = Random(20, 42);
					spread = 18;
					break;
			}
			for (int i = 0; i < shots; i++)
			{
				deg = GetArktan(gMC.x - tgt_x, gMC.y - tgt_y);
				deg += (unsigned char)Random(-spread, spread);
				BULLET *bully = SetBullet(bul_no, gMC.x, gMC.y);
				if (bully != NULL)
				{
					bully->ym = GetSin(deg) * (6+level*2);
					bully->xm = GetCos(deg) * (6+level*2);
					bully->direct = direct;
					bully->life_count = 12 + level*7;
					bully->life = 1;
					bully->damage = 2;
				}
			}
			
			PlaySoundObject(49, SOUND_MODE_PLAY);
		}
	}
	else
	{
		++wait;

		if (wait > 60)
		{
			wait = 0;
			ChargeArmsEnergy(1);
		}
	}
}


void ShootBullet_WaterStar(int level)
{
	int bul_no;
	ZeroExpMyChar();

	switch (level)
	{
		case 1:
			bul_no = 4;
			break;

		case 2:
			bul_no = 5;
			break;

		case 3:
			bul_no = 6;
			break;
	}

	if (CountArmsBullet(2) > 1)
		return;

	if (gKeyTrg & gKeyShot)
	{
		if (!UseArmsEnergy(1))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);
		}
		else
		{
			if ((gKey & gKeyUp && gMC.equip & EQUIP_FAR_PLACE) || gMC.up)
			{
				SetCaret(gMC.x + (1 * 0x200), gMC.y - (9 * 0x200), 3, 0);
			}
			else if ((gKey & gKeyDown && gMC.equip & EQUIP_FAR_PLACE) || gMC.down)
			{
				SetCaret(gMC.x + (1 * 0x200), gMC.y + (9 * 0x200), 3, 0);
			}
			else
			{
				if (gMC.direct == 0)
				{
					SetCaret(gMC.x - (16 * 0x200), gMC.y - (5 * 0x200), 3, 0);
				}
				else
				{
					SetCaret(gMC.x + (16 * 0x200), gMC.y - (5 * 0x200), 3, 0);
				}
			}
			SetNpChar(299, gMC.x, gMC.y, 0, 0, 0, NULL, 0x100);
			PlaySoundObject(32, SOUND_MODE_PLAY);
		}
	}
}

void ShootBullet_FireBall(int level)
{
	int bul_no;
	int limit;

	switch (level)
	{
		case 1:
			limit = 2 + (gMC.equip & EQUIP_OVERCHARGE ? 1 : 0);
			if (GetNPCFlag(4405)) limit *= 1.5;
			if (CountArmsBullet(3) > limit)
				return;

			bul_no = 7;
			break;

		case 2:
			limit = 3 + (gMC.equip & EQUIP_OVERCHARGE ? 1 : 0);
			if (GetNPCFlag(4405)) limit *= 1.5;
			if (CountArmsBullet(3) > limit)
				return;

			bul_no = 8;
			break;

		case 3:
			limit = 5 + (gMC.equip & EQUIP_OVERCHARGE ? 1 : 0);
			if (GetNPCFlag(4405)) limit *= 1.5;
			if (CountArmsBullet(3) > limit)
				return;

			bul_no = 9;
			break;
		
		case 4:
			limit = 2 + (gMC.equip & EQUIP_OVERCHARGE ? 1 : 0);
			if (GetNPCFlag(4405)) limit *= 1.5;
			if (CountBulletNum(68) > limit)
				return;

			bul_no = 68;
			break;
	}

	if (gKeyTrg & gKeyShot)
	{
		if (!UseArmsEnergy(1))
		{
			ChangeToFirstArms();
		}
		else
		{
			if (gMC.up)
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (4 * 0x200), gMC.y - (8 * 0x200), (gMC.flip ? 3 : 1));
					SetCaret(gMC.x - (4 * 0x200), gMC.y - (8 * 0x200), 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (4 * 0x200), gMC.y - (8 * 0x200), (gMC.flip ? 3 : 1));
					SetCaret(gMC.x + (4 * 0x200), gMC.y - (8 * 0x200), 3, 0);
				}
			}
			else if (gMC.down)
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (4 * 0x200), gMC.y + (8 * 0x200), (gMC.flip ? 1 : 3));
					SetCaret(gMC.x - (4 * 0x200), gMC.y + (8 * 0x200), 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (4 * 0x200), gMC.y + (8 * 0x200), (gMC.flip ? 1 : 3));
					SetCaret(gMC.x + (4 * 0x200), gMC.y + (8 * 0x200), 3, 0);
				}
			}
			else
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y + (2 * 0x200), 0);
					SetCaret(gMC.x - (12 * 0x200), gMC.y + (2 * 0x200), 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y + (2 * 0x200), 2);
					SetCaret(gMC.x + (12 * 0x200), gMC.y + (2 * 0x200), 3, 0);
				}
			}

			PlaySoundObject(34, SOUND_MODE_PLAY);
		}
	}
}

void ShootBullet_Hook(int level)
{
	int bul_no;

	switch (level)
	{
		case 1:
			bul_no = 49;
			break;

		case 2:
			bul_no = 50;
			break;

		case 3:
			bul_no = 51;
			break;
		case 4:
			bul_no = 73;
			break;
	}

	if (level == 4){
		if ((GetNPCFlag(4450) && CountBulletNum(73) > 1))
			return;
		else if (!GetNPCFlag(4450) && CountBulletNum(73) > 0)
			return;
	} else {
		if ((GetNPCFlag(4450) && CountBulletNum(48 + level) > 1))
			return;
		else if (!GetNPCFlag(4450) && CountBulletNum(48 + level) > 0)
			return;
	}

	if ((!(gMouseHold) && !(gKey & gKeyShot)))
		gMC.rensha = 0;

	if ((GetNPCFlag(4450) && gKeyTrg & gKeyShot) || (!GetNPCFlag(4450) && gKey & gKeyShot))
	{
		if (!GetNPCFlag(4450))
			if (++gMC.rensha < 7)
				return;

		gMC.rensha = 0;

		int fx, fy, x, y;
		BULLET* bul;
		if (gMC.up)
		{
			if (gMC.direct == 0)
			{
				bul = SetBullet(bul_no, gMC.x - (4 * 0x200), gMC.y - (8 * 0x200));
			}
			else
			{
				bul = SetBullet(bul_no, gMC.x + (4 * 0x200), gMC.y - (8 * 0x200));
			}
			if (bul != NULL){
			bul->tgt_y = gMC.y - 0x2000 * 10;
			bul->tgt_x = gMC.x;
			}
		}
		else if (gMC.down)
		{
			if (gMC.direct == 0)
			{
				bul = SetBullet(bul_no, gMC.x - (4 * 0x200), gMC.y + (8 * 0x200));
			}
			else
			{
				bul = SetBullet(bul_no, gMC.x + (4 * 0x200), gMC.y + (8 * 0x200));
			}
			if (bul != NULL){
			bul->tgt_y = gMC.y + 0x2000 * 10;
			bul->tgt_x = gMC.x;
			}
		}
		else
		{
			if (gMC.direct == 0)
			{
				bul = SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y + (2 * 0x200));
				if (bul != NULL){
				bul->tgt_x = gMC.x - 0x2000 * 10;
				bul->tgt_y = gMC.y;
				}
			}
			else
			{
				bul = SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y + (2 * 0x200));
				if (bul != NULL){
				bul->tgt_x = gMC.x + 0x2000 * 10;
				bul->tgt_y = gMC.y;
				}
			}
		}

		PlaySoundObject(103, SOUND_MODE_PLAY);
		chainWrap = true;
	} else if ((GetNPCFlag(4450) && gKeyTrg & gMouseClick) || (!GetNPCFlag(4450) && gMouseHold))
	{
		if (!GetNPCFlag(4450))
			if (++gMC.rensha < 7)
				return;

		gMC.rensha = 0;

		int fx, fy, x, y;
		GetFramePosition(&fx, &fy);
		x = fx + ScreenToSubpixelCoord(gMouseX);
		y = fy + ScreenToSubpixelCoord(gMouseY);
		BULLET* bul;
		if (gMC.up)
		{
			if (gMC.direct == 0)
			{
				bul = SetBullet(bul_no, gMC.x - (4 * 0x200), gMC.y - (8 * 0x200));
			}
			else
			{
				bul = SetBullet(bul_no, gMC.x + (4 * 0x200), gMC.y - (8 * 0x200));
			}
		}
		else if (gMC.down)
		{
			if (gMC.direct == 0)
			{
				bul = SetBullet(bul_no, gMC.x - (4 * 0x200), gMC.y + (8 * 0x200));
			}
			else
			{
				bul = SetBullet(bul_no, gMC.x + (4 * 0x200), gMC.y + (8 * 0x200));
			}
		}
		else
		{
			if (gMC.direct == 0)
			{
				bul = SetBullet(bul_no, gMC.x - (2 * 0x200), gMC.y + (2 * 0x200));
			}
			else
			{
				bul = SetBullet(bul_no, gMC.x + (2 * 0x200), gMC.y + (2 * 0x200));
			}
		}
		if (bul != NULL){
		bul->tgt_x = x;
		bul->tgt_y = y;
		}

		PlaySoundObject(103, SOUND_MODE_PLAY);
		chainWrap = true;
	}
}

void ShootBullet_Machinegun1(int level)
{
	int bul_no;
	static int wait;

	if (CountArmsBullet(4) > (gMC.equip & EQUIP_OVERCHARGE ? 16 : 8))
		return;

	switch (level)
	{
		case 1:
			bul_no = 10;
			break;

		case 2:
			bul_no = 11;
			break;

		case 3:
			bul_no = 12;
			break;

		case 4:
			bul_no = 69;
			break;
	}

	if (!(gKey & gKeyShot))
		gMC.rensha = gMC.equip & EQUIP_OVERCHARGE ? 3 : 4;

	if (gKey & gKeyShot)
	{
		if (++gMC.rensha < (gMC.equip & EQUIP_OVERCHARGE ? 3 : 4))
			return;

		gMC.rensha = 0;

		if (!UseArmsEnergy(1))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);

			if (empty == 0)
			{
				SetCaret(gMC.x, gMC.y, 16, 0);
				empty = 50;
			}

			return;
		}

		int i = 1;
		if (Random(0, 2) == 0 && gMC.equip & EQUIP_OVERCHARGE){
			i = 2;
		}

		if (gMC.up)
		{
			if (level >= 3 || (GetNPCFlag(4200))){
				if (GetNPCFlag(4200)){
					switch (level){
						case 1:
							gMC.ym += 0x60; break;
						case 2:
							gMC.ym += 0x90; break;
						case 3:
							gMC.ym += 0x135; break;
					}
				}
				else
					gMC.ym += 0x100;
			}

			if (gMC.direct == 0)
			{
				SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				SetCaret(gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), 3, 0);
			}
			else
			{
				SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				SetCaret(gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), 3, 0);
			}
		}
		else if (gMC.down)
		{
			if (level >= 3 || (GetNPCFlag(4200)))
			{
				float mod = 1;

				if (GetNPCFlag(4200)){
					switch (level){
						case 1:
							mod = 0.4; break;
						case 2:
							mod = 0.6; break;
						case 3:
							mod = 1.2; break;
					}
				}

				if (gMC.ym > 0)
					gMC.ym /= 2 * mod;

				if (gMC.ym > -0x400 * (gMC.flip ? -1 : 1) * mod)
				{
					gMC.ym -= 0x200 * (gMC.flip ? -1 : 1) * mod;
					if (gMC.ym < -0x400 * (gMC.flip ? -1 : 1) * mod)
						gMC.ym = -0x400 * (gMC.flip ? -1 : 1) * mod;
				}
			}

			if (gMC.direct == 0)
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				SetCaret(gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), 3, 0);
			}
			else
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				SetCaret(gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), 3, 0);
			}
		}
		else
		{
			if (gMC.direct == 0)
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), 0);
				SetCaret(gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
			}
			else
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), 2);
				SetCaret(gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
			}
		}

		if (level >= 3)
			PlaySoundObject(49, SOUND_MODE_PLAY);
		else
			PlaySoundObject(32, SOUND_MODE_PLAY);
	} else
	{
		++wait;

		if (gMC.equip & EQUIP_TURBOCHARGE)
		{
			if (wait > 1)
			{
				wait = 0;
				ChargeArmsEnergy(1);
			}
		}
		else
		{
			if (wait > 3)
			{
				wait = 0;
				ChargeArmsEnergy(1);
			}
		}
	}
}

void ShootBullet_Machinegun2(int level)
{
	int bul_no = 10;
	static int wait;

	if (!(gKey & gKeyShot))
		gMC.rensha = gMC.equip & EQUIP_OVERCHARGE ? 1 : 2;

	if (gKey & gKeyShot)
	{
		if (++gMC.rensha < (gMC.equip & EQUIP_OVERCHARGE ? 1 : 2))
			return;

		gMC.rensha = 0;

		if (!UseArmsEnergy(level))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);

			if (empty == 0)
			{
				SetCaret(gMC.x, gMC.y, 16, 0);
				empty = 20;
			}

			return;
		}

		int i = 1;
		if (Random(0, 2) == 0 && gMC.equip & EQUIP_OVERCHARGE){
			i = 2;
		}

		if (gMC.up)
		{
			if (level >= 3 || (GetNPCFlag(4200))){
				if (GetNPCFlag(4200)){
					switch (level){
						case 1:
							gMC.ym += 0x60; break;
						case 2:
							gMC.ym += 0x90; break;
						case 3:
							gMC.ym += 0x135; break;
					}
				}
				else
					gMC.ym += 0x100 / 3;
			}

			if (gMC.direct == 0)
			{
				SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				SetCaret(gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), 3, 0);
			}
			else
			{
				SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				SetCaret(gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), 3, 0);
			}
		}
		else if (gMC.down)
		{
			if (level >= 3 || (GetNPCFlag(4200)))
			{
				float mod = 1;

				if (GetNPCFlag(4200)){
					switch (level){
						case 1:
							mod = 0.4; break;
						case 2:
							mod = 0.6; break;
						case 3:
							mod = 1.2; break;
					}
				}

				if (gMC.ym > 0)
					gMC.ym /= 2 * mod;

				if (gMC.ym > -0x400 * (gMC.flip ? -1 : 1) * mod)
				{
					gMC.ym -= 0x200 * (gMC.flip ? -1 : 1) * mod;
					if (gMC.ym < -0x400 * (gMC.flip ? -1 : 1) * mod)
						gMC.ym = -0x400 * (gMC.flip ? -1 : 1) * mod;
				}
			}

			if (gMC.direct == 0)
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				SetCaret(gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), 3, 0);
			}
			else
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				SetCaret(gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), 3, 0);
			}
		}
		else
		{
			if (gMC.direct == 0)
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), 0);
				SetCaret(gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
			}
			else
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), 2);
				SetCaret(gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
			}
		}

			PlaySoundObject(32, SOUND_MODE_PLAY);
	} else
	{
		++wait;

		if (gMC.equip & EQUIP_TURBOCHARGE)
		{
			ChargeArmsEnergy(1);
		}
		else
		{
			if (wait > 1)
			{
				wait = 0;
				ChargeArmsEnergy(1);
			}
		}
	}
}

bool robotBoost = false;

void ShootBullet_RobotGun(int level)
{
	int bul_no = 67;
	static int wait;
	if (gKey & gKeyJump && gMC.boost_sw == 1){
		if (!UseArmsEnergy(1))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);
			robotBoost = false;

			if (empty == 0)
			{
				SetCaret(gMC.x, gMC.y, 16, 0);
				empty = 50;
			}

			return;
		}
		if (gMC.ym > 0)
			gMC.ym = 0;
		// Booster particles
		if (gArmsData[gSelectedArms].num % 2 == 0)
		{
			SetBullet(67, gMC.x, gMC.y + (8 * 0x200), GravityDirect(3));
			if (level > 1 && Random(0, 25) == 0)
				SetBullet(3, gMC.x, gMC.y + (8 * 0x200), GravityDirect(3));
			if (level > 1 && Random(0, 2) == 0)
				SetBullet(67, gMC.x, gMC.y + (8 * 0x200), GravityDirect(3));
			if (level > 2)
				SetBullet(67, gMC.x, gMC.y + (8 * 0x200), GravityDirect(3));
			if (level > 2 && Random(0, 4) == 0)
				SetBullet(22, gMC.x, gMC.y + (8 * 0x200), GravityDirect(3));
			PlaySoundObject(113, SOUND_MODE_PLAY);
		}

		if (gMC.ym > -0x600){
			// Upwards force
			gMC.ym -= 0x30 + level*0x30;
		}

		// Bounce off of ceiling
		if (gMC.flag & 2)
			gMC.ym = 0x200;
	}
	else if (gKey & gKeyShot)
	{
		if (!UseArmsEnergy(1))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);

			if (empty == 0)
			{
				SetCaret(gMC.x, gMC.y, 16, 0);
				empty = 50;
			}

			return;
		}

		if (gMC.up)
		{
			if (gMC.direct == 0)
			{
				SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				if (level > 1 && Random(0, 25) == 0)
					SetBullet(3, gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				if (level > 1 && Random(0, 2) == 0)
					SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				if (level > 2)
					SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				if (level > 2 && Random(0, 4) == 0)
					SetBullet(22, gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
			}
			else
			{
				SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				if (level > 1 && Random(0, 25) == 0)
					SetBullet(3, gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				if (level > 1 && Random(0, 2) == 0)
					SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				if (level > 2)
					SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				if (level > 2 && Random(0, 4) == 0)
					SetBullet(22, gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
			}
		}
		else if (gMC.down)
		{
			if (gMC.direct == 0)
			{
				SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				if (level > 1 && Random(0, 25) == 0)
					SetBullet(3, gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				if (level > 1 && Random(0, 2) == 0)
					SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				if (level > 2)
					SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				if (level > 2 && Random(0, 4) == 0)
					SetBullet(22, gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
			}
			else
			{
				SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				if (level > 1 && Random(0, 25) == 0)
					SetBullet(3, gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				if (level > 1 && Random(0, 2) == 0)
					SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				if (level > 2)
					SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				if (level > 2 && Random(0, 4) == 0)
					SetBullet(22, gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
			}
		}
		else
		{
			if (gMC.direct == 0)
			{
				SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y + (3 * 0x200), 0);
				if (level > 1 && Random(0, 25) == 0)
					SetBullet(3, gMC.x - (6 * 0x200), gMC.y + (3 * 0x200), 0);
				if (level > 1 && Random(0, 2) == 0)
					SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y + (3 * 0x200), 0);
				if (level > 2)
					SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y + (3 * 0x200), 0);
				if (level > 2 && Random(0, 4) == 0)
					SetBullet(22, gMC.x - (6 * 0x200), gMC.y + (3 * 0x200), 0);
			}
			else
			{
				SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y + (3 * 0x200), 2);
				if (level > 1 && Random(0, 25) == 0)
					SetBullet(3, gMC.x + (6 * 0x200), gMC.y + (3 * 0x200), 2);
				if (level > 1 && Random(0, 2) == 0)
					SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y + (3 * 0x200), 2);
				if (level > 2)
					SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y + (3 * 0x200), 2);
				if (level > 2 && Random(0, 4) == 0)
					SetBullet(22, gMC.x + (6 * 0x200), gMC.y + (3 * 0x200), 2);
			}
		}

		PlaySoundObject(32, SOUND_MODE_PLAY);
	} else
	{
		if (gMC.flag & 8)
		ChargeArmsEnergy(1);
	}
}

void ShootBullet_MiningLaser(int level)
{
	int bul_no;
	static int wait;
	if (gKeyTrg & gKeyAbility){
		++gArmsData[gSelectedArms].level;
		if (gArmsData[gSelectedArms].level > 6){
			gArmsData[gSelectedArms].level = 1;
			CutNPCFlag(6906);
			SetNPCFlag(6901);
		} else {
			CutNPCFlag(6900 + gArmsData[gSelectedArms].level - 1);
			SetNPCFlag(6900 + gArmsData[gSelectedArms].level);
		}
	}
	if (gKey & gKeyDown && gArmsData[gSelectedArms].exp > 0 
		&& gArmsData[gSelectedArms].num < gArmsData[gSelectedArms].max_num){
		char caret = Random(0, 1);
		SetCaret(gMC.x, gMC.y + 3_pix, caret ? 11 : 1, caret ? 0 : 1);
		PlaySoundObject(59, SOUND_MODE_PLAY);
		if (++wait > 5){
			wait = 0;
			gArmsData[gSelectedArms].exp--;
			if (gArmsData[gSelectedArms].num < gArmsData[gSelectedArms].max_num)
				PlaySoundObject(65, SOUND_MODE_PLAY);
			ChargeArmsEnergy(1);
		}
	} else {
		wait = 0;
	}
	if (level == 2 && CountBulletNum(65) > 0)
		return;
	if (level == 3 && CountBulletNum(64) > 0)
		return;

	bul_no = 64;
	int rensha, ammo;
	switch (level)
	{
		case 1: rensha = 7; ammo = 1; break;
		case 2: rensha = 20; ammo = 8; bul_no = 65; break;
		case 3: rensha = 30; ammo = 30; break;
		case 4: rensha = 11; ammo = 2; break;
		case 5: rensha = 3; ammo = 1; break;
		case 6: rensha = 12; ammo = 2; break;
	}
	if (!(gKey & gKeyShot))
		gMC.rensha = rensha;

	if (gKey & gKeyShot)
	{
		if (++gMC.rensha < rensha)
			return;

		gMC.rensha = 0;

		if (gArmsData[gSelectedArms].num < ammo || !UseArmsEnergy(ammo))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);

			if (empty == 0)
			{
				SetCaret(gMC.x, gMC.y, 16, 0);
				empty = 50;
			}

			return;
		}

		int i = 1;
		if (level == 4) i = 2;
		if (level != 3){
		if (gMC.up)
		{
			if (gMC.direct == 0)
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				if (level == 6){
					SetBullet(bul_no, gMC.x - (11 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				}
				SetCaret(gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), 3, 0);
			}
			else
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				if (level == 6){
					SetBullet(bul_no, gMC.x + (11 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				}
				SetCaret(gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), 3, 0);
			}
		}
		else if (gMC.down)
		{
			if (gMC.direct == 0)
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				if (level == 6){
					SetBullet(bul_no, gMC.x - (9 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				}
				SetCaret(gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), 3, 0);
			}
			else
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				if (level == 6){
					SetBullet(bul_no, gMC.x + (9 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				}
				SetCaret(gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), 3, 0);
			}
		}
		else
		{
			if (gMC.direct == 0)
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), 0);
				if (level == 6){
					SetBullet(bul_no, gMC.x - (12 * 0x200), gMC.y - (8 * 0x200), 0);
				}
				SetCaret(gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
			}
			else
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), 2);
				if (level == 6){
					SetBullet(bul_no, gMC.x + (12 * 0x200), gMC.y - (8 * 0x200), 2);
				}
				SetCaret(gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
			}
		}
		} else {
			int tgt_x, tgt_y, direct;
			if (gMC.up)
			{
				if (gMC.direct == 0)
				{
					tgt_x = gMC.x;
					tgt_y = gMC.y - 0x200 * 10;
					direct = 1;
					SetCaret(gMC.x - (1 * 0x200), gMC.y - (8 * 0x200), 3, 0);
				}
				else
				{
					tgt_x = gMC.x;
					tgt_y = gMC.y - 0x200 * 10;
					direct = 1;
					SetCaret(gMC.x + (1 * 0x200), gMC.y - (8 * 0x200), 3, 0);
				}
			}
			else if (gMC.down)
			{
				if (gMC.direct == 0)
				{
					tgt_x = gMC.x;
					tgt_y = gMC.y + 0x200 * 10;
					direct = 3;
					SetCaret(gMC.x - (1 * 0x200), gMC.y + (8 * 0x200), 3, 0);
				}
				else
				{
					tgt_x = gMC.x;
					tgt_y = gMC.y + 0x200 * 10;
					direct = 3;
					SetCaret(gMC.x + (1 * 0x200), gMC.y + (8 * 0x200), 3, 0);
				}
			}
			else
			{
				if (gMC.direct == 0)
				{
					tgt_x = gMC.x - 0x200 * 16;
					tgt_y = gMC.y + 0x200 * 3;
					direct = 0;
					SetCaret(gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
				}
				else
				{
					tgt_x = gMC.x + 0x200 * 16;
					tgt_y = gMC.y + 0x200 * 3;
					direct = 2;
					SetCaret(gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
				}
			}

			int shots, spread, deg, speed;
			shots = Random(25, 35);
			spread = 50;
			speed = 9;
			for (int i = 0; i < shots; i++)
			{
				deg = GetArktan(gMC.x - tgt_x, gMC.y - tgt_y);
				deg += (unsigned char)Random(-spread, spread);
				BULLET *bully = SetBullet(bul_no, gMC.x, gMC.y);
				if (bully != NULL)
				{
					bully->direct = direct;
					bully->xm = GetCos(deg) * speed;
					bully->ym = GetSin(deg) * speed;
					bully->count2 = 1;
				}
			}
		}

		if (level == 3)
			PlaySoundObject(64, SOUND_MODE_PLAY);
		else
			PlaySoundObject(62, SOUND_MODE_PLAY);
	}
}

unsigned char frontieRensha;

void FrontieBust3_OverheatFlamingShoot()
{
	int targetX = gMC.direct == 0 ? gMC.x - 12_pix : gMC.x + 12_pix;
	int targetY = gMC.y + 3_pix;
	if (gMC.up || gMC.down){
		targetX = gMC.direct == 0 ? gMC.x - 3_pix : gMC.x + 3_pix;
		targetY = gMC.down ? gMC.y + 8_pix : gMC.y - 8_pix;
	}
	BULLET *bully = SetBullet(84, targetX, targetY);
	if (bully != NULL)
	{
		PlaySoundObject(33, SOUND_MODE_PLAY);
		bully->direct = gMC.direct;
		if (gMC.direct == 0)
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
		bully->xm *= 0.75;
		bully->ym *= 0.75;
	}
}

void ShootBullet_FrontieBuster(int level)
{
	int bul_no;
	static int wait;
	int shootingCaret = 3;
	int shootingSound = 32;
	static bool lol;

	switch (level)
	{
		case 1:
			bul_no = 87;
			if (Random(0, 8) == 0)
				bul_no = 88;
			break;

		case 2:
			bul_no = 89;
			break;

		case 3:
			bul_no = 90;
			if (Random(0, 30) == 0)
				bul_no = 89;
			break;

		case 4:
			bul_no = 91;
			break;
	}

	if (!(gKey & gKeyShot) && lol){
		PlaySoundObject(107);
		PlaySoundObject(52);
		SetCaret(gMC.x, gMC.y, 12, 0);
		SetCaret(gMC.x-0x1000, gMC.y, 2, 2);
		SetCaret(gMC.x+0x1000, gMC.y, 2, 2);
		SetCaret(gMC.x, gMC.y+0x1000, 2, 2);
		SetCaret(gMC.x, gMC.y-0x1000, 2, 2);
		lol = false;
	}

	int cooldown;
	switch (level){
		case 1:
			cooldown = 9;
			break;
		case 2:
			cooldown = 40;
			break;
		case 3:
			cooldown = 3;
			break;
		case 4:
			cooldown = std::max(15, 40 - CountBulletNum(bul_no)*5);
			break;
	}

	if (gKey & gKeyShot)
	{
		if (++frontieRensha < cooldown){
			if (lol == false)
				PlaySoundObject(37, SOUND_MODE_PLAY);
			if (level == 3 && Random(0, 2) == 0){
				FrontieBust3_OverheatFlamingShoot();
			}
			return;
		}

		frontieRensha = 0;
		lol = true;

		if (!UseArmsEnergy(level == 3 ? Random(0, 2) : 1))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);

			if (empty == 0)
			{
				SetCaret(gMC.x, gMC.y, 16, 0);
				empty = 50;
			}

			return;
		}

		if (level == 1 || level == 3){
			if (level == 3)
				shootingSound = 49;
			if (Random(0, 4) == 0){
				shootingSound = 22;
				shootingCaret = 2;
			} else if (Random(0, 3) == 0){
				shootingSound = 49;
				shootingCaret = 12;
			}
			if (bul_no == 88){
				shootingSound = 117;
			}
			if (bul_no == 89){
				shootingSound = 35;
				shootingCaret = 12;
			}
		} else if (level == 2){
			shootingSound = 35;
			shootingCaret = 12;
		} else if (level == 4){
			shootingSound = 64;
			shootingCaret = 2;
		}

		int i = 1;

		if (gMC.up)
		{
			if (gMC.direct == 0)
			{
				SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				SetCaret(gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), shootingCaret, 0);
			}
			else
			{
				SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				SetCaret(gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), shootingCaret, 0);
			}
		}
		else if (gMC.down)
		{
			if (gMC.direct == 0)
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				SetCaret(gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), shootingCaret, 0);
			}
			else
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				SetCaret(gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), shootingCaret, 0);
			}
		}
		else
		{
			if (gMC.direct == 0)
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), 0);
				SetCaret(gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), shootingCaret, 0);
			}
			else
			{
				for (int l = 0; l < i; l++)
				SetBullet(bul_no, gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), 2);
				SetCaret(gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), shootingCaret, 0);
			}
		}

		PlaySoundObject(shootingSound, SOUND_MODE_PLAY);
	} else
	{
		++wait;
		if (wait > cooldown / 2)
		{
			wait = 0;
			ChargeArmsEnergy(1);
		}
	}
}

void ShootBullet_Missile(int level, bool bSuper)
{
	int bul_no;

	if (bSuper)
	{
		switch (level)
		{
			case 1:
				bul_no = 28;
				break;

			case 2:
				bul_no = 29;
				break;

			case 3:
				bul_no = 30;
				break;
		}

		switch (level)
		{
			case 1:
				if (CountArmsBullet(10) > 0)
					return;

				if (CountArmsBullet(11) > 0)
					return;

				break;

			case 2:
				if (CountArmsBullet(10) > 1)
					return;

				if (CountArmsBullet(11) > 1)
					return;

				break;

			case 3:
				if (CountArmsBullet(10) > 3)
					return;

				if (CountArmsBullet(11) > 3)
					return;

				break;
		}

	}
	else
	{
		ZeroExpMyChar();
		bul_no = 14;

		switch (level)
		{
			case 1:
				if (CountArmsBullet(5) > 0)
					return;

				if (CountArmsBullet(6) > 0)
					return;

				break;

			case 2:
				if (CountArmsBullet(5) > 1)
					return;

				if (CountArmsBullet(6) > 1)
					return;

				break;

			case 3:
				if (CountArmsBullet(5) > 3)
					return;

				if (CountArmsBullet(6) > 3)
					return;

				break;
		}
	}

	if (gKeyTrg & gKeyShot)
	{
		if (level < 3)
		{
			if (!UseArmsEnergy(1))
			{
				PlaySoundObject(37, SOUND_MODE_PLAY);

				if (empty == 0)
				{
					SetCaret(gMC.x, gMC.y, 16, 0);
					empty = 50;
				}

				return;
			}

			if (gMC.up)
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
					SetCaret(gMC.x - (1 * 0x200), gMC.y - (8 * 0x200), 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
					SetCaret(gMC.x + (1 * 0x200), gMC.y - (8 * 0x200), 3, 0);
				}
			}
			else if (gMC.down)
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
					SetCaret(gMC.x - (1 * 0x200), gMC.y + (8 * 0x200), 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
					SetCaret(gMC.x + (1 * 0x200), gMC.y + (8 * 0x200), 3, 0);
				}
			}
			else
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y, 0);
					SetCaret(gMC.x - (12 * 0x200), gMC.y, 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y, 2);
					SetCaret(gMC.x + (12 * 0x200), gMC.y, 3, 0);
				}
			}
		}
		else
		{
			if (!UseArmsEnergy(1))
			{
				PlaySoundObject(37, SOUND_MODE_PLAY);

				if (empty == 0)
				{
					SetCaret(gMC.x, gMC.y, 16, 0);
					empty = 50;
				}

				return;
			}

			if (gMC.up)
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y - (8 * 0x200), 1);
					SetCaret(gMC.x - (1 * 0x200), gMC.y - (8 * 0x200), 3, 0);
					SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y, 1);
					SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y, 1);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y - (8 * 0x200), 1);
					SetCaret(gMC.x + (1 * 0x200), gMC.y - (8 * 0x200), 3, 0);
					SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y, 1);
					SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y, 1);
				}
			}
			else if (gMC.down)
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y + (8 * 0x200), 3);
					SetCaret(gMC.x - (1 * 0x200), gMC.y + (8 * 0x200), 3, 0);
					SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y, 3);
					SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y, 3);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y + (8 * 0x200), 3);
					SetCaret(gMC.x + (1 * 0x200), gMC.y + (8 * 0x200), 3, 0);
					SetBullet(bul_no, gMC.x - (3 * 0x200), gMC.y, 3);
					SetBullet(bul_no, gMC.x + (3 * 0x200), gMC.y, 3);
				}
			}
			else
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y + (1 * 0x200), 0);
					SetCaret(gMC.x - (12 * 0x200), gMC.y + (1 * 0x200), 3, 0);
					SetBullet(bul_no, gMC.x, gMC.y - (8 * 0x200), 0);
					SetBullet(bul_no, gMC.x + (4 * 0x200), gMC.y - (1 * 0x200), 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y + (1 * 0x200), 2);
					SetCaret(gMC.x + (12 * 0x200), gMC.y + (1 * 0x200), 3, 0);
					SetBullet(bul_no, gMC.x, gMC.y - (8 * 0x200), 2);
					SetBullet(bul_no, gMC.x - (4 * 0x200), gMC.y - (1 * 0x200), 2);
				}
			}
		}

		PlaySoundObject(32, SOUND_MODE_PLAY);
	}
}

void ShootBullet_Bubblin1(void)
{
	static int wait;

	if (gKeyTrg & gKeyShot)
	{
		if (!UseArmsEnergy(1))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);

			if (empty == 0)
			{
				SetCaret(gMC.x, gMC.y, 16, 0);
				empty = 50;
			}

			return;
		}

		if (gMC.up)
		{
			if (gMC.direct == 0)
			{
				SetBullet(19, gMC.x - (1 * 0x200), gMC.y - (2 * 0x200), GravityDirect(1));
				SetCaret(gMC.x - (1 * 0x200), gMC.y - (2 * 0x200), 3, 0);
			}
			else
			{
				SetBullet(19, gMC.x + (1 * 0x200), gMC.y - (2 * 0x200), GravityDirect(1));
				SetCaret(gMC.x + (1 * 0x200), gMC.y - (2 * 0x200), 3, 0);
			}
		}
		else if (gMC.down)
		{
			if (gMC.direct == 0)
			{
				SetBullet(19, gMC.x - (1 * 0x200), gMC.y + (2 * 0x200), GravityDirect(3));
				SetCaret(gMC.x - (1 * 0x200), gMC.y + (2 * 0x200), 3, 0);
			}
			else
			{
				SetBullet(19, gMC.x + (1 * 0x200), gMC.y + (2 * 0x200), GravityDirect(3));
				SetCaret(gMC.x + (1 * 0x200), gMC.y + (2 * 0x200), 3, 0);
			}
		}
		else
		{
			if (gMC.direct == 0)
			{
				SetBullet(19, gMC.x - (6 * 0x200), gMC.y + (3 * 0x200), 0);
				SetCaret(gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
			}
			else
			{
				SetBullet(19, gMC.x + (6 * 0x200), gMC.y + (3 * 0x200), 2);
				SetCaret(gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
			}
		}

		PlaySoundObject(48, SOUND_MODE_PLAY);
	}
	else if (++wait > 20)
	{
		wait = 0;
		ChargeArmsEnergy(1);
	}
}

void ShootBullet_Bubblin2(int level)
{
	static int wait;

	if (CountArmsBullet(7) > (gMC.equip & EQUIP_OVERCHARGE ? 60 : 30))
		return;
	
	if (level != 4)
		level += 18;
	else
		level = 71;

	if (!(gKey & gKeyShot))
		gMC.rensha = (gMC.equip & EQUIP_OVERCHARGE ? 2 : 3);

	if (gKey & gKeyShot)
	{
		if (++gMC.rensha < (gMC.equip & EQUIP_OVERCHARGE ? 2 : 3))
			return;

		gMC.rensha = 0;

		if (!UseArmsEnergy((level == 71 && Random(0, 7) == 0 ? 0 : 1)))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);

			if (empty == 0)
			{
				SetCaret(gMC.x, gMC.y, 16, 0);
				empty = 50;
			}

			return;
		}

		int i = 1;
		if (Random(0, 2) == 0 && gMC.equip & EQUIP_OVERCHARGE){
			i = 2;
		}

		if (gMC.up)
		{
			if (gMC.direct == 0)
			{
				for (int l = 0; l < i; l++)
				SetBullet(level, gMC.x - (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				SetCaret(gMC.x - (3 * 0x200), gMC.y - (16 * 0x200), 3, 0);
			}
			else
			{
				for (int l = 0; l < i; l++)
				SetBullet(level, gMC.x + (3 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
				SetCaret(gMC.x + (3 * 0x200), gMC.y - (16 * 0x200), 3, 0);
			}
		}
		else if (gMC.down)
		{
			if (gMC.direct == 0)
			{
				for (int l = 0; l < i; l++)
				SetBullet(level, gMC.x - (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				SetCaret(gMC.x - (3 * 0x200), gMC.y + (16 * 0x200), 3, 0);
			}
			else
			{
				for (int l = 0; l < i; l++)
				SetBullet(level, gMC.x + (3 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
				SetCaret(gMC.x + (3 * 0x200), gMC.y + (16 * 0x200), 3, 0);
			}
		}
		else
		{
			if (gMC.direct == 0)
			{
				for (int l = 0; l < i; l++)
				SetBullet(level, gMC.x - (6 * 0x200), gMC.y + (3 * 0x200), 0);
				SetCaret(gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
			}
			else
			{
				for (int l = 0; l < i; l++)
				SetBullet(level, gMC.x + (6 * 0x200), gMC.y + (3 * 0x200), 2);
				SetCaret(gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
			}
		}

		PlaySoundObject(48, SOUND_MODE_PLAY);
	}
	else 
	{
		++wait;

		if (gMC.equip & EQUIP_TURBOCHARGE)
		{
			if (wait > 1)
			{
				wait = 0;
				ChargeArmsEnergy(1);
			}
		}
		else
		{
			if (wait > 2)
			{
				wait = 0;
				ChargeArmsEnergy(1);
			}
		}
	}
}

void ShootBullet_Sword(int level)
{
	int bul_no;

	if (CountArmsBullet(9) > (level == 3 ? 1 : 0))
		return;
	if (CountBulletNum(72) > 2)
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

		case 4:
			bul_no = 72;
			break;
	}

	if (gKeyTrg & gKeyShot)
	{
		if (gMC.up)
		{
			if (gMC.direct == 0)
				SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y + (4 * 0x200), GravityDirect(1));
			else
				SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y + (4 * 0x200), GravityDirect(1));
		}
		else if (gMC.down)
		{
			if (gMC.direct == 0)
				SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y - (6 * 0x200), GravityDirect(3));
			else
				SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y - (6 * 0x200), GravityDirect(3));
		}
		else
		{
			if (gMC.direct == 0)
				SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y - (3 * 0x200), 0);
			else
				SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y - (3 * 0x200), 2);
		}

		PlaySoundObject(34, SOUND_MODE_PLAY);
	}
}

void ShootBullet_BoomerangSword(int level)
{
	int bul_no;
	if (level == 4 && CountBulletNum(75) > 2)
		return;

	if (level == 1){
		if (CountBulletNum(52) > 10)
			return;
	} else {
		if (CountBulletNum(51 + level) > 0)
			return;
	}

	switch (level)
	{
		case 1:
			bul_no = 52;
			break;

		case 2:
			bul_no = 53;
			break;

		case 3:
			bul_no = 54;
			break;

		case 4:
			bul_no = 75;
			break;
	}

	if (gKeyTrg & gKeyShot)
	{
		if (gMC.up)
		{
			if (gMC.direct == 0)
				SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y + (4 * 0x200), GravityDirect(1));
			else
				SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y + (4 * 0x200), GravityDirect(1));
		}
		else if (gMC.down)
		{
			if (gMC.direct == 0)
				SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y - (6 * 0x200), GravityDirect(3));
			else
				SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y - (6 * 0x200), GravityDirect(3));
		}
		else
		{
			if (gMC.direct == 0)
				SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y - (3 * 0x200), 0);
			else
				SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y - (3 * 0x200), 2);
		}

		PlaySoundObject(34, SOUND_MODE_PLAY);
	}
}

void ShootBullet_Nemesis(int level)
{
	int bul_no;
	static int wait;
	ZeroExpMyChar();

	switch (level)
	{
		case 1:
			bul_no = 34;
			break;

		case 2:
			bul_no = 35;
			break;

		case 3:
			bul_no = 36;
			break;
	}

	if (CountBulletNum(34) > 1)
		return;

	if (gKeyTrg & gMouseClick && GetNPCFlag(871))
	{
		if (gBL.flag || !UseArmsEnergy(2))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);
			return;
		}
		else
		{
			int fx, fy, x, y;
			GetFramePosition(&fx, &fy);
			x = fx + ScreenToSubpixelCoord(gMouseX);
			y = fy + ScreenToSubpixelCoord(gMouseY);
			float dist = 5.5;
			if (GetNPCFlag(871)) dist = 7.5;
			if (Distance(gMC.x, gMC.y, x, y) > dist*0x2000 || GetTile(round(((float)x) / 0x2000), round(((float)y) / 0x2000)) != 1)
			{
				ChargeArmsEnergy(2);
				PlaySoundObject(37, SOUND_MODE_PLAY);
				return;
			}
			else
			{
				DestroyBlock(round(((float)x) / 0x2000), round(((float)y) / 0x2000));
				if (gMC.up)
				{
					SetCaret(gMC.x + (1 * 0x200), gMC.y - (9 * 0x200), 3, 0);
				}
				else if (gMC.down)
				{
					SetCaret(gMC.x + (1 * 0x200), gMC.y + (9 * 0x200), 3, 0);
				}
				else
				{
					if (gMC.direct == 0)
					{
						SetCaret(gMC.x - (16 * 0x200), gMC.y - (5 * 0x200), 3, 0);
					}
					else
					{
						SetCaret(gMC.x + (16 * 0x200), gMC.y - (5 * 0x200), 3, 0);
					}
				}
			}
		}
	}

	if (gKeyTrg & gKeyShot)
	{
		if (!UseArmsEnergy(gBL.flag ? 0 : 1))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);
		}
		else
		{
			if (gMC.up)
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y - (12 * 0x200) * (gMC.flip ? -1 : 1), GravityDirect(1));
					SetCaret(gMC.x - (1 * 0x200), gMC.y - (12 * 0x200), 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y - (12 * 0x200) * (gMC.flip ? -1 : 1), GravityDirect(1));
					SetCaret(gMC.x + (1 * 0x200), gMC.y - (12 * 0x200), 3, 0);
				}
			}
			else if (gMC.down)
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y + (12 * 0x200), GravityDirect(3));
					SetCaret(gMC.x - (1 * 0x200), gMC.y + (12 * 0x200), 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y + (12 * 0x200), GravityDirect(3));
					SetCaret(gMC.x + (1 * 0x200), gMC.y + (12 * 0x200), 3, 0);
				}
			}
			else
			{
				if (gMC.direct == 0)
				{
					if (gMC.equip & EQUIP_MULTIBREAK)
						SetBullet(bul_no, gMC.x - (16 * 0x200), gMC.y + (-6 * 0x200), 0);
					else
						SetBullet(bul_no, gMC.x - (16 * 0x200), gMC.y + (3 * 0x200), 0);
					SetCaret(gMC.x - (16 * 0x200), gMC.y + (3 * 0x200), 3, 0);
				}
				else
				{
					if (gMC.equip & EQUIP_MULTIBREAK)
						SetBullet(bul_no, gMC.x + (16 * 0x200), gMC.y + (-6 * 0x200), 2);
					else
						SetBullet(bul_no, gMC.x + (16 * 0x200), gMC.y + (3 * 0x200), 2);
					SetCaret(gMC.x + (16 * 0x200), gMC.y + (3 * 0x200), 3, 0);
				}
			}

			switch (level)
			{
				case 1:
					PlaySoundObject(117, SOUND_MODE_PLAY);
					break;

				case 2:
					PlaySoundObject(49, SOUND_MODE_PLAY);
					break;

				case 3:
					PlaySoundObject(60, SOUND_MODE_PLAY);
					break;
			}
		}
	}
	else
	{
		++wait;

		if (gMC.equip & EQUIP_TURBOCHARGE)
		{
			if (wait > 120)
			{
				wait = 0;
				ChargeArmsEnergy(1);
			}
		}
	}
}

int spur_charge;

void ResetSpurCharge(void)
{
	spur_charge = 0;

	if (gArmsData[gSelectedArms].code == 13)
		ZeroExpMyChar();
}

void ShootBullet_Spur(int level)
{
	int bul_no;
	bool bShot;
	static bool bMax;

	bShot = false;

	if (gKey & gKeyShot)
	{
		if (gMC.equip & EQUIP_TURBOCHARGE)
			AddExpMyChar(3);
		else
			AddExpMyChar(2);

		if (++spur_charge / 2 % 2)
		{
			switch (level)
			{
				case 1:
					PlaySoundObject(59, SOUND_MODE_PLAY);
					break;

				case 2:
					PlaySoundObject(60, SOUND_MODE_PLAY);
					break;

				case 3: case 4:
					if (!IsMaxExpMyChar())
						PlaySoundObject(61, SOUND_MODE_PLAY);

					break;
			}
		}
	}
	else
	{
		if (spur_charge)
			bShot = true;

		spur_charge = 0;
	}

	if (IsMaxExpMyChar())
	{
		if (!bMax)
		{
			bMax = true;
			PlaySoundObject(65, SOUND_MODE_PLAY);
		}
	}
	else
	{
		bMax = false;
	}

	if (!(gKey & gKeyShot))
		ZeroExpMyChar();

	switch (level)
	{
		case 1:
			bul_no = 6;
			bShot = false;
			break;

		case 2:
			bul_no = 37;
			break;

		case 3:
			if (bMax)
				bul_no = 39;
			else
				bul_no = 38;

			break;

		case 4:
			if (bMax)
				bul_no = 76;
			else
				bul_no = 39;
			break;
	}

	if (CountArmsBullet(13) > 0 || CountArmsBullet(14) > 0)
		return;

	if (gKeyTrg & gKeyShot || bShot)
	{
		if (!UseArmsEnergy(1))
		{
			PlaySoundObject(37, SOUND_MODE_PLAY);
		}
		else
		{
			if (gMC.up)
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
					SetCaret(gMC.x - (1 * 0x200), gMC.y - (8 * 0x200), 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y - (8 * 0x200), GravityDirect(1));
					SetCaret(gMC.x + (1 * 0x200), gMC.y - (8 * 0x200), 3, 0);
				}
			}
			else if (gMC.down)
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
					SetCaret(gMC.x - (1 * 0x200), gMC.y + (8 * 0x200), 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y + (8 * 0x200), GravityDirect(3));
					SetCaret(gMC.x + (1 * 0x200), gMC.y + (8 * 0x200), 3, 0);
				}
			}
			else
			{
				if (gMC.direct == 0)
				{
					SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y + (3 * 0x200), 0);
					SetCaret(gMC.x - (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
				}
				else
				{
					SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y + (3 * 0x200), 2);
					SetCaret(gMC.x + (12 * 0x200), gMC.y + (3 * 0x200), 3, 0);
				}
			}

			switch (bul_no)
			{
				case 6:
					PlaySoundObject(49, SOUND_MODE_PLAY);
					break;

				case 37:
					PlaySoundObject(62, SOUND_MODE_PLAY);
					break;

				case 38:
					PlaySoundObject(63, SOUND_MODE_PLAY);
					break;

				case 39: case 76:
					PlaySoundObject(64, SOUND_MODE_PLAY);
					break;
			}
		}
	}
}

void ShootBullet_SwordSwing(int level)
{
	int bul_no;

	if (CountBulletNum(58) > 0 || CountBulletNum(59) > 0 || CountBulletNum(60) > 0 || CountBulletNum(74) > 0)
		return;

	switch (level)
	{
		case 1:
			bul_no = 58;
			break;

		case 2:
			bul_no = 59;
			break;

		case 3:
			bul_no = 60;
			break;
		
		case 4:
			bul_no = 74;
			break;
	}

	if (gKey & gKeyShot)
	{
		if (gMC.up)
		{
			if (gMC.direct == 0)
				SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y + (4 * 0x200), 0);
			else
				SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y + (4 * 0x200), 2);
		}
		else if (gMC.down)
		{
			if (gMC.direct == 0)
				SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y - (6 * 0x200), 0);
			else
				SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y - (6 * 0x200), 2);
		}
		else
		{
			if (gMC.direct == 0)
				SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y - (3 * 0x200), 0);
			else
				SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y - (3 * 0x200), 2);
		}

		PlaySoundObject(106, SOUND_MODE_PLAY);
	}
}

void ShootBullet_SwordSwing2(int level)
{
	int bul_no;

	if (CountBulletNum(80) > 0 || CountBulletNum(81) > 0 || CountBulletNum(82) > 0 || CountBulletNum(85) > 0)
		return;

	switch (level)
	{
		case 1:
			bul_no = 80;
			break;

		case 2:
			bul_no = 81;
			break;

		case 3:
			bul_no = 82;
			break;
		
		case 4:
			bul_no = 85;
			break;
	}

	if (gKey & gKeyShot)
	{
		if (gMC.up)
		{
			if (gMC.direct == 0)
				SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y + (4 * 0x200), 0);
			else
				SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y + (4 * 0x200), 2);
		}
		else if (gMC.down)
		{
			if (gMC.direct == 0)
				SetBullet(bul_no, gMC.x - (1 * 0x200), gMC.y - (6 * 0x200), 0);
			else
				SetBullet(bul_no, gMC.x + (1 * 0x200), gMC.y - (6 * 0x200), 2);
		}
		else
		{
			if (gMC.direct == 0)
				SetBullet(bul_no, gMC.x + (6 * 0x200), gMC.y - (3 * 0x200), 0);
			else
				SetBullet(bul_no, gMC.x - (6 * 0x200), gMC.y - (3 * 0x200), 2);
		}

		PlaySoundObject(106, SOUND_MODE_PLAY);
	}
}

void ShootBullet(void)
{
	static int soft_rensha;	// 'rensha' is Japanese for 'rapid-fire', apparently

	if (empty != 0)
		--empty;

	// Only let the player shoot every 4 frames
	if (soft_rensha != 0)
		--soft_rensha;

	if (gKeyTrg & gKeyShot)
	{
		if (soft_rensha != 0)
			return;

		soft_rensha = 2;
	}

	// Run functions
	if (gMC.cond & 2 || CheckForCooldown(gCooldowns, SHEEP_CURSE_ID) > 0)
		return;

	switch (gArmsData[gSelectedArms].code)
	{
		case 1:
			ShootBullet_Frontia1(gArmsData[gSelectedArms].level);
			break;

		case 2:
			ShootBullet_PoleStar(gArmsData[gSelectedArms].level);
			break;

		case 3:
			ShootBullet_FireBall(gArmsData[gSelectedArms].level);
			break;

		case 4:
			ShootBullet_Machinegun1(gArmsData[gSelectedArms].level);
			break;

		case 21:
			for (int i = 0; i < gArmsData[gSelectedArms].level; i++)
				ShootBullet_Machinegun2(gArmsData[gSelectedArms].level);
			break;

		case 5:
			ShootBullet_Missile(gArmsData[gSelectedArms].level, false);
			break;

		case 7:
			switch (gArmsData[gSelectedArms].level)
			{
				case 1:
					ShootBullet_Bubblin1();
					break;

				case 2:
					ShootBullet_Bubblin2(2);
					break;

				case 3:
					ShootBullet_Bubblin2(3);
					break;

				case 4:
					ShootBullet_Bubblin2(4);
					break;
			}

			break;

		case 9:
			ShootBullet_Sword(gArmsData[gSelectedArms].level);
			break;

		case 10:
			ShootBullet_Missile(gArmsData[gSelectedArms].level, true);
			break;

		case 11:
			ShootBullet_Shotgun(gArmsData[gSelectedArms].level);
			break;

		case 12:
			ShootBullet_Nemesis(gArmsData[gSelectedArms].level);
			break;

		case 13:
			ShootBullet_Spur(gArmsData[gSelectedArms].level);
			break;

		case 14:
			ShootBullet_WaterStar(gArmsData[gSelectedArms].level);
			break;

		case 15:
			ShootBullet_BoomerangSword(gArmsData[gSelectedArms].level);
			break;

		case 16:
			ShootBullet_Rifle(gArmsData[gSelectedArms].level);
			break;

		case 17:
			ShootBullet_SwordSwing(gArmsData[gSelectedArms].level);
			break;

		case 18:
			ShootBullet_MiningLaser(gArmsData[gSelectedArms].level);
			break;

		case 8:
			ShootBullet_Hook(gArmsData[gSelectedArms].level);
			break;

		case 19:
			ShootBullet_RobotGun(gArmsData[gSelectedArms].level);
			break;

		case 20:
			ShootBullet_SwordSwing2(gArmsData[gSelectedArms].level);
			break;

		case 22:
			ShootBullet_FrontieBuster(gArmsData[gSelectedArms].level);
			break;
	}
}
