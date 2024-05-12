#ifndef __NPCTBL_H__
#define __NPCTBL_H__


#include "CommonDefines.h"

#include <map>
#include <string>
#include <vector>
#include "NpChar.h"

struct NPC_TBL_Rect
{
	unsigned char front;
	unsigned char top;
	unsigned char back;
	unsigned char bottom;
};

struct NPC_TABLE
{
	unsigned short bits;
	unsigned short life;
	unsigned char surf;
	unsigned char hit_voice;
	unsigned char destroy_voice;
	unsigned char size;
	long exp;
	long damage;
	NPC_TBL_Rect hit;
	NPC_TBL_Rect view;
};

struct NPC_SPAWN_CONFIG
{
	int npcCode;
	int minNpcs;
	int maxNpcs;
	int extraNpcs = 0;
	std::map<char, float> countModifiers;
	int distanceFromPlayer = 10;
	int minDepth = 1;
	int maxDepth = 99;
	int tilesetRequirement = -1;
	int gimmickRequirement = -1;
	bool requiresSurface = true;
	bool requiresEmptySpace = false;
	bool requiresWall = false;
	bool requiresNoEmptySpace = false;
	bool countFromDoctor = false;
	bool wallDirection = false;
	bool requiresCeiling = false;
	bool bothCeilingAndSurface = false;
	bool logicType = true;
	bool destroyTile = false;
	int sideOffset = 0;
	int verticalOffset = 0;
	int direct = 0;
	bool standardOrder = true;
	bool randomDirection = false;
	bool mobModifier = true;
	bool canOverlap = true;
	int onTopOfMob = 0;
};

extern NPC_TABLE *gNpcTable;
extern std::vector<int> bonkableEnemies;
extern std::vector<int> lifeImmuneEnemies;
extern std::vector<int> lifestealEnemies;
extern std::vector<int> waterBullets;
extern std::vector<int> knockImmuneEnemies;
extern std::vector<int> etherealEntities;
extern std::vector<int> aquaticEntities;
extern std::map<std::string, NPC_SPAWN_CONFIG> npcSpawning;

void LoadNPCTags();
bool LoadNpcTable(const char *path);
void ReleaseNpcTable(void);

// NPC Function table
typedef void (*NPCFUNCTION)(NPCHAR*);
extern const NPCFUNCTION gpNpcFuncTbl[];

#endif // __NPCTBL_H__