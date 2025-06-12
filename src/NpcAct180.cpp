#include "CommonDefines.h"
#include "NpcAct.h"

#include <cstddef>

#include "Back.h"
#include "Bullet.h"
#include "Caret.h"
#include "Flags.h"
#include "Frame.h"
#include "Game.h"
#include "KeyControl.h"
#include "Map.h"
#include "MyChar.h"
#include "NpChar.h"
#include "NpcHit.h"
#include "Sound.h"
#include "Stage.h"
#include "Triangle.h"

// Curly AI
void ActNpcLittleMan(NPCHAR *npc)
{
	auto coords = MyCharCoords();
	bool bonked = false;

	Rect rcLeft[11] = {
		{0, 96, 16, 112},
		{16, 96, 32, 112},
		{0, 96, 16, 112},
		{32, 96, 48, 112},
		{0, 96, 16, 112},
		{48, 96, 64, 112}
	};

	Rect rcRight[11] = {
		{0, 112, 16, 128},
		{16, 112, 32, 128},
		{0, 112, 16, 128},
		{32, 112, 48, 128},
		{0, 112, 16, 128},
		{48, 112, 64, 128}
	};

	if (gCurlyShoot != NULL && npc->act_no == 1){
		npc->tgt_x = gCurlyShoot->x;
		npc->tgt_y = gCurlyShoot->y;
		npc->act_no = 2;
	} else {
		npc->tgt_x = gMC.x + gMC.xm*30 + Random(-6_pix, 6_pix);
		npc->tgt_y = gMC.y + gMC.ym*10;
		npc->act_no = 1;
		NPCHAR* passiveTarget;
		if ((passiveTarget = GetClosest(npc->x, npc->y, 84_pix)) != NULL && !IsObstructed(npc->x, npc->y, passiveTarget->x, passiveTarget->y) && (passiveTarget->damage > 0 || CheckNPCTrait(passiveTarget, NPCTraits::BONKABLE)) && !CheckNPCTrait(passiveTarget, NPCTraits::INANIMATE)){
			gCurlyShoot = passiveTarget;
		}
	}

	if (CheckFlagTrait(FlagTraits::CONTROLLED_LITTLE_MAN)){
	if (gKey & gKeyLeft){
		npc->xm -= 0x14 * 6;
	}
	if (gKey & gKeyRight){
		npc->xm += 0x14 * 6;
	}
	}

	int acceleration;

	switch (npc->act_no)
	{
		case 0:
			npc->x = coords.first;
			npc->y = coords.second;
			npc->act_no = 1;
			npc->ani_no = 0;
			gCurlyShoot = NULL;
			SetNpChar(183, 0, 0, 0, 0, 0, npc, 0x100);

			break;
		case 1:
			acceleration = 0x50;
			if (!(npc->flag & 8))
				acceleration /= 4;
			if (npc->x > npc->tgt_x)
			{
				npc->xm -= acceleration;
			}
			else
			{
				npc->xm += acceleration;
			}

			npc->direct = gMC.direct;

			if (npc->flag & 1){
				bonked = true;
			}

			if (npc->flag & 4){
				bonked = true;
			}

			npc->ym += 0x20;

			if (npc->xm > 0x3A0)
				npc->xm = 0x3A0;
			if (npc->xm < -0x3A0)
				npc->xm = -0x3A0;

			if (bonked && npc->flag & 8){
				npc->ym = -0x500;
				npc->xm = 0;
				bonked = false;
				PlaySoundObject(15);
			}

			break;

		case 2:
			int acceleration = 0x70;
			if (npc->x > gCurlyShoot->x + gCurlyShoot->xm * 3)
			{
				npc->xm -= acceleration;
				npc->direct = 0;
			}
			else
			{
				npc->xm += acceleration;
				npc->direct = 1;
			}

			if (npc->flag & 1){
				bonked = true;
			}

			if (npc->flag & 4){
				bonked = true;
			}

			npc->ym += 0xA;
			if (npc->y < gCurlyShoot->y)
				npc->ym += 0x50;

			if (npc->xm > 0x5FF)
				npc->xm = 0x5FF;
			if (npc->xm < -0x5FF)
				npc->xm = -0x5FF;

			if ((bonked || Distance(npc->x, npc->y, gCurlyShoot->x, gCurlyShoot->y) < 0x4000) && npc->flag & 8){
				npc->ym = -0x550;
				npc->xm = 0;
				bonked = false;
				PlaySoundObject(15);
			}

			if (gCurlyShoot == NULL || IsObstructed(npc->x, npc->y, gCurlyShoot->x, gCurlyShoot->y) || !(gCurlyShoot->cond & 0x80)){
				npc->act_no = 1;
				gCurlyShoot = NULL;
			}

			break;
	}

	int tpDistance = (gCurlyShoot != NULL ? 13 : 8) * 16_pix;

	if (Distance(npc->x, npc->y, coords.first, coords.second) > tpDistance){
		npc->x = coords.first;
		npc->y = coords.second;
		SetNpChar(4, coords.first, coords.second, 0, 0, 0, NULL, 0x100);
		gCurlyShoot = NULL;
		npc->act_no = 1;
	}

	if (npc->flag & 8 && (std::abs(npc->y - npc->tgt_y) >= 24_pix || (CheckFlagTrait(FlagTraits::CONTROLLED_LITTLE_MAN) && gKeyTrg & gKeyJump)))
	{
		npc->ym = -0x600;
		npc->ani_no = 1;
		PlaySoundObject(15);
	}

	if (CheckFlagTrait(FlagTraits::SHOOTING_LITTLE_MAN) && 
	npc->exp % GetFlagTraitData<int>(FlagTraits::SHOOTING_LITTLE_MAN, "rate", 9) == 0 
	&& npc->xm != 0 && gCurlyShoot != NULL){
		BULLET *bul = SetBullet(GetFlagTraitData<int>(FlagTraits::SHOOTING_LITTLE_MAN, "bullet"), npc->x,  npc->y);
		if (bul != NULL){
			bul->tgt_x = gCurlyShoot->x + Random(-0x2000 * 3.5, 0x2000 * 3.5);
			bul->tgt_y = gCurlyShoot->y + Random(-0x2000 * 3.5, 0x2000 * 3.5);
		}
	}

	if (npc->flag & 8)
	{
		++npc->ani_wait;

		npc->ani_no = 1 + (npc->ani_wait / 4 % 4);
	} else {
		npc->ani_no = 0;
	}

	if (++npc->exp % 15 == 0){
		if (npc->exp % 45 == 0 && gCurlyShoot != NULL)
			SetNpChar(333, gCurlyShoot->x, gCurlyShoot->y, 0, 0, 3, NULL, 0x100);
		NPCHAR* target = FindNpCharHere(npc->x + npc->xm, npc->y + npc->ym, 12_pix, 12_pix);
		if (target != NULL){
			if (target->code_char != 69 && target->code_char != 156){
				DealDamage(*target, 6);
				npc->ym = -0x480;
				npc->xm = 0;
			}
		}
	}

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Curly AI Machine Gun
void ActNpc181(NPCHAR *npc)
{
	Rect rcLeft[2] = {
		{216, 152, 232, 168},
		{232, 152, 248, 168},
	};

	Rect rcRight[2] = {
		{216, 168, 232, 184},
		{232, 168, 248, 184},
	};

	if (npc->pNpc == NULL)
		return;

	if (npc->pNpc->ani_no < 5)
	{
		if (npc->pNpc->direct == 0)
		{
			npc->direct = 0;
			npc->x = npc->pNpc->x - (8 * 0x200);
		}
		else
		{
			npc->direct = 2;
			npc->x = npc->pNpc->x + (8 * 0x200);
		}

		npc->y = npc->pNpc->y;
		npc->ani_no = 0;
	}
	else
	{
		if (npc->pNpc->direct == 0)
		{
			npc->direct = 0;
			npc->x = npc->pNpc->x;
		}
		else
		{
			npc->direct = 2;
			npc->x = npc->pNpc->x;
		}

		npc->y = npc->pNpc->y - (10 * 0x200);
		npc->ani_no = 1;
	}

	if (npc->pNpc->ani_no == 1 || npc->pNpc->ani_no == 3 || npc->pNpc->ani_no == 6 || npc->pNpc->ani_no == 8)
		npc->y -= 1 * 0x200;

	switch (npc->act_no)
	{
		case 0:
			if (npc->pNpc->count2 == 10)
			{
				npc->pNpc->count2 = 0;
				npc->act_no = 10;
				npc->act_wait = 0;
			}

			break;

		case 10:
			if (++npc->act_wait % 6 == 1)
			{
				if (npc->ani_no == 0)
				{
					if (npc->direct == 0)
					{
						SetBullet(Bullets::MACHINE_GUN_LVL_3, npc->x - (4 * 0x200), npc->y + (3 * 0x200), 0);
						SetCaret(npc->x - (4 * 0x200), npc->y + (3 * 0x200), 3, 0);
					}
					else
					{
						SetBullet(Bullets::MACHINE_GUN_LVL_3, npc->x + (4 * 0x200), npc->y + (3 * 0x200), 2);
						SetCaret(npc->x + (4 * 0x200), npc->y + (3 * 0x200), 3, 0);
					}
				}
				else
				{
					if (npc->direct == 0)
					{
						SetBullet(Bullets::MACHINE_GUN_LVL_3, npc->x - (2 * 0x200), npc->y - (4 * 0x200), 1);
						SetCaret(npc->x - (2 * 0x200), npc->y - (4 * 0x200), 3, 0);
					}
					else
					{
						SetBullet(Bullets::MACHINE_GUN_LVL_3, npc->x + (2 * 0x200), npc->y - (4 * 0x200), 1);
						SetCaret(npc->x + (2 * 0x200), npc->y - (4 * 0x200), 3, 0);
					}
				}
			}

			if (npc->act_wait == 60)
				npc->act_no = 0;

			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Curly AI Polar Star
void ActNpc182(NPCHAR *npc)
{
	Rect rcLeft[2] = {
		{184, 152, 200, 168},
		{200, 152, 216, 168},
	};

	Rect rcRight[2] = {
		{184, 168, 200, 184},
		{200, 168, 216, 184},
	};

	if (npc->pNpc == NULL)
		return;

	if (npc->pNpc->ani_no < 5)
	{
		if (npc->pNpc->direct == 0)
		{
			npc->direct = 0;
			npc->x = npc->pNpc->x - (8 * 0x200);
		}
		else
		{
			npc->direct = 2;
			npc->x = npc->pNpc->x + (8 * 0x200);
		}

		npc->y = npc->pNpc->y;
		npc->ani_no = 0;
	}
	else
	{
		if (npc->pNpc->direct == 0)
		{
			npc->direct = 0;
			npc->x = npc->pNpc->x;
		}
		else
		{
			npc->direct = 2;
			npc->x = npc->pNpc->x;
		}

		npc->y = npc->pNpc->y - (10 * 0x200);
		npc->ani_no = 1;
	}

	if (npc->pNpc->ani_no == 1 || npc->pNpc->ani_no == 3 || npc->pNpc->ani_no == 6 || npc->pNpc->ani_no == 8)
		npc->y -= 1 * 0x200;

	switch (npc->act_no)
	{
		case 0:
			if (npc->pNpc->count2 == 10)
			{
				npc->pNpc->count2 = 0;
				npc->act_no = 10;
				npc->act_wait = 0;
			}

			break;

		case 10:
			if (++npc->act_wait % 12 == 1)
			{
				if (npc->ani_no == 0)
				{
					if (npc->direct == 0)
					{
						SetBullet(Bullets::SPUR_LVL_0, npc->x - (4 * 0x200), npc->y + (3 * 0x200), 0);
						SetCaret(npc->x - (4 * 0x200), npc->y + (3 * 0x200), 3, 0);
					}
					else
					{
						SetBullet(Bullets::SPUR_LVL_0, npc->x + (4 * 0x200), npc->y + (3 * 0x200), 2);
						SetCaret(npc->x + (4 * 0x200), npc->y + (3 * 0x200), 3, 0);
					}
				}
				else
				{
					if (npc->direct == 0)
					{
						SetBullet(Bullets::SPUR_LVL_0, npc->x - (2 * 0x200), npc->y - (4 * 0x200), 1);
						SetCaret(npc->x - (2 * 0x200), npc->y - (4 * 0x200), 3, 0);
					}
					else
					{
						SetBullet(Bullets::SPUR_LVL_0, npc->x + (2 * 0x200), npc->y - (4 * 0x200), 1);
						SetCaret(npc->x + (2 * 0x200), npc->y - (4 * 0x200), 3, 0);
					}
				}
			}

			if (npc->act_wait == 60)
				npc->act_no = 0;

			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Curly Air Tank Bubble
void ActNpc183(NPCHAR *npc)
{
	Rect rect[2] = {
		{56, 96, 80, 120},
		{80, 96, 104, 120},
	};

	if (npc->pNpc == NULL)
		return;

	switch (npc->act_no)
	{
		case 0:
			npc->x = npc->pNpc->x;
			npc->y = npc->pNpc->y;
			npc->act_no = 1;
			break;
	}

	npc->x += (npc->pNpc->x - npc->x) / 2;
	npc->y += (npc->pNpc->y - npc->y) / 2;

	if (++npc->ani_wait > 1)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 1)
		npc->ani_no = 0;

	if (npc->pNpc->flag & 0x100)
		npc->rect = rect[npc->ani_no];
	else
		npc->rect.right = 0;
}

// Big Shutter
void ActNpc184(NPCHAR *npc)
{
	int i;

	Rect rc[4] = {
		{0, 64, 32, 96},
		{32, 64, 64, 96},
		{64, 64, 96, 96},
		{32, 64, 64, 96},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->x += 8 * 0x200;
			npc->y += 8 * 0x200;
			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 1;
			npc->act_wait = 0;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 11:
			switch (npc->direct)
			{
				case 0:
					npc->x -= 0x80;
					break;

				case 1:
					npc->y -= 0x80;
					break;

				case 2:
					npc->x += 0x80;
					break;

				case 3:
					npc->y += 0x80;
					break;
			}

			if ((++npc->act_wait % 8) == 0)
				PlaySoundObject(26, SOUND_MODE_PLAY);

			SetQuake(20);
			break;

		case 20:
			for (i = 0; i < 4; ++i)
				SetNpChar(4, npc->x + (Random(-12, 12) * 0x200), npc->y + (16 * 0x200), Random(-0x155, 0x155), Random(-0x600, 0), 0, NULL, 0x100);

			npc->act_no = 1;
			break;
	}

	if (++npc->ani_wait > 10)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 3)
		npc->ani_no = 0;

	npc->rect = rc[npc->ani_no];
}

// Small Shutter
void ActNpc185(NPCHAR *npc)
{
	Rect rc = {96, 64, 112, 96};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->y += 8 * 0x200;
			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 1;
			npc->act_wait = 0;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 11:
			switch (npc->direct)
			{
				case 0:
					npc->x -= 0x80;
					break;

				case 1:
					npc->y -= 0x80;
					break;

				case 2:
					npc->x += 0x80;
					break;

				case 3:
					npc->y += 0x80;
					break;
			}

			++npc->act_wait;
			break;

		case 20:
			npc->y -= 24 * 0x200;
			npc->act_no = 1;
			break;
	}

	npc->rect = rc;
}

// Lift block
void ActNpc186(NPCHAR *npc)
{
	Rect rc[4] = {
		{48, 48, 64, 64},
		{64, 48, 80, 64},
		{80, 48, 96, 64},
		{64, 48, 80, 64},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			// Fallthrough
		case 1:
			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 1;
			npc->act_wait = 0;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 11:
			switch (npc->direct)
			{
				case 0:
					npc->x -= 0x80;
					break;

				case 1:
					npc->y -= 0x80;
					break;

				case 2:
					npc->x += 0x80;
					break;

				case 3:
					npc->y += 0x80;
					break;
			}

			++npc->act_wait;
			break;
	}

	if (++npc->ani_wait > 10)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 3)
		npc->ani_no = 0;

	npc->rect = rc[npc->ani_no];
}

// Fuzz Core
void ActNpcSteelCore(NPCHAR *npc)
{
	int i;

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			npc->count1 = 120;
			npc->act_wait = Random(0, 50);
			if (gMC.difficulty > DIFFICULTY_HARDENED){
				for (i = 0; i < 18; ++i)
					SetNpChar(188, 0, 0, 0, 0, 14 * i, npc, 0x100);
			} else {
				for (i = 0; i < 10; ++i)
					SetNpChar(188, 0, 0, 0, 0, 26 * i, npc, 0x100);
			}
			// Fallthrough
		case 1:
			if (++npc->act_wait < 50)
				break;

			npc->act_wait = 0;
			npc->act_no = 2;
			npc->ym = 0x300;
			break;

		case 2:
			npc->count1 += 6;

			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			if (npc->tgt_y < npc->y)
				npc->ym -= 0x30;
			if (npc->tgt_y > npc->y)
				npc->ym += 0x30;

			if (npc->ym > 0x355)
				npc->ym = 0x355;
			if (npc->ym < -0x355)
				npc->ym = -0x355;

			break;
	}

	if (npc->flag & 0x100){
		npc->x += npc->xm / 2;
		npc->y += npc->ym / 2;
	} else {
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	Rect rect_left[2] = {
		{224, 104, 256, 136},
		{256, 104, 288, 136},
	};

	Rect rect_right[2] = {
		{224, 136, 256, 168},
		{256, 136, 288, 168},
	};

	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 1)
		npc->ani_no = 0;

	if (npc->direct == 0)
		npc->rect = rect_left[npc->ani_no];
	else
		npc->rect = rect_right[npc->ani_no];
}

