#ifndef __GAME_H__
#define __GAME_H__


#include "cstdint"
#include "CommonDefines.h"

#define MAX_CHARACTERS 19

enum GameFlagsValues
{
	// To be continued
	GAME_FLAG_iS_GAME_GOING = 1,

	/**
	 * While this bit is NOT set, the game will :
	 * - Disable manual movement of the character
	 * - Disable shooting bullets
	 * - Disable shooting Curly's nemesis
	 * - Disable changing weapons
	 * - Disable speeding up the display of text in TSC scripts
	 * - Disable damage of the character
	 * - Not display the HUD (Life, EXP, air, weapons)
	 * - Disable animation of the character
	 * - Disable movement of the inventory cursor
	 * - Disable getting out of the inventory while on the item section
	 * - Create a bullet if some other conditions are fullfilled while iterating over the stars in ActStar (If you have any idea of how that actually works, you may
	 * want to replace this line with a better explanation)
	 */
	GAME_FLAG_IS_CONTROL_ENABLED = 2,
	GAME_FLAG_IS_GAMEPLAY_NOT_POSSIBLE = 4,
	GAME_FLAG_ARE_CREDITS_ROLLING = 8,
	GAME_FLAG_ARE_WE_IN_DIFFERENT_WINDOW = 16

	// To be continued
};

enum SelectedCustomItem
{
	ITEM_MinCapsules = 1,
	ITEM_MaxCapsules = 2,
	ITEM_CapsuleLifeLimit = 3,
	ITEM_MinSpikes = 4,
	ITEM_MaxSpikes = 5,
	ITEM_VillageSpikeDivider = 6,
	ITEM_HellSpikeMultiplier = 7,
	ITEM_MinRefills = 8,
	ITEM_MaxRefills = 9,
	ITEM_MinChests = 10,
	ITEM_MaxChests = 11,
	ITEM_MinPots = 12,
	ITEM_MaxPots = 13,
	ITEM_StarCount = 14,
	ITEM_MinPresses = 15,
	ITEM_MaxPresses = 16,
	ITEM_ExtraPresses = 17,
	ITEM_HellPressMultiplier = 18,
	ITEM_MinCritters = 19,
	ITEM_MaxCritters = 20,
	ITEM_ExtraCritters = 21,
	ITEM_MinPowerCritters = 22,
	ITEM_MaxPowerCritters = 23,
	ITEM_ExtraPowerCritters = 24,
	ITEM_MinSkullheads = 25,
	ITEM_MaxSkullheads = 26,
	ITEM_ExtraSkullheads = 27,
	ITEM_MinCrows = 28,
	ITEM_MaxCrows = 29,
	ITEM_ExtraCrows = 30,
	ITEM_MinBats = 31,
	ITEM_MaxBats = 32,
	ITEM_ExtraBats = 33,
	ITEM_MinBeetles = 34,
	ITEM_MaxBeetles = 35,
	ITEM_ExtraBeetles = 36,
	ITEM_MinArchers = 37,
	ITEM_MaxArchers = 38,
	ITEM_ExtraArchers = 39,
	ITEM_RequiredHPArcherSpawn = 40,
	ITEM_MinChains = 41,
	ITEM_MaxChains = 42,
	ITEM_VillageChainMultiplier = 43,
	ITEM_MinPlatforms = 44,
	ITEM_MaxPlatforms = 45,
	ITEM_VillagePlatformMultiplier = 46,
	ITEM_MinBG_Tiles = 47,
	ITEM_MaxBG_Tiles = 48,
	ITEM_VillageBG_TileMultiplier = 49
};

namespace DeathReason {
enum DeathReason {
	NOTHING,
	CRITTER,
	BAT,
	PRESS,
	SPIKE,
	POWER_CRITTER,
	ARCHER,
	BEETLE,
	CROW,
	SKULLHEAD,
	BALROG,
	ENERGY_SHOT,
	RED_DEMON,
	MESA,
	JELLY,
	KURARA,
	POSSUE,
	IGOR,
	MANNAN,
	TURRET,
	CORE_BASE,
	LASER,
	DOCTOR_P1,
	DOCTOR_P2,
	WATER,
	HUNGER,
	FALLING_BLOCK,
	PUPPY,
	COLONBALL,
	FIRE,
	ENV_CRITTERS,
	ROLLINGS,
	EXPLOSIVES,
	BALFROG,
	OMEGA,
	MONSTER_X,
	CORE,
	JETHEAD,
	SISTERS,
	ARMOR_GAUDIS,
	DRAGONS,
	FROGS,
	METAL_FUZZ,
	PUCHI,
	DARK_MISERY,
	DARK_POSMIS,
	DARK_TOROKO,
	BALLOS,
	DROWNING,
	FALLING_OUT
};
}

namespace RandomCategories{
enum RandomCategories {
	NORMAL,
	SEEDED_PRIMARY,
	SEEDED_CHESTS,
	SEEDED_NPCS
};
}

extern int g_GameFlags;
extern int gCounter;
extern bool gContinue;
extern int gGameMode;

int Random(int min, int max, RandomCategories::RandomCategories category = RandomCategories::NORMAL);
uint64_t Random64(uint64_t min, uint64_t max);
void PutNumber4(int x, int y, int value, bool bZero);
void PutNumber5(int x, int y, int value, bool bZero);
//added by kait
void PutNumber9(int x, int y, size_t value, bool bZero);
void PutNumber10(int x, int y, size_t value, bool bZero);
void PutNumber19(int x, int y, size_t value, bool bZero);

void PutDebugInfo();

bool TimeStopActive(int npc_id = 0);

bool Game(void);

int operator"" _pix (unsigned long long int units);
int operator"" _pix (long double units);
#endif // __GAME_H__