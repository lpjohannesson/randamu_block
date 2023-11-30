#include "MycHit.h"

#include <algorithm>

#include "CommonDefines.h"

#include "ArmsItem.h"
#include "Back.h"
#include "Boss.h"
#include "BossLife.h"
#include "Bullet.h"
#include "Caret.h"
#include "Game.h"
#include "Flags.h"
#include "KeyControl.h"
#include "Main.h"
#include "Map.h"
#include "MyChar.h"
#include "MycParam.h"
#include "NpChar.h"
#include "NpcHit.h"
#include "NpcTbl.h"
#include "Sound.h"
#include "Stage.h"
#include "TextScr.h"

void ResetMyCharFlag(void)
{
	gMC.flag = 0;
}

static void PutlittleStar(void)
{
	if (!(gMC.cond & 2) && gMC.ym < -0x200)
	{
		PlaySoundObject(3, SOUND_MODE_PLAY);
		SetCaret(gMC.x, gMC.y - gMC.hit.top, 13, 0);
		SetCaret(gMC.x, gMC.y - gMC.hit.top, 13, 0);
	}
}

int JudgeHitMyCharBlock(int x, int y)
{
	int hit = 0;

	// Left wall
	if (gMC.y - gMC.hit.top < (y * 0x10 + 4) * 0x200
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 4) * 0x200
		&& gMC.x - gMC.hit.back < (x * 0x10 + 8) * 0x200
		&& gMC.x - gMC.hit.back > x * 0x10 * 0x200)
	{
		// Clip
		gMC.x = ((x * 0x10 + 8) * 0x200) + gMC.hit.back;

		// Halt momentum
		if (gMC.xm < -0x180)
			gMC.xm = -0x180;
		if (!(gKey & gKeyLeft) && gMC.xm < 0)
			gMC.xm = 0;

		// Set that a left wall was hit
		hit |= 1;
	}

	// Right wall
	if (gMC.y - gMC.hit.top < (y * 0x10 + 4) * 0x200
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 4) * 0x200
		&& gMC.x + gMC.hit.back > (x * 0x10 - 8) * 0x200
		&& gMC.x + gMC.hit.back < x * 0x10 * 0x200)
	{
		// Clip
		gMC.x = ((x * 0x10 - 8) * 0x200) - gMC.hit.back;

		// Halt momentum
		if (gMC.xm > 0x180)
			gMC.xm = 0x180;
		if (!(gKey & gKeyRight) && gMC.xm > 0)
			gMC.xm = 0;

		// Set that a right wall was hit
		hit |= 4;
	}

	// Ceiling
	if (gMC.x - gMC.hit.back < (x * 0x10 + 5) * 0x200
		&& gMC.x + gMC.hit.back > (x * 0x10 - 5) * 0x200
		&& gMC.y - gMC.hit.top < (y * 0x10 + 8) * 0x200
		&& gMC.y - gMC.hit.top > y * 0x10 * 0x200)
	{
		// Clip
		gMC.y = ((y * 0x10 + 8) * 0x200) + gMC.hit.top;

		// Halt momentum
		if (!gMC.flip){
			if (!(gMC.cond & 2) && gMC.ym < -0x200)
				PutlittleStar();
			}
		else {
			if (gMC.ym < -0x400 && !(gMC.equip & EQUIP_HEAVY_WEIGHT))
				PlaySoundObject(23, SOUND_MODE_PLAY);
		}
		if (gMC.ym < 0)
			gMC.ym = 0;

		// Set that a ceiling was hit
		if (gMC.flip){
			hit |= 8;
			gMC.pogo = true;
		}
		else
			hit |= 2;
	}

	// Floor
	if (gMC.x - gMC.hit.back < (x * 0x10 + 5) * 0x200
		&& gMC.x + gMC.hit.back > ((x * 0x10 - 5) * 0x200)
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 8) * 0x200
		&& gMC.y + gMC.hit.bottom < y * 0x10 * 0x200)
	{
		// Clip
		gMC.y = ((y * 0x10 - 8) * 0x200) - gMC.hit.bottom;

		// Halt momentum
		if (gMC.flip){
			if (!(gMC.cond & 2)/*  && gMC.ym > 0x200 */)
				PutlittleStar();
		}
		else {
			if (gMC.ym > 0x400 && !(gMC.equip & EQUIP_HEAVY_WEIGHT))
				PlaySoundObject(23, SOUND_MODE_PLAY);
		}
		if (gMC.ym > 0)
			gMC.ym = 0;

		// Set that a floor was hit
		if (gMC.flip)
			hit |= 2;
		else {
			hit |= 8;
			gMC.pogo = true;
		}
	}

	return hit;
}

int JudgeHitMyCharTriangleA(int x, int y)
{
	int hit = 0;

	if (gMC.x < (x * 0x10 + 8) * 0x200
		&& gMC.x > (x * 0x10 - 8) * 0x200
		&& gMC.y - gMC.hit.top < (y * 0x10 * 0x200) - (gMC.x - x * 0x10 * 0x200) / 2 + 0x800
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 8) * 0x200)
	{
		// Clip
		gMC.y = (y * 0x10 * 0x200) - ((gMC.x - x * 0x10 * 0x200) / 2) + 0x800 + gMC.hit.top;

		// Halt momentum
		if (!(gMC.cond & 2) && gMC.ym < -0x200)
			PutlittleStar();
		if (gMC.ym < 0)
			gMC.ym = 0;

		// Set that hit a ceiling
		if (gMC.flip)
			hit |= 8;
		else
			hit |= 2;
	}

	return hit;
}

int JudgeHitMyCharTriangleB(int x, int y)
{
	int hit = 0;

	if (gMC.x < (x * 0x10 + 8) * 0x200
		&& gMC.x > (x * 0x10 - 8) * 0x200
		&& gMC.y - gMC.hit.top < (y * 0x10 * 0x200) - ((gMC.x - x * 0x10 * 0x200) / 2) - 0x800
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 8) * 0x200)
	{
		// Clip
		gMC.y = (y * 0x10 * 0x200) - ((gMC.x - x * 0x10 * 0x200) / 2) - 0x800 + gMC.hit.top;

		// Halt momentum
		if (!(gMC.cond & 2) && gMC.ym < -0x200)
			PutlittleStar();
		if (gMC.ym < 0)
			gMC.ym = 0;

		// Set that hit a ceiling
		if (gMC.flip)
			hit |= 8;
		else
			hit |= 2;
	}

	return hit;
}