// Fuzz
void ActNpcSteelFuzz(NPCHAR *npc)
{
	unsigned char deg;

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->count1 = npc->direct;
			npc->act_wait = npc->direct*3;
			if (gMC.difficulty > DIFFICULTY_HARDENED)
				npc->act_wait = npc->direct*4;
			// Fallthrough
		case 1:
			if (npc->pNpc->code_char == 187 && npc->pNpc->cond & 0x80)
			{
				deg = (npc->pNpc->count1 + npc->count1) % 0x100;
				npc->x = npc->pNpc->x + (GetSin(deg) * 0x20);
				npc->y = npc->pNpc->y + (GetCos(deg) * 0x20);
				if (gMC.difficulty > DIFFICULTY_HARDENED){
					npc->x = npc->pNpc->x + (GetSin(deg) * 0x32);
					npc->y = npc->pNpc->y + (GetCos(deg) * 0x32);
				}

				if (++npc->act_wait % 375 == 0 && 
					Distance(gMC.x, gMC.y, npc->x, npc->y) < 200_pix &&
						Distance(gMC.x, gMC.y, npc->x, npc->y) > 48_pix){
					deg = GetArktan(npc->x - gMC.x, npc->y - gMC.y);
					deg += Random(-0x12, 0x12);
					int ym = GetSin(deg) * 6;
					int xm = GetCos(deg) * 6;
					SetNpChar(209, npc->x, npc->y, xm, ym, 0, NULL, 0x100);

				if (!(gMC.cond & 2))
					PlaySoundObject(61, SOUND_MODE_PLAY);
				}
			}
			else
			{
				SetDestroyNpChar(npc->x, npc->y, 0x2000, 6);
				npc->cond = 0;
			}

			break;

		case 10:
			if (gMC.x < npc->x)
				npc->xm -= 0x20;
			else
				npc->xm += 0x20;

			if (gMC.y < npc->y)
				npc->ym -= 0x20;
			else
				npc->ym += 0x20;

			if (npc->xm > 0x800)
				npc->xm = 0x800;
			if (npc->xm < -0x800)
				npc->xm = -0x800;

			if (npc->ym > 0x200)
				npc->ym = 0x200;
			if (npc->ym < -0x200)
				npc->ym = -0x200;

			if (npc->flag & 0x100){
				npc->x += npc->xm / 2;
				npc->y += npc->ym / 2;
			} else {
				npc->x += npc->xm;
				npc->y += npc->ym;
			}

			break;
	}

	if (gMC.x < npc->x)
		npc->direct = 0;
	else
		npc->direct = 2;

	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 1)
		npc->ani_no = 0;

	Rect rect_left[2] = {
		{288, 104, 304, 120},
		{304, 104, 320, 120},
	};

	Rect rect_right[2] = {
		{288, 120, 304, 136},
		{304, 120, 320, 136},
	};

	if (npc->direct == 0)
		npc->rect = rect_left[npc->ani_no];
	else
		npc->rect = rect_right[npc->ani_no];
}

