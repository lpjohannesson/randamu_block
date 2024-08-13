#include "NpcAct.h"

#include <cstddef>

#include "Back.h"
#include "Boss.h"
#include "BossLife.h"
#include "Bullet.h"
#include "Flags.h"
#include "Frame.h"
#include "Game.h"
#include "Map.h"
#include "MyChar.h"
#include "MycHit.h"
#include "NpChar.h"
#include "ResourceCode.h"
#include "Sound.h"
#include "Stage.h"
#include "Triangle.h"

// Sue (being teleported by Misery)
void ActNpc280(NPCHAR *npc)
{
	Rect rcLeft[2] = {
		{112, 32, 128, 48},
		{144, 32, 160, 48},
	};

	Rect rcRight[2] = {
		{112, 48, 128, 64},
		{144, 48, 160, 64},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->x += 6 * 0x200;
			npc->tgt_x = npc->x;
			PlaySoundObject(29, SOUND_MODE_PLAY);
			// Fallthrough
		case 1:
			if (++npc->act_wait == 64)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
			}

			break;

		case 2:
			npc->ani_no = 0;

			if (npc->flag & 8)
			{
				npc->act_no = 4;
				npc->act_wait = 0;
				npc->ani_no = 1;
				PlaySoundObject(23, SOUND_MODE_PLAY);
			}

			break;
	}

	if (npc->act_no > 1)
	{
		npc->ym += 0x20;
		if (npc->ym > 0x5FF)
			npc->ym = 0x5FF;

		npc->y += npc->ym;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	if (npc->act_no == 1)
	{
		npc->rect.bottom = npc->rect.top + npc->act_wait / 4;

		if (npc->act_wait / 2 % 2)
			npc->x = npc->tgt_x;
		else
			npc->x = npc->tgt_x + (1 * 0x200);
	}
}

// Doctor (red energy form)
void ActNpc281(NPCHAR *npc)
{
	Rect rc = {0, 0, 0, 0};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 10;
			break;

		case 10:
			npc->act_no = 11;
			npc->act_wait = 0;
			// Fallthrough
		case 11:
			++npc->act_wait;
			if (npc->act_wait % 4 == 0 && gWaterY <= npc->y)
				SetNpChar(270, npc->x, npc->y, 0, 0, 2, npc, 0x100);

			// if (npc->act_wait > 150)
			// 	npc->act_no = 12;

			break;

		case 20:
			npc->act_no = 21;
			npc->act_wait = 0;
			// Fallthrough
		case 21:
			if (++npc->act_wait > 250)
			{
				DeleteNpCharCode(270, false);
				npc->act_no = 22;
			}

			break;
	}

	npc->rect = rc;
}

