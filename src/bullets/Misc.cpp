#include "Misc.h"

#include "..\Boss.h"
#include "..\BossLife.h"
#include "..\Bullet.h"
#include "..\Caret.h"
#include "..\Cooldown.h"
#include "..\Flags.h"
#include "..\Frame.h"
#include "..\KeyControl.h"
#include "..\Map.h"
#include "..\MyChar.h"
#include "..\MycParam.h"
#include "..\Sound.h"
#include "..\TextScr.h"
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
						BULLET *bullet = SetBullet(Bullets::ENERGY_PROJECTILE, bul->x, bul->y);
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

void ActBullet_EnergyShot(BULLET *bul)
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

		int deg = GetArktan(bul->x - bul->tgt_x, bul->y - bul->tgt_y);
		if (bul->direct != 0)
			deg = GetArktan(bul->tgt_x - bul->x, bul->tgt_y - bul->y);
		deg += (unsigned char)Random(-6, 6);
		bul->ym = GetSin(deg) * 8;
		bul->xm = GetCos(deg) * 8;
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
	if (bul->count1 % 2 == 0)
	SetCaret(bul->x, bul->y, 3, 0);

	if (bul->ani_no > 2)
		bul->ani_no = 0;

	Rect rcLeft[3] = {
		{130, 144, 142, 158},
		{147, 147, 157, 157},
		{162, 146, 174, 159}
	};
		
	bul->rect = rcLeft[bul->ani_no];
}

