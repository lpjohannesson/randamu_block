#include "SelStage.h"

#include <algorithm>
#include <cstring>
#include <string>

#include "Draw.h"
#include "Escape.h"
#include "KeyControl.h"
#include "MyChar.h"
#include "Game.h"
#include "Log.h"
#include "Main.h"
#include "Pause.h"
#include "Sound.h"
#include "TextScr.h"

std::vector<PERMIT_STAGE> gPermitStage;

constexpr const int STAGE_PER_LINE = 6;
constexpr const int IMAGES_PER_LINE = 18;

int gSelectedStage;
int gStageSelectTitleY;

void ClearPermitStage(void)
{
	gPermitStage.clear();
}

bool AddPermitStage(int index, int event)
{
	if (gPermitStage.size() == 25)
		return false;

	gPermitStage.push_back({index, event});

	return true;
}

bool CheckPermitStage(int index){
	return std::find_if(gPermitStage.begin(), gPermitStage.end(), [&index](PERMIT_STAGE ps){
		return ps.index == index;
	}) != gPermitStage.end();
}

bool SubPermitStage(int index)
{
	int i;

	auto erasedStages = std::erase_if(gPermitStage, [&index](PERMIT_STAGE ps){
		return ps.index == index;
	});

	gSelectedStage = 0;

	return erasedStages == 0;
}

bool bChange;

void MoveStageSelectCursor(void)
{
	const int stage_num = gPermitStage.size();
	int stage_x;

	stage_x = (WINDOW_WIDTH - (stage_num * 40)) / 2;	// Unused

	if (stage_num == 0)
		return;

	bChange = false;

	if (gKeyTrg & gKeyLeft)
	{
		if (gSelectedStage % STAGE_PER_LINE == 0)
			gSelectedStage += STAGE_PER_LINE - 1;
		else
			gSelectedStage -= 1;

		bChange = true;
	}

	if (gKeyTrg & gKeyRight)
	{
		if (gSelectedStage == stage_num - 1)
			gSelectedStage = (gSelectedStage / STAGE_PER_LINE) * STAGE_PER_LINE;	// Round down to multiple of 6
		else if (gSelectedStage % STAGE_PER_LINE == STAGE_PER_LINE-1)
			gSelectedStage -= STAGE_PER_LINE-1;	// Loop around row
		else
			gSelectedStage += 1;

		bChange = true;
	}

	if (gKeyTrg & gKeyUp)
	{
		if (gSelectedStage / STAGE_PER_LINE == 0)
			gSelectedStage = 0;	// We're on the first row, go to beginning
		else
			gSelectedStage -= STAGE_PER_LINE;

		bChange = true;
	}

	if (gKeyTrg & gKeyDown)
	{
		if (gSelectedStage / STAGE_PER_LINE == (stage_num - 1) / (STAGE_PER_LINE-1))
			gSelectedStage = 0;	// We're on the last row, go to beginning
		else
			gSelectedStage += STAGE_PER_LINE;

		bChange = true;
	}

	if (gSelectedStage < 0)
		gSelectedStage = stage_num - 1;

	if (gSelectedStage > stage_num - 1)
		gSelectedStage = 0;

	if (bChange){
		StartTextScript(gPermitStage[gSelectedStage].index + 1000);
		PlaySoundObject(1, SOUND_MODE_PLAY);
	}
}

