#include "BulHit.h"

#include <cstddef>

#include "CommonDefines.h"

#include "BossLife.h"
#include "Bullet.h"
#include "ArmsItem.h"
#include "Caret.h"
#include "Game.h"
#include "Flags.h"
#include "Map.h"
#include "MyChar.h"
#include "NpChar.h"
#include "ResourceCode.h"
#include "Sound.h"
#include "Stage.h"

static void Vanish(BULLET *bul)
{
	if (bul->code_bullet != Bullets::MINING_LASER && bul->code_bullet != Bullets::MINING_BOOM && bul->code_bullet != 36 && bul->code_bullet != 37 && bul->code_bullet != 38 && bul->code_bullet != 39)
		PlaySoundObject(28, SOUND_MODE_PLAY);
	else if (bul->code_bullet != 36)
		SetCaret(bul->x, bul->y, 2, 1);
	if (bul->code_bullet == 54){
		SetBullet(bul->code_bullet + 1, bul->x, bul->y, bul->count2 == 1 ? 4 : 0);
			SetBullet(bul->code_bullet + 1, bul->x, bul->y, bul->count2 == 1 ? 5 : 1);
			SetBullet(bul->code_bullet + 1, bul->x, bul->y, bul->count2 == 1 ? 6 : 2);
			SetBullet(bul->code_bullet + 1, bul->x, bul->y, bul->count2 == 1 ? 7 : 3);
	}
	bul->cond = 0;
	if (bul->code_bullet != 36)
		SetCaret(bul->x, bul->y, 2, 2);
}

int JudgeHitBulletBlock(int x, int y, BULLET *bul)
{
	int i;
	int hit = 0;
	if (bul->x - bul->blockXL < ((x * 16) + 8) * 1_pix && bul->x + bul->blockXL > ((x * 16) - 8) * 1_pix && bul->y - bul->blockYL < ((y * 16) + 8) * 1_pix && bul->y + bul->blockYL > ((y * 16) - 8) * 1_pix)
		hit |= 0x200;

	if (hit && bul->bbits & 0x60 && InsideMap(x, y) && (GetAttribute(x, y) == 0x43 || (bul->code_bullet == Bullets::MINING_LASER || bul->code_bullet == Bullets::MINING_BOOM || bul->code_bullet == 34 || bul->code_bullet == 35 ||
	 (CheckFlagTrait(FlagTraits::INVIDIA) && (bul->code_bullet == 37 || bul->code_bullet == 38 || bul->code_bullet == 39)))))
	{
		if (gMC.equip & EQUIP_MULTIBREAK && (bul->code_bullet == 34 || bul->code_bullet == 35) && bul->life > 1){
			bul->life--;
			if (bul->life == 0){
				bul->cond = 0;
			} else {
				if (bul->x - bul->blockXL < ((x * 16) + 8) * 1_pix){
					bul->x += bul->blockXL+0x200*17;
				}
				if (bul->x + bul->blockXL > ((x * 16) - 8) * 1_pix){
					bul->x -= bul->blockXL+0x200*17;
				}
			}
		}
		else if (!((CheckFlagTrait(FlagTraits::INVIDIA) && (bul->code_bullet == Bullets::MINING_LASER || bul->code_bullet == Bullets::MINING_BOOM || bul->code_bullet == 37 || bul->code_bullet == 38 || bul->code_bullet == 39))))
			bul->cond = 0;

		if (gStageNo == gGeneratedStage && gBL.flag != true && ((bul->code_bullet == Bullets::MINING_LASER || bul->code_bullet == Bullets::MINING_BOOM || bul->code_bullet == 34 || bul->code_bullet == 35) || (CheckFlagTrait(FlagTraits::INVIDIA) && (bul->code_bullet == 37 || bul->code_bullet == 38 || bul->code_bullet == 39))))
		{
			DestroyBlock(x, y);
			if (bul->code_bullet == Bullets::MINING_BOOM){
				DestroyBlock(x-1, y-1);
				DestroyBlock(x+1, y+1);
				DestroyBlock(x-1, y);
				DestroyBlock(x+1, y);
				DestroyBlock(x, y-1);
				DestroyBlock(x, y+1);
				DestroyBlock(x+1, y-1);
				DestroyBlock(x-1, y+1);
				SetBullet(Bullets::BIGGER_EXPLOSION, ((x * 16) + 8) * 1_pix, ((y * 16) + 8) * 1_pix, 0);
			}
		}
	}

	return hit;
}

