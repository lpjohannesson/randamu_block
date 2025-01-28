#ifndef __STAGE_H__
#define __STAGE_H__


#include "Json.h"
#include "Profile.h"
#include "CommonDefines.h"
#include "StageGen.h"
#include <cstdint>

/// IMPORTANT ///

extern uint32_t Q[4096], c, Q2[4096], c2, Q3[4096], c3;
extern uint32_t _Q[4096], _c, _Q2[4096], _c2, _Q3[4096], _c3;
extern uint32_t i1;
extern uint32_t i2;
extern uint32_t i3;
extern uint32_t _i1, _i2, _i3;

void msvc_srand_s(uint32_t x);
uint32_t msvc_rand_s();
void msvc_srand_s2(uint32_t x);
uint32_t msvc_rand_s2();
void msvc_srand_s3(uint32_t x);
uint32_t msvc_rand_s3();

extern int MinCapsules;
extern int MaxCapsules;
extern int CapsuleLifeLimit;

extern int MinSpikes;
extern int MaxSpikes;
extern int VillageSpikeDivider;
extern int HellSpikeMultiplier;

extern int MinRefills;
extern int MaxRefills;

extern int MinChests;
extern int MaxChests;

extern int StarCount;

/// OBJECTS ///

extern int MinChains;
extern int MaxChains;

extern int VillageChainMultiplier;

extern int MinPlatforms;
extern int MaxPlatforms;

extern int VillagePlatformMultiplier;

extern int MinBG_Tiles;
extern int MaxBG_Tiles;

extern int VillageBG_TileMultiplier;

enum
{
			TYPE_FIRST_CAVE = '1',
			TYPE_SAND_ZONE = '2',
			TYPE_LABYRINTH = '3',
			TYPE_MIMIGA_VILLAGE = '4',
			TYPE_LIVING_WATERWAY = '5',
			TYPE_GRASSTOWN = '6',
			TYPE_HELL = '7',
			TYPE_OUTER_WALL = '8',
			TYPE_EGGS = '9',
			TYPE_ALMOND = 'A',
			TYPE_PLANTATION = 'B',
			TYPE_ARCADIA = 'C',
			TYPE_ICE = 'D',
			TYPE_LABORATORY = 'E',
			TYPE_SPACESTATION = 'F',
			TYPE_MARSH = 'G',
			TYPE_INDOORS = 'H',
			TYPE_DARKCAVE = 'I',
			TYPE_IKACHAN = 'J',
			TYPE_HEADMAN = 'K',
			TYPE_DUNGEON = 'L',
			TYPE_MESA = 'M',
			TYPE_CASTLE = 'N',
			TYPE_SHEEP = 'O',
			TYPE_SHADOW = 'P'
};

enum {
	DIFFICULTY_BASIC = 0,
	DIFFICULTY_HARDENED = 1,
	DIFFICULTY_REINFORCED = 2,
	DIFFICULTY_RESONANT = 3
};

enum {
	GIMMICK_GRASS = 1,
	GIMMICK_DARKNESS = 2,
	GIMMICK_VAULT = 3,
	GIMMICK_FRAGILE = 4,
	GIMMICK_BLOCKS = 5,
	GIMMICK_PUPPY = 6,
	GIMMICK_ROOMS = 7,
	GIMMICK_OPEN = 8,
	GIMMICK_DEFENDED = 9
};

struct GenerationContext {
	uint64_t seed;
	int depth;
	char tileset;
	int gimmick;
	bool newStage;
	bool legacyRNG;
	double crampiness;
	double npcCount;
	Rect bossRect;
	int bossRate;
	int doctorStage;
	struct {
		int x; int y;
	} initialPlayerPos;
};

extern std::map<std::string, GenerationFunction> generationStepDefintions;

extern GenerationContext genContext;

