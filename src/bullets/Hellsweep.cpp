#include "Hellsweep.h"

#include "..\Bullet.h"
#include "..\Caret.h"
#include "..\Cooldown.h"
#include "..\Map.h"
#include "..\MyChar.h"
#include "..\Sound.h"

void ActBullet_Hellsweep(BULLET *bul, int level)
{
	gMC.direct = bul->direct;

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		return;
	}

	bool isBoostActive = CheckForCooldown(gCooldowns, HELLSWEEP_BOOST_ID) != -1;

	if (bul->act_no == 0){
		if (bul->direct == 2){
			bul->angle = -120;
			bul->centerX = 0;
			bul->centerY = 1;
		}
		else{
			bul->centerX = 1;
			bul->centerY = 1;
			bul->angle = 120;
		}
		if (gMC.life <= gMC.max_life / 3 || isBoostActive){
			bul->life_count /= 2;
			bul->count2 = 1;
		}
		bul->iframes = bul->life_count;
		bul->act_no = 1;
		if (bul->ani_no != 0){
			bul->damage *= 4;
		}
	} else {
		int spinValue = 3 + level*1;
		int swingValue = 26;
		if (bul->count2 == 1){
			spinValue *= 2;
			swingValue /= 2;
		}
		if (bul->direct == 2)
			bul->angle += spinValue;
		else
			bul->angle -= spinValue;

		int whirlLimit = 3;
		if (level == 4) whirlLimit = 5;

		if (bul->ani_no != 0){
			SetCaret(bul->x + Random(-4_pix, 4_pix), bul->y + Random(-4_pix, 4_pix), 1, 2);
		}

		if (isBoostActive){
			SetBullet(Bullets::PROCGEN_GUN_SPARK, gMC.x + Random(-16_pix, 16_pix), gMC.y + Random(-16_pix, 16_pix), Random(0, 3));
		}

		if ((level == 3) && bul->count1 == swingValue){
			int lol = Random(5, 20);
			for (int i = 0; i < lol; i++)
			{
				BULLET *bully = SetBullet(Bullets::HELLSWEEP_FLAME, bul->x, bul->y);
				if (bully != NULL)
				{
					PlaySoundObject(33, SOUND_MODE_PLAY);
					bully->direct = gMC.direct;
					if (bul->direct == 0)
						bully->xm = -Random(0, 2000);
					else
						bully->xm = Random(0, 2000);
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
					if (bul->count2 == 1){
						bully->xm *= 1.2;
						bully->ym *= 1.2;
					}
				}
			}
		}

		if ((level == 2 || level == 4) && bul->count1 == swingValue){
			int lol = 2;
			if (level == 4) lol+=2;
			for (int i = 0; i < lol; i++)
			{
				int flameX = bul->x + Random(-0x2000, 0x2000), flameY = bul->y;
				while (IsEmpty(flameX / 0x2000, flameY / 0x2000) && bul->y - flameY < 0x2000 * 4){
					flameY -= 0x2000;
				}
				flameY += 0x2000;
				int flameID = Bullets::FIREBALL_LVL_2;
				if (level == 4) flameID = Bullets::HELLSWEEP_FIREMINE;
				BULLET *bully = SetBullet(flameID, flameX, flameY);
				if (bully != NULL)
				{
					SetDestroyNpChar(flameX, flameY, 0x1000, 8);
					PlaySoundObject(71, SOUND_MODE_PLAY);
					bully->direct = gMC.direct;
					if (level == 2)
						bully->damage = 1;
					bully->life = Random(2, 5);
					if (bul->direct == 0)
						bully->xm = -Random(0, 1800);
					else
						bully->xm = Random(0, 1800);
					if (gMC.up){
						bully->ym = Random(-1300, -400);
						bully->xm /= 5;
					}
					else if (gMC.down){
						bully->ym = -Random(-1300, -400);
						bully->xm /= 5;
					}
					else
						bully->ym = Random(-700, 80);
					if (bul->count2 == 1){
						bully->xm *= 1.4;
						bully->ym *= 1.4;
					}
				}
			}
		}

		if (bul->life != 100){
			if (bul->ani_no != 0){
				if (CheckForCooldown(gCooldowns, HELLSWEEP_BOOST_ID) == -1)
					InitiateCooldown(gCooldowns, HELLSWEEP_BOOST_ID);
				bul->damage = std::max(bul->damage - gBulletTable[bul->code_bullet].table.damage, static_cast<int>(gBulletTable[bul->code_bullet].table.damage));
				PlaySoundObject(72);
			}
			if ((level == 1 || level == 4)){
				bul->life = 100;
				BULLET *bully = SetBullet(Bullets::HELLSWEEP_FIREMINE, bul->x, bul->y);
				if (bully != NULL)
				{
					PlaySoundObject(21, SOUND_MODE_PLAY);
					bully->direct = gMC.direct;
				}
			}
		}
	}

	bul->x = gMC.x + gMC.xm;
	if (gMC.direct == 0){
		bul->x -= 0x200 * 12 * (float(bul->count1) / float(bul->life_count));
	} else {
		bul->x += 3_pix + 0x200 * 12 * (float(bul->count1) / float(bul->life_count));
	}
	bul->y = gMC.y + gMC.ym - 0x200 * 7 + 0x200 * 5 * (float(bul->count1) / float(bul->life_count));

	Rect rcRight[4] = {
		{224, 176, 237, 189},
		{240, 176, 255, 191},
		{256, 176, 272, 192},
		{272, 176, 289, 193},
	};

	Rect rcLeft[4] = {
		{224, 189, 237, 202},
		{240, 191, 255, 204},
		{256, 192, 272, 205},
		{272, 193, 289, 210},
	};

	if (bul->direct == 0)
		bul->rect = rcLeft[level-1];
	else
		bul->rect = rcRight[level-1];
}

