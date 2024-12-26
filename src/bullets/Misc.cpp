#include "Misc.h"

#include "..\Bullet.h"
#include "..\Caret.h"
#include "..\Cooldown.h"
#include "..\Flags.h"
#include "..\MyChar.h"
#include "..\Triangle.h"

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

void ActBullet_Duck(BULLET *bul)
{
	if (bul->act_no == 0)
	{
		bul->ani_no = Random(0, 2);
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = Random(-0x800, -0x200);
				bul->ym = Random(-0x200, -0x80);
				break;
			case 1:
				bul->ym = Random(-0x800, -0x200);
				bul->xm = Random(-0x200, -0x202);
				break;
			case 2:
				bul->xm = Random(0x200, 0x800);
				bul->ym = Random(-0x200, -0x80);
				break;
			case 3:
				bul->ym = Random(0x100, 0x500);
				bul->xm = Random(-0x200, -0x202);
				break;
		}
	}
	else
	{
		bul->ym += 50;
		if (bul->ym >= 0x6FF)
			bul->ym = 0x6FF;
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	if (++bul->ani_wait > 10)
	{
		bul->ani_wait = 0;
		if (bul->ani_no == 0)
			bul->ani_no = 1;
		else
			bul->ani_no = 0;
	}

	Rect rcs[][2] = {
		{{30, 147, 42, 157}, {30, 162, 42, 172}},
		{{66, 147, 76, 159}, {51, 147, 61, 159}},
		{{78, 147, 90, 157}, {78, 162, 90, 172}},
		{{50, 161, 60, 173}, {65, 161, 75, 173}}
	};

	bul->rect = rcs[bul->direct][bul->ani_no];
}

void ActBullet_Hitbox(BULLET *bul)
{
	if (++bul->count1 > bul->life_count)
		bul->cond = 0;
}

void ActBullet_RedBurst(BULLET *bul)
{
	if (++bul->ani_no > Random(0, 3)){
		bul->ani_no = 0;
		SetCaret(bul->x, bul->y, 11, 0);
	}

	bul->y += bul->ym;
	bul->x += bul->xm;
}

void ActBullet_SpikeDrop(BULLET *bul)
{
	Rect rc = {0, 0, 0, 0};

	if (++bul->act_wait > 2)
		bul->cond = 0;

	bul->rect = rc;
}

void ActBullet_Nanites(BULLET *bul)
{
	unsigned char deg, lol, speed;

	if (bul->act_no == 0)
	{
		bul->ani_no = Random(0, 2);
		bul->act_no = 1;

		bul->count1 = 0;
		bul->count2 = 60;
	}
	else
	{
		if (CheckForCooldown(gCooldowns, NANITES_CD_ID) == -1){
			lol = 3;
			speed = (CheckForCooldown(gCooldowns, NANITES_TIME_ID) > 0 ? 16 : 8);
			
			if (GetNPCFlag(Flags::MILITARY_ARMAMENT)){
				short rate = (CheckForCooldown(gCooldowns, NANITES_TIME_ID) > 0 ? 5 : 40);
				if (++bul->act_wait % rate == 0){
					short amount = (CheckForCooldown(gCooldowns, NANITES_TIME_ID) > 0 ? 2 : 1);
					for (int i = 0; i < amount; i++){
						BULLET *bullet = SetBullet(47, bul->x, bul->y);
						if (bullet != NULL){
							bullet->direct = 1;
							bullet->tgt_x = gMC.x;
							bullet->tgt_y = gMC.y;
						}
					}
				}
			}
		} else {
			lol = 0;
			speed = 20;
		}
		bul->count1 += speed;
		SetNpChar(129, bul->x, bul->y, 0, 0, bul->ani_no + lol, NULL, 0x100);

		deg = bul->count1 / 4;

		bul->x = gMC.x + gMC.xm + bul->count2 * GetCos(deg) / 4;
		bul->y = gMC.y + gMC.ym + bul->count2 * GetSin(deg) / 4;

		// bul->xm = bul->tgt_x - bul->x;
		// bul->ym = bul->tgt_y - bul->y;
		// if ((bul->count1 / 2 % 2) == 0)
		// 	bul->ym = (bul->tgt_y - bul->y) / 2;
	}

	if (++bul->ani_wait > 0)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 2)
		bul->ani_no = 0;

	// bul->x += bul->xm;
	// bul->y += bul->ym;

	if (!GetNPCFlag(Flags::NANITES_ABSORBER) && !GetNPCFlag(Flags::MILITARY_ARMAMENT)){
		bul->cond = 0;
		return;
	}
		
	bul->rect = {0,0,0,0};
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
    gBulletTable.insert({Bullets::DUCK, {
        {2, 1, 22, 32, 4, 4, 4, 4, {8, 8, 8, 8}, 0.25, 0.25},
        ActBullet_Duck
    }});
    gBulletTable.insert({Bullets::UNUSED_SHIELD_HITBOX, {
        {1, 1, 1, 36, 4, 4, 4, 4, {1, 1, 1, 1}, 0.22, 0.22},
        ActBullet_Hitbox
    }});
    gBulletTable.insert({Bullets::OMEGA_SELFHURT, {
        {3, 1, 1, 36, 4, 4, 4, 4, {1, 1, 1, 1}, 0.22, 0.22},
        ActBullet_Hitbox
    }});
    gBulletTable.insert({Bullets::RED_CRYSTAL_PROJECTILE, {
        {4, 1, 20, 32, 2, 2, 2, 2, {8, 8, 8, 8}, 0.14, 0.14},
        ActBullet_RedBurst
    }});
    gBulletTable.insert({Bullets::FALLING_SPIKE, {
        {127, 1, 2, 4, 8, 4, 8, 4, {0, 0, 0, 0}, 1, 1},
        ActBullet_SpikeDrop
    }});
    gBulletTable.insert({Bullets::NANITES_VFX, {
        {-1, 2, 100, 12, 8, 16, 4, 2, {8, 8, 8, 8}, 0.8, 1},
        ActBullet_Nanites
    }});
}