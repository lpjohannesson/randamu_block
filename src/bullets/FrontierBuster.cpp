#include "FrontierBuster.h"

#include "../Bullet.h"
#include "../Caret.h"
#include "../NpChar.h"
#include "../Sound.h"
#include "../Triangle.h"

void ActBullet_FrontierBuster1(BULLET *bul, int level)
{
	int baseSpeed = 0x1000;
	int inaccuracy = 0x200;
	if (level == 3){
		baseSpeed *= 0.85;
		inaccuracy *= 3.5f;
		bul->local_iframes = 3;
	}

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -baseSpeed;
				bul->ym = Random(-inaccuracy, inaccuracy);
				break;
			case 1:
				bul->ym = -baseSpeed;
				bul->xm = Random(-inaccuracy, inaccuracy);
				break;
			case 2:
				bul->xm = baseSpeed;
				bul->ym = Random(-inaccuracy, inaccuracy);
				break;
			case 3:
				bul->ym = baseSpeed;
				bul->xm = Random(-inaccuracy, inaccuracy);
				break;
		}
	}
	else
	{
		bul->x += bul->xm;
		bul->y += bul->ym;

		if (bul->flag & 0xD)
			PlaySoundObject(34, SOUND_MODE_PLAY);
	}

	Rect rect_left1[4] = {
		{128, 176, 144, 192},
		{144, 176, 160, 192},
		{160, 176, 176, 192},
		{176, 176, 192, 192},
	};

	Rect rect_right1[4] = {
		{128, 192, 144, 208},
		{144, 192, 160, 208},
		{160, 192, 176, 208},
		{176, 192, 192, 208},
	};

	++bul->ani_no;
	if (bul->ani_no > 3)
		bul->ani_no = 0;

	if (bul->direct == 0)
		bul->rect = rect_left1[bul->ani_no];
	else
		bul->rect = rect_right1[bul->ani_no];

	SetNpChar(129, bul->x, bul->y, 0, -0x200, std::max(0, bul->ani_no-1) + 3, NULL, 0x100);
}

