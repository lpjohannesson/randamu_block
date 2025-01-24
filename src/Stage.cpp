#include "Stage.h"

#include <charconv>
#include <string_view>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <array>

#include "Back.h"
#include "Boss.h"
#include "Bullet.h"
#include "Caret.h"
#include "Cooldown.h"
#include "Draw.h"
#include "File.h"
#include "Flash.h"
#include "Flags.h"
#include "Frame.h"
#include "Frame.h"
#include "Json.h"
#include "Log.h"
#include "Game.h"
#include "Generic.h"
#include "GenericLoad.h"
#include "Main.h"
#include "Map.h"
#include "MapName.h"
#include "MiniMap.h"
#include "MyChar.h"
#include "NpChar.h"
#include "NpcTbl.h"
#include "Organya.h"
#include "Profile.h"
#include "Random.h"
#include "ResourceCode.h"
#ifdef EXTRA_SOUND_FORMATS
#include "ExtraSoundFormats.h"
#endif
#include "Score.h"
#include "Star.h"
#include "TextScr.h"
#include "ValueView.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define PHI 0x9e3779b9

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define STAGE_ENTRY(parts, map, bkType, back, npc, boss, boss_no, name_en, name_jp) \
	{                                                                               \
		parts, map, bkType, back, npc, boss, boss_no, name_en, name_jp                       \
	}

int gStageNo;
MusicID gMusicNo;
unsigned int gOldPos;
MusicID gOldNo;

uint32_t Q[4096], c, Q2[4096], c2, Q3[4096], c3;
uint32_t _Q[4096], _c, _Q2[4096], _c2, _Q3[4096], _c3;
uint32_t i1 = 4095;
uint32_t i2 = 4095;
uint32_t i3 = 4095;
uint32_t _i1 = 4095, _i2 = 4095, _i3 = 4095;


unsigned int gNameTimer = 0;
bool gNameFlag = false;
bool gDaily = false;

// Amount of life capsules that can be in a level
int MinCapsules = 2;
int MaxCapsules = 4;
// How much life the player needs to have before life capsules stop spawning
int CapsuleLifeLimit = 60;

// Amount of spikes that can be in a level
int MinSpikes = 50;
int MaxSpikes = 80;

// How much spikes are divided by when the map is mimiga village
int VillageSpikeDivider = 2;

// How much spikes are multiplyed by when the map is hell
int HellSpikeMultiplier = 2;

// Amount of life refill stations that can be in a level
int MinRefills = 2;
int MaxRefills = 3;

// Amount of chests that can be in a level
int MinChests = 2;
int MaxChests = 2;

// Amount of stars that can be in a level (not randomized)
int StarCount = 20;

// How many chains can spawn in a map
int MinChains = 15;
int MaxChains = 40;

// How much chains are multiplied by in the village map
int VillageChainMultiplier = 2;

// How many platforms can spawn in a map
int MinPlatforms = 20;
int MaxPlatforms = 40;

// How much platforms are multiplied by in the village map
int VillagePlatformMultiplier = 2;

// How many background tiles can spawn in a map
int MinBG_Tiles = 20;
int MaxBG_Tiles = 50;

// How much background tiles are multiplied by in the village map
int VillageBG_TileMultiplier = 2;

int doorX, doorY;

const STAGE_TABLE gTMTDefault[95] = {
	STAGE_ENTRY("0", "0", 4, "bk0", "Guest", "0", 0, "Null", "\x96\xB3"),																					  /* 無 */
	STAGE_ENTRY("Pens", "Pens1", 1, "bkBlue", "Guest", "0", 0, "Arthur's House", "\x83\x41\x81\x5B\x83\x54\x81\x5B\x82\xCC\x89\xC6"),						  /* アーサーの家 */
	STAGE_ENTRY("Eggs", "Eggs", 1, "bkGreen", "Eggs1", "Ravil", 0, "Egg Corridor", "\x83\x5E\x83\x7D\x83\x53\x89\xF1\x98\x4C"),								  /* タマゴ回廊 */
	STAGE_ENTRY("EggX", "EggX", 4, "bk0", "Eggs1", "0", 0, "Egg No. 00", "\x83\x5E\x83\x7D\x83\x53\x20\x4E\x6F\x2E\x30\x30"),								  /* タマゴ No.00 */
	STAGE_ENTRY("EggIn", "Egg6", 4, "bk0", "Eggs1", "0", 0, "Egg No. 06", "\x83\x5E\x83\x7D\x83\x53\x20\x4E\x6F\x2E\x30\x36"),								  /* タマゴ No.06 */
	STAGE_ENTRY("Store", "EggR", 4, "bk0", "Eggs1", "0", 0, "Egg Observation Room", "\x83\x5E\x83\x7D\x83\x53\x8A\xC4\x8E\x8B\x8E\xBA"),					  /* タマゴ監視室 */
	STAGE_ENTRY("Weed", "Weed", 1, "bkBlue", "Weed", "0", 0, "Grasstown", "\x83\x4E\x83\x54\x83\x80\x83\x89"),												  /* クサムラ */
	STAGE_ENTRY("Barr", "Santa", 4, "bk0", "Weed", "0", 0, "Santa's House", "\x83\x54\x83\x93\x83\x5E\x82\xCC\x89\xC6"),									  /* サンタの家 */
	STAGE_ENTRY("Barr", "Chako", 1, "bkBlue", "Guest", "0", 0, "Chaco's House", "\x83\x60\x83\x83\x83\x52\x82\xCC\x89\xC6"),								  /* チャコの家 */
	STAGE_ENTRY("Maze", "MazeI", 4, "bk0", "Maze", "0", 0, "Labyrinth I", "\x96\xC0\x8B\x7B\x82\x68"),														  /* 迷宮Ｉ */
	STAGE_ENTRY("Sand", "Sand", 1, "bkGreen", "Sand", "Omg", 1, "Sand Zone", "\x8D\xBB\x8B\xE6"),															  /* 砂区 */
	STAGE_ENTRY("Mimi", "Mimi", 1, "bkBlue", "Guest", "0", 0, "Mimiga Village", "\x83\x7E\x83\x7E\x83\x4B\x81\x5B\x82\xCC\x91\xBA"),						  /* ミミガーの村 */
	STAGE_ENTRY("Cave", "Cave", 4, "bk0", "Cemet", "0", 0, "First Cave", "\x8D\xC5\x8F\x89\x82\xCC\x93\xB4\x8C\x41"),										  /* 最初の洞窟 */
	STAGE_ENTRY("Cave", "Start", 4, "bk0", "Cemet", "0", 0, "Start Point", "\x83\x58\x83\x5E\x81\x5B\x83\x67\x92\x6E\x93\x5F"),								  /* スタート地点 */
	STAGE_ENTRY("Mimi", "Barr", 4, "bk0", "Cemet", "Bllg", 0, "Shack", "\x83\x6F\x83\x89\x83\x62\x83\x4E\x8F\xAC\x89\xAE"),									  /* バラック小屋 */
	STAGE_ENTRY("Mimi", "Pool", 1, "bkBlue", "Guest", "0", 0, "Reservoir", "\x92\x99\x90\x85\x92\x72"),														  /* 貯水池 */
	STAGE_ENTRY("Mimi", "Cemet", 4, "bk0", "Cemet", "0", 0, "Graveyard", "\x82\xCD\x82\xA9\x82\xCE"),														  /* はかば */
	STAGE_ENTRY("Mimi", "Plant", 1, "bkGreen", "Plant", "0", 0, "Yamashita Farm", "\x8E\x52\x89\xBA\x94\x5F\x89\x80"),										  /* 山下農園 */
	STAGE_ENTRY("Store", "Shelt", 4, "bk0", "Eggs1", "0", 0, "Shelter", "\x83\x56\x83\x46\x83\x8B\x83\x5E\x81\x5B"),										  /* シェルター */
	STAGE_ENTRY("Pens", "Comu", 1, "bkBlue", "Guest", "0", 0, "Assembly Hall", "\x8F\x57\x89\xEF\x8F\xEA"),													  /* 集会場 */
	STAGE_ENTRY("Mimi", "MiBox", 4, "bk0", "0", "0", 0, "Save Point", "\x83\x5A\x81\x5B\x83\x75\x83\x7C\x83\x43\x83\x93\x83\x67"),							  /* セーブポイント */
	STAGE_ENTRY("Store", "EgEnd1", 4, "bk0", "0", "0", 0, "Side Room", "\x83\x5E\x83\x7D\x83\x53\x89\xF1\x98\x4C\x82\xCC\x8C\xC2\x8E\xBA"),					  /* タマゴ回廊の個室 */
	STAGE_ENTRY("Store", "Cthu", 4, "bk0", "0", "0", 0, "Cthulhu's Abode", "\x83\x4E\x83\x67\x83\x44\x83\x8B\x81\x5B\x82\xCC\x8F\x5A\x8F\x88"),				  /* クトゥルーの住処 */
	STAGE_ENTRY("EggIn", "Egg1", 4, "bk0", "Eggs1", "0", 0, "Egg No. 01", "\x83\x5E\x83\x7D\x83\x53\x20\x4E\x6F\x2E\x30\x31"),								  /* タマゴ No.01 */
	STAGE_ENTRY("Pens", "Pens2", 1, "bkBlue", "Guest", "0", 0, "Arthur's House", "\x83\x41\x81\x5B\x83\x54\x81\x5B\x82\xCC\x89\xC6"),						  /* アーサーの家 */
	STAGE_ENTRY("Barr", "Malco", 1, "bkBlue", "Weed", "Bllg", 0, "Power Room", "\x93\x64\x8C\xB9\x8E\xBA"),													  /* 電源室 */
	STAGE_ENTRY("Barr", "WeedS", 1, "bkBlue", "0", "0", 0, "Save Point", "\x83\x5A\x81\x5B\x83\x75\x83\x7C\x83\x43\x83\x93\x83\x67"),						  /* セーブポイント */
	STAGE_ENTRY("Store", "WeedD", 1, "bkBlue", "0", "0", 0, "Execution Chamber", "\x8F\x88\x8C\x59\x8E\xBA"),												  /* 処刑室 */
	STAGE_ENTRY("Weed", "Frog", 2, "bkGreen", "Weed", "Frog", 2, "Gum", "\x83\x4B\x83\x80"),																  /* ガム */
	STAGE_ENTRY("Sand", "Curly", 4, "bk0", "Sand", "Curly", 0, "Sand Zone Residence", "\x8D\xBB\x8B\xE6\x92\x93\x8D\xDD\x8F\x8A"),							  /* 砂区駐在所 */
	STAGE_ENTRY("Pens", "WeedB", 1, "bkBlue", "Ravil", "0", 0, "Grasstown Hut", "\x83\x4E\x83\x54\x83\x80\x83\x89\x82\xCC\x8F\xAC\x89\xAE"),				  /* クサムラの小屋 */
	STAGE_ENTRY("River", "Stream", 5, "bkBlue", "Stream", "IronH", 5, "Main Artery", "\x91\xE5\x93\xAE\x96\xAC"),											  /* 大動脈 */
	STAGE_ENTRY("Pens", "CurlyS", 4, "bk0", "Sand", "Curly", 0, "Small Room", "\x8F\xAC\x95\x94\x89\xAE"),													  /* 小部屋 */
	STAGE_ENTRY("Barr", "Jenka1", 4, "bk0", "Sand", "Bllg", 0, "Jenka's House", "\x83\x57\x83\x46\x83\x93\x83\x4A\x82\xCC\x89\xC6"),						  /* ジェンカの家 */
	STAGE_ENTRY("Sand", "Dark", 1, "bkBlack", "Sand", "0", 0, "Deserted House", "\x94\x70\x89\xAE"),														  /* 廃屋 */
	STAGE_ENTRY("Gard", "Gard", 1, "bkGard", "Toro", "Bllg", 0, "Sand Zone Storehouse", "\x8D\xBB\x8B\xE6\x91\x71\x8C\xC9"),								  /* 砂区倉庫 */
	STAGE_ENTRY("Barr", "Jenka2", 4, "bk0", "Sand", "Bllg", 0, "Jenka's House", "\x83\x57\x83\x46\x83\x93\x83\x4A\x82\xCC\x89\xC6"),						  /* ジェンカの家 */
	STAGE_ENTRY("Sand", "SandE", 1, "bkGreen", "Sand", "Bllg", 0, "Sand Zone", "\x8D\xBB\x8B\xE6"),															  /* 砂区 */
	STAGE_ENTRY("Maze", "MazeH", 4, "bk0", "Maze", "0", 0, "Labyrinth H", "\x96\xC0\x8B\x7B\x82\x67"),														  /* 迷宮Ｈ */
	STAGE_ENTRY("Maze", "MazeW", 1, "bkMaze", "Maze", "X", 3, "Labyrinth W", "\x96\xC0\x8B\x7B\x82\x76"),													  /* 迷宮Ｗ */
	STAGE_ENTRY("Maze", "MazeO", 4, "bk0", "Guest", "0", 0, "Camp", "\x83\x4C\x83\x83\x83\x93\x83\x76"),													  /* キャンプ */
	STAGE_ENTRY("Maze", "MazeD", 4, "bk0", "Guest", "Dark", 0, "Clinic Ruins", "\x90\x66\x97\xC3\x8F\x8A\x90\xD5"),											  /* 診療所跡 */
	STAGE_ENTRY("Store", "MazeA", 4, "bk0", "Maze", "0", 0, "Labyrinth Shop", "\x96\xC0\x8B\x7B\x82\xCC\x93\x58"),											  /* 迷宮の店 */
	STAGE_ENTRY("Maze", "MazeB", 1, "bkBlue", "Maze", "0", 0, "Labyrinth B", "\x96\xC0\x8B\x7B\x82\x61"),													  /* 迷宮Ｂ */
	STAGE_ENTRY("Maze", "MazeS", 2, "bkGray", "Maze", "Bllg", 0, "Boulder Chamber", "\x91\xE5\x90\xCE\x82\xCC\x8D\xC7\x82\xAE\x8F\x8A"),					  /* 大石の塞ぐ所 */
	STAGE_ENTRY("Maze", "MazeM", 1, "bkRed", "Maze", "0", 0, "Labyrinth M", "\x96\xC0\x8B\x7B\x82\x6C"),													  /* 迷宮Ｍ */
	STAGE_ENTRY("Cave", "Drain", 3, "bkWater", "Cemet", "0", 0, "Dark Place", "\x88\xC3\x82\xA2\x8F\x8A"),													  /* 暗い所 */
	STAGE_ENTRY("Almond", "Almond", 3, "bkWater", "Cemet", "Almo1", 4, "Core", "\x83\x52\x83\x41"),															  /* コア */
	STAGE_ENTRY("River", "River", 2, "bkGreen", "Weed", "0", 0, "Waterway", "\x90\x85\x98\x48"),															  /* 水路 */
	STAGE_ENTRY("Eggs", "Eggs2", 1, "bkGreen", "Eggs2", "0", 0, "Egg Corridor?", "\x83\x5E\x83\x7D\x83\x53\x89\xF1\x98\x4C\x81\x48"),						  /* タマゴ回廊？ */
	STAGE_ENTRY("Store", "Cthu2", 4, "bk0", "Eggs1", "0", 0, "Cthulhu's Abode?", "\x83\x4E\x83\x67\x83\x44\x83\x8B\x81\x5B\x82\xCC\x8F\x5A\x8F\x88\x81\x48"), /* クトゥルーの住処？ */
	STAGE_ENTRY("Store", "EggR2", 4, "bk0", "Eggs1", "TwinD", 6, "Egg Observation Room?", "\x83\x5E\x83\x7D\x83\x53\x8A\xC4\x8E\x8B\x8E\xBA\x81\x48"),		  /* タマゴ監視室？ */
	STAGE_ENTRY("EggX", "EggX2", 4, "bk0", "Eggs1", "0", 0, "Egg No. 00", "\x83\x5E\x83\x7D\x83\x53\x20\x4E\x6F\x2E\x30\x30"),								  /* タマゴ No.00 */
	STAGE_ENTRY("Oside", "Oside", 6, "bkMoon", "Moon", "0", 0, "Outer Wall", "\x8A\x4F\x95\xC7"),															  /* 外壁 */
	STAGE_ENTRY("Store", "EgEnd2", 4, "bk0", "Eggs1", "0", 0, "Side Room", "\x83\x5E\x83\x7D\x83\x53\x89\xF1\x98\x4C\x82\xCC\x8C\xC2\x8E\xBA"),				  /* タマゴ回廊の個室 */
	STAGE_ENTRY("Store", "Itoh", 2, "bkBlue", "Guest", "0", 0, "Storehouse", "\x91\x71\x8C\xC9"),															  /* 倉庫 */
	STAGE_ENTRY("Cent", "Cent", 1, "bkGreen", "Guest", "Cent", 0, "Plantation", "\x91\xE5\x94\x5F\x89\x80"),												  /* 大農園 */
	STAGE_ENTRY("Jail", "Jail1", 4, "bk0", "Guest", "Cent", 0, "Jail No. 1", "\x91\xE6\x82\x50\x98\x53"),													  /* 第１牢 */
	STAGE_ENTRY("Jail", "Momo", 4, "bk0", "Guest", "0", 0, "Hideout", "\x83\x4A\x83\x4E\x83\x8C\x83\x4B"),													  /* カクレガ */
	STAGE_ENTRY("Jail", "Lounge", 4, "bk0", "Guest", "0", 0, "Rest Area", "\x8B\x78\x8C\x65\x8F\x8A"),														  /* 休憩所 */
	STAGE_ENTRY("Store", "CentW", 4, "bk0", "Guest", "Cent", 0, "Teleporter", "\x93\x5D\x91\x97\x8E\xBA"),													  /* 転送室 */
	STAGE_ENTRY("Store", "Jail2", 4, "bk0", "Guest", "Cent", 0, "Jail No. 2", "\x91\xE6\x82\x51\x98\x53"),													  /* 第２牢 */
	STAGE_ENTRY("White", "Blcny1", 7, "bkFog", "Ravil", "Heri", 0, "Balcony", "\x83\x6F\x83\x8B\x83\x52\x83\x6A\x81\x5B"),									  /* バルコニー */
	STAGE_ENTRY("Jail", "Priso1", 4, "bkGray", "Red", "0", 0, "Final Cave", "\x8D\xC5\x8C\xE3\x82\xCC\x93\xB4\x8C\x41"),									  /* 最後の洞窟 */
	STAGE_ENTRY("White", "Ring1", 7, "bkFog", "Guest", "Miza", 0, "Throne Room", "\x89\xA4\x82\xCC\x8B\xCA\x8D\xC0"),										  /* 王の玉座 */
	STAGE_ENTRY("White", "Ring2", 7, "bkFog", "Guest", "Dr", 0, "The King's Table", "\x89\xA4\x82\xCC\x90\x48\x91\xEC"),									  /* 王の食卓 */
	STAGE_ENTRY("Pens", "Prefa1", 4, "bk0", "0", "0", 0, "Prefab Building", "\x83\x76\x83\x8C\x83\x6E\x83\x75"),											  /* プレハブ */
	STAGE_ENTRY("Jail", "Priso2", 4, "bkGray", "Red", "0", 0, "Last Cave (Hidden)", "\x8D\xC5\x8C\xE3\x82\xCC\x93\xB4\x8C\x41\x81\x45\x97\xA0"),			  /* 最後の洞窟・裏 */
	STAGE_ENTRY("White", "Ring3", 4, "bk0", "Miza", "Almo2", 7, "Black Space", "\x8D\x95\x82\xA2\x8D\x4C\x8A\xD4"),											  /* 黒い広間 */
	STAGE_ENTRY("Pens", "Little", 2, "bkBlue", "Guest", "0", 0, "Little House", "\x83\x8A\x83\x67\x83\x8B\x89\xC6"),										  /* リトル家 */
	STAGE_ENTRY("White", "Blcny2", 7, "bkFog", "Ravil", "Heri", 0, "Balcony", "\x83\x6F\x83\x8B\x83\x52\x83\x6A\x81\x5B"),									  /* バルコニー */
	STAGE_ENTRY("Fall", "Fall", 1, "bkFall", "Guest", "Heri", 0, "Fall", "\x97\x8E\x89\xBA"),																  /* 落下 */
	STAGE_ENTRY("White", "Kings", 4, "bk0", "Kings", "0", 0, "u", "\x75"),																					  /* u */
	STAGE_ENTRY("Pens", "Pixel", 1, "bkBlue", "Guest", "0", 0, "Waterway Cabin", "\x90\x85\x98\x48\x82\xCC\x8F\xAC\x95\x94\x89\xAE"),						  /* 水路の小部屋 */
	STAGE_ENTRY("Maze", "e_Maze", 1, "bkMaze", "Guest", "Maze", 3, "", ""),
	STAGE_ENTRY("Barr", "e_Jenk", 4, "bk0", "Sand", "Bllg", 0, "", ""),
	STAGE_ENTRY("Barr", "e_Malc", 1, "bkBlue", "Weed", "Bllg", 0, "", ""),
	STAGE_ENTRY("Mimi", "e_Ceme", 4, "bk0", "Plant", "0", 0, "", ""),
	STAGE_ENTRY("Fall", "e_Sky", 1, "bkFall", "Guest", "Heri", 0, "", ""),
	STAGE_ENTRY("Pens", "Prefa2", 4, "bk0", "0", "0", 0, "Prefab House", "\x83\x76\x83\x8C\x83\x6E\x83\x75"),								/* プレハブ */
	STAGE_ENTRY("Hell", "Hell1", 2, "bkRed", "Hell", "0", 0, "Sacred Ground - B1", "\x90\xB9\x88\xE6\x92\x6E\x89\xBA\x82\x50\x8A\x4B"),		/* 聖域地下１階 */
	STAGE_ENTRY("Hell", "Hell2", 2, "bkRed", "Hell", "0", 0, "Sacred Ground - B2", "\x90\xB9\x88\xE6\x92\x6E\x89\xBA\x82\x51\x8A\x4B"),		/* 聖域地下２階 */
	STAGE_ENTRY("Hell", "Hell3", 1, "bkRed", "Hell", "Press", 8, "Sacred Ground - B3", "\x90\xB9\x88\xE6\x92\x6E\x89\xBA\x82\x52\x8A\x4B"), /* 聖域地下３階 */
	STAGE_ENTRY("Cave", "Mapi", 2, "bk0", "Cemet", "0", 0, "Storage", "\x95\xA8\x92\x75"),													/* 物置 */
	STAGE_ENTRY("Hell", "Hell4", 4, "bk0", "Hell", "0", 0, "Passage?", "\x92\xCA\x98\x48\x81\x48"),											/* 通路？ */
	STAGE_ENTRY("Hell", "Hell42", 4, "bk0", "Hell", "Press", 8, "Passage?", "\x92\xCA\x98\x48\x81\x48"),									/* 通路？ */
	STAGE_ENTRY("Hell", "Statue", 1, "bkBlue", "0", "Cent", 0, "Statue Chamber", "\x90\xCE\x91\x9C\x82\xCC\x8A\xD4"),						/* 石像の間 */
	STAGE_ENTRY("Hell", "Ballo1", 2, "bkBlue", "Priest", "Ballos", 9, "Seal Chamber", "\x95\x95\x88\xF3\x82\xCC\x8A\xD4"),					/* 封印の間 */
	STAGE_ENTRY("White", "Ostep", 7, "bkFog", "0", "0", 0, "Corridor", "\x82\xED\x82\xBD\x82\xE8\x98\x4C\x89\xBA"),							/* わたり廊下 */
	STAGE_ENTRY("Labo", "e_Labo", 4, "bk0", "Guest", "0", 0, "", ""),
	STAGE_ENTRY("Cave", "Pole", 4, "bk0", "Guest", "0", 0, "Hermit Gunsmith", "\x82\xCD\x82\xAE\x82\xEA\x8F\x65\x92\x62\x96\xE8"), /* はぐれ銃鍛冶 */
	STAGE_ENTRY("0", "Island", 4, "bk0", "Island", "0", 0, "", ""),
	STAGE_ENTRY("Hell", "Ballo2", 2, "bkBlue", "Priest", "Bllg", 9, "Seal Chamber", "\x95\x95\x88\xF3\x82\xCC\x8A\xD4"), /* 封印の間 */
	STAGE_ENTRY("White", "e_Blcn", 7, "bkFog", "Miza", "0", 9, "", ""),
	STAGE_ENTRY("Oside", "Clock", 6, "bkMoon", "Moon", "0", 0, "Clock Room", "\x8E\x9E\x8C\x76\x89\xAE"), /* 時計屋 */
};

