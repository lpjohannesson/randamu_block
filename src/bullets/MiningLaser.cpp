#include "MiningLaser.h"

#include "..\Bullet.h"
#include "..\Caret.h"

void ActBullet_MiningLaser(BULLET *bul, int level)
{
	bool bHit;

	static unsigned int inc;

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

	bul->local_iframes = 3;

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

void RegisterMiningLaser(){
    gBulletTable.insert({Bullets::MINING_LASER, {
        {9, 100, 30, 32, 2, 2, 1, 1, {8, 8, 8, 8}, 0, 0},
        [](BULLET* bul){ ActBullet_MiningLaser(bul, 1); }
    }});
    gBulletTable.insert({Bullets::MINING_BOOM, {
        {9, 100, 40, 40, 4, 4, 4, 4, {8, 8, 8, 8}, 0, 0},
        [](BULLET* bul){ ActBullet_MiningLaser(bul, 2); }
    }});
}