int JudgeHitBulletBlock2(int x, int y, unsigned char *atrb, BULLET *bul)
{
	bool block[4];
	int workX, workY;
	int hit, i;

	hit = 0;

	if (bul->bbits & 0x40)
	{
		for (i = 0; i < 4; ++i)
		{
			if (*atrb == 0x41 || *atrb == 0x61)
				block[i] = true;
			else
				block[i] = false;

			++atrb;
		}
	}
	else
	{
		for (i = 0; i < 4; ++i)
		{
			if (*atrb == 0x41 || *atrb == 0x43 || *atrb == 0x61 || (*atrb == 0x46 && bul->code_bullet == 14))
				block[i] = true;
			else
				block[i] = false;

			++atrb;
		}
	}

	workX = ((x * 16) + 8) * 1_pix;
	workY = ((y * 16) + 8) * 1_pix;

	// Left wall
	if (block[0] && block[2])
	{
		if (bul->x - bul->blockXL < workX)
			hit |= 1;
	}
	else if (block[0] && !block[2])
	{
		if (bul->x - bul->blockXL < workX && bul->y - bul->blockYL < workY - 3_pix)
			hit |= 1;
	}
	else if (!block[0] && block[2])
	{
		if (bul->x - bul->blockXL < workX && bul->y + bul->blockYL > workY + 3_pix)
			hit |= 1;
	}

	// Right wall
	if (block[1] && block[3])
	{
		if (bul->x + bul->blockXL > workX)
			hit |= 4;
	}
	else if (block[1] && !block[3])
	{
		if (bul->x + bul->blockXL > workX && bul->y - bul->blockYL < workY - 3_pix)
			hit |= 4;
	}
	else if (!block[1] && block[3])
	{
		if (bul->x + bul->blockXL > workX && bul->y + bul->blockYL > workY + 3_pix)
			hit |= 4;
	}

	// Ceiling
	if (block[0] && block[1])
	{
		if (bul->y - bul->blockYL < workY)
			hit |= 2;
	}
	else if (block[0] && !block[1])
	{
		if (bul->y - bul->blockYL < workY && bul->x - bul->blockXL < workX - 3_pix)
			hit |= 2;
	}
	else if (!block[0] && block[1])
	{
		if (bul->y - bul->blockYL < workY && bul->x + bul->blockXL > workX + 3_pix)
			hit |= 2;
	}

	// Ground
	if (block[2] && block[3])
	{
		if (bul->y + bul->blockYL > workY)
			hit |= 8;
	}
	else if (block[2] && !block[3])
	{
		if (bul->y + bul->blockYL > workY && bul->x - bul->blockXL < workX - 3_pix)
			hit |= 8;
	}
	else if (!block[2] && block[3])
	{
		if (bul->y + bul->blockYL > workY && bul->x + bul->blockXL > workX + 3_pix)
			hit |= 8;
	}

	// Clip
	if (bul->bbits & 8)
	{
		if (!(bul->bbits & 1)){
		if (hit & 1)
			bul->x = workX + bul->blockXL;
		else if (hit & 4)
			bul->x = workX - bul->blockXL;
		else if (hit & 2)
			bul->y = workY + bul->blockYL;
		else if (hit & 8)
			bul->y = workY - bul->blockYL;
		}
	}
	else
	{
		if (hit & 0xF){
			Vanish(bul);
		}
	}

	return hit;
}