const STAGE_TABLE *gTMT = gTMTDefault;

bool LoadStageTable(void)
{
	std::string path;

	unsigned char *file_buffer;
	size_t file_size;

	// Try to load stage.tbl
	path = "stage.tbl";
	file_buffer = LoadFileToMemory(path.c_str(), &file_size);

	if (file_buffer != NULL)
	{
		const unsigned long entry_count = file_size / 0xE5;

		STAGE_TABLE *pTMT = (STAGE_TABLE *)malloc(entry_count * sizeof(STAGE_TABLE));

		if (pTMT != NULL)
		{
			for (unsigned long i = 0; i < entry_count; ++i)
			{
				unsigned char *entry = file_buffer + i * 0xE5;

				memcpy(pTMT[i].parts, entry, 0x20);
				memcpy(pTMT[i].map, entry + 0x20, 0x20);
				pTMT[i].bkType = (entry[0x40 + 3] << 24) | (entry[0x40 + 2] << 16) | (entry[0x40 + 1] << 8) | entry[0x40];
				memcpy(pTMT[i].back, entry + 0x44, 0x20);
				memcpy(pTMT[i].npc, entry + 0x64, 0x20);
				memcpy(pTMT[i].boss, entry + 0x84, 0x20);
				pTMT[i].boss_no = entry[0xA4];
			if (gJapanese)
				memcpy(pTMT[i].name, entry + 0xA5, 0x20);
			else
				memcpy(pTMT[i].name, entry + 0xC5, 0x20);
			}

			gTMT = pTMT;
			free(file_buffer);
			return true;
		}

		free(file_buffer);
	}

	// Try to load mrmap.bin
	path = "mrmap.bin";
	file_buffer = LoadFileToMemory(path.c_str(), &file_size);

	if (file_buffer != NULL)
	{
		const unsigned long entry_count = file_buffer[0] | (file_buffer[1] << 8) | (file_buffer[2] << 16) | (file_buffer[3] << 24);

		STAGE_TABLE *pTMT = (STAGE_TABLE *)malloc(entry_count * sizeof(STAGE_TABLE));

		if (pTMT != NULL)
		{
			for (unsigned long i = 0; i < entry_count; ++i)
			{
				// For compatibility with Booster's Lab, we store our stage table in "MOD_MR" format.
				// This way, BL will load the sprites as PNG files instead of BMP.
				unsigned char *entry = file_buffer + 4 + i * 0x74;

				memcpy(pTMT[i].parts, entry, 0x10);
				memcpy(pTMT[i].map, entry + 0x10, 0x10);
				pTMT[i].bkType = entry[0x20];
				memcpy(pTMT[i].back, entry + 0x21, 0x10);
				memcpy(pTMT[i].npc, entry + 0x31, 0x10);
				memcpy(pTMT[i].boss, entry + 0x41, 0x10);
				pTMT[i].boss_no = entry[0x51];
				memcpy(pTMT[i].name, entry + 0x52, 0x22);
			}

			gTMT = pTMT;
			free(file_buffer);
			return true;
		}

		free(file_buffer);
	}

	printf("Failed to load stage.tbl/mrmap.bin\n");
	return false;
}

void msvc_srand_s(uint32_t x)
{
	int i;
	c = 362436;
	i1 = 4095;
	memset(Q, 0, sizeof(Q));

	Q[0] = x;
	Q[1] = x + PHI;
	Q[2] = x + PHI + PHI;

	for (i = 3; i < 4096; i++)
		Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}

uint32_t msvc_rand_s()
{
	uint64_t t, a = 18782LL;
	uint32_t x, r = 0xfffffffe;
	i1 = (i1 + 1) & 4095;
	t = a * Q[i1] + c;
	c = (t >> 32);
	x = t + c;
	if (x < c)
	{
		x++;
		c++;
	}
	return (Q[i1] = r - x);
}

std::vector<bool> GeneratePatch(int w, int h, float fill, int clustering, bool forceFillRate)
{
	int length = w * h;
	std::vector<bool> cur(length);
	std::vector<bool> off(length);

	int fillDiff = round(length * fill);

	if (forceFillRate && clustering > 0)
	{
		fill += (0.5f - fill) * 0.5f;
	}

	for (int i = 0; i < length; i++)
	{
		off[i] = Random(0, 100, RandomCategories::SEEDED_PRIMARY) < fill * 100;
		if (off[i])
			fillDiff++;
	}

	for (int i = 0; i < clustering; i++)
	{

		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{

				int pos = x + y * w;
				int count = 0;
				int neighbours = 0;

				if (y > 0)
				{
					if (x > 0)
					{
						if (off[pos - w - 1])
							count++;
						neighbours++;
					}
					if (off[pos - w])
						count++;
					neighbours++;
					if (x < (w - 1))
					{
						if (off[pos - w + 1])
							count++;
						neighbours++;
					}
				}

				if (x > 0)
				{
					if (off[pos - 1])
						count++;
					neighbours++;
				}
				if (off[pos])
					count++;
				neighbours++;
				if (x < (w - 1))
				{
					if (off[pos + 1])
						count++;
					neighbours++;
				}

				if (y < (h - 1))
				{
					if (x > 0)
					{
						if (off[pos + w - 1])
							count++;
						neighbours++;
					}
					if (off[pos + w])
						count++;
					neighbours++;
					if (x < (w - 1))
					{
						if (off[pos + w + 1])
							count++;
						neighbours++;
					}
				}

				cur[pos] = 2 * count >= neighbours;
				if (cur[pos] != off[pos])
					fillDiff += cur[pos] ? +1 : -1;
			}
		}

		std::vector<bool> tmp(cur.begin(), cur.end());
		cur = off;
		off = tmp;
	}

	if (forceFillRate && MIN(w, h) > 2) {
			int neighbours[] = {-w - 1, -w, -w + 1, -1, 0, +1, +w - 1, +w, +w + 1};
			bool growing = fillDiff < 0;

			while (fillDiff != 0) {
				int cell;
				int tries = 0;

				//random cell, not in the map's borders
				// try length/10 times to find a cell we can grow from, and not start a new patch/hole
				do {
					cell = Random(1, w - 2, RandomCategories::SEEDED_PRIMARY) + Random(1, h - 2, RandomCategories::SEEDED_PRIMARY) * w;
					tries++;
				} while (off[cell] != growing && tries * 10 < length);

				for (int i : neighbours) {
					if (fillDiff != 0 && off[cell + i] != growing) {
						off[cell + i] = growing;
						fillDiff += growing ? +1 : -1;
					}
				}
			}
	}

	return off;
}

void msvc_srand_s2(uint32_t x)
{
	int i;
	c2 = 362436;
	i2 = 4095;
	memset(Q2, 0, sizeof(Q2));

	Q2[0] = x;
	Q2[1] = x + PHI;
	Q2[2] = x + PHI + PHI;

	for (i = 3; i < 4096; i++)
		Q2[i] = Q2[i - 3] ^ Q2[i - 2] ^ PHI ^ i;
}

int LifeCapsuleLimit()
{
	int limit = 18;
	if (GetNPCFlag(Flags::MAHIN_CHARACTER))
	{
		limit = 26;
	}

	if (gMC.depth > genContext.doctorStage){
		limit *= 1.5;
	}
	if (gMC.difficulty > DIFFICULTY_HARDENED){
		limit *= 1.33;
	}

	return limit;
}

uint32_t msvc_rand_s2()
{
	uint64_t t, a = 18782LL;
	uint32_t x, r = 0xfffffffe;
	i2 = (i2 + 1) & 4095;
	t = a * Q2[i2] + c2;
	c2 = (t >> 32);
	x = t + c2;
	if (x < c2)
	{
		x++;
		c2++;
	}
	return (Q2[i2] = r - x);
}

void msvc_srand_s3(uint32_t x)
{
	int i;
	c3 = 362436;
	i3 = 4095;
	memset(Q3, 0, sizeof(Q3));

	Q3[0] = x;
	Q3[1] = x + PHI;
	Q3[2] = x + PHI + PHI;

	for (i = 3; i < 4096; i++)
		Q3[i] = Q3[i - 3] ^ Q3[i - 2] ^ PHI ^ i;
}

uint32_t msvc_rand_s3()
{
	uint64_t t, a = 18782LL;
	uint32_t x, r = 0xfffffffe;
	i3 = (i3 + 1) & 4095;
	t = a * Q3[i3] + c3;
	c3 = (t >> 32);
	x = t + c3;
	if (x < c3)
	{
		x++;
		c3++;
	}
	return (Q3[i3] = r - x);
}

void SaveRNG(json& profileJson)
{
	if (genContext.legacyRNG){
		json profile = json::object();
		profile["rngInfo"] = json::object();
		profile["rngInfo"]["Q1"] = _Q;
		profile["rngInfo"]["Q2"] = _Q2;
		profile["rngInfo"]["Q3"] = _Q2;
		profile["rngInfo"]["c1"] = _c;
		profile["rngInfo"]["c2"] = _c2;
		profile["rngInfo"]["c3"] = _c3;
		profile["rngInfo"]["i1"] = _i1;
		profile["rngInfo"]["i2"] = _i2;
		profile["rngInfo"]["i3"] = _i3;
		WriteJSON(gModulePath + "/RNG.json", profile);
	} else {
		profileJson["rngInfo"] = json::object();
		profileJson["rngInfo"]["primary"] = seed_primary_clone;
		profileJson["rngInfo"]["chests"] = seed_chests_clone;
		profileJson["rngInfo"]["npcs"] = seed_npcs_clone;
	}
}

void LoadRNG(json& profileJson)
{
	if (IsKeyFile("RNG.json")){
		json profile = ReadJSON(gModulePath + "/RNG.json");
		auto pQ = profile["rngInfo"]["Q1"].get<std::vector<uint32_t>>();
		std::copy(pQ.begin(), pQ.end(), Q);
		auto pQ2 = profile["rngInfo"]["Q2"].get<std::vector<uint32_t>>();
		std::copy(pQ2.begin(), pQ2.end(), Q2);
		auto pQ3 = profile["rngInfo"]["Q3"].get<std::vector<uint32_t>>();
		std::copy(pQ3.begin(), pQ3.end(), Q3);
		c = profile["rngInfo"]["c1"].get<int>();
		c2 = profile["rngInfo"]["c2"].get<int>();
		c3 = profile["rngInfo"]["c3"].get<int>();
		i1 = profile["rngInfo"]["i1"].get<int>();
		i2 = profile["rngInfo"]["i2"].get<int>();
		i3 = profile["rngInfo"]["i3"].get<int>();
	} else {
		seed_primary = profileJson["rngInfo"]["primary"].get<pcg32_random_t>();
		seed_chests = profileJson["rngInfo"]["chests"].get<pcg32_random_t>();
		seed_npcs = profileJson["rngInfo"]["npcs"].get<pcg32_random_t>();
	}
}

static int chestDrops[] = {401, 501, 601, 701, 801, 901, 1001, 1101, 1201, 1301, 1401, 1501, 1601, 1701, 1801, 1901, 2001, 2101, 2201, 2301, 2401, 2501, 2601, 2701, 2801, 2901, 3001, 3101, 3201, 3301, 3401, 3501, 3601, 3701, 3801, 3901, 4001, 4501, 4601, 4701, 4801, 4901, 5001, 5101, 5301, 5401, 5501, 5601, 5701, 5801};

std::vector<int> BlacklistedItems()
{
	std::vector<int> items = {};
	if (GetNPCFlag(Flags::MAHIN_CHARACTER))
	{
		items = {401, 501, 601, 701, 801, 901, 1301, 1501, 1601, 1901, 3101, 3201, 3701, 4601, 4801, 5501, 5601};
	}
	if (ChallengeEnabled("no_breaking") || ChallengeEnabled("no_placing"))
	{
		items.push_back(1901);
	}
	if (ChallengeEnabled("no_breaking"))
	{
		items.push_back(3001);
		items.push_back(5701);
	}
	if (ChallengeEnabled("no_placing"))
	{
		items.push_back(3101);
	}
	return items;
}

bool IntersectCheck(Rect rect, int x, int y){
	return (x > rect.left && x < rect.right && y < rect.bottom && y > rect.top);
}

template<typename T = std::vector<int>>
void Shuffle(T& cont, RandomCategories::RandomCategories rng)
{
	for (int i = 0; i < cont.size() - 1; i++)
	{
		int j = Random(i, cont.size()-1, rng);
		if (j != i)
		{
			auto t = cont[i];
			cont[i] = cont[j];
			cont[j] = t;
		}
	}
}

void LogSeed(unsigned long seed){
	std::string path = gModulePath + "/last_seed.txt";
	FILE *fp;

	fp = fopen(path.c_str(), "a+");
	const time_t _t = time(0);

	if (fp != NULL)
	{
		fprintf(fp, "Last seed used at ");
		int v1, v2, v3, v4;
		GetCompileVersion(&v1, &v2, &v3, &v4);
		fprintf(fp, "%.24s: %lu, version: %d.%d.%d.%d\n", ctime(&_t), seed, v1, v2, v3, v4);
		fclose(fp);
	}
}

