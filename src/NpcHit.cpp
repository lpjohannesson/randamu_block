#include "NpcHit.h"

#include "cmath"

#include "Back.h"
#include "BossLife.h"
#include "Bullet.h"
#include "Caret.h"
#include "Flags.h"
#include "Game.h"
#include "Log.h"
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
		if (Random(0, 1) == 0 && GetNPCFlag(3333)){
			SetLifeObject(npc->x, npc->y, 1);
		}
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
	}

	if (std::find(bossNPCsList.begin(), bossNPCsList.end(), npc->code_char) != bossNPCsList.end())
	{
		SetNpChar(18, npc->x, npc->y - 8 + 0x200, 0, 0, 0, NULL, 0x100, 99, 0x2000);
		AddScore(7500);  StartBossLife2();
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

bool IntersectDamageCheck(NPCHAR &npc, int x, int y, int xl, int yl)
{
	return (npc.bits & NPC_SHOOTABLE
				&& npc.x - npc.hit.back < x + xl
				&& npc.x + npc.hit.back > x - xl
				&& npc.y - npc.hit.top < y + yl
				&& npc.y + npc.hit.bottom > y - yl);
}

void SpawnQuiverProjectile(NPCHAR &npc)
{
	int x, y;
		do
		{
			x = Random(0, gMap.width*0x2000);
			y = Random(0, gMap.length*0x2000);
		} while (!(Distance(npc.x, npc.y, x, y) < 90_pix  && 
					Distance(npc.x, npc.y, x, y) > 8_pix));
		BULLET *quiverProj = SetBullet(47, x, y);
		if (quiverProj != NULL){
			quiverProj->tgt_x = npc.x;
			quiverProj->tgt_y = npc.y;
		}
}

bool DealDamage(NPCHAR &npc, int dmg, BULLET* bul)
{
	if (!(npc.bits & NPC_SHOOTABLE))
		return false;

	if (npc.code_char == 100){
		if (bul == NULL || !IsBreakingBullet(*bul))
			return false;
	}

	bool isEthereal = std::find(etherealEntities.begin(), etherealEntities.end(), npc.code_char) != etherealEntities.end();
	bool isBoss = std::find(bossNPCsList.begin(), bossNPCsList.end(), npc.code_char) != bossNPCsList.end();
	if (isEthereal) dmg = 0;

	if (gMC.equip & EQUIP_NIKUMARU_COUNTER && Random(1, 3) == 1 && npc.damage > 0){
		if (npc.code_char == 263 || npc.code_char == 267){
			npc.freeze += dmg*5;
		} else
			npc.freeze += dmg*15;
		if (npc.freeze > 80){
			npc.freeze = 80;
		}
		return false;
	}

	if (bul != NULL && bul->code_bullet == 89){
		float speed;
		switch (bul->direct){
			case 0: case 2: speed = std::abs(bul->xm); break;
			case 1: case 3: speed = std::abs(bul->ym); break;
		}
		dmg += std::round(speed / 512.0f * 3.5f);
	}

	if (CheckForCooldown(gCooldowns, NANITES_TIME_ID) != -1){
		dmg *= 1.66;
	}

	if (GetNPCFlag(874)){ //discordian visor
		dmg = round(dmg * 1.25f);
	}

	if (npc.targetingEffect > 0){
		dmg *= 3;
		npc.targetingEffect = 0;
		if (dmg > 0){
			PlaySoundObject(12);
			SetCaret(npc.x-0x2500, npc.y, 2, 2);
			SetCaret(npc.x-0x1800, npc.y-0x1800, 2, 2);
			SetCaret(npc.x+0x2500, npc.y, 2, 2);
			SetCaret(npc.x+0x1800, npc.y-0x1800, 2, 2);
			SetCaret(npc.x, npc.y+0x2500, 2, 2);
			SetCaret(npc.x+0x1800, npc.y+0x1800, 2, 2);
			SetCaret(npc.x, npc.y-0x2500, 2, 2);
			SetCaret(npc.x-0x1800, npc.y+0x1800, 2, 2);
		}
	}

	if (gMC.equip & EQUIP_SUNGLASSES){ //cool sunglasses
		dmg *= 2;
	}

	if (GetNPCFlag(4242)){ //quate
		dmg *= float(Random(50, 175)) / 100.0f;
	}

	if (bul != NULL){

		if (std::find(waterBullets.begin(), waterBullets.end(), bul->code_bullet) != waterBullets.end()){
			npc.waterEffect = std::min(npc.waterEffect + Random(10, 25), 120);
		}

		if (bul->code_bullet == 19){
			PlaySoundObject(21, SOUND_MODE_PLAY);
			int lol = Random(10, 30);
			for (int i = 0; i < lol; i++)
			{
				int x, y;
				do
				{
					x = Random(0, gMap.width*0x2000);
					y = Random(0, gMap.length*0x2000);
				} while (!(Distance(npc.x, npc.y, x, y) < 32_pix  && 
							Distance(npc.x, npc.y, x, y) > 4_pix));
				BULLET *bully = SetBullet(61, x, y);
				if (bully != NULL)
				{
					bully->direct = 0;
					bully->xm = Random(-1000, 1000);
					bully->ym = Random(-800, 400);
				}
			}
		}

		if (std::find(knockImmuneEnemies.begin(), knockImmuneEnemies.end(), npc.code_char) == knockImmuneEnemies.end() &&
				(GetNPCFlag(711) || bul->code_bullet == 14)){
			npc.knockx = bul->xm * bul->knockX * npc.knockEffect;
			npc.knocky = bul->ym * bul->knockY * npc.knockEffect;
		}

		if (bul->code_bullet == 4){
			npc.targetingEffect = 1;
		}

		if (bul->code_bullet > 6 && bul->code_bullet < 10 && !CountBulletNum(16) && GetNPCFlag(873)){
			SetBullet(16, bul->x, bul->y, 0);
		}

		if (bul->code_bullet == 87 && Random(0, 3) == 0)
			SpawnQuiverProjectile(npc);

		if (bul->code_bullet == 89){
			SetCaret(npc.x, npc.y, 12, 0);
			if (Random(0, 4) == 0 && !isEthereal)
				for (int i = 0; i < 5; i++)
					SpawnQuiverProjectile(npc);
		}

		if (bul->code_bullet == 60 || bul->code_bullet == 74)
			SpawnQuiverProjectile(npc);
	}

	if (ChallengeEnabled("oneDamage") && dmg > 0) //one damage option
		dmg = 1;

	npc.life -= dmg;

	if (Random(0, 2) == 0 && dmg != 0 && gMC.equip & EQUIP_NANO_QUIVER && 
		(bul == NULL || (bul->code_bullet != 47 && bul->code_bullet != 45)))
	{
		SpawnQuiverProjectile(npc);
	}

	for (int b1 = 0; b1 < BULLET_MAX; b1++){
		if (gBul[b1].code_bullet == 57 && gBul[b1].npc == NULL && !isEthereal){ //whimsical star targetting
			gBul[b1].npc = &npc;
			break;
		}
	}

	bool isSword = bul != NULL;
	if (isSword == true)
		isSword = (bul->code_bullet == 58 || bul->code_bullet == 60 || bul->code_bullet == 59 || bul->code_bullet == 74
			|| bul->code_bullet == 80 || bul->code_bullet == 81 || bul->code_bullet == 82 || bul->code_bullet == 85);

	if (npc.life < 1 || ChallengeEnabled("debug"))
	{
		npc.life = 0;

		if (npc.bits & NPC_SHOW_DAMAGE && !ChallengeEnabled("debug"))
			npc.damage_view -= dmg;

		if ((gMC.cond & 0x80) && npc.bits & NPC_EVENT_WHEN_KILLED)
			StartTextScript(npc.code_event);
		else
			npc.cond |= 8;

		if (isSword){
			if (Random(0, 1))
				SetBullet(79, npc.x, npc.y, 0);
			else
				SetBullet(79, npc.x, npc.y, 2);
			if (bul->code_bullet == 58 || bul->code_bullet == 74){
				PlaySoundObject(65, SOUND_MODE_PLAY);
				int lol = Random(20, 30);
				if (bul->count1 == 1) lol *= 1.5;
				for (int i = 0; i < lol; i++)
				{
					int x, y;
					do
					{
						x = Random(0, gMap.width*0x2000);
						y = Random(0, gMap.length*0x2000);
					} while (!(Distance(npc.x, npc.y, x, y) < 48_pix  && 
								Distance(npc.x, npc.y, x, y) > 4_pix));
					BULLET *bully = SetBullet(61, x, y);
					if (bully != NULL)
					{
						bully->direct = 0;
						bully->xm = Random(-1000, 1000);
						bully->ym = Random(-600, 600);
						if (bul->count2 == 1){
							bully->xm *= 1.4;
							bully->ym *= 1.4;
						}
					}
				}
			}
		}
	}
	else
	{
		if (npc.shock < 14)
		{
			if (!isEthereal){
				int hurtX = bul != NULL ? ((bul->x + npc.x) / 2) : (npc.x + (npc.rect.right - npc.rect.left) / 2);
				int hurtY = bul != NULL ? ((bul->y + npc.y) / 2) : (npc.y + (npc.rect.bottom - npc.rect.top) / 2);
				SetCaret(hurtX, hurtY, 11, 0);
				SetCaret(hurtX, hurtY, 11, 0);
				SetCaret(hurtX, hurtY, 11, 0);
			}
			PlaySoundObject(npc.hit_voice, SOUND_MODE_PLAY);
			npc.shock = 16;
			if (gBL.flag == false && std::find(bossNPCsList.begin(), bossNPCsList.end(), npc.code_char) != bossNPCsList.end())
				StartBossLife(npc.code_event);
			if (isSword)
			{
				if (Random(0, 1))
					SetBullet(79, npc.x, npc.y, 0);
				else
					SetBullet(79, npc.x, npc.y, 2);
				if (std::find(knockImmuneEnemies.begin(), knockImmuneEnemies.end(), npc.code_char) == knockImmuneEnemies.end()){
					if (bul->direct == 0)
						npc.knockx = -Random(0x300, 0x5F0);
					else
						npc.knockx = Random(0x300, 0x5F0);
					if (gMC.y < npc.y)
						npc.knocky = Random(0x300, 0x5F0) * 0.66;
					if (gMC.y > npc.y)
						npc.knocky = -Random(0x300, 0x5F0) * 0.66;
				}
			}
		}

		if (npc.bits & NPC_SHOW_DAMAGE)
			npc.damage_view -= dmg;
		starTarget = &npc;
	}
	
	return true;
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

		bool isEthereal = std::find(etherealEntities.begin(), etherealEntities.end(), gNPC[n].code_char) != etherealEntities.end();

		Rect necessaryRect = {genContext.bossRect.left - 1, genContext.bossRect.top - 1, 
						genContext.bossRect.right, genContext.bossRect.bottom};

		if (gMC.depth > 1 && gMC.depth % genContext.bossRate == 0){
				if (std::find(bossNPCsList.begin(), bossNPCsList.end(), gNPC[n].code_char) != bossNPCsList.end())
					if(IntersectCheck(necessaryRect, gNPC[n].x / 16_pix, gNPC[n].y / 16_pix)){
						if ((gBL.flag == false && !GetNPCFlag(555) && !IntersectCheck(necessaryRect, gMC.x / 16_pix, gMC.y / 16_pix)) || gMusicNo == 52){
							SetCaret(gNPC[n].x - gNPC[n].rect.left / 2, gNPC[n].y + gNPC[n].rect.bottom / 2, 2, 2);
							continue;
						}
					}
		}

		for (b = 0; b < BULLET_MAX; ++b)
		{
			bool isSword = gBul[b].code_bullet == 58 || gBul[b].code_bullet == 60 || gBul[b].code_bullet == 59
				|| gBul[b].code_bullet == 80 || gBul[b].code_bullet == 81 || gBul[b].code_bullet == 82 || gBul[b].code_bullet == 85;

			if (!(gBul[b].cond & 0x80))
				continue;

			if (gBul[b].iframes > 0 && static_iframes[n].contains(gBul[b].code_bullet)){
				if (static_iframes[n][gBul[b].code_bullet] != 0)
					continue;
			}
			if (gBul[b].local_iframes > 0 && local_iframes[n].contains(b)){
				if (local_iframes[n][b] != 0)
					continue;
			}

			if (gNPC[n].shock > 0 && (
				 (gBul[b].code_bullet == 57) || (gBul[b].code_bullet == 69) || (GetNPCFlag(873) && gBul[b].code_bullet > 6 && gBul[b].code_bullet < 10 && CountBulletNum(16))))
				continue;

			if (gBul[b].damage == -1)
				continue;

			if (gBul[b].code_bullet == 4 && gNPC[n].targetingEffect > 0)
				continue;

			if (gBul[b].code_bullet == 77 && gNPC[n].code_char == 48)
				continue;

			if (gNPC[n].shock > 0 && gNPC[n].code_event == 30)
				continue;

			if (gNPC[n].code_char == 126 && gBul[b].code_bullet == 46)
				continue;

			if (gNPC[n].code_char == 100 && !IsBreakingBullet(gBul[b]))
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
					bool success = DealDamage(gNPC[n], gBul[b].damage, &gBul[b]);

					if (!success)
						continue;

					if (gBul[b].code_bullet == 88){
						PlaySoundObject(54);
						SetDestroyNpChar(gNPC[n].x, gNPC[n].y, gNPC[n].hit.back, 16);
					}

					if (gBul[b].iframes > 0)
						static_iframes[n][gBul[b].code_bullet] = gBul[b].iframes;

					if (gBul[b].local_iframes > 0)
						local_iframes[n][b] = gBul[b].local_iframes;
				}
				else if (gBul[b].code_bullet == 13
					|| gBul[b].code_bullet == 14
					|| gBul[b].code_bullet == 15)
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
		}
	}
}
