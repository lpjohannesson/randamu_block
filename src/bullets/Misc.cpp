#include "Misc.h"

#include "..\Bullet.h"
#include "..\Caret.h"

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

			if (Random(0, 1))
				bul->damage = 1;
			else
				bul->damage = 0;

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

void RegisterMiscBullets(){
    gBulletTable.insert({Bullets::WATER_DROPLET, {
        {-1, 1, 35, 8, 8, 8, 8, 8, {4, 2, 5, 2}, 0.5, 0.5},
        ActBullet_Droplet
    }});
    gBulletTable.insert({Bullets::BLADE_SLASH, {
        {0, 100, 0, 36, 7, 7, 8, 8, {12, 12, 12, 12}, 0, 0},
        ActBullet_Edge
    }});
    gBulletTable.insert({Bullets::SPUR_LVL_0, {
        {4, 1, 15, 32, 6, 6, 2, 2, {8, 8, 8, 8}, 0.4, 0.4},
        [](BULLET* bul){ ActBullet_PoleStar(bul, 3); }
    }});
    gBulletTable.insert({Bullets::SHOTGUN_SHOT, {
        {6, 2, 11, 32, 6, 6, 2, 2, {8, 8, 8, 8}, 0.6, 0.6},
        [](BULLET* bul){ ActBullet_PoleStar(bul, 1); }
    }});
    gBulletTable.insert({Bullets::PROCGEN_GUN_SPARK, {
        {1, 1, 45, 32, 4, 4, 4, 4, {4, 4, 4, 4}, 0.25, 0.25},
        ActBullet_Spark
    }});
    gBulletTable.insert({Bullets::BLADE_SLASH_FAKE, {
        {0, 100, 0, 36, 8, 8, 8, 8, {12, 12, 12, 12}, 0, 0},
        ActBullet_Edge2
    }});
    gBulletTable.insert({Bullets::ENEMY_CLEAR, {
        {0, 4, 4, 4, 0, 0, 0, 0, {0, 0, 0, 0}, 1, 1},
        ActBullet_Edge2
    }});
}