void SelectCharacter(std::string character) {
	if (character != "")
	{
		if (character == "God")
		{
			SetNPCFlag(Flags::MAHIN_CHARACTER);
			gMC.star = 3;
			gMIMCurrentNum = 15;
			int life = 15;
			gMC.life = life;
			gMC.max_life = life;
		}
		if (character == "AmeManga")
		{
			SetNPCFlag(Flags::AMY_CHARACTER);
			gMIMCurrentNum = 16;
			gMC.life = 4;
			gMC.max_life = 4;
		}
		if (character == "SamHogan")
		{
			SetNPCFlag(Flags::PROCGEN_CHARACTER);
			gMIMCurrentNum = 17;
			gMC.life = 6;
			gMC.max_life = 6;
		}
		if (character == "LunarSha")
		{
			SetNPCFlag(Flags::SERENITY_CHARACTER);
			gMIMCurrentNum = 18;
			gMC.life = 6;
			gMC.max_life = 6;
		}
	}
	if (CheckFlagTrait(FlagTraits::TOTAL_RANDOMNESS)){
		int life = Random(GetFlagTraitData<int>(FlagTraits::TOTAL_RANDOMNESS, "min_start_life"), GetFlagTraitData<int>(FlagTraits::TOTAL_RANDOMNESS, "max_start_life"), RandomCategories::SEEDED_PRIMARY);
		gMC.life = life;
		gMC.max_life = life;
	}
	if (ChallengeEnabled("secondCampaign") && !CheckFlagTrait(FlagTraits::NO_LIFE_CAPSULES))
	{
		int lifeCapsuleValue = 3;
		if (GetNPCFlag(Flags::MAHIN_CHARACTER))
			lifeCapsuleValue = 4;
		if (GetNPCFlag(Flags::AMY_CHARACTER))
			lifeCapsuleValue = 2;
		
		int lifeCapsuleAmount = LifeCapsuleLimit() / 1.5f * 0.75f;
		if (ChallengeEnabled("fast_run"))
			lifeCapsuleAmount *= 0.6f;

		for (int i = 0; i < lifeCapsuleAmount; i++){
			if (gMC.difficulty > DIFFICULTY_BASIC && i < lifeCapsuleAmount / 3)
				gMC.max_life += 2;
			else
				gMC.max_life += lifeCapsuleValue;
		}

		gMC.life = gMC.max_life;
	}
}

GenerationContext genContext;

unsigned long GetDailySeed(){
	unsigned long seed = time(0);
	seed -= seed % (60*60*24);
	seed += std::numeric_limits<int>::max();
	return seed;
}

unsigned long GenerateSeed(){
	unsigned long seed;
	if (gDaily){
		seed = GetDailySeed();
		gMC.randomSeed = false;
		playerHighScore = 0;
		LoadScore(seed);
	}
	else if (IsKeyFile("seed.txt"))
	{
		FILE *fp;
		std::string path;

		// Open seed
		path = gModulePath + "/seed.txt";

		fp = fopen(path.c_str(), "rb");
		if (fp == NULL)
			seed = time(0);
		else
		{
			// Read data
			fread(&seed, 1, 1, fp);
			if (seed >= '0' && seed <= '9')
			{
				// read as number
				fseek(fp, 0, SEEK_SET);
				fscanf(fp, "%lu", &seed);
			}
			else
			{
				// read as letters
				fseek(fp, 0, SEEK_SET);
				// fread(&seed, sizeof(unsigned long), 1, fp);
				char letteredSeed[12];
				if (fgets(letteredSeed, 10, fp) != NULL)
				{
					seed = 0;
					int seedLen = strlen(letteredSeed);
					for (int i = 0; i < seedLen; i++)
					{
						seed += (letteredSeed[i] - '0') * (pow(10, (seedLen - i - 1)));
					}
					int l = 0;
				}
				else
				{
					seed = time(0);
				}
			}
			fclose(fp);
			gMC.randomSeed = false;
			playerHighScore = 0;
			LoadScore(seed);
		}
	}
	else
	{
		seed = Random(0, std::numeric_limits<unsigned int>::max()-2);
		playerHighScore = 0;
		LoadScore(0);
		if (aprilFools)
		{
			std::vector<std::string> possibleSeeds = {"April", "Fool", "AprilFool", "April1st", "FoolsDay", "Trolling", "Fooling", "AprilFirs", "FirstApri", "AprilLol", "Foolish", "Trolls", "GetTroll", "DoNotBeli", "AprilJoke"};
			std::string aprilSeed = possibleSeeds[Random(0, possibleSeeds.size() - 1)];
			const char *letteredSeed = aprilSeed.c_str();
			seed = 0;
			int seedLen = strlen(letteredSeed);
			for (int i = 0; i < seedLen; i++)
			{
				seed += (letteredSeed[i] - '0') * (pow(10, (seedLen - i - 1)));
			}
		}
		gMC.randomSeed = true;
	}
	return seed;
} 

bool IsItSecondCampaign(){
	return gMC.depth >= genContext.doctorStage;
}

void InitTileset(){
	std::vector<char> tilesets{};
	int rlDepth = gMC.depth;
	int shadowDepth = 9 + genContext.doctorStage;
	if (genContext.newStage)
		rlDepth++;
	if (ChallengeEnabled("fast_run")){
		rlDepth *= 2;
		shadowDepth = 10 + genContext.doctorStage*2;
	}
	if (gMC.depth < genContext.doctorStage)
	{
		tilesets = {TYPE_FIRST_CAVE, TYPE_SAND_ZONE, TYPE_GRASSTOWN, TYPE_MIMIGA_VILLAGE, TYPE_SHEEP, TYPE_PLANTATION, TYPE_IKACHAN};

		if (rlDepth >= 2)
		{
			tilesets.push_back(TYPE_MARSH);
			tilesets.push_back(TYPE_ICE);
			tilesets.push_back(TYPE_LABYRINTH);
		}
		if (rlDepth >= 3)
		{
			tilesets.push_back(TYPE_HEADMAN);
			tilesets.push_back(TYPE_OUTER_WALL);
			tilesets.push_back(TYPE_MESA);
		}
		if (rlDepth >= 4)
		{
			tilesets.push_back(TYPE_DUNGEON);
			tilesets.push_back(TYPE_EGGS);
			tilesets.push_back(TYPE_INDOORS);
		}
		if (rlDepth >= 5)
		{
			tilesets.push_back(TYPE_LIVING_WATERWAY);
			tilesets.push_back(TYPE_ALMOND);
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_MIMIGA_VILLAGE));
		}
		if (rlDepth >= 6)
		{
			tilesets.push_back(TYPE_SPACESTATION);
			tilesets.push_back(TYPE_DARKCAVE);
			tilesets.push_back(TYPE_LABORATORY);
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_GRASSTOWN));
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_FIRST_CAVE));
		}
		if (rlDepth >= 7)
		{
			tilesets.push_back(TYPE_CASTLE);
			tilesets.push_back(TYPE_HELL);
			tilesets.push_back(TYPE_ARCADIA);
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_SHEEP));
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_SAND_ZONE));
		}
	} else if (rlDepth < shadowDepth) {
		tilesets = {TYPE_FIRST_CAVE, TYPE_SAND_ZONE, TYPE_GRASSTOWN, TYPE_MIMIGA_VILLAGE,
		 TYPE_EGGS, TYPE_PLANTATION, TYPE_IKACHAN, TYPE_LABYRINTH, TYPE_OUTER_WALL, TYPE_ICE};
		if (rlDepth >= genContext.doctorStage + 2){
			tilesets.push_back(TYPE_MARSH);
			tilesets.push_back(TYPE_HEADMAN);
			tilesets.push_back(TYPE_MESA);
			tilesets.push_back(TYPE_LIVING_WATERWAY);
			tilesets.push_back(TYPE_SHEEP);
		}
		if (rlDepth >= genContext.doctorStage + 3){
			tilesets.push_back(TYPE_DUNGEON);
			tilesets.push_back(TYPE_INDOORS);
			tilesets.push_back(TYPE_SPACESTATION);
			tilesets.push_back(TYPE_DARKCAVE);
			tilesets.push_back(TYPE_LABORATORY);
		}
		if (rlDepth >= genContext.doctorStage + 4){
			tilesets.push_back(TYPE_CASTLE);
			tilesets.push_back(TYPE_HELL);
			tilesets.push_back(TYPE_ARCADIA);
			tilesets.push_back(TYPE_ALMOND);
		}
		if (rlDepth >= genContext.doctorStage + 5){
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_MIMIGA_VILLAGE));
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_FIRST_CAVE));
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_GRASSTOWN));
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_SAND_ZONE));
		}
		if (rlDepth >= genContext.doctorStage + 6){
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_ICE));
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_LABYRINTH));
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_OUTER_WALL));
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_IKACHAN));
		}
		if (rlDepth >= genContext.doctorStage + 6){
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_MARSH));
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_SHEEP));
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_PLANTATION));
			tilesets.erase(std::find(tilesets.begin(), tilesets.end(), TYPE_INDOORS));
		}
	} else {
		tilesets = {TYPE_SHADOW};
	}
	Shuffle(tilesets, RandomCategories::SEEDED_PRIMARY);
	char type;
	char gimmicks[] = {GIMMICK_VAULT, GIMMICK_VAULT, GIMMICK_GRASS, GIMMICK_GRASS, GIMMICK_DARKNESS, GIMMICK_DARKNESS, GIMMICK_FRAGILE, GIMMICK_FRAGILE, GIMMICK_BLOCKS, GIMMICK_BLOCKS, GIMMICK_PUPPY, GIMMICK_PUPPY, GIMMICK_ROOMS, GIMMICK_ROOMS, GIMMICK_OPEN, GIMMICK_DEFENDED};
	int size = tilesets.size()-1;
	if (size == 0){
		type = tilesets[0];
		Random(0, 1, RandomCategories::SEEDED_PRIMARY);
	} else {
		type = tilesets[Random(0, size, RandomCategories::SEEDED_PRIMARY)];
	}
	if (Random(0, 15, RandomCategories::SEEDED_PRIMARY) <= 6 && (rlDepth > 1 || gMC.difficulty > DIFFICULTY_REINFORCED) && type != TYPE_SHADOW)
	{
		for (;;)
		{
			gMC.gimmick = gimmicks[Random(0, ARRAY_SIZE(gimmicks) - 1, RandomCategories::SEEDED_PRIMARY)];
			if (!genContext.newStage && gMC.depth > 4)
			{
				break;
			}
			else if (genContext.newStage && gMC.depth > 3)
			{
				break;
			}
			else
			{
				if (gMC.gimmick != GIMMICK_DARKNESS && gMC.gimmick != GIMMICK_BLOCKS && gMC.gimmick != GIMMICK_ROOMS && gMC.gimmick != GIMMICK_FRAGILE && gMC.gimmick != GIMMICK_PUPPY && gMC.gimmick != GIMMICK_VAULT)
					break;
			}
		}
	}
	else
	{
		gMC.gimmick = 0;
	}
	if (ChallengeExists("tileset"))
	{
		std::string typeName = gJConfig["tileset"];
		if (typeName != "")
		{
			for (char i = TYPE_FIRST_CAVE; i < TYPE_SHADOW + 1; i++)
			{
				if (TilesetName(i) == typeName)
				{
					type = i;
					break;
				}
			}
		}
	}
	if (ChallengeExists("gimmick"))
	{
		std::string typeName = gJConfig["gimmick"];
		if (typeName != "")
		{
			for (char i = GIMMICK_GRASS; i < GIMMICK_DEFENDED + 1; i++)
			{
				if (GetGimmick(typeName) == i)
				{
					gMC.gimmick = i;
					break;
				}
			}
		}
	}
	genContext.tileset = type;
	genContext.gimmick = gMC.gimmick;
}

void ConfigureMap(){
	ChangeMusic(MUS_MELTDOWN2);

	if (genContext.newStage)
	{
		for (int i = Flags::STAR_SAVE_FLAG_BEGIN; i < Flags::STAR_SAVE_FLAG_BEGIN + StarCount + 1; i++)
		{
			CutNPCFlag(i);
		}
		for (int i = 300; i < 399; i++)
		{
			CutNPCFlag(i);
		}
		for (int i = Flags::REFILL_SAVE_FLAG_BEGIN; i < Flags::REFILL_SAVE_FLAG_END; i++)
		{
			CutNPCFlag(i);
		}
		CutNPCFlag(Flags::LEVEL_BOSS_DEFEATED);
	}
	if (genContext.newStage)
	{
		gMC.depth++;
	}
	genContext.depth = gMC.depth;

	genContext.crampiness = 1;
	if (ChallengeValueReal("space_modifier") != -1)
	{
		genContext.crampiness = ChallengeValueReal("space_modifier");
	}
	if (genContext.gimmick == GIMMICK_ROOMS)
	{
		genContext.crampiness *= 0.23;
	}
	if (genContext.gimmick == GIMMICK_OPEN)
	{
		genContext.crampiness *= 3;
	}
	if (genContext.tileset == TYPE_DARKCAVE)
	{
		genContext.crampiness *= 0.77;
	}
	if (genContext.tileset == TYPE_SHADOW)
	{
		genContext.crampiness *= 1.1;
	}
	genContext.npcCount = 1;
	if (ChallengeValueReal("mob_modifier") != -1)
	{
		genContext.npcCount = ChallengeValueReal("mob_modifier");
	}
	if (genContext.gimmick == GIMMICK_VAULT)
	{
		genContext.npcCount *= 1.5;
	}
	if (genContext.tileset == TYPE_DUNGEON)
	{
		genContext.npcCount *= 1.75;
		if (gMC.depth > genContext.doctorStage){
			genContext.npcCount *= 1.33;
		}
	}
	if (genContext.tileset == TYPE_SHEEP)
	{
		genContext.npcCount *= 0.5;
	}
	if (gMC.difficulty > DIFFICULTY_BASIC){
		genContext.npcCount *= Random(133, 175, RandomCategories::SEEDED_NPCS) / 100.0f;
	}
}

constexpr const int defaultWidth = 118;
constexpr const int defaultHeight = 118;

bool LoadMapAssets(){
	std::string path_dir = "Stage";
	std::string path;
	bool bError = false;
	InitTileset();
	if (genContext.gimmick == GIMMICK_DARKNESS || ChallengeEnabled("darkness")){
		path = path_dir + "/PrtNCave" + genContext.tileset;
		if (!ResourceExists(path))
			path = path_dir + "/PrtCave" + genContext.tileset;
	} else {
		path = path_dir + "/PrtCave" + genContext.tileset;
	}

	if (IsItSecondCampaign() && genContext.tileset != TYPE_SHADOW)
		path += "_2";
	if (!ReloadBitmap_File(path.c_str(), SURFACE_ID_LEVEL_TILESET))
		bError = true;

	path = path_dir + '/' + gTMT[13].parts + ".pxa";
	if (!LoadAttributeData(path.c_str()))
		bError = true;

	// path = path_dir + '/' + gTMT[13].map + ".pxm";
	// if (genContext.tileset == TYPE_OUTER_WALL)
	// {
	// 	path = path_dir + '/' + gTMT[13].map + "2.pxm";
	// }
	// if (!LoadMapData2(path.c_str()))
	// 	bError = true;

	int stageWidth = defaultWidth;
	int stageHeight = defaultHeight;
	if (ChallengeEnabled("randomSizeMode")){
		stageWidth = Random(defaultWidth * 0.75f, defaultWidth * 1.50f, RandomCategories::SEEDED_PRIMARY);
		stageHeight = Random(defaultHeight * 0.66f, defaultHeight, RandomCategories::SEEDED_PRIMARY);
	}

	gMap.width = stageWidth;
	gMap.length = stageHeight;
	WriteGameLog("Map width: ", gMap.width);
	WriteGameLog("Map height: ", gMap.length);
	for (int x = 0; x < gMap.width; x++){
		for (int y = 0; y < gMap.length; y++){
			char neededTile;
			switch (x % 2){
				case 0:
					switch (y % 2){
						case 0:
							neededTile = 32; break;
						case 1:
							neededTile = 48; break;
					}
					break;
				case 1:
					switch (y % 2){
						case 0:
							neededTile = 33; break;
						case 1:
							neededTile = 49; break;
					}
					break;
			}
			gMap.data[x + y * gMap.width] = neededTile;
		}
	}
	// for (int i = 0; i < gMap.width * gMap.length; i++){
	// 	char neededTile;
	// 	switch (i % 4){
	// 		case 0:
	// 			neededTile = 32; break;
	// 		case 1:
	// 			neededTile = 33; break;
	// 		case 2:
	// 			neededTile = 48; break;
	// 		case 3:
	// 			neededTile = 49; break;
	// 	}
	// 	gMap.data[i] = neededTile;
	// }

	path = path_dir + '/' + gTMT[13].map + ".tsc";
	if (!LoadTextScript_Stage(path.c_str()))
		bError = true;

	switch (genContext.tileset)
	{
	case TYPE_FIRST_CAVE:
		path = "bkBlue";
		break;
	case TYPE_LIVING_WATERWAY:
		path = "bkWater";
		break;
	case TYPE_GRASSTOWN:
		path = "bkGard";
		break;
	case TYPE_SAND_ZONE:
		path = "bkGreen";
		break;
	case TYPE_LABYRINTH:
		path = "bkMaze";
		break;
	case TYPE_MIMIGA_VILLAGE:
		path = "bkGray";
		break;
	case TYPE_EGGS:
		path = "bkEggs";
		break;
	case TYPE_HELL:
		path = "bkRed";
		break;
	case TYPE_OUTER_WALL:
		path = "bkMoon";
		break;
	case TYPE_ALMOND:
		path = "bkAlmond";
		break;
	case TYPE_PLANTATION:
		path = "bkCent";
		break;
	case TYPE_ARCADIA:
		path = "bkArcadia";
		break;
	case TYPE_ICE:
		path = "bkIce";
		break;
	case TYPE_LABORATORY:
		path = "bkLab";
		break;
	case TYPE_SPACESTATION:
		path = "bkAlmostSpace";
		break;
	case TYPE_MARSH:
		path = "bkMarsh";
		break;
	case TYPE_INDOORS:
		path = "bkInside";
		break;
	case TYPE_DARKCAVE:
		path = "bkDarkcave";
		break;
	case TYPE_HEADMAN:
		path = "bkFoggy";
		break;
	case TYPE_DUNGEON:
		path = "bkDungeon";
		break;
	case TYPE_MESA:
		path = "bkMesa";
		break;
	case TYPE_CASTLE:
		path = "bkServ";
		break;
	case TYPE_SHEEP:
		path = "bkRed";
		break;
	case TYPE_IKACHAN:
		path = "bkIka";
		break;
	case TYPE_SHADOW:
		path = "bkShadow";
		break;
	}
	if (IsItSecondCampaign() && (fs::exists(GetResourcePath(path + "_2.png")) || 
								fs::exists(GetResourcePath(path + "_2.pbm")) ||
								fs::exists(GetResourcePath(path + "_2.bmp"))))
		path += "_2";
	gMC.tileset = genContext.tileset;
	int scrollType = gTMT[13].bkType;
	if (genContext.tileset == TYPE_OUTER_WALL || genContext.tileset == TYPE_SPACESTATION || genContext.tileset == TYPE_CASTLE)
		scrollType = 7;
	if (genContext.tileset == TYPE_ARCADIA || genContext.tileset == TYPE_HEADMAN || genContext.tileset == TYPE_MESA)
		scrollType = 0;
	if (!InitBack(path.c_str(), scrollType))
		bError = true;

	path_dir = "Npc";

	// Load NPC sprite sheets
	path = path_dir + "/Npc" + gTMT[13].npc;
	if (genContext.tileset == TYPE_SHADOW){
		path = path_dir + "/NpcToro";
	}
	if (!ReloadBitmap_File(path.c_str(), SURFACE_ID_LEVEL_SPRITESET_1))
		bError = true;

	path = path_dir + "/Npc" + gTMT[13].boss;
	if (!ReloadBitmap_File(path.c_str(), SURFACE_ID_LEVEL_SPRITESET_2))
		bError = true;
	return bError;
}