void ActBullet_WhimsicalStar(BULLET *bul)
{
	Rect rcLeft[] = {
		{160, 48, 184, 72},
		{184, 48, 208, 72},
		{208, 48, 232, 72},
		{232, 48, 256, 72}
	};

	Rect rcRight[] = {
		{232, 72, 256, 96},
		{208, 72, 232, 96},
		{184, 72, 208, 96},
		{160, 72, 184, 96}
	};

	if (bul->npc != NULL && (!(bul->npc->cond & 0x80) || bul->npc->cond & 8 || bul->npc->life <= 0)){
			bul->npc = NULL;
			return;
	}
	bul->life = 100;
	int fx, fy, mx, my;
	if (gKeyTrg & KEY_MOUSE){
		PlaySoundObject(18);
		if (bul->direct == 0)
			bul->direct = 1;
		else
			bul->direct = 0;
	}

	float speedMod = 1.0f;
	if (bul->direct == 1)
		speedMod = 1.5f;

	if (bul->direct == 1){
		mx = gMC.x + gMC.xm + Random(-64_pix, 64_pix);
		my = gMC.y + gMC.ym + Random(-64_pix, 64_pix);
		if (bul->npc != NULL){
			mx = gMC.x + gMC.xm;
			my = gMC.y + gMC.ym;
		}
	} else {
		GetFramePosition(&fx, &fy);
		mx = fx + ScreenToSubpixelCoord(gMouseX);
		my = fy + ScreenToSubpixelCoord(gMouseY);
	}

	if (bul->npc == NULL)
	{
		bul->damage = 0;
		switch (bul->act_no)
		{
		default:
			bul->act_no = 1;
			bul->ani_no = 0;
			// Fallthrough
		case 1:
			bul->y += bul->ym;

			if (++bul->act_wait > 2)
				bul->act_no = 10;

			break;

		case 10:

			if (mx < bul->x)
				bul->xm -= 250 * speedMod;
			else
				bul->xm += 250 * speedMod;

			if (my < bul->y)
				bul->ym -= 250 * speedMod;
			else
				bul->ym += 250 * speedMod;
			if (bul->xm > 0x600 * speedMod)
				bul->xm = 0x600 * speedMod;
			if (bul->xm < -0x600 * speedMod)
				bul->xm = -0x600 * speedMod;

			if (bul->ym > 0x600 * speedMod)
				bul->ym = 0x600 * speedMod;
			if (bul->ym < -0x600 * speedMod)
				bul->ym = -0x600 * speedMod;

			bul->x += bul->xm;
			bul->y += bul->ym;

			for (int i = 0; i < NPC_MAX; i++)
			{
				if ((bul->x > gNPC[i].x - gNPC[i].view.front &&
					 bul->x < gNPC[i].x + gNPC[i].view.back &&
					 bul->y > gNPC[i].y - gNPC[i].view.top &&
					 bul->y < gNPC[i].y + gNPC[i].view.bottom) &&
					gNPC[i].damage == 0 && (gNPC[i].cond & 0x80))
				{
					if (gNPC[i].code_char == 1)
					{
						PlaySoundObject(14, SOUND_MODE_PLAY);
						AddExpMyChar(gNPC[i].exp);
						gNPC[i].cond = 0;
					}

					if (gNPC[i].code_char == 86)
					{
						if (gBL.flag == true){
							if (static_iframes[i][0] == 0){
								SetCaret((bul->x + gNPC[i].x) / 2, (bul->y + gNPC[i].y) / 2, 2, 2);
								PlaySoundObject(31, SOUND_MODE_PLAY);
								static_iframes[i][0] = 20;
							}
						} else {
							PlaySoundObject(42, SOUND_MODE_PLAY);
							AddBulletMyChar(gNPC[i].code_event, gNPC[i].exp);
							SetCaret(gNPC[i].x, gNPC[i].y, 3, 0);
							SetCaret(gNPC[i].x, gNPC[i].y, 18, 0);
							if (gMC.a == 10)
							{
								StartTextScript(305);
							}
							if (gMC.a == 20)
							{
								StartTextScript(303);
							}
							gNPC[i].cond = 0;
						}
					}

					if (gNPC[i].code_char == 87)
					{
						PlaySoundObject(20, SOUND_MODE_PLAY);
						AddLifeMyChar(gNPC[i].exp);
						gNPC[i].cond = 0;
					}
				}
			}

			break;
		}
	}
	else {
		int deg;
		if (Distance(mx, my, bul->x, bul->y) > 0x2000 * 8){
			bul->npc = NULL;
			return;
		}
		bul->iframes = 8;
		switch (bul->act_no)
		{
		default:
			bul->act_no = 20;
			//Fallthrough
		case 20:
			bul->act_no = 21;
			bul->act_wait = 0;
			bul->ani_no = 0;
			bul->ani_wait = 0;
			// Fallthrough
		case 21:
			// bul->xm = (bul->xm * 6) / 7;
			// bul->ym = (bul->ym * 6) / 7;

			if (++bul->act_wait > 10)
				bul->act_no = 30;

			break;

		case 30:
			bul->act_no = 31;
			bul->act_wait = 0;
			bul->ani_no = 2;
			// Fallthrough
		case 31:
			if (Distance(bul->x, bul->y, bul->npc->x, bul->npc->y) < 0x2000){
				bul->act_no = 34;
			} else {
				bul->act_no = 32;
			}

			break;

		case 32:
			// bul->act_no = 33;
			bul->act_wait = 0;

			bul->tgt_x = bul->npc->x;
			bul->tgt_y = bul->npc->y;

			if (bul->tgt_x < bul->x)
				bul->xm -= 200 * speedMod;
			else
				bul->xm += 200 * speedMod;

			if (bul->tgt_y < bul->y)
				bul->ym -= 200 * speedMod;
			else
				bul->ym += 200 * speedMod;
			if (bul->xm > 0x600 * speedMod)
				bul->xm = 0x600 * speedMod;
			if (bul->xm < -0x600 * speedMod)
				bul->xm = -0x600 * speedMod;

			if (bul->ym > 0x600 * speedMod)
				bul->ym = 0x600 * speedMod;
			if (bul->ym < -0x600 * speedMod)
				bul->ym = -0x600 * speedMod;
			if (Distance(bul->x, bul->y, bul->npc->x, bul->npc->y) < 0x2000){
				bul->act_no = 34;
			}
			break;

		case 34:
			bul->act_no = 35;
			bul->act_wait = 0;
			bul->damage = 3;

			bul->tgt_x = bul->npc->x;
			bul->tgt_y = bul->npc->y;

			deg = GetArktan(bul->x - bul->tgt_x, bul->y - bul->tgt_y);
			deg += Random(-16, 16);
			bul->ym = 1 * GetSin(deg) * speedMod;
			bul->xm = 1 * GetCos(deg) * speedMod;
			// Fallthrough
		case 35:
			if (++bul->act_wait > 2){
				bul->act_no = 20;
			}

			if (++bul->ani_wait > 1)
			{
				bul->ani_wait = 0;
				++bul->ani_no;
			}

			if (bul->ani_no > 3)
				bul->ani_no = 0;

			break;
		}

		bul->x += bul->xm;
		bul->y += bul->ym;
		if (!(bul->npc->cond & 0x80) || bul->npc->cond & 8 || bul->npc->life <= 0){
			bul->npc = NULL;
			return;
		}
	}

	SetCaret(bul->x, bul->y, 3, 0);
	SetCaret(bul->x, bul->y, 13, 0);
	SetCaret(bul->x, bul->y, 13, 0);
	SetCaret(bul->x, bul->y, 13, 0);
}

