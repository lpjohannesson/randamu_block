#include "MiniMap.h"

#include <map>

#include <cstring>

#include "Back.h"
#include "Draw.h"
#include "Escape.h"
#include "Generic.h"
#include "KeyControl.h"
#include "Game.h"
#include "Main.h"
#include "Map.h"
#include "MapName.h"
#include "MyChar.h"
#include "MycParam.h"
#include "NpChar.h"
#include "Pause.h"
#include "Stage.h"

std::map<int, Rect> mapPixels = {
	{15, {240, 27, 242, 29}},
	{86, {242, 29, 244, 31}},
	{32, {240, 31, 242, 33}},
	{17, {242, 31, 244, 33}},
	{16, {240, 33, 242, 35}},
	{20, {240, 37, 242, 39}},
	{253, {240, 35, 242, 37}},
	{268, {242, 33, 244, 35}},
	{276, {242, 33, 244, 35}},
	{317, {242, 33, 244, 35}},
	{36, {242, 33, 244, 35}},
	{284, {242, 33, 244, 35}},
	{44, {242, 33, 244, 35}}
};

void WriteMiniMapLine(int line)
{
	int x;
	unsigned char a; short e;

	Rect rcLevel[] = {
		{240, 25, 242, 27}, //empty
		{242, 25, 244, 27}, //background
		{240, 27, 242, 29}, //chests
		{242, 27, 244, 29}, //solid block
		{242, 29, 244, 31}, //star
		{240, 31, 242, 33}, //life capsule
		{242, 31, 244, 33}, //refill
		{240, 33, 242, 35}, //spike
		{242, 33, 244, 35}, //door or boss
		{240, 35, 242, 37}, //miracle
		{242, 35, 244, 37}, //pc
		{242, 35, 244, 37} //
	};

	for (x = 0; x < gMap.width; ++x)
	{
		a = GetAttribute(x, line);
		e = GetNpCharHere(x, line);

		// Yup, this really is an if/else chain.
		// No switch here.
		if (a == 0){
			if (gWaterY / 0x2000 < line)
				Surface2Surface(x*2, line*2, &rcLevel[1], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
			else {
				if (e == 100)
					Surface2Surface(x*2, line*2, &rcLevel[11], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
				else
					Surface2Surface(x*2, line*2, &rcLevel[0], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
			}
		}
		else if (a == 68 ||
			a == 1 ||
			a == 64 ||
			a == 128 ||
			a == 129 ||
			a == 130 ||
			a == 131 ||
			a == 81 ||
			a == 82 ||
			a == 85 ||
			a == 86 ||
			a == 2 ||
			a == 96 ||
			a == 113 ||
			a == 114 ||
			a == 117 ||
			a == 118 ||
			a == 160 ||
			a == 161 ||
			a == 162 ||
			a == 163)
			Surface2Surface(x*2, line*2, &rcLevel[1], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
		else if (a == 67 ||
			a == 99 ||
			a == 80 ||
			a == 83 ||
			a == 84 ||
			a == 87 ||
			a == 96 ||	// This is already listed above, so that part of the expression is always false
			a == 112 ||
			a == 115 ||
			a == 116 ||
			a == 119)
			Surface2Surface(x*2, line*2, &rcLevel[11], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
		else {
			Surface2Surface(x*2, line*2, &rcLevel[3], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
		}
	}

	Rect rcDoor = {242, 33, 244, 35};

	for(int i = 0; i < NPC_MAX; i++){
        if (gNPC[i].cond & 0x80 && gNPC[i].y / 0x2000 == line){
			int map_x = gNPC[i].x / 0x2000;
			if(gNPC[i].code_char == 18 || gNPC[i].code_char == 22){
				Surface2Surface(map_x*2, (line-1)*2, &rcDoor, SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
				Surface2Surface(map_x*2, line*2, &rcDoor, SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
			}
			if (mapPixels.find(gNPC[i].code_char) != mapPixels.end()){
				if (gNPC[i].code_char == 17 && gNPC[i].count1 == 1)
					continue;
				Surface2Surface(map_x*2, line*2, &mapPixels[gNPC[i].code_char], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
			}
		}
	}
}

int MiniMapLoop(void)
{
	int f, line;
	Rect rcView;
	Rect rcMiniMap;

	int my_x;
	int my_y;
	unsigned char my_wait;
	Rect my_rect = {240, 29, 242, 31};

	my_x = ((gMC.x / 0x200) + 4) / 8;
	my_y = ((gMC.y / 0x200) + 4) / 8;

	for (f = 0; f <= 8; ++f)
	{
		GetTrg();

		if (gKey & KEY_PAUSE)
		{
			switch (Call_Pause())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

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

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);

		rcView.left = (WINDOW_WIDTH / 3.5) - (((gMap.width * f) / 8) / 4);
		rcView.right = (WINDOW_WIDTH / 3.5) + (((gMap.width * f)/4));
		rcView.top = (WINDOW_HEIGHT / 7.35) - (((gMap.length * f) / 8) / 4);
		rcView.bottom = (WINDOW_HEIGHT / 7.35) + (((gMap.length * f)/4));

		PutBitmap3(&grcGame, 0, 0, &grcGame, SURFACE_ID_BLACK_OVERLAY);

		PutFramePerSecound();
		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;
	}

	rcMiniMap.left = 0;
	rcMiniMap.right = gMap.width*2;
	rcMiniMap.top = 0;
	rcMiniMap.bottom = gMap.length*2;

	rcView.right = --rcView.left + gMap.width*2 + 2;
	rcView.bottom = --rcView.top + gMap.length*2 + 2;
	CortBox2(&rcMiniMap, 0, SURFACE_ID_MAP);

	line = 0;
	my_wait = 0;
	while (1)
	{
		GetTrg();

		if (gKeyTrg & (gKeyOk | gKeyCancel))
			break;

		if (gKey & KEY_PAUSE)
		{
			switch (Call_Pause())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

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

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		if (gKeyTrg & KEY_F2){
			gKeyTrg = 0;
			DeleteSave();
			return 2;
		}

		if (gKeyTrg & KEY_F3){
			ChangeMusic(MUS_MELTDOWN2);
		}

		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);
		PutBitmap3(&grcGame, 0, 0, &grcGame, SURFACE_ID_BLACK_OVERLAY);

		if (line < gMap.length)
		{
			WriteMiniMapLine(line);
			++line;
		}
		if (line < gMap.length)
		{
			WriteMiniMapLine(line);
			++line;
		}
		if (line < gMap.length)
		{
			WriteMiniMapLine(line);
			++line;
		}

		PutBitmap3(&grcGame, PixelToScreenCoord(rcView.left + 1), PixelToScreenCoord(rcView.top + 1), &rcMiniMap, SURFACE_ID_MAP);

		if (++my_wait / 8 % 2)
			PutBitmap3(&grcGame, PixelToScreenCoord(my_x + rcView.left + 1), PixelToScreenCoord(my_y + rcView.top + 1), &my_rect, SURFACE_ID_TEXT_BOX);

		++gCounter;
		PutMyScore(-12);

		PutFramePerSecound();
		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;
	}

	for (f = 8; f >= -1; --f)
	{
		GetTrg();

		if (gKey & KEY_PAUSE)
		{
			switch (Call_Pause())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

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

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);

		rcView.left = (WINDOW_WIDTH / 4) - (((gMap.width * f) / 8) / 4);
		rcView.right = (WINDOW_WIDTH / 4) + (((gMap.width * f) / 8) / 4);
		rcView.top = (WINDOW_HEIGHT / 7) - (((gMap.length * f) / 8) / 4);
		rcView.bottom = (WINDOW_HEIGHT / 7) + (((gMap.length * f) / 8) / 4);

		PutBitmap3(&grcGame, 0, 0, &grcGame, SURFACE_ID_BLACK_OVERLAY);

		PutFramePerSecound();
		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;
	}

	return enum_ESCRETURN_continue;
}

signed char gMapping[0x80];

bool IsMapping(void)
{
	if (!gMapping[gStageNo])
		return false;
	else
		return true;
}

void StartMapping(void)
{
	memset(gMapping, false, sizeof(gMapping));
}

void SetMapping(int a)
{
	gMapping[a] = true;
}