void CreateRectangles(GenerationContext& genContext, GenerationArguments& genArguments)
{
	int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["tilesets_with_reduction"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = Random(std::get<int>(genArguments["reduction_min"]), std::get<int>(genArguments["reduction_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["ruiny_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms /= std::get<int>(genArguments["ruiny_reduction"]);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["dungeon_tileset"])))
	{
		rooms *= std::get<int>(genArguments["dungeon_boost"]);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["artificial_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = Random(std::get<int>(genArguments["artificial_min"]), std::get<int>(genArguments["artificial_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["dried_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["dried_min"]), std::get<int>(genArguments["dried_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["otherworldy_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = Random(std::get<int>(genArguments["otherworldy_min"]), std::get<int>(genArguments["otherworldy_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["laboratory_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["laboratory_min"]), std::get<int>(genArguments["laboratory_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	rooms /= genContext.crampiness;
	for (int l = 0; l < rooms; l++)
	{
		auto w_stats = std::get<std::vector<int>>(genArguments["width"]);
		auto h_stats = std::get<std::vector<int>>(genArguments["height"]);
		if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["dungeon_tileset"])))
		{
			w_stats = std::get<std::vector<int>>(genArguments["dungeon_width"]);
			h_stats = std::get<std::vector<int>>(genArguments["dungeon_height"]);
		}
		int w = Random(w_stats[0], w_stats[1], RandomCategories::SEEDED_PRIMARY);
		int h = Random(h_stats[0], h_stats[1], RandomCategories::SEEDED_PRIMARY);
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["open_tilesets"])){
			if (genContext.tileset == GetTileset(tileset)){
				w *= std::get<float>(genArguments["open_boost"]);
				h *= std::get<float>(genArguments["open_boost"]);
			}
		}
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["closed_gimmick"])))
		{
			w *= std::get<float>(genArguments["closed_gimmick_boost"]);
			h *= std::get<float>(genArguments["closed_gimmick_boost"]);
		}
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["open_gimmick"])))
		{
			w *= std::get<float>(genArguments["open_gimmick_boost"]);
			h *= std::get<float>(genArguments["open_gimmick_boost"]);
		}
		PaintRectangle(0, Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY), Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY), w, h);
	}
}

void CreateCircles(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["maze_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["maze_min"]), std::get<int>(genArguments["maze_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["sandy_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["sandy_min"]), std::get<int>(genArguments["sandy_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["contained_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = Random(std::get<int>(genArguments["contained_min"]), std::get<int>(genArguments["contained_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["blacklist_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = 0;
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["rounded_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = Random(std::get<int>(genArguments["rounded_min"]), std::get<int>(genArguments["rounded_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["arcadia_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["arcadia_min"]), std::get<int>(genArguments["arcadia_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	rooms /= genContext.crampiness;
	for (int l = 0; l < rooms; l++)
	{
		auto radius = std::get<std::vector<int>>(genArguments["radius"]);
		int r = Random(radius[0], radius[1], RandomCategories::SEEDED_PRIMARY);
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["tilesets_in_sky"])){
			if (genContext.tileset == GetTileset(tileset)){
				r *= std::get<float>(genArguments["radius_mod_in_sky"]);
			}
		}
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["tilesets_with_boost"])){
			if (genContext.tileset == GetTileset(tileset)){
				r *= std::get<float>(genArguments["radius_boost"]);
			}
		}
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["closed_gimmick"])))
		{
			r *= std::get<float>(genArguments["closed_gimmick_boost"]);
		}
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["open_gimmick"])))
		{
			r *= std::get<float>(genArguments["open_gimmick_boost"]);
		}
		PaintEllipse(0, Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY), Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY), r);
	}
}

void CreateMediumRooms(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["maze_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["maze_min"]), std::get<int>(genArguments["maze_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["sandy_tileset"])))
	{
		rooms *= std::get<float>(genArguments["sandy_modifier"]);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["blacklist_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = 0;
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["grassy_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["grassy_min"]), std::get<int>(genArguments["grassy_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["natural_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = Random(std::get<int>(genArguments["natural_min"]), std::get<int>(genArguments["natural_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["arcadia_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["arcadia_min"]), std::get<int>(genArguments["arcadia_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["contained_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = Random(std::get<int>(genArguments["contained_min"]), std::get<int>(genArguments["contained_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	rooms /= genContext.crampiness;
	for (int l = 0; l < rooms; l++)
	{
		int x, y;
		do
		{
			x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (!IsEmpty(x, y));
		auto w_stats = std::get<std::vector<int>>(genArguments["width"]);
		auto h_stats = std::get<std::vector<int>>(genArguments["height"]);
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["wide_tilesets"])){
			if (genContext.tileset == GetTileset(tileset)){
				w_stats = std::get<std::vector<int>>(genArguments["wide_width"]);
				h_stats = std::get<std::vector<int>>(genArguments["wide_height"]);
			}
		}
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["unsafe_tilesets"])){
			if (genContext.tileset == GetTileset(tileset)){
				w_stats = std::get<std::vector<int>>(genArguments["unsafe_width"]);
				h_stats = std::get<std::vector<int>>(genArguments["unsafe_height"]);
			}
		}
		int w = Random(w_stats[0], w_stats[1], RandomCategories::SEEDED_PRIMARY);
		int h = Random(h_stats[0], h_stats[1], RandomCategories::SEEDED_PRIMARY);
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["closed_gimmick"])))
		{
			w *= std::get<float>(genArguments["closed_gimmick_boost"]);
			h *= std::get<float>(genArguments["closed_gimmick_boost"]);
		}
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["open_gimmick"])))
		{
			w *= std::get<float>(genArguments["open_gimmick_boost"]);
			h *= std::get<float>(genArguments["open_gimmick_boost"]);
		}
		PaintRectangle(0, x, y, w, h);
		auto aRooms = std::get<std::vector<int>>(genArguments["micro_rooms"]);
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["micro_tilesets_boost"])){
			if (genContext.tileset == GetTileset(tileset)){
				aRooms = std::get<std::vector<int>>(genArguments["micro_boost"]);
			}
		}
		if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["micro_tileset_sky"])))
		{
			aRooms = std::get<std::vector<int>>(genArguments["micro_sky_boost"]);
		}
		if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["micro_tileset_arcadia"])))
		{
			aRooms = std::get<std::vector<int>>(genArguments["micro_arcadia_boost"]);
		}
		if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["micro_tileset_eggs"])))
		{
			aRooms = std::get<std::vector<int>>(genArguments["micro_eggs_boost"]);
		}
		int addRooms = Random(aRooms[0], aRooms[1], RandomCategories::SEEDED_PRIMARY);
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["closed_micro_gimmick"])))
		{
			addRooms *= std::get<float>(genArguments["closed_micro_gimmick_boost"]);
		}
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["open_micro_gimmick"])))
		{
			addRooms *= std::get<float>(genArguments["open_micro_gimmick_boost"]);
		}
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["micro_blacklist_tilesets"])){
			if (genContext.tileset == GetTileset(tileset))
				addRooms = 0;
		}
		for (int ll = 0; ll < addRooms; ll++)
		{
			auto mw_stats = std::get<std::vector<int>>(genArguments["micro_width"]);
			auto mh_stats = std::get<std::vector<int>>(genArguments["micro_height"]);
			PaintRectangle(1, Random(x, x + w, RandomCategories::SEEDED_PRIMARY), Random(y, y + h, RandomCategories::SEEDED_PRIMARY), 
				Random(mw_stats[0], mw_stats[1], RandomCategories::SEEDED_PRIMARY), Random(mh_stats[0], mh_stats[1], RandomCategories::SEEDED_PRIMARY));
		}
	}
}

void CreateBigRooms(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["sandy_tileset"])))
	{
		rooms *= std::get<float>(genArguments["sandy_modifier"]);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["maze_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["maze_min"]), std::get<int>(genArguments["maze_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["grassy_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["grassy_min"]), std::get<int>(genArguments["grassy_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["wider_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = Random(std::get<int>(genArguments["wider_min"]), std::get<int>(genArguments["wider_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["castle_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["castle_min"]), std::get<int>(genArguments["castle_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["natural_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = Random(std::get<int>(genArguments["natural_min"]), std::get<int>(genArguments["natural_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["blacklist_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = 0;
	}
	rooms /= genContext.crampiness;
	for (int l = 0; l < rooms; l++)
	{
		int x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
		int y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		auto w_stats = std::get<std::vector<int>>(genArguments["width"]);
		auto h_stats = std::get<std::vector<int>>(genArguments["height"]);
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["wide_tilesets"])){
			if (genContext.tileset == GetTileset(tileset)){
				w_stats = std::get<std::vector<int>>(genArguments["wide_width"]);
				h_stats = std::get<std::vector<int>>(genArguments["wide_height"]);
			}
		}
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["unsafe_tilesets"])){
			if (genContext.tileset == GetTileset(tileset)){
				w_stats = std::get<std::vector<int>>(genArguments["unsafe_width"]);
				h_stats = std::get<std::vector<int>>(genArguments["unsafe_height"]);
			}
		}
		int w = Random(w_stats[0], w_stats[1], RandomCategories::SEEDED_PRIMARY);
		int h = Random(h_stats[0], h_stats[1], RandomCategories::SEEDED_PRIMARY);
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["closed_gimmick"])))
		{
			w *= std::get<float>(genArguments["closed_gimmick_boost"]);
			h *= std::get<float>(genArguments["closed_gimmick_boost"]);
		}
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["open_gimmick"])))
		{
			w *= std::get<float>(genArguments["open_gimmick_boost"]);
			h *= std::get<float>(genArguments["open_gimmick_boost"]);
		}
		PaintRectangle(0, x, y, w, h);
		auto aRooms = std::get<std::vector<int>>(genArguments["micro_rooms"]);
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["micro_tilesets_boost"])){
			if (genContext.tileset == GetTileset(tileset)){
				aRooms = std::get<std::vector<int>>(genArguments["micro_boost"]);
			}
		}
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["micro_tilesets_sky"])){
			if (genContext.tileset == GetTileset(tileset)){
				aRooms = std::get<std::vector<int>>(genArguments["micro_boost_sky"]);
			}
		}
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["micro_tilesets_unsafe"])){
			if (genContext.tileset == GetTileset(tileset)){
				aRooms = std::get<std::vector<int>>(genArguments["micro_boost_unsafe"]);
			}
		}
		if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["micro_tileset_eggs"])))
		{
			aRooms = std::get<std::vector<int>>(genArguments["micro_eggs_boost"]);
		}
		int addRooms = Random(aRooms[0], aRooms[1], RandomCategories::SEEDED_PRIMARY);
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["closed_micro_gimmick"])))
		{
			addRooms *= std::get<float>(genArguments["closed_micro_gimmick_boost"]);
		}
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["open_micro_gimmick"])))
		{
			addRooms *= std::get<float>(genArguments["open_micro_gimmick_boost"]);
		}
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["micro_blacklist_tilesets"])){
			if (genContext.tileset == GetTileset(tileset))
				addRooms = 0;
		}
		for (int ll = 0; ll < addRooms; ll++)
		{
			auto mw_stats = std::get<std::vector<int>>(genArguments["micro_width"]);
			auto mh_stats = std::get<std::vector<int>>(genArguments["micro_height"]);
			PaintRectangle(1, Random(x, x + w, RandomCategories::SEEDED_PRIMARY), Random(y, y + h, RandomCategories::SEEDED_PRIMARY), 
				Random(mw_stats[0], mw_stats[1], RandomCategories::SEEDED_PRIMARY), Random(mh_stats[0], mh_stats[1], RandomCategories::SEEDED_PRIMARY));
		}
	}
}

void CreateCorridors(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["constrained_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = Random(std::get<int>(genArguments["constrained_min"]), std::get<int>(genArguments["constrained_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["tight_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms *= std::get<float>(genArguments["tight_modifier"]);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["dungeon_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["dungeon_min"]), std::get<int>(genArguments["dungeon_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["grassy_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["grassy_min"]), std::get<int>(genArguments["grassy_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["artificial_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = Random(std::get<int>(genArguments["artificial_min"]), std::get<int>(genArguments["artificial_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["maze_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["maze_min"]), std::get<int>(genArguments["maze_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["arcadia_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["arcadia_min"]), std::get<int>(genArguments["arcadia_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["frigid_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["frigid_min"]), std::get<int>(genArguments["frigid_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["laboratory_tileset"])))
	{
		rooms = Random(std::get<int>(genArguments["laboratory_min"]), std::get<int>(genArguments["laboratory_max"]), RandomCategories::SEEDED_PRIMARY);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["blacklist_tilesets"])){
		if (genContext.tileset == GetTileset(tileset))
			rooms = 0;
	}
	rooms /= genContext.crampiness;
	for (int l = 0; l < rooms; l++)
	{
		int x, y;
		do
		{
			x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (!IsEmpty(x, y));
		auto w_stats = std::get<std::vector<int>>(genArguments["width"]);
		auto h_stats = std::get<std::vector<int>>(genArguments["height"]);
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["small_tilesets"])){
			if (genContext.tileset == GetTileset(tileset)){
				w_stats = std::get<std::vector<int>>(genArguments["small_width"]);
				h_stats = std::get<std::vector<int>>(genArguments["small_height"]);
			}
		}
		int w = Random(w_stats[0], w_stats[1], RandomCategories::SEEDED_PRIMARY);
		int h = Random(h_stats[0], h_stats[1], RandomCategories::SEEDED_PRIMARY);
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["closed_gimmick"])))
		{
			w *= std::get<float>(genArguments["closed_gimmick_boost"]);
			h *= std::get<float>(genArguments["closed_gimmick_boost"]);
		}
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["open_gimmick"])))
		{
			w *= std::get<float>(genArguments["open_gimmick_boost"]);
			h *= std::get<float>(genArguments["open_gimmick_boost"]);
		}
		for (auto tileset: std::get<std::vector<std::string>>(genArguments["sky_tilesets"])){
			if (genContext.tileset == GetTileset(tileset)){
				w *= std::get<float>(genArguments["sky_modifier"]);
				h *= std::get<float>(genArguments["sky_modifier"]);
			}
		}
		PaintRectangle(0, x, y, w, h);
	}
}

void CreateSandPatches(GenerationContext& genContext, GenerationArguments& genArguments){
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["tileset"]))){
		int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
		rooms /= genContext.crampiness;
		for (int l = 0; l < rooms; l++)
		{
			int x, y;
			do
			{
				x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
				y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
			} while (IsEmpty(x, y));
			int w = std::get<int>(genArguments["width"]);
			int h = std::get<int>(genArguments["height"]);
			if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["closed_gimmick"])))
			{
				w *= std::get<float>(genArguments["closed_gimmick_boost"]);
				h *= std::get<float>(genArguments["closed_gimmick_boost"]);
			}
			if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["open_gimmick"])))
			{
				w *= std::get<float>(genArguments["open_gimmick_boost"]);
				h *= std::get<float>(genArguments["open_gimmick_boost"]);
			}
			std::vector<bool> patch = GeneratePatch(w, h, 
				std::get<float>(genArguments["patch_fill_ratio"]), 
				std::get<int>(genArguments["patch_clustering"]), 
				std::get<bool>(genArguments["patch_force_fill_rate"]));

			int pWidth = w - 2;

			for (int i = 0; i < patch.size() - pWidth; i++)
			{
				if (!patch[i])
					continue;

				// we don't need to check above because we are either at the top
				//  or have already dealt with those tiles

				// down-left
				if (i % pWidth != 0)
				{
					if (patch[i - 1 + pWidth] && !(patch[i - 1] || patch[i + pWidth]))
					{
						patch[i - 1 + pWidth] = false;
					}
				}

				// down-right
				if ((i + 1) % pWidth != 0)
				{
					if (patch[i + 1 + pWidth] && !(patch[i + 1] || patch[i + pWidth]))
					{
						patch[i + 1 + pWidth] = false;
					}
				}
			}

			for (int l = 0; l < w; l++)
			{
				for (int k = 0; k < h; k++)
				{
					if (patch[l + k * w])
					{
						ChangeMap(x + l, y + k, 0);
					}
				}
			}
		}
	}
}

void CreateMazes(GenerationContext& genContext, GenerationArguments& genArguments){
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["tileset"]))){
		int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
			if (gMC.depth > genContext.doctorStage) rooms *= std::get<int>(genArguments["second_campaign_scaling"]);
		rooms /= genContext.crampiness;
		for (int l = 0; l < rooms; l++)
		{
			int dir, dir1;
			int x, y;
			dir = -1;
			do
			{
				x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
				y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
			} while (IsEmpty(x, y));
			auto lin = std::get<std::vector<int>>(genArguments["lines"]);
			int lines = Random(lin[0], lin[1], RandomCategories::SEEDED_PRIMARY);
			if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["closed_gimmick"])))
			{
				lines *= std::get<float>(genArguments["closed_gimmick_boost"]);
			}
			if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["open_gimmick"])))
			{
				lines *= std::get<float>(genArguments["open_gimmick_boost"]);
			}
			for (int m = 0; m < lines; m++)
			{
				do
				{
					dir1 = Random(0, 3, RandomCategories::SEEDED_PRIMARY);
				} while ((dir == 0 && (dir1 == 0 || dir1 == 2)) ||
						(dir == 2 && (dir1 == 0 || dir1 == 2)) ||
						(dir == 1 && (dir1 == 1 || dir1 == 3)) ||
						(dir == 3 && (dir1 == 1 || dir1 == 3)));
				dir = dir1;
				auto length = std::get<std::vector<int>>(genArguments["length"]);
				int lineLength = Random(length[0], length[1], RandomCategories::SEEDED_PRIMARY);
				switch (dir)
				{
				case 0:
					PaintLine(0, x, y, x - lineLength, y);
					x -= lineLength;
					break;
				case 2:
					PaintLine(0, x, y, x + lineLength, y);
					x += lineLength;
					break;
				case 1:
					PaintLine(0, x, y, x, y - lineLength);
					y -= lineLength;
					break;
				case 3:
					PaintLine(0, x, y, x, y + lineLength);
					y += lineLength;
					break;
				}
			}
		}
	}
}