void ActBullet_FrontierBuster2(BULLET *bul)
{
	Rect rect[3] = {
		{192, 16, 208, 32},
		{208, 16, 224, 32},
		{224, 16, 240, 32}
	};

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}
	bul->iframes = 30;

	if (bul->act_no == 0)
	{
		bul->ani_no = 0;
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x380;
				break;
			case 1:
				bul->ym = -0x380;
				break;
			case 2:
				bul->xm = 0x380;
				break;
			case 3:
				bul->ym = 0x380;
				break;
		}
	}
	else
	{
		switch (bul->direct)
		{
			case 0:
				bul->xm += -0x60;
				if (bul->xm < -0x1400){
					bul->xm = -0x1400;
					SetCaret(bul->x + Random(-bul->view.front, bul->view.back),
						bul->y + Random(-bul->view.top, bul->view.bottom), 13, 0);
				}
				break;
			case 1:
				bul->ym += -0x60;
				if (bul->ym < -0x1400){
					bul->ym = -0x1400;
					SetCaret(bul->x + Random(-bul->view.front, bul->view.back),
						bul->y + Random(-bul->view.top, bul->view.bottom), 13, 0);
				}
				break;
			case 2:
				bul->xm += 0x60;
				if (bul->xm > 0x1400){
					bul->xm = 0x1400;
					SetCaret(bul->x + Random(-bul->view.front, bul->view.back),
						bul->y + Random(-bul->view.top, bul->view.bottom), 13, 0);
				}
				break;
			case 3:
				bul->ym += 0x60;
				if (bul->ym > 0x1400){
					bul->ym = 0x1400;
					SetCaret(bul->x + Random(-bul->view.front, bul->view.back),
						bul->y + Random(-bul->view.top, bul->view.bottom), 13, 0);
				}
				break;
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

	bul->rect = rect[bul->ani_no];
}

void ActBullet_HomingFrontierBuster(BULLET *bul)
{
	if (bul->npc == NULL){
		if (++bul->count1 > bul->life_count)
		{
			bul->cond = 0;
			SetCaret(bul->x, bul->y, 3, 0);
			return;
		}
	}

	bul->npc = GetClosest(bul->x, bul->y, 60_pix);

	if (bul->act_no == 0){
		bul->act_no = 1;

		// Set speed
		switch (bul->direct)
		{
		case 0:
			bul->xm = -0xB00;
			break;
		case 1:
			bul->ym = -0xB00;
			break;
		case 2:
			bul->xm = 0xB00;
			break;
		case 3:
			bul->ym = 0xB00;
			break;
		}
	} else {
		if (bul->npc != NULL){
			unsigned char deg = GetArktan(bul->x - bul->npc->x, bul->y - bul->npc->y);
			bul->ym = GetSin(deg) * 6;
			bul->xm = GetCos(deg) * 6;
		}
	}

	bul->x += bul->xm;
	bul->y += bul->ym;

	++bul->ani_no;
	if (bul->ani_no > 2)
		bul->ani_no = 0;

	SetNpChar(129, bul->x, bul->y, 0, 0, bul->ani_no + 6, NULL, 0x800 + bul->ani_no*20);
}

/// @todo When shot quickly enough, this can be used as actual photoviscerator, dedicate a new weapon for that. 
void ActBullet_HomingFrontierBuster2(BULLET *bul)
{
	if (bul->npc == NULL){
		if (++bul->count1 > bul->life_count)
		{
			bul->cond = 0;
			SetCaret(bul->x, bul->y, 3, 0);
			return;
		}
	}

	bul->npc = GetClosest(bul->x, bul->y, 55_pix);
	bul->local_iframes = 5;
	int inaccuracy = 0x260;

	if (bul->act_no == 0){
		bul->act_no = 1;

		// Set speed
		switch (bul->direct)
		{
		case 0:
			bul->xm = -0x1000;
			bul->ym = Random(-inaccuracy, inaccuracy);
			break;
		case 1:
			bul->ym = -0x1000;
			bul->xm = Random(-inaccuracy, inaccuracy);
			break;
		case 2:
			bul->xm = 0x1000;
			bul->ym = Random(-inaccuracy, inaccuracy);
			break;
		case 3:
			bul->ym = 0x1000;
			bul->xm = Random(-inaccuracy, inaccuracy);
			break;
		}
	} else {
		if (bul->npc != NULL){
			unsigned char deg = GetArktan(bul->x - bul->npc->x, bul->y - bul->npc->y);
			bul->ym = GetSin(deg) * 8;
			bul->xm = GetCos(deg) * 8;
			bul->bbits = 4;
		}
	}

	bul->x += bul->xm;
	bul->y += bul->ym;

	++bul->ani_no;
	if (bul->ani_no > 2)
		bul->ani_no = 0;

	SetNpChar(129, bul->x, bul->y, 0, 0, bul->ani_no + 6, NULL, 0x800 + bul->ani_no*10);
}

void RegisterFrontierBuster(){
    gBulletTable.insert({Bullets::FRONTIER_BUSTER_LVL_1, {
        {4, 1, 100, 32, 8, 16, 4, 2, {8, 8, 8, 8}, 0.8, 1},
        [](BULLET* bul){ ActBullet_FrontierBuster1(bul, 1); }
    }});
    gBulletTable.insert({Bullets::FRONTIER_BUSTER_LVL_2, {
        {1, 3, 120, 32, 4, 4, 2, 2, {8, 8, 8, 8}, 0.5, 0.5},
        ActBullet_FrontierBuster2
    }});
    gBulletTable.insert({Bullets::FRONTIER_BUSTER_LVL_3, {
        {4, 100, 30, 36, 8, 16, 4, 2, {8, 8, 8, 8}, 0.8, 1},
        [](BULLET* bul){ ActBullet_FrontierBuster1(bul, 3); }
    }});
    gBulletTable.insert({Bullets::FRONTIER_BUSTER_LVL_4, {
        {1, 100, 60, 45, 10, 10, 1, 1, {12, 12, 12, 12}, 0.1, 0.1},
        ActBullet_HomingFrontierBuster2
    }});

    gBulletTable.insert({Bullets::FRONTIER_BUSTER_LVL_1_HOMING, {
        {12, 1, 100, 32, 10, 10, 4, 4, {12, 12, 12, 12}, 0.1, 0.1},
        ActBullet_HomingFrontierBuster
    }});
}