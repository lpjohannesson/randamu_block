#include "Spur.h"

#include "..\Bullet.h"
#include "..\Caret.h"
#include "..\Flags.h"

void ActBullet_Spur(BULLET *bul, int level)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (CheckFlagTrait(FlagTraits::INVIDIA)){
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

	if (!CheckFlagTrait(FlagTraits::INVIDIA)){
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

	SetBullet(Bullets::SPUR_TRAIL_LVL_1 + (level-1), bul->x, bul->y, bul->direct);
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

	if (!CheckFlagTrait(FlagTraits::INVIDIA)){
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

void ActBullet_Spur2(BULLET *bul, int level)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (CheckFlagTrait(FlagTraits::INVIDIA)){
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

	SetBullet(Bullets::BIGGER_EXPLOSION, bul->x, bul->y, bul->direct);
}

void RegisterSpur(){
    gBulletTable.insert({Bullets::SPUR_LVL_1, {
        {4, 4, 45, 64, 6, 6, 3, 3, {8, 8, 8, 8}, 1, 1},
        [](BULLET* bul){ ActBullet_Spur(bul, 1); }
    }});
    gBulletTable.insert({Bullets::SPUR_LVL_2, {
        {8, 8, 60, 64, 6, 6, 3, 3, {8, 8, 8, 8}, 1, 1},
        [](BULLET* bul){ ActBullet_Spur(bul, 2); }
    }});
    gBulletTable.insert({Bullets::SPUR_LVL_3, {
        {12, 12, 90, 64, 6, 6, 3, 3, {8, 8, 8, 8}, 1, 1},
        [](BULLET* bul){ ActBullet_Spur(bul, 3); }
    }});
    gBulletTable.insert({Bullets::SPUR_LVL_4, {
        {13, 13, 120, 64, 6, 6, 3, 3, {8, 8, 8, 8}, 1, 1},
        [](BULLET* bul){ ActBullet_Spur2(bul, 3); }
    }});

    gBulletTable.insert({Bullets::SPUR_TRAIL_LVL_1, {
        {3, 100, 45, 32, 6, 6, 3, 3, {4, 4, 4, 4}, 1, 1},
        [](BULLET* bul){ ActBullet_SpurTail(bul, 1); }
    }});
    gBulletTable.insert({Bullets::SPUR_TRAIL_LVL_2, {
        {6, 100, 45, 32, 6, 6, 3, 3, {4, 4, 4, 4}, 1, 1},
        [](BULLET* bul){ ActBullet_SpurTail(bul, 2); }
    }});
    gBulletTable.insert({Bullets::SPUR_TRAIL_LVL_3, {
        {11, 100, 45, 32, 6, 6, 3, 3, {4, 4, 4, 4}, 1, 1},
        [](BULLET* bul){ ActBullet_SpurTail(bul, 3); }
    }});
}