int JudgeHitMyCharTriangleC(int x, int y)
{
	int hit = 0;

	if (gMC.x < (x * 0x10 + 8) * 0x200
		&& gMC.x > (x * 0x10 - 8) * 0x200
		&& gMC.y - gMC.hit.top < (y * 0x10 * 0x200) + ((gMC.x - x * 0x10 * 0x200) / 2) - 0x800
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 8) * 0x200)
	{
		// Clip
		gMC.y = (y * 0x10 * 0x200) + ((gMC.x - x * 0x10 * 0x200) / 2) - 0x800 + gMC.hit.top;

		// Halt momentum
		if (!(gMC.cond & 2) && gMC.ym < -0x200)
			PutlittleStar();
		if (gMC.ym < 0)
			gMC.ym = 0;

		// Set that hit a ceiling
		if (gMC.flip)
			hit |= 8;
		else
			hit |= 2;
	}

	return hit;
}

int JudgeHitMyCharTriangleD(int x, int y)
{
	int hit = 0;

	if (gMC.x < (x * 0x10 + 8) * 0x200
		&& gMC.x > (x * 0x10 - 8) * 0x200
		&& gMC.y - gMC.hit.top < (y * 0x10 * 0x200) + ((gMC.x - x * 0x10 * 0x200) / 2) + 0x800
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 8) * 0x200)
	{
		// Clip
		gMC.y = (y * 0x10 * 0x200) + ((gMC.x - x * 0x10 * 0x200) / 2) + 0x800 + gMC.hit.top;

		// Halt momentum
		if (!(gMC.cond & 2) && gMC.ym < -0x200)
			PutlittleStar();
		if (gMC.ym < 0)
			gMC.ym = 0;

		// Set that hit a ceiling
		if (gMC.flip)
			hit |= 8;
		else
			hit |= 2;
	}

	return hit;
}

int JudgeHitMyCharTriangleE(int x, int y)
{
	int hit = 0;

	hit |= 0x10000;

	if (gMC.x < (x * 0x10 + 8) * 0x200
		&& gMC.x > (x * 0x10 - 8) * 0x200
		&& gMC.y + gMC.hit.bottom > (y * 0x10 * 0x200) + ((gMC.x - x * 0x10 * 0x200) / 2) - 0x800
		&& gMC.y - gMC.hit.top < (y * 0x10 + 8) * 0x200)
	{
		// Clip
		gMC.y = (y * 0x10 * 0x200) + ((gMC.x - x * 0x10 * 0x200) / 2) - 0x800 - gMC.hit.bottom;

		// Halt momentum
		if (gMC.ym > 0x400)
			PlaySoundObject(23, SOUND_MODE_PLAY);
		if (gMC.ym > 0)
			gMC.ym = 0;

		// Set that hit this slope
		hit |= 0x28;
	}

	return hit;
}

int JudgeHitMyCharTriangleF(int x, int y)
{
	int hit = 0;

	hit |= 0x20000;

	if (gMC.x < (x * 0x10 + 8) * 0x200
		&& gMC.x > (x * 0x10 - 8) * 0x200
		&& gMC.y + gMC.hit.bottom > (y * 0x10 * 0x200) + ((gMC.x - x * 0x10 * 0x200) / 2) + 0x800
		&& gMC.y - gMC.hit.top < (y * 0x10 + 8) * 0x200)
	{
		// Clip
		gMC.y = (y * 0x10 * 0x200) + ((gMC.x - x * 0x10 * 0x200) / 2) + 0x800 - gMC.hit.bottom;

		// Halt momentum
		if (gMC.ym > 0x400)
			PlaySoundObject(23, SOUND_MODE_PLAY);
		if (gMC.ym > 0)
			gMC.ym = 0;

		// Set that hit this slope
		hit |= 0x28;
	}

	return hit;
}

int JudgeHitMyCharTriangleG(int x, int y)
{
	int hit = 0;

	hit |= 0x40000;

	if (gMC.x < (x * 0x10 + 8) * 0x200
		&& gMC.x > (x * 0x10 - 8) * 0x200
		&& gMC.y + gMC.hit.bottom > (y * 0x10 * 0x200) - ((gMC.x - x * 0x10 * 0x200) / 2) + 0x800
		&& gMC.y - gMC.hit.top < (y * 0x10 + 8) * 0x200)
	{
		// Clip
		gMC.y = (y * 0x10 * 0x200) - ((gMC.x - x * 0x10 * 0x200) / 2) + 0x800 - gMC.hit.bottom;

		// Halt momentum
		if (gMC.ym > 0x400)
			PlaySoundObject(23, SOUND_MODE_PLAY);
		if (gMC.ym > 0)
			gMC.ym = 0;

		// Set that hit this slope
		hit |= 0x18;
	}

	return hit;
}

int JudgeHitMyCharTriangleH(int x, int y)
{
	int hit = 0;

	hit |= 0x80000;

	if (gMC.x < (x * 0x10 + 8) * 0x200
		&& gMC.x > (x * 0x10 - 8) * 0x200
		&& gMC.y + gMC.hit.bottom > (y * 0x10 * 0x200) - ((gMC.x - x * 0x10 * 0x200) / 2) - 0x800
		&& gMC.y - gMC.hit.top < (y * 0x10 + 8) * 0x200)
	{
		// Clip
		gMC.y = (y * 0x10 * 0x200) - ((gMC.x - x * 0x10 * 0x200) / 2) - 0x800 - gMC.hit.bottom;

		// Halt momentum
		if (gMC.ym > 0x400)
			PlaySoundObject(23, SOUND_MODE_PLAY);
		if (gMC.ym > 0)
			gMC.ym = 0;

		// Set that hit this slope
		hit |= 0x18;
	}

	return hit;
}