void ActBullet_FireMine(BULLET *bul)
{
	bool bBreak;

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		SetBullet(Bullets::HELLSWEEP_EXPLOSION, bul->x, bul->y, 0);
		return;
	}

	if (bul->count1 > 10){
		bul->damage = 0;
	}

	bBreak = false;
	if (bul->flag & 2 && bul->flag & 8)
		bBreak = true;
	if (bul->flag & 1 && bul->flag & 4)
		bBreak = true;
	if (bul->life < 100)
		bBreak = true;

	if (bul->direct == 0 && bul->flag & 1){
		bul->direct = 2;
	}
	if (bul->direct == 2 && bul->flag & 4){
		bul->direct = 0;
	}

	if (bBreak)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 2, 0);
		SetBullet(Bullets::HELLSWEEP_EXPLOSION, bul->x, bul->y, 0);
		PlaySoundObject(28, SOUND_MODE_PLAY);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x350;
				break;

			case 1:
				bul->xm = gMC.xm;

				if (gMC.xm < 0)
					bul->direct = 0;
				else
					bul->direct = 2;

				if (gMC.direct == 0)
					bul->xm -= 0xA0;
				else
					bul->xm += 0xA0;

				bul->ym = -0x5FF;
				break;

			case 2:
				bul->xm = 0x350;
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
	}
	else
	{
		if (bul->flag & 8){
			bul->ym = -0x400;
		}
		else if (bul->flag & 1){
			bul->xm = 0x350;
		}
		else if (bul->flag & 4){
			bul->xm = -0x350;
		}

		bul->ym += 40;
		if (bul->ym > 0x3FF)
			bul->ym = 0x3FF;

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

	++bul->ani_no;
	if (bul->ani_no > 5)
		bul->ani_no = 0;

		if (bul->direct == 0)
			bul->rect = rect_left1[(bul->ani_no+1)/2];
		else
			bul->rect = rect_right1[(bul->ani_no+1)/2];
}

void ActBullet_Flamings(BULLET *bul)
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
	bul->iframes = 15;

	bul->xm *= 0.975;

	bul->x += bul->xm;
	bul->y += bul->ym;

	bul->opacity *= 0.995;


	if (++bul->count1 > bul->life_count)
		bul->cond = 0;
	else if (bul->count1 > 25)
		bul->damage = 2;
}

void RegisterHellsweep(){
    gBulletTable.insert({Bullets::HELLSWEEP_LVL_1, {
        {12, 100, 48, 52, 14, 12, 5, 3, {8, 8, 8, 8}, 1, 1},
        [](BULLET* bul){ ActBullet_Hellsweep(bul, 1); }
    }});
    gBulletTable.insert({Bullets::HELLSWEEP_LVL_2, {
        {19, 100, 38, 52, 16, 14, 5, 3, {10, 10, 10, 10}, 1, 1},
        [](BULLET* bul){ ActBullet_Hellsweep(bul, 2); }
    }});
    gBulletTable.insert({Bullets::HELLSWEEP_LVL_3, {
        {25, 100, 34, 52, 17, 17, 5, 3, {12, 12, 12, 12}, 1, 1},
        [](BULLET* bul){ ActBullet_Hellsweep(bul, 3); }
    }});
    gBulletTable.insert({Bullets::HELLSWEEP_LVL_4, {
        {32, 100, 28, 52, 18, 18, 5, 3, {12, 12, 12, 12}, 1, 1},
        [](BULLET* bul){ ActBullet_Hellsweep(bul, 4); }
    }});

    gBulletTable.insert({Bullets::HELLSWEEP_FIREMINE, {
        {-1, 100, 75, 8, 8, 16, 4, 2, {8, 8, 8, 8}, 0.8, 1},
        ActBullet_FireMine
    }});
    gBulletTable.insert({Bullets::HELLSWEEP_FLAME, {
        {-1, 100, 200, 8, 8, 8, 8, 8, {4, 2, 5, 2}, 0.5, 0.5},
        ActBullet_Flamings
    }});
}