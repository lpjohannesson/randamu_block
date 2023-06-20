#include "Map.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>

#include "ArmsItem.h"
#include "Caret.h"
#include "Draw.h"
#include "File.h"
#include "Game.h"
#include "Log.h"
#include "Main.h"
#include "MyChar.h"
#include "MycParam.h"
#include "NpChar.h"
#include "Random.h"
#include "ResourceCode.h"
#include "Sound.h"
#include "Stage.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define PXM_BUFFER_SIZE 0x4B0000

MAP_DATA gMap;

const char *code_pxma = "PXM";

bool InitMapData2(void)
{
	gMap.data = (unsigned char*)malloc(PXM_BUFFER_SIZE);
	return true;
}

bool LoadMapData2(const char *path_map)
{
	FILE *fp;
	char check[3];
	std::string path;

	// Get path
	path = path_map;

	// Open file
	fp = GetResource(path);
	if (fp == NULL)
		return false;

	// Make sure file begins with "PXM"
	fread(check, 1, 3, fp);

	if (memcmp(check, code_pxma, 3))
	{
		fclose(fp);
		return false;
	}

	unsigned char dum;
	fread(&dum, 1, 1, fp);
	// Get width and height
	gMap.width = File_ReadLE16(fp);
	gMap.length = File_ReadLE16(fp);

	if (gMap.data == NULL)
	{
		fclose(fp);
		return false;
	}

	// Read tile data
	fread(gMap.data, 1, gMap.width * gMap.length, fp);
	fclose(fp);
	return true;
}

//stuff for level gen
void PaintRectangle(int no, int x, int y, int width, int height){
    for (int i = x; i < x + width; i++){
        for (int l = y; l < y + height; l++){
            ChangeMap(i, l, no);
        } 
    }
}

void ReplaceRectangle(int old_no, int new_no, int x, int y, int width, int height){
    for (int i = x; i < x + width; i++){
        for (int l = y; l < y + height; l++){
            ReplaceMap(i, l, old_no, new_no);
        } 
    }
}