// Unused homing flame object (possibly related to the Core?)
void ActNpc189(NPCHAR *npc)
{
	auto coords = MyCharCoords();

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->xm = -0x40;
			npc->x = coords.first;
			npc->y = coords.second + 0x2000*1;
			// Fallthrough
		case 1:
			npc->y += npc->ym;

			if (++npc->act_wait > 0x20)
				npc->act_no = 10;

			break;

		case 10:
			if (coords.first < npc->x)
				npc->xm -= Random(10, 20);
			else
				npc->xm += Random(10, 20);

			if (coords.second < npc->y)
				npc->ym -= Random(10, 20);
			else
				npc->ym += Random(10, 20);

			if (npc->xm > 0x300)
				npc->xm = 0x300;
			if (npc->xm < -0x300)
				npc->xm = -0x300;

			if (npc->ym > 0x300)
				npc->ym = 0x300;
			if (npc->ym < -0x300)
				npc->ym = -0x300;

			if (npc->flag & 0x100){
				npc->x += npc->xm / 2;
				npc->y += npc->ym / 2;
			} else {
				npc->x += npc->xm;
				npc->y += npc->ym;
			}

			break;
	}

	if (coords.first < npc->x)
		npc->direct = 0;
	else
		npc->direct = 2;

	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	npc->rect = {224, 184, 232, 200};
}