int JudgeHitMyCharWater(int x, int y)
{
	int hit = 0;

	if (gMC.x - gMC.hit.back < (x * 0x10 + 5) * 0x200
		&& gMC.x + gMC.hit.back > ((x * 0x10 - 5) * 0x200)
		&& gMC.y - gMC.hit.top < ((y * 0x10 + 5) * 0x200)
		&& gMC.y + gMC.hit.bottom > y * 0x10 * 0x200)
		hit |= 0x100;

	return hit;
}

int JudgeHitMyCharDamage(int x, int y)
{
	int hit = 0;

	if (abs(gMC.xm) > 1250 && (GetNPCFlag(Flags::SUE_CHARACTER) || GetNPCFlag(Flags::OMNIQUICK_SNEAKERS))) return hit;

	if (gMC.dashDuration > 0) return hit;

	if (gMC.x - 0x800 < (x * 0x10 + 4) * 0x200
		&& gMC.x + 0x800 > (x * 0x10 - 4) * 0x200
		&& gMC.y - 0x800 < (y * 0x10 + 3) * 0x200
		&& gMC.y + 0x800 > (y * 0x10 - 3) * 0x200)
		hit |= 0x400;

	return hit;
}

int JudgeHitMyCharDamageW(int x, int y)
{
	int hit = 0;

	if (abs(gMC.xm) > 1250 && (GetNPCFlag(Flags::SUE_CHARACTER) || GetNPCFlag(Flags::OMNIQUICK_SNEAKERS))) return hit;

	if (gMC.dashDuration > 0) return hit;

	if (gMC.x - 0x800 < (x * 0x10 + 4) * 0x200
		&& gMC.x + 0x800 > (x * 0x10 - 4) * 0x200
		&& gMC.y - 0x800 < (y * 0x10 + 3) * 0x200
		&& gMC.y + 0x800 > (y * 0x10 - 3) * 0x200)
		hit |= 0xD00;

	return hit;
}

int JudgeHitMyCharVectLeft(int x, int y)
{
	int hit = 0;
	if (gMC.x - gMC.hit.back < (x * 0x10 + 6) * 0x200
		&& gMC.x + gMC.hit.back > (x * 0x10 - 6) * 0x200
		&& gMC.y - gMC.hit.top < (y * 0x10 + 6) * 0x200
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 6) * 0x200)
		hit |= 0x1000;

	return hit;
}

int JudgeHitMyCharVectUp(int x, int y)
{
	int hit = 0;
	if (gMC.x - gMC.hit.back < (x * 0x10 + 6) * 0x200
		&& gMC.x + gMC.hit.back > (x * 0x10 - 6) * 0x200
		&& gMC.y - gMC.hit.top < (y * 0x10 + 6) * 0x200
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 6) * 0x200)
		hit |= 0x2000;

	return hit;
}

int JudgeHitMyCharVectRight(int x, int y)
{
	int hit = 0;
	if (gMC.x - gMC.hit.back < (x * 0x10 + 6) * 0x200
		&& gMC.x + gMC.hit.back > (x * 0x10 - 6) * 0x200
		&& gMC.y - gMC.hit.top < (y * 0x10 + 6) * 0x200
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 6) * 0x200)
		hit |= 0x4000;

	return hit;
}

int JudgeHitMyCharVectDown(int x, int y)
{
	int hit = 0;
	if (gMC.x - gMC.hit.back < (x * 0x10 + 6) * 0x200
		&& gMC.x + gMC.hit.back > (x * 0x10 - 6) * 0x200
		&& gMC.y - gMC.hit.top < (y * 0x10 + 6) * 0x200
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 6) * 0x200)
		hit |= 0x8000;

	return hit;
}

void HitMyCharMap(void)
{

	int x, y;
	int i;
	unsigned char atrb[4];

	x = gMC.x / 0x10 / 0x200;
	y = gMC.y / 0x10 / 0x200;

	int offx[4];
	int offy[4];

	offx[0] = 0;
	offx[1] = 1;
	offx[2] = 0;
	offx[3] = 1;

	offy[0] = 0;
	offy[1] = 0;
	offy[2] = 1;
	offy[3] = 1;

	for (i = 0; i < 4; ++i)
	{
		atrb[i] = GetAttribute(x + offx[i], y + offy[i]);

		switch (atrb[i])
		{
			// Block
			case 0x05:
			case 0x41:
			case 0x43:
			case 0x46:
				gMC.flag |= JudgeHitMyCharBlock(x + offx[i], y + offy[i]);
				break;

			// Slopes
			case 0x50:
				gMC.flag |= JudgeHitMyCharTriangleA(x + offx[i], y + offy[i]);
				break;

			case 0x51:
				gMC.flag |= JudgeHitMyCharTriangleB(x + offx[i], y + offy[i]);
				break;

			case 0x52:
				gMC.flag |= JudgeHitMyCharTriangleC(x + offx[i], y + offy[i]);
				break;

			case 0x53:
				gMC.flag |= JudgeHitMyCharTriangleD(x + offx[i], y + offy[i]);
				break;

			case 0x54:
				gMC.flag |= JudgeHitMyCharTriangleE(x + offx[i], y + offy[i]);
				break;

			case 0x55:
				gMC.flag |= JudgeHitMyCharTriangleF(x + offx[i], y + offy[i]);
				break;

			case 0x56:
				gMC.flag |= JudgeHitMyCharTriangleG(x + offx[i], y + offy[i]);
				break;

			case 0x57:
				gMC.flag |= JudgeHitMyCharTriangleH(x + offx[i], y + offy[i]);
				break;

			// Spikes
			case 0x42:
				gMC.flag |= JudgeHitMyCharDamage(x + offx[i], y + offy[i]);
				break;

			// Water spikes
			case 0x62:
				gMC.flag |= JudgeHitMyCharDamageW(x + offx[i], y + offy[i]);
				break;

			// Wind
			case 0x80:
				gMC.flag |= JudgeHitMyCharVectLeft(x + offx[i], y + offy[i]);
				break;

			case 0x81:
				gMC.flag |= JudgeHitMyCharVectUp(x + offx[i], y + offy[i]);
				break;

			case 0x82:
				gMC.flag |= JudgeHitMyCharVectRight(x + offx[i], y + offy[i]);
				break;

			case 0x83:
				gMC.flag |= JudgeHitMyCharVectDown(x + offx[i], y + offy[i]);
				break;

			// Water
			case 0x02:
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;

			// Water and water blocks (same as the previous case)
			case 0x60:
				if (!gMC.hell){
					gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
					break;
				}
				else {
					if (gMC.air >= 1 && gMC.equip & EQUIP_AIR_TANK)
						gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
					else 
						gMC.flag |= JudgeHitMyCharDamageW(x + offx[i], y + offy[i]); break;
				}
					

			case 0x61:
				gMC.flag |= JudgeHitMyCharBlock(x + offx[i], y + offy[i]);
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;

			// Water slopes
			case 0x70:
				gMC.flag |= JudgeHitMyCharTriangleA(x + offx[i], y + offy[i]);
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;

			case 0x71:
				gMC.flag |= JudgeHitMyCharTriangleB(x + offx[i], y + offy[i]);
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;

			case 0x72:
				gMC.flag |= JudgeHitMyCharTriangleC(x + offx[i], y + offy[i]);
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;

			case 0x73:
				gMC.flag |= JudgeHitMyCharTriangleD(x + offx[i], y + offy[i]);
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;

			case 0x74:
				gMC.flag |= JudgeHitMyCharTriangleE(x + offx[i], y + offy[i]);
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;

			case 0x75:
				gMC.flag |= JudgeHitMyCharTriangleF(x + offx[i], y + offy[i]);
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;

			case 0x76:
				gMC.flag |= JudgeHitMyCharTriangleG(x + offx[i], y + offy[i]);
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;

			case 0x77:
				gMC.flag |= JudgeHitMyCharTriangleH(x + offx[i], y + offy[i]);
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;

			// Water current
			case 0xA0:
				gMC.flag |= JudgeHitMyCharVectLeft(x + offx[i], y + offy[i]);
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;

			case 0xA1:
				gMC.flag |= JudgeHitMyCharVectUp(x + offx[i], y + offy[i]);
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;

			case 0xA2:
				gMC.flag |= JudgeHitMyCharVectRight(x + offx[i], y + offy[i]);
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;

			case 0xA3:
				gMC.flag |= JudgeHitMyCharVectDown(x + offx[i], y + offy[i]);
				gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
				break;
		}
	}

	if (gMC.y > gWaterY + (4 * 0x200))
		gMC.flag |= 0x100;
}

