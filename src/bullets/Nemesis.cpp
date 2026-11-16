#include "Nemesis.h"

#include "../Bullet.h"
#include "../Caret.h"
#include "../Flags.h"

void ActBullet_Nemesis(BULLET *bul, int type)
{
	if (++bul->count1 > bul->life_count && type != 2)
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
		if (type == 2){
			bul->xm /= 1.5;
			bul->ym /= 1.5;
			bul->local_iframes = 8;
		}
		if (CheckFlagTrait(FlagTraits::NEMESIS_BOOST) && type == 1){
			bul->enemyXL *= 2;
			bul->enemyYL *= 2;
			bul->local_iframes = 16;
			bul->damage *= bul->life;
		}
	}
	else
	{
		if (bul->count1 % 4 == 1)
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

	bul->rect.left += (type - 1) * 128;
	bul->rect.right += (type - 1) * 128;
}

void RegisterNemesis(){
    gBulletTable.insert({Bullets::NEMESIS_MAIN, {
        {3, 3, 22, 32, 4, 4, 3, 3, {8, 8, 8, 8}, 0.65, 0.65},
        [](BULLET* bul){ ActBullet_Nemesis(bul, 1); }
    }});
    gBulletTable.insert({Bullets::NEMESIS_CURLY, {
        {8, 1, 17, 32, 4, 4, 3, 3, {8, 8, 24, 8}, 0.75, 0.75},
        [](BULLET* bul){ ActBullet_Nemesis(bul, 1); }
    }});
	gBulletTable.insert({Bullets::BOOSTERAMA_PROJECTILE, {
        {2, 127, 32, 32, 4, 4, 3, 3, {8, 8, 24, 8}, 0.75, 0.75},
        [](BULLET* bul){ ActBullet_Nemesis(bul, 2); }
    }});
}