// Broken robot
void ActNpc190(NPCHAR *npc)
{
	ActNpc189(npc);

	npc->rect = {232, 184, 240, 200};
}

// Water level
void ActNpc191(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 20;
			// Fallthrough
		case 20:
			npc->act_no = 21;
			npc->act_wait = 0;
			// Fallthrough
		case 21:
			if (npc->y < gMap.length * 0x2000 * 0.85)
				npc->ym += 6;
			else
				npc->ym -= 6;

			if (npc->ym < -0x300)
				npc->ym = -0x300;
			if (npc->ym > 0x300)
				npc->ym = 0x300;

			npc->y += npc->ym;

			if (++npc->act_wait > 1000){
				npc->act_no = 22;
				npc->act_wait = 0;
			}

			break;

		case 22:
			if (npc->y < gMap.length * 0x2000 * 0.3)
				npc->ym += 6;
			else
				npc->ym -= 6;

			if (npc->ym < -0x300)
				npc->ym = -0x300;
			if (npc->ym > 0x300)
				npc->ym = 0x300;

			npc->y += npc->ym;

			if (++npc->act_wait > 1000)
			{
				npc->act_no = 21;
				npc->act_wait = 0;
			}
			break;

		case 30:
			if (npc->y < 0)
				npc->ym += 4;
			else
				npc->ym -= 4;

			if (npc->ym < -0x200)
				npc->ym = -0x200;
			if (npc->ym > 0x100)
				npc->ym = 0x100;

			npc->y += npc->ym;
			break;
	}

	gWaterY = npc->y;

	npc->rect.right = 0;
	npc->rect.bottom = 0;
}