void CreateDungeonTunnels(GenerationContext& genContext, GenerationArguments& genArguments){
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["tileset"]))){
		int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
		rooms /= genContext.crampiness;
		for (int i = 0; i < rooms; i++)
		{
			int x, y;
			do
			{
				x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
				y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
			} while (!IsEmpty(x, y));
			int direction = Random(0, 3, RandomCategories::SEEDED_PRIMARY);
			if (direction == 0)
			{ // left
				while (GetTile(x, y) == 0 && InsideMap(x, y))
				{
					x--;
				}
				while (GetTile(x, y) != 0 && InsideMap(x, y))
				{
					ChangeMap(x, y, 0);
					ChangeMap(x, y - 1, 0);
					x--;
				}
			}
			else if (direction == 1)
			{ // up
				while (GetTile(x, y) == 0 && InsideMap(x, y))
				{
					y--;
				}
				while (GetTile(x, y) != 0 && InsideMap(x, y))
				{
					ChangeMap(x, y, 0);
					ChangeMap(x - 1, y, 0);
					y--;
				}
			}
			else if (direction == 2)
			{ // right
				while (GetTile(x, y) == 0 && InsideMap(x, y))
				{
					x++;
				}
				while (GetTile(x, y) != 0 && InsideMap(x, y))
				{
					ChangeMap(x, y, 0);
					ChangeMap(x, y - 1, 0);
					x++;
				}
			}
			else if (direction == 3)
			{ // down
				while (GetTile(x, y) == 0 && InsideMap(x, y))
				{
					y++;
				}
				while (GetTile(x, y) != 0 && InsideMap(x, y))
				{
					ChangeMap(x, y, 0);
					ChangeMap(x - 1, y, 0);
					y++;
				}
			}
		}
	}
}

void CreateSkyBorders(GenerationContext& genContext, GenerationArguments& genArguments){
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["tilesets"])){
		if (genContext.tileset == GetTileset(tileset)){
			for (int i = 0; i < gMap.width; i++)
			{
				gMap.data[i] = 0;
			}
			for (int i = gMap.width; i < gMap.width * 2; i++)
			{
				gMap.data[i] = 0;
			}
			for (int i = 0; i < gMap.width * gMap.length; i += gMap.width)
			{
				gMap.data[i] = 0;
			}
			for (int i = 1; i < gMap.width * gMap.length; i += gMap.width)
			{
				gMap.data[i] = 0;
			}
			for (int i = gMap.width - 1; i < gMap.width * gMap.length; i += gMap.width)
			{
				gMap.data[i] = 0;
			}
			for (int i = gMap.width - 2; i < gMap.width * gMap.length; i += gMap.width)
			{
				gMap.data[i] = 0;
			}
			for (int i = gMap.width * (gMap.length - 2); i < gMap.width * gMap.length; i++)
			{
				gMap.data[i] = 0;
			}
			// for (int i = gMap.width*(gMap.length-1); i < gMap.width*gMap.length; i += 1){
			// 	gMap.data[i] = 0;
			// }
			// for (int i = gMap.width*(gMap.length-2); i < gMap.width*gMap.length; i += 1){
			// 	gMap.data[i] = 0;
			// }
			gMC.outerWallInit = true;
			if (gMC.depth > genContext.doctorStage){
				SetNpChar(296, 0, 0, 0, 0, 2, NULL, 0x100);
			}
		}
	}
}

int fluidcounting(std::vector<int>& visited, int x, int y, bool solid)
{
	int count = 0;
	if(InsideMap(x, y) && 
		(solid ? GetAttribute(x, y) == 0x41 : GetAttribute(x, y) != 0)
		&& visited[x + (y * gMap.width)] == 0)
    {
		visited[x + (y * gMap.width)] = 1;
        count += 1;
        count += fluidcounting(visited, x+1, y, solid);
        count += fluidcounting(visited, x-1, y, solid);
		count += fluidcounting(visited, x, y-1, solid);
		count += fluidcounting(visited, x, y+1, solid);
    }
	return count;
}

void fluidfilling(int x, int y, bool solid)
{
	if(InsideMap(x, y) && (solid ? GetAttribute(x, y) == 0x41 : GetAttribute(x, y) == 0))
    {
		std::array<std::array<int, 2>, 2> possibleTiles = {{{32, 33}, {48, 49}}};
		int tile = possibleTiles[y % 2][x % 2];
		if (solid)
			tile = 0;
		gMap.data[x + (y * gMap.width)] = tile;
        fluidfilling(x+1, y, solid);
        fluidfilling(x-1, y, solid);
		fluidfilling(x, y-1, solid);
		fluidfilling(x, y+1, solid);
    }
}

void PolishStage(GenerationContext& genContext, GenerationArguments& genArguments){
	std::vector<int> visited(gMap.width*gMap.length);
	for (int t = 0; t < gMap.width * gMap.length; t++){
		int count = fluidcounting(visited, XCoord(t), YCoord(t), false);
		if (count > 0 && count <= std::get<int>(genArguments["too_small_space_threshold"])){
			fluidfilling(XCoord(t), YCoord(t), false);
		}
	}
	visited.clear();
	visited.resize(gMap.width*gMap.length);
	for (int t = 0; t < gMap.width * gMap.length; t++){
		int count = fluidcounting(visited, XCoord(t), YCoord(t), true);
		if (count > 0 && count <= std::get<int>(genArguments["too_small_solid_threshold"])){
			fluidfilling(XCoord(t), YCoord(t), true);
		}
	}

	int width = gMap.width;
	const int NEIGHBOURS8[] = {-width - 1, -width, -width + 1, -1, +1, +width - 1, +width, +width + 1};
	for (int t = 0; t < gMap.width * gMap.length; t++)
	{
		bool surrounded = true;
		for (int i = 0; i < 8; i++)
		{
			if (InsideMap(t + NEIGHBOURS8[i]))
			{
				if (gMap.data[t + NEIGHBOURS8[i]] == 0)
				{
					surrounded = false;
				}
			}
		}
		if (!surrounded && gMap.data[t] != 0)
			gMap.data[t] = 1;
	}
}

void CreateSpikes(GenerationContext& genContext, GenerationArguments& genArguments){
	gMC.hell = false;
	int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY) 
		+ std::get<int>(genArguments["scaling"]) * gMC.depth;
	double spikeMod = 1;
	if (ChallengeValueReal("spike_modifier") != -1)
	{
		spikeMod = ChallengeValueReal("spike_modifier");
	}
	if (gMC.difficulty > DIFFICULTY_BASIC){
		spikeMod *= std::get<float>(genArguments["difficulty_scaling"]);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["village_tileset"])))
	{
		rooms *= std::get<float>(genArguments["village_scaling"]);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["sky_tilesets"])){
			if (genContext.tileset == GetTileset(tileset))
				rooms *= std::get<float>(genArguments["sky_scaling"]);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["hell_tilesets"])){
			if (genContext.tileset == GetTileset(tileset)){
				rooms *= std::get<float>(genArguments["hell_scaling"]);
				gMC.hell = true;
			}
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["final_tileset"])))
	{
		rooms *= std::get<float>(genArguments["final_scaling"]);
		gMC.hell = true;
	}
	rooms *= spikeMod;
	for (int l = 0; l < rooms; l++)
	{
		int x, y;
		do
		{
			x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (!(HasSurface(x, y) || GetTile(x, y) == 66));
		ChangeMap(x, y, 66);
	}
}

void CreateChests(GenerationContext& genContext, GenerationArguments& genArguments){
	int limit, rooms;
	if (CheckItemSpace())
	{
		rooms = std::get<int>(genArguments["amount"]);
		if (CheckFlagTrait(FlagTraits::CHEST_BOOST))
		{
			rooms *= GetFlagTraitData<float>(FlagTraits::CHEST_BOOST, "base");
		}
		limit = ChallengeValue("chest_limit");
		if (limit != -1 && (limit <= rooms || limit > std::get<int>(genArguments["amount"])))
		{
			rooms = limit;
		}
		if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["vault_gimmick"])))
		{
			rooms += std::get<int>(genArguments["vault_boost"]);
		}
		if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["dungeon_tileset"])))
		{
			rooms += std::get<int>(genArguments["dungeon_boost"]);
		}
		if (CheckFlagTrait(FlagTraits::CHEST_BOOST))
		{
			rooms += GetFlagTraitData<int>(FlagTraits::CHEST_BOOST, "amount");
		}
		if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["blacklist_tileset"])))
		{
			rooms = 0;
		}

		if (genContext.newStage)
		{
			memset(gMC.generatedChests, 0, sizeof(gMC.generatedChests));
		}

		for (int l = 0; l < rooms; l++)
		{
			int dropID;
			if (gAvailableIDs.size() > 0)
			{
				int chestID, dropID, tscEvent;
				if (genContext.newStage)
				{
					chestID = Random(0, gAvailableIDs.size() - 1, RandomCategories::SEEDED_CHESTS);
					dropID = gAvailableIDs[chestID];
				}
				else
				{
					Random(0, 1, RandomCategories::SEEDED_CHESTS);
					dropID = gMC.generatedChests[l];
				}
				tscEvent = ((dropID - 201) / 100);
				int x2, y2;
				do
				{
					x2 = Random(0, gMap.width, RandomCategories::SEEDED_CHESTS);
					y2 = Random(0, gMap.length, RandomCategories::SEEDED_CHESTS);
				} while (!(HasSurface(x2, y2) && GetNpCharHere(x2, y2) == -1));
				if (!GetNPCFlag(dropID))
					SetNpChar(15, x2 * 0x2000, y2 * 0x2000, 0, 0, 0, NULL, 0, 200 + tscEvent, 0x2000);
				else
					SetNpChar(21, x2 * 0x2000, y2 * 0x2000, 0, 0, 0, NULL, 0, 200 + tscEvent, 0x2000);
				SetNPCFlag(dropID - 1);
				if (genContext.newStage)
				{
					std::vector<int>::iterator it = gAvailableIDs.begin();
					it += chestID;
					gAvailableIDs.erase(it);
					gMC.generatedChests[l] = dropID;
				}
				PaintLine(41, x2 - 2, y2 + 1, x2 + 1, y2 + 1);
				if (gAvailableIDs.size() == 0)
					break;
			}
		}
	}
}

void CreateLifeCapsules(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_NPCS);
	int limit = ChallengeValue("health_limit");
	if (genContext.gimmick == GetGimmick(std::get<std::string>(genArguments["gimmick"])))
	{
		rooms = std::get<int>(genArguments["max"]);
	}
	if (limit == -1)
	{
		limit = rooms;
	}
	if (limit > std::get<int>(genArguments["max"]))
	{
		rooms = limit;
	}
	if (ChallengeEnabled("no_health") || CheckFlagTrait(FlagTraits::NO_LIFE_CAPSULES))
	{
		limit = 0;
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["blacklist_tileset"]))){
		rooms = 0;
	}
	for (int l = 0; l < rooms; l++)
	{
		if (gMC.lifeCapsuleLimit < LifeCapsuleLimit())
		{
			int x2, y2;
			do
			{
				x2 = Random(0, gMap.width, RandomCategories::SEEDED_NPCS);
				y2 = Random(0, gMap.length, RandomCategories::SEEDED_NPCS);
			} while (!(HasSurface(x2, y2) && GetNpCharHere(x2, y2) == -1));
			if (l <= (limit - 1))
			{
				if (!GetNPCFlag(Flags::CAPSULE_SAVE_FLAG_BEGIN + l))
				{
					int eventNumber = Flags::CAPSULE_SAVE_FLAG_BEGIN;
					if (Random(0, 3) == 0 && gMC.difficulty > DIFFICULTY_BASIC){
						eventNumber = Flags::CAPSULE_SAVE_FLAG_BEGIN + 2;
					}
					SetNpChar_EventFlag(32, x2 * 0x2000, y2 * 0x2000, 0, 0, 0, NULL, eventNumber, Flags::CAPSULE_SAVE_FLAG_BEGIN + l);
					PaintLine(41, x2 - 2, y2 + 1, x2 + 1, y2 + 1);
					if (genContext.newStage)
						gMC.lifeCapsuleLimit++;
				}
			}
		}
	}
}

void CreateRefills(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = std::get<int>(genArguments["amount"]);
	int limit = rooms;
	ModifyWithChallengeValue(&limit, "refill_limit");
	if (rooms >= limit)
	{
		rooms = limit;
	}
	if (gMC.difficulty > DIFFICULTY_BASIC)
		rooms = std::max(1, rooms - std::get<int>(genArguments["difficulty_reduction"])*gMC.difficulty);
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["blacklist_tilesets"])){
		if (genContext.tileset == GetTileset(tileset)){
			rooms = 0;
		}
	}
	for (int l = 0; l < rooms; l++)
	{
		int x, y;
		do
		{
			x = Random(0, gMap.width, RandomCategories::SEEDED_NPCS);
			y = Random(0, gMap.length, RandomCategories::SEEDED_NPCS);
		} while (!(HasSurface(x, y) && GetNpCharHere(x, y) == -1));
		NPCHAR* refillNPC = CreateNpChar(17, x * 0x2000, y * 0x2000, 0, 0, 0, NULL, 0);
		if (refillNPC != NULL){
			refillNPC->code_event = 17;
			refillNPC->bits = NPC_INTERACTABLE;
			if (GetNPCFlag(Flags::REFILL_SAVE_FLAG_BEGIN + l)){
				refillNPC->count1 = 1;
			}
		}
		PaintLine(41, x - 2, y + 1, x + 1, y + 1);
	}
}

void CreateSavePoint(GenerationContext& genContext, GenerationArguments& genArguments){
	int x, y;
	int tries = 0, distanceBonus = 0;
	do
	{
		x = Random(0, gMap.width, RandomCategories::SEEDED_NPCS);
		y = Random(0, gMap.length, RandomCategories::SEEDED_NPCS);
		tries++;
		if (tries >= 250){
			distanceBonus++;
			tries = 0;
			if (ChallengeEnabled("stage_debug")){
				WriteGameLog("It looks like save point generation doesn't work at distance of ", 
					std::get<int>(genArguments["spawn_distance"]) + distanceBonus - 1,
					", trying at distance of ",
					std::get<int>(genArguments["spawn_distance"]) + distanceBonus);
			}
		}
	} while (!(HasSurface(x, y) && GetNpCharHere(x, y) == -1 &&
			Distance(x, y, doorX, doorY) < std::get<int>(genArguments["spawn_distance"]) + distanceBonus));
	if (!ChallengeEnabled("timer"))
	{
		SetNpChar(16, x * 0x2000, y * 0x2000, 0, 0, 0, NULL, 0, 16, 0x2000);
		PaintLine(41, x - 2, y + 1, x + 1, y + 1);
	}
}

void GenerateWater(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
	int maxHeight = gMap.length * std::get<float>(genArguments["max_height_ratio"]);
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["rising_water_tileset"])))
	{
		rooms = 0;
		SetNpChar(191, 0, gMap.length * std::get<float>(genArguments["max_height_ratio"]) * 0x2000, 0, 0, 0, NULL, 0x100);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["additional_water_tileset"])))
	{
		rooms += std::get<int>(genArguments["additional_water_boost"]);
	}
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["pure_water_tileset"]))){
		rooms = std::get<int>(genArguments["pure_water_amount"]);
		maxHeight *= std::get<float>(genArguments["pure_water_max_height_ratio"]);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["blacklist_tilesets"])){
		if (genContext.tileset == GetTileset(tileset)){
			rooms = 0;
		}
	}
	bool water = rooms > 0;
	for (int l = 0; l < rooms; l++)
	{
		int x, y;
		do
		{
			x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			y = Random(maxHeight, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (!HasSurface(x, y));
		FluidFall(x, y, 0, 30);
		for (int t = 0; t < gMap.length * gMap.width; t++)
		{
			if (GetTile(XCoord(t), YCoord(t) - 1) == 0 && GetTile(XCoord(t), YCoord(t)) == 30)
			{
				int x = XCoord(t);
				int y = YCoord(t);
				while (GetAttribute(x, y) != 96)
				{
					x -= 1;
				}
				while (GetTile(x, y) == 30 && IsEmpty(x, y - 1))
				{
					ChangeMap(x, y, 13);
					x += 1;
				}
			}
		}
	}
	for (int t = 0; t < gMap.width * gMap.length; t++)
	{
		if (GetTile(XCoord(t), YCoord(t) - 1) == 30 && GetTile(XCoord(t), YCoord(t)) == 13)
		{
			gMap.data[t] = 30;
		}
	}

	if (water)
	{
		int waterAmount = 0;
		for (int t = 0; t < gMap.length * gMap.width; t++)
		{
			if (gMap.data[t] == 30)
			{
				waterAmount++;
			}
		}
		if (waterAmount > std::get<int>(genArguments["jellyfish_water_amount_requirement"]))
		{
			for (auto tileset: std::get<std::vector<std::string>>(genArguments["hell_tilesets"])){
				if (genContext.tileset == GetTileset(tileset)){
					return;
				}
			}
			rooms = Random(waterAmount / std::get<int>(genArguments["min_jellyfish_ratio"]), 
				waterAmount / std::get<int>(genArguments["max_jellyfish_ratio"]), RandomCategories::SEEDED_PRIMARY);
			for (auto tileset: std::get<std::vector<std::string>>(genArguments["acid_water_tilesets"])){
				if (genContext.tileset == GetTileset(tileset)){
					rooms *= std::get<float>(genArguments["acid_jellyfish_ratio"]);
				}
			}
			rooms *= genContext.npcCount;
			for (int l = 0; l < rooms; l++)
			{
				int x, y;
				do
				{
					x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
					y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
				} while (GetTile(x, y) != 30);
				SetNpChar(95, x * 0x2000, y * 0x2000, 0, 0, 0, NULL, 0x100);
			}
			if (gMC.max_life >= std::get<int>(genArguments["min_kulala_life"]))
			{
				rooms = Random(waterAmount / std::get<int>(genArguments["min_kulala_ratio"]), 
				waterAmount / std::get<int>(genArguments["max_kulala_ratio"]), RandomCategories::SEEDED_NPCS);
				for (auto tileset: std::get<std::vector<std::string>>(genArguments["acid_water_tilesets"])){
					if (genContext.tileset == GetTileset(tileset)){
						rooms *= std::get<float>(genArguments["acid_jellyfish_ratio"]);
					}
				}
				rooms *= genContext.npcCount;
				for (int l = 0; l < rooms; l++)
				{
					int x, y, x1, y1;
					do
					{
						x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
						y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
					} while (!(GetTile(x, y) == 13));
					SetNpChar(94, x * 0x2000, y * 0x2000, 0, 0, Random(0, 1, RandomCategories::SEEDED_PRIMARY) == 0 ? 0 : 1, NULL, 0x100);
				}
			}
		}
	}
}

void CreateHouses(GenerationContext& genContext, GenerationArguments& genArguments){
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["tileset"]))){
		int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
		for (int i = 0; i < rooms; i++)
		{
			int x, y;
			auto w_stats = std::get<std::vector<int>>(genArguments["width"]);
			auto h_stats = std::get<std::vector<int>>(genArguments["height"]);
			int w = Random(w_stats[0], w_stats[1], RandomCategories::SEEDED_PRIMARY);
			int h = Random(h_stats[0], h_stats[1], RandomCategories::SEEDED_PRIMARY);
			x = Random(2, gMap.width - w - 1, RandomCategories::SEEDED_PRIMARY);
			y = Random(2, gMap.length - h - 1, RandomCategories::SEEDED_PRIMARY);
			PaintRectangle(38, x - 1, y - 1, w + 2, h + 2);
			PaintRectangle(39, x, y, w, h);
			ChangeMap(x + w, y + h - 1, 39);
			ChangeMap(x - 1, y + h - 1, 39);
		}
	}
}