int JudgeHitMyCharNPC(NPCHAR *npc)
{
	int hit = 0;
	int mod = 1;
	if (GetNPCFlag(Flags::MAXY_CHARACTER) && npc->damage){
		mod = 3;
	}

	if (gMC.y - gMC.hit.top < npc->y + npc->hit.bottom / mod - (3 * 0x200)
		&& gMC.y + gMC.hit.bottom > npc->y - npc->hit.top / mod + (3 * 0x200)
		&& gMC.x - gMC.hit.back < npc->x + npc->hit.back / mod
		&& gMC.x - gMC.hit.back > npc->x)
	{
		if (gMC.xm < 0x200)
			gMC.xm += 0x200;
		hit |= 1;
	}

	if (gMC.y - gMC.hit.top < npc->y + npc->hit.bottom / mod - (3 * 0x200)
		&& gMC.y + gMC.hit.bottom > npc->y - npc->hit.top / mod + (3 * 0x200)
		&& gMC.x + gMC.hit.back - 0x200 > npc->x - npc->hit.back / mod
		&& gMC.x + gMC.hit.back - 0x200 < npc->x)
	{
		if (gMC.xm > -0x200)
			gMC.xm -= 0x200;
		hit |= 4;
	}

	if (gMC.x - gMC.hit.back < npc->x + npc->hit.back / mod - (3 * 0x200)
		&& gMC.x + gMC.hit.back > npc->x - npc->hit.back / mod + (3 * 0x200)
		&& gMC.y - gMC.hit.top < npc->y + npc->hit.bottom / mod
		&& gMC.y - gMC.hit.top > npc->y)
	{
		if (gMC.ym < 0)
			gMC.ym = 0;
		if (gMC.flip)
			hit |= 8;
		else
			hit |= 2;
	}

	if (gMC.x - gMC.hit.back < npc->x + npc->hit.back / mod - (3 * 0x200)
		&& gMC.x + gMC.hit.back > npc->x - npc->hit.back / mod + (3 * 0x200)
		&& gMC.y + gMC.hit.bottom > npc->y - npc->hit.top / mod
		&& gMC.hit.bottom + gMC.y < npc->y + (3 * 0x200))
	{
		if (npc->bits & NPC_BOUNCY)
		{
			gMC.ym = npc->ym - 0x200;
			if (gMC.flip)
			hit |= 2;
		else
			hit |= 8;
		}
		else if (!(gMC.flag & 8) && gMC.ym > npc->ym)
		{
			gMC.y = npc->y - npc->hit.top - gMC.hit.bottom / mod + 0x200;
			gMC.ym = npc->ym;
			gMC.x += npc->xm;
			if (gMC.flip)
			hit |= 2;
		else
			hit |= 8;
		}
	}

	return hit;
}

unsigned char JudgeHitMyCharNPC3(NPCHAR *npc)
{
	int size = 2 * 0x200;
	int mod = 1;
	if (GetNPCFlag(Flags::MAXY_CHARACTER) && npc->damage){
		size = 0x200;
		mod = 3;
	}

	if (npc->direct == 0)
	{
		if (gMC.x + size > npc->x - npc->hit.front / mod
			&& gMC.x - size < npc->x + npc->hit.back / mod
			&& gMC.y + size > npc->y - npc->hit.top / mod
			&& gMC.y - size < npc->y + npc->hit.bottom / mod)
			return 1;
	}
	else
	{
		if (gMC.x + size > npc->x - npc->hit.back / mod
			&& gMC.x - size < npc->x + npc->hit.front / mod
			&& gMC.y + size > npc->y - npc->hit.top / mod
			&& gMC.y - size < npc->y + npc->hit.bottom / mod)
			return 1;
	}

	return 0;
}

