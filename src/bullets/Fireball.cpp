#include "Fireball.h"

#include "../Bullet.h"
#include "../Caret.h"
#include "../Flags.h"
#include "../MyChar.h"
#include "../NpChar.h"
#include "../Sound.h"

void ActBullet_FireBall(BULLET *bul, int level)
{
	bool bBreak;
	if (CheckFlagTrait(FlagTraits::WITCH_FIREBALL)){
		bul->life_count = 60;
	}

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		if (CheckFlagTrait(FlagTraits::WITCH_FIREBALL)){
		int lol = Random(4, 8);
		for (int i = 0; i < lol; i++){
			BULLET* bully = SetBullet(Bullets::BOOMERANG_BLADE_LVL_2_FLAME, bul->x, bul->y);
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
			SetBullet(Bullets::FIREBALL_LVL_2, bul->x, bul->y, 0);
		}
	}
	if (bul->direct == 2 && bul->flag & 4){
		bul->direct = 0;
		if (level == 4){
			SetBullet(Bullets::FIREBALL_LVL_2, bul->x, bul->y, 0);
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
		if (CheckFlagTrait(FlagTraits::WITCH_FIREBALL)){
			bul->xm *= 1.7;
			bul->ym *= 1.3;
		}
	}
	else
	{
		if (CheckFlagTrait(FlagTraits::PROPANE_TRAIL) && bul->count1 % GetFlagTraitData<int>(FlagTraits::PROPANE_TRAIL, "rate") == 0){
			int xCoord, yCoord;
			xCoord = bul->x - 0x200 * 5;
			yCoord = bul->y - 0x200 * 5;
			SetBullet(Bullets::FIREBALL_FLAME, xCoord, yCoord, 0);
		}
		if (bul->flag & 8){
			bul->ym = -0x400;
			if (level == 4){
				SetBullet(Bullets::FIREBALL_LVL_2, bul->x, bul->y, 1);
			}
			if (CheckFlagTrait(FlagTraits::WITCH_FIREBALL)){
				bul->ym *= 1.5;
			}
		}
		else if (bul->flag & 1){
			bul->xm = 0x500;
			if (CheckFlagTrait(FlagTraits::WITCH_FIREBALL)){
				bul->xm *= 1.5;
			}
		}
		else if (bul->flag & 4){
			bul->xm = -0x500;
			if (CheckFlagTrait(FlagTraits::WITCH_FIREBALL)){
				bul->xm *= 1.5;
			}
		}

		bul->ym += 40;
		if (bul->ym > 0x3FF)
			bul->ym = 0x3FF;
		if (CheckFlagTrait(FlagTraits::WITCH_FIREBALL)){
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

void ActBullet_FireballFlame(BULLET *bul)
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

void RegisterFireball(){
    gBulletTable.insert({Bullets::FIREBALL_LVL_1, {
        {2, 2, 120, 8, 8, 16, 4, 2, {8, 8, 8, 8}, 0.8, 1},
        [](BULLET* bul){ ActBullet_FireBall(bul, 1); }
    }});
    gBulletTable.insert({Bullets::FIREBALL_LVL_2, {
        {3, 2, 120, 8, 4, 4, 4, 2, {8, 8, 8, 8}, 0.9, 1},
        [](BULLET* bul){ ActBullet_FireBall(bul, 2); }
    }});
    gBulletTable.insert({Bullets::FIREBALL_LVL_3, {
        {4, 2, 120, 8, 4, 4, 4, 2, {8, 8, 8, 8}, 0.9, 1},
        [](BULLET* bul){ ActBullet_FireBall(bul, 3); }
    }});
    gBulletTable.insert({Bullets::FIREBALL_LVL_4, {
        {5, 2, 120, 8, 4, 4, 4, 2, {8, 8, 8, 8}, 0.92, 0.95},
        [](BULLET* bul){ ActBullet_FireBall(bul, 4); }
    }});

    gBulletTable.insert({Bullets::FIREBALL_FLAME, {
        {1, 1, 100, 8, 8, 8, 8, 8, {4, 2, 5, 2}, 0.5, 0.5},
        ActBullet_FireballFlame
    }});
}