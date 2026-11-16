#include "Rifle.h"

#include "../Bullet.h"
#include "../Caret.h"

void ActBullet_Rifle(BULLET *bul, int level)
{
	bul->local_iframes = 10;

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
						bul->enemyYL = 0x600;
						break;
					case 1:
						bul->enemyXL = 0x600;
						break;
					case 2:
						bul->enemyYL = 0x600;
						break;
					case 3:
						bul->enemyXL = 0x600;
						break;
				}
				break;
			case 2: case 3:
				switch (bul->direct)
				{
					case 0:
						bul->enemyYL = 0x900;
						break;
					case 1:
						bul->enemyXL = 0x900;
						break;
					case 2:
						bul->enemyYL = 0x900;
						break;
					case 3:
						bul->enemyXL = 0x900;
						break;
				}
				break;
		}
	}
	else
	{
		// Move
		bul->life = 100;
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

void RegisterRifle(){
    gBulletTable.insert({Bullets::RIFLE_LVL_1, {
        {4, 2, 24, 32, 6, 6, 2, 2, {8, 8, 8, 8}, 0.3, 0.4},
        [](BULLET* bul){ ActBullet_Rifle(bul, 1); }
    }});
    gBulletTable.insert({Bullets::RIFLE_LVL_2, {
        {7, 3, 30, 32, 6, 6, 2, 2, {8, 8, 8, 8}, 0.3, 0.4},
        [](BULLET* bul){ ActBullet_Rifle(bul, 2); }
    }});
    gBulletTable.insert({Bullets::RIFLE_LVL_3, {
        {10, 4, 46, 32, 6, 6, 2, 2, {8, 8, 8, 8}, 0.3, 0.4},
        [](BULLET* bul){ ActBullet_Rifle(bul, 3); }
    }});
}