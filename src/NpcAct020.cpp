#include "NpcAct.h"

#include <cstddef>

#include "BossLife.h"
#include "Caret.h"
#include "Game.h"
#include "Frame.h"
#include "MyChar.h"
#include "NpChar.h"
#include "Sound.h"
#include "Triangle.h"
#include "Stage.h"

// Computer
void ActNpcTerminal(NPCHAR *npc)
{
	Rect rcLeft = {288, 16, 320, 40};

	Rect rcRight[3] = {
		{288, 40, 320, 64},
		{288, 40, 320, 64},
		{288, 64, 320, 88},
	};

	if (++npc->ani_wait > 3)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2)
		npc->ani_no = 0;

	// Gravity
	if (gMC.tileset == 'F'){
			npc->ym += 0x20;
			if (npc->ym > 0x2FF)
				npc->ym = 0x2FF;
		} else {
	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;
		}
	if (npc->flag & 0x100){
		npc->x += npc->xm / 2;
		npc->y += npc->ym / 2;
	} else {
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft;
	else
		npc->rect = rcRight[npc->ani_no];
}

// Chest (open)
void ActNpcOpenChest(NPCHAR *npc)
{
	if (npc->act_no == 0)
	{
		npc->act_no = 1;

		if (npc->direct == 2)
			npc->y += 16 * 0x200;
	}

	Rect rect = {224, 40, 240, 48};

	// Gravity
	if (gMC.tileset == 'F'){
			npc->ym += 0x20;
			if (npc->ym > 0x2FF)
				npc->ym = 0x2FF;
		} else {
	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;
		}

	// Move
	if (npc->flag & 0x100){
		npc->x += npc->xm / 2;
		npc->y += npc->ym / 2;
	} else {
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	npc->rect = rect;
}

// Teleporter
void ActNpcTeleporter(NPCHAR *npc)
{
	Rect rect[2] = {
		{240, 16, 264, 48},
		{248, 152, 272, 184},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->ani_no = 0;
			break;

		case 1:
			if (++npc->ani_no > 1)
				npc->ani_no = 0;

			break;
	}

	npc->rect = rect[npc->ani_no];
}

// Teleporter lights
void ActNpc023(NPCHAR *npc)
{
	Rect rect[8] = {
		{264, 16, 288, 20},
		{264, 20, 288, 24},
		{264, 24, 288, 28},
		{264, 28, 288, 32},
		{264, 32, 288, 36},
		{264, 36, 288, 40},
		{264, 40, 288, 44},
		{264, 44, 288, 48},
	};

	if (++npc->ani_wait > 1)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 7)
		npc->ani_no = 0;

	npc->rect = rect[npc->ani_no];
}