int JudgeHitMyCharNPC4(NPCHAR *npc)
{
	// TODO: comment this
	int hit = 0;
	int mod = 1;
	if (GetNPCFlag(Flags::MAXY_CHARACTER) && npc->damage){
		mod = 3;
	}

	float fy1;
	float fx1;
	float fx2;
	float fy2;

	if (npc->x > gMC.x)
		fx1 = (float)(npc->x - gMC.x);
	else
		fx1 = (float)(gMC.x - npc->x);

	if (npc->y > gMC.y)
		fy1 = (float)(npc->y - gMC.y);
	else
		fy1 = (float)(gMC.y - npc->y);

	fx2 = (float)npc->hit.back/mod;
	fy2 = (float)npc->hit.top/mod;

	if (fx1 == 0.0f)
		fx1 = 1.0f;
	if (fx2 == 0.0f)
		fx2 = 1.0f;

	if (fy1 / fx1 > fy2 / fx2)
	{
		if (gMC.x - gMC.hit.back < npc->x + npc->hit.back/mod && gMC.x + gMC.hit.back > npc->x - npc->hit.back/mod)
		{
			if (gMC.y - gMC.hit.top < npc->y + npc->hit.bottom/mod && gMC.y - gMC.hit.top > npc->y)
			{
				if (gMC.ym < npc->ym)
				{
					gMC.y = npc->y + npc->hit.bottom/mod + gMC.hit.top + 0x200;
					gMC.ym = npc->ym;
				}
				else
				{
					if (gMC.ym < 0)
						gMC.ym = 0;
				}

				if (gMC.flip)
					hit |= 8;
				else
					hit |= 2;
			}

			if (gMC.y + gMC.hit.bottom > npc->y - npc->hit.top/mod && gMC.hit.bottom + gMC.y < npc->y + (3 * 0x200))
			{
				if (gMC.ym - npc->ym > 2 * 0x200)
					PlaySoundObject(23, SOUND_MODE_PLAY);

				if (gMC.unit == 1)
				{
					gMC.y = npc->y - npc->hit.top/mod - gMC.hit.bottom + 0x200;
					if (gMC.flip)
						hit |= 2;
					else
						hit |= 8;
				}
				else if (npc->bits & NPC_BOUNCY)
				{
					gMC.ym = npc->ym - 0x200;
					if (gMC.flip)
						hit |= 2;
					else
						hit |= 8;
				}
				else if (!(gMC.flag & 8) && gMC.ym > npc->ym)
				{
					gMC.y = npc->y - npc->hit.top/mod - gMC.hit.bottom + 0x200;
					gMC.ym = npc->ym;
					gMC.x += npc->xm;
					if (gMC.flip)
						hit |= 2;
					else
						hit |= 8;
				}
			}
		}
	}
	else
	{
		if (gMC.y - gMC.hit.top < npc->y + npc->hit.bottom/mod && gMC.y + gMC.hit.bottom > npc->y - npc->hit.top/mod)
		{
			if (gMC.x - gMC.hit.back < npc->x + npc->hit.back/mod && gMC.x - gMC.hit.back > npc->x)
			{
				if (gMC.xm < npc->xm)
					gMC.xm = npc->xm;

				gMC.x = npc->x + npc->hit.back/mod + gMC.hit.back;

				hit |= 1;
			}

			if (gMC.x + gMC.hit.back > npc->x - npc->hit.back/mod && gMC.hit.back + gMC.x < npc->x)
			{
				if (gMC.xm > npc->xm)
					gMC.xm = npc->xm;

				gMC.x = npc->x - npc->hit.back/mod - gMC.hit.back;

				hit |= 4;
			}
		}
	}

	return hit;
}

int hitTimer = 0;
int grassTimer = 0;