void ActBullet_Explosion(BULLET *bul, int level)
{
	if (bul->code_bullet == Bullets::MISSILE_EXPLOSION){
	gMC.unit = 0;
	gMC.shock = 30;
	}

	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;

			switch (level)
			{
				case 1:
					bul->act_wait = 7;
					break;
				case 2:
					bul->act_wait = 20;
					break;
				case 3:
					bul->act_wait = 10;
					break;
			}

			PlaySoundObject(44, SOUND_MODE_PLAY);
			// Fallthrough
		case 1:
			switch (level)
			{
				case 1:
					if (bul->act_wait % 3 == 0)
						SetDestroyNpCharUp(bul->x + (Random(-24, 24) * 0x200), bul->y + (Random(-24, 24) * 0x200), bul->enemyXL, 2);
					break;

				case 2:
					if (bul->act_wait % 5 == 0)
						SetDestroyNpCharUp(bul->x + (Random(-32, 32) * 0x200), bul->y + (Random(-32, 32) * 0x200), bul->enemyXL, 2);
					break;

				case 3:
					if (bul->act_wait % 3 == 0)
						SetDestroyNpCharUp(bul->x + (Random(-40, 40) * 0x200), bul->y + (Random(-40, 40) * 0x200), bul->enemyXL, 2);
					break;
			}

			if (--bul->act_wait < 0)
				bul->cond = 0;

			break;
	}
}

void ActBullet_SuperExplosion(BULLET *bul, int level)
{
	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;

			switch (level)
			{
				case 1:
					bul->act_wait = 35;
					break;
				case 2:
					bul->act_wait = 35;
					break;
				case 3:
					bul->act_wait = 6;
					break;
			}

			PlaySoundObject(44, SOUND_MODE_PLAY);
			// Fallthrough
		case 1:
			switch (level)
			{
				case 1:
					if (bul->act_wait % 3 == 0)
						SetDestroyNpCharUp(bul->x + (Random(-32, 32) * 0x200), bul->y + (Random(-32, 32) * 0x200), bul->enemyXL, 3);
					break;
				case 2:
					bul->x = gMC.x + gMC.xm * 2;
					bul->y = gMC.y + gMC.ym * 2;
					if (bul->act_wait % 3 == 0)
						SetDestroyNpCharUp(bul->x + (Random(-32, 32) * 0x200), bul->y + (Random(-32, 32) * 0x200), bul->enemyXL, 5);
					break;
				case 3:
					if (bul->act_wait % 3 == 0)
						SetDestroyNpCharUp(bul->x + (Random(-40, 40) * 0x200), bul->y + (Random(-40, 40) * 0x200), bul->enemyXL, 2);
					break;
			}

			if (--bul->act_wait < 0)
				bul->cond = 0;

			break;
	}
}