void PaintRectangle(int no, Rect rect){
    PaintRectangle(no, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}

void PaintRectangle(int no, Rect rect, int m){
    PaintRectangle(no, rect.left + m, rect.top + m, (rect.right - rect.left) - m*2, (rect.bottom - rect.top) - m*2);
}

void PaintLine(int no, int x1, int y1, int x2, int y2){
    float dx = (float)x2 - (float)x1;
    float dy = (float)y2 - (float)y1;
    bool movingByX = abs(dx) >= abs(dy);
    if (movingByX){
        dy /= abs(dx);
        dx /= abs(dx);
    } else {
        dx /= abs(dy);
        dy /= abs(dy);
    }
	if ((gStageNo == gGeneratedStage) && no != 0 && (GetNpCharHere(dx, dy) == -1))
    	ChangeMap(round(dx), round(dy), no);
    while((movingByX && x2 != x1) || (!movingByX && y2 != y1)){
        x1 += dx;
        y1 += dy;
		if ((gStageNo == gGeneratedStage) && no != 0 && (GetNpCharHere(x1, y1) == -1))
       		ChangeMap(round(x1), round(y1), no);
    }
}

void PaintEllipse(int no, int x, int y, int r){
    for (int xx = -r; xx < r; xx++)
    {
        int height = round(sqrt(r * r - xx * xx));

        for (int yy = -height + 1; yy < height; yy++)
        ChangeMap(xx + x, yy + y, no);
    }
}

void ReplaceEllipse(int old_no, int new_no, int x, int y, int r){
    for (int xx = -r; xx < r ; xx++)
    {
        int height = round(sqrt(r * r - xx * xx));

        for (int yy = -height; yy < height; yy++)
        ReplaceMap(xx + x, yy + y, old_no, new_no);
    }
}

bool InsideMap(int x, int y){
	if (gMC.outerWallInit){
		return x > 1 && y > 1 && x < (gMap.width-2) && y < (gMap.length-2);
	}
    return x > 0 && y > 0 && x < (gMap.width-1) && y < (gMap.length-1);
}

bool InsideMap(int cell){
    return InsideMap(XCoord(cell), YCoord(cell));
}

int Distance(int x1, int y1, int x2, int y2){
    return (float)sqrt(pow(abs( x1 - x2 ), 2) + pow(abs( y1 - y2 ), 2));
}

void FluidFall(int x, int y, char old_no, char new_no)
{
	if (!InsideMap(x, y)) return;
    if(GetTile(x,y) == old_no || (old_no == 0 && GetTile(x, y) == 66))
    {
        ChangeMap(x,y,new_no);
        FluidFall(x+1,y,old_no, new_no);
        FluidFall(x-1,y,old_no, new_no);
        FluidFall(x,y+1,old_no, new_no);
    }
}

void FluidFallS(int x, int y, char old_no, char new_no)
{
	if (!InsideMap(x, y)) return;
    if(GetTile(x,y) == old_no || (GetAttribute(x + y * gMap.width) == 0) || (old_no == 0 && GetTile(x, y) == 66))
    {
        ChangeMapS(x,y,new_no);
        FluidFallS(x+1,y,old_no, new_no);
        FluidFallS(x-1,y,old_no, new_no);
        FluidFallS(x,y+1,old_no, new_no);
    }
}

char GetTile(int x, int y){
    if (InsideMap(x, y)){
        return gMap.data[x + y * gMap.width];
    }
	if (gMC.outerWallInit) return 0;
    return 1;
}

bool IsEmpty(int x, int y){
	if (!InsideMap(x, y)) return false;
    return GetTile(x, y) == 0 || GetAttribute(x + y * gMap.width) == 0;
}

bool HasSurface(int x, int y){
	if (!InsideMap(x, y)) return false;
    return IsEmpty(x, y) == true && (GetTile(x, y+1) == 1 || GetAttribute(x + (y+1) * gMap.width) == 65);
}

bool HasCeiling(int x, int y){
	if (!InsideMap(x, y)) return false;
    return IsEmpty(x, y) == true && (GetTile(x, y-1) == 1 || GetAttribute(x + (y-1) * gMap.width) == 65);
}

bool HasWall(int x, int y){
	if (!InsideMap(x, y)) return true;
    return IsEmpty(x, y) == true && (GetTile(x-1, y) == 1 || GetTile(x+1, y) == 1);
}

int XCoord(int cell){
	return cell % gMap.width;
}

int YCoord(int cell){
	return cell / gMap.width;
}

bool LoadAttributeData(const char *path_atrb)
{
	FILE *fp;
	std::string path;

	// Open file
	path = path_atrb;

	fp = GetResource(path);
	if (fp == NULL)
		return false;

	// Read data
	fread(gMap.atrb, 1, sizeof(gMap.atrb), fp);
	fclose(fp);
	return true;
}

void EndMapData(void)
{
	free(gMap.data);
}

void ReleasePartsImage(void)
{
	ReleaseSurface(SURFACE_ID_LEVEL_TILESET);
	ReleaseSurface(SURFACE_ID_LEVEL_TILESET_WATER);
}

void GetMapData(unsigned char **data, short *mw, short *ml)
{
	if (data != NULL)
		*data = gMap.data;

	if (mw != NULL)
		*mw = gMap.width;

	if (ml != NULL)
		*ml = gMap.length;
}

unsigned char GetAttribute(int x, int y)
{
	size_t a;

	if (x < 0 || y < 0 || x >= gMap.width || y >= gMap.length){
		if (gMC.outerWallInit) return 0;
		return 65;
	}

	a = *(gMap.data + x + (y * gMap.width));	// Yes, the original code really does do this instead of a regular array access
	return gMap.atrb[a];
}

unsigned char GetAttribute(int cell)
{
	if (cell < 0 || cell >= gMap.length*gMap.width){
		if (gMC.outerWallInit) return 0;
		return 65;
	}
        
	return gMap.atrb[gMap.data[cell]];
}

void DeleteMapParts(int x, int y)
{
	*(gMap.data + x + (y * gMap.width)) = 0;
}

void DeleteMapPartsS(int x, int y)
{
	*(gMap.data + x + (y * gMap.width)) = 0;
	mapModifications.insert_or_assign({x, y}, 0);
	// *(gMC.mapMods + x + (y * gMap.width)) = 0;
}

void ShiftMapParts(int x, int y)
{
	*(gMap.data + x + (y * gMap.width)) -= 1;
}

bool ChangeMap(int x, int y, unsigned char no)
{
    if (x < gMap.width-1 && y < gMap.length-1 && x > 1 && y > 1)
        gMap.data[x + (y * gMap.width)] = no;

	return true;
}

bool EditMap(int t, unsigned char no)
{
    gMap.data[t] = no;

	return true;
}

bool EditMapS(int t, unsigned char no)
{
    gMap.data[t] = no;
	mapModifications.insert_or_assign({XCoord(t), YCoord(t)}, no);
	// gMC.mapMods[t] = no;

	return true;
}

bool ChangeMapS(int x, int y, unsigned char no)
{
    if (x < gMap.width-1 && y < gMap.length-1 && x > 1 && y > 1){
        gMap.data[x + (y * gMap.width)] = no;
		// gMC.mapMods[x + (y * gMap.width)] = no;
		mapModifications.insert_or_assign({x, y}, no);
	}

	return true;
}

bool ReplaceMap(int x, int y, unsigned char old_no, unsigned char new_no)
{
    if (x < gMap.width-1 && y < gMap.length-1 && x > 1 && y > 1){
		if (gMap.data[x + (y * gMap.width)] == old_no)
			gMap.data[x + (y * gMap.width)] = new_no;
	}

	return true;
}

bool ReplaceMapS(int x, int y, unsigned char old_no, unsigned char new_no)
{
    if (x < gMap.width-1 && y < gMap.length-1 && x > 0 && y > 0){
		if (gMap.data[x + (y * gMap.width)] == old_no){
			gMap.data[x + (y * gMap.width)] = new_no;
			// gMC.mapMods[x + (y * gMap.width)] = new_no;
			mapModifications.insert_or_assign({x, y}, new_no);
		}
	}

	return true;
}

bool ChangeMapParts(int x, int y, unsigned char no)
{
	int i;

	if (*(gMap.data + x + (y * gMap.width)) == no)
		return false;

	*(gMap.data + x + (y * gMap.width)) = no;

	for (i = 0; i < 3; ++i)
		SetNpChar(4, x * 0x200 * 0x10, y * 0x200 * 0x10, 0, 0, 0, NULL, 0);

	return true;
}

void DestroyBlock(int x, int y)
{
	if (!InsideMap(x, y)) return;
		PlaySoundObject(12, SOUND_MODE_PLAY);

		for (int i = 0; i < 4; ++i)
			SetNpChar(4, x * 0x200 * 0x10, y * 0x200 * 0x10, Random(-0x200, 0x200), Random(-0x200, 0x200), 0, NULL, 0x100);
	DeleteMapPartsS(x, y);
	int a = 0;
	while (a < ARMS_MAX && gArmsData[a].code != 2)
		++a;

	if (a != ARMS_MAX)
	{
		gArmsData[a].num += 1;
		if (gArmsData[a].num > gArmsData[a].max_num)
			gArmsData[a].num = gArmsData[a].max_num;
	}
	if (gMC.gimmick == GIMMICK_FRAGILE){
		if (InsideMap(x, y - 1) && GetAttribute(x, y - 1) == 65 && GetAttribute(x, y) == 0){
			int tile = GetTile(x, y - 1);
			ChangeMap(x, y-1, 0);
			int y1 = y - 1;
			while (GetAttribute(x, y1+1) == 0){
				y1++;
			}
			ChangeMapS(x, y1, tile);
			if (gMC.x > x * 0x2000 && gMC.y > y * 0x2000 && gMC.y < (y1 + 1)*0x2000 && gMC.x < (x+1)*0x2000){
				DamageMyChar(5);
				if (gMC.life <= 0) gMC.deathReason = DeathReason::FALLING_BLOCK;
			}
		}
	}
	for (int t = 0; t < gMap.width * gMap.length; t++)
	{
		if (gMap.data[t] == 66)
		{
			int y = YCoord(t);
			int x = XCoord(t);
			ReplaceMapS(x, y, 66, 0);
			while (InsideMap(x, y) && GetAttribute(x, y + 1) == 0)
			{
				y += 1;
				for (int i = 0; i < 3; ++i)
					SetNpChar(4, x * 0x200 * 0x10, y * 0x200 * 0x10, 0, 0, 0, NULL, 0);
				// ReplaceMap(x, y, 0, 66);
			}
			if (InsideMap(x, y))
				ChangeMapS(x, y, 66);
		}
		if (gMap.data[t] == 72)
		{
			int y = YCoord(t);
			int x = XCoord(t);
			ReplaceMapS(x, y, 72, 0);
			while (InsideMap(x, y) && GetAttribute(x, y + 1) == 0)
			{
				y += 1;
				// ReplaceMap(x, y, 0, 66);
			}
			if (InsideMap(x, y))
				ChangeMapS(x, y, 72);
		}
		if (gMap.data[t] == 73)
		{
			int y = YCoord(t);
			int x = XCoord(t);
			ReplaceMapS(x, y, 73, 0);
			while (InsideMap(x, y) && GetAttribute(x, y + 1) == 0)
			{
				y += 1;
				// ReplaceMap(x, y, 0, 66);
			}
			if (InsideMap(x, y))
				ChangeMapS(x, y, 73);
		}
		if (gMap.data[t] == 74)
		{
			int y = YCoord(t);
			int x = XCoord(t);
			ReplaceMapS(x, y, 74, 0);
			while (InsideMap(x, y) && GetAttribute(x, y + 1) == 0)
			{
				y += 1;
				// ReplaceMap(x, y, 0, 66);
			}
			if (InsideMap(x, y))
				ChangeMapS(x, y, 74);
		}
		if (gMap.data[t] == 57)
		{
			int y = YCoord(t);
			int x = XCoord(t);
			if (GetAttribute(x, y - 1) != 65)
			{
				ReplaceMapS(x, y, 57, 0);
			}
		}
		if (gMap.data[t] == 43)
		{
			int y = YCoord(t);
			int x = XCoord(t);
			if (GetAttribute(x, y - 1) != 65)
			{
				ChangeMapS(x, y, 0);
				while (GetTile(x, y + 1) == 44 || GetTile(x, y + 1) == 45)
				{
					ChangeMapS(x, y + 1, 0);
					y += 1;
				}
			}
		}
		if (gMap.data[t] == 45)
		{
			int y = YCoord(t);
			int x = XCoord(t);
			if (GetAttribute(x, y - 1) != 65)
			{
				ChangeMapS(x, y, 44);
				while (GetAttribute(x, y + 1) == 0)
				{
					ChangeMapS(x, y + 1, 44);
					y += 1;
				}
				ChangeMapS(x, y, 45);
			}
		}
	}
	int width = gMap.width;
	int NEIGHBOURS8[] = {-width - 1, -width, -width + 1, -1, +1, +width - 1, +width, +width + 1};
	bool surrounded;
	int N3[][2] = {{0, -1}, {1, 0}, {-1, 0}};
	int x1, y1;
	surrounded = false;
	for (int i = 0; i < 3; i++)
	{
		x1 = x + N3[i][0];
		y1 = y + N3[i][1];
		if (InsideMap(x1, y1))
		{
			if (GetAttribute(x1, y1) == 96)
			{
				surrounded = true;
			}
		}
	}
	if (surrounded)
	{
		FluidFallS(x, y, 0, 30);
		FluidFallS(x, y, 39, 30);
		for (int t = 0; t < gMap.length * gMap.width; t++)
		{
			if (GetTile(XCoord(t), YCoord(t) - 1) == 0 &&
				(GetTile(XCoord(t), YCoord(t)) == 30 || GetTile(XCoord(t), YCoord(t)) == 13))
			{
				int x = XCoord(t);
				int y = YCoord(t);
				while (GetAttribute(x, y) != 96)
				{
					x -= 1;
				}
				while (GetTile(x, y) == 30 && IsEmpty(x, y - 1))
				{
					ChangeMapS(x, y, 13);
					x += 1;
				}
			}
		}
	}
	for (int t = 0; t < gMap.width * gMap.length; t++)
	{
		if (GetTile(XCoord(t), YCoord(t) - 1) == 30 && GetTile(XCoord(t), YCoord(t)) == 13)
		{
			EditMapS(t, 30);
		}
	}
	for (int t = 0; t < gMap.width * gMap.length; t++)
	{
		if (IsEmpty(XCoord(t), YCoord(t) - 1) && GetTile(XCoord(t), YCoord(t)) == 30)
		{
			EditMapS(t, 13);
		}
	}
}

bool IsItDark(){
	return ChallengeEnabled("darkness") || gMC.gimmick == GIMMICK_DARKNESS || gMC.tileset == TYPE_DARKCAVE;
}

void PutStage_Darkness(int fx, int fy)
{
	int i, j;
	Rect rect;
	int offset;

	// Get range to draw
	int num_x = MIN(gMap.width*2, ((WINDOW_WIDTH + (16 - 1))/8));
	int num_y = MIN(gMap.length*2, ((WINDOW_HEIGHT + (16 - 1))/8));
	int put_x = MAX(0, ((fx / 0x200)/8));
	int put_y = MAX(0, ((fy / 0x200)/8));

	int atrb;

	for (j = put_y; j < put_y + num_y; ++j)
	{
		for (i = put_x; i < put_x + num_x; ++i)
		{
			rect = {0, 0, 8, 8};
			int dist = Distance(gMC.x / 0x1000, gMC.y / 0x1000, i, j);
			if (gMC.tileset == TYPE_DARKCAVE && (gMC.gimmick == GIMMICK_DARKNESS || ChallengeEnabled("darkness") || gMC.depth > genContext.doctorStage)) dist *= 1.5;
			if (dist > 13)
				PutBitmap3(&grcGame, PixelToScreenCoord((i*8)) - SubpixelToScreenCoord(fx), PixelToScreenCoord((j*8)) - SubpixelToScreenCoord(fy), &rect, SURFACE_ID_MY_CHAR_9);
			if (dist > 21)
				PutBitmap3(&grcGame, PixelToScreenCoord((i*8)) - SubpixelToScreenCoord(fx), PixelToScreenCoord((j*8)) - SubpixelToScreenCoord(fy), &rect, SURFACE_ID_MY_CHAR_9);
			if (dist > 27)
				PutBitmap3(&grcGame, PixelToScreenCoord((i*8)) - SubpixelToScreenCoord(fx), PixelToScreenCoord((j*8)) - SubpixelToScreenCoord(fy), &rect, SURFACE_ID_MY_CHAR_9);
		}
	}
}

void PutStage_Back(int fx, int fy)
{
	int i, j;
	Rect rect;
	int offset;

	// Get range to draw
	int num_x = MIN(gMap.width, ((WINDOW_WIDTH + (16 - 1)) / 16) + 1);
	int num_y = MIN(gMap.length, ((WINDOW_HEIGHT + (16 - 1)) / 16) + 1);
	int put_x = MAX(0, ((fx / 0x200) + 8) / 16);
	int put_y = MAX(0, ((fy / 0x200) + 8) / 16);

	int atrb;

	for (j = put_y; j < put_y + num_y; ++j)
	{
		for (i = put_x; i < put_x + num_x; ++i)
		{
			// Get attribute
			offset = (j * gMap.width) + i;
			atrb = GetAttribute(i, j);

			if (atrb >= 0x20)
				continue;

			// Draw tile
			rect.left = (gMap.data[offset] % 16) * 16;
			rect.top = (gMap.data[offset] / 16) * 16;
			rect.right = rect.left + 16;
			rect.bottom = rect.top + 16;

			PutBitmap3(&grcGame, PixelToScreenCoord((i * 16) - 8) - SubpixelToScreenCoord(fx), PixelToScreenCoord((j * 16) - 8) - SubpixelToScreenCoord(fy), &rect, SURFACE_ID_LEVEL_TILESET);
		}
	}
}

void PutStage_Front(int fx, int fy)
{
	Rect rcSnack = {256, 48, 272, 64};
	int i, j;
	Rect rect;
	int offset;

	// Get range to draw
	int num_x = MIN(gMap.width, ((WINDOW_WIDTH + (16 - 1)) / 16) + 1);
	int num_y = MIN(gMap.length, ((WINDOW_HEIGHT + (16 - 1)) / 16) + 1);
	int put_x = MAX(0, ((fx / 0x200) + 8) / 16);
	int put_y = MAX(0, ((fy / 0x200) + 8) / 16);

	int atrb;

	for (j = put_y; j < put_y + num_y; ++j)
	{
		for (i = put_x; i < put_x + num_x; ++i)
		{
			// Get attribute
			offset = (j * gMap.width) + i;
			atrb = GetAttribute(i, j);

			if (atrb < 0x40 || atrb >= 0x80)
				continue;

			// Draw tile
			int tileX = (gMap.data[offset] % 16);
			int tileY = (gMap.data[offset] / 16);
			rect.left = tileX * 16;
			rect.top = tileY * 16;
			rect.right = rect.left + 16;
			rect.bottom = rect.top + 16;

			PutBitmap3(&grcGame, PixelToScreenCoord((i * 16) - 8) - SubpixelToScreenCoord(fx), PixelToScreenCoord((j * 16) - 8) - SubpixelToScreenCoord(fy), &rect, SURFACE_ID_LEVEL_TILESET);

			if (atrb == 0x43)
				PutBitmap3(&grcGame, PixelToScreenCoord((i * 16) - 8) - SubpixelToScreenCoord(fx), PixelToScreenCoord((j * 16) - 8) - SubpixelToScreenCoord(fy), &rcSnack, SURFACE_ID_NPC_SYM);
		}
	}
}

void PutMapDataVector(int fx, int fy)
{
	int i, j;
	Rect rect;
	int offset;

	int num_x;
	int num_y;
	int put_x;
	int put_y;

	static unsigned char count = 0;

	int atrb;

	// Animate the wind
	count += 2;

	// Get range to draw
	num_x = MIN(gMap.width, ((WINDOW_WIDTH + (16 - 1)) / 16) + 1);
	num_y = MIN(gMap.length, ((WINDOW_HEIGHT + (16 - 1)) / 16) + 1);
	put_x = MAX(0, ((fx / 0x200) + 8) / 16);
	put_y = MAX(0, ((fy / 0x200) + 8) / 16);

	for (j = put_y; j < put_y + num_y; ++j)
	{
		for (i = put_x; i < put_x + num_x; ++i)
		{
			// Get attribute
			offset = (j * gMap.width) + i;
			atrb = GetAttribute(i, j);

			if (atrb != 0x80
				&& atrb != 0x81
				&& atrb != 0x82
				&& atrb != 0x83
				&& atrb != 0xA0
				&& atrb != 0xA1
				&& atrb != 0xA2
				&& atrb != 0xA3)
				continue;

			switch (atrb)
			{
				case 128:
				case 160:
					rect.left = 224 + (count % 16);
					rect.right = rect.left + 16;
					rect.top = 48;
					rect.bottom = rect.top + 16;
					break;

				case 129:
				case 161:
					rect.left = 224;
					rect.right = rect.left + 16;
					rect.top = 48 + (count % 16);
					rect.bottom = rect.top + 16;
					break;

				case 130:
				case 162:
					rect.left = 240 - (count % 16);
					rect.right = rect.left + 16;
					rect.top = 48;
					rect.bottom = rect.top + 16;
					break;

				case 131:
				case 163:
					rect.left = 224;
					rect.right = rect.left + 16;
					rect.top = 64 - (count % 16);
					rect.bottom = rect.top + 16;
					break;
			}

			PutBitmap3(&grcGame, PixelToScreenCoord((i * 16) - 8) - SubpixelToScreenCoord(fx), PixelToScreenCoord((j * 16) - 8) - SubpixelToScreenCoord(fy), &rect, SURFACE_ID_CARET);
		}
	}
}