void HitMyCharNpChar(void)
{
	int i;
	int hit = 0;

	if (!(gMC.cond & 0x80) || gMC.cond & 2)
		return;

	if (--hitTimer < 0)
			hitTimer = 0;

	for (i = 0; i < NPC_MAX; ++i)
	{
		if (!(gNPC[i].cond & 0x80))
			continue;

		hit = 0;

		if (gNPC[i].bits & NPC_SOLID_SOFT)
		{
			hit = JudgeHitMyCharNPC(&gNPC[i]);
			gMC.flag |= hit;
		}
		else if (gNPC[i].bits & NPC_SOLID_HARD)
		{
			hit = JudgeHitMyCharNPC4(&gNPC[i]);
			gMC.flag |= hit;
		}
		else
		{
			hit = JudgeHitMyCharNPC3(&gNPC[i]);
		}

		if ((std::find(aquaticEntities.begin(), aquaticEntities.end(), gNPC[i].code_char) != aquaticEntities.end()) && (gMC.equip & EQUIP_FISHY || GetNPCFlag(Flags::DIVING_SUIT))){
			hit = 0;
		}

		if (gMC.x + 0x200 * 8 > gNPC[i].x - gNPC[i].hit.front
			&& gMC.x - 0x200 * 8 < gNPC[i].x + gNPC[i].hit.back
			&& gMC.y + 0x200 * 8 > gNPC[i].y - gNPC[i].hit.top
			&& gMC.y - 0x200 * 8 < gNPC[i].y + gNPC[i].hit.bottom
			&& gMC.dashDuration > 0 && gNPC[i].damage && gNPC[i].life == 1 && (GetNPCFlag(Flags::MAXY_CHARACTER) || GetNPCFlag(Flags::MAHIN_CHARACTER))){
				SetDestroyNpChar(gNPC[i].x, gNPC[i].y, 5 * 0x200, 10);
				LoseNpChar(&gNPC[i], false);
		}

		// Special NPCs (pickups)
		if (hit != 0 && gNPC[i].code_char == 1)
		{
			PlaySoundObject(14, SOUND_MODE_PLAY);
			AddExpMyChar(gNPC[i].exp);
			gNPC[i].cond = 0;
		}

		if (hit != 0 && gNPC[i].code_char == 214){
			if (gMC.freeze < 700)
			gMC.freeze += 30;
		}

		if (hit != 0 && gNPC[i].code_char == 30){
			if (++grassTimer > 100){
				grassTimer = 0;
				AddLifeMyChar(1);
			}
			if (gNPC[i].shock == 0 && gMC.xm != 0)
				DealDamage(gNPC[i], 1);
		}

		if (hit != 0 && gNPC[i].code_char == 86)
		{
			if (gBL.flag == true){
				if (static_iframes[i][0] == 0){
					SetCaret((gMC.x + gNPC[i].x) / 2, (gMC.y + gNPC[i].y) / 2, 2, 2);
					PlaySoundObject(31, SOUND_MODE_PLAY);
					static_iframes[i][0] = 20;
				}
			} else {
				PlaySoundObject(42, SOUND_MODE_PLAY);
				AddBulletMyChar(gNPC[i].code_event, gNPC[i].exp);
				SetCaret(gNPC[i].x, gNPC[i].y, 3, 0);
				SetCaret(gNPC[i].x, gNPC[i].y, 18, 0);
				if (gMC.a == 10){
					StartTextScript(305);
				}
				if (gMC.a == 20){
					StartTextScript(303);
				}
					gNPC[i].cond = 0;
			}
		}

		if (hit != 0 && gNPC[i].code_char == 87)
		{
			PlaySoundObject(20, SOUND_MODE_PLAY);
			AddLifeMyChar(gNPC[i].exp);
			gMC.air = std::min(gMC.air + gNPC[i].exp * 50, 1000);
			gNPC[i].cond = 0;
		}

		if (hit != 0 && gNPC[i].code_char == 299 && !(gMC.flag & 0x100))
		{
			// PlaySoundObject(20, SOUND_MODE_PLAY);
			gMC.flag |= 0x100;
		}
		if (hit != 0 && gNPC[i].code_char == 299 && (gMC.flag & 0x100))
		{
			// PlaySoundObject(20, SOUND_MODE_PLAY);
			SetNPCFlag(Flags::WATER_GUN_EFFECT);
		} else if (!(gMC.flag & 0x100)){
			CutNPCFlag(Flags::WATER_GUN_EFFECT);
		}

		if (hit != 0 && gNPC[i].code_char == 270){
			gMC.air += 25;
			SetCaret(gNPC[i].x, gNPC[i].y, 2, 0);
			if (gMC.air > 1000){
				gMC.air = 1000;
			}
			gNPC[i].cond = 0;
		}

		if (hit != 0){
			if ((gMC.dashDuration > 0)){
				if ((gMC.equip & EQUIP_SUPER_DASH || GetNPCFlag(Flags::SHIELD_OF_TIME))&& 
				std::find(bonkableEnemies.begin(), bonkableEnemies.end(), gNPC[i].code_char) != bonkableEnemies.end()){
					if (GetNPCFlag(Flags::SACRED_AEGIS)){
						SetBullet(16, gMC.x,  gMC.y, 0);
						SetBullet(16, gMC.x,  gMC.y, 0);
					} else {
						if (IntersectDamageCheck(gNPC[i], gMC.x, gMC.y))
							DealDamage(gNPC[i], 30);
						gMC.xm /= -4;
						gMC.dashLock = -1;
						gMC.dashDuration = 0;
						gMC.shock += 40;
						if (GetNPCFlag(Flags::SHIELD_OF_TIME)){
							gNPC[i].freeze = 320;
							IncrementCooldown(gCooldowns, DASH_TIMER_ID, 120);
						}
					}
				} else {
					int dashDamage = 4;
					if (gMC.ameDashState){
						dashDamage += 3;
					}
					if (IntersectDamageCheck(gNPC[i], gMC.x, gMC.y))
						DealDamage(gNPC[i], dashDamage);
				}
			}
		}

		if (hit != 0 && gNPC[i].bits & NPC_SHOOTABLE && (gMC.equip & EQUIP_HEAVY_WEIGHT) && (gMC.ym >= 200) && gKey & gKeyAbility){
			if (GetNPCFlag(Flags::NUCLEAR_STOMPER)){
				SetBullet(16, gMC.x,  gMC.y, 0);
				SetBullet(16, gMC.x,  gMC.y, 0);
				SetBullet(16, gMC.x,  gMC.y, 0);
				SetBullet(16, gMC.x,  gMC.y, 0);
			} else {
				SetBullet(26, gNPC[i].x, gNPC[i].y, 3);
			}
		}

		// Run event on contact
		if (!(g_GameFlags & GAME_FLAG_IS_GAMEPLAY_NOT_POSSIBLE) && hit != 0 && gNPC[i].bits & NPC_EVENT_WHEN_TOUCHED)
			StartTextScript(gNPC[i].code_event);

		// NPC damage
		if (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED && !(gNPC[i].bits & NPC_INTERACTABLE))
		{
			if (gNPC[i].bits & NPC_REAR_AND_TOP_DONT_HURT)
			{
				if (gMC.saraDash > 0 && hit != 0){
					if (hitTimer == 0){
						DealDamage(gNPC[i], 2);
						hitTimer = 1;
					}
				}
				if ((gMC.equip & EQUIP_HEAVY_WEIGHT) && (gMC.ym >= 0x100) && gKey & gKeyAbility){
				}
				else if ((gMC.dashDuration > 0)){
				}
				else {
					int damage = gNPC[i].damage;
					if (gNPC[i].targetingEffect > 0) damage *= 1.5;
				if (hit & 4 && gNPC[i].xm < 0)
					DamageMyChar(damage);
				if (hit & 1 && gNPC[i].xm > 0)
					DamageMyChar(damage);
				if (hit & 8 && gNPC[i].ym < 0)
					DamageMyChar(damage);
				if (hit & 2 && gNPC[i].ym > 0)
					DamageMyChar(damage);
				}
				if (gMC.life <= 0 && gMC.deathReason == 0) DetermineHitReason(gNPC[i].code_char);
			}
			else if (hit != 0 && gNPC[i].damage && !(g_GameFlags & GAME_FLAG_IS_GAMEPLAY_NOT_POSSIBLE))
			{
				if (gMC.saraDash > 0){
					if (hitTimer == 0){
						DealDamage(gNPC[i], 2);
						hitTimer = 1;
					}
				}
				if ((gMC.equip & EQUIP_HEAVY_WEIGHT) && (gMC.ym >= 0x100) && gKey & gKeyAbility){
				}
				else if ((gMC.dashDuration > 0)){
				}
				else {
					if (ChallengeEnabled("spike_kill") && (gNPC[i].code_char == 246 || gNPC[i].code_char == 114) && gNPC[i].damage == 13){
						Die();
					} else {
						int damage = gNPC[i].damage;
						if (gNPC[i].targetingEffect > 0) damage *= 1.5;
						DamageMyChar(damage);
					}
					if (gNPC[i].code_char == 38) gMC.shock *= 0.88;
					if (gNPC[i].code_char == 70) gMC.shock *= 0.75;
					if (gNPC[i].code_char == 142) gMC.shock *= 0.6;
					if (gMC.life <= 0 && gMC.deathReason == 0) DetermineHitReason(gNPC[i].code_char);
				}
			}
		}

		// Interaction
		if (!(g_GameFlags & GAME_FLAG_IS_GAMEPLAY_NOT_POSSIBLE) && hit != 0 && (gMC.cond & 1 || ((ChallengeEnabled("debug") || gMC.flag & 0x100) && 
			((gKeyTrg & gKeyDown)))) && gNPC[i].bits & NPC_INTERACTABLE)
		{
			lastInteractedEntityID = i;
			if (gNPC[i].code_char == 231){
				gNPC[i].code_event = 999;
				SetNpCharActionNo(999, 10, 0);
				gNPC[i].code_event = 0;
			}
			if (gNPC[i].code_char == 17)
			{
				if (gNPC[i].count1 == 1){
					StartTextScript(15);
				} else {
					if (CheckItem(1)){
						StartTextScript(14);
					} else {
						StartTextScript(17);
					}
				}
			}
			if (gNPC[i].code_char == 15 && FullInventory()){
				StartTextScript(199);
			} else if (gNPC[i].code_char == 15) {
				if (gBL.flag){
					SetCaret((gMC.x + gNPC[i].x) / 2, (gMC.y + gNPC[i].y) / 2, 2, 2);
					PlaySoundObject(31, SOUND_MODE_PLAY);
					gMC.cond &= ~1;
				} else {
					AddScore(1000);
					StartTextScript(gNPC[i].code_event);
				}
			}
			if (gNPC[i].code_char != 15 && gNPC[i].code_char != 17)
				StartTextScript(gNPC[i].code_event);
			if (gNPC[i].code_char == 32 || gNPC[i].code_char == 253) {
				if (gBL.flag){
					SetCaret((gMC.x + gNPC[i].x) / 2, (gMC.y + gNPC[i].y) / 2, 2, 2);
					PlaySoundObject(31, SOUND_MODE_PLAY);
					gMC.cond &= ~1;
				} else {
					SetNPCFlag(gNPC[i].code_flag);
					gNPC[i].cond = 0;
					AddScore(1000);
				}
			}
			
			gMC.xm = 0;
			gMC.ques = false;
		}
	}

	// Create question mark when NPC hasn't been interacted with
	if (gMC.ques)
		SetCaret(gMC.x, gMC.y, 9, 0);
}