void ActBullet_Missile(BULLET *bul)
{
	bool bHit;

	static unsigned int inc;

	bHit = false;

	// if (bul->life != 10)
	// 	bHit = true;
	if (bul->flag & 0xF || (!(gMC.x > 32_pix && gMC.x < (gMap.width-3)*0x2000 && gMC.y > 32_pix && gMC.y < (gMap.length-3)*0x2000) && gMC.outerWallInit))
		bHit = true;
	if (bHit)
	{
		SetBullet(Bullets::MISSILE_EXPLOSION, bul->x, bul->y, 0);
		bul->cond = 0;
		gMC.direct = 2;
	}

	bul->life = 1000;
	if (++bul->count1 % 3 == 0){
		bul->damage = 1;
	} else {
		bul->damage = 0;
	}

	switch (bul->act_no)
	{
	case 0:
		bul->act_no = 1;

		switch (bul->direct)
		{
		case 0:
		case 2:
			bul->tgt_y = bul->y;
			break;
		case 1:
		case 3:
			bul->tgt_x = bul->x;
			break;
		}
		bul->ani_no = 0x120;
		// Fallthrough
	case 1:

		int max_move;

		if (gMC.flag & 0x100 && !(gMC.flag & (0x8000 | 0x4000 | 0x2000 | 0x1000)))
			max_move = gMC.physics_underwater.max_move * 2.1; // Underwater or in wind
		else
			max_move = gMC.physics_normal.max_move * 2.1; // Normal conditions

		if (gMC.cond == 0){
			SetBullet(Bullets::MISSILE_EXPLOSION, bul->x, bul->y, 0);
		bul->cond = 0;
		gMC.direct = 2;
		}

		if ((GetNPCFlag(Flags::LILY_CHARACTER) || GetNPCFlag(Flags::MAHIN_CHARACTER)) && gKey & gKeyJump){
			if (gKey & gKeyUp){
				bul->xm = 0;
				if (bul->ym > 0){
					bul->ym = 0;
				}
				bul->ym -= bul->ani_no * 0.8;
				bul->direct = 1;
			} else if (gKey & gKeyDown){
				bul->xm = 0;
				if (bul->ym < 0){
					bul->ym = 0;
				}
				bul->ym += bul->ani_no * 0.8;
				bul->direct = 3;
			} else if (gKey & gKeyLeft){
				if (bul->xm > 0){
					bul->xm = 0;
				}
				bul->xm -= bul->ani_no * 0.8;
				bul->ym = 0;
				bul->direct = 0;
			} else if (gKey & gKeyRight){
				if (bul->xm < 0){
					bul->xm = 0;
				}
				bul->xm += bul->ani_no * 0.8;
				bul->ym = 0;
				bul->direct = 2;
			} else {
				bul->xm = 0;
				bul->ym = 0;
			}
		} else {
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

		if ((GetNPCFlag(Flags::LILY_CHARACTER) || GetNPCFlag(Flags::MAHIN_CHARACTER)) && gKey & gKeyJump){
			
		} else {
			gMC.shock = 2;
		}

		if (bul->xm < -max_move)
			bul->xm = -max_move;
		if (bul->ym < -max_move)
			bul->ym = -max_move;

		if (bul->xm > max_move)
			bul->xm = max_move;
		if (bul->ym > max_move)
			bul->ym = max_move;

		bul->x += bul->xm;
		bul->y += bul->ym;

		gMC.unit = 2;
		if ((GetNPCFlag(Flags::LILY_CHARACTER) || GetNPCFlag(Flags::MAHIN_CHARACTER)) && gKey & gKeyJump){
			gMC.x = bul->x;
			gMC.y = bul->y;
		} else {
			gMC.x += bul->xm;
			gMC.y += bul->ym;
		}
		gMC.direct = bul->direct;

		// Camera
		if (gBoss[0].code_char != 5){
			if (gMC.direct == 0)
			{
				gMC.index_x -= 0x200;
				if (gMC.index_x < -0x8000)
					gMC.index_x = -0x8000;
			}
			else
			{
				gMC.index_x += 0x200;
				if (gMC.index_x > 0x8000)
					gMC.index_x = 0x8000;
			}
			if (gKey & gKeyUp)
			{
				gMC.index_y -= 0x200;
				if (gMC.index_y < -0x8000)
					gMC.index_y = -0x8000;
			}
			else if (gKey & gKeyDown)
			{
				gMC.index_y += 0x200;
				if (gMC.index_y > 0x8000)
					gMC.index_y = 0x8000;
			}
			else
			{
				if (gMC.index_y > 0x200)
					gMC.index_y -= 0x200;
				if (gMC.index_y < -0x200)
					gMC.index_y += 0x200;
			}

			gMC.tgt_x = gMC.x + gMC.index_x;
			gMC.tgt_y = gMC.y + gMC.index_y;
			SetFrameTargetMyChar(8);
		}

		break;
	}

	switch (bul->direct)
	{
	case 0:
		SetCaret(bul->x + (8 * 0x200), bul->y, 7, 2);
		break;
	case 1:
		SetCaret(bul->x, bul->y + (8 * 0x200), 7, 3);
		break;
	case 2:
		SetCaret(bul->x - (8 * 0x200), bul->y, 7, 0);
		break;
	case 3:
		SetCaret(bul->x, bul->y - (8 * 0x200), 7, 1);
		break;
	}

	if (gKeyTrg & gKeyAbility && gMC.equip & (EQUIP_THROTTLER))
	{
		bul->cond = 0;
		SetBullet(Bullets::MISSILE_EXPLOSION, bul->x, bul->y, 0);
		return;
	}
    
    Rect rect[4] = {
		{0, 16, 16, 32},
		{16, 16, 32, 32},
		{32, 16, 48, 32},
		{48, 16, 64, 32},
	};

	
	bul->rect = rect[bul->direct];
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
    gBulletTable.insert({Bullets::ENERGY_PROJECTILE, {
        {2, 1, 30, 36, 4, 4, 2, 2, {8, 8, 8, 8}, 0, 0},
        ActBullet_EnergyShot
    }});
    gBulletTable.insert({Bullets::WHIMSICAL_STAR, {
        {4, 100, 0, 52, 10, 10, 8, 8, {12, 12, 12, 12}, 0.1, 0.1},
        ActBullet_WhimsicalStar
    }});
    gBulletTable.insert({Bullets::MOVING_MISSILE, {
        {1, 127, 70, 40, 10, 10, 4, 4, {8, 8, 8, 8}, 1, 1},
        ActBullet_Missile
    }});
    gBulletTable.insert({Bullets::EXPLOSION, {
        {1, 100, 100, 20, 16, 16, 0, 0, {0, 0, 0, 0}, 0, 0},
        [](BULLET* bul){ ActBullet_Explosion(bul, 1); }
    }});
    gBulletTable.insert({Bullets::BIGGER_EXPLOSION, {
        {2, 100, 100, 20, 12, 12, 0, 0, {0, 0, 0, 0}, 1, 1},
        [](BULLET* bul){ ActBullet_SuperExplosion(bul, 1); }
    }});
    gBulletTable.insert({Bullets::MISSILE_EXPLOSION, {
        {1, 100, 100, 20, 16, 16, 0, 0, {0, 0, 0, 0}, 0, 0},
        [](BULLET* bul){ ActBullet_Explosion(bul, 2); }
    }});
    gBulletTable.insert({Bullets::HELLSWEEP_EXPLOSION, {
        {1, 100, 100, 20, 16, 16, 0, 0, {0, 0, 0, 0}, 0, 0},
        [](BULLET* bul){ ActBullet_Explosion(bul, 3); }
    }});
}