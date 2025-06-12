#ifndef __MAP_H__
#define __MAP_H__


#include "CommonDefines.h"

struct MAP_DATA
{
	unsigned char *data;
	unsigned char atrb[0x100];
	short width;
	short length;
};

extern MAP_DATA gMap;

bool InitMapData2(void);
bool LoadMapData2(const char *path_map);
bool LoadMapData3(const char *path_map);

void PaintRectangle(int no, int x, int y, int width, int height);
void ReplaceRectangle(int old_no, int new_no, int x, int y, int width, int height);
void PaintRectangle(int no, Rect rect);
void PaintRectangle(int no, Rect rect, int m);
void PaintLine(int no, int x1, int y1, int x2, int y2);
void PaintEllipse(int no, int x, int y, int r);
void ReplaceEllipse(int old_no, int new_no, int x, int y, int r);
bool InsideMap(int x, int y);
bool InsideMap(int cell);
char GetTile(int x, int y);
int Distance(int x1, int y1, int x2, int y2);
void FluidFall(int x, int y, char old_no, char new_no);
void FluidFallS(int x, int y, char old_no, char new_no);
bool IsEmpty(int x, int y);
bool IsSolid(int x, int y);
bool HasSurface(int x, int y);
bool HasCeiling(int x, int y);
bool HasWall(int x, int y);
int YCoord(int cell);
int XCoord(int cell);
bool IsObstructed(int x1, int y1, int x2, int y2);

bool LoadAttributeData(const char *path_atrb);
void EndMapData(void);
void ReleasePartsImage(void);
void GetMapData(unsigned char **data, short *mw, short *ml);
unsigned char GetAttribute(int x, int y);
unsigned char GetAttribute(int cell);
void DeleteMapParts(int x, int y);
void DeleteMapPartsS(int x, int y);
void ShiftMapParts(int x, int y);
bool ChangeMap(int x, int y, unsigned char no);
bool ChangeMapS(int x, int y, unsigned char no);
bool ReplaceMap(int x, int y, unsigned char old_no, unsigned char new_no);
bool ReplaceMapS(int x, int y, unsigned char old_no, unsigned char new_no);
bool EditMapS(int t, unsigned char no);
bool EditMap(int t, unsigned char no);
bool ChangeMapParts(int x, int y, unsigned char no);
void DestroyBlock(int x, int y);
void PutStage_Back(int fx, int fy);
void PutStage_Front(int fx, int fy);
void PutStage_Darkness(int fx, int fy);
void PutMapDataVector(int fx, int fy);
bool IsItDark();

#endif // __MAP_H__