// Scooter
void ActNpc192(NPCHAR *npc)
{
	ActNpc189(npc);

	npc->rect = {240, 184, 248, 200};
}

// Scooter (broken)
void ActNpc193(NPCHAR *npc)
{
	ActNpc189(npc);

	npc->rect = {216, 192, 223, 200};
}

// Blue robot (broken)
void ActNpc194(NPCHAR *npc)
{
	ActNpc189(npc);

	npc->rect = {248, 192, 256, 199};
}

// Grate
void ActNpc195(NPCHAR *npc)
{
	ActNpc189(npc);

	npc->rect = {216, 183, 224, 192};
}

// Ironhead motion wall
void ActNpc196(NPCHAR *npc)
{
	npc->x = (13) * GetSin(npc->exp % 256) + gMC.x + gMC.xm + 0x200 * 4;
	npc->y = (13) * GetCos(npc->exp % 256) + gMC.y + gMC.ym + 0x200 * 2;
	bool lol = false;
	for (int n = 0; n < NPC_MAX; n++){
		if (npc->x + npc->hit.front >= gNPC[n].x - gNPC[n].hit.front
			&& npc->x - npc->hit.back <= gNPC[n].x + gNPC[n].hit.back
			&& npc->y + npc->hit.top >= gNPC[n].y - gNPC[n].hit.top
			&& npc->y - npc->hit.bottom <= gNPC[n].y + gNPC[n].hit.bottom && gNPC[n].code_char != 196
			&& gNPC[n].bits & NPC_SHOOTABLE){
				if (gNPC[n].shock < 12){
					SetBullet(Bullets::UNUSED_SHIELD_HITBOX, gNPC[n].x, gNPC[n].y, 0);
				}
			}
	}
	npc->exp += 20;

	npc->rect = {107, 51, 117, 61};
}