void DetermineHitReason(int npcID){
	gMC.deathReason = 0;
	switch (npcID){
		case 6: gMC.deathReason = DeathReason::BEETLE; break;
		case 11: gMC.deathReason = DeathReason::ENERGY_SHOT; break;
		case 24: gMC.deathReason = DeathReason::POWER_CRITTER; break;
		case 26: gMC.deathReason = DeathReason::BAT; break;
		case 35: case 103: gMC.deathReason = DeathReason::MANNAN; break;
		case 33: case 36: gMC.deathReason = DeathReason::BALROG; break;
		case 49: gMC.deathReason = DeathReason::SKULLHEAD; break;
		case 57: gMC.deathReason = DeathReason::CROW; break;
		case 88: case 268: gMC.deathReason = DeathReason::IGOR; break;
		case 94: gMC.deathReason = DeathReason::KURARA; break;
		case 95: gMC.deathReason = DeathReason::JELLY; break;
		case 104: gMC.deathReason = DeathReason::FROGS; break;
		case 110: gMC.deathReason = DeathReason::PUCHI; break;
		case 114: case 246: gMC.deathReason = DeathReason::PRESS; break;
		case 126: gMC.deathReason = DeathReason::PUPPY; break;
		case 140: case 141: case 142: gMC.deathReason = DeathReason::DARK_TOROKO; break;
		case 147: case 148: gMC.deathReason = DeathReason::CRITTER; break;
		case 155: gMC.deathReason = DeathReason::LASER; break;
		case 156: 
			if (gMC.depth > genContext.doctorStage + 3)
				gMC.deathReason = DeathReason::PUCHI; 
			else
				gMC.deathReason = DeathReason::LASER; 
			break;
		case 158: case 257: case 263: case 264: case 265: case 266: gMC.deathReason = DeathReason::DOCTOR_P1; break;
		case 170: case 238: gMC.deathReason = DeathReason::TURRET; break;
		case 174: gMC.deathReason = DeathReason::ARMOR_GAUDIS; break;
		case 176: case 202: gMC.deathReason = DeathReason::CORE_BASE; break;
		case 187: case 188: case 209: gMC.deathReason = DeathReason::METAL_FUZZ; break;
		case 200: case 218: gMC.deathReason = DeathReason::DRAGONS; break;
		case 247: case 248: case 249: case 250: case 251: case 252: gMC.deathReason = DeathReason::DARK_MISERY; break;
		case 267: gMC.deathReason = DeathReason::DOCTOR_P2; break;
		case 276: case 277: gMC.deathReason = DeathReason::RED_DEMON; break;
		case 279: gMC.deathReason = DeathReason::FALLING_BLOCK; break;
		case 283: case 289: case 290: case 301: gMC.deathReason = DeathReason::DARK_POSMIS; break;
		case 284: gMC.deathReason = DeathReason::POSSUE; break;
		case 311: case 312: gMC.deathReason = DeathReason::ARCHER; break;
		case 317: case 309: case 319: case 323: gMC.deathReason = DeathReason::MESA; break;
		case 44: case 122: gMC.deathReason = DeathReason::COLONBALL; break;
		case 38: gMC.deathReason = DeathReason::FIRE; break;
		case 241: case 28: gMC.deathReason = DeathReason::ENV_CRITTERS; break;
		case 330: gMC.deathReason = DeathReason::ROLLINGS; break;
		case 206: case 322: gMC.deathReason = DeathReason::EXPLOSIVES; break;
	}
	switch (gStageNo){
		case 28: //grass chamber
			gMC.deathReason = DeathReason::BALFROG; break;
		case 37: //sandy chamber
			gMC.deathReason = DeathReason::OMEGA; break;
		case 39: //earth chamber
			gMC.deathReason = DeathReason::MONSTER_X; break;
		case 47: //water chamber
			gMC.deathReason = DeathReason::CORE; break;
		case 31: //sky chamber
			gMC.deathReason = DeathReason::JETHEAD; break;
		case 51: //dragon chamber
			gMC.deathReason = DeathReason::SISTERS; break;
		case 87: //ballos
			gMC.deathReason = DeathReason::BALLOS; break;
	}
}

