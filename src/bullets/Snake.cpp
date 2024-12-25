#include "Snake.h"

#include "..\Bullet.h"
#include "..\Caret.h"

void ActBullet_Frontia1(BULLET *bul)
{
	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->ani_no = Random(0, 2);
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0xF00;
				break;
			case 1:
				bul->ym = -0xF00;
				break;
			case 2:
				bul->xm = 0xF00;
				break;
			case 3:
				bul->ym = 0xF00;
				break;
		}
	}
	else
	{
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	if (++bul->ani_wait > 0)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 0;

	Rect rcLeft[4] = {
		{136, 80, 152, 80},
		{120, 80, 136, 96},
		{136, 64, 152, 80},
		{120, 64, 136, 80},
	};

	Rect rcRight[4] = {
		{120, 64, 136, 80},
		{136, 64, 152, 80},
		{120, 80, 136, 96},
		{136, 80, 152, 80},
	};

	if (bul->direct == 0)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void ActBullet_Frontia2(BULLET *bul, int level)
{
	static unsigned int inc;

	bul->local_iframes = 15;

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->ani_no = Random(0, 2);
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x500;
				break;
			case 1:
				bul->ym = -0x500;
				break;
			case 2:
				bul->xm = 0x500;
				break;
			case 3:
				bul->ym = 0x500;
				break;
		}

		++inc;

		switch (bul->direct)
		{
			case 0:
			case 2:
				if (inc % 2)
					bul->ym = 0x600;
				else
					bul->ym = -0x600;

				break;

			case 1:
			case 3:
				if (inc % 2)
					bul->xm = 0x600;
				else
					bul->xm = -0x600;

				break;
		}
	}
	else
	{
		switch (bul->direct)
		{
			case 0:
				bul->xm -= 0x140;
				break;
			case 1:
				bul->ym -= 0x140;
				break;
			case 2:
				bul->xm += 0x140;
				break;
			case 3:
				bul->ym += 0x140;
				break;
		}

		switch (bul->direct)
		{
			case 0:
			case 2:
				if (bul->count1 % 5 == 2)
				{
					if (bul->ym < 0)
						bul->ym = 0x600;
					else
						bul->ym = -0x600;
				}

				break;

			case 1u:
			case 3u:
				if (bul->count1 % 5 == 2)
				{
					if (bul->xm < 0)
						bul->xm = 0x600;
					else
						bul->xm = -0x600;
				}

				break;
		}
		if (bul->count1 % 4 == 0 && level == 4){
		int lol = Random(0, 2);
		for (int i = 0; i < lol; i++){
			BULLET* bully = SetBullet(56, bul->x, bul->y);
			if (bully != NULL){
				bully->direct = 0;
				bully->xm = Random(-1700, 1700);
				bully->ym = Random(-1000, 200);
				}
			}
		}

		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	if (++bul->ani_wait > 0)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 2)
		bul->ani_no = 0;

	Rect rect[3] = {
		{192, 16, 208, 32},
		{208, 16, 224, 32},
		{224, 16, 240, 32},
	};

	bul->rect = rect[bul->ani_no];

	if (level == 2)
		SetNpChar(129, bul->x, bul->y, 0, -0x200, bul->ani_no, NULL, 0x100);
	else
		SetNpChar(129, bul->x, bul->y, 0, -0x200, bul->ani_no + 3, NULL, 0x100);
}

void RegisterSnake(){
    gBulletTable.insert({Bullets::SNAKE_LVL_1, {
        {4, 1, 9, 36, 5, 5, 2, 2, {8, 8, 8, 8}, 0.25, 0.25},
        ActBullet_Frontia1
    }});
    gBulletTable.insert({Bullets::SNAKE_LVL_2, {
        {5, 2, 12, 36, 5, 5, 2, 2, {8, 8, 8, 8}, 0.25, 0.25},
        [](BULLET* bul){ ActBullet_Frontia2(bul, 2); }
    }});
    gBulletTable.insert({Bullets::SNAKE_LVL_3, {
        {6, 3, 15, 36, 5, 5, 2, 2, {8, 8, 8, 8}, 0.25, 0.25},
        [](BULLET* bul){ ActBullet_Frontia2(bul, 3); }
    }});
    gBulletTable.insert({Bullets::SNAKE_LVL_4, {
        {6, 4, 19, 36, 5, 5, 2, 2, {8, 8, 8, 8}, 1, 1},
        [](BULLET* bul){ ActBullet_Frontia2(bul, 4); }
    }});

    gBulletTable.insert({Bullets::MANNAN_ATTACK_VFX, {
        {-1, 2, 8, 36, 5, 5, 2, 2, {8, 8, 8, 8}, 0.25, 0.25},
        [](BULLET* bul){ ActBullet_Frontia2(bul, 3); }
    }});
}