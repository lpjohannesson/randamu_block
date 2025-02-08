#ifndef __MYCHAR_H__
#define __MYCHAR_H__


#include "Cooldown.h"
#include "Json.h"
#include "CommonDefines.h"

#include <functional>
#include <map>
#include <vector>
#include <utility>

// TODO - When I add bitmask constants for gMC.flags...
// 0x100 is a 'player is underwater' flag

struct MYCHAR_PHYSICS
{
	int max_dash;
	int max_move;
	int gravity1;
	int gravity2;
	int dash1;
	int dash2;
	int resist;
	int jump;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MYCHAR_PHYSICS, max_dash, max_move, gravity1, gravity2, dash1, dash2, resist, jump)
};

/// @brief The structure of character definition JSON. 
struct MYCHAR_DEFINITION
{
	/// @brief The name of character in-game.
	/// @since 1.45
	std::string name = "";
	/// @brief The string used to identify the character in saves and rankings.
	/// @warning The value in file is ignored and replaced with folder's name.
	/// @since 1.45
	std::string codeName = "";
	/// @brief The name of character in character select screen.
	/// @since 1.45
	std::string fullName = "";
	/// @brief The name of picture file that will be used for the character. Has to be PNG file.
	/// @since 1.45
	std::string spritesheet = "MyChar";
	
	/// @brief Defines the differences in character's physics values, compared to default, like speed or gravity.
	/// @since 1.45 
	struct MYCHAR_PHYSICS_DIFF
	{
		/// @brief The max running speed multiplier.
		/// @since 1.45
		float max_dash = 1.0f;
		/// @brief The max movement speed multiplier.
		/// @since 1.45
		float max_move = 1.0f;
		/// @brief The multiplier for gravitation used when falling.
		/// @since 1.45 
		float gravity1 = 1.0f;
		/// @brief The multiplier for gravitation used when ascending.
		/// @since 1.45 
		float gravity2 = 1.0f;
		/// @brief The acceleration multiplier for grounded movement.
		/// @since 1.45 
		float dash1 = 1.0f;
		/// @brief The acceleration multiplier for movement in the air.
		/// @since 1.45 
		float dash2 = 1.0f;
		/// @brief The friction multiplier.
		/// @since 1.45 
		float resist = 1.0f;
		/// @brief The jump force multiplier.
		/// @since 1.45 
		float jump = 1.0f;

		/// @internal
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MYCHAR_PHYSICS_DIFF, max_dash, max_move, gravity1, gravity2, dash1, dash2, resist, jump)
	} physics_diff;

	/// @brief The starting maximum life of the character.
	/// @since 1.45
	int maxLife = 6;

	/// @brief The sound ID used when character is hurt.
	/// @since 1.45
	int hurtSound = 16;
	/// @brief The sound ID used for character's death.
	/// @since 1.45
	int deathSound = 17;
	/// @brief The amount of immunity frames character will get, when hurt.
	/// @since 1.45 
	int immunityFrames = 128;
	/// @brief The amount of life capsules character can collect through the run.
	/// @since 1.45 
	int lifeCapsuleLimit = 18;
	/// @brief The value of each life capsule.
	/// @note This is used for calculating starting life during Second Campaign Start run option.
	/// @since 1.45 
	int lifeCapsuleValue = 3;
	/// @brief The list of chest flags that will not be in item pool for this character.
	/// @note The items connected to particular chest flags can be seen in generated stage's TSC script.
	/// @since 1.45 
	std::vector<int> blacklistedChestFlags;
	/// @brief The list of flags that are set at start of the run.
	/// @details Most of characters should define one or two flags and define character's traits via \ref #FlagTraits "flag traits" functionality.
	/// @since 1.45 
	std::vector<int> characterFlags;
	/// @brief The list of item IDs, that will be given at start of the run.
	/// @since 1.45
	std::vector<int> startingItems;

	/// @brief The weapon data, defined similarly to `<AM+` TSC command.
	/// @since 1.45
	struct WEAPON {
		/// @brief The weapon ID.
		/// @since 1.45
		int id; 
		/// @brief The maximum ammo of the weapon. If set to 0, the weapon will consume no ammo.
		/// @since 1.45 
		int maxAmmo;
		/// @internal
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(WEAPON, id, maxAmmo)
	};

	/// @brief The list of weapons that will be given at the start of the run.
	/// @note By default, it is Fireball with infinite ammo.
	/// @since 1.45 
	std::vector<WEAPON> startingWeapons{{3, 0}};
	/// @brief Makes the character spawn with Nemesis, the block-breaking weapon.
	/// @since 1.45 
	bool startWithPolarStar = true;
	/// @brief Makes the character spawn with Polar Star, the block-placing weapon.
	/// @since 1.45 
	bool startWithNemesis = true;

	/// @brief The character's primary description on character selection screen.
	/// @since 1.45 
	std::string description1 = "";
	/// @brief The character's secondary description on character selection screen.
	/// @since 1.45
	std::string description2 = "";

	/// @internal
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MYCHAR_DEFINITION, name, codeName, fullName, spritesheet, physics_diff, maxLife,
		hurtSound, deathSound, immunityFrames, lifeCapsuleLimit, lifeCapsuleValue,
		blacklistedChestFlags, characterFlags, startingItems, startingWeapons, startWithPolarStar, startWithNemesis,
		description1, description2)
};

