#include "NpcAct.h"

#include <cstddef>

#include "Bullet.h"
#include "Caret.h"
#include "Frame.h"
#include "Game.h"
#include "MyChar.h"
#include "NpChar.h"
#include "Sound.h"
#include "Stage.h"
#include "Triangle.h"

// Dragon Zombie
void ActNpcDragon(NPCHAR *npc)
{
	unsigned char deg;
	int xm, ym;

	Rect rcLeft[6] = {
		{120, 166, 160, 200},
		{160, 166, 200, 200},
		{200, 166, 240, 200},
		{240, 166, 280, 200},
		{280, 166, 320, 200},
	};

	Rect rcRight[6] = {
		{120, 205, 160, 240},
		{160, 205, 200, 240},
		{200, 205, 240, 240},
		{240, 205, 280, 240},
		{280, 205, 320, 240},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 10;
			npc->count1 = 0;
			npc->bits |= NPC_INVULNERABLE;
			npc->bits &= ~NPC_SHOOTABLE;
			// Fallthrough
		case 10:
			if (++npc->ani_wait > 20)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (npc->count1)
				--npc->count1;

			if (npc->count1 == 0 && gMC.x > npc->x - (175 * 0x200) && gMC.x < npc->x + (175 * 0x200) &&
				gMC.y > npc->y - (175 * 0x200) && gMC.y < npc->y + (175 * 0x200)){
				npc->act_no = 20;
			}

			break;

		case 20:
			npc->act_no = 21;
			npc->act_wait = 0;
			// Fallthrough
		case 21:
			if (++npc->act_wait / 2 % 2)
				npc->ani_no = 2;
			else
				npc->ani_no = 3;

			if (npc->act_wait > 35 || gMC.difficulty > DIFFICULTY_HARDENED)
				npc->act_no = 30;

			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			break;

		case 30:
			npc->act_no = 31;
			npc->act_wait = 0;
			npc->ani_no = 4;
			npc->bits &= ~NPC_INVULNERABLE;
			npc->bits |= NPC_SHOOTABLE;
			npc->tgt_x = gMC.x;
			npc->tgt_y = gMC.y;
			// Fallthrough
		case 31:
			if (++npc->act_wait == 10)
			{
				if (npc->direct == 0)
					deg = GetArktan(npc->x - (14 * 0x200) - npc->tgt_x, npc->y - npc->tgt_y);
				else
					deg = GetArktan(npc->x + (14 * 0x200) - npc->tgt_x, npc->y - npc->tgt_y);

				deg += (unsigned char)Random(-5, 5);

				ym = GetSin(deg) * 3;
				xm = GetCos(deg) * 3;
				if (gMC.difficulty > DIFFICULTY_HARDENED){
					ym *= 1.5;
					xm *= 1.5;
				}

				if (npc->direct == 0)
					SetNpChar(218, npc->x - (14 * 0x200), npc->y, xm, ym, 0, NULL, 0x100);
				else
					SetNpChar(218, npc->x + (14 * 0x200), npc->y, xm, ym, 0, NULL, 0x100);

				if (!(gMC.cond & 2))
					PlaySoundObject(101, SOUND_MODE_PLAY);
			}

			if (npc->act_wait > 60)
			{
				npc->act_no = 10;
				npc->count1 = Random(75, 150);
				npc->ani_wait = 0;
				npc->bits |= NPC_INVULNERABLE;
				npc->bits &= ~NPC_SHOOTABLE;
			}

			break;

		case 100:
			npc->ani_no = 5;
			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Dragon Zombie (dead)
void ActNpc201(NPCHAR *npc)
{
	Rect rcLeft[1] = {
		{200, 0, 240, 40}
	};

	Rect rcRight[1] = {
		{200, 40, 240, 80}
	};

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Dragon Zombie projectile
void ActNpcFlameProjectile(NPCHAR *npc)
{
	if (npc->flag & 0xFF)
	{
		npc->cond = 0;
		SetCaret(npc->x, npc->y, 2, 0);
	}

	if (npc->flag & 0x100){
		npc->x += npc->xm / 2;
		npc->y += npc->ym / 2;
	} else {
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	Rect rect_left[3] = {
		{184, 216, 200, 240},
		{200, 216, 216, 240},
		{216, 216, 232, 240},
	};

	if (++npc->ani_wait > 1)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2)
		npc->ani_no = 0;

	npc->rect = rect_left[npc->ani_no];

	if (++npc->count1 > 300)
	{
		npc->cond = 0;
		SetCaret(npc->x, npc->y, 2, 0);
	}
}

// Critter (destroyed Egg Corridor)
void ActNpc203(NPCHAR *npc)
{
	Rect rcLeft[3] = {
		{0, 80, 16, 96},
		{16, 80, 32, 96},
		{32, 80, 48, 96},
	};

	Rect rcRight[3] = {
		{0, 96, 16, 112},
		{16, 96, 32, 112},
		{32, 96, 48, 112},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->y += 3 * 0x200;
			npc->act_no = 1;
			// Fallthrough
		case 1:
			if (npc->x > gMC.x)
				npc->direct = 0;
			else
				npc->direct = 2;

			if (npc->act_wait >= 8 && npc->x - (112 * 0x200) < gMC.x && npc->x + (112 * 0x200) > gMC.x && npc->y - (80 * 0x200) < gMC.y && npc->y + (80 * 0x200) > gMC.y)
			{
				npc->ani_no = 1;
			}
			else
			{
				if (npc->act_wait < 8)
					++npc->act_wait;

				npc->ani_no = 0;
			}

			if (npc->shock)
			{
				npc->act_no = 2;
				npc->ani_no = 0;
				npc->act_wait = 0;
			}

			if (npc->act_wait >= 8 && npc->x - (48 * 0x200) < gMC.x && npc->x + (48 * 0x200) > gMC.x && npc->y - (80 * 0x200) < gMC.y && npc->y + (48 * 0x200) > gMC.y)
			{
				npc->act_no = 2;
				npc->ani_no = 0;
				npc->act_wait = 0;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 3;
				npc->ani_no = 2;
				npc->ym = -0x5FF;

				if (!(gMC.cond & 2))
					PlaySoundObject(30, SOUND_MODE_PLAY);

				if (npc->direct == 0)
					npc->xm = -0x100;
				else
					npc->xm = 0x100;
			}

			break;

		case 3:
			if (npc->flag & 8)
			{
				npc->xm = 0;
				npc->act_wait = 0;
				npc->ani_no = 0;
				npc->act_no = 1;

				if (!(gMC.cond & 2))
					PlaySoundObject(23, SOUND_MODE_PLAY);
			}

			break;
	}

	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Falling spike (small)
void ActNpc204(NPCHAR *npc)
{
	Rect rc[2] = {
		{240, 80, 256, 96},
		{240, 144, 256, 160},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->tgt_x = npc->x;
			// Fallthrough
		case 1:
			if (gMC.x > npc->x - (12 * 0x200) && gMC.x < npc->x + (12 * 0x200) && gMC.y > npc->y)
				npc->act_no = 2;

			break;

		case 2:
			if (++npc->act_wait / 6 % 2)
				npc->x = npc->tgt_x - (1 * 0x200);
			else
				npc->x = npc->tgt_x;

			if (npc->act_wait > 30)
			{
				npc->act_no = 3;
				npc->ani_no = 1;
			}

			break;

		case 3:
			npc->ym += 0x20;

			if (npc->flag & 0xFF)
			{
				if (!(gMC.cond & 2))
					PlaySoundObject(12, SOUND_MODE_PLAY);

				SetDestroyNpChar(npc->x, npc->y, npc->view.back, 4);
				npc->cond = 0;
				return;
			}

			break;
	}

	if (npc->ym > 0xC00)
		npc->ym = 0xC00;

	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];
}

// Falling spike (large)
void ActNpc205(NPCHAR *npc)
{
	Rect rc[2] = {
		{112, 80, 128, 112},
		{128, 80, 144, 112},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->tgt_x = npc->x;
			npc->y += 4 * 0x200;
			// Fallthrough
		case 1:
			if (gMC.x > npc->x - (12 * 0x200) && gMC.x < npc->x + (12 * 0x200) && gMC.y > npc->y)
				npc->act_no = 2;

			break;

		case 2:
			if (++npc->act_wait / 6 % 2)
				npc->x = npc->tgt_x - (1 * 0x200);
			else
				npc->x = npc->tgt_x;

			if (npc->act_wait > 30)
			{
				npc->act_no = 3;
				npc->ani_no = 1;
				npc->act_wait = 0;
			}

			break;

		case 3:
			npc->ym += 0x20;

			if (gMC.y > npc->y)
			{
				npc->bits &= ~NPC_SOLID_HARD;
				npc->damage = 0x7F;
			}
			else
			{
				npc->bits |= NPC_SOLID_HARD;
				npc->damage = 0;
			}

			if (++npc->act_wait > 8 && npc->flag & 0xFF)
			{
				npc->bits |= NPC_SOLID_HARD;
				npc->act_no = 4;
				npc->act_wait = 0;
				npc->ym = 0;
				npc->damage = 0;
				PlaySoundObject(12, SOUND_MODE_PLAY);
				SetDestroyNpChar(npc->x, npc->y, npc->view.back, 4);
				SetBullet(24, npc->x, npc->y, 0);
				return;
			}

			break;

		case 4:
			if (++npc->act_wait > 4)
			{
				npc->act_no = 5;
				npc->bits |= NPC_SHOOTABLE;
			}

			break;
	}

	if (npc->ym > 0xC00)
		npc->ym = 0xC00;

	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];
}

// Counter Bomb
void ActNpcCastleBomb(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			npc->count1 = 120;
			npc->act_wait = Random(0, 50);
			// Fallthrough
		case 1:
			if (++npc->act_wait < 50)
				break;

			npc->act_wait = 0;
			npc->act_no = 2;
			npc->ym = 0x300;
			break;

		case 2:
			if (gMC.x > npc->x - (64 * 0x200) && gMC.x < npc->x + (64 * 0x200) && 
				gMC.y > npc->y - (64 * 0x200) && gMC.y < npc->y + (64 * 0x200))
			{
				npc->act_wait = 0;
				npc->act_no = 3;
			}

			if (npc->shock)
			{
				npc->act_wait = 0;
				npc->act_no = 3;
			}

			break;

		case 3:
			switch (npc->act_wait)
			{
				// Interestingly, this NPC counts down at 60 frames
				// per second, while NPC322 (Deleet) counts at 50.
				case 30 * 0:
					SetNpChar(207, npc->x + (16 * 0x200), npc->y + (4 * 0x200), 0, 0, 0, NULL, 0x100);
					break;

				case 30 * 1:
					SetNpChar(207, npc->x + (16 * 0x200), npc->y + (4 * 0x200), 0, 0, 1, NULL, 0x100);
					break;

				case 30 * 2:
					SetNpChar(207, npc->x + (16 * 0x200), npc->y + (4 * 0x200), 0, 0, 2, NULL, 0x100);
					break;

				case 30 * 3:
					SetNpChar(207, npc->x + (16 * 0x200), npc->y + (4 * 0x200), 0, 0, 3, NULL, 0x100);
					break;

				case 30 * 4:
					SetNpChar(207, npc->x + (16 * 0x200), npc->y + (4 * 0x200), 0, 0, 4, NULL, 0x100);
					break;

				case 30 * 5:
					npc->hit.back = 48 * 0x200;
					npc->hit.front = 48 * 0x200;
					npc->hit.top = 32 * 0x200;
					npc->hit.bottom = 32 * 0x200;
					npc->damage = 15;
					PlaySoundObject(35, SOUND_MODE_PLAY);
					for (int deg = 0; deg < 0x100; deg += 0x2)
					{
						int xm = GetCos(deg) * 3.5;
						int ym = GetSin(deg) * 3.5;
						SetNpChar(38, npc->x, npc->y, xm, ym, 1, NULL, 0x100);
						SetNpChar(38, npc->x, npc->y, xm, ym, 1, NULL, 0x100);
					}
					SetDestroyNpChar(npc->x, npc->y, 0x10000, 100);
					SetQuake(20);
					npc->cond |= 8;
					break;
			}

			++npc->act_wait;
			break;
	}

	if (npc->act_no > 1)
	{
		if (npc->tgt_y < npc->y)
			npc->ym -= 0x20;
		if (npc->tgt_y > npc->y)
			npc->ym += 0x20;

		if (npc->ym > 0x200)
			npc->ym = 0x200;
		if (npc->ym < -0x200)
			npc->ym = -0x200;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	Rect rect_left[3] = {
		{192, 176, 232, 216},
		{232, 176, 272, 216},
		{272, 176, 312, 216},
	};

	if (++npc->ani_wait > 4)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2)
		npc->ani_no = 0;

	npc->rect = rect_left[npc->ani_no];
}

// Counter Bomb's countdown
void ActNpc207(NPCHAR *npc)
{
	Rect rc[5] = {
		{0, 144, 16, 160},
		{16, 144, 32, 160},
		{32, 144, 48, 160},
		{48, 144, 64, 160},
		{64, 144, 80, 160},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = npc->direct;
			PlaySoundObject(43, SOUND_MODE_PLAY);
			// Fallthrough
		case 1:
			npc->x += 1 * 0x200;

			if (++npc->act_wait > 8)
			{
				npc->act_wait = 0;
				npc->act_no = 2;
			}

			break;

		case 2:
			if (++npc->act_wait > 30)
			{
				npc->cond = 0;
				return;
			}

			break;
	}

	npc->rect = rc[npc->ani_no];
}

// Basu (destroyed Egg Corridor)
void ActNpc208(NPCHAR *npc)
{
	Rect rcLeft[3] = {
		{248, 80, 272, 104},
		{272, 80, 296, 104},
		{296, 80, 320, 104},
	};

	Rect rcRight[3] = {
		{248, 104, 272, 128},
		{272, 104, 296, 128},
		{296, 104, 320, 128},
	};

	switch (npc->act_no)
	{
		case 0:
			if (gMC.x < npc->x + (16 * 0x200) && gMC.x > npc->x - (16 * 0x200))
			{
				npc->bits |= NPC_SHOOTABLE;
				npc->ym = -0x200;
				npc->tgt_x = npc->x;
				npc->tgt_y = npc->y;
				npc->act_no = 1;
				npc->act_wait = 0;
				npc->count1 = npc->direct;
				npc->count2 = 0;
				npc->damage = 6;

				if (npc->direct == 0)
				{
					npc->x = gMC.x + (256 * 0x200);
					npc->xm = -0x2FF;
				}
				else
				{
					npc->x = gMC.x - (256 * 0x200);
					npc->xm = 0x2FF;
				}

				return;
			}

			npc->rect.right = 0;
			npc->damage = 0;
			npc->xm = 0;
			npc->ym = 0;
			npc->bits &= ~NPC_SHOOTABLE;
			return;

		case 1:
			if (npc->x > gMC.x)
			{
				npc->direct = 0;
				npc->xm -= 0x10;
			}
			else
			{
				npc->direct = 2;
				npc->xm += 0x10;
			}

			if (npc->flag & 1)
				npc->xm = 0x200;
			if (npc->flag & 4)
				npc->xm = -0x200;

			if (npc->y < npc->tgt_y)
				npc->ym += 8;
			else
				npc->ym -= 8;

			if (npc->xm > 0x2FF)
				npc->xm = 0x2FF;
			if (npc->xm < -0x2FF)
				npc->xm = -0x2FF;

			if (npc->ym > 0x200)
				npc->ym = 0x200;
			if (npc->ym < -0x200)
				npc->ym = -0x200;

			if (npc->shock)
			{
				npc->x += npc->xm / 2;
				npc->y += npc->ym / 2;
			}
			else
			{
				npc->x += npc->xm;
				npc->y += npc->ym;
			}

			if (gMC.x > npc->x + (400 * 0x200) || gMC.x < npc->x - (400 * 0x200))
			{
				npc->act_no = 0;
				npc->xm = 0;
				npc->direct = npc->count1;
				npc->x = npc->tgt_x;
				npc->rect.right = 0;
				npc->damage = 0;
				return;
			}

			break;
	}

	if (npc->act_no != 0)
	{
		if (npc->act_wait < 150)
			++npc->act_wait;

		if (npc->act_wait == 150)
		{
			if (++npc->count2 % 8 == 0 && npc->x < gMC.x + (160 * 0x200) && npc->x > gMC.x - (160 * 0x200))
			{
				unsigned char deg;
				int xm;
				int ym;

				deg = GetArktan(npc->x - gMC.x, npc->y - gMC.y);
				deg += (unsigned char)Random(-6, 6);
				ym = GetSin(deg) * 3;
				xm = GetCos(deg) * 3;
				SetNpChar(209, npc->x, npc->y, xm, ym, 0, NULL, 0x100);
				PlaySoundObject(39, SOUND_MODE_PLAY);
			}

			if (npc->count2 > 16)
			{
				npc->act_wait = 0;
				npc->count2 = 0;
			}
		}
	}

	if (++npc->ani_wait > 1)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 1)
		npc->ani_no = 0;

	if (npc->act_wait > 120 && npc->act_wait / 2 % 2 == 1 && npc->ani_no == 1)
		npc->ani_no = 2;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Basu projectile (destroyed Egg Corridor)
void ActNpcSemiHomingProjectile(NPCHAR *npc)
{
	if (npc->flag & 0xFF)
	{
		npc->cond = 0;
		SetCaret(npc->x, npc->y, 2, 0);
	}
	
	if (++npc->act_wait > 40){
		npc->damage = 5;
		npc->bits &= ~NPC_IGNORE_SOLIDITY;
		if (npc->x > gMC.x)
			npc->xm -= 0x40;
		else
			npc->xm += 0x40;
		if (npc->y > gMC.y)
			npc->ym -= 0x40;
		else
			npc->ym += 0x40;
		if (npc->xm > 0x400)
			npc->xm = 0x400;
		if (npc->xm < -0x400)
			npc->xm = -0x400;
		if (npc->ym > 0x400)
			npc->ym = 0x400;
		if (npc->ym < -0x400)
			npc->ym = -0x400;
	}

	if (npc->flag & 0x100){
		npc->x += npc->xm / 2;
		npc->y += npc->ym / 2;
	} else {
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	Rect rect_left[6] = {
		{208, 104, 224, 120},
		{224, 104, 240, 120},
		{240, 104, 256, 120},
		{80, 176, 96, 192},
		{96, 176, 112, 192},
		{112, 176, 128, 192},
	};

	int offset = 0;
	if (npc->act_wait < 40){
		offset = 3;
		if (npc->ani_no == 0)
			npc->ani_no = 3;
	}

	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2 + offset)
		npc->ani_no = 0 + offset;

	npc->rect = rect_left[npc->ani_no];

	if (++npc->count1 > 150)
	{
		npc->cond = 0;
		SetCaret(npc->x, npc->y, 2, 0);
	}
}

// Beetle (destroyed Egg Corridor)
void ActNpc210(NPCHAR *npc)
{
	Rect rcLeft[2] = {
		{0, 112, 16, 128},
		{16, 112, 32, 128},
	};

	Rect rcRight[2] = {
		{32, 112, 48, 128},
		{48, 112, 64, 128},
	};

	switch (npc->act_no)
	{
		case 0:
			if (gMC.x < npc->x + (16 * 0x200) && gMC.x > npc->x - (16 * 0x200))
			{
				npc->bits |= NPC_SHOOTABLE;
				npc->ym = -0x200;
				npc->tgt_y = npc->y;
				npc->act_no = 1;
				npc->damage = 2;

				if (npc->direct == 0)
				{
					npc->x = gMC.x + (256 * 0x200);
					npc->xm = -0x2FF;
				}
				else
				{
					npc->x = gMC.x - (256 * 0x200);
					npc->xm = 0x2FF;
				}
			}
			else
			{
				npc->bits &= ~NPC_SHOOTABLE;
				npc->rect.right = 0;
				npc->damage = 0;
				npc->xm = 0;
				npc->ym = 0;
				return;
			}

			break;

		case 1:
			if (npc->x > gMC.x)
			{
				npc->direct = 0;
				npc->xm -= 0x10;
			}
			else
			{
				npc->direct = 2;
				npc->xm += 0x10;
			}

			if (npc->xm > 0x2FF)
				npc->xm = 0x2FF;
			if (npc->xm < -0x2FF)
				npc->xm = -0x2FF;

			if (npc->y < npc->tgt_y)
				npc->ym += 8;
			else
				npc->ym -= 8;

			if (npc->ym > 0x200)
				npc->ym = 0x200;
			if (npc->ym < -0x200)
				npc->ym = -0x200;

			if (npc->shock)
			{
				npc->x += npc->xm / 2;
				npc->y += npc->ym / 2;
			}
			else
			{
				npc->x += npc->xm;
				npc->y += npc->ym;
			}

			break;
	}

	if (++npc->ani_wait > 1)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 1)
		npc->ani_no = 0;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Spikes (small)
void ActNpc211(NPCHAR *npc)
{
	Rect rects[4] = {
		{256, 200, 272, 216},
		{272, 200, 288, 216},
		{288, 200, 304, 216},
		{304, 200, 320, 216},
	};

	npc->rect = rects[npc->code_event];
}

// Sky Dragon
void ActNpc212(NPCHAR *npc)
{
	Rect rcRight[4] = {
		{160, 152, 200, 192},
		{200, 152, 240, 192},
		{240, 112, 280, 152},
		{280, 112, 320, 152},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->y -= 4 * 0x200;
			// Fallthrough
		case 1:
			if (++npc->ani_wait > 30)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			npc->tgt_y = npc->y - (16 * 0x200);
			npc->tgt_x = npc->x - (6 * 0x200);
			npc->ym = 0;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 11:
			if (npc->x < npc->tgt_x)
				npc->xm += 8;
			else
				npc->xm -= 8;

			if (npc->y < npc->tgt_y)
				npc->ym += 8;
			else
				npc->ym -= 8;

			npc->x += npc->xm;
			npc->y += npc->ym;

			if (++npc->ani_wait > 5)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 2;

			break;

		case 20:
			npc->act_no = 21;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 21:
			if (npc->y < npc->tgt_y)
				npc->ym += 0x10;
			else
				npc->ym -= 0x10;

			npc->xm += 0x20;

			if (npc->xm > 0x600)
				npc->xm = 0x600;
			if (npc->xm < -0x600)
				npc->xm = -0x600;

			npc->x += npc->xm;
			npc->y += npc->ym;

			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 2;

			break;

		case 30:
			npc->act_no = 31;
			SetNpChar(297, 0, 0, 0, 0, 0, npc, 0x100);
			break;
	}

	npc->rect = rcRight[npc->ani_no];
}

// Night Spirit
void ActNpcWindController(NPCHAR *npc)
{
	Rect rect[10] = {
		{0, 0, 0, 0},
		{0, 0, 48, 48},
		{48, 0, 96, 48},
		{96, 0, 144, 48},
		{144, 0, 192, 48},
		{192, 0, 240, 48},
		{240, 0, 288, 48},
		{0, 48, 48, 96},
		{48, 48, 96, 96},
		{96, 48, 144, 96},
	};



	switch (npc->act_no)
	{
		case 0:
			npc->ani_no = 0;
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			// Fallthrough
		case 1:
			if (gMC.y > npc->y - (8 * 0x200) && gMC.y < npc->y + (8 * 0x200))
			{
				if (npc->direct == 0)
					npc->y -= 240 * 0x200;
				else
					npc->y += 240 * 0x200;

				npc->act_no = 10;
				npc->act_wait = 0;
				npc->ani_no = 1;
				npc->ym = 0;
			}

			break;

		case 10:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 1;

			if (++npc->act_wait > 200)
			{
				npc->act_no = 20;
				npc->act_wait = 0;
				npc->ani_no = 4;
			}

			break;

		case 20:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 6)
				npc->ani_no = 4;

			if (++npc->act_wait > 50)
			{
				npc->act_no = 30;
				npc->act_wait = 0;
				npc->ani_no = 7;
			}

			break;

		case 30:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 9)
				npc->ani_no = 7;

			if (++npc->act_wait % 12 == 1)
			{
				SetNpChar(214, npc->x, npc->y, (Random(3, 12) * 0x200) / 4, Random(-0x300, 0x300), 0, NULL, 0x100);
			}

			if (npc->act_wait > 100)
			{
				npc->act_no = 10;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 40:
			if (npc->y < npc->tgt_y)
				npc->ym += 0x40;
			else
				npc->ym -= 0x40;

			if (npc->ym < -0x400)
				npc->ym = -0x400;
			if (npc->ym > 0x400)
				npc->ym = 0x400;

			if (npc->shock)
				npc->y += npc->ym / 2;
			else
				npc->y += npc->ym;

			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 6)
				npc->ani_no = 4;

			if (gMC.y < npc->tgt_y + (240 * 0x200) && gMC.y > npc->tgt_y - (240 * 0x200))
			{
				npc->act_no = 20;
				npc->act_wait = 0;
				npc->ani_no = 4;
			}

			break;
	}

	npc->y = npc->tgt_y = gMC.y + Random(-24_pix, 24_pix);
	npc->x = gFrame.x + ScreenToSubpixelCoord(WINDOW_WIDTH)*2.5f;

	if (npc->act_no >= 10 && npc->act_no <= 30)
	{
		if (npc->y < gMC.y)
			npc->ym += 25;
		else
			npc->ym -= 25;

		if (npc->ym < -0x400)
			npc->ym = -0x400;
		if (npc->ym > 0x400)
			npc->ym = 0x400;

		if (npc->flag & 2)
			npc->ym = 0x200;
		if (npc->flag & 8)
			npc->ym = -0x200;

		if (npc->shock)
			npc->y += npc->ym / 2;
		else
			npc->y += npc->ym;

		if (gMC.y > npc->tgt_y + (240 * 0x200) || gMC.y < npc->tgt_y - (240 * 0x200))
			npc->act_no = 40;
	}
}

