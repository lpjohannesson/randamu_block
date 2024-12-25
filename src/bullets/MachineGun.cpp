#include "MachineGun.h"
#include "..\ArmsItem.h"
#include "..\Bullet.h"
#include "..\Caret.h"
#include "..\Flags.h"
#include "..\MyChar.h"
#include "..\NpChar.h"

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

	if (bul->code_bullet != Bullets::MACHINE_GUN_GATLING){
		if (GetNPCFlag(Flags::CURLY_CHARACTER) || GetNPCFlag(Flags::MAHIN_CHARACTER)){
			switch (level){
				case 1: bul->damage = Random(3, 4); break;
				case 2: bul->damage = Random(5, 6); break;
				case 3: bul->damage = Random(7, 8); break;
				case 4: bul->damage = Random(9, 10); break;
			}
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
			secondMove *= 7;

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

void RegisterMachineGun(){
    gBulletTable.insert({Bullets::MACHINE_GUN_LVL_1, {
        {3, 3, 16, 32, 5, 3, 2, 2, {8, 8, 8, 8}, 0.14, 0.14},
        [](BULLET* bul){ ActBullet_MachineGun(bul, 1); }
    }});
    gBulletTable.insert({Bullets::MACHINE_GUN_LVL_2, {
        {5, 3, 16, 32, 5, 3, 2, 2, {8, 8, 8, 8}, 0.18, 0.18},
        [](BULLET* bul){ ActBullet_MachineGun(bul, 2); }
    }});
    gBulletTable.insert({Bullets::MACHINE_GUN_LVL_3, {
        {7, 3, 16, 32, 5, 3, 2, 2, {8, 8, 8, 8}, 0.29, 0.29},
        [](BULLET* bul){ ActBullet_MachineGun(bul, 3); }
    }});
    gBulletTable.insert({Bullets::MACHINE_GUN_LVL_4, {
        {9, 6, 16, 32, 5, 3, 2, 2, {8, 8, 8, 8}, 0.29, 0.29},
        [](BULLET* bul){ ActBullet_MachineGun(bul, 4); }
    }});

    gBulletTable.insert({Bullets::MACHINE_GUN_GATLING, {
        {1, 2, 20, 32, 4, 2, 2, 2, {8, 8, 8, 8}, 0.10, 0.10},
        [](BULLET* bul){ ActBullet_MachineGun(bul, 1); }
    }});
}