void CreateClouds(GenerationContext& genContext, GenerationArguments& genArguments){
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["tileset"]))){
		int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
		for (int i = 0; i < rooms; i++)
		{
			int x, y;
			int w = 4;
			int h = 2;
			do
			{
				x = Random(2, gMap.width - w - 1, RandomCategories::SEEDED_PRIMARY);
				y = Random(2, gMap.length - h - 1, RandomCategories::SEEDED_PRIMARY);
			} while (!IsEmpty(x, y));
			ChangeMap(x, y, 60);
			ChangeMap(x + 1, y, 61);
			ChangeMap(x + 2, y, 62);
			ChangeMap(x + 3, y, 63);
			ChangeMap(x, y + 1, 76);
			ChangeMap(x + 1, y + 1, 77);
			ChangeMap(x + 2, y + 1, 78);
			ChangeMap(x + 3, y + 1, 79);
		}
	}
}

void CreateSpaceCells(GenerationContext& genContext, GenerationArguments& genArguments){
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["tileset"]))){
		int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
		for (int i = 0; i < rooms; i++)
		{
			int x, y;
			auto w_stats = std::get<std::vector<int>>(genArguments["width"]);
			auto h_stats = std::get<std::vector<int>>(genArguments["height"]);
			int w = Random(w_stats[0], w_stats[1], RandomCategories::SEEDED_PRIMARY);
			int h = Random(h_stats[0], h_stats[1], RandomCategories::SEEDED_PRIMARY);
			do
			{
				x = Random(2, gMap.width - w - 1, RandomCategories::SEEDED_PRIMARY);
				y = Random(2, gMap.length - h - 1, RandomCategories::SEEDED_PRIMARY);
			} while (IsEmpty(x, y));
			PaintRectangle(70, x - 1, y - 1, w + 2, h + 2);
			PaintRectangle(0, x, y, w, h);
			ChangeMap(x + w, y + h - 1, 1);
			ChangeMap(x - 1, y + h - 1, 1);
		}
	}
}

void SpawnPlayer(GenerationContext& genContext, GenerationArguments& genArguments){
	do
	{
		genContext.initialPlayerPos.x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
		genContext.initialPlayerPos.y = Random(0, gMap.length / (genContext.tileset == GetTileset(std::get<std::string>(genArguments["rising_water_tileset"])) ? std::get<int>(genArguments["rising_water_ratio"]) : 1), RandomCategories::SEEDED_PRIMARY);
	} while (!HasSurface(genContext.initialPlayerPos.x, genContext.initialPlayerPos.y));
	genContext.initialPlayerPos.x *= 0x2000;
	genContext.initialPlayerPos.y *= 0x2000;
	if (genContext.newStage)
		SetNpChar(18, genContext.initialPlayerPos.x, genContext.initialPlayerPos.y, 0, 0, 0, NULL, 0x100, 22, 0x0000);
	if (CheckFlagTrait(FlagTraits::STAGE_COMPLETE_AMMO_BOOST) && genContext.newStage && gMC.depth > 1){
		auto weapons = GetFlagTraitData<std::vector<int>>(FlagTraits::STAGE_COMPLETE_AMMO_BOOST, "weapons");
		auto ammos = GetFlagTraitData<std::vector<int>>(FlagTraits::STAGE_COMPLETE_AMMO_BOOST, "ammo");
		if (weapons.size() == ammos.size()){
			for (const auto&[weapon, ammo] : std::views::zip(weapons, ammos)){
				AddArmsData(weapon, ammo);
			}
		}
	}
}

void SpawnAllies(GenerationContext& genContext, GenerationArguments& genArguments){
	if (CheckFlagTrait(FlagTraits::CURLY_SPAWN) && !GetNPCFlag(GetFlagTraitData<int>(FlagTraits::REVIVAL_FROM_CURLY, "mark_flag", Flags::BADGE_OF_LOVE_REVIVE_PROC)))
		SetNpChar(320, gMC.x, gMC.y, 0, 0, 0, NULL, 0x100, 0, NPC_IGNORE_SOLIDITY);
	if (CheckFlagTrait(FlagTraits::LITTLE_MAN_SPAWN))
	{
		int amount = GetFlagTraitData<int>(FlagTraits::LITTLE_MAN_SPAWN, "amount");
		for (int i = 0; i < amount; i++)
			SetNpChar(180, 0, 0, 0, 0, 2, NULL, 0x100);
	}

	for (const auto&[fl, npcIDs] : GetActualFlagTraitData<std::vector<int>>(FlagTraits::MEDAL_SPAWN, "ids")){
		for (const auto& npcID: npcIDs){
			if (CountNpCharCode(npcID) == 0)
				SetNpChar(npcID, gMC.x, gMC.y, 0, 0, 0, NULL, 0x100, 0, NPC_IGNORE_SOLIDITY, 0);
		}
	}
}

void CreateBoss(GenerationContext& genContext, GenerationArguments& genArguments){
	if (gMC.depth < genContext.doctorStage){
		if (gMC.depth > 1 && gMC.depth % genContext.bossRate == 0)
		{
			int x, y, x1, y1;
			auto params = std::get<std::vector<int>>(genArguments["room_size"]);
			if (gMC.difficulty > DIFFICULTY_REINFORCED){
				params = std::get<std::vector<int>>(genArguments["room_size_hardmode"]);
			}
			int w = params[0];
			int h = params[1];
			do
			{
				x = Random(3, gMap.width - w - 1, RandomCategories::SEEDED_NPCS);
				y = Random(3, gMap.length - h - 1, RandomCategories::SEEDED_NPCS);
			} while (Distance(genContext.initialPlayerPos.x / 0x2000, genContext.initialPlayerPos.y / 0x2000, x, y) < std::get<int>(genArguments["spawn_distance"]));
			if (!ChallengeEnabled("no_breaking"))
				PaintRectangle(41, x - 1, y - 1, w + 2, h + 2);
			PaintRectangle(68, x, y, w, h);
			genContext.bossRect = {x, y, x + w, y + h};
			x1 = x + w / 2;
			y1 = y + h - 2;
			int bossID;
			bossID = bossNPCsList[Random(0, bossNPCsList.size()-1, RandomCategories::SEEDED_NPCS)];
			if (!GetNPCFlag(Flags::LEVEL_BOSS_DEFEATED))
				SetNpChar(bossID, x1 * 0x2000, y1 * 0x2000 + (bossID == 44 ? 0x200 * 8 : 0), 0, 0, gMC.direct, NULL, 0x100, 7000,
						NPC_SOLID_SOFT | NPC_SHOOTABLE | NPC_SHOW_DAMAGE, Flags::LEVEL_BOSS_DEFEATED);
			else
				SetNpChar(18, x1 * 0x2000, y1 * 0x2000, 0, 0, 0, NULL, 0, 99, NPC_INTERACTABLE);
		}
	}
}

void CreateChains(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["tileset_with_boost"])))
	{
		rooms *= std::get<int>(genArguments["boost_amount"]);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["blacklist_tilesets"])){
		if (genContext.tileset == GetTileset(tileset)){
			rooms = 0;
		}
	}
	for (int l = 0; l < rooms; l++)
	{
		int x, y;
		do
		{
			x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (!HasCeiling(x, y));
		ReplaceMap(x, y, 0, 43);
		do
		{
			y += 1;
			ReplaceMap(x, y, 0, 44);
		} while (IsEmpty(x, y + 1));
		ReplaceMap(x, y, 44, 45);
	}
}

void CreateCeilings(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["tileset_with_boost"])))
	{
		rooms *= std::get<int>(genArguments["boost_amount"]);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["tilesets_with_reduction"])){
		if (genContext.tileset == GetTileset(tileset)){
			rooms *= std::get<float>(genArguments["reduction_amount"]);
		}
	}
	for (int l = 0; l < rooms; l++)
	{
		int x, y;
		do
		{
			x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (!HasCeiling(x, y));
		ReplaceMap(x, y, 0, 57);
		while (HasCeiling(x - 1, y))
		{
			x -= 1;
		}
		while (HasCeiling(x + 1, y))
		{
			x += 1;
			ReplaceMap(x, y, 0, 57);
		}
	}
}

void CreatePlatforms(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["tileset_with_boost"])))
	{
		rooms *= std::get<int>(genArguments["boost_amount"]);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["tilesets_with_reduction"])){
		if (genContext.tileset == GetTileset(tileset)){
			rooms *= std::get<float>(genArguments["reduction_amount"]);
		}
	}

	auto platformPlace = [](int x, int y, int xd, int xdd = 0){
		if (IsEmpty(x + xd, y) && GetAttribute(x + y * gMap.width) != 65)
			SetNpChar(100, (x+xdd) * 0x2000, y * 0x2000, 0, 0, 0, NULL, 0x10);
	};

	for (int l = 0; l < rooms; l++)
	{
		int x, y, xd;
		do
		{
			x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (!HasWall(x, y));
		bool rightWall = GetAttribute(x + 1, y) == 0x40;
		if (rightWall)
		{
			xd = 1;
		}
		else
		{
			xd = -1;
		}
		platformPlace(x, y, xd);
		while (!HasWall(x + xd, y))
		{
			x += xd;
			platformPlace(x, y, xd);
		}
		platformPlace(x, y, xd, xd);
	}
}

void CreateWalls(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
	if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["tileset_with_boost"])))
	{
		rooms *= std::get<int>(genArguments["boost_amount"]);
	}
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["tilesets_with_reduction"])){
		if (genContext.tileset == GetTileset(tileset)){
			rooms *= std::get<float>(genArguments["reduction_amount"]);
		}
	}
	for (int l = 0; l < rooms; l++)
	{
		int x, y;
		do
		{
			x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (!IsEmpty(x, y));
		auto radius = std::get<std::vector<int>>(genArguments["radius"]);
		int r = Random(radius[0], radius[1], RandomCategories::SEEDED_PRIMARY);
		ReplaceEllipse(0, 68, x, y, r);
	}
}