// Night Spirit projectile
void ActNpcFreezeWind(NPCHAR *npc)
{
	Rect rect[3] = {
		{0, 240, 32, 256},
		{32, 240, 64, 256},
		{64, 240, 96, 256},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 1:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 2)
				npc->ani_no = 0;

			npc->xm -= 12;

			npc->x += npc->xm;
			npc->y += npc->ym;

			if (++npc->act_wait > 500)
			{
				npc->cond = 0;
			}

			break;
	}
	for (int i = 0; i < 5; i++)
	SetCaret(npc->x + Random(-npc->view.front, npc->view.back),
					 npc->y + Random(-npc->view.top, npc->view.bottom), 13, 0);
}

// Sandcroc (Outer Wall)
void ActNpc215(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			npc->ani_no = 0;
			npc->act_no = 1;
			npc->act_wait = 0;
			npc->tgt_y = npc->y;
			npc->bits &= ~NPC_SHOOTABLE;
			npc->bits &= ~NPC_INVULNERABLE;
			npc->bits &= ~NPC_SOLID_SOFT;
			npc->bits &= ~NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 1:
			if (gMC.x > npc->x - (12 * 0x200) && gMC.x < npc->x + (12 * 0x200) && gMC.y > npc->y && gMC.y < npc->y + (8 * 0x200))
			{
				npc->act_no = 15;
				npc->act_wait = 0;
			}

			break;

		case 15:
			if (++npc->act_wait > 10)
			{
				PlaySoundObject(102, SOUND_MODE_PLAY);
				npc->act_no = 20;
			}

			break;

		case 20:
			if (++npc->ani_wait > 3)
			{
				++npc->ani_no;
				npc->ani_wait = 0;
			}

			if (npc->ani_no == 3)
				npc->damage = 15;

			if (npc->ani_no == 4)
			{
				npc->bits |= NPC_SHOOTABLE;
				npc->act_no = 30;
				npc->act_wait = 0;
			}

			break;

		case 30:
			npc->bits |= NPC_SOLID_SOFT;
			npc->damage = 0;
			++npc->act_wait;

			if (npc->shock)
			{
				npc->act_no = 40;
				npc->act_wait = 0;
			}

			break;

		case 40:
			npc->bits |= NPC_IGNORE_SOLIDITY;
			npc->y += 1 * 0x200;

			if (++npc->act_wait == 32)
			{
				npc->bits &= ~NPC_SOLID_SOFT;
				npc->bits &= ~NPC_SHOOTABLE;
				npc->act_no = 50;
				npc->act_wait = 0;
			}

			break;

		case 50:
			if (npc->act_wait < 100)
			{
				++npc->act_wait;
			}
			else
			{
				npc->y = npc->tgt_y;
				npc->ani_no = 0;
				npc->act_no = 0;
			}

			break;
	}

	Rect rect[5] = {
		{0, 0, 0, 0},
		{0, 96, 48, 128},
		{48, 96, 96, 128},
		{96, 96, 144, 128},
		{144, 96, 192, 128},
	};

	npc->rect = rect[npc->ani_no];
}