// Mini Undead Core (active)
void ActNpc282(NPCHAR *npc)
{
	// Yes, Pixel spelt this wrong (should be 'rc')
	Rect tc[3] = {
		{256, 80, 320, 120},
		{256, 0, 320, 40},
		{256, 120, 320, 160},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 20;
			npc->tgt_y = npc->y;

			if (Random(0, 100) % 2)
				npc->ym = -0x100;
			else
				npc->ym = 0x100;
			// Fallthrough
		case 20:
			npc->xm = -0x200;

			if (npc->x < -64 * 0x200)
				npc->cond = 0;

			if (npc->tgt_y < npc->y)
				npc->ym -= 0x10;
			if (npc->tgt_y > npc->y)
				npc->ym += 0x10;

			if (npc->ym > 0x100)
				npc->ym = 0x100;
			if (npc->ym < -0x100)
				npc->ym = -0x100;

			if (gMC.flag & 8 && gMC.y < npc->y - (4 * 0x200) && gMC.x > npc->x - (24 * 0x200) && gMC.x < npc->x + (24 * 0x200))
			{
				npc->tgt_y = 144 * 0x200;
				npc->ani_no = 2;
			}
			else if (npc->ani_no != 1)
			{
				npc->ani_no = 0;
			}

			if (gMC.flag & 1 && gMC.x < npc->x - npc->hit.back && gMC.x > npc->x - npc->hit.back - (8 * 0x200) && gMC.y + gMC.hit.bottom > npc->y - npc->hit.top && gMC.y - gMC.hit.top < npc->y + npc->hit.bottom)
			{
				npc->bits &= ~NPC_SOLID_HARD;
				npc->ani_no = 1;
			}
			else if (gMC.flag & 4 && gMC.x > npc->x + npc->hit.back && gMC.x < npc->x + npc->hit.back + (8 * 0x200) && gMC.y + gMC.hit.bottom > npc->y - npc->hit.top && gMC.y - gMC.hit.top < npc->y + npc->hit.bottom)
			{
				npc->bits &= ~NPC_SOLID_HARD;
				npc->ani_no = 1;
			}
			else if (gMC.flag & 2 && gMC.y < npc->y - npc->hit.top && gMC.y > npc->y - npc->hit.top - (8 * 0x200) && gMC.x + gMC.hit.front > npc->x - npc->hit.back && gMC.x - gMC.hit.back < npc->x + npc->hit.front)
			{
				npc->bits &= ~NPC_SOLID_HARD;
				npc->ani_no = 1;
			}
			else if (gMC.flag & 8 && gMC.y > npc->y + npc->hit.bottom - (4 * 0x200) && gMC.y < npc->y + npc->hit.bottom + (12 * 0x200) && gMC.x + gMC.hit.front > npc->x - npc->hit.back - (4 * 0x200) && gMC.x - gMC.hit.back < npc->x + npc->hit.front + (4 * 0x200))
			{
				npc->bits &= ~NPC_SOLID_HARD;
				npc->ani_no = 1;
			}
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = tc[npc->ani_no];
}

// Misery (transformed)
void ActNpcWildMiseryEnemy(NPCHAR *npc)
{
	int x, y, direct;

	Rect rcLeft[11] = {
		{0, 64, 32, 96},
		{32, 64, 64, 96},
		{64, 64, 96, 96},
		{96, 64, 128, 96},
		{128, 64, 160, 96},
		{160, 64, 192, 96},
		{192, 64, 224, 96},
		{224, 64, 256, 96},
		{0, 0, 0, 0},
		{256, 64, 288, 96},
		{288, 64, 320, 96},
	};

	Rect rcRight[11] = {
		{0, 96, 32, 128},
		{32, 96, 64, 128},
		{64, 96, 96, 128},
		{96, 96, 128, 128},
		{128, 96, 160, 128},
		{160, 96, 192, 128},
		{192, 96, 224, 128},
		{224, 96, 256, 128},
		{0, 0, 0, 0},
		{256, 96, 288, 128},
		{288, 96, 320, 128},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 20;
			npc->y -= 8 * 0x200;
			PlaySoundObject(29, SOUND_MODE_PLAY);
			// Fallthrough
		case 1:
			if (++npc->act_wait / 2 % 2)
				npc->ani_no = 9;
			else
				npc->ani_no = 0;

			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 9;
			break;

		case 20:
			gSuperXpos = 0;
			npc->act_no = 21;
			npc->act_wait = 0;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 21:
			npc->xm = 7 * npc->xm / 8;
			npc->ym = 7 * npc->ym / 8;

			if (++npc->ani_wait > 20)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (++npc->act_wait > 100 && Distance(gMC.x, gMC.y, npc->x, npc->y) < 200_pix)
				npc->act_no = 30;

			if (npc->x < gMC.x)
				npc->direct = 2;
			else
				npc->direct = 0;

			break;

		case 30:
			npc->act_no = 31;
			npc->act_wait = 0;
			npc->ani_no = 2;
			npc->count2 = npc->life;
			// Fallthrough
		case 31:
			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 2;
			
			if (Distance(gMC.x, gMC.y, npc->x, npc->y) < 200_pix){

				if (npc->flag & 8)
					npc->ym = -0x200;

				if (npc->x > gMC.x)
					npc->xm -= 0x20;
				else
					npc->xm += 0x20;

				if (npc->y > gMC.y)
					npc->ym -= 0x10;
				else
					npc->ym += 0x10;

				if (npc->xm > 0x200)
					npc->xm = 0x200;
				if (npc->xm < -0x200)
					npc->xm = -0x200;

				if (npc->ym > 0x200)
					npc->ym = 0x200;
				if (npc->ym < -0x200)
					npc->ym = -0x200;

				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;

				if (npc->life < npc->count2 - 35)
				{
					npc->act_no = 40;
				}

				if (++npc->act_wait > 275)
					npc->act_no = 50;
			}

			break;

		case 40:
			npc->act_no = 41;
			npc->act_wait = 0;
			npc->xm = 0;
			npc->ym = 0;

			if (npc->x > gMC.x)
				npc->direct = 0;
			else
				npc->direct = 2;

			PlaySoundObject(103, SOUND_MODE_PLAY);

			if (gMC.flag & 8)
				npc->count2 = 290;
			else
				npc->count2 = 289;
			// Fallthrough
		case 41:
			if (++npc->act_wait / 2 % 2)
				npc->ani_no = 4;
			else
				npc->ani_no = 5;

			if (npc->act_wait % 6 == 1)
			{
				if (npc->count2 == 289)
				{
					x = npc->x + (Random(-0x40, 0x40) * 0x200);
					y = npc->y + (Random(-0x20, 0x20) * 0x200);
				}
				else
				{
					x = npc->x + (Random(-0x20, 0x20) * 0x200);
					y = npc->y + (Random(-0x40, 0x40) * 0x200);
				}

				if (x < 32 * 0x200)
					x = 32 * 0x200;
				if (x > (gMap.width - 2) * 0x200 * 0x10)
					x = (gMap.width - 2) * 0x200 * 0x10;

				if (y < 32 * 0x200)
					y = 32 * 0x200;
				if (y > (gMap.length - 2) * 0x200 * 0x10)
					y = (gMap.length - 2) * 0x200 * 0x10;

				PlaySoundObject(39, SOUND_MODE_PLAY);
				SetNpChar(npc->count2, x, y, 0, 0, 0, NULL, 0x100);
			}

			if (npc->act_wait > 50)
			{
				npc->act_no = 42;
				npc->act_wait = 0;

				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;
			}

			break;

		case 42:
			++npc->act_wait;
			npc->ani_no = 6;

			if (npc->act_wait > 50)
			{
				npc->ym = -0x200;

				if (npc->direct == 0)
					npc->xm = 0x200;
				else
					npc->xm = -0x200;

				npc->act_no = 30;
			}

			break;

		case 50:
			npc->act_no = 51;
			npc->act_wait = 0;
			npc->xm = 0;
			npc->ym = 0;

			if (npc->x > gMC.x)
				npc->direct = 0;
			else
				npc->direct = 2;

			PlaySoundObject(103, SOUND_MODE_PLAY);
			// Fallthrough
		case 51:
			if (++npc->act_wait / 2 % 2)
				npc->ani_no = 4;
			else
				npc->ani_no = 5;

			if (gMC.equip & EQUIP_BOOSTER_2_0)
			{
				if (npc->act_wait % 10 == 1)
				{
					if (npc->direct == 0)
					{
						x = npc->x + (10 * 0x200);
						y = npc->y;

						switch (npc->act_wait / 6 % 4)
						{
							case 0:
								direct = 0xD8;
								break;

							case 1:
								direct = 0xEC;
								break;

							case 2:
								direct = 0x14;
								break;

							case 3:
								direct = 0x28;
								break;
						}
					}
					else
					{
						x = npc->x - (10 * 0x200);
						y = npc->y;

						switch (npc->act_wait / 6 % 4)
						{
							case 0:
								direct = 0x58;
								break;

							case 1:
								direct = 0x6C;
								break;

							case 2:
								direct = 0x94;
								break;

							case 3:
								direct = 0xA8;
								break;
						}
					}

					PlaySoundObject(39, SOUND_MODE_PLAY);
					SetNpChar(301, x, y, 0, 0, direct, NULL, 0x100);
				}
			}
			else if (npc->act_wait % 24 == 1)
			{
				if (npc->direct == 0)
				{
					x = npc->x + (10 * 0x200);
					y = npc->y;

					switch (npc->act_wait / 6 % 4)
					{
						case 0:
							direct = 0xD8;
							break;

						case 1:
							direct = 0xEC;
							break;

						case 2:
							direct = 0x14;
							break;

						case 3:
							direct = 0x28;
							break;
					}
				}
				else
				{
					x = npc->x - (10 * 0x200);
					y = npc->y;

					switch (npc->act_wait / 6 % 4)
					{
						case 0:
							direct = 0x58;
							break;

						case 1:
							direct = 0x6C;
							break;

						case 2:
							direct = 0x94;
							break;

						case 3:
							direct = 0xA8;
							break;
					}
				}

				PlaySoundObject(39, SOUND_MODE_PLAY);
				SetNpChar(301, x, y, 0, 0, direct, NULL, 0x100);
			}

			if (npc->act_wait > 50)
			{
				npc->act_no = 42;
				npc->act_wait = 0;

				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;
			}

			break;

		case 99:
			npc->xm = 0;
			npc->ym = 0;
			npc->ani_no = 9;
			npc->bits &= ~NPC_SHOOTABLE;
			break;

		case 100:
			npc->act_no = 101;
			npc->ani_no = 9;
			npc->damage = 0;
			npc->bits &= ~NPC_SHOOTABLE;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			npc->ym = -0x200;
			npc->shock += 50;
			npc->hit.bottom = 12 * 0x200;
			++gBoss[0].ani_no;
			// Fallthrough
		case 101:
			npc->ym += 0x20;

			if (npc->y > (216 * 0x200) - npc->hit.bottom)
			{
				npc->y = (216 * 0x200) - npc->hit.bottom;
				npc->act_no = 102;
				npc->ani_no = 10;
				npc->xm = 0;
				npc->ym = 0;
			}

			break;
	}

	npc->y += npc->ym;

	if (npc->shock)
		npc->x += npc->xm / 2;
	else
		npc->x += npc->xm;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Sue (transformed)
void ActNpc284(NPCHAR *npc)
{
	unsigned char deg;

	Rect rcLeft[13] = {
		{0, 128, 32, 160},
		{32, 128, 64, 160},
		{64, 128, 96, 160},
		{96, 128, 128, 160},
		{128, 128, 160, 160},
		{160, 128, 192, 160},
		{192, 128, 224, 160},
		{224, 128, 256, 160},
		{0, 0, 0, 0},
		{256, 128, 288, 160},
		{288, 128, 320, 160},
		{224, 64, 256, 96},
		{208, 32, 224, 48},
	};

	Rect rcRight[13] = {
		{0, 160, 32, 192},
		{32, 160, 64, 192},
		{64, 160, 96, 192},
		{96, 160, 128, 192},
		{128, 160, 160, 192},
		{160, 160, 192, 192},
		{192, 160, 224, 192},
		{224, 160, 256, 192},
		{0, 0, 0, 0},
		{256, 160, 288, 192},
		{288, 160, 320, 192},
		{224, 96, 256, 128},
		{208, 48, 224, 64},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->y += 0x200 * 12;
			// Fallthrough
		case 1:
			if (++npc->ani_wait > 12){
				if (npc->ani_no == 0)
					npc->ani_no = 1;
				else
					npc->ani_no = 0;
				npc->ani_wait = 0;
			}
			if (npc->shock){
				npc->act_no = 20;
			}
			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 11;
			npc->view.top = 16 * 0x200;
			npc->view.back = 16 * 0x200;
			npc->view.front = 16 * 0x200;
			DeleteNpCharCode(257, true);
			break;

		case 20:
			npc->act_no = 21;
			npc->act_wait = 0;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->damage = 0;
			npc->bits |= NPC_SHOOTABLE;
			npc->bits &= ~NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 21:
			npc->xm = (npc->xm * 6) / 7;
			npc->ym = (npc->ym * 6) / 7;

			if (++npc->ani_wait > 20)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (++npc->act_wait > 20)
				npc->act_no = 30;

			if (npc->x < gMC.x)
				npc->direct = 2;
			else
				npc->direct = 0;

			break;

		case 30:
			npc->act_no = 31;
			npc->act_wait = 0;
			npc->ani_no = 2;
			npc->xm = 0;
			npc->ym = 0;
			// Fallthrough
		case 31:
			if (++npc->act_wait > 4)
			{
				int patterns = 4;
				if (++npc->count2 == patterns){
					++npc->count1;
					npc->count1 %= 4;
					npc->count2 = 0;
				}

				switch (npc->count1)
				{
					case 0:
					case 2:
						npc->act_no = 34;
						break;

					case 1:	// Identical to case 2
						npc->act_no = 32;
						break;

					case 3:	// Identical to case 0
						npc->act_no = 32;
						break;
				}
			}

			break;

		case 32:
			npc->act_no = 33;
			npc->act_wait = 0;
			npc->bits &= ~NPC_SHOOTABLE;
			npc->damage = 4;

			if (gMC.x < npc->x)
				npc->tgt_x = gMC.x - (200 * 0x200);
			else
				npc->tgt_x = gMC.x + (200 * 0x200);

			npc->tgt_y = gMC.y;

			deg = GetArktan(npc->x - npc->tgt_x, npc->y - npc->tgt_y);
			deg += Random(-10, 10);
			npc->xm = 3.75 * GetCos(deg);
			npc->ym = 12 * GetSin(deg);
			if (gMC.difficulty >= DIFFICULTY_REINFORCED){
				for (int a = 0; a < 2; a++)
					SetNpChar(269, npc->x + Random(-16_pix, 16_pix), npc->y + Random(-16_pix, 16_pix), 3 * GetCos(deg), 6 * GetSin(deg), npc->direct, NULL, 0x100);
			}
			PlaySoundObject(102, SOUND_MODE_PLAY);
			// npc->bits &= ~NPC_IGNORE_SOLIDITY;

			// if (npc->x < (gMap.width * 0x200 * 0x10) / 2 && npc->xm > 0)
			// {
			// 	if (npc->y < (gMap.length * 0x200 * 0x10) / 2 && npc->ym > 0)
			// 		npc->bits |= NPC_IGNORE_SOLIDITY;
			// 	if (npc->y > (gMap.length * 0x200 * 0x10) / 2 && npc->ym < 0)
			// 		npc->bits |= NPC_IGNORE_SOLIDITY;
			// }
			// if (npc->x > (gMap.width * 0x200 * 0x10) / 2 && npc->xm < 0)
			// {
			// 	if (npc->y < (gMap.length * 0x200 * 0x10) / 2 && npc->ym > 0)
			// 		npc->bits |= NPC_IGNORE_SOLIDITY;
			// 	if (npc->y > (gMap.length * 0x200 * 0x10) / 2 && npc->ym < 0)
			// 		npc->bits |= NPC_IGNORE_SOLIDITY;
			// }

			if (npc->xm > 0)
				npc->direct = 2;
			else
				npc->direct = 0;
			// Fallthrough
		case 33:
			if (++npc->act_wait / 2 % 2)
				npc->ani_no = 3;
			else
				npc->ani_no = 8;

			if (npc->act_wait > 32 || npc->flag & 5)
				npc->act_no = 20;
			npc->ym += 0x40;
			if (npc->ym > 0x400)
				npc->ym = 0x400;

			break;

		case 34:
			npc->act_no = 35;
			npc->act_wait = 0;
			npc->damage = 3;

			npc->tgt_x = gMC.x + gMC.xm;
			npc->tgt_y = gMC.y + gMC.ym;

			deg = GetArktan(npc->x - npc->tgt_x, npc->y - npc->tgt_y);
			deg += Random(-50, 50);
			npc->ym = 6 * GetSin(deg);
			npc->xm = 6 * GetCos(deg);
			PlaySoundObject(103, SOUND_MODE_PLAY);
			if (gMC.difficulty > DIFFICULTY_HARDENED && npc->count2 % 2 == 0){
				SetNpChar(333, gMC.x + gMC.xm + Random(-16_pix, 16_pix), gMC.y - 4_pix, 0, 0, 0, NULL, 0x100);
			}
			// npc->bits &= ~NPC_IGNORE_SOLIDITY;

			// if (npc->x < (gMap.width * 0x200 * 0x10) / 2 && npc->xm > 0)
			// {
			// 	if (npc->y < (gMap.length * 0x200 * 0x10) / 2 && npc->ym > 0)
			// 		npc->bits |= NPC_IGNORE_SOLIDITY;
			// 	if (npc->y > (gMap.length * 0x200 * 0x10) / 2 && npc->ym < 0)
			// 		npc->bits |= NPC_IGNORE_SOLIDITY;
			// }

			// if (npc->x > (gMap.width * 0x200 * 0x10) / 2 && npc->xm < 0)
			// {
			// 	if (npc->y < (gMap.length * 0x200 * 0x10) / 2 && npc->ym > 0)
			// 		npc->bits |= NPC_IGNORE_SOLIDITY;
			// 	if (npc->y > (gMap.length * 0x200 * 0x10) / 2 && npc->ym < 0)
			// 		npc->bits |= NPC_IGNORE_SOLIDITY;
			// }

			if (npc->xm > 0)
				npc->direct = 2;
			else
				npc->direct = 0;
			// Fallthrough
		case 35:
			if (++npc->act_wait > 6 && (npc->shock && Random(0, 4) == 0))
				npc->act_no = 40;
			else if (npc->act_wait > 8 || npc->flag & 5)
				npc->act_no = 20;

			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 7)
				npc->ani_no = 4;

			if (npc->act_wait % 2 == 1)
				PlaySoundObject(109, SOUND_MODE_PLAY);

			break;

		case 40:
			npc->act_no = 41;
			npc->act_wait = 0;
			npc->ani_no = 2;
			npc->damage = 0;
			npc->bits &= ~NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 41:
			npc->xm = (npc->xm * 8) / 9;
			npc->ym = (npc->ym * 8) / 9;

			if (++npc->act_wait > 3)
			{
				npc->act_no = 42;
				npc->act_wait = 0;
				npc->ym = -0x300;

				if (npc->direct == 0)
					npc->xm = 0x300;
				else
					npc->xm = -0x300;
			}

			break;

		case 42:
			npc->ani_no = 9;

			if (npc->flag & 8)
			{
				npc->act_no = 43;
				npc->act_wait = 0;
				npc->ani_no = 2;

				if (npc->x < gMC.x)
					npc->direct = 2;
				else
					npc->direct = 0;
			}

			npc->ym += 0x20;
			if (npc->ym > 0x5FF)
				npc->ym = 0x5FF;

			break;

		case 43:
			if (++npc->act_wait > 3)
				npc->act_no = 20;

			break;

		case 99:
			npc->xm = 0;
			npc->ym = 0;
			npc->ani_no = 9;
			npc->bits &= ~NPC_SHOOTABLE;
			break;

		case 100:
			npc->act_no = 101;
			npc->ani_no = 9;
			npc->damage = 0;
			npc->bits &= ~NPC_SHOOTABLE;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			npc->ym = -0x200;
			npc->shock += 50;
			++gBoss[0].ani_no;
			// Fallthrough
		case 101:
			npc->ym += 0x20;

			if (npc->y > (216 * 0x200) - npc->hit.bottom)
			{
				npc->y = (216 * 0x200) - npc->hit.bottom;
				npc->act_no = 102;
				npc->ani_no = 10;
				npc->xm = 0;
				npc->ym = 0;
			}

			break;
	}

	if (npc->x < gMC.x)
			npc->direct = 2;
		else
			npc->direct = 0;

	if (npc->flag & 0x100){
		npc->x += npc->xm / 2;
		npc->y += npc->ym / 2;
	} else {
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Undead Core spiral projectile
void ActNpc285(NPCHAR *npc)
{
	Rect rc = {232, 104, 248, 120};
	unsigned char deg;

	if (npc->x < 0 || npc->x > gMap.width * 0x10 * 0x200)
	{
		VanishNpChar(npc);
		return;
	}

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			npc->count1 = npc->direct / 8;
			npc->direct %= 8;
			// Fallthrough
		case 1:
			npc->count1 += 24;
			npc->count1 %= 0x100;

			deg = npc->count1;

			if (npc->act_wait < 128)
				++npc->act_wait;

			if (npc->direct == 0)
				npc->xm -= 21;
			else
				npc->xm += 21;

			npc->tgt_x += npc->xm;

			npc->x = npc->tgt_x + (GetCos(deg) * 4);
			npc->y = npc->tgt_y + (GetSin(deg) * 6);

			SetNpChar(286, npc->x, npc->y, 0, 0, 0, NULL, 0x100);

			break;
	}

	npc->rect = rc;
}

// Undead Core spiral shot trail
void ActNpc286(NPCHAR *npc)
{
	Rect rc[3] = {
		{232, 120, 248, 136},
		{232, 136, 248, 152},
		{232, 152, 248, 168},
	};

	if (++npc->ani_wait > 0)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2)
		npc->cond = 0;
	else
		npc->rect = rc[npc->ani_no];
}

// Orange smoke
void ActNpc287(NPCHAR *npc)
{
	Rect rcLeft[7] = {
		{0, 224, 16, 240},
		{16, 224, 32, 240},
		{32, 224, 48, 240},
		{48, 224, 64, 240},
		{64, 224, 80, 240},
		{80, 224, 96, 240},
		{96, 224, 112, 240},
	};

	if (npc->act_no == 0)
	{
		npc->xm = Random(-4, 4) * 0x200;
		npc->act_no = 1;
	}
	else
	{
		npc->xm = (npc->xm * 20) / 21;
		npc->ym = (npc->ym * 20) / 21;

		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	if (++npc->ani_wait > 1)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 6)
		npc->cond = 0;
	else
		npc->rect = rcLeft[npc->ani_no];
}

// Undead Core exploding rock
void ActNpc288(NPCHAR *npc)
{
	Rect rc[5] = {
		{232, 72, 248, 88},
		{232, 88, 248, 104},
		{232, 0, 256, 24},
		{232, 24, 256, 48},
		{232, 48, 256, 72},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->xm = -0x200;
			// Fallthrough
		case 1:
			if (npc->direct == 1)
			{
				npc->ym -= 0x20;
				if (npc->ym < -0x5FF)
					npc->ym = -0x5FF;

				if (npc->flag & 2)
					npc->act_no = 2;
			}
			else if (npc->direct == 3)
			{
				npc->ym += 0x20;
				if (npc->ym > 0x5FF)
					npc->ym = 0x5FF;

				if (npc->flag & 8)
					npc->act_no = 2;
			}

			if (++npc->ani_wait > 3)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			break;

		case 2:
			PlaySoundObject(44, SOUND_MODE_PLAY);
			npc->act_no = 3;
			npc->act_wait = 0;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			npc->ym = 0;

			if (npc->x > gMC.x)
				npc->xm = -0x400;
			else
				npc->xm = 0x400;

			npc->view.back = 12 * 0x200;
			npc->view.front = 12 * 0x200;
			npc->view.top = 12 * 0x200;
			npc->view.bottom = 12 * 0x200;
			// Fallthrough
		case 3:
			if (++npc->ani_no > 4)
				npc->ani_no = 2;

			if (++npc->act_wait % 4 == 1)
			{
				if (npc->direct == 1)
					SetNpChar(287, npc->x, npc->y, 0, 0x400, 0, NULL, 0x100);
				else
					SetNpChar(287, npc->x, npc->y, 0, -0x400, 0, NULL, 0x100);
			}

			if (npc->x < 1 * 0x200 * 0x10 || npc->x > (gMap.width * 0x200 * 0x10) - (1 * 0x200 * 0x10))
				npc->cond = 0;

			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];
}

// Critter (orange, Misery)
void ActNpc289(NPCHAR *npc)
{
	Rect rcLeft[3] = {
		{160, 32, 176, 48},
		{176, 32, 192, 48},
		{192, 32, 208, 48},
	};

	Rect rcRight[3] = {
		{160, 48, 176, 64},
		{176, 48, 192, 64},
		{192, 48, 208, 64},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 2;

			if (npc->x > gMC.x)
				npc->direct = 0;
			else
				npc->direct = 2;
			// Fallthrough
		case 1:
			if (++npc->act_wait > 16)
			{
				npc->act_no = 10;
				npc->view.top = 8 * 0x200;
				npc->view.bottom = 8 * 0x200;
				npc->damage = 2;
				npc->bits |= NPC_SHOOTABLE;
			}

			break;

		case 10:
			if (npc->flag & 8)
			{
				npc->act_no = 11;
				npc->ani_no = 0;
				npc->act_wait = 0;
				npc->xm = 0;

				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;
			}

			break;

		case 11:
			if (++npc->act_wait > 10)
			{
				if (++npc->count1 > 4)
					npc->act_no = 12;
				else
					npc->act_no = 10;

				PlaySoundObject(30, SOUND_MODE_PLAY);
				npc->ym = -0x600;

				if (npc->direct == 0)
					npc->xm = -0x200;
				else
					npc->xm = 0x200;

				npc->ani_no = 2;
			}

			break;

		case 12:
			npc->bits |= NPC_IGNORE_SOLIDITY;

			if (npc->y > gMap.length * 0x200 * 0x10)
			{
				VanishNpChar(npc);
				return;
			}

			break;
	}

	if (npc->act_no >= 10)
		npc->ym += 0x40;

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	if (npc->act_no == 1)
	{
		npc->rect.top += 8 - (npc->act_wait / 2);
		npc->rect.bottom -= 8 + (npc->act_wait / 2);
		npc->view.top = (npc->act_wait * 0x200) / 2;
		npc->view.bottom = (npc->act_wait * 0x200) / 2;
	}
}

// Bat (Misery)
void ActNpc290(NPCHAR *npc)
{
	Rect rcLeft[3] = {
		{112, 32, 128, 48},
		{128, 32, 144, 48},
		{144, 32, 160, 48},
	};

	Rect rcRight[3] = {
		{112, 48, 128, 64},
		{128, 48, 144, 64},
		{144, 48, 160, 64},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 2;

			if (npc->x > gMC.x)
				npc->direct = 0;
			else
				npc->direct = 2;
			// Fallthrough
		case 1:
			if (++npc->act_wait > 16)
			{
				npc->act_no = 10;
				npc->view.top = 8 * 0x200;
				npc->view.bottom = 8 * 0x200;
				npc->damage = 2;
				npc->bits |= NPC_SHOOTABLE;
				npc->tgt_y = npc->y;
				npc->ym = 0x400;
			}

			break;

		case 10:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 2)
				npc->ani_no = 0;

			if (npc->y < npc->tgt_y)
				npc->ym += 0x40;
			else
				npc->ym -= 0x40;

			if (npc->direct == 0)
				npc->xm -= 0x10;
			else
				npc->xm += 0x10;

			if (npc->x < 0 || npc->y < 0 || npc->x > gMap.width * 0x200 * 0x10 || npc->y > gMap.length * 0x200 * 0x10)
			{
				VanishNpChar(npc);
				return;
			}

			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	if (npc->act_no == 1)
	{
		npc->rect.top += 8 - (npc->act_wait / 2);
		npc->rect.bottom -= 8 + (npc->act_wait / 2);
		npc->view.top = (npc->act_wait * 0x200) / 2;
		npc->view.bottom = (npc->act_wait * 0x200) / 2;
	}
}

// Mini Undead Core (inactive)
void ActNpc291(NPCHAR *npc)
{
	Rect tc[2] = {
		{256, 80, 320, 120},
		{256, 0, 320, 40},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 20;

			if (npc->direct == 2)
			{
				npc->bits &= ~NPC_SOLID_HARD;
				npc->ani_no = 1;
			}

			break;
	}

	npc->rect = tc[npc->ani_no];
}

// Quake
void ActNpc292(NPCHAR *npc)
{
	(void)npc;

	SetQuake2(2);
}

// Undead Core giant energy shot
void ActNpc293(NPCHAR *npc)
{
	Rect rect[2] = {
		{240, 200, 280, 240},
		{280, 200, 320, 240},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			// Fallthrough
		case 1:
			if (++npc->ani_no > 1)
				npc->ani_no = 0;

			SetNpChar(4, npc->x + (Random(0, 0x10) * 0x200), npc->y + (Random(-0x10, 0x10) * 0x200), 0, 0, 0, NULL, 0x100);

			npc->x -= 8 * 0x200;

			if (npc->x < -32 * 0x200)
				npc->cond = 0;

			break;
	}

	npc->rect = rect[npc->ani_no];
}

// Quake + falling block generator
void ActNpcFallingBlockController(NPCHAR *npc)
{
	int x, y, dir;

	switch (npc->act_no)
	{
		case 0:
			if (gMC.x < (gMap.width - 6) * 0x200 * 0x10)
			{
				npc->act_no = 1;
				npc->act_wait = 0;
			}

			break;

		case 1:
			++npc->act_wait;
			npc->x = gMC.x + (96 * 0x200);

			if (npc->act_wait > 170)
			{
				x = gMC.x + (Random(-13, 13) * 0x2000);

				y = gMC.y - (11 * 0x2000);

				if (Random(0, 10) % 2)	// Because just doing 'Random(0, 1)' is too hard
					dir = 0;
				else
					dir = 2;

				SetNpChar(279, x, y, 0, 0, dir, NULL, 0x100);

				npc->act_wait = Random(0, 60);
			}

			break;
	}
}

// Cloud
void ActNpc295(NPCHAR *npc)
{
	Rect rc[4] = {
		{0, 0, 208, 64},
		{32, 64, 144, 96},
		{32, 96, 104, 0x80},
		{104, 96, 144, 0x80},
	};
	npc->surf = SURFACE_ID_CLOUDS;

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = npc->direct % 4;
			switch (npc->direct)
			{
				case 0:
					npc->ym = -1000;
					npc->view.back = 104 * 0x200;
					npc->view.front = 104 * 0x200;
					break;

				case 1:
					npc->ym = -0x800;
					npc->view.back = 56 * 0x200;
					npc->view.front = 56 * 0x200;
					break;

				case 2:
					npc->ym = -0x400;
					npc->view.back = 32 * 0x200;
					npc->view.front = 32 * 0x200;
					break;

				case 3:
					npc->ym = -0x200;
					npc->view.back = 20 * 0x200;
					npc->view.front = 20 * 0x200;
					break;

				case 4:
					npc->xm = -0x400;
					npc->view.back = 104 * 0x200;
					npc->view.front = 104 * 0x200;
					break;

				case 5:
					npc->xm = -0x200;
					npc->view.back = 56 * 0x200;
					npc->view.front = 56 * 0x200;
					break;

				case 6:
					npc->xm = -0x100;
					npc->view.back = 32 * 0x200;
					npc->view.front = 32 * 0x200;
					break;

				case 7:
					npc->xm = -0x80;
					npc->view.back = 20 * 0x200;
					npc->view.front = 20 * 0x200;
					break;
			}
			if (gStageNo == gGeneratedStage){
				npc->view.top = (rc[npc->ani_no].bottom - rc[npc->ani_no].top) / 4 * 0x200;
				npc->hit.top = 0;
				npc->hit.bottom = (rc[npc->ani_no].bottom - rc[npc->ani_no].top) * 0.5f * 0x200;
				npc->hit.front = npc->hit.back = (rc[npc->ani_no].right - rc[npc->ani_no].left) * 0.7f / 2 * 0x200;
				npc->bits |= (NPC_SOLID_SOFT | NPC_SHOOTABLE);
			}

			break;

		case 1:
			npc->x += npc->xm;
			npc->y += npc->ym;

			if (npc->x < -64 * 0x200)
				npc->cond = 0;
			if (npc->y < -32 * 0x200)
				npc->cond = 0;

			break;
	}

	npc->rect = rc[npc->ani_no];
}

