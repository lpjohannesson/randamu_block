#include "MapName.h"

#include <cstring>

#include "Draw.h"
#include "ResourceCode.h"

MAP_NAME gMapName;

static Rect rc = { 0, 0, 160, 12 };

void ReadyMapName(const char *str)
{
	int a;

	// Reset map name flags
	gMapName.flag = false;
	gMapName.wait = 0;

	// Copy map's name to the global map name
	strcpy(gMapName.name, str);

	// Draw the text to the surface
	a = (int)strlen(gMapName.name);
	CortBox2(&rc, 0, SURFACE_ID_ROOM_NAME);
	PutText2(((160 - (a * 6)) / 2) + 6, 1, gMapName.name, RGB(0x11, 0x00, 0x22), SURFACE_ID_ROOM_NAME, gJapanese);
	PutText2(((160 - (a * 6)) / 2) + 6, 0, gMapName.name, RGB(0xFF, 0xFF, 0xFE), SURFACE_ID_ROOM_NAME, gJapanese);
}

void PutMapName(bool bMini)
{
	// 'unused_rect' isn't the original name. The Linux port optimised this out, so there's no known name for it.
	Rect unused_rect = {0, 0, 160, 16};
	if (gMapName.flag)
	{
		// MNA
		PutBitmap3(&grcGame, PixelToScreenCoord((WINDOW_WIDTH / 2) - 86), PixelToScreenCoord((WINDOW_HEIGHT / 2) - 40), &rc, SURFACE_ID_ROOM_NAME);
		if (++gMapName.wait > 160)
			gMapName.flag = false;
	}
}

void StartMapName(void)
{
	gMapName.flag = true;
	gMapName.wait = 0;
}

void RestoreMapName(void)
{
	int a = (int)strlen(gMapName.name);

	CortBox2(&rc, 0, SURFACE_ID_ROOM_NAME);
	PutText2(((160 - (a * 6)) / 2) + 6, 1, gMapName.name, RGB(0x11, 0x00, 0x22), SURFACE_ID_ROOM_NAME, gJapanese);
	PutText2(((160 - (a * 6)) / 2) + 6, 0, gMapName.name, RGB(0xFF, 0xFF, 0xFE), SURFACE_ID_ROOM_NAME, gJapanese);
}