// Power Critter
void ActNpcPowerCritter(NPCHAR *npc)
{
	Rect rcLeft[6] = {
		{0, 0, 24, 24},
		{24, 0, 48, 24},
		{48, 0, 72, 24},
		{72, 0, 96, 24},
		{96, 0, 120, 24},
		{120, 0, 144, 24},
	};

	Rect rcRight[6] = {
		{0, 24, 24, 48},
		{24, 24, 48, 48},
		{48, 24, 72, 48},
		{72, 24, 96, 48},
		{96, 24, 120, 48},
		{120, 24, 144, 48},
	};
	int waitTime;
	int detectRange1 = 128_pix;
	int detectRange2 = 96_pix;
	if (gMC.difficulty >= DIFFICULTY_REINFORCED){
		detectRange1 *= 1.25;
		detectRange2 *= 1.25;
	}

	switch (npc->act_no)
	{
		case 0:
			npc->y += 3 * 0x200;
			npc->act_no = 1;
			// Fallthrough
		case 1:
			if (npc->act_wait >= 8 && npc->x - detectRange1 < gMC.x && npc->x + detectRange1 > gMC.x && npc->y - detectRange1 < gMC.y && npc->y + (detectRange2 / 2) > gMC.y)
			{
				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;

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

			if (npc->act_wait >= 8 && npc->x - detectRange2 < gMC.x && npc->x + detectRange2 > gMC.x && npc->y - detectRange2 < gMC.y && npc->y + (detectRange2 / 2) > gMC.y)
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
				PlaySoundObject(108, SOUND_MODE_PLAY);

				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;

				if (npc->direct == 0)
					npc->xm = -0x100;
				else
					npc->xm = 0x100;
				if (gMC.difficulty >DIFFICULTY_HARDENED){
					npc->xm *= 3;
				}
			}

			break;

		case 3:
			if (npc->ym > 0x200)
			{
				npc->tgt_y = npc->y;
				npc->act_no = 4;
				npc->ani_no = 3;
				npc->act_wait = 0;
				npc->act_wait = 0;	// Pixel duplicated this line
			}

			break;

		case 4:
			if (npc->x < gMC.x)
				npc->direct = 2;
			else
				npc->direct = 0;

			++npc->act_wait;

			 waitTime = 100;
			if (gMC.difficulty > DIFFICULTY_HARDENED)
				waitTime = 0;

			if (npc->flag & 7 || npc->act_wait > waitTime)
			{
				npc->damage = 12;
				npc->act_no = 5;
				npc->ani_no = 2;
				if (gMC.difficulty < DIFFICULTY_REINFORCED)
					npc->xm /= 2;
				break;
			}

			if (npc->act_wait % 4 == 1)
				PlaySoundObject(110, SOUND_MODE_PLAY);

			if (++npc->ani_wait > 0)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 3;

			break;

		case 5:
			// if (gMC.difficulty > DIFFICULTY_HARDENED){
			// if (npc->x < gMC.x){
			// 	npc->direct = 2;
			// 	npc->xm += 0x40;
			// }
			// else {
			// 	npc->direct = 0;
			// 	npc->xm -= 0x40;
			// }
			// if (npc->xm > 0x300)
			// 	npc->xm = 0x300;
			// if (npc->xm < -0x300)
			// 	npc->xm = -0x300;
			// }
			if (npc->flag & 8)
			{
				npc->damage = 2;
				npc->xm = 0;
				npc->act_wait = 0;
				npc->ani_no = 0;
				npc->act_no = 1;
				PlaySoundObject(26, SOUND_MODE_PLAY);
				SetQuake(30);
			}

			break;
	}

	if (npc->act_no != 4)
	{
		int velocity = 0x20;
		int maxSpeed = 0x5FF;
		if (gMC.tileset == 'F'){
			velocity = 0x10;
			maxSpeed = 0x2FF;
		}
		npc->ym += velocity;
		if (npc->ym > maxSpeed)
			npc->ym = maxSpeed;
	}
	else
	{
		if (npc->x < gMC.x)
			npc->xm += 0x20;
		else
			npc->xm -= 0x20;

		if (npc->y > npc->tgt_y)
			npc->ym -= 0x10;
		else
			npc->ym += 0x10;

		if (npc->ym > 0x200)
			npc->ym = 0x200;
		if (npc->ym < -0x200)
			npc->ym = -0x200;

		if (npc->xm > 0x200)
			npc->xm = 0x200;
		if (npc->xm < -0x200)
			npc->xm = -0x200;
	}

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

// Egg Corridor lift
void ActNpc025(NPCHAR *npc)
{
	Rect rcLeft[2] = {
		{256, 64, 288, 80},
		{256, 80, 288, 96},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->x += 8 * 0x200;
			// Fallthrough
		case 1:
			if (++npc->act_wait > 150)
			{
				npc->act_wait = 0;
				++npc->act_no;
			}

			break;

		case 2:	// Identical to case 4
			if (++npc->act_wait <= 0x40)
			{
				npc->y -= 1 * 0x200;
			}
			else
			{
				npc->act_wait = 0;
				++npc->act_no;
			}

			break;

		case 3:
			if (++npc->act_wait > 150)
			{
				npc->act_wait = 0;
				++npc->act_no;
			}

			break;

		case 4:	// Identical to case 2
			if (++npc->act_wait <= 0x40)
			{
				npc->y -= 1 * 0x200;
			}
			else
			{
				npc->act_wait = 0;
				++npc->act_no;
			}

			break;

		case 5:
			if (++npc->act_wait > 150)
			{
				npc->act_wait = 0;
				++npc->act_no;
			}

			break;

		case 6:
			if (++npc->act_wait <= 0x40)
			{
				npc->y += 1 * 0x200;
			}
			else
			{
				npc->act_wait = 0;
				++npc->act_no;
			}

			break;

		case 7:
			if (++npc->act_wait > 150)
			{
				npc->act_wait = 0;
				++npc->act_no;
			}

			break;

		case 8:
			if (++npc->act_wait <= 0x40)
			{
				npc->y += 1 * 0x200;
			}
			else
			{
				npc->act_wait = 0;
				npc->act_no = 1;
			}

			break;
	}

	switch (npc->act_no)
	{
		case 2:
		case 4:
		case 6:
		case 8:
			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			break;
	}

	npc->rect = rcLeft[npc->ani_no];
}

// Bat (Grasstown, flying)
void ActNpcBatEnemy(NPCHAR *npc)
{
	unsigned char deg;

	int timer = 120;
	if (gMC.depth > genContext.doctorStage){
		timer = 60;
	}
	int lol, xspeed, yTrigger;

	switch (npc->act_no)
	{
		case 0:
			deg = Random(0, 0xFF);
			npc->xm = GetCos(deg);
			lol = 8;
			if (gMC.depth > genContext.doctorStage){
				npc->xm *= 2;
				lol *= 2;
			}
			deg += 0x40;
			npc->tgt_x = npc->x + (GetCos(deg) * lol);

			deg = Random(0, 0xFF);
			npc->ym = GetSin(deg);
			if (gMC.depth > genContext.doctorStage){
				npc->ym *= 2;
			}
			deg += 0x40;
			npc->tgt_y = npc->y + (GetSin(deg) * lol);

			npc->act_no = 1;
			npc->count1 = timer;
			if (gMC.depth > genContext.doctorStage){
				npc->life *= 4.5;
				npc->damage *= 3;
			}
			// Fallthrough
		case 1:
			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			xspeed = 0x10;
			if (gMC.depth > genContext.doctorStage){
				xspeed *= 3;
			}

			if (npc->tgt_x < npc->x)
				npc->xm -= xspeed;
			if (npc->tgt_x > npc->x)
				npc->xm += xspeed;

			if (npc->tgt_y < npc->y)
				npc->ym -=  xspeed;
			if (npc->tgt_y > npc->y)
				npc->ym +=  xspeed;

			if (npc->xm > xspeed*16)
				npc->xm = xspeed*16;
			if (npc->xm < -xspeed*16)
				npc->xm = -xspeed*16;

			if (npc->ym > xspeed*16)
				npc->ym = xspeed*16;
			if (npc->ym < -xspeed*16)
				npc->ym = -xspeed*16;

			if (npc->count1 < timer)
			{
				++npc->count1;
				break;
			}
			yTrigger = 96;
			if (gMC.depth > genContext.doctorStage){
				yTrigger *= 2;
			}

			if (npc->x - (8 * 0x200) < gMC.x && npc->x + (8 * 0x200) > gMC.x && npc->y < gMC.y && npc->y + (yTrigger * 0x200) > gMC.y)
			{
				npc->xm /= 2;
				npc->ym = 0;
				npc->act_no = 3;
				npc->bits &= ~NPC_IGNORE_SOLIDITY;
			}

			break;

		case 3:
			if (gMC.depth > genContext.doctorStage){
				npc->ym += 0x90;
				if (npc->ym > 0x750)
					npc->ym = 0x750;
			}
			else {
			npc->ym += 0x40;
			if (npc->ym > 0x5FF)
				npc->ym = 0x5FF;
			}

			if (gMC.difficulty > DIFFICULTY_HARDENED){
				if (gMC.x < npc->x)
					npc->xm -= 20;
				else
					npc->xm += 20;

				if (npc->xm > 0x400)
					npc->xm = 0x400;
				if (npc->xm < -0x400)
					npc->xm = -0x400;
			}

			if (npc->flag & 8)
			{
				npc->ym = 0;
				npc->xm *= 2;
				npc->count1 = 0;
				npc->act_no = 1;
				npc->bits |= NPC_IGNORE_SOLIDITY;
			}

			break;
	}

	if (npc->flag & 0x100){
		npc->x += npc->xm / 2;
		npc->y += npc->ym / 2;
	} else {
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	Rect rect_left[4] = {
		{32, 32, 48, 48},
		{48, 32, 64, 48},
		{64, 32, 80, 48},
		{80, 32, 96, 48},
	};

	Rect rect_right[4] = {
		{32, 32, 48, 48},
		{48, 32, 64, 48},
		{64, 32, 80, 48},
		{80, 32, 96, 48},
	};

	if (npc->act_no == 3)
	{
		npc->ani_no = 3;
	}
	else
	{
		if (++npc->ani_wait > 1)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 2)
			npc->ani_no = 0;
	}

	if (npc->direct == 0)
		npc->rect = rect_left[npc->ani_no];
	else
		npc->rect = rect_right[npc->ani_no];
}

// Death trap
void ActNpc027(NPCHAR *npc)
{
	Rect rcLeft[1] = {
		{96, 64, 128, 88}
	};

	npc->rect = rcLeft[npc->ani_no];
}

// Flying Critter (Grasstown)
void ActNpcGrassCritter(NPCHAR *npc)
{
	Rect rcLeft[6] = {
		{0, 48, 16, 64},
		{16, 48, 32, 64},
		{32, 48, 48, 64},
		{48, 48, 64, 64},
		{64, 48, 80, 64},
		{80, 48, 96, 64},
	};

	Rect rcRight[6] = {
		{0, 64, 16, 80},
		{16, 64, 32, 80},
		{32, 64, 48, 80},
		{48, 64, 64, 80},
		{64, 64, 80, 80},
		{80, 64, 96, 80},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->y += 3 * 0x200;
			npc->act_no = 1;
			// Fallthrough
		case 1:
			if (npc->act_wait >= 8 && npc->x - (48 * 0x200) < gMC.x && npc->x + (48 * 0x200) > gMC.x && npc->y - (32 * 0x200) < gMC.y && npc->y + (32 * 0x200) > gMC.y)
			{
				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;

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

			if (npc->act_wait >= 8 && npc->x - (48 * 0x200) < gMC.x && npc->x + (48 * 0x200) > gMC.x && npc->y - (32 * 0x200) < gMC.y && npc->y + (32 * 0x200) > gMC.y)
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
				npc->ym = -0x6AA;
				npc->damage = 3;
				PlaySoundObject(30, SOUND_MODE_PLAY);

				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;

				if (npc->direct == 0)
					npc->xm = -0x150;
				else
					npc->xm = 0x150;
			}

			break;

		case 3:
			if (npc->ym > 0x100)
			{
				npc->tgt_y = npc->y;
				npc->act_no = 4;
				npc->ani_no = 3;
				npc->act_wait = 0;
				npc->act_wait = 0;	// Pixel duplicated this line
			}

			break;

		case 4:
			if (npc->x < gMC.x)
				npc->direct = 2;
			else
				npc->direct = 0;

			++npc->act_wait;
			npc->damage = 2;

			if (npc->flag & 7 || npc->act_wait > 50)
			{
				npc->damage = 2;
				npc->act_no = 5;
				npc->ani_no = 2;
				npc->xm /= 2;
				break;
			}

			if (npc->act_wait % 4 == 1)
				PlaySoundObject(109, SOUND_MODE_PLAY);

			if (npc->flag & 8)
				npc->ym = -0x200;

			if (++npc->ani_wait > 0)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 3;

			break;

		case 5:
			if (npc->flag & 8)
			{
				npc->damage = 0;
				npc->xm = 0;
				npc->act_wait = 0;
				npc->ani_no = 0;
				npc->act_no = 1;
				PlaySoundObject(23, SOUND_MODE_PLAY);
			}

			break;
	}

	if (npc->act_no != 4)
	{
		if (gMC.tileset == 'F'){
			npc->ym += 0x20;
			if (npc->ym > 0x2FF)
				npc->ym = 0x2FF;
		} else {
		npc->ym += 0x40;
		if (npc->ym > 0x5FF)
			npc->ym = 0x5FF;
		}
	}
	else
	{
		if (npc->x < gMC.x)
			npc->xm += 0x30;
		else
			npc->xm -= 0x30;

		if (npc->y > npc->tgt_y)
			npc->ym -= 0x17;
		else
			npc->ym += 0x17;

		if (npc->ym > 0x300)
			npc->ym = 0x300;
		if (npc->ym < -0x300)
			npc->ym = -0x300;

		if (npc->xm > 0x300)
			npc->xm = 0x300;
		if (npc->xm < -0x300)
			npc->xm = -0x300;
	}

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

// Cthulhu
void ActNpc029(NPCHAR *npc)
{
	Rect rcLeft[2] = {
		{0, 192, 16, 216},
		{16, 192, 32, 216},
	};

	Rect rcRight[2] = {
		{0, 216, 16, 240},
		{16, 216, 32, 240},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (npc->x - (48 * 0x200) < gMC.x && npc->x + (48 * 0x200) > gMC.x && npc->y - (48 * 0x200) < gMC.y && npc->y + (16 * 0x200) > gMC.y)
				npc->ani_no = 1;
			else
				npc->ani_no = 0;

			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Gunsmith
void ActNpcGrassObject(NPCHAR *npc)
{
	Rect rc[7] = {
		{64, 208, 80, 224},
		{80, 208, 96, 224},
		{96, 208, 112, 224},
		{112, 208, 128, 224},

		{80, 184, 96, 208},
		{96, 184, 112, 208},
		{112, 184, 128, 208}
	};

	if (++npc->act_wait > 50){
		if (++npc->life > 100)
			npc->life = 100;
		npc->act_wait = 0;
	}

	if (npc->direct == 0){
		npc->direct = 1;
		npc->ani_no = Random(0, 3);
	}

	npc->ym += 0x60;
	if (npc->ym > 0x500)
		npc->ym = 0x500;

	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];
}

// Bat (Grasstown, hanging)
void ActNpc031(NPCHAR *npc)
{
	Rect rcLeft[5] = {
		{0, 80, 16, 96},
		{16, 80, 32, 96},
		{32, 80, 48, 96},
		{48, 80, 64, 96},
		{64, 80, 80, 96},
	};

	Rect rcRight[5] = {
		{0, 96, 16, 112},
		{16, 96, 32, 112},
		{32, 96, 48, 112},
		{48, 96, 64, 112},
		{64, 96, 80, 112},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			if (npc->x - (8 * 0x200) < gMC.x && npc->x + (8 * 0x200) > gMC.x && npc->y - (8 * 0x200) < gMC.y && npc->y + (96 * 0x200) > gMC.y)
			{
				npc->ani_no = 0;
				npc->act_no = 3;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 3:
			npc->ani_no = 0;

			if (npc->shock || npc->x - (20 * 0x200) > gMC.x || npc->x + (20 * 0x200) < gMC.x)
			{
				npc->ani_no = 1;
				npc->ani_wait = 0;
				npc->act_no = 4;
				npc->act_wait = 0;
			}

			break;

		case 4:
			npc->ym += 0x20;
			if (npc->ym > 0x5FF)
				npc->ym = 0x5FF;

			if (++npc->act_wait < 20 && !(npc->flag & 8))
				break;

			if (npc->flag & 8 || npc->y > gMC.y - (16 * 0x200))
			{
				npc->ani_wait = 0;
				npc->ani_no = 2;
				npc->act_no = 5;
				npc->tgt_y = npc->y;

				if (npc->flag & 8)
					npc->ym = -0x200;
			}

			break;

		case 5:
			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 4)
				npc->ani_no = 2;

			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			if (gMC.x < npc->x)
				npc->xm -= 0x10;
			if (gMC.x > npc->x)
				npc->xm += 0x10;

			if (npc->tgt_y < npc->y)
				npc->ym -= 0x10;
			if (npc->tgt_y > npc->y)
				npc->ym += 0x10;

			if (npc->xm > 0x200)
				npc->xm = 0x200;
			if (npc->xm < -0x200)
				npc->xm = -0x200;

			if (npc->ym > 0x200)
				npc->ym = 0x200;
			if (npc->ym < -0x200)
				npc->ym = -0x200;

			if (npc->flag & 8)
				npc->ym = -0x200;
			if (npc->flag & 2)
				npc->ym = 0x200;

			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Life capsule
void ActNpcLifeCapsule(NPCHAR *npc)
{
	Rect rect[2] = {
		{32, 96, 48, 112},
		{48, 96, 64, 112},
	};

	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 1)
		npc->ani_no = 0;

	npc->rect = rect[npc->ani_no];

	// Gravity
	if (gMC.tileset == 'F'){
			npc->ym += 0x20;
			if (npc->ym > 0x2FF)
				npc->ym = 0x2FF;
		} else {
	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;
		}

	// Move
	if (npc->flag & 0x100){
		npc->x += npc->xm / 2;
		npc->y += npc->ym / 2;
	} else {
		npc->x += npc->xm;
		npc->y += npc->ym;
	}
}

// Balrog bouncing projectile
void ActNpc033(NPCHAR *npc)
{
	if (npc->flag & 5)
	{
		SetCaret(npc->x, npc->y, 2, 0);
		npc->cond = 0;
	}
	else if (npc->flag & 8)
	{
		npc->ym = -0x400;
	}

	npc->ym += 0x2A;

	if (npc->flag & 0x100){
		npc->x += npc->xm / 2;
		npc->y += npc->ym / 2;
	} else {
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	Rect rect_left[2] = {
		{240, 64, 256, 80},
		{240, 80, 256, 96},
	};

	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;

		if (++npc->ani_no > 1)
			npc->ani_no = 0;
	}

	npc->rect = rect_left[npc->ani_no];

	if (++npc->act_wait > 250)
	{
		SetCaret(npc->x, npc->y, 2, 0);
		npc->cond = 0;
	}
}

// Bed
void ActNpc034(NPCHAR *npc)
{
	Rect rcLeft = {192, 48, 224, 64};
	Rect rcRight = {192, 184, 224, 200};

	if (npc->direct == 0)
		npc->rect = rcLeft;
	else
		npc->rect = rcRight;
}

// Mannan
void ActNpcMannan(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
		case 1:
			if (npc->shock)
			{
				if (npc->direct == 0)
					SetNpChar(103, npc->x - (8 * 0x200), npc->y + (8 * 0x200), 0, 0, npc->direct, NULL, 0x100);
				else
					SetNpChar(103, npc->x + (8 * 0x200), npc->y + (8 * 0x200), 0, 0, npc->direct, NULL, 0x100);
				
				if (gMC.difficulty > DIFFICULTY_HARDENED){
					if (npc->direct == 2)
						SetNpChar(103, npc->x - (8 * 0x200), npc->y + (8 * 0x200), 0, 0, 0, NULL, 0x100);
					else
						SetNpChar(103, npc->x + (8 * 0x200), npc->y + (8 * 0x200), 0, 0, 2, NULL, 0x100);
				}

				npc->ani_no = 1;
				npc->act_no = 2;
				npc->act_wait = 0;
			}

			break;

		case 2:
			if (++npc->act_wait > 10)
			{
				npc->act_wait = 0;
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 3:
			if (++npc->act_wait == 25 || npc->act_wait == 30)
				npc->ani_no = 3;

			if (npc->act_wait == 26 || npc->act_wait == 31)
				npc->ani_no = 2;

			if (npc->act_wait > 50)
				npc->act_no = 4;

			break;
	}

	Rect rcLeft[4] = {
		{96, 64, 120, 96},
		{120, 64, 144, 96},
		{144, 64, 168, 96},
		{168, 64, 192, 96},
	};

	Rect rcRight[4] = {
		{96, 96, 120, 128},
		{120, 96, 144, 128},
		{144, 96, 168, 128},
		{168, 96, 192, 128},
	};

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Balrog (hover)
void ActNpc036(NPCHAR *npc)
{
	int i;
	unsigned char deg;
	int xm, ym;

	switch (npc->act_no)
	{
		case 0:
			if (npc->shock){
				npc->act_no = 1;
			}
			break;
			// Fallthrough
		case 1:
			if (++npc->act_wait > 40)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->count1 = 9;
				if (gMC.difficulty > DIFFICULTY_HARDENED)
					npc->count1 = 16;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 4)
			{
				--npc->count1;
				npc->act_wait = 0;

				deg = GetArktan(npc->x - gMC.x, npc->y + (4 * 0x200) - gMC.y);
				deg += (unsigned char)Random(-0x14, 0x14);
				ym = GetSin(deg)*3;
				xm = GetCos(deg)*3;
				int shotID = 11;
				if (gMC.difficulty > DIFFICULTY_HARDENED){
					shotID = 156;
					ym = GetSin(deg)*5;
					xm = GetCos(deg)*5;
				}

				SetNpChar(shotID, npc->x, npc->y + 0x800, xm, ym, 0, NULL, 0x100);
				PlaySoundObject(39, SOUND_MODE_PLAY);

				if (npc->count1 == 0)
				{
					npc->act_no = 3;
					npc->act_wait = 0;
				}
			}

			break;

		case 3:
			if (++npc->act_wait > 20)
			{
				npc->act_no = 4;
				npc->act_wait = 0;
				npc->xm = (gMC.x - npc->x) / 125;
				npc->ym = -0x600;
				npc->ani_no = 3;
			}

			break;

		case 4:
			if (npc->ym > -0x200)
			{
				if (npc->life > 100)
				{
					npc->act_no = 5;
					npc->ani_no = 4;
					npc->ani_wait = 0;
					npc->act_wait = 0;
					npc->tgt_y = npc->y;
				}
				else
				{
					npc->act_no = 6;
				}
			}

			break;

		case 5:
			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
			{
				npc->ani_no = 4;
				PlaySoundObject(47, SOUND_MODE_PLAY);
			}

			if (++npc->act_wait > 120)
			{
				npc->act_no = 6;
				npc->ani_no = 3;
			}

			if (npc->y < npc->tgt_y)
				npc->ym += 0x40;
			else
				npc->ym -= 0x40;

			if (gMC.difficulty > DIFFICULTY_HARDENED){
				if (npc->act_wait % 11 == 0)
					SetNpChar(33, npc->x, npc->y, Random(-0x60, 0x60), Random(-0x100, 0x200), 0, NULL, 0x100);
			}

			if (npc->ym < -0x200)
				npc->ym = -0x200;
			if (npc->ym > 0x200)
				npc->ym = 0x200;

			break;

		case 6:
			if (npc->y + (16 * 0x200) < gMC.y)
				npc->damage = 10;
			else
				npc->damage = 0;

			if (npc->flag & 8)
			{
				npc->act_no = 7;
				npc->act_wait = 0;
				npc->ani_no = 2;
				PlaySoundObject(26, SOUND_MODE_PLAY);
				PlaySoundObject(25, SOUND_MODE_PLAY);
				SetQuake(30);
				npc->damage = 0;

				for (i = 0; i < 12; ++i)
					SetNpChar(4, npc->x + (Random(-12, 12) * 0x200), npc->y + (Random(-12, 12) * 0x200), Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);
				if (gMC.difficulty < DIFFICULTY_REINFORCED){
					for (i = 0; i < 12; ++i)
						SetNpChar(33, npc->x + (Random(-12, 12) * 0x200), npc->y + (Random(-12, 12) * 0x200), Random(-0x700, 0x700), Random(-0x400, 0), 0, NULL, 0x100);
				}
			}

			break;

		case 7:
			npc->xm = 0;

			if (++npc->act_wait > 50)
			{
				npc->act_no = 1;
				npc->act_wait = 0;
			}

			break;
	}

	if (npc->act_no != 5)
	{
		if (gMC.tileset == 'F'){
			npc->ym += 0x15;
			if (npc->ym > 0x2FF)
				npc->ym = 0x2FF;
		} else {
		npc->ym += 0x33;
		}

		if (npc->x < gMC.x)
			npc->direct = 2;
		else
			npc->direct = 0;
	}

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	if (npc->flag & 0x100){
		npc->x += npc->xm / 2;
		npc->y += npc->ym / 2;
	} else {
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	Rect rect_left[6] = {
		{0, 0, 40, 24},
		{40, 0, 80, 24},
		{80, 0, 120, 24},
		{120, 0, 160, 24},
		{160, 48, 200, 72},
		{200, 48, 240, 72},
	};

	Rect rect_right[6] = {
		{0, 24, 40, 48},
		{40, 24, 80, 48},
		{80, 24, 120, 48},
		{120, 24, 160, 48},
		{160, 72, 200, 96},
		{200, 72, 240, 96},
	};

	if (npc->direct == 0)
		npc->rect = rect_left[npc->ani_no];
	else
		npc->rect = rect_right[npc->ani_no];
}

// Signpost
void ActNpcTutorialSign(NPCHAR *npc)
{
	Rect rect[3] = {
		{192, 64, 208, 80},
		{208, 64, 224, 80},
		{240, 64, 256, 80}
	};

	if (++npc->ani_wait > 1)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 1)
		npc->ani_no = 0;

	if (npc->code_event == 69)
		npc->ani_no = 2;

	npc->ym += 0x60;
	if (npc->ym > 0x500)
		npc->ym = 0x500;

	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];
}

// Fireplace
void ActNpc038(NPCHAR *npc)
{
	Rect rect[4] = {
		{128, 64, 144, 80},
		{144, 64, 160, 80},
		{160, 64, 176, 80},
		{176, 64, 192, 80},
	};
	if (npc->direct == 1){
		for (int i = 0; i < 4; i++){
			rect[i].top += 16;
			rect[i].bottom += 16;
		}
	}
	npc->life = 100;

	switch (npc->act_no)
	{
		case 0:
			if (++npc->ani_wait > 3)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 0;

			if (++npc->act_wait > 45){
				npc->cond = 0;
				SetCaret(npc->x, npc->y, 3, 0);
				return;
			}

			npc->rect = rect[npc->ani_no];
			break;

		case 10:
			npc->act_no = 11;
			SetDestroyNpChar(npc->x, npc->y, npc->view.back, 8);
			// Fallthrough
		case 11:
			npc->rect.left = 0;
			npc->rect.right = 0;
			break;
	}

	if (npc->flag & 0x100){
		npc->cond = 0;
		SetCaret(npc->x, npc->y, 3, 0);
		return;
	} else {
		npc->x += npc->xm;
		npc->y += npc->ym;
	}
}

// Save sign
void ActNpc039(NPCHAR *npc)
{
	Rect rect[2] = {
		{224, 64, 240, 80},
		{240, 64, 256, 80},
	};

	if (npc->direct == 0)
		npc->ani_no = 0;
	else
		npc->ani_no = 1;

	npc->rect = rect[npc->ani_no];
}