// Debug Cat
void ActNpc216(NPCHAR *npc)
{
	Rect rect = {256, 192, 272, 216};

	npc->rect = rect;
}

// Itoh
void ActNpc217(NPCHAR *npc)
{
	Rect rect[8] = {
		{144, 64, 160, 80},
		{160, 64, 176, 80},
		{176, 64, 192, 80},
		{192, 64, 208, 80},
		{144, 80, 160, 96},
		{160, 80, 176, 96},
		{144, 80, 160, 96},
		{176, 80, 192, 96},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 10:
			npc->ani_no = 2;
			npc->xm = 0;
			break;

		case 20:
			npc->act_no = 21;
			npc->ani_no = 2;
			npc->xm += 0x200;
			npc->ym -= 0x400;
			break;

		case 21:
			if (npc->flag & 8)
			{
				npc->ani_no = 3;
				npc->act_no = 30;
				npc->act_wait = 0;
				npc->xm = 0;
				npc->tgt_x = npc->x;
			}

			break;

		case 30:
			npc->ani_no = 3;

			if (++npc->act_wait / 2 % 2)
				npc->x = npc->tgt_x + (1 * 0x200);
			else
				npc->x = npc->tgt_x;

			break;

		case 40:
			npc->act_no = 41;
			npc->ym = -0x200;
			npc->ani_no = 2;
			// Fallthrough
		case 41:
			if (npc->flag & 8)
			{
				npc->act_no = 42;
				npc->ani_no = 4;
			}

			break;

		case 42:
			npc->xm = 0;
			npc->ani_no = 4;
			break;

		case 50:
			npc->act_no = 51;
			npc->act_wait = 0;
			// Fallthrough
		case 51:
			if (++npc->act_wait > 32)
				npc->act_no = 42;

			npc->xm = 0x200;

			if (++npc->ani_wait > 3)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 7)
				npc->ani_no = 4;

			break;
	}

	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];
}

