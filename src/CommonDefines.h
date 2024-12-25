#ifndef __COMMONDEFINES_H__
#define __COMMONDEFINES_H__


#define WINDOW_WIDTH 426
#define WINDOW_HEIGHT 240

enum Collisions
{
	COLL_LEFT_WALL = 1,     // Touching a left wall
	COLL_CEILING = 2,       // Touching a ceiling
	COLL_RIGHT_WALL = 4,    // Touching a right wall
	COLL_GROUND = 8         // Touching the ground
	// To be continued
};

enum Direction
{
	DIR_LEFT = 0,
	DIR_UP = 1,
	DIR_RIGHT = 2,
	DIR_DOWN = 3,
	DIR_AUTO = 4
};

struct OTHER_Rect	// The original name for this struct is unknown
{
	int front;
	int top;
	int back;
	int bottom;
};

#define RGBA(r,g,b,a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))
#define RGB(r,g,b) ((r) | ((g) << 8) | ((b) << 16))

struct Rect
{
	long left;
	long top;
	long right;
	long bottom;
};

namespace Flags {
	inline constexpr int BALLOS_BOSS_DONE         = 222;
	inline constexpr int LAST_LEVEL_CH1_DOOR      = 999;
	inline constexpr int DOCTOR_PHASE_2_DONE      = 1111;
	inline constexpr int CH2_BOSS_DOOR_BEGIN      = 4750;
	inline constexpr int LAST_LEVEL_CH2_DOOR      = 6999;
	inline constexpr int DOCTOR_PHASE_1_DONE      = 5002;

	inline constexpr int CAPSULE_SAVE_FLAG_BEGIN  = 300;
	inline constexpr int LEVEL_BOSS_DEFEATED      = 555;

	//items
	inline constexpr int CHAOS_KEY                = 703;
	inline constexpr int BATTERY_PACK             = 705;
	inline constexpr int NICE_SKATES              = 708;
	inline constexpr int GRAVITY_NECKLACE         = 710;

	inline constexpr int LEAD_WEIGHT              = 711;
	inline constexpr int NANO_QUIVER              = 712;
	inline constexpr int OVERCLOCK                = 720;

	inline constexpr int COPPER_BULWARK           = 724;
	inline constexpr int SILVER_LOCKET            = 725;

	inline constexpr int COOL_SUNGLASSES          = 737;
	inline constexpr int MOMENTUM_THROTTLER       = 738;
	inline constexpr int STEEL_PLATING            = 739;
	inline constexpr int LITTLE_MAN               = 740;

	inline constexpr int CREATION_ENERGIZER       = 741;
	inline constexpr int DESTRUCTION_ENERGIZER    = 742;
	inline constexpr int BOOSTER_2_0              = 743;
	inline constexpr int PROPANE_TANK             = 744;
	inline constexpr int CLAY_FIGURE_MEDAL        = 745;
	inline constexpr int IRON_BOND                = 746;
	inline constexpr int FROSTBITE_STOPWATCH      = 747;
	inline constexpr int BALLOS_BURGER            = 748;
	inline constexpr int ALIEN_MEDAL              = 749;
	inline constexpr int HEAVY_WEIGHT             = 750;

	inline constexpr int CURLY_UNDERWEAR          = 751;
	inline constexpr int ENERGY_CANISTER          = 752;
	inline constexpr int AUXILIARY_BOOSTER        = 753;
	inline constexpr int WATER_THURSTERS          = 754;
	inline constexpr int PAIR_OF_WINGS            = 755;
	inline constexpr int SPEED_SHOES              = 756;
	inline constexpr int WALL_HOOK                = 757;
	inline constexpr int FISHY_TUBE               = 760;

	inline constexpr int WHIMSICAL_STAR           = 851;
	inline constexpr int TIMELINE_RING            = 860;

	inline constexpr int ENERGY_CRYSTAL           = 861;
	inline constexpr int SUPER_BOOSTER            = 862;
	inline constexpr int DIVING_SUIT              = 863;
	inline constexpr int SHIELD_OF_RESILIENCE     = 864;
	inline constexpr int AUGMENTED_MAN            = 865;
	inline constexpr int CAPE_OF_SHADOWS          = 866;
	inline constexpr int SACRED_AEGIS             = 867;
	inline constexpr int FLAMING_BOOSTER          = 868;
	inline constexpr int TERRA_BOOTS              = 869;
	inline constexpr int BADGE_OF_LOVE            = 870;

	inline constexpr int UNIVERSAL_CATALYST       = 871;
	inline constexpr int AMPHIBIAN_MEDAL          = 872;
	inline constexpr int HIGH_OCTANE_TANK         = 873;
	inline constexpr int DISCORDIAN_VISOR         = 874;
	inline constexpr int WINTER_CLOCK             = 875;
	inline constexpr int SHIELD_OF_TIME           = 876;
	inline constexpr int LITTLE_MAN_UNDERWEAR     = 877;
	inline constexpr int NUCLEAR_STOMPER          = 878;
	inline constexpr int OMNIQUICK_SNEAKERS       = 879;
	inline constexpr int FREEZE_STAR              = 880;

	inline constexpr int WATER_GUN_EFFECT         = 881;

	inline constexpr int NANITES_ABSORBER         = 882;
	inline constexpr int MILITARY_ARMAMENT        = 883;

	inline constexpr int STAR_SAVE_FLAG_BEGIN     = 950;
	inline constexpr int BADGE_OF_LOVE_REVIVE_PROC= 1070;

	inline constexpr int PROCGEN_CHARACTER        = 1024;
	inline constexpr int SERENITY_CHARACTER       = 2048;
	inline constexpr int MODERN_QUOTE_CHARACTER   = 2222;
	inline constexpr int REFILL_SAVE_FLAG_BEGIN   = 2630;
	inline constexpr int REFILL_SAVE_FLAG_END     = 2699;
	inline constexpr int HUNTER_CHARACTER         = 3333;
	inline constexpr int SUE_CHARACTER            = 4096;
	inline constexpr int KING_CHARACTER           = 4100;
	inline constexpr int EXCERPT_CHARACTER        = 4150;
	inline constexpr int CURLY_CHARACTER          = 4200;
	inline constexpr int QUATE_CHARACTER          = 4242;
	inline constexpr int SARA_CHARACTER           = 4250;
	inline constexpr int DUCKMAN_CHARACTER        = 4280;
	inline constexpr int TOROKO_CHARACTER         = 4300;
	inline constexpr int TOROKO_BLEEDING_OUT      = 4301;
	inline constexpr int LILY_CHARACTER           = 4350;
	inline constexpr int MISERY_CHARACTER         = 4400;
	inline constexpr int MAXY_CHARACTER           = 4405;
	inline constexpr int BRACKET_CHARACTER        = 4450;
	inline constexpr int PUPPY_CHARACTER          = 4475;
	inline constexpr int AMY_CHARACTER            = 5120;
	inline constexpr int MAHIN_CHARACTER          = 7999;

	inline constexpr int MINING_LASER_MODE_BEGIN  = 6900;
	
};

namespace Bullets {
	inline constexpr int NOTHING                  = 0;
	inline constexpr int SNAKE_LVL_1              = 1;
	inline constexpr int SNAKE_LVL_2              = 2;
	inline constexpr int SNAKE_LVL_3              = 3;
	inline constexpr int SNAKE_LVL_4              = 70;

	inline constexpr int MANNAN_ATTACK_VFX        = 94;
}

#endif // __COMMONDEFINES_H__