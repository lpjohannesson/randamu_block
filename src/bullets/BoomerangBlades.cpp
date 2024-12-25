#include "BoomerangBlades.h"

#include "..\Bullet.h"
#include "..\Map.h"
#include "..\MyChar.h"
#include "..\Sound.h"
#include "..\Triangle.h"

void ActBullet_BoomerangSword1(BULLET *bul)
{
	bul->local_iframes = 20;

	if (++bul->count1 > bul->life_count && bul->act_no == 1)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}
	else if (bul->life < 100 && bul->act_no == 1)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}
	else if (bul->flag & 0xF && bul->act_no == 1)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}

	if (bul->act_no == 1){
		if (bul->code_bullet == 55)
			bul->damage = 5;
		else
			bul->damage = 4;
	}

	if (bul->count1 == 3)
		bul->bbits &= ~4;

	if (bul->count1 % 25 == 1)
		PlaySoundObject(34, SOUND_MODE_PLAY);

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x700;
				break;
			case 1:
				bul->ym = -0x700;
				break;
			case 2:
				bul->xm = 0x700;
				break;
			case 3:
				bul->ym = 0x700;
				break;
			case 4:
				bul->xm = -0x700;
				bul->ym = -0x700;
				break;
			case 5:
				bul->ym = -0x700;
				bul->xm = 0x700;
				break;
			case 6:
				bul->xm = 0x700;
				bul->ym = 0x700;
				break;
			case 7:
				bul->ym = 0x700;
				bul->xm = -0x700;
				break;
		}
	}
	else
	{
		if (bul->act_no == 2){
			if (bul->x > gMC.x - 0x200*8 && bul->x < gMC.x + 0x200*8 &&
					 bul->y > gMC.y - 0x200*8 && bul->y < gMC.y + 0x200*8){
				bul->cond = 0;
				return;
			}
			int deg = GetArktan(bul->x - gMC.x, bul->y - gMC.y);
			bul->ym = GetSin(deg) * 2.9;
			bul->xm = GetCos(deg) * 2.9;
			if (bul->code_bullet == 55){
				bul->ym = GetSin(deg) * 6.96;
				bul->xm = GetCos(deg) * 6.96;
			}
		}
			bul->x += bul->xm;
			bul->y += bul->ym;
	}

	Rect rcLeft[4] = {
		{0, 48, 16, 64},
		{16, 48, 32, 64},
		{32, 48, 48, 64},
		{48, 48, 64, 64},
	};

	Rect rcRight[4] = {
		{64, 48, 80, 64},
		{80, 48, 96, 64},
		{96, 48, 112, 64},
		{112, 48, 128, 64},
	};

	if (++bul->ani_wait > 1)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 0;

	if (bul->direct == 0 || bul->direct == 4)
		bul->rect = rcLeft[bul->ani_no];
	else
		bul->rect = rcRight[bul->ani_no];
}

