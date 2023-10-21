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
	//items
	const int CHAOS_KEY                = 703;
	const int NICE_SKATES              = 708;
	const int GRAVITY_NECKLACE         = 710;

	const int LEAD_WEIGHT              = 711;
	const int NANO_QUIVER              = 712;
	const int OVERCLOCK                = 720;

	const int COPPER_BULWARK           = 724;
	const int SILVER_LOCKET            = 725;

	const int COOL_SUNGLASSES          = 737;
	const int MOMENTUM_THROTTLER       = 738;
	const int STEEL_PLATING            = 739;
	const int LITTLE_MAN               = 740;

	const int CREATION_ENERGIZER       = 741;
	const int DESTRUCTION_ENERGIZER    = 742;
	const int BOOSTER_2_0              = 743;
	const int PROPANE_TANK             = 744;
	const int CLAY_FIGURE_MEDAL        = 745;
	const int IRON_BOND                = 746;
	const int FROSTBITE_STOPWATCH      = 747;
	const int BALLOS_BURGER            = 748;
	const int ALIEN_MEDAL              = 749;
	const int HEAVY_WEIGHT             = 750;

	const int CURLY_UNDERWEAR          = 751;
	const int ENERGY_CANISTER          = 752;
	const int AUXILIARY_BOOSTER        = 753;
	const int WATER_THURSTERS          = 754;
	const int PAIR_OF_WINGS            = 755;
	const int SPEED_SHOES              = 756;
	const int WALL_HOOK                = 757;
	const int FISHY_TUBE               = 760;

	const int WHIMSICAL_STAR           = 851;
	const int TIMELINE_RING            = 860;

	const int SUPER_BOOSTER            = 862;
	const int DIVING_SUIT              = 863;
	const int SHIELD_OF_RESILIENCE     = 864;
	const int AUGMENTED_MAN            = 865;
	const int CAPE_OF_SHADOWS          = 866;
	const int SACRED_AEGIS             = 867;
	const int FLAMING_BOOSTER          = 868;
	const int TERRA_BOOTS              = 869;
	const int BADGE_OF_LOVE            = 870;

	const int UNIVERSAL_CATALYST       = 871;
	const int AMPHIBIAN_MEDAL          = 872;
	const int HIGH_OCTANE_TANK         = 873;
	const int DISCORDIAN_VISOR         = 874;
	const int WINTER_CLOCK             = 875;
	const int SHIELD_OF_TIME           = 876;
	const int LITTLE_MAN_UNDERWEAR     = 877;
	const int NUCLEAR_STOMPER          = 878;
	const int OMNIQUICK_SNEAKERS       = 879;
	const int FREEZE_STAR              = 880;
	
	const int NANITES_ABSORBER         = 882;
	const int MILITARY_ARMAMENT        = 883;
};

#endif // __COMMONDEFINES_H__