void PutStageSelectObject(void)
{
	static unsigned int flash;

	int i;
	Rect rcStage;

	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	Rect rcMiracle = {228, 0, 252, 16};

	Rect rcCur[2] = {
		{80, 88, 112, 104},
		{80, 104, 112, 120},
	};

	Rect rcTitle1 = {80, 64, 144, 72};

	int stage_num;
	int stage_x;

	if (gStageSelectTitleY > (WINDOW_HEIGHT / 2) - 74)
		--gStageSelectTitleY;
	
	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(WINDOW_WIDTH / 2 - 15), PixelToScreenCoord(gStageSelectTitleY - 17), &rcMiracle, SURFACE_ID_ITEM_IMAGE);
	PutNumber4(WINDOW_WIDTH / 2 - 17, gStageSelectTitleY - 12, gMC.miracle, false);

	PutBitmap3(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 32), PixelToScreenCoord(gStageSelectTitleY), &rcTitle1, SURFACE_ID_TEXT_BOX);

	stage_num = gPermitStage.size();

	++flash;

	if (stage_num != 0)
	{
		stage_x = WINDOW_WIDTH / 2 - ((std::min(stage_num, STAGE_PER_LINE)) * 40) / 2;

		PutBitmap3(&rcView, PixelToScreenCoord(stage_x + ((gSelectedStage % STAGE_PER_LINE) * 40)), 
			PixelToScreenCoord((WINDOW_HEIGHT / 2) - 56 + 18 * (gSelectedStage / STAGE_PER_LINE)), &rcCur[flash / 2 % 2], SURFACE_ID_TEXT_BOX);

		for (i = 0; i < gPermitStage.size(); ++i)
		{
			if (gPermitStage[i].index == 0)
				break;

			rcStage.left = (gPermitStage[i].index % IMAGES_PER_LINE) * 32;
			rcStage.right = rcStage.left + 32;
			rcStage.top = (gPermitStage[i].index / IMAGES_PER_LINE) * 16;
			rcStage.bottom = rcStage.top + 16;

			PutBitmap3(&rcView, PixelToScreenCoord(stage_x + ((i % STAGE_PER_LINE) * 40)), 
				PixelToScreenCoord((WINDOW_HEIGHT / 2) - 56 + (i / STAGE_PER_LINE)*18), &rcStage, SURFACE_ID_STAGE_ITEM);
		}
	}
}

int StageSelectLoop(int *p_event)
{
	std::string old_script_path;

	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	gSelectedStage = 0;
	const int initialSelectionIndex = gPermitStage.empty() ? 0 : gPermitStage[gSelectedStage].index;

	BackupSurface(SURFACE_ID_SCREEN_GRAB, &grcFull);
	old_script_path = GetTextScriptPath();
	LoadTextScript2("StageSelect.tsc");
	gStageSelectTitleY = (WINDOW_HEIGHT / 2) - 66;
	StartTextScript(initialSelectionIndex + 1000);
	g_GameFlags |= GAME_FLAG_ARE_WE_IN_DIFFERENT_WINDOW;

	for (;;)
	{
		GetTrg();

		if (gKey & KEY_PAUSE)
		{
			switch (Call_Pause())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_ranks:
					return enum_ESCRETURN_ranks;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		if (gKey & KEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_ranks:
					return enum_ESCRETURN_ranks;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		if (gKeyTrg & gKeyTurboText){
			PlaySoundObject(80);
			turboTextEnabled = !turboTextEnabled;
		}

		MoveStageSelectCursor();

		switch (TextScriptProc())
		{
			case enum_ESCRETURN_exit:
				return enum_ESCRETURN_exit;

			case enum_ESCRETURN_ranks:
				return enum_ESCRETURN_ranks;

			case enum_ESCRETURN_restart:
				return enum_ESCRETURN_restart;
		}

#ifdef FIX_BUGS
		PutBitmap4(&rcView, 0, 0, &rcView, SURFACE_ID_SCREEN_GRAB);
#else
		// The original accidentally drew the screencap with transparency enabled
		PutBitmap3(&rcView, 0, 0, &rcView, SURFACE_ID_SCREEN_GRAB);
#endif
		PutStageSelectObject();
		PutTextScript();

		if (gKeyTrg & gKeyOk)
		{
			StopTextScript();
			break;
		}
		else if (gKeyTrg & gKeyCancel)
		{
			StopTextScript();
			LoadTextScript_Stage(old_script_path.c_str());
			*p_event = 0;
			g_GameFlags &= ~GAME_FLAG_ARE_WE_IN_DIFFERENT_WINDOW;
			return enum_ESCRETURN_continue;
		}

		PutFramePerSecound();

		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;
	}
	gTS.item = 0;

	LoadTextScript_Stage(old_script_path.c_str());
	*p_event = gPermitStage.empty() ? 0 : gPermitStage[gSelectedStage].event;
	g_GameFlags &= ~GAME_FLAG_ARE_WE_IN_DIFFERENT_WINDOW;
	return enum_ESCRETURN_continue;
}
