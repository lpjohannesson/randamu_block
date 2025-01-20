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

// gMC.equip
enum
{
	EQUIP_MAP = 2,
	EQUIP_ARMS_BARRIER = 4,
	EQUIP_TURBOCHARGE = 8,
	EQUIP_AIR_TANK = 0x10,
	EQUIP_SPEED_SHOES = 0x800,
	EQUIP_WINGS = 0x1000,
	EQUIP_WATER_THURST = 0x2000,
	EQUIP_MORE_AMMO = 0x8000,
	EQUIP_SUNGLASSES = 0x1000000,
	EQUIP_DASH_AMULET = 0x4000000
};

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

struct MYCHAR_DEFINITION
{
	std::string name;
	std::string spritesheet = "MyChar.png";
	
	struct MYCHAR_PHYSICS_DIFF
	{
		float max_dash = 1.0f;
		float max_move = 1.0f;
		float gravity1 = 1.0f;
		float gravity2 = 1.0f;
		float dash1 = 1.0f;
		float dash2 = 1.0f;
		float resist = 1.0f;
		float jump = 1.0f;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MYCHAR_PHYSICS_DIFF, max_dash, max_move, gravity1, gravity2, dash1, dash2, resist, jump)
	} physics_diff;

	int maxLife = 6;

	int hurtSound = 16;
	int deathSound = 17;
	int immunityFrames = 128;
	int lifeCapsuleLimit = 18;
	int lifeCapsuleValue = 3;
	std::vector<int> blacklistedChestFlags;
	std::vector<int> characterFlags;
	std::vector<int> startingItems;

	struct WEAPON {
		int id; int maxAmmo;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(WEAPON, id, maxAmmo)
	};

	std::vector<WEAPON> startingWeapons{{3, 0}};

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MYCHAR_DEFINITION, name, spritesheet, physics_diff, maxLife,
		hurtSound, deathSound, immunityFrames, lifeCapsuleLimit, lifeCapsuleValue,
		blacklistedChestFlags, characterFlags, startingItems, startingWeapons)
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
	int equip;
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
	unsigned long seed;
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
};

extern std::map<std::pair<short, short>, unsigned char> mapModifications;

extern std::vector<int> gAvailableIDs;
extern std::vector<Cooldown> gCooldowns;

extern nlohmann::ordered_map<std::string, MYCHAR_DEFINITION> availableCharacters;

extern std::vector<MYCHAR_EQUIP_DEFINITION> equipVisuals;

extern MYCHAR gMC;

void InitMyChar(void);
void AnimationMyChar(bool bKey);
void ShowMyChar(bool bShow);
void PutMyChar(int fx, int fy);
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
void EquipItem2(int flag, bool b);
void ResetCheck(void);
void SetNoise(int no, int freq);
void CutNoise(void);
void ResetNoise(void);
void SleepNoise(void);
std::pair<int, int> MyCharCoords();

int DashCooldown();

#endif // __MYCHAR_H__