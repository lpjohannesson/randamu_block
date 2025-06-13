#include "BossIronH.h"

#include <cstddef>

#include "CommonDefines.h"

#include "Boss.h"
#include "Caret.h"
#include "Frame.h"
#include "Game.h"
#include "MyChar.h"
#include "NpChar.h"
#include "Sound.h"
#include "Stage.h"
#include "Triangle.h"

int Ironhead_HealthValue(){
	switch (gMC.difficulty){
		default:
			return 1000;
		case DIFFICULTY_REINFORCED: case DIFFICULTY_RESONANT:
			return 1100;
	}
}

void ActBossChar_Ironhead(void)
{
	int i;
	NPCHAR *npc = gBoss;
	static unsigned char flash;
	int shipRate = 2;
	int shipAmount = 25;
	int dashes = 3;
	if (gMC.difficulty > DIFFICULTY_HARDENED){
		shipRate = 1;
		shipAmount = 20;
		dashes = 6;
	}

	switch (npc->act_no)
	{
		case 0:
			npc->cond = 0x80;
			npc->exp = 1;
			npc->direct = 2;
			npc->act_no = 100;
			npc->x = 100 * 0x200;
			npc->y = 128 * 0x200;
			npc->view.front = 40 * 0x200;
			npc->view.top = 12 * 0x200;
			npc->view.back = 24 * 0x200;
			npc->view.bottom = 12 * 0x200;
			npc->hit_voice = 54;
			npc->hit.front = 16 * 0x200;
			npc->hit.top = 10 * 0x200;
			npc->hit.back = 16 * 0x200;
			npc->hit.bottom = 10 * 0x200;
			npc->bits = (NPC_IGNORE_SOLIDITY | NPC_SHOOTABLE | NPC_EVENT_WHEN_KILLED | NPC_SHOW_DAMAGE);
			if (gMC.difficulty > DIFFICULTY_HARDENED){
				npc->bits |= NPC_SOLID_SOFT;
				npc->bits |= NPC_BOUNCY;
			}	
			npc->size = 3;
			npc->damage = 10;
			npc->code_event = 1000;
			npc->life = Ironhead_HealthValue();
			break;

		case 100:
			npc->act_no = 101;
			npc->bits &= ~NPC_SHOOTABLE;
			npc->act_wait = 0;
			// Fallthrough
		case 101:
			++npc->act_wait;

			if (npc->act_wait > shipAmount * 2)
			{
				npc->act_no = 250;
				npc->act_wait = 0;
			}

			if (npc->act_wait % shipRate == 0 && npc->act_wait < shipAmount)
				SetNpChar(197, Random(14, 18) * (16 * 0x200), Random(2, 15) * (16 * 0x200), 0, 0, 0, NULL, 0x100);

			break;

		case 250:
			npc->act_no = 251;

			if (npc->direct == 2)
			{
				npc->x = 180 * 0x200;
				npc->y = gMC.y;
			}
			else
			{
				npc->x = 775 * 0x200;
				npc->y = Random(2, 13) * (16 * 0x200);
			}

			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;

			npc->ym = Random(-0x300, 0x300);
			npc->xm = Random(-0x300, 0x300);

			npc->bits |= NPC_SHOOTABLE;
			// Fallthrough
		case 251:
			if (npc->direct == 2)
			{
				npc->tgt_x += 2 * 0x200;
				SetCaret(npc->x - (12 * 0x200), npc->y - (5 * 0x200), 7, 0);
				SetCaret(npc->x - (12 * 0x200), npc->y + (5 * 0x200), 7, 0);
			}
			else
			{
				npc->tgt_x -= 1 * 0x200;

				if (npc->tgt_y < gMC.y)
					npc->tgt_y += 1 * 0x200;
				else
					npc->tgt_y -= 1 * 0x200;
			}

			if (npc->x < npc->tgt_x)
				npc->xm += 8;
			else
				npc->xm -= 8;

			if (npc->y < npc->tgt_y)
				npc->ym += 8;
			else
				npc->ym -= 8;

			if (npc->ym > 0x200)
				npc->ym = 0x200;
			if (npc->ym < -0x200)
				npc->ym = -0x200;
			
			if (gMC.difficulty > DIFFICULTY_HARDENED){
				npc->x += npc->xm * 2;
				npc->y += npc->ym * 2;
			} else {
				npc->x += npc->xm;
				npc->y += npc->ym;
			}

			if (npc->direct == 2)
			{
				if (npc->x > 775 * 0x200)
				{
					npc->direct = 0;
					npc->act_no = 100;
					if (npc->life < Ironhead_HealthValue() / 2){
						npc->act_no = 300;
					}
				}
			}
			else
			{
				if (npc->x < 190 * 0x200)
				{
					npc->direct = 2;
					npc->act_no = 100;
					if (npc->life < Ironhead_HealthValue() / 2){
						npc->act_no = 300;
					}
				}
			}

			if (npc->direct == 0)
			{
				++npc->act_wait;
				if (gMC.difficulty > DIFFICULTY_HARDENED){

				if (npc->act_wait >= 200 && npc->act_wait < 600 && npc->act_wait % 20 == 0)
				{
					PlaySoundObject(39, SOUND_MODE_PLAY);
					SetNpChar(198, npc->x + (10 * 0x200), npc->y + (1 * 0x200), Random(-3, 2) * 0x200, Random(-3, 3) * 0x200, 2, NULL, 0x100);
				}
				} else {
				if (npc->act_wait == 300 || npc->act_wait == 310 || npc->act_wait == 320)
				{
					PlaySoundObject(39, SOUND_MODE_PLAY);
					SetNpChar(198, npc->x + (10 * 0x200), npc->y + (1 * 0x200), Random(-3, 0) * 0x200, Random(-3, 3) * 0x200, 2, NULL, 0x100);
				}
				}
			}

			++npc->ani_wait;

			if (npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 7)
				npc->ani_no = 0;

			break;

		case 300:
			npc->act_no = 450;
			npc->bits &= ~NPC_SHOOTABLE;
			npc->act_wait = 0;
			// Fallthrough

		case 450:
			npc->act_no = 451;

			if (npc->direct == 2)
			{
				npc->x = 180 * 0x200;
				npc->y = gMC.y;
			}
			else
			{
				npc->x = 775 * 0x200;
				npc->y = Random(2, 13) * (16 * 0x200);
			}

			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;

			npc->ym = Random(0, 0x400);
			npc->xm = Random(0, 0x500);

			npc->bits |= NPC_SHOOTABLE;
			// Fallthrough
		case 451:
			if (npc->direct == 2)
			{
				npc->tgt_x += 7 * 0x200;
				SetCaret(npc->x - (12 * 0x200), npc->y - (5 * 0x200), 7, 0);
				SetCaret(npc->x - (12 * 0x200), npc->y + (5 * 0x200), 7, 0);
				SetCaret(npc->x - (15 * 0x200), npc->y - (5 * 0x200), 7, 0);
				SetCaret(npc->x - (15 * 0x200), npc->y + (5 * 0x200), 7, 0);
				SetCaret(npc->x - (10 * 0x200), npc->y + (2 * 0x200), 7, 0);
				SetCaret(npc->x - (10 * 0x200), npc->y - (2 * 0x200), 7, 0);
			}
			else
			{
				npc->tgt_x -= 2 * 0x200;

				if (npc->tgt_y < gMC.y)
					npc->tgt_y += 3 * 0x200;
				else
					npc->tgt_y -= 3 * 0x200;
			}

			if (npc->x < npc->tgt_x){
				npc->xm += 50;
				npc->damage = 18;
			}
			else {
				npc->xm -= 10;
				if (gMC.difficulty > DIFFICULTY_HARDENED){
					npc->xm -= 30;
				}
				npc->damage = 5;
			}

			if (npc->x < gMC.x){
				npc->bits |= NPC_INVULNERABLE;
				npc->bits &= ~NPC_SHOOTABLE;
			} else {
				npc->bits &= ~NPC_INVULNERABLE;
				npc->bits |= NPC_SHOOTABLE;
			}

			if (npc->y < npc->tgt_y)
				npc->ym += 30;
			else
				npc->ym -= 30;

			if (npc->ym > 0x600)
				npc->ym = 0x600;
			if (npc->ym < -0x600)
				npc->ym = -0x600;

			npc->x += npc->xm;
			npc->y += npc->ym;

			if (npc->direct == 2)
			{
				if (npc->x > 775 * 0x200)
				{
					npc->count1++;
					if (npc->count1 > dashes-1){
						npc->direct = 0;
						npc->act_no = 300;
						npc->count1 = 0;
					} else {
						npc->act_no = 450;
						npc->x = 700 * 0x200;
					}
				}
			}
			else
			{
				if (npc->x < 180 * 0x200)
				{
					npc->direct = 2;
					npc->act_no = 300;
				}
			}

			if (npc->direct == 0)
			{
				++npc->act_wait;

				if (npc->act_wait == 200 || npc->act_wait == 230 || npc->act_wait == 260)
				{
					PlaySoundObject(101, SOUND_MODE_PLAY);
					for (int deg = 0; deg < 0x100; deg += 0x8 * 2)
					{
						int xm = GetCos(deg) * 5;
						int ym = GetSin(deg) * 5;
						SetNpChar(266, npc->x, npc->y, xm, ym, 1, NULL, 0x100);
					}
				}
			}

			++npc->ani_wait;

			if (npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 7)
				npc->ani_no = 0;

			break;

		case 1000:
			npc->bits &= ~NPC_SHOOTABLE;
			npc->ani_no = 8;
			npc->damage = 0;
			npc->act_no = 1001;
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			SetQuake(20);

			for (i = 0; i < 0x20; ++i)
				SetNpChar(4, npc->x + (Random(-128, 128) * 0x200), npc->y + (Random(-64, 64) * 0x200), Random(-128, 128) * 0x200, Random(-128, 128) * 0x200, 0, NULL, 0x100);

			DeleteNpCharCode(197, true);
			DeleteNpCharCode(271, true);
			DeleteNpCharCode(272, true);
			// Fallthrough
		case 1001:
			npc->tgt_x -= 1 * 0x200;

			npc->x = npc->tgt_x + (Random(-1, 1) * 0x200);
			npc->y = npc->tgt_y + (Random(-1, 1) * 0x200);

			if (++npc->act_wait % 2 == 0)
				SetNpChar(4, npc->x + (Random(-128, 128) * 0x200), npc->y + (Random(-64, 64) * 0x200), Random(-128, 128) * 0x200, Random(-128, 128) * 0x200, 0, NULL, 0x100);

			break;
	}

	Rect rc[9] = {
		{0, 0, 64, 24},
		{64, 0, 128, 24},
		{128, 0, 192, 24},
		{64, 0, 128, 24},
		{0, 0, 64, 24},
		{192, 0, 256, 24},
		{256, 0, 320, 24},
		{192, 0, 256, 24},
		{256, 48, 320, 72},
	};

	Rect rcDamage[9] = {
		{0, 24, 64, 48},
		{64, 24, 128, 48},
		{128, 24, 192, 48},
		{64, 24, 128, 48},
		{0, 24, 64, 48},
		{192, 24, 256, 48},
		{256, 24, 320, 48},
		{192, 24, 256, 48},
		{256, 48, 320, 72},
	};

	if (npc->shock != 0)
	{
		if (++flash / 2 % 2)
			npc->rect = rc[npc->ani_no];
		else
			npc->rect = rcDamage[npc->ani_no];
	}
	else
	{
		npc->rect = rc[npc->ani_no];
	}
}