// Cloud generator
void ActNpc296(NPCHAR *npc)
{
	int x, y, dir, pri;
	int waitTime = 16;
	if (gStageNo == gGeneratedStage){
		waitTime = 380;
		npc->y = gMC.y + Random(-24_pix, 24_pix);
		npc->x = gFrame.x + ScreenToSubpixelCoord(PixelToScreenCoord(WINDOW_WIDTH));
	}

	if (++npc->act_wait > waitTime)
	{
		npc->act_wait = Random(0, waitTime);

		dir = Random(0, 100) % 4;

		if (npc->direct == 0)
		{
			switch (dir)
			{
				case 0:
					pri = 0x180;
					break;

				case 1:
					pri = 0x80;
					break;

				case 2:
					pri = 0x40;
					break;

				case 3:
					pri = 0x00;
					break;
			}

			x = Random(-10, 10) * 0x200 * 0x10 + npc->x;
			y = npc->y;
			SetNpChar(295, x, y, 0, 0, dir, NULL, pri);
		}
		else
		{
			switch (dir)
			{
				case 0:
					pri = 0x80;
					break;

				case 1:
					pri = 0x55;
					break;

				case 2:
					pri = 0x40;
					break;

				case 3:
					pri = 0x00;
					break;
			}

			x = npc->x;
			y = Random(-7, 7) * 0x200 * 0x10 + npc->y;
			SetNpChar(295, x, y, 0, 0, dir + 4, NULL, pri);
		}
	}
}