int JudgeHitBulletTriangleA(int x, int y, BULLET *bul)
{
	int hit = 0;
	if (bul->x < ((x * 16) + 8) * 1_pix && bul->x > ((x * 16) - 8) * 1_pix && bul->y - (2 * 1_pix) < ((y * 16) * 1_pix) - ((bul->x - ((x * 16) * 1_pix)) / 2) + (4 * 1_pix) && bul->y + (2 * 1_pix) > ((y * 16) - 8) * 1_pix)
	{
		if (bul->bbits & 8)
			bul->y = ((y * 16) * 1_pix) - ((bul->x - ((x * 16) * 1_pix)) / 2) + (6 * 1_pix);
		else
			Vanish(bul);

		hit |= 0x82;
	}

	return hit;
}

int JudgeHitBulletTriangleB(int x, int y, BULLET *bul)
{
	int hit = 0;
	if (bul->x < ((x * 16) + 8) * 1_pix && bul->x > ((x * 16) - 8) * 1_pix && bul->y - (2 * 1_pix) < ((y * 16) * 1_pix) - ((bul->x - ((x * 16) * 1_pix)) / 2) - (4 * 1_pix) && bul->y + (2 * 1_pix) > ((y * 16) - 8) * 1_pix)
	{
		if (bul->bbits & 8)
			bul->y = ((y * 16) * 1_pix) - ((bul->x - ((x * 16) * 1_pix)) / 2) - (2 * 1_pix);
		else
			Vanish(bul);

		hit |= 0x82;
	}

	return hit;
}

int JudgeHitBulletTriangleC(int x, int y, BULLET *bul)
{
	int hit = 0;
	if (bul->x < ((x * 16) + 8) * 1_pix && bul->x > ((x * 16) - 8) * 1_pix && bul->y - (2 * 1_pix) < ((y * 16) * 1_pix) + ((bul->x - ((x * 16) * 1_pix)) / 2) - (4 * 1_pix) && bul->y + (2 * 1_pix) > ((y * 16) - 8) * 1_pix)
	{
		if (bul->bbits & 8)
			bul->y = ((y * 16) * 1_pix) + ((bul->x - ((x * 16) * 1_pix)) / 2) - (2 * 1_pix);
		else
			Vanish(bul);

		hit |= 0x42;
	}

	return hit;
}

int JudgeHitBulletTriangleD(int x, int y, BULLET *bul)
{
	int hit = 0;
	if (bul->x < ((x * 16) + 8) * 1_pix && bul->x > ((x * 16) - 8) * 1_pix && bul->y - (2_pix) < ((y * 16) * 1_pix) + ((bul->x - ((x * 16) * 1_pix)) / 2) + (4_pix) && bul->y + (2_pix) > ((y * 16) - 8) * 1_pix)
	{
		if (bul->bbits & 8)
			bul->y = ((y * 16) * 1_pix) + ((bul->x - ((x * 16) * 1_pix)) / 2) + (6_pix);
		else
			Vanish(bul);

		hit |= 0x42;
	}

	return hit;
}

int JudgeHitBulletTriangleE(int x, int y, BULLET *bul)
{
	int hit = 0;
	if (bul->x < ((x * 16) + 8) * 1_pix && bul->x - (1 * 1_pix) > ((x * 16) - 8) * 1_pix && bul->y + (2 * 1_pix) > ((y * 16) * 1_pix) + ((bul->x - ((x * 16) * 1_pix)) / 2) - (4 * 1_pix) && bul->y - (2 * 1_pix) < ((y * 16) + 8) * 1_pix)
	{
		if (bul->bbits & 8)
			bul->y = ((y * 16) * 1_pix) + ((bul->x - ((x * 16) * 1_pix)) / 2) - (6 * 1_pix);
		else
			Vanish(bul);

		hit |= 0x28;
	}

	return hit;
}

int JudgeHitBulletTriangleF(int x, int y, BULLET *bul)
{
	int hit = 0;
	if (bul->x < ((x * 16) + 8) * 1_pix && bul->x > ((x * 16) - 8) * 1_pix && bul->y + (2 * 1_pix) > ((y * 16) * 1_pix) + ((bul->x - ((x * 16) * 1_pix)) / 2) + (4 * 1_pix) && bul->y - (2 * 1_pix) < ((y * 16) + 8) * 1_pix)
	{
		if (bul->bbits & 8)
			bul->y = ((y * 16) * 1_pix) + ((bul->x - ((x * 16) * 1_pix)) / 2) + (2 * 1_pix);
		else
			Vanish(bul);

		hit |= 0x28;
	}

	return hit;
}

