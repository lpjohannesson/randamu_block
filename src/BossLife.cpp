#include "BossLife.h"

#include "CommonDefines.h"
#include "Boss.h"
#include "Draw.h"
#include "Game.h"
#include "Map.h"
#include "Stage.h"
#include "NpChar.h"
#include "ResourceCode.h"

BOSSLIFE gBL;

void InitBossLife(void)
{
	gBL.flag = false;
}

bool StartBossLife(int code_event)
{
	int i = 0;
	while (i < NPC_MAX && gNPC[i].code_event != code_event)
		++i;

	if (i == NPC_MAX)
		return false;

	gBL.flag = true;
	gBL.max = gNPC[i].life;
	gBL.br = gNPC[i].life;
	gBL.pLife = &gNPC[i].life;
	if (gMC.difficulty > DIFFICULTY_HARDENED)
		ChangeMusic(MUS_ENEMY_HALL);
	else
		ChangeMusic(MUS_EYES_OF_FLAME);

	if (gStageNo == gGeneratedStage){
		int w = genContext.bossRect.right - genContext.bossRect.left;
		int h = genContext.bossRect.bottom - genContext.bossRect.top;
		PaintRectangle(41, genContext.bossRect.left - 1, genContext.bossRect.top - 1, 
				w + 2, h + 2);
		ReplaceRectangle(41, 68, genContext.bossRect.left, genContext.bossRect.top, w, h);
	}

	return true;
}

bool StartBossLife2(void)
{
	gBL.flag = true;
	gBL.max = gBoss[0].life;
	gBL.br = gBoss[0].life;
	gBL.pLife = &gBoss[0].life;
	ReCallMusic();
	return true;
}

void PutBossLife(void)
{
	Rect rcText = {0, 48, 32, 56};
	Rect rcBox1 = {0, 0, 244, 8};
	Rect rcBox2 = {0, 16, 244, 24};
	Rect rcLife = {0, 24, 0, 32};
	Rect rcBr = {0, 32, 232, 40};
	Rect rcPer = {72, 48, 80, 56};

	if (gBL.flag == false)
		return;

	if (*gBL.pLife < 1)
	{
		gBL.flag = false;
		return;
	}

	rcLife.right = (*gBL.pLife * 198) / gBL.max;

	if (gBL.br > *gBL.pLife)
	{
		if (++gBL.count > 30)
			--gBL.br;
	}
	else
	{
		gBL.count = 0;
	}

	rcBr.right = (gBL.br * 198) / gBL.max;

	PutBitmap3(&grcGame, PixelToScreenCoord((WINDOW_WIDTH / 2) - 128), PixelToScreenCoord(WINDOW_HEIGHT - 20), &rcBox1, SURFACE_ID_TEXT_BOX);
	PutBitmap3(&grcGame, PixelToScreenCoord((WINDOW_WIDTH / 2) - 128), PixelToScreenCoord(WINDOW_HEIGHT - 12), &rcBox2, SURFACE_ID_TEXT_BOX);
	PutBitmap3(&grcGame, PixelToScreenCoord((WINDOW_WIDTH / 2) - 88), PixelToScreenCoord(WINDOW_HEIGHT - 16), &rcBr, SURFACE_ID_TEXT_BOX);
	PutBitmap3(&grcGame, PixelToScreenCoord((WINDOW_WIDTH / 2) - 88), PixelToScreenCoord(WINDOW_HEIGHT - 16), &rcLife, SURFACE_ID_TEXT_BOX);
	PutBitmap3(&grcGame, PixelToScreenCoord((WINDOW_WIDTH / 2) - 120), PixelToScreenCoord(WINDOW_HEIGHT - 16), &rcText, SURFACE_ID_TEXT_BOX);
	PutNumber4((WINDOW_WIDTH - 256) / 2, WINDOW_HEIGHT - 35, *gBL.pLife, true);
    PutBitmap3(&grcGame, PixelToScreenCoord((WINDOW_WIDTH - 256) / 2 + 35), PixelToScreenCoord(WINDOW_HEIGHT - 35), &rcPer, SURFACE_ID_TEXT_BOX);
    PutNumber4((WINDOW_WIDTH - 256) / 2 + 42, WINDOW_HEIGHT - 35, gBL.max, true);
}