void HitMyCharBoss(void)
{
	int b;
	int hit = 0;

	if (!(gMC.cond & 0x80) || gMC.cond & 2)
		return;

	for (b = 0; b < BOSS_MAX; ++b)
	{
		if (!(gBoss[b].cond & 0x80))
			continue;

		hit = 0;

		if (gBoss[b].bits & NPC_SOLID_SOFT)
		{
			hit = JudgeHitMyCharNPC(&gBoss[b]);
			gMC.flag |= hit;
		}
		else if (gBoss[b].bits & NPC_SOLID_HARD)
		{
			hit = JudgeHitMyCharNPC4(&gBoss[b]);
			gMC.flag |= hit;
		}
		else
		{
			hit = JudgeHitMyCharNPC3(&gBoss[b]);
		}

		if (hit != 0){
			if ((gMC.dashDuration > 0)){
				if ((gMC.equip & EQUIP_SUPER_DASH || GetNPCFlag(Flags::SHIELD_OF_TIME)) && gBoss[b].bits & NPC_SHOOTABLE){
					if (GetNPCFlag(Flags::SACRED_AEGIS)){
						SetBullet(16, gMC.x,  gMC.y, 0);
						SetBullet(16, gMC.x,  gMC.y, 0);
					} else {
						if (IntersectDamageCheck(gBoss[b], gMC.x, gMC.y))
							DealDamage(gBoss[b], 30);
						gMC.xm /= -4;
						gMC.dashLock = -1;
						gMC.dashDuration = 0;
						gMC.shock += 40;
					}
				} else {
					int dashDamage = 4;
					if (gMC.ameDashState){
						dashDamage += 3;
					}
					if (IntersectDamageCheck(gBoss[b], gMC.x, gMC.y))
						DealDamage(gBoss[b], dashDamage);
				}
			}
		}

		if (hit != 0 && gBoss[b].bits & NPC_SHOOTABLE && (gMC.equip & EQUIP_HEAVY_WEIGHT) && (gMC.ym >= 200) && gKey & gKeyAbility){
			if (GetNPCFlag(Flags::NUCLEAR_STOMPER)){
				SetBullet(16, gMC.x,  gMC.y, 0);
				SetBullet(16, gMC.x,  gMC.y, 0);
				SetBullet(16, gMC.x,  gMC.y, 0);
				SetBullet(16, gMC.x,  gMC.y, 0);
			} else {
				SetBullet(26, gBoss[b].x, gBoss[b].y, 3);
			}
		}

		if (!(g_GameFlags & GAME_FLAG_IS_GAMEPLAY_NOT_POSSIBLE) && hit != 0 && gBoss[b].bits & NPC_EVENT_WHEN_TOUCHED)
		{
			StartTextScript(gBoss[b].code_event);
			gMC.ques = false;
		}

		if (gBoss[b].bits & NPC_REAR_AND_TOP_DONT_HURT)
		{
			if (gMC.saraDash > 0 && hit != 0)
			{
				if (hitTimer == 0)
				{
					DealDamage(gBoss[b], 2);
					hitTimer = 1;
				}
			}
			if ((gMC.equip & EQUIP_HEAVY_WEIGHT) && (gMC.ym >= 0x100) && gKey & gKeyAbility)
			{
			}
			else if ((gMC.dashDuration > 0))
			{
			}
			else
			{
				int damage = gBoss[b].damage;
				if (gBoss[gBoss[b].cond & 0x10 ? 0 : b].targetingEffect > 0) damage *= 1.5;
				if (hit & 4 && gBoss[b].xm < 0)
					DamageMyChar(damage);
				if (hit & 1 && gBoss[b].xm > 0)
					DamageMyChar(damage);
				if (gMC.life <= 0 && gMC.deathReason == 0) DetermineHitReason(-1);
			}
		}
		else if (hit != 0 && gBoss[b].damage != 0 && !(g_GameFlags & GAME_FLAG_IS_GAMEPLAY_NOT_POSSIBLE))
		{
			if (gMC.saraDash > 0)
			{
				if (hitTimer == 0)
				{
					DealDamage(gBoss[b], 2);
					hitTimer = 1;
				}
			}
			if ((gMC.equip & EQUIP_HEAVY_WEIGHT) && (gMC.ym >= 0x100) && gKey & gKeyAbility)
			{
			}
			else if ((gMC.dashDuration > 0))
			{
			}
			else
			{
				int damage = gBoss[b].damage;
				if (gBoss[gBoss[b].cond & 0x10 ? 0 : b].targetingEffect > 0) damage *= 1.5;
				DamageMyChar(damage);
			}
			if (gMC.life <= 0 && gMC.deathReason == 0)
				DetermineHitReason(-1);
		}

		if (!(g_GameFlags & GAME_FLAG_IS_GAMEPLAY_NOT_POSSIBLE) && hit != 0 && gMC.cond & 1 && gBoss[b].bits & NPC_INTERACTABLE)
		{
			StartTextScript(gBoss[b].code_event);
			gMC.xm = 0;
			gMC.ques = false;
		}
	}

	if (gMC.ques)
		SetCaret(gMC.x, gMC.y, 9, 0);
}