struct MYCHAR_EQUIP_DEFINITION
{
	int flag; int accs_id;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MYCHAR_EQUIP_DEFINITION, flag, accs_id)
};

struct MYCHAR
{
	unsigned char cond;
	unsigned int flag;
	int direct;
	bool up;
	bool down;
	int unit;
	int x;
	int y;
	int tgt_x;
	int tgt_y;
	int index_x;
	int index_y;
	int xm;
	int ym;
	int ani_wait;
	int ani_no;
	OTHER_Rect hit;
	OTHER_Rect view;
	Rect rect;
	Rect rect_arms;
	int level;
	int exp_wait;
	int exp_count;
	unsigned short shock;
	unsigned char no_life;
	unsigned char rensha;
	unsigned char bubble;
	short life;
	short star;
	short max_life;
	short a;
	int lifeBr;
	int lifeBr_count;
	int air;
	int air_get;
	signed char sprash; // This is explicitly a char, but used like a bool
	signed char ques;   // Same for this variable as well
	signed char boost_sw;
	int boost_cnt;
	// Custom
	MYCHAR_PHYSICS physics_normal;
	MYCHAR_PHYSICS physics_underwater;
	int no_splash_or_air_limit_underwater; // Determines whether the behaviours in which entering water causes a splash, being in water draws the air timer and being in water decreases your air are enabled (0 means it's enabled, non-0 means it's disabled)
	bool pogo;
	int score;
	int depth;
	bool hell;
	bool outerWallInit;
	char tileset;
	uint64_t seed;
	bool randomSeed;
	int regenCount;
	float regenCounter;
	int dashDuration;
	int visualDashDuration;
	int dashLock;
	int drowningTimer;
	bool flip;
	bool lethalMomentum;
	int lifeCapsuleLimit;
	int noMoreLifeCapsules;
	unsigned char mapMods[118*118];
	int generatedChests[50];
	int chaosAnim;
	int saraDash;
	int miracle;
	int miracleGenerated;
	int miracleDepth;
	int deathReason;
	int gimmick;
	bool grass;
	int chargeCD;
	bool chargeActive;
	bool ameDashState;
	int difficulty;
	int freeze;
	MYCHAR_DEFINITION characterData;
};

extern std::map<std::pair<short, short>, unsigned char> mapModifications;

extern std::vector<int> gAvailableIDs;
extern std::vector<Cooldown> gCooldowns;

extern nlohmann::ordered_map<std::string, MYCHAR_DEFINITION> availableCharacters;
extern std::vector<std::string> characterCodenames;

extern std::vector<MYCHAR_EQUIP_DEFINITION> equipVisuals;

extern MYCHAR gMC;

void InitMyChar(void);
void AnimationMyChar(bool bKey);
void ShowMyChar(bool bShow);
void PutMyChar(int fx, int fy);
void InitMyCharSprite(std::string characterName);
void ActMyChar_Normal(bool bKey);
bool IsDashEquipped();
bool IsASheep();
void ActMyChar(bool bKey);
void GetMyCharPosition(int *x, int *y);
void SetMyCharPosition(int x, int y);
void MoveMyChar(int x, int y);
void ZeroMyCharXMove(void);
int GetUnitMyChar(void);
void SetMyCharDirect(unsigned char dir);
void ChangeMyUnit(unsigned char a);
void PitMyChar(void);
void EquipItem(int flag, bool b);
void ResetCheck(void);
void SetNoise(int no, int freq);
void CutNoise(void);
void ResetNoise(void);
void SleepNoise(void);
std::pair<int, int> MyCharCoords();

int DashCooldown();

#endif // __MYCHAR_H__