// Sue in dragon's mouth
void ActNpc297(NPCHAR *npc)
{
	Rect rc = {112, 48, 0x80, 64};

	npc->x = npc->pNpc->x + (16 * 0x200);
	npc->y = npc->pNpc->y + (8 * 0x200);

	npc->rect = rc;
}

// Doctor (opening)
void ActNpc298(NPCHAR *npc)
{
	Rect rc[8] = {
		{72, 0x80, 88, 160},
		{88, 0x80, 104, 160},
		{104, 0x80, 120, 160},
		{72, 0x80, 88, 160},
		{120, 0x80, 136, 160},
		{72, 0x80, 88, 160},
		{104, 160, 120, 192},
		{120, 160, 136, 192},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->y -= 8 * 0x200;
			// Fallthrough

		case 1:
			npc->ani_no = 0;
			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->count1 = 0;
			// Fallthrough

		case 11:
			if (++npc->ani_wait > 6)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
			{
				npc->ani_no = 0;

				if (++npc->count1 > 7)
				{
					npc->ani_no = 0;
					npc->act_no = 1;
				}
			}

			break;

		case 20:
			npc->act_no = 21;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough

		case 21:
			if (++npc->ani_wait > 10)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			npc->x += 0x100;

			break;

		case 30:
			npc->ani_no = 6;
			break;

		case 40:
			npc->act_no = 41;
			npc->ani_no = 6;
			npc->ani_wait = 0;
			npc->count1 = 0;
			// Fallthrough

		case 41:
			if (++npc->ani_wait > 6)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 7)
			{
				npc->ani_no = 6;

				if (++npc->count1 > 7)
				{
					npc->ani_no = 6;
					npc->act_no = 30;
				}
			}

			break;
	}

	npc->rect = rc[npc->ani_no];
}

