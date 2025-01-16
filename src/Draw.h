#ifndef __DRAW_H__
#define __DRAW_H__


#include <string>

#include "CommonDefines.h"

extern Rect grcGame;
extern Rect grcFull;

extern bool gb60fps;
extern bool gbSmoothScrolling;
extern bool gbVsync;
extern bool gbExtendedInfo;

enum SurfaceID
{
	SURFACE_ID_TITLE = 0,
	SURFACE_ID_PIXEL = 1,
	SURFACE_ID_LEVEL_TILESET = 2,
	SURFACE_ID_DOCTOR = 3,
	SURFACE_ID_MAZE = 4,
	SURFACE_ID_BLLG = 5,
	SURFACE_ID_FADE = 6,
	SURFACE_ID_LEVEL_TILESET_WATER = 7,
	SURFACE_ID_ITEM_IMAGE = 8,
	SURFACE_ID_MAP = 9,
	SURFACE_ID_SCREEN_GRAB = 10,
	SURFACE_ID_ARMS = 11,
	SURFACE_ID_ARMS_IMAGE = 12,
	SURFACE_ID_ROOM_NAME = 13,
	SURFACE_ID_STAGE_ITEM = 14,
	SURFACE_ID_LOADING = 15,
	SURFACE_ID_MY_CHAR = 16,
	SURFACE_ID_BULLET = 17,
	SURFACE_ID_BUTE = 18,
	SURFACE_ID_CARET = 19,
	SURFACE_ID_NPC_SYM = 20,
	SURFACE_ID_LEVEL_SPRITESET_1 = 21,
	SURFACE_ID_LEVEL_SPRITESET_2 = 22,
	SURFACE_ID_NPC_REGU = 23,
	SURFACE_ID_RAVIL = 24,
	SURFACE_ID_RED = 25,
	SURFACE_ID_TEXT_BOX = 26,
	SURFACE_ID_FACE = 27,
	SURFACE_ID_LEVEL_BACKGROUND = 28,
	SURFACE_ID_VALUE_VIEW = 29,
	SURFACE_ID_TEXT_LINE1 = 30,
	SURFACE_ID_TEXT_LINE2 = 31,
	SURFACE_ID_TEXT_LINE3 = 32,
	SURFACE_ID_TEXT_LINE4 = 33,
	SURFACE_ID_TEXT_LINE5 = 34,
	SURFACE_ID_CREDIT_CAST = 35,
	SURFACE_ID_CREDITS_IMAGE = 36,
	SURFACE_ID_CASTS = 37,
	SURFACE_ID_MY_CHAR_2 = 38,
	SURFACE_ID_MY_ACCESSORIES = 39,
	SURFACE_ID_DASH_VISUAL = 40,
	SURFACE_ID_MY_ACCESSORIES_DROWNED = 41,
	SURFACE_ID_MY_ACCESSORIES_DASH = 42,
	SURFACE_ID_WATER_FOREGROUND = 43,
	SURFACE_ID_IMPROVED_MAP_FRAME = 44,
	SURFACE_ID_BLACK_OVERLAY = 45,
	SURFACE_ID_CLOUDS = 46,
	SURFACE_ID_CREDITS_IMAGE_2 = 47,
	SURFACE_ID_RESOURCE_ICON = 48,
	SURFACE_ID_DEATH = 49,
	SURFACE_ID_COOLDOWNS = 50,
	SURFACE_ID_CHARACTER_SELECT = 51,
	SURFACE_ID_MAX = 52,
};

bool Flip_SystemTask(void);
bool StartDirectDraw(const char *title, int lMagnification, bool b60fps, bool bSmoothScrolling, bool bVsync, bool bExtendedInfo);
void EndDirectDraw(void);
void ReleaseSurface(SurfaceID s);
bool MakeSurface_File(const char *name, SurfaceID surf_no, bool loadGlobal = false);
bool ReloadBitmap_File(const char *name, SurfaceID surf_no, bool loadGlobal = false);
bool MakeSurface_Generic(int bxsize, int bysize, SurfaceID surf_no, bool bSystem);
void BackupSurface(SurfaceID surf_no, const Rect *rect);
void PutBitmap3Ex(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no, int angle = 0, float centerX = 0.5f, float centerY = 0.5f, float opacity = 1.0f);
void PutBitmap3(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no);
void PutBitmap3(const Rect *rcView, int x, int y, const Rect *rect, float scale, SurfaceID surf_no);
void PutBitmap3(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no, float opacity);
void PutBitmap3_NoFlip(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no);
void PutBitmap3_Flip(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no);
void PutBitmap3_Flip(const Rect *rcView, int x, int y, const Rect *rect, int color, SurfaceID surf_no);
void PutBitmap3_Flip(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no, float opacity);
void PutBitmap4(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no);
void Surface2Surface(int x, int y, const Rect *rect, int to, int from);
void Surface2Surface(int x, int y, const Rect *rect, int to, int from, float scale);
unsigned long GetCortBoxColor(unsigned long col);
void CortBox(const Rect *rect, unsigned long col);
void CortBox2(const Rect *rect, unsigned long col);
void CortBox2(const Rect *rect, unsigned long col, SurfaceID surf_no);
int RestoreSurfaces(void);
int SubpixelToScreenCoord(int coord);
int SubpixelToScreenCoord(long long coord);
int ScreenToSubpixelCoord(int coord);
int PixelToScreenCoord(int coord);
void InitTextObject();
void PutText(int x, int y, const std::string& text, unsigned long color, bool shift_jis = false);
void PutText(int x, int y, const char *text, unsigned long color, bool shift_jis = false);
void PutText2(int x, int y, const char *text, unsigned long color, SurfaceID surf_no, bool shift_jis = false);
void EndTextObject(void);

#endif // __DRAW_H__