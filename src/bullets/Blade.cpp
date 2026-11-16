#include "Blade.h"

#include "../Bullet.h"
#include "../Caret.h"
#include "../MyChar.h"
#include "../Sound.h"

void ActBullet_WeightBlade(BULLET *bul)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->count1 == 3)
		bul->bbits &= ~4;

	if (bul->count1 % 7 == 1)
		PlaySoundObject(106, SOUND_MODE_PLAY);

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x800;
				break;
			case 1:
				bul->ym = -0x800;
				break;
			case 2:
				bul->xm = 0x800;
				break;
			case 3:
				bul->ym = 0x800;
				break;
		}
	}
	else
	{
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	Rect rcLeft[4] = {
		{160, 48, 184, 72},
		{184, 48, 208, 72},
		{208, 48, 232, 72},
		{232, 48, 256, 72},
	};

	Rect rcRight[4] = {
		{160, 72, 184, 96},
		{184, 72, 208, 96},
		{208, 72, 232, 96},
		{232, 72, 256, 96},
	};

	if (++bul->ani_wait > 1)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 0;

	if (bul->direct == 0)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void ActBullet_Sword(BULLET *bul, int level)
{
	Rect rcLeft[4] = {
		{160, 48, 184, 72},
		{184, 48, 208, 72},
		{208, 48, 232, 72},
		{232, 48, 256, 72},
	};

	Rect rcRight[4] = {
		{160, 72, 184, 96},
		{184, 72, 208, 96},
		{208, 72, 232, 96},
		{232, 72, 256, 96},
	};

	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;
			bul->xm = 0;
			bul->ym = 0;
			// Fallthrough
		case 1:
			switch (bul->direct)
			{
				case 0:
					bul->xm = -0x900;
					break;
				case 1:
					bul->ym = -0x900;
					break;
				case 2:
					bul->xm = 0x900;
					break;
				case 3:
					bul->ym = 0x900;
					break;
			}
			if (level == 3){
				bul->xm *= 1.55;
				bul->ym *= 1.55;
			}

			if (bul->life < 97)
			{
				bul->act_no = 2;
				bul->ani_no = 1;
				bul->damage = -1;
				bul->act_wait = 0;
			}

			if (++bul->act_wait % 4 == 1)
			{
				PlaySoundObject(106, SOUND_MODE_PLAY);

				if (++bul->count1 % 2){
					SetBullet(Bullets::BLADE_SLASH, bul->x, bul->y, 0);
				}
				else
					SetBullet(Bullets::BLADE_SLASH, bul->x, bul->y, 2);

				if (level == 2 && Random(0, 3) <= 2){
					if (++bul->count1 % 2){
						SetBullet(Bullets::BLADE_SLASH, bul->x, bul->y, 0);
					}
					else
						SetBullet(Bullets::BLADE_SLASH, bul->x, bul->y, 2);
				}
			}

			if (++bul->count1 == 5)
				bul->bbits &= ~4;

			if (bul->count1 > bul->life_count)
			{
				bul->cond = 0;
				SetCaret(bul->x, bul->y, 3, 0);
				return;
			}

			break;

		case 2:
			bul->xm = 0;
			bul->ym = 0;
			++bul->act_wait;
			int rngRoll = Random(0, 2);
			if (level >= 2){
				int rngRoll2 = Random(0, 2);
				if (rngRoll2 > rngRoll) rngRoll = rngRoll2;
			}

			if (rngRoll == 2)
			{
				PlaySoundObject(106, SOUND_MODE_PLAY);

				if (Random(0, 1) % 2)
					SetBullet(Bullets::BLADE_SLASH, bul->x + (Random(-0x15, 0x15) * 0x200), bul->y + (Random(-0x15, 0x15) * 0x200), 0);
				else
					SetBullet(Bullets::BLADE_SLASH, bul->x + (Random(-0x15, 0x15) * 0x200), bul->y + (Random(-0x15, 0x15) * 0x200), 2);
			}

			if (level >= 2 && Random(0, 3) <= 1){
				int rngRoll = Random(-1, 1);

			if (rngRoll == 0)
			{
				PlaySoundObject(106, SOUND_MODE_PLAY);

				if (Random(0, 1) % 2)
					SetBullet(Bullets::BLADE_SLASH, bul->x + (Random(-0x15, 0x15) * 0x200), bul->y + (Random(-0x15, 0x15) * 0x200), 0);
				else
					SetBullet(Bullets::BLADE_SLASH, bul->x + (Random(-0x15, 0x15) * 0x200), bul->y + (Random(-0x15, 0x15) * 0x200), 2);
			}
			}

			if (bul->act_wait > 95)
				bul->cond = 0;
	}

	bul->x += bul->xm;
	bul->y += bul->ym;

	if (gMC.dashDuration > 0){
		bul->x = gMC.x;
		bul->y = gMC.y;
	}
	
	if (++bul->ani_wait > 1)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 0;

	if (bul->direct == 0)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void RegisterBlade(){
    gBulletTable.insert({Bullets::BLADE_LVL_1, {
        {2, 100, 32, 36, 8, 8, 4, 4, {12, 12, 12, 12}, 1.2, 1.2},
        [](BULLET* bul){ ActBullet_Sword(bul, 1); }
    }});
    gBulletTable.insert({Bullets::BLADE_LVL_2, {
        {3, 100, 37, 36, 8, 8, 4, 4, {12, 12, 12, 12}, 1.66, 1.66},
        [](BULLET* bul){ ActBullet_Sword(bul, 2); }
    }});
    gBulletTable.insert({Bullets::BLADE_LVL_4, {
        {3, 100, 35, 36, 6, 6, 4, 4, {12, 12, 12, 12}, 1, 1},
        [](BULLET* bul){ ActBullet_Sword(bul, 3); }
    }});

    gBulletTable.insert({Bullets::HEAVY_WEIGHT_BLADES, {
        {10, 3, 1, 32, 10, 10, 4, 2, {12, 12, 12, 12}, 0.1, 0.1},
        ActBullet_WeightBlade
    }});
}