// Porcupine Fish
void ActNpc197(NPCHAR *npc)
{
	Rect rc[4] = {
		{0, 0, 16, 16},
		{16, 0, 32, 16},
		{32, 0, 48, 16},
		{48, 0, 64, 16},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 10;
			npc->ani_wait = 0;
			npc->ym = Random(-0x200, 0x200);
			npc->xm = 0x800;
			// Fallthrough
		case 10:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (npc->xm < 0)
			{
				npc->damage = 0;
				npc->act_no = 20;
				npc->act_wait = Random(20, 30);
				npc->bits &= ~NPC_SHOOTABLE;
				npc->bits |= NPC_INVULNERABLE;
			}

			break;

		case 20:
			npc->bits &= ~NPC_SHOOTABLE;
			npc->bits |= NPC_INVULNERABLE;

			if (++npc->ani_wait > 0)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 2;

			if (++npc->act_wait == 70){
				int deg = GetArktan(npc->x - gMC.x, npc->y - gMC.y);
				deg += (unsigned char)Random(-10, 10);
				int ym = GetSin(deg) * 4;
				int xm = GetCos(deg) * 4;
				NPCHAR* shot = CreateNpChar(11, npc->x, npc->y, xm, ym, 0, NULL, 0x100);
				PlaySoundObject(39, SOUND_MODE_PLAY);
				shot->life = 100;
			}

			if (npc->x < 48 * 0x200)
			{
				npc->destroy_voice = 0;
				LoseNpChar(npc, true);
			}

			break;
	}

	if (npc->flag & 2)
		npc->ym = 0x200;
	if (npc->flag & 8)
		npc->ym = -0x200;

	npc->xm -= 12;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];
}