typedef enum MusicID
{
	MUS_SILENCE = 0x0,
	MUS_KERO_1 = 0x1,
	MUS_KERO_2 = 0x2,
	MUS_GAME_OVER = 0x3,
	MUS_MELTDOWNHALF = 0x4,
	MUS_MELTDOWN1 = 0x5,
	MUS_MELTDOWN2 = 0x6,
	MUS_EYES_OF_FLAME = 0x7,
	MUS_KERO_3 = 0x8,
	MUS_RESONANCE = 0x9,
	MUS_GOT_ITEM = 0xA,
	MUS_66ALL = 0xB,
	MUS_TEMPORAL = 0xC,
	MUS_ELECTRO_CAVE = 0xD,
	MUS_PROBABLY = 0xE,
	MUS_VICTORY = 0xF,
	MUS_GET_HEART_TANK = 0x10,
	MUS_DUST_IN_THE_WIND = 0x11,
	MUS_GALAXY = 0x12,
	MUS_GREETINGS = 0x13,
	MUS_MAN_ROBIC = 0x14,
	MUS_JUNGLE = 0x15,
	MUS_WATERY_CAVES = 0x16,
	MUS_EDOCASTLE = 0x17,
	MUS_CRAZY_LA_PAINT = 0x18,
	MUS_PICTIONARY = 0x19,
	MUS_COGENT = 0x1A,
	MUS_MELTDOWN3 = 0x1B,
	MUS_TIME = 0x1C,
	MUS_DOCTOR_MEETING = 0x1D,
	MUS_BALCONY = 0x1E,
	MUS_DOCTOR_P1 = 0x1F,
	MUS_DOCTOR_P2 = 0x20,
	MUS_FREAK = 0x21,
	MUS_ARCADIA_BGM = 0x22,
	MUS_CORE_BGM = 0x23,
	MUS_SKYLAND_BGM = 0x24,
	MUS_EGGS_BGM = 0x25,
	MUS_VILLAGE_BGM = 0x26,
	MUS_WATERWAY_BGM = 0x27,
	MUS_HELL_BGM = 0x28,
	MUS_GRASSTOWN_BGM = 0x29,
	MUS_LABYRINTH_BGM = 0x2A,
	MUS_FIRST_CAVE_BGM = 0x2B,
	MUS_PLANTATION_BGM = 0x2C,
	MUS_CHANGE = 0x2D,
	MUS_IN = 0x2E,
	MUS_ICE_BGM = 0x2F,
	MUS_LABORATORY_BGM = 0x30,
	MUS_REPETIR = 0x31,
	MUS_DARKENED_SKY = 0x32,
	MUS_TUTORIAL = 0x33,
	MUS_STARFANFARE = 0x34,
	MUS_SELECT = 0x35,
	MUS_TINN2D = 0x36,
	MUS_MARSH_BGM = 0x37,
	MUS_INDOORS_BGM = 0x38,
	MUS_DARKCAVE_BGM = 0x39,
	MUS_HEADMAN_BGM = 0x3A,
	MUS_DUNGEON_BGM = 0x3B,
	MUS_MESA_BGM = 0x3C,
	MUS_CASTLE_BGM = 0x3D,
	MUS_SHEEP_BGM = 0x3E,
	MUS_BOSS_2 = 0x3F,
	MUS_FINAL_CHAMBER = 0x40,
	MUS_BOSS_3 = 0x41,
	MUS_CREDITS = 0x42,
	MUS_IKA_BGM = 0x43,
	MUS_SHADOW_BGM = 0x44,
	MUS_MOVE = 0x45,
	MUS_WALK = 0x46,
	MUS_CITYONE = 0x47,
	MUS_CHIP_CHALLENGE = 0x48,
	MUS_ANOMALY = 0x49,
	MUS_WIND_BOMB = 0x4A,
	MUS_TECHNO_LOL = 0x4B,
	MUS_FIRST_CAVE_BGM_2 = 0x4C,
	MUS_DARKCAVE_BGM_2 = 0x4D,
	MUS_HEADMAN_BGM_2 = 0x4E,
	MUS_SHEEP_BGM_2 = 0x4F,
	MUS_CASTLE_BGM_2 = 0x50,
	MUS_NAVICUST4 = 0x51,
	MUS_MESA_BGM_2 = 0x52,
	MUS_DUNGEON_BGM_2 = 0x53,
	MUS_EGGS_BGM_2 = 0x54,
	MUS_NOTMELTDOWN = 0x55,
	MUS_VILLAGE_BGM_2 = 0x56,
	MUS_WATERWAY_BGM_2 = 0x57,
	MUS_CORE_BGM_2 = 0x58,
	MUS_IKA_BGM_2 = 0x59,
	MUS_LABYRINTH_BGM_2 = 0x5A,
	MUS_SKYLAND_BGM_2 = 0x5B,
	MUS_ICE_BGM_2 = 0x5C,
	MUS_LABORATORY_BGM_2 = 0x5D,
	MUS_HELL_BGM_2 = 0x5E,
	MUS_GRASSTOWN_BGM_2 = 0x5F,
	MUS_ARCADIA_BGM_2 = 0x60,
	MUS_INDOORS_BGM_2 = 0x61,
	MUS_MARSH_BGM_2 = 0x62,
	MUS_PLANTATION_BGM_2 = 0x63,
} MusicID;

struct STAGE_TABLE
{
	char parts[0x20];
	char map[0x20];
	int bkType;
	char back[0x20];
	char npc[0x20];
	char boss[0x20];
	signed char boss_no;
	char name[0x22];
	char name_jp[0x22];
};

extern int gStageNo;
extern MusicID gMusicNo;
extern unsigned int gOldPos;
extern MusicID gOldNo;

extern const STAGE_TABLE *gTMT;
extern bool gDaily;

bool LoadStageTable();
void SaveRNG(json& profileJson);
void LoadRNG(json& profileJson);
bool IntersectCheck(Rect rect, int x, int y);
void ProcessIndividualNPCSpawning(const std::string& name);
void ProcessNPCSpawning(GenerationContext& genContext, GenerationArguments& genArguments);
bool TransferStage(int no, int w, int x, int y);
void ChangeMusic(MusicID no);
void ReCallMusic(void);
std::string CharacterName();
std::string TilesetName();
std::string TilesetName(char id);
std::string GimmickName();
std::string CharacterCode(int id);
bool IsItSecondCampaign();
void PutMappingName(void);
char GetGimmick(std::string id);
char GetTileset(std::string id);
std::uint64_t GetDailySeed();

#endif // __STAGE_H__