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

void RegisterSnake(){
    gBulletTable.insert({Bullets::SNAKE_LVL_1, {
        {4, 1, 9, 36, 5, 5, 2, 2, {8, 8, 8, 8}, 0.25, 0.25},
        ActBullet_Frontia1
    }});
}