void ActBullet_BoomerangSword2(BULLET *bul)
{
	bul->iframes = 20;

	++bul->count1;
	if (bul->count1 > bul->life_count && bul->act_no < 2)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}
	if (bul->life < 100 && bul->act_no < 2)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}
	else if (bul->flag & 0xF && bul->act_no == 1)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}

	bul->damage = 7 + 8 * std::min(1.0f, float(bul->count1)/float(bul->life_count));

	if (bul->count1 == 3)
		bul->bbits &= ~4;

	if (bul->count1 % 7 == 1)
		PlaySoundObject(106, SOUND_MODE_PLAY);

	if (bul->act_no == 0)
	{
		bul->act_no = 1;
		bul->count2 = 0;

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x1240;
				break;
			case 1:
				bul->ym = -0x1240;
				break;
			case 2:
				bul->xm = 0x1240;
				break;
			case 3:
				bul->ym = 0x1240;
				break;
		}
	}
	else
	{
		if (bul->act_no == 2){
			if (bul->x > gMC.x - 0x200*8 && bul->x < gMC.x + 0x200*8 &&
					 bul->y > gMC.y - 0x200*8 && bul->y < gMC.y + 0x200*8){
				bul->cond = 0;
				return;
			}
			if (bul->count2 == 1 && bul->count1 % 3 == 0){
				BULLET* bully = SetBullet(Bullets::BOOMERANG_BLADE_LVL_2_FLAME, bul->x, bul->y);
				if (bully != NULL){
					bully->direct = 0;
					bully->xm = Random(-1700, 1700);
					bully->ym = Random(-1000, 200);
				}
			}
			if (bul->count2 == 0 && bul->life == 100 && !(bul->flag & 0xF)){
				bul->count2 = 1;
				int lol = Random(5, 7);
				for (int i = 0; i < lol; i++){
					BULLET* bully = SetBullet(Bullets::BOOMERANG_BLADE_LVL_2_FLAME, bul->x, bul->y);
					if (bully != NULL){
					bully->direct = 0;
					bully->xm = Random(-1700, 1700);
					bully->ym = Random(-1000, 200);
					}
				}
			}
			
			int deg = GetArktan(bul->x - gMC.x, bul->y - gMC.y);
			bul->ym = GetSin(deg) * 12;
			bul->xm = GetCos(deg) * 12;
		}
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

void ActBullet_BoomerangSword3(BULLET *bul)
{
	static unsigned int inc;

	bul->local_iframes = 20;

	++bul->count1;
	if (bul->count1 > bul->life_count && bul->act_no < 2)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}
	if (bul->life < 100 && bul->act_no < 2)
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}

	bul->damage = 0;

	if (bul->count1 == 3)
		bul->bbits &= ~4;
	if (bul->count1 % 7 == 1)
		PlaySoundObject(106, SOUND_MODE_PLAY);
	if (bul->count1 % 10 > 6 && bul->count1 % 10 < 9){
			if (bul->count1 % 10 == 7){
				SetBullet(Bullets::BLADE_SLASH, bul->x, bul->y, 0);
			}
			else{
				SetBullet(Bullets::BLADE_SLASH, bul->x, bul->y, 2);
			}
	}

	if (bul->act_no == 0)
	{
		bul->act_no = 1;
		inc++;

		if (inc % 2){
			bul->count2 = 1;
		} else {
			bul->count2 = 0;
		}

		switch (bul->direct)
		{
			case 0:
				bul->xm = -0x999;
				break;
			case 1:
				bul->ym = -0x999;
				break;
			case 2:
				bul->xm = 0x999;
				break;
			case 3:
				bul->ym = 0x999;
				break;
		}
	}
	else
	{
		if (bul->act_no == 2){
			SetBullet(Bullets::BOOMERANG_BLADE_LVL_3_SPLIT, bul->x, bul->y, bul->count2 == 1 ? 4 : 0);
			SetBullet(Bullets::BOOMERANG_BLADE_LVL_3_SPLIT, bul->x, bul->y, bul->count2 == 1 ? 5 : 1);
			SetBullet(Bullets::BOOMERANG_BLADE_LVL_3_SPLIT, bul->x, bul->y, bul->count2 == 1 ? 6 : 2);
			SetBullet(Bullets::BOOMERANG_BLADE_LVL_3_SPLIT, bul->x, bul->y, bul->count2 == 1 ? 7 : 3);
			bul->cond = 0;
			return;
			// if (bul->x > gMC.x - 0x200*8 && bul->x < gMC.x + 0x200*8 &&
			// 		 bul->y > gMC.y - 0x200*8 && bul->y < gMC.y + 0x200*8){
			// 	bul->cond = 0;
			// 	return;
			// }
			// int deg = GetArktan(bul->x - gMC.x, bul->y - gMC.y);
			// bul->ym = GetSin(deg) * 8;
			// bul->xm = GetCos(deg) * 8;
		}
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

void ActBullet_BoomerangSword4(BULLET *bul)
{
	bul->iframes = 20;

	++bul->count1;
	if (bul->count1 > bul->life_count && (bul->act_no == 3 || bul->act_no == 1))
	{
		bul->act_no = 2;
		bul->count1 = 0;
	}

	if (bul->life < 100 && bul->count2 < 12)
	{
		bul->count2++;
		bul->act_no = 3;
		bul->life = 100;
		bul->count1 = 0;
	}
	else if (bul->flag & 0xF && bul->count2 < 12)
	{
		bul->count2++;
		bul->act_no = 3;
		bul->count1 = 0;
	}

	bul->damage = 8;

	if (bul->count1 == 3)
		bul->bbits &= ~4;

	if (bul->count1 % 7 == 1)
		PlaySoundObject(106, SOUND_MODE_PLAY);

	if (bul->act_no == 0)
	{
		bul->act_no = 1;
		bul->count2 = 0;

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
	}
	else
	{
		if (bul->act_no == 3){
			bul->act_no = 1;
			int deg = GetArktan(bul->x - (bul->x + Random(-0x200 * 4, 0x200 * 4)), 
			bul->y - (bul->y + Random(-0x200 * 4, 0x200 * 4)));
			deg += Random(-8, 8);
			bul->ym = GetSin(deg) * 10;
			bul->xm = GetCos(deg) * 10;
		}
		if (bul->act_no == 2){
			if ((bul->x > gMC.x - 0x200*8 && bul->x < gMC.x + 0x200*8 &&
					 bul->y > gMC.y - 0x200*8 && bul->y < gMC.y + 0x200*8)){
				bul->cond = 0;
				return;
			}
			int deg = GetArktan(bul->x - gMC.x, bul->y - gMC.y);
			bul->ym = GetSin(deg) * 10;
			bul->xm = GetCos(deg) * 10;
		}
		if (bul->count1 % 4 == 0){
			SetBullet(Bullets::BLADE_SLASH, bul->x, bul->y, 0);
			SetBullet(Bullets::BLADE_SLASH, bul->x, bul->y, 2);
		}
			
		bul->x += bul->xm;
		bul->y += bul->ym;
		if (Distance(bul->x, bul->y, gMC.x, gMC.y) > 8 * 0x2000 || bul->count2 > 12){
			bul->bbits |= 4;
			bul->act_no = 2;
		}
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

void ActBullet_BoomerangFlame(BULLET *bul)
{
	Rect rc_h_lv1[3] = {
		{0, 144+16, 8, 160+16},
		{8, 144+16, 16, 160+16},
		{16, 144+16, 24, 160+16},
	};

	if (++bul->ani_no > 2)
		bul->ani_no = 0;

	bul->rect = rc_h_lv1[bul->ani_no];

	if (bul->flag & 8)
		bul->ym = -0x120;
	
	bul->xm *= 0.95;

	if (bul->ym < 0)
		bul->ym += 30;
	else
		bul->ym += 90;
	if (bul->ym > 0x450)
		bul->ym = 0x450;

	bul->y += bul->ym;
	bul->x += bul->xm;

	bul->opacity -= 0.02f;

	if (++bul->count1 > bul->life_count){
		bul->cond = 0;
		// SetCaret(bul->x, bul->y, 3, 0);
	}
}

void RegisterBoomerangBlades(){
    gBulletTable.insert({Bullets::BOOMERANG_BLADE_LVL_1, {
        {4, 100, 50, 44, 8, 8, 4, 2, {8, 8, 8, 8}, 0.75, 0.75},
        ActBullet_BoomerangSword1
    }});
    gBulletTable.insert({Bullets::BOOMERANG_BLADE_LVL_2, {
        {8, 100, 13, 44, 10, 10, 4, 2, {12, 12, 12, 12}, 0.75, 0.75},
        ActBullet_BoomerangSword2
    }});
    gBulletTable.insert({Bullets::BOOMERANG_BLADE_LVL_3, {
        {7, 100, 30, 36, 10, 10, 4, 2, {12, 12, 12, 12}, 0.75, 0.75},
        ActBullet_BoomerangSword3
    }});
    gBulletTable.insert({Bullets::BOOMERANG_BLADE_LVL_4, {
        {6, 100, 30, 44, 10, 10, 3, 3, {12, 12, 12, 12}, 0.75, 0.75},
        ActBullet_BoomerangSword4
    }});

    gBulletTable.insert({Bullets::BOOMERANG_BLADE_LVL_2_FLAME, {
        {3, 1, 40, 8, 8, 8, 8, 8, {4, 2, 5, 2}, 0.5, 0.5},
        ActBullet_BoomerangFlame
    }});
    gBulletTable.insert({Bullets::BOOMERANG_BLADE_LVL_3_SPLIT, {
        {-1, 100, 19, 44, 8, 8, 4, 2, {8, 8, 8, 8}, 0.75, 0.75},
        ActBullet_BoomerangSword1
    }});
}