// Core giant energy ball projectile
void ActNpc218(NPCHAR *npc)
{
	Rect rc[2] = {
		{208, 120, 239, 152},
		{240, 120, 272, 152},
	};
	if (npc->flag & 0x100){
		npc->x += npc->xm / 2;
		npc->y += npc->ym / 2;
	} else {
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	if (npc->flag & 0xF)
	{
		npc->hit.back = 32 * 0x200;
		npc->hit.front = 32 * 0x200;
		npc->hit.top = 32 * 0x200;
		npc->hit.bottom = 32 * 0x200;
		npc->damage = 20;
		PlaySoundObject(35, SOUND_MODE_PLAY);
		SetDestroyNpChar(npc->x, npc->y, 32_pix, 90);
		SetQuake(20);
		npc->cond |= 8;
	}

	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 1)
		npc->ani_no = 0;

	npc->rect = rc[npc->ani_no];
}

// Smoke generator
void ActNpc219(NPCHAR *npc)
{
	Rect rc = {0, 0, 0, 0};

	if (npc->direct == 0)
	{
		if (Random(0, 40) == 1)
			SetNpChar(4, npc->x + (Random(-20, 20) * 0x200), npc->y, 0, -0x200, 0, NULL, 0x100);
	}
	else
	{
		SetNpChar(199, npc->x + (Random(-0xA0, 0xA0) * 0x200), npc->y + (Random(-0x80, 0x80) * 0x200), 0, 0, 2, NULL, 0x100);
	}

	npc->rect = rc;
}