int JudgeHitBulletTriangleG(int x, int y, BULLET *bul)
{
	int hit = 0;
	if (bul->x < ((x * 16) + 8) * 1_pix && bul->x > ((x * 16) - 8) * 1_pix && bul->y + (2 * 1_pix) > ((y * 16) * 1_pix) - ((bul->x - ((x * 16) * 1_pix)) / 2) + (4 * 1_pix) && bul->y - (2 * 1_pix) < ((y * 16) + 8) * 1_pix)
	{
		if (bul->bbits & 8)
			bul->y = ((y * 16) * 1_pix) - ((bul->x - ((x * 16) * 1_pix)) / 2) + (2 * 1_pix);
		else
			Vanish(bul);

		hit |= 0x18;
	}

	return hit;
}

int JudgeHitBulletTriangleH(int x, int y, BULLET *bul)
{
	int hit = 0;
	if (bul->x < ((x * 16) + 8) * 1_pix && bul->x > ((x * 16) - 8) * 1_pix && bul->y + (2 * 1_pix) > ((y * 16) * 1_pix) - ((bul->x - ((x * 16) * 1_pix)) / 2) - (4 * 1_pix) && bul->y - (2 * 1_pix) < ((y * 16) + 8) * 1_pix)
	{
		if (bul->bbits & 8)
			bul->y = ((y * 16) * 1_pix) - ((bul->x - ((x * 16) * 1_pix)) / 2) - (6 * 1_pix);
		else
			Vanish(bul);

		hit |= 0x18;
	}

	return hit;
}

void HitBulletMap(void)
{
	int x, y;
	unsigned char atrb[4];

	int i, j;

	for (i = 0; i < BULLET_MAX; ++i)
	{
		int offx[4];
		int offy[4];

		if (!(gBul[i].cond & 0x80))
			continue;

		x = gBul[i].x / 16_pix;
		y = gBul[i].y / 16_pix;

		// Get surrounding tiles
		offx[0] = 0;
		offx[1] = 1;
		offx[2] = 0;
		offx[3] = 1;
		offy[0] = 0;
		offy[1] = 0;
		offy[2] = 1;
		offy[3] = 1;

		atrb[0] = GetAttribute(x, y);
		atrb[1] = GetAttribute(x + 1, y);
		atrb[2] = GetAttribute(x, y + 1);
		atrb[3] = GetAttribute(x + 1, y + 1);

		// Clear hit tiles
		gBul[i].flag = 0;

		if (gBul[i].bbits & 4)
		{
			// There probably used to be commented-out code here
		}
		else
		{
			for (j = 0; j < 4; ++j)
			{
				if (!(gBul[i].cond & 0x80))
					continue;

				switch (atrb[j])
				{
				case 0x41:
				case 0x43:
				case 0x44:
				case 0x61:
				case 0x64:
					gBul[i].flag |= JudgeHitBulletBlock(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x50:
				case 0x70:
					gBul[i].flag |= JudgeHitBulletTriangleA(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x51:
				case 0x71:
					gBul[i].flag |= JudgeHitBulletTriangleB(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x52:
				case 0x72:
					gBul[i].flag |= JudgeHitBulletTriangleC(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x53:
				case 0x73:
					gBul[i].flag |= JudgeHitBulletTriangleD(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x54:
				case 0x74:
					gBul[i].flag |= JudgeHitBulletTriangleE(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x55:
				case 0x75:
					gBul[i].flag |= JudgeHitBulletTriangleF(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x56:
				case 0x76:
					gBul[i].flag |= JudgeHitBulletTriangleG(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x57:
				case 0x77:
					gBul[i].flag |= JudgeHitBulletTriangleH(x + offx[j], y + offy[j], &gBul[i]);
					break;
				}
			}

			gBul[i].flag |= JudgeHitBulletBlock2(x, y, atrb, &gBul[i]);
		}
	}
}