// Balrog/Misery (opening)
void ActNpc299(NPCHAR *npc)
{
	Rect rc[2] = {
		{48, 256, 112, 320},
		{48, 256, 112, 320}
	};

	if (npc->act_no == 0)
	{
		npc->act_no = 1;

		if (npc->direct == 0)
		{
			npc->ani_no = 1;
			npc->act_wait = 25;
			npc->y -= 0x40 * (50 / 2);
		}
		else
		{
			npc->ani_no = 0;
			npc->act_wait = 0;
		}
	}

	if (++npc->act_wait / 50 % 2)
		npc->y += 0x45;
	else
		npc->y -= 0x45;

	if (npc->act_wait > 250){
		npc->cond = 0;
		SetDestroyNpChar(npc->x, npc->y, 20_pix, 10);
		PlaySoundObject(65, SOUND_MODE_PLAY);
		for (int deg = 0; deg < 0x100; deg += 0x10)
		{
			int xm = GetCos(deg) * 2.5;
			int ym = GetSin(deg) * 2.5;
			BULLET* bul = SetBullet(93, npc->x, npc->y);
			bul->act_no = 1;
			bul->direct = 4;
			bul->xm = xm;
			bul->ym = ym;
		}
		return;
	}

	for (NPCHAR& npcW : gNPC){
		if (npcW.x - npcW.hit.back < npc->x + npc->hit.back
		&& npcW.x + npcW.hit.back > npc->x - npc->hit.back
		&& npcW.y - npcW.hit.top < npc->y + npc->hit.bottom
		&& npcW.y + npcW.hit.bottom > npc->y - npc->hit.top){
			npcW.flag |= 0x100;
			npcW.waterEffect = 120;
		}
	}

	if (JudgeHitMyCharNPC3(npc) == 0 && GetNPCFlag(Flags::WATER_GUN_EFFECT))
		CutNPCFlag(Flags::WATER_GUN_EFFECT);

	npc->rect = rc[npc->ani_no];
}
