#include "NpcHit.h"

#include "cmath"

#include "Back.h"
#include "BossLife.h"
#include "Bullet.h"
#include "Caret.h"
#include "Flags.h"
#include "Game.h"
#include "Main.h"
#include "Map.h"
#include "MyChar.h"
#include "MycParam.h"
#include "NpChar.h"
#include "NpcTbl.h"
#include "Sound.h"
#include "Stage.h"
#include "Star.h"
#include "TextScr.h"
#include "ValueView.h"

void JadgeHitNpCharBlock(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	if (npc->y - npc->hit.top < (y * 0x10 + 5) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 - 5) * 0x200
		&& npc->x - npc->hit.back < (x * 0x10 + 8) * 0x200
		&& npc->x - npc->hit.back > x * 0x10 * 0x200)
	{
		npc->x = ((x * 0x10 + 8) * 0x200) + npc->hit.back;
		hit |= 1;
	}

	if (npc->y - npc->hit.top < (y * 0x10 + 5) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 - 5) * 0x200
		&& npc->x + npc->hit.back > (x * 0x10 - 8) * 0x200
		&& npc->x + npc->hit.back < x * 0x10 * 0x200)
	{
		npc->x = ((x * 0x10 - 8) * 0x200) - npc->hit.back;
		hit |= 4;
	}

	if (npc->x - npc->hit.back < (x * 0x10 + 5) * 0x200
		&& npc->x + npc->hit.back > (x * 0x10 - 5) * 0x200
		&& npc->y - npc->hit.top < (y * 0x10 + 8) * 0x200
		&& npc->y - npc->hit.top > y * 0x10 * 0x200)
	{
		npc->y = ((y * 0x10 + 8) * 0x200) + npc->hit.top;
		npc->ym = 0;
		hit |= 2;
	}

	if (npc->x - npc->hit.back < (x * 0x10 + 5) * 0x200
		&& npc->x + npc->hit.back > (x * 0x10 - 5) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 - 8) * 0x200
		&& npc->y + npc->hit.bottom < y * 0x10 * 0x200)
	{
		npc->y = ((y * 0x10 - 8) * 0x200) - npc->hit.bottom;
		npc->ym = 0;
		hit |= 8;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleA(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y - npc->hit.top < (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800
		&& npc->y + npc->hit.bottom > (y * 0x10 - 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800 + npc->hit.top;

		// Halt momentum
		if (npc->ym < 0)
			npc->ym = 0;

		// Set that hit a ceiling
		hit |= 2;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleB(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y - npc->hit.top < (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800
		&& npc->y + npc->hit.bottom > (y * 0x10 - 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800 + npc->hit.top;

		// Halt momentum
		if (npc->ym < 0)
			npc->ym = 0;

		// Set that hit a ceiling
		hit |= 2;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleC(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y - npc->hit.top < (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800
		&& npc->y + npc->hit.bottom > (y * 0x10 - 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800 + npc->hit.top;

		// Halt momentum
		if (npc->ym < 0)
			npc->ym = 0;

		// Set that hit a ceiling
		hit |= 2;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleD(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y - npc->hit.top < (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800
		&& npc->y + npc->hit.bottom > (y * 0x10 - 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800 + npc->hit.top;

		// Halt momentum
		if (npc->ym < 0)
			npc->ym = 0;

		// Set that hit a ceiling
		hit |= 2;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleE(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	hit |= 0x10000;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800
		&& npc->y - npc->hit.top < (y * 0x10 + 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800 - npc->hit.bottom;

		// Halt momentum
		if (npc->ym > 0)
			npc->ym = 0;

		// Set that hit this slope
		hit |= 0x28;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleF(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	hit |= 0x20000;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x >= (x * 0x10 - 8) * 0x200	// Note that this function uses '>='. I'm not sure if this is a bug.
		&& npc->y + npc->hit.bottom > (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800
		&& npc->y - npc->hit.top < (y * 0x10 + 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800 - npc->hit.bottom;

		// Halt momentum
		if (npc->ym > 0)
			npc->ym = 0;

		// Set that hit this slope
		hit |= 0x28;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleG(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	hit |= 0x40000;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800
		&& npc->y - npc->hit.top < (y * 0x10 + 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800 - npc->hit.bottom;

		// Halt momentum
		if (npc->ym > 0)
			npc->ym = 0;

		// Set that hit this slope
		hit |= 0x18;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleH(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	hit |= 0x80000;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800
		&& npc->y - npc->hit.top < (y * 0x10 + 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800 - npc->hit.bottom;

		// Halt momentum
		if (npc->ym > 0)
			npc->ym = 0;

		// Set that hit this slope
		hit |= 0x18;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharWater(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	if (npc->x - npc->hit.back < (x * 0x10 + 6) * 0x200
		&& npc->x + npc->hit.back > (x * 0x10 - 6) * 0x200
		&& npc->y - npc->hit.top < (y * 0x10 + 6) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 - 6) * 0x200)
		hit |= 0x100;

	npc->flag |= hit;
}

void HitNpCharMap(void)
{
	int x, y;
	int judg;

	int offx[9];
	int offy[9];

	int i, j;

	offx[0] = 0;
	offx[1] = 1;
	offx[2] = 0;
	offx[3] = 1;
	offx[4] = 2;
	offx[5] = 2;
	offx[6] = 2;
	offx[7] = 0;
	offx[8] = 1;

	offy[0] = 0;
	offy[1] = 0;
	offy[2] = 1;
	offy[3] = 1;
	offy[4] = 0;
	offy[5] = 1;
	offy[6] = 2;
	offy[7] = 2;
	offy[8] = 2;

	for (i = 0; i < NPC_MAX; ++i)
	{
		if (!(gNPC[i].cond & 0x80))
			continue;

		if (gNPC[i].bits & NPC_IGNORE_SOLIDITY)
			continue;

		if (gNPC[i].size >= 3)
		{
			judg = 9;
			x = (gNPC[i].x - 0x1000) / 0x10 / 0x200;
			y = (gNPC[i].y - 0x1000) / 0x10 / 0x200;
		}
		else
		{
			judg = 4;
			x = gNPC[i].x / 0x10 / 0x200;
			y = gNPC[i].y / 0x10 / 0x200;
		}

		gNPC[i].flag = 0;

		for (j = 0; j < judg; ++j)
		{
			switch (GetAttribute(x + offx[j], y + offy[j]))
			{
				// No NPC block
				case 0x44:
					if (gNPC[i].bits & NPC_IGNORE_TILE_44)
						break;
					// Fallthrough
				// Block
				case 0x03:
				case 0x05:
				case 0x41:
				case 0x43:
					JadgeHitNpCharBlock(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				// Slopes
				case 0x50:
					JudgeHitNpCharTriangleA(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x51:
					JudgeHitNpCharTriangleB(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x52:
					JudgeHitNpCharTriangleC(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x53:
					JudgeHitNpCharTriangleD(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x54:
					JudgeHitNpCharTriangleE(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x55:
					JudgeHitNpCharTriangleF(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x56:
					JudgeHitNpCharTriangleG(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x57:
					JudgeHitNpCharTriangleH(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				// Water
				case 0x02:
				case 0x60:
				case 0x62:
					JudgeHitNpCharWater(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				// Water block
				case 0x04:
				case 0x61:
				case 0x64:
					JadgeHitNpCharBlock(&gNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				// Water slopes
				case 0x70:
					JudgeHitNpCharTriangleA(&gNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x71:
					JudgeHitNpCharTriangleB(&gNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x72:
					JudgeHitNpCharTriangleC(&gNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x73:
					JudgeHitNpCharTriangleD(&gNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x74:
					JudgeHitNpCharTriangleE(&gNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x75:
					JudgeHitNpCharTriangleF(&gNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x76:
					JudgeHitNpCharTriangleG(&gNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x77:
					JudgeHitNpCharTriangleH(&gNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0xA0:
					gNPC[i].flag |= 0x100;
					// Fallthrough
				case 0x80:
					gNPC[i].flag |= 0x1000;
					break;

				case 0xA1:
					gNPC[i].flag |= 0x100;
					// Fallthrough
				case 0x81:
					gNPC[i].flag |= 0x2000;
					break;

				case 0xA2:
					gNPC[i].flag |= 0x100;
					// Fallthrough
				case 0x82:
					gNPC[i].flag |= 0x4000;
					break;

				case 0xA3:
					gNPC[i].flag |= 0x100;
					// Fallthrough
				case 0x83:
					gNPC[i].flag |= 0x8000;
					break;
			}

			if (gNPC[i].y > gWaterY + 0x800)
				gNPC[i].flag |= 0x100;
		}

		if (gNPC[i].waterEffect){
			--gNPC[i].waterEffect;
			gNPC[i].flag |= 0x100;
			if (gNPC[i].waterEffect % 2 == 0)
			SetCaret(gNPC[i].x, gNPC[i].y, 1, 0);
		}
	}
}

void LoseNpChar(NPCHAR *npc, bool bVanish)
{
	int val;

	// Play death sound
	PlaySoundObject(npc->destroy_voice, SOUND_MODE_PLAY);

	// Create smoke
	switch (npc->size)
	{
		case 1:
			SetDestroyNpChar(npc->x, npc->y, npc->view.back, 3);
			break;

		case 2:
			SetDestroyNpChar(npc->x, npc->y, npc->view.back, 7);
			break;

		case 3:
			SetDestroyNpChar(npc->x, npc->y, npc->view.back, 12);
			break;
	}

	if (gMC.difficulty > DIFFICULTY_REINFORCED && Random(0, 4) == 0){
		npc->exp /= 2;
	}

	if (gMC.difficulty > DIFFICULTY_REINFORCED && npc->code_char == 78 && Random(0, 4) == 0){
		npc->exp = 0;
		int randomMobs[] = {147, 28, 6};
		NPCHAR* surpriseMob = CreateNpChar(randomMobs[Random(0, 2)], npc->x, npc->y, 0, 0, 0, NULL, 0x100);
		surpriseMob->exp = 1;
	}

	// Create drop
	if (npc->exp != 0)
	{
		int roll = Random(1, 5);
		if (gMC.difficulty > DIFFICULTY_REINFORCED){
			roll = Random(1, 7);
		}
		switch (roll)
		{
			case 1:
				// Spawn health
				if (npc->exp > 6)
					val = 6;
				else
					val = 2;

				SetLifeObject(npc->x, npc->y, val);

				break;

			default:
				// Spawn weapon energy
				SetExpObjects(npc->x, npc->y, npc->exp);
				break;
		}
	}

	switch (npc->code_char){
		case 78: //pot
			AddScore(25); break;
		case 95: //jelly
			AddScore(15); break;
		case 26: case 309: //bat
			AddScore(40); break;
		case 147: //critter
			AddScore(60); break;
		case 6: //beetle
			AddScore(30); break;
		case 311: //archer
			AddScore(100); break;
		case 246: //press
			AddScore(75);  break;
		case 24: //power critter
			AddScore(80);  break;
		case 49: //skullhead
			AddScore(100);  break;
		case 57: //crow
			AddScore(100);  break;	
		case 94: //kulala
			AddScore(80);  break;
		case 238: //turret
			AddScore(140);  break;
		case 176: //core base
			AddScore(350);  break;
		case 35: //mannan
			AddScore(110);  break;
		case 155: //laser gaudi
			AddScore(200);  break;
		case 173: //armor gaudi
			AddScore(250);  break;
		case 200: //dragons
			AddScore(300);  break;
		case 104: //frogs
			AddScore(315);  break;
		case 287: //metal fuzz
			AddScore(350);  break;
		case 310: //laser puchi
			AddScore(400);  break;
		case 247: //misery
			AddScore(500);  break;
		case 283: //posmisery
			AddScore(550);  break;
		case 140: //toroko
			AddScore(600);  break;
		case 268: case 276: case 317: case 36: case 284: case 44: //bosses
			SetNpChar(18, npc->x, npc->y - 8 + 0x200, 0, 0, 0, NULL, 0x100, 99, 0x2000);
			AddScore(7500);  StartBossLife2(); break;
	}

	// Set flag
	SetNPCFlag(npc->code_flag);

	// Create value view
	if (npc->bits & NPC_SHOW_DAMAGE)
	{
		if ((npc->bits & NPC_SHOW_DAMAGE) && npc->damage_view)	// npc->bits & NPC_SHOW_DAMAGE is already verified at this point, so this is redundant
			SetValueView(&npc->x, &npc->y, npc->damage_view);
		if (bVanish)
			VanishNpChar(npc);
	}
	else
	{
		npc->cond = 0;
	}

	if (/* GetNPCFlag(851) */ true)
	{
		for (int b1 = 0; b1 < BULLET_MAX; b1++)
		{
			if (gBul[b1].code_bullet == 57 && gBul[b1].npc == npc)
			{
				gBul[b1].npc = NULL;
			}
		}
	}
}

void HitNpCharBullet(void)
{
	int n, b;
	bool bHit;

	for (n = 0; n < NPC_MAX; ++n)
	{
		if (!(gNPC[n].cond & 0x80))
			continue;

		if (gNPC[n].bits & NPC_SHOOTABLE && gNPC[n].bits & NPC_INTERACTABLE)
			continue;

		if ((std::find(aquaticEntities.begin(), aquaticEntities.end(), gNPC[n].code_char) != aquaticEntities.end()) && (gMC.equip & EQUIP_FISHY || GetNPCFlag(863))){
			continue;
		}

		for (b = 0; b < BULLET_MAX; ++b)
		{
			bool isSword = gBul[b].code_bullet == 58 || gBul[b].code_bullet == 60 || gBul[b].code_bullet == 59
				|| gBul[b].code_bullet == 80 || gBul[b].code_bullet == 81 || gBul[b].code_bullet == 82 || gBul[b].code_bullet == 85;
			bool isEthereal = std::find(etherealEntities.begin(), etherealEntities.end(), gNPC[n].code_char) != etherealEntities.end();

			if (!(gBul[b].cond & 0x80))
				continue;

			if (gBul[b].iframes > 0 && static_iframes[n][gBul[b].code_bullet] != 0)
				continue;

			if (gNPC[n].shock > 0 && ((gBul[b].code_bullet > 27 && gBul[b].code_bullet < 31) ||
				 (gBul[b].code_bullet == 57) || (gBul[b].code_bullet == 69) || (GetNPCFlag(873) && gBul[b].code_bullet > 6 && gBul[b].code_bullet < 10 && CountBulletNum(16))))
				continue;

			if (gBul[b].damage == -1)
				continue;

			if (gBul[b].code_bullet == 77 && gNPC[n].code_char == 48)
				continue;

			if (gNPC[n].shock > 0 && gNPC[n].code_event == 30)
				continue;

			if (gNPC[n].code_char == 126 && gBul[b].code_bullet == 46)
				continue;

			if (gNPC[n].code_char == 100 && !(gBul[b].code_bullet == 64 || gBul[b].code_bullet == 65 || gBul[b].code_bullet == 34 || gBul[b].code_bullet == 35 ||
	 ((GetNPCFlag(4400) || GetNPCFlag(7999)) && (gBul[b].code_bullet == 37 || gBul[b].code_bullet == 38 || gBul[b].code_bullet == 39))))
	 			continue;

			// Check if bullet touches npc
			bHit = false;
			if (gNPC[n].bits & NPC_SHOOTABLE
				&& gNPC[n].x - gNPC[n].hit.back < gBul[b].x + gBul[b].enemyXL * (isSword && gBul[b].direct == 0 ? 0.9f : 1.0f)
				&& gNPC[n].x + gNPC[n].hit.back > gBul[b].x - gBul[b].enemyXL * (isSword && gBul[b].direct == 2 ? 0.9f : 1.0f)
				&& gNPC[n].y - gNPC[n].hit.top < gBul[b].y + gBul[b].enemyYL * (isSword ? 0.75f : 1.0f)
				&& gNPC[n].y + gNPC[n].hit.bottom > gBul[b].y - gBul[b].enemyYL)
				bHit = true;
			else if (gNPC[n].bits & NPC_INVULNERABLE
				&& gNPC[n].x - gNPC[n].hit.back < gBul[b].x + gBul[b].blockXL
				&& gNPC[n].x + gNPC[n].hit.back > gBul[b].x - gBul[b].blockXL
				&& gNPC[n].y - gNPC[n].hit.top < gBul[b].y + gBul[b].blockYL
				&& gNPC[n].y + gNPC[n].hit.bottom > gBul[b].y - gBul[b].blockYL)
				bHit = true;

			if (bHit)
			{
				// Damage NPC
				if (gNPC[n].bits & NPC_SHOOTABLE)
				{
					int dmg = gBul[b].damage;
					if (isEthereal) dmg = 0;
					if (gMC.equip & EQUIP_NIKUMARU_COUNTER && Random(1, 3) == 1 && gNPC[n].damage > 0){
						if (gNPC[n].code_char == 263 || gNPC[n].code_char == 267){
							gNPC[n].freeze += gBul[b].damage*5;
						} else
							gNPC[n].freeze += gBul[b].damage*15;
						if (gNPC[n].freeze > 80){
							gNPC[n].freeze = 80;
						}
						continue;
					}
					if (CheckForCooldown(gCooldowns, NANITES_TIME_ID) != -1){
						dmg *= 1.66;
					}
					if (GetNPCFlag(874)){
						dmg = round(dmg * 1.25f);
					}
					if (gMC.equip & EQUIP_SUNGLASSES){
						dmg *= 2;
					}
					if (GetNPCFlag(4242)){
						dmg *= float(Random(50, 175)) / 100.0f;
					}
					if (std::find(waterBullets.begin(), waterBullets.end(), gBul[b].code_bullet) != waterBullets.end()){
						gNPC[n].waterEffect = Random(0, 7);
					}
					if (ChallengeEnabled("oneDamage") && dmg > 0)
						dmg = 1;
					gNPC[n].life -= dmg;
					if (std::find(knockImmuneEnemies.begin(), knockImmuneEnemies.end(), gNPC[n].code_char) == knockImmuneEnemies.end() &&
						 (GetNPCFlag(711) || gBul[b].code_bullet == 14)){
						gNPC[n].knockx = gBul[b].xm * gBul[b].knockX * gNPC[n].knockEffect;
						gNPC[n].knocky = gBul[b].ym * gBul[b].knockY * gNPC[n].knockEffect;
					}
					if (gBul[b].code_bullet > 6 && gBul[b].code_bullet < 10 && !CountBulletNum(16) && GetNPCFlag(873)){
						SetBullet(16, gBul[b].x, gBul[b].y, 0);
					}

					if (Random(0, 2) == 0 && dmg != 0 && gMC.equip & EQUIP_NANO_QUIVER && (gBul[b].code_bullet != 47 && gBul[b].code_bullet != 45))
					{
						int x, y;
						do
						{
							x = Random(0, gMap.width*0x2000);
							y = Random(0, gMap.length*0x2000);
						} while (!(Distance(gNPC[n].x, gNPC[n].y, x, y) < 90_pix  && 
									Distance(gNPC[n].x, gNPC[n].y, x, y) > 8_pix));
						BULLET *bul = SetBullet(47, x, y);
						if (bul != NULL){
						bul->tgt_x = gNPC[n].x;
						bul->tgt_y = gNPC[n].y;
						}
					}

					if (/* GetNPCFlag(851) */true){
						for (int b1 = 0; b1 < BULLET_MAX; b1++){
							if (gBul[b1].code_bullet == 57 && gBul[b1].npc == NULL && !isEthereal){
								gBul[b1].npc = &gNPC[n];
								break;
							}
						}
					}

					if (gNPC[n].life < 1 || ChallengeEnabled("debug"))
					{
						gNPC[n].life = 0;

						if (gNPC[n].bits & NPC_SHOW_DAMAGE && !ChallengeEnabled("debug"))
							gNPC[n].damage_view -= dmg;

						if ((gMC.cond & 0x80) && gNPC[n].bits & NPC_EVENT_WHEN_KILLED)
							StartTextScript(gNPC[n].code_event);
						else
							gNPC[n].cond |= 8;

						if (isSword){
							if (Random(0, 1))
								SetBullet(79, gNPC[n].x, gNPC[n].y, 0);
							else
								SetBullet(79, gNPC[n].x, gNPC[n].y, 2);
						}
					}
					else
					{
						if (gBul[b].iframes > 0)
							static_iframes[n][gBul[b].code_bullet] = gBul[b].iframes;
						if (gNPC[n].shock < 14)
						{
							if (!isEthereal){
							SetCaret((gBul[b].x + gNPC[n].x) / 2, (gBul[b].y + gNPC[n].y) / 2, 11, 0);
							SetCaret((gBul[b].x + gNPC[n].x) / 2, (gBul[b].y + gNPC[n].y) / 2, 11, 0);
							SetCaret((gBul[b].x + gNPC[n].x) / 2, (gBul[b].y + gNPC[n].y) / 2, 11, 0);
							}
							PlaySoundObject(gNPC[n].hit_voice, SOUND_MODE_PLAY);
							gNPC[n].shock = 16;
							if (isSword)
							{
								// gNPC[n].shock = 16 + gBul[b].life_count;
								// gNPC[n].sword_iframes = gBul[b].life_count;
								if (Random(0, 1))
									SetBullet(79, gNPC[n].x, gNPC[n].y, 0);
								else
									SetBullet(79, gNPC[n].x, gNPC[n].y, 2);
								if (std::find(knockImmuneEnemies.begin(), knockImmuneEnemies.end(), gNPC[n].code_char) == knockImmuneEnemies.end()){
									if (gBul[b].direct == 0)
										gNPC[n].knockx = -Random(0x300, 0x5F0);
									else
										gNPC[n].knockx = Random(0x300, 0x5F0);
									if (gMC.y < gNPC[n].y)
										gNPC[n].knocky = Random(0x300, 0x5F0) * 0.66;
									if (gMC.y > gNPC[n].y)
										gNPC[n].knocky = -Random(0x300, 0x5F0) * 0.66;
								}
							}
						}

						if (gNPC[n].bits & NPC_SHOW_DAMAGE)
							gNPC[n].damage_view -= dmg;
						starTarget = &gNPC[n];
					}
				}
				else if (gBul[b].code_bullet == 13
					|| gBul[b].code_bullet == 14
					|| gBul[b].code_bullet == 15
					|| gBul[b].code_bullet == 28
					|| gBul[b].code_bullet == 29
					|| gBul[b].code_bullet == 30)
				{
					// Strange empty case that's needed for accurate assembly
				}
				else if (!(gBul[b].bbits & 0x10))
				{
					// Hit invulnerable NPC
					SetCaret((gBul[b].x + gNPC[n].x) / 2, (gBul[b].y + gNPC[n].y) / 2, 2, 2);
					PlaySoundObject(31, SOUND_MODE_PLAY);
					gBul[b].life = 0;
					continue;
				}
				if (!isEthereal)
					--gBul[b].life;
			}
		}

		if (gNPC[n].cond & 8){
			LoseNpChar(&gNPC[n], true);
			if (Random(0, 2) == 0 && gNPC[n].exp > 0 && GetNPCFlag(3333)){
				SetLifeObject(gNPC[n].x, gNPC[n].y, 1);
			}
		}
	}
}
