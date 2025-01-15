#include "EnergyFlail.h"

#include "..\Bullet.h"
#include "..\Caret.h"
#include "..\KeyControl.h"
#include "..\Flags.h"
#include "..\Map.h"
#include "..\MyChar.h"
#include "..\Triangle.h"

void ActBullet_EnergyFlail(BULLET *bul, int level)
{
	if (bul->code_bullet == Bullets::ENERGY_FLAIL_LVL_4_SPAWN)
		bul->local_iframes = 4;
	else 
		bul->iframes = 15;

	if (level == 4) bul->iframes /= 4;

	if (CheckFlagTrait(FlagTraits::HOOK_FLAIL))
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
		if (bul->code_bullet != Bullets::ENERGY_FLAIL_LVL_4_SPAWN)
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
		if (!CheckFlagTrait(FlagTraits::HOOK_FLAIL))
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
			if (bul->code_bullet == Bullets::ENERGY_FLAIL_LVL_4_SPAWN)
				bul->local_iframes = 2;
			else
				bul->iframes = 6;

			if (level == 4) bul->iframes /= 3;
			
			if ((!(gKey & gKeyShot && !((level == 4 || bul->code_bullet == Bullets::ENERGY_FLAIL_LVL_4_SPAWN))) || CheckFlagTrait(FlagTraits::HOOK_FLAIL))  && (bul->x > gMC.x - 0x200*8 && bul->x < gMC.x + 0x200*8 &&
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
			if (gKey & gKeyShot && !(level == 4 || bul->code_bullet == Bullets::ENERGY_FLAIL_LVL_4_SPAWN) && !CheckFlagTrait(FlagTraits::HOOK_FLAIL) && Distance(gMC.x, gMC.y, bul->x, bul->y) < 8 * 16_pix){
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
				BULLET *bully = SetBullet(Bullets::ENERGY_FLAIL_LVL_4_SPAWN, x, y);
				if (bully != NULL)
				{
					bully->tgt_x = bul->x;
					bully->tgt_y = bul->y;
					bully->damage = 4;
				}
			}
			bul->x += bul->xm;
			bul->y += bul->ym;
			if (bul->act_no == 2 && level == 2 && bul->count1 % 4 == 0 && gKey & gKeyShot && !CheckFlagTrait(FlagTraits::HOOK_FLAIL)){
				int x, y;
				do
				{
					x = Random(0, gMap.width*0x2000);
					y = Random(0, gMap.length*0x2000);
				} while (!(Distance(bul->x, bul->y, x, y) < 90_pix  && 
							Distance(bul->x, bul->y, x, y) > 8_pix));
				for (int i = 0; i < Random(1, 2); i++){
					BULLET *quiverProj = SetBullet(Bullets::ENERGY_PROJECTILE, bul->x, bul->y);
					if (quiverProj != NULL){
						quiverProj->tgt_x = x;
						quiverProj->tgt_y = y;
						quiverProj->count1 = quiverProj->life_count/1.5f;
					}
				}
			}
			if (level == 3){
				if (bul->count1 % 8 == 0){
					SetBullet(Bullets::BLADE_SLASH, bul->x, bul->y, 0);
				}
				else if (bul->count1 % 8 == 2){
					SetBullet(Bullets::BLADE_SLASH, bul->x, bul->y, 2);
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

void RegisterEnergyFlail(){
    gBulletTable.insert({Bullets::ENERGY_FLAIL_LVL_1, {
        {12, 100, 25, 105, 10, 10, 6, 6, {4, 4, 4, 4}, 0.33, 0.33},
        [](BULLET* bul){ ActBullet_EnergyFlail(bul, 1); }
    }});
    gBulletTable.insert({Bullets::ENERGY_FLAIL_LVL_2, {
        {15, 100, 30, 105, 11, 11, 6, 6, {4, 4, 4, 4}, 0.33, 0.33},
        [](BULLET* bul){ ActBullet_EnergyFlail(bul, 2); }
    }});
    gBulletTable.insert({Bullets::ENERGY_FLAIL_LVL_3, {
        {10, 100, 35, 105, 12, 12, 6, 6, {4, 4, 4, 4}, 0.33, 0.33},
        [](BULLET* bul){ ActBullet_EnergyFlail(bul, 3); }
    }});
    gBulletTable.insert({Bullets::ENERGY_FLAIL_LVL_4, {
        {4, 100, 30, 105, 12, 12, 4, 4, {4, 4, 4, 4}, 0.33, 0.33},
        [](BULLET* bul){ ActBullet_EnergyFlail(bul, 4); }
    }});
    gBulletTable.insert({Bullets::ENERGY_FLAIL_LVL_4_SPAWN, {
        {12, 100, 25, 105, 10, 10, 6, 6, {4, 4, 4, 4}, 0.33, 0.33},
        [](BULLET* bul){ ActBullet_EnergyFlail(bul, 1); }
    }});
}