// Ironhead projectile
void ActNpc198(NPCHAR *npc)
{
	Rect rcRight[3] = {
		{208, 48, 224, 72},
		{224, 48, 240, 72},
		{240, 48, 256, 72},
	};

	switch (npc->act_no)
	{
		case 0:
			if (++npc->act_wait > 20)
			{
				npc->act_no = 1;
				npc->xm = 0;
				npc->ym = 0;
				npc->count1 = 0;
			}

			break;

		case 1:
			npc->xm += 0x20;
			break;
	}

	if (++npc->ani_wait > 0)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2)
		npc->ani_no = 0;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rcRight[npc->ani_no];

	if (++npc->count1 > 100)
		npc->cond = 0;

	if (npc->count1 % 4 == 1)
		PlaySoundObject(46, SOUND_MODE_PLAY);
}

// Water/wind particles
void ActNpc199(NPCHAR *npc)
{
	Rect rect[5] = {
		{72, 16, 74, 18},
		{74, 16, 76, 18},
		{76, 16, 78, 18},
		{78, 16, 80, 18},
		{80, 16, 82, 18},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = Random(0, 2);

			switch (npc->direct)
			{
				case 0:
					npc->xm = -1;
					break;

				case 1:
					npc->ym = -1;
					break;

				case 2:
					npc->xm = 1;
					break;

				case 3:
					npc->ym = 1;
					break;
			}

			npc->xm *= (Random(4, 8) * 0x200) / 2;
			npc->ym *= (Random(4, 8) * 0x200) / 2;
			break;
	}

	if (++npc->ani_wait > 6)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 4)
	{
		npc->cond = 0;
	#ifdef FIX_BUGS
		return;	// The code below will use 'ani_no' to access 'rect', even though it's now too high
	#endif
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];
}