void CreateAlternateTiles(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = Random(std::get<int>(genArguments["min1"]), std::get<int>(genArguments["max1"]), RandomCategories::SEEDED_PRIMARY);
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["tilesets_with_reduction"])){
		if (genContext.tileset == GetTileset(tileset)){
			rooms *= std::get<float>(genArguments["reduction_amount"]);
		}
	}
	for (int l = 0; l < rooms; l++)
	{
		int x, y;
		do
		{
			x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (IsEmpty(x, y));
		auto params = std::get<std::vector<int>>(genArguments["radius1"]);
		int r = Random(params[0], params[1], RandomCategories::SEEDED_PRIMARY);
		ReplaceEllipse(1, 69, x, y, r);
	}

	rooms = Random(std::get<int>(genArguments["min2"]), std::get<int>(genArguments["max2"]), RandomCategories::SEEDED_PRIMARY);
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["tilesets_with_reduction"])){
		if (genContext.tileset == GetTileset(tileset)){
			rooms *= std::get<float>(genArguments["reduction_amount"]);
		}
	}
	for (int l = 0; l < rooms; l++)
	{
		int x, y;
		do
		{
			x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (IsEmpty(x, y));
		auto width = std::get<std::vector<int>>(genArguments["width2"]);
		auto height = std::get<std::vector<int>>(genArguments["height2"]);
		int w = Random(width[0], width[1], RandomCategories::SEEDED_PRIMARY);
		int h = Random(height[0], height[1], RandomCategories::SEEDED_PRIMARY);
		ReplaceRectangle(32, 34, x, y, w, h);
		ReplaceRectangle(33, 35, x, y, w, h);
		ReplaceRectangle(48, 50, x, y, w, h);
		ReplaceRectangle(49, 51, x, y, w, h);
	}

	rooms = Random(std::get<int>(genArguments["min3"]), std::get<int>(genArguments["max3"]), RandomCategories::SEEDED_PRIMARY);
	for (auto tileset: std::get<std::vector<std::string>>(genArguments["tilesets_with_reduction"])){
		if (genContext.tileset == GetTileset(tileset)){
			rooms *= std::get<float>(genArguments["reduction_amount"]);
		}
	}
	for (int l = 0; l < rooms; l++)
	{
		int x, y;
		do
		{
			x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (IsEmpty(x, y));
		auto params = std::get<std::vector<int>>(genArguments["radius3"]);
		int r = Random(params[0], params[1], RandomCategories::SEEDED_PRIMARY);
		ReplaceEllipse(1, 70, x, y, r);
		ReplaceEllipse(69, 70, x, y, r);
	}
}

void CreateDecos(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = Random(std::get<int>(genArguments["min"]), std::get<int>(genArguments["max"]), RandomCategories::SEEDED_PRIMARY);
	for (int l = 0; l < rooms; l++)
	{
		int x, y;
		do
		{
			x = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			y = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (!(HasSurface(x, y)));
		auto params = std::get<std::vector<int>>(genArguments["deco_list"]);
		int roll = params[Random(0, params.size()-1, RandomCategories::SEEDED_PRIMARY)];
		ChangeMap(x, y, roll);
		if (genContext.tileset == GetTileset(std::get<std::string>(genArguments["tileset_with_bubbles"])) 
			&& Random(0, std::get<int>(genArguments["bubbles_ratio"]), RandomCategories::SEEDED_PRIMARY) == 0)
		{
			SetNpChar(281, x * 0x2000, y * 0x2000, 0, 0, 0, NULL, 0x100);
		}
	}
}

void CreateTutorialSign(GenerationContext& genContext, GenerationArguments& genArguments){
	if (gMC.depth < std::get<int>(genArguments["cutoff"]))
	{
		int x1, y1;
		do
		{
			do
			{
				x1 = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
				y1 = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
			} while (Distance(x1, y1, genContext.initialPlayerPos.x / 0x2000, genContext.initialPlayerPos.y / 0x2000) > std::get<int>(genArguments["spawn_distance"]));
		} while (!(HasSurface(x1, y1) && (GetNpCharHere(x1, y1) != 17) && (GetNpCharHere(x1, y1) != 16) && (GetNpCharHere(x1, y1) != 32)) || IntersectCheck(genContext.bossRect, x1, y1 + 1));
		SetNpChar(37, x1 * 0x2000, y1 * 0x2000, 0, 0, 0, NULL, 0, 
			genContext.legacyRNG ? 69 : 70 + gMC.depth, NPC_INTERACTABLE);
	}
}

void CreateExitDoor(GenerationContext& genContext, GenerationArguments& genArguments){
	if ((gMC.depth == 1 || (gMC.depth % genContext.bossRate != 0)))
	{
		// create a door to next randomized stage
		do
		{
			doorX = Random(0, gMap.width, RandomCategories::SEEDED_NPCS);
			doorY = Random(0, gMap.length, RandomCategories::SEEDED_NPCS);
		} while (!(HasSurface(doorX, doorY) && IsEmpty(doorX, doorY - 1) && GetNpCharHere(doorX, doorY) == -1));
		if (gMC.depth == genContext.doctorStage - 1)
		{
			SetNPCFlag(Flags::LAST_LEVEL_CH1_DOOR);
		}
		int shadowDepth = 9 + genContext.doctorStage;
		if (ChallengeEnabled("fast_run")){
			shadowDepth = 5 + genContext.doctorStage;
		}
		if (gMC.depth == shadowDepth){
			SetNPCFlag(Flags::LAST_LEVEL_CH2_DOOR);
		}
		SetNpChar(18, doorX * 0x2000, doorY * 0x2000, 0, 0, 0, NULL, 0, ChallengeEnabled("full_clear") ? 116 : 99, NPC_INTERACTABLE);
	}
	if (gMC.depth > genContext.doctorStage && gMC.depth % genContext.bossRate == 0){
		do
		{
			doorX = Random(0, gMap.width, RandomCategories::SEEDED_NPCS);
			doorY = Random(0, gMap.length, RandomCategories::SEEDED_NPCS);
		} while (!(HasSurface(doorX, doorY) && IsEmpty(doorX, doorY - 1) && GetNpCharHere(doorX, doorY) == -1));
		int mapBossID = Random(0, 5);
		for (int i = 0; i < 6; i++){
			CutNPCFlag(Flags::CH2_BOSS_DOOR_BEGIN + i);
		}
		SetNPCFlag(Flags::CH2_BOSS_DOOR_BEGIN + mapBossID);
		SetNpChar(22, doorX * 0x2000, doorY * 0x2000, 0, 0, 0, NULL, 0, ChallengeEnabled("full_clear") ? 91 : 90, NPC_INTERACTABLE);
		SetNpChar(0, doorX * 0x2000, (doorY+1) * 0x2000, 0, 0, 0, NULL, 0, 7000, 0);
		SetNpChar(23, doorX * 0x2000, (doorY-1) * 0x2000, 0, 0, 0, NULL, 0);
	}
}

void HandleMiracles(GenerationContext& genContext, GenerationArguments& genArguments){
	auto miracleRatio = std::get<int>(genArguments["miracle_ratio"]);
	if (gMC.depth == gMC.miracleGenerated * miracleRatio + 1 && (gMC.depth != 1))
	{
		int doorX, doorY;
		do
		{
			doorX = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			doorY = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (!(HasSurface(doorX, doorY) && IsEmpty(doorX, doorY - 1) && GetNpCharHere(doorX, doorY) == -1 && !IntersectCheck(genContext.bossRect, doorX, doorY)));
		SetNpChar(20, doorX * 0x2000, doorY * 0x2000, 0, 0, 2, NULL, 0, 310, NPC_INTERACTABLE);
		PaintLine(41, doorX - 2, doorY + 1, doorX + 1, doorY + 1);
	}

	if (gMC.miracleDepth == 0)
		gMC.miracleDepth = gMC.miracleGenerated * miracleRatio + 1 + Random(0, miracleRatio-1, RandomCategories::SEEDED_PRIMARY);
	if (gMC.miracleDepth == genContext.doctorStage)
		gMC.miracleDepth = genContext.doctorStage - 1;
	if (gMC.depth == gMC.miracleDepth)
	{
		if (genContext.newStage)
			CutNPCFlag(940);
		gMC.miracleDepth = 0;
		int doorX, doorY;
		do
		{
			doorX = Random(0, gMap.width, RandomCategories::SEEDED_PRIMARY);
			doorY = Random(0, gMap.length, RandomCategories::SEEDED_PRIMARY);
		} while (!(HasSurface(doorX, doorY) && IsEmpty(doorX, doorY - 1) && GetNpCharHere(doorX, doorY) == -1 && !IntersectCheck(genContext.bossRect, doorX, doorY)));
		SetNpChar(253, doorX * 0x2000, doorY * 0x2000, 0, 0, 0, NULL, 0, 304, NPC_INTERACTABLE, 940);
		gMC.miracleGenerated++;
	}
}

void HandleHell(GenerationContext& genContext, GenerationArguments& genArguments){
	if (gMC.hell)
	{
		SetNpChar(17, genContext.initialPlayerPos.x, genContext.initialPlayerPos.y, 0, 0, 0, NULL, 0x100, 17, 0x2000);
		SetNpChar(292, genContext.initialPlayerPos.x, genContext.initialPlayerPos.y, 0, 0, 0, NULL, 0x100, 17, 0x2000);
	}
}

void CreateStars(GenerationContext& genContext, GenerationArguments& genArguments){
	int rooms = std::get<int>(genArguments["star_count"]);
	if (gMC.hell)
		rooms += std::get<int>(genArguments["hell_boost"]);
	for (int l = 0; l < rooms; l++)
	{
		int x, y;
		x = 0;
		y = 0;
		int x1, y1;
		do
		{
			x1 = Random(0, gMap.width, RandomCategories::SEEDED_NPCS);
			y1 = Random(0, gMap.length, RandomCategories::SEEDED_NPCS);
			if (Distance(x, y, x1, y1) > std::get<int>(genArguments["distance"]))
			{
				x = x1;
				y = y1;
			}
		} while (!((IsEmpty(x, y) && (GetNpCharHere(x, y) == -1)) || (GetTile(x, y) == 30 && gMC.hell == false)));
		if (!GetNPCFlag(Flags::STAR_SAVE_FLAG_BEGIN + l))
			SetNpChar(86, x * 0x2000, y * 0x2000, 0, 0, 0, NULL, 0, Flags::STAR_SAVE_FLAG_BEGIN + l, 0);
	}
}

void RegenerateMapEdits(GenerationContext& genContext, GenerationArguments& genArguments){
	if (!genContext.newStage)
	{
		for (auto entry : mapModifications){
			auto coords = entry.first;
			auto tile = entry.second;
			ChangeMap(coords.first, coords.second, tile);
		}
	} else {
		mapModifications.clear();
	}
}

bool CheckSpawnCondition(NPC_SPAWN_CONFIG &spawnInfo, int x, int y){
	bool successfulSpawn = spawnInfo.logicType;
	bool conditionTriggered = false;
	if (spawnInfo.requiresSurface){
		conditionTriggered = true;
		if (spawnInfo.logicType)
			successfulSpawn = successfulSpawn && HasSurface(x, y);
		else {
			if (!successfulSpawn)
				successfulSpawn = HasSurface(x, y);
		}
	}
	if (spawnInfo.requiresCeiling){
		conditionTriggered = true;
		if (spawnInfo.logicType)
			successfulSpawn = successfulSpawn && HasCeiling(x, y);
		else {
			if (!successfulSpawn)
				successfulSpawn = HasCeiling(x, y);
		}
	}
	if (spawnInfo.requiresEmptySpace){
		conditionTriggered = true;
		if (spawnInfo.logicType)
			successfulSpawn = successfulSpawn && IsEmpty(x, y);
		else {
			if (!successfulSpawn)
				successfulSpawn = IsEmpty(x, y);
		}
	}
	if (!spawnInfo.canOverlap){
		conditionTriggered = true;
		if (spawnInfo.logicType)
			successfulSpawn = successfulSpawn && !GetNpCharHere(x, y) != spawnInfo.npcCode;
		else {
			if (!successfulSpawn)
				successfulSpawn = GetNpCharHere(x, y) != spawnInfo.npcCode;
		}
	}
	if (spawnInfo.requiresNoEmptySpace){
		conditionTriggered = true;
		if (spawnInfo.logicType)
			successfulSpawn = successfulSpawn && !IsEmpty(x, y);
		else {
			if (!successfulSpawn)
				successfulSpawn = !IsEmpty(x, y);
		}
	}
	if (spawnInfo.requiresWall){
		conditionTriggered = true;
		if (spawnInfo.logicType)
			successfulSpawn = successfulSpawn && HasWall(x, y);
		else {
			if (!successfulSpawn)
				successfulSpawn = HasWall(x, y);
		}
	}
	if (spawnInfo.onTopOfMob != 0){
		conditionTriggered = true;
		if (spawnInfo.logicType)
			successfulSpawn = successfulSpawn && (GetNpCharHere(x, y) == spawnInfo.onTopOfMob);
		else {
			if (!successfulSpawn)
				successfulSpawn = (GetNpCharHere(x, y) == spawnInfo.onTopOfMob);
		}
	}
	return !conditionTriggered ? false : successfulSpawn;
}

void ProcessIndividualNPCSpawning(const std::string& name){
	if (npcSpawning.find(name) != npcSpawning.end())
	{
		auto spawnInfo = npcSpawning[name];
		if ((spawnInfo.gimmickRequirement == -1 || genContext.gimmick == spawnInfo.gimmickRequirement) &&
			(spawnInfo.tilesetRequirement == -1 || genContext.tileset == spawnInfo.tilesetRequirement) &&
			(gMC.depth < genContext.doctorStage || spawnInfo.countFromDoctor))
		{
			int baseDepth = spawnInfo.countFromDoctor ? genContext.doctorStage : 0;
			if (gMC.depth > baseDepth + (spawnInfo.minDepth - 1) && gMC.depth <= baseDepth + (spawnInfo.maxDepth))
			{
				int rooms;
				if (spawnInfo.minNpcs == spawnInfo.maxNpcs){
					rooms = spawnInfo.minNpcs;
					Random(0, 1, RandomCategories::SEEDED_PRIMARY);
				} else {
					rooms = Random(spawnInfo.minNpcs, spawnInfo.maxNpcs, RandomCategories::SEEDED_PRIMARY);
				}
				rooms += spawnInfo.extraNpcs * (gMC.depth - (baseDepth + spawnInfo.minDepth));
				for (auto &[stage, modifier] : spawnInfo.countModifiers)
				{
					if (genContext.tileset == stage)
						rooms *= modifier;
				}
				if (spawnInfo.mobModifier) rooms *= genContext.npcCount;
				for (int l = 0; l < rooms; l++)
				{
					int x, y;
					do
					{
						do
						{
							x = Random(0, gMap.width, RandomCategories::SEEDED_NPCS);
							y = Random(0, gMap.length, RandomCategories::SEEDED_NPCS);
						} while (Distance(x, y, genContext.initialPlayerPos.x / 0x2000, genContext.initialPlayerPos.y / 0x2000) < spawnInfo.distanceFromPlayer);
					} while (!(CheckSpawnCondition(spawnInfo, x, y)) || IntersectCheck(genContext.bossRect, x, y));
					int direction = ((!spawnInfo.wallDirection || GetTile(x + 1, y) == 1)) ? 0 : 2;
					if (spawnInfo.bothCeilingAndSurface)
					{
						direction = HasSurface(x, y) ? 0 : 2;
					}
					if (spawnInfo.randomDirection){
						direction = Random(0, 1, RandomCategories::SEEDED_NPCS) ? 0 : 1;
					}
					if (spawnInfo.destroyTile)
					{
						y += 1;
						ChangeMap(x, y, 0);
					}
					if (spawnInfo.direct != -1)
						direction = spawnInfo.direct;
					SetNpChar(spawnInfo.npcCode, (GetTile(x + 1, y) == 1 ? spawnInfo.sideOffset : -spawnInfo.sideOffset) + x * 0x2000, y* 0x2000 + spawnInfo.verticalOffset, 0, 0, direction, NULL, 0x100);
				}
			}
		}
	}
}

void ProcessNPCSpawning(GenerationContext& genContext, GenerationArguments& genArguments){
	for (auto &[name, spawnInfo]: npcSpawning){
		if ((spawnInfo.standardOrder) != (genContext.tileset == GetTileset(std::get<std::string>(genArguments["blacklist_tileset"]))))
			ProcessIndividualNPCSpawning(name);
	}
}

std::map<std::string, GenerationFunction> generationStepDefintions {
	{"rectangles", CreateRectangles},
	{"circles", CreateCircles},
	{"mid_rooms", CreateMediumRooms},
	{"big_rooms", CreateBigRooms},
	{"corridors", CreateCorridors},
	{"sand_patches", CreateSandPatches},
	{"labyrinth_mazes", CreateMazes},
	{"dungeon_tunnels", CreateDungeonTunnels},
	{"sky_borders", CreateSkyBorders},
	{"polishing", PolishStage},
	{"spikes", CreateSpikes},
	{"chests", CreateChests},
	{"life_capsules", CreateLifeCapsules},
	{"refill_stations", CreateRefills},
	{"save_point", CreateSavePoint},
	{"water", GenerateWater},
	{"mimiga_houses", CreateHouses},
	{"headman_clouds", CreateClouds},
	{"space_cells", CreateSpaceCells},
	{"player", SpawnPlayer},
	{"allies", SpawnAllies},
	{"boss", CreateBoss},
	{"npc_spawning", ProcessNPCSpawning},
	{"chains", CreateChains},
	{"ceilings", CreateCeilings},
	{"platforms", CreatePlatforms},
	{"walls", CreateWalls},
	{"alternate_tiles", CreateAlternateTiles},
	{"decorations", CreateDecos},
	{"exit_door", CreateExitDoor},
	{"miracle_matter", HandleMiracles},
	{"hell_objects", HandleHell},
	{"tutorial_sign", CreateTutorialSign},
	{"stars", CreateStars},
	{"map_edits", RegenerateMapEdits}
};

bool TransferStage(int no, int w, int x, int y)
{
	ReloadConfig();
	LoadInterface(ChallengeEnabled("legacyUI"));
	auto timing = std::chrono::steady_clock::now();
	genContext = {};
	std::string path;
	std::string path_dir;
	bool bError = false;
	genContext.newStage = w != 0;
	genContext.doctorStage = 10;
	genContext.bossRate = 4;
	genContext.bossRect = {0, 0, 0, 0};
	genContext.legacyRNG = IsKeyFile("RNG.json");
	if (ChallengeEnabled("fast_run"))
	{
		genContext.doctorStage = 6;
		genContext.bossRate = 2;
	}
	if (ChallengeEnabled("secondCampaign"))
	{
		genContext.doctorStage = 0;
	}

	// Move character
	SetMyCharPosition(x * 0x10 * 0x200, y * 0x10 * 0x200);

	if (!ChallengeEnabled("no_map"))
		SetMapping(no);

	if (gMC.depth == 0)
	{
		time_count = 0;
		genContext.seed = GenerateSeed();

		// use new PCG generators from post-1.42
		pcg32_srandom_r(&seed_primary, genContext.seed, 3);
		pcg32_srandom_r(&seed_chests, genContext.seed, 5);
		pcg32_srandom_r(&seed_npcs, genContext.seed, 7);

		gMC.seed = genContext.seed;

		LogSeed(genContext.seed);

		SelectCharacter(gDaily ?
					 CharacterCode(genContext.seed % MAX_CHARACTERS) :
					 gJConfig.value("character", ""));
		
		std::vector<int> blacklist = BlacklistedItems();
		gAvailableIDs.clear();
		for (int i = 0; i < ARRAY_SIZE(chestDrops); i++)
		{
			if (std::find(std::begin(blacklist), std::end(blacklist), chestDrops[i]) == std::end(blacklist))
			{
				gAvailableIDs.push_back(chestDrops[i]);
			}
		}
	}
	if (genContext.legacyRNG){
		memcpy(_Q, Q, sizeof(_Q));
		memcpy(_Q2, Q2, sizeof(_Q2));
		memcpy(_Q3, Q3, sizeof(_Q3));
		_c = c;
		_c2 = c2;
		_c3 = c3;
		_i1 = i1;
		_i2 = i2;
		_i3 = i3;
	} else {
		memcpy(&seed_primary_clone, &seed_primary, sizeof(seed_primary_clone));
		memcpy(&seed_chests_clone, &seed_chests, sizeof(seed_chests_clone));
		memcpy(&seed_npcs_clone, &seed_npcs, sizeof(seed_npcs_clone));
	}
	WriteGameLog("Level initial setup in ", since(timing).count(), " ms, seed: ", genContext.seed, ".");
	timing = std::chrono::steady_clock::now();

	if (no == gGeneratedStage)
	{
		InitNpChar();
		if (genContext.newStage)
		{
			gMC.outerWallInit = false;
			CutNPCFlag(Flags::TOROKO_BLEEDING_OUT);
		}

		if (LoadMapAssets())
			return false;

		ReadyMapName(gTMT[no].name);
		StartTextScript(w);
		ConfigureMap();
		std::string_view lastStage;
		gStageNo = no;

		try {
			for (auto generationStage : stageGenerationOrder){
				if (generationStepDefintions.find(generationStage) != generationStepDefintions.end() &&
					stageGenerationMetadata.find(generationStage) != stageGenerationMetadata.end()){
					lastStage = generationStage;
					if (ChallengeEnabled("stage_debug"))
						WriteGameLog("Current generation stage: ", lastStage);
					generationStepDefintions[generationStage]
						(genContext, stageGenerationMetadata[generationStage]);
				}
			}
		} catch (std::bad_variant_access error){
			WriteGameLog("At stage ", lastStage, " the exception with metadata happened: ", error.what());
		}
		if (genContext.newStage)
			SetMyCharPosition(genContext.initialPlayerPos.x, genContext.initialPlayerPos.y);

		SetFrameMyChar();
		ClearBullet();
		InitStar();
		InitCaret();
		ClearValueView();
		ResetQuake();
		InitBossChar(gTMT[no].boss_no);
		ResetFlash();
		if (CheckFlagTrait(FlagTraits::WHIMSICAL_STAR))
			SetBullet(Bullets::WHIMSICAL_STAR, gMC.x, gMC.y + 0x2500, 1);
		gNameFlag = true;
		gNameTimer = 0;
		if (IsItSecondCampaign() && !CheckItem(34))
			AddItemData(34);

		WriteGameLog("Level generated in ", since(timing).count(), " ms.");
	}
	else
	{
		gMC.gimmick = 0;
		gMC.tileset = 0;
		// Get path
		path_dir = "Stage";

		// Load tileset
		path = path_dir + '/' + "Prt" + gTMT[no].parts;
		if (!ReloadBitmap_File(path.c_str(), SURFACE_ID_LEVEL_TILESET))
			bError = true;

		path = path_dir + '/' + gTMT[no].parts + ".pxa";
		if (!LoadAttributeData(path.c_str()))
			bError = true;

		// Load tilemap
		path = path_dir + '/' + gTMT[no].map + ".pxm";
		if (!LoadMapData2(path.c_str()))
			bError = true;

		// Load NPCs
		path = path_dir + '/' + gTMT[no].map + ".pxe";
		if (!LoadEvent(path.c_str()))
			bError = true;

		// Load script
		path = path_dir + '/' + gTMT[no].map + ".tsc";
		if (!LoadTextScript_Stage(path.c_str()))
			bError = true;

		// Load background
		path = gTMT[no].back;
		if (!InitBack(path.c_str(), gTMT[no].bkType))
			bError = true;

		// Get path
		path_dir = "Npc";

		// Load NPC sprite sheets
		path = path_dir + "/Npc" + gTMT[no].npc;
		if (!ReloadBitmap_File(path.c_str(), SURFACE_ID_LEVEL_SPRITESET_1))
			bError = true;

		path = path_dir + "/Npc" + gTMT[no].boss;
		if (!ReloadBitmap_File(path.c_str(), SURFACE_ID_LEVEL_SPRITESET_2))
			bError = true;

		if (bError)
			return false;

		if (no == 65)
		{
			gMC.depth = genContext.doctorStage;
		}
		if (no == 87)
		{
			if (ChallengeEnabled("secondCampaign"))
				gMC.depth += 1;
			else
				gMC.depth = genContext.doctorStage*2;
		}
		gMC.a = 0;
		SpawnAllies(genContext, stageGenerationMetadata["allies"]);

		// Load map name
		ReadyMapName(gTMT[no].name);

		StartTextScript(w);
		SetFrameMyChar();
		ClearBullet();
		InitCaret();
		InitStar();
		ClearValueView();
		ResetQuake();
		InitBossChar(gTMT[no].boss_no);
		ResetFlash();
		if (CheckFlagTrait(FlagTraits::WHIMSICAL_STAR))
			SetBullet(Bullets::WHIMSICAL_STAR, gMC.x, gMC.y + 0x2500, 1);
		gStageNo = no;
	}

	return true;
}

enum
{
	MUSIC_TYPE_ORGANYA,
	MUSIC_TYPE_OTHER
};

//Music
static const struct
{
	std::string intro_file_path;
	std::string loop_file_path;
	int type;
	bool loop; // Only applicable to non-Organya songs
} music_table[] = {
	{"Resource/ORG/XXXX", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Kero1", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Kero2", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Gameover", "", MUSIC_TYPE_ORGANYA, false},
	{"Resource/ORG/MDown1", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/MDown0", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/MDown2", "", MUSIC_TYPE_ORGANYA, true}, //sand zone
	{"Resource/ORG/cool thing", "", MUSIC_TYPE_ORGANYA, true}, //boss fight
	{"Resource/ORG/Kero3", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Lol1", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Fanfale1", "", MUSIC_TYPE_ORGANYA, false}, //item get
	{"Resource/ORG/Lol2", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Temporal", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/electronic cavern", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/probably", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Fanfale3", "", MUSIC_TYPE_ORGANYA, false}, //doctor defeat fanfale
	{"Resource/ORG/Fanfale2", "", MUSIC_TYPE_ORGANYA, false},
	{"Resource/ORG/dust in the wind", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/space FIXED", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Lol4", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/man robic", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Jungle", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Watery caves2", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/EdoCastle", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Crazy la Paint0", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/pictures", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Cogent", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/MDown3", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Time", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/True Darknes", "", MUSIC_TYPE_ORGANYA, true}, //doctor's monologue
	{"Resource/ORG/lullaby", "", MUSIC_TYPE_ORGANYA, true}, //king's table
	{"Resource/ORG/Majin1", "", MUSIC_TYPE_ORGANYA, true}, //doctor p1
	{"Resource/ORG/LastBtl3", "", MUSIC_TYPE_ORGANYA, true}, //doctor p2
	{"Resource/ORG/Freak", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Untitleable", "", MUSIC_TYPE_ORGANYA, true},	 //arcadia
	{"Resource/ORG/Orbital", "", MUSIC_TYPE_ORGANYA, true},		 //core
	{"Resource/ORG/Skylands", "", MUSIC_TYPE_ORGANYA, true},		 //outer wall
	{"Resource/ORG/Toppa", "", MUSIC_TYPE_ORGANYA, true},			 //egg corridor
	{"Resource/ORG/5", "", MUSIC_TYPE_ORGANYA, true},				 //village
	{"Resource/ORG/Worry", "", MUSIC_TYPE_ORGANYA, true},			 //waterway
	{"Resource/ORG/Fatality", "", MUSIC_TYPE_ORGANYA, true},		 //hell
	{"Resource/ORG/Determination3", "", MUSIC_TYPE_ORGANYA, true}, //grasstown
	{"Resource/ORG/Ambience", "", MUSIC_TYPE_ORGANYA, true},		 //labyrinth
	{"Resource/ORG/Curly", "", MUSIC_TYPE_ORGANYA, true},			 //first cave
	{"Resource/ORG/is this donkey", "", MUSIC_TYPE_ORGANYA, true}, //plantation
	{"Resource/ORG/Change", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/jazzles", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/gestation", "", MUSIC_TYPE_ORGANYA, true}, //ice
	{"Resource/ORG/electric", "", MUSIC_TYPE_ORGANYA, true},	//laboratory
	{"Resource/ORG/repetir", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/darkened sky", "", MUSIC_TYPE_ORGANYA, true}, //rankings screen
	{"Resource/ORG/blurgo", "", MUSIC_TYPE_ORGANYA, true}, //tutorial screen
	{"Resource/ORG/StarFanfale", "", MUSIC_TYPE_ORGANYA, true}, //fanfale for 10 stars
	{"Resource/ORG/pixels", "", MUSIC_TYPE_ORGANYA, true}, //character select
	{"Resource/ORG/tinn2d duck", "", MUSIC_TYPE_ORGANYA, true}, //space
	{"Resource/ORG/Magicant", "", MUSIC_TYPE_ORGANYA, true}, //marsh
	{"Resource/ORG/bathroom", "", MUSIC_TYPE_ORGANYA, true}, //frontdoors
	{"Resource/ORG/bossang", "", MUSIC_TYPE_ORGANYA, true}, //darkest cave
	{"Resource/ORG/snowy mountain", "", MUSIC_TYPE_ORGANYA, true}, //headman
	{"Resource/ORG/crayteria", "", MUSIC_TYPE_ORGANYA, true}, //dungeon
	{"Resource/ORG/ass blaster 3000", "", MUSIC_TYPE_ORGANYA, true}, //mines
	{"Resource/ORG/vampire kiler", "", MUSIC_TYPE_ORGANYA, true}, //castle
	{"Resource/ORG/Kero4", "", MUSIC_TYPE_ORGANYA, true}, //pasture
	{"Resource/ORG/ShadowSmash", "", MUSIC_TYPE_ORGANYA, true}, //map bosses
	{"Resource/ORG/bossofdoom", "", MUSIC_TYPE_ORGANYA, true}, //final chamber
	{"Resource/ORG/Whirlwind", "", MUSIC_TYPE_ORGANYA, true}, //ballos last phase
	{"Resource/ORG/echcredits", "", MUSIC_TYPE_ORGANYA, true}, //credits
	{"Resource/ORG/forest", "", MUSIC_TYPE_ORGANYA, true}, //reefs (placeholder)
	{"Resource/ORG/The_Start", "", MUSIC_TYPE_ORGANYA, true}, //shadow caves theme
	{"Resource/ORG/Move", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Walk", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/CityOne", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/Regular_Battle_Chip_Challenge", "", MUSIC_TYPE_ORGANYA, true}, //reinforced boss fight
	{"Resource/ORG/Abstract Anomaly", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/wind bombing", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/TechnoShmeckno", "", MUSIC_TYPE_ORGANYA, true},
	{"Resource/ORG/ninety", "", MUSIC_TYPE_ORGANYA, true}, //first cave (second camp)
	{"Resource/ORG/Fragments", "", MUSIC_TYPE_ORGANYA, true}, //darkest cave (second camp)
	{"Resource/ORG/Hub", "", MUSIC_TYPE_ORGANYA, true}, //headman (second camp)
	{"Resource/ORG/Ascension Demake", "", MUSIC_TYPE_ORGANYA, true}, //pasture (second camp)
	{"Resource/ORG/battlefield 18percent slower", "", MUSIC_TYPE_ORGANYA, true}, //castle (second camp)
	{"Resource/ORG/NaviCust4 16percent slower", "", MUSIC_TYPE_ORGANYA, true}, //space (second camp)
	{"Resource/ORG/Cyber Academy", "", MUSIC_TYPE_ORGANYA, true}, //mines (second camp)
	{"Resource/ORG/CidHighwind", "", MUSIC_TYPE_ORGANYA, true}, //dungeon (second camp)
	{"Resource/ORG/Tech3", "", MUSIC_TYPE_ORGANYA, true}, //egg corridor (second camp)
	{"Resource/ORG/IHateCrypticPuzzles", "", MUSIC_TYPE_ORGANYA, true}, //sand zone (second camp)
	{"Resource/ORG/underworld", "", MUSIC_TYPE_ORGANYA, true}, //village (second camp)
	{"Resource/ORG/Gold", "", MUSIC_TYPE_ORGANYA, true}, //waterway (second camp)
	{"Resource/ORG/grean sleaves", "", MUSIC_TYPE_ORGANYA, true}, //core (second camp)
	{"Resource/ORG/i made the ocean floor org better", "", MUSIC_TYPE_ORGANYA, true}, //watery caves (second camp)
	{"Resource/ORG/the_imps_org", "", MUSIC_TYPE_ORGANYA, true}, //labyrinth (second camp)
	{"Resource/ORG/synth fortress 14percent slower", "", MUSIC_TYPE_ORGANYA, true}, //outer wall (second camp)
	{"Resource/ORG/tensioin", "", MUSIC_TYPE_ORGANYA, true}, //ice (second camp)
	{"Resource/ORG/OPERATION 15percent slower", "", MUSIC_TYPE_ORGANYA, true}, //laboratory (second camp)
	{"Resource/ORG/DEADLOCKED", "", MUSIC_TYPE_ORGANYA, true}, //hell (second camp)
	{"Resource/ORG/MoonMarsSakurabe3", "", MUSIC_TYPE_ORGANYA, true}, //grasstown (second camp)
	{"Resource/ORG/VS DUO 12percent slower", "", MUSIC_TYPE_ORGANYA, true}, //arcadia (second camp)
	{"Resource/ORG/invisible wing", "", MUSIC_TYPE_ORGANYA, true}, //frontdoors (second camp)
	{"Resource/ORG/Gesticulation", "", MUSIC_TYPE_ORGANYA, true}, //marsh (second camp)
	{"Resource/ORG/Normality", "", MUSIC_TYPE_ORGANYA, true}, //plantation (second camp)
};

std::vector<std::string> fileExtension = {"ogg", "mp3", "org"}; 

void ChangeMusic(MusicID no)
{
	if (no == (MusicID)6)
	{
		int musicIDs[] = {1, 2, 4, 5, 6, 8, 9, 11, 12, 13, 14, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 33, 45, 46, 49, 69, 70, 71, 73, 74, 75};
		bool secondCamp = (gMC.depth >= genContext.doctorStage);
		MusicID id = (MusicID)musicIDs[Random(0, ARRAY_SIZE(musicIDs) - 1, RandomCategories::SEEDED_PRIMARY)];
		no = id;
		if (Random(0, 2, RandomCategories::SEEDED_PRIMARY) == 0)
		{
			switch (gMC.tileset)
			{
			case TYPE_FIRST_CAVE:
				no = secondCamp ? MUS_FIRST_CAVE_BGM_2 : MUS_FIRST_CAVE_BGM;
				break;
			case TYPE_SAND_ZONE:
				no = secondCamp ? MUS_NOTMELTDOWN : MUS_MELTDOWN2;
				break;
			case TYPE_LABYRINTH:
				no = secondCamp ? MUS_LABYRINTH_BGM_2 : MUS_LABYRINTH_BGM;
				break;
			case TYPE_MIMIGA_VILLAGE:
				no = secondCamp ? MUS_VILLAGE_BGM_2 : MUS_VILLAGE_BGM;
				break;
			case TYPE_LIVING_WATERWAY:
				no = secondCamp ? MUS_WATERWAY_BGM_2 : MUS_WATERWAY_BGM;
				break;
			case TYPE_EGGS:
				no = secondCamp ? MUS_EGGS_BGM_2 : MUS_EGGS_BGM;
				break;
			case TYPE_GRASSTOWN:
				no = secondCamp ? MUS_GRASSTOWN_BGM_2 : MUS_GRASSTOWN_BGM;
				break;
			case TYPE_OUTER_WALL:
				no = secondCamp ? MUS_SKYLAND_BGM_2 : MUS_SKYLAND_BGM;
				break;
			case TYPE_ALMOND:
				no = secondCamp ? MUS_CORE_BGM_2 : MUS_CORE_BGM;
				break;
			case TYPE_PLANTATION:
				no = secondCamp ? MUS_PLANTATION_BGM_2 : MUS_PLANTATION_BGM;
				break;
			case TYPE_ICE:
				no = secondCamp ? MUS_ICE_BGM_2 : MUS_ICE_BGM;
				break;
			case TYPE_LABORATORY:
				no = secondCamp ? MUS_LABORATORY_BGM_2 : MUS_LABORATORY_BGM;
				break;
			case TYPE_SPACESTATION:
				no = secondCamp ? MUS_NAVICUST4 : MUS_TINN2D;
				break;
			case TYPE_MARSH:
				no = secondCamp ? MUS_MARSH_BGM_2 : MUS_MARSH_BGM;
				break;
			case TYPE_INDOORS:
				no = secondCamp ? MUS_INDOORS_BGM_2 : MUS_INDOORS_BGM;
				break;
			case TYPE_HEADMAN:
				no = secondCamp ? MUS_HEADMAN_BGM_2 : MUS_HEADMAN_BGM;
				break;
			case TYPE_DUNGEON:
				no = secondCamp ? MUS_DUNGEON_BGM_2 : MUS_DUNGEON_BGM;
				break;
			case TYPE_MESA:
				no = secondCamp ? MUS_MESA_BGM_2 : MUS_MESA_BGM;
				break;
			case TYPE_CASTLE:
				no = secondCamp ? MUS_CASTLE_BGM_2 : MUS_CASTLE_BGM;
				break;
			case TYPE_SHEEP:
				no = secondCamp ? MUS_SHEEP_BGM_2 : MUS_SHEEP_BGM;
				break;
			case TYPE_IKACHAN:
				no = secondCamp ? MUS_IKA_BGM_2 : MUS_IKA_BGM;
				break;
			}
		}
		switch (gMC.tileset)
		{
		case TYPE_HELL:
			no = secondCamp ? MUS_HELL_BGM_2 : MUS_HELL_BGM;
			break;
		case TYPE_ARCADIA:
			no = secondCamp ? MUS_ARCADIA_BGM_2 : MUS_ARCADIA_BGM;
			break;
		case TYPE_DARKCAVE:
			no = secondCamp ? MUS_DARKCAVE_BGM_2 : MUS_DARKCAVE_BGM;
			break;
		case TYPE_SHADOW:
			no = MUS_SHADOW_BGM;
			break;
		}
	}

	if (no != MUS_SILENCE && no == gMusicNo){
		Volume = 100;
		bFadeout = false;
		return;
	}

	if (ChallengeEnabled("organya_mute")){
		no = MUS_SILENCE;
	}

	// Stop and keep track of old song
	gOldPos = GetOrganyaPosition();
	gOldNo = gMusicNo;
	StopOrganyaMusic();
	if (ChallengeEnabled("pitch_randomizer")){
		pitch_mod = Random(1, 12) - 6;
	} else {
		pitch_mod = 0;
	}
#ifdef EXTRA_SOUND_FORMATS
	ExtraSound_PauseMusic();
#endif

	std::string intro_file_path;
	std::string loop_file_path;
	int musicType;
	for (auto extension: fileExtension){
		if (music_table[no].intro_file_path != "")
			intro_file_path = music_table[no].intro_file_path + "." + extension;

		if (music_table[no].loop_file_path != "")
			loop_file_path = music_table[no].loop_file_path + "." + extension;
		
		if (extension == "org" && ResourceExists((music_table[no].intro_file_path + "." + extension))){
			musicType = MUSIC_TYPE_ORGANYA;
			break;
		} else if (ResourceExists((music_table[no].intro_file_path + "." + extension)) ||
			ResourceExists((music_table[no].loop_file_path + "." + extension))){
			if (loop_file_path == ""){
				loop_file_path = intro_file_path;
			}
			musicType = MUSIC_TYPE_OTHER;
			break;
		}
	}

	switch (musicType)
	{
	case MUSIC_TYPE_ORGANYA:
		// Load 
		LoadOrganya(intro_file_path.c_str());

		// Reset position, volume, and then play the song
		ChangeOrganyaVolume(100);
		SetOrganyaPosition(0);
		PlayOrganyaMusic();

#ifdef EXTRA_SOUND_FORMATS
		ExtraSound_LoadMusic(NULL, NULL, false); // Play a null song so any current song gets pushed back to the backup slot
#endif
		break;

#ifdef EXTRA_SOUND_FORMATS
	case MUSIC_TYPE_OTHER:
		ExtraSound_LoadMusic(intro_file_path.c_str(), music_table[no].loop ? loop_file_path.c_str() : NULL, music_table[no].loop);
		ExtraSound_UnpauseMusic();

		// Play a null Organya song so focussing and refocussing the window doesn't cause the old Organya song to start playing again
		intro_file_path = music_table[0].intro_file_path + ".org";
		LoadOrganya(intro_file_path.c_str());

		ChangeOrganyaVolume(100);
		SetOrganyaPosition(0);
		PlayOrganyaMusic();
		break;
#endif
	}

	gMusicNo = no;
}

void ReCallMusic(void)
{
	std::string path;

	// Stop old song
	StopOrganyaMusic();
#ifdef EXTRA_SOUND_FORMATS
	ExtraSound_PauseMusic();
#endif

	switch (music_table[gOldNo].type)
	{
	case MUSIC_TYPE_ORGANYA:
		// Load org that was playing before
		path = music_table[gOldNo].intro_file_path + ".org";
		LoadOrganya(path.c_str());

		// Reset position, volume, and then play the song
		SetOrganyaPosition(gOldPos);
		ChangeOrganyaVolume(100);
		PlayOrganyaMusic();
		break;

#ifdef EXTRA_SOUND_FORMATS
	case MUSIC_TYPE_OTHER:
		ExtraSound_LoadPreviousMusic();
		ExtraSound_UnpauseMusic();
		break;
#endif
	}

	gMusicNo = gOldNo;
}

std::string CharacterName()
{
	for (auto names : characterNames)
	{
		if (names.first != "default")
		{
			std::string_view flagNumStr = names.first;
			int flagNum;
			std::from_chars(flagNumStr.data(), flagNumStr.data() + flagNumStr.size(), flagNum);
			if (GetNPCFlag(flagNum))
			{
				return names.second;
			}
		}
	}
	return characterNames["default"];
}

std::string TilesetName(){
	return TilesetName(gMC.tileset);
}

std::string TilesetName(char id){
	auto idString = std::string{id};
	if (IsItSecondCampaign() && stageNames.contains(idString + "_2"))
		idString += "_2";
	auto search = stageNames.find(idString);
	if (search != stageNames.end()) return search->second;
	return "";
}

char GetGimmick(std::string id){
	auto humanReadableStageID = std::find(gimmickNames.begin(), gimmickNames.end(), id);
	if (humanReadableStageID != gimmickNames.end())
	{
		return (char)(humanReadableStageID - gimmickNames.begin());
	} else if (std::stoi(id) <= gimmickNames.size())
	{
		return (char)(std::stoi(id));
	}
	return -1;
}

char GetTileset(std::string id){
	auto symbolStageID = std::find(stageSymbols.begin(), stageSymbols.end(), id);
	if (symbolStageID != stageSymbols.end())
	{
		return (*symbolStageID)[0];
	}
	else
	{
		auto humanReadableStageID = std::find_if(stageNames.begin(), stageNames.end(), [id](std::pair<std::string, std::string> p)
												 { return p.second == id; });
		if (humanReadableStageID != stageNames.end())
		{
			return humanReadableStageID->first[0];
		}
	}
	return -1;
}

std::string GimmickName(){
	if (gimmickNames.size()-1 < gMC.gimmick)
		return "";
	else return gimmickNames[gMC.gimmick];
}

void PutMappingName(void){
	if(!gNameFlag){
		return;
	}

	Rect rc_note = {0, 160, 16, 176};
	Rect rc_background = {0, 0, 180, 18};
	rc_note.left += (gMC.tileset - (gMC.tileset > 64 ? 56 : 49)) * 16;
	rc_note.right = rc_note.left + 16;
	if (gMC.tileset > 'F'){
		rc_note.left = (gMC.tileset - 71)*16;
		rc_note.right = rc_note.left + 16;
		rc_note.top += 16;
		rc_note.bottom += 16;
	}

	char music_name[100];
	if (GimmickName() != gimmickNames[0])
		sprintf(music_name, "%s %s", GimmickName().c_str(), TilesetName().c_str());
	else
		sprintf(music_name, "%s", TilesetName().c_str());

	PutBitmap3(&grcGame, PixelToScreenCoord(0), PixelToScreenCoord(WINDOW_HEIGHT - 21), &rc_background, SURFACE_ID_BLACK_OVERLAY);

	PutText(24, WINDOW_HEIGHT - 20, music_name, RGB(0xff,0xff,0xfe));
	PutBitmap3(&grcGame, PixelToScreenCoord(4), PixelToScreenCoord(WINDOW_HEIGHT - 20), &rc_note, SURFACE_ID_TEXT_BOX);

	gNameTimer++;

	if(gNameTimer > 250){
		gNameFlag = false;
		gNameTimer = 0;
	}
}

std::string CharacterCode(int id){
	switch (id)
	{
	case 1:
		return "CurlyBrac";
	case 2:
		return "KingTable";
	case 3:
		return "FluffMimi";
	case 4:
		return "Sakamoto";
	case 5:
		return "Raymundo";
	case 6:
		return "Upntscale";
	case 7:
		return "Russia";
	case 8:
		return "SaraACSS";
	case 9:
		return "ColdWorkE";
	case 10:
		return "WTFEvilQu";
	case 11:
		return "Jamark";
	case 12:
		return "Dog";
	case 13:
		return "Magician";
	case 14:
		return "LuigiGuy";
	case 15:
		return "Meme";
	case 16:
		return "God";
	case 17:
		return "AmeManga";
	case 18:
		return "SamHogan";
	case 19:
		return "LunarSha";
	default:
		return "";
	}
	
}
