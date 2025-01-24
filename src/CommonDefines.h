#ifndef __COMMONDEFINES_H__
#define __COMMONDEFINES_H__

#include <string>

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

	inline constexpr int WATER_GUN_EFFECT         = 881;

	inline constexpr int STAR_SAVE_FLAG_BEGIN     = 950;
	inline constexpr int BADGE_OF_LOVE_REVIVE_PROC= 1070;

	inline constexpr int PROCGEN_CHARACTER        = 1024;
	inline constexpr int SERENITY_CHARACTER       = 2048;
	inline constexpr int MODERN_QUOTE_CHARACTER   = 2222;
	inline constexpr int REFILL_SAVE_FLAG_BEGIN   = 2630;
	inline constexpr int REFILL_SAVE_FLAG_END     = 2699;
	inline constexpr int EXCERPT_CHARACTER        = 4150;
	inline constexpr int QUATE_CHARACTER          = 4242;
	inline constexpr int DUCKMAN_CHARACTER        = 4280;
	inline constexpr int TOROKO_BLEEDING_OUT      = 4301;
	inline constexpr int MAXY_CHARACTER           = 4405;
	inline constexpr int BRACKET_CHARACTER        = 4450;
	inline constexpr int PUPPY_CHARACTER          = 4475;
	inline constexpr int AMY_CHARACTER            = 5120;
	inline constexpr int MAHIN_CHARACTER          = 7999;

	inline constexpr int MINING_LASER_MODE_BEGIN  = 6900;
	
};

namespace FlagTraits {
	inline constexpr const char* NANITES                       = "nanites";
	inline constexpr const char* TIMESTOP                      = "timestop";
	inline constexpr const char* INVIDIA                       = "invidia";
	inline constexpr const char* BLADE_DASH                    = "blade_dash";
	inline constexpr const char* DUCKS                         = "ducks";
	inline constexpr const char* HOOK_FLAIL                    = "hook_flail";
	inline constexpr const char* WITCH_FIREBALL                = "witch_fireball";
	inline constexpr const char* PROPANE_TRAIL                 = "propane_trail";
	inline constexpr const char* BUFFED_MACHINE_GUN            = "buffed_machine_gun";
	inline constexpr const char* CONTROLLED_ROCKET             = "controlled_rocket";
	inline constexpr const char* SUPER_DASH                    = "super_dash";
	inline constexpr const char* SPIKE_EVADING                 = "spike_evading";
	inline constexpr const char* WITCH_HITBOX                  = "witch_hitbox";
	inline constexpr const char* WATER_MOB_FRIEND              = "water_mob_friend";
	inline constexpr const char* PROJECTILE_REPEL              = "dash_projectile_repel";
	inline constexpr const char* STOMPING                      = "stomping";
	inline constexpr const char* LEVEL_4_WEAPONS               = "level_4_weapons";
	inline constexpr const char* IFRAMES_BOOST                 = "iframes_boost";
	inline constexpr const char* KO_PREVENTION                 = "ko_prevention";
	inline constexpr const char* EXPLODE_ON_HIT                = "explode_on_hit";
	inline constexpr const char* LAST_STAND                    = "last_stand";
	inline constexpr const char* SNAKE_EMPOWER                 = "buffed_snake";
	inline constexpr const char* SWORD_SHEATHING               = "sword_sheathing";
	inline constexpr const char* HUNGER_SYSTEM                 = "hunger_system";
	inline constexpr const char* KNOCKBACK                     = "knockback";
	inline constexpr const char* MOUSE_TELEPORT                = "mouse_teleport";
	inline constexpr const char* ICE_SKATES                    = "ice_skates";
	inline constexpr const char* NO_SLIPPERY                   = "no_slippery";
	inline constexpr const char* EXCERPT_ALLIES                = "excerpt_allies";
	inline constexpr const char* LOW_HP_RESIST                 = "low_hp_resist";
	inline constexpr const char* DASH_SPRINT                   = "dash_sprint";
	inline constexpr const char* DUCK_HEALTH                   = "duck_health";
	inline constexpr const char* BUFFED_DASH                   = "buffed_dash";
	inline constexpr const char* WALL_JUMP                     = "wall_jump";
	inline constexpr const char* WITCH_ASCENDING               = "witch_ascending";
	inline constexpr const char* DASH_FLOAT                    = "dash_float";
	inline constexpr const char* CAT_DASH                      = "cat_dash";
	inline constexpr const char* CAT_FLOAT                     = "cat_float";
	inline constexpr const char* WITCH_DASH_VFX                = "witch_dash_vfx";
	inline constexpr const char* STAR_DASH_VFX                 = "star_dash_vfx";
	inline constexpr const char* BETTER_DUCKS                  = "better_ducks";
	inline constexpr const char* CAN_BOOST                     = "can_boost";
	inline constexpr const char* BOOSTER_0_8                   = "booster_0_8";
	inline constexpr const char* BOOSTER_2_0                   = "booster_2_0";
	inline constexpr const char* GRAVITY_FLIP                  = "gravity_flip";
	inline constexpr const char* EXPLODING_FIREBALL            = "exploding_fireball";
	inline constexpr const char* PRESS_DMG_REDUCE              = "press_dmg_reduce";
	inline constexpr const char* BULLET_LIFE_BOOST             = "bullet_life_boost";
	inline constexpr const char* DOUBLE_JUMP                   = "double_jump";
	inline constexpr const char* CHEST_BOOST                   = "chest_boost";
	inline constexpr const char* HIDDEN_EQUIPS                 = "hidden_equips";
	inline constexpr const char* AMPHIBIOUS                    = "amphibious";
	inline constexpr const char* HIDDEN_WEAPONS                = "hidden_weapons";
	inline constexpr const char* BOOSTER_FUEL_BOOST            = "booster_fuel_boost";
	inline constexpr const char* DONT_SLOW_ON_BOOST_END        = "dont_slow_on_boost_end";
	inline constexpr const char* ACID_IMMUNITY                 = "acid_immunity";
	inline constexpr const char* LIFESTEAL                     = "lifesteal";
	inline constexpr const char* LIFESTEAL_MODIFIER            = "lifesteal_modifier";
	inline constexpr const char* JUMP_ON_SPIKES                = "jump_on_spikes";
	inline constexpr const char* SPIKE_DAMAGE_RESIST           = "spike_damage_resist";
	inline constexpr const char* CHILL                         = "chill";
	inline constexpr const char* WALL_STICKY                   = "wall_sticky";
	inline constexpr const char* ENERGY_PROJECTILE_ON_HIT      = "energy_projectile_on_hit";
	inline constexpr const char* RANGED_BLOCK_PLACE            = "ranged_block_place";
	inline constexpr const char* PRECISE_PLACEMENT             = "precise_placement";
	inline constexpr const char* MANUAL_ROCKET_STOP            = "manual_rocket_stop";
	inline constexpr const char* TIGHTER_TURNING               = "tighter_turning";
	inline constexpr const char* MULTI_BLOCK_BREAKING          = "multi_block_breaking";
	inline constexpr const char* OFFSET_NEMESIS                = "offset_nemesis";
	inline constexpr const char* BULLET_LIMIT_BOOST            = "bullet_limit_boost";
	inline constexpr const char* EXTRA_BULLET_CHANCE           = "extra_bullet_chance";
	inline constexpr const char* FIRERATE_BOOST                = "firerate_boost";
	inline constexpr const char* INACCURATE_VELOCITY_MODIFIER  = "inaccurate_velocity_modifier";
	inline constexpr const char* ACCELERATE_FALL               = "accelerate_fall";
	inline constexpr const char* SPEED_BOOTS                   = "speed_boots";
	inline constexpr const char* LOW_DAMAGE_NULLIFY            = "low_damage_nullify";
	inline constexpr const char* WATER_FLOATING                = "water_floating";
	inline constexpr const char* MAPPING                       = "mapping";
	inline constexpr const char* EXP_SAVING                    = "exp_saving";
	inline constexpr const char* MINING_CHARGE_FROM_DAMAGE     = "mining_charge_from_damage";
	inline constexpr const char* MINING_PASSIVE_CHARGE         = "mining_passive_charge";
	inline constexpr const char* SPUR_CHARGE_BOOST             = "spur_charge_boost";
	inline constexpr const char* WEAPON_RECHARGE_BOOST         = "weapon_recharge_boost";
	inline constexpr const char* LAVA_PROTECTION               = "lava_protection";
	inline constexpr const char* BREATH_EXTENSION              = "breath_extension";
	inline constexpr const char* UNDERWATER_BUBBLE_VFX         = "underwater_bubble_vfx";
	inline constexpr const char* DROWNING_SLOWDOWN             = "drowning_slowdown";
	inline constexpr const char* MINING_STORAGE_BOOST          = "mining_storage_boost";
	inline constexpr const char* JUMP_BOOST                    = "jump_boost";
	inline constexpr const char* GLIDING                       = "gliding";
	inline constexpr const char* SWIM_BOOST                    = "swim_boost";
	inline constexpr const char* CHARGED_SWIMS                 = "charged_swims";
	inline constexpr const char* UNDERWATER_DASH               = "underwater_dash";
	inline constexpr const char* ATTACK_DAMAGE_MODIFIER        = "attack_damage_modifier";
	inline constexpr const char* UNIVERSAL_IFRAMES_MODIFIER    = "universal_iframes_modifier";
	inline constexpr const char* DASH_MODIFIER                 = "dash_modifier";
	inline constexpr const char* MEDAL_SPAWN                   = "medal_spawn";
	inline constexpr const char* LITTLE_MAN_SPAWN              = "little_man_spawn";
	inline constexpr const char* CURLY_SPAWN                   = "curly_spawn";
	inline constexpr const char* WHIMSICAL_STAR                = "whimsical_star";
	inline constexpr const char* UNHINDERED_DIVING             = "unhindered_diving";
	inline constexpr const char* UNLIMITED_SWIM                = "unlimited_swim";
	inline constexpr const char* DASH_FLAME_TRAIL              = "dash_flame_trail";
	inline constexpr const char* CONTROLLED_LITTLE_MAN         = "controlled_little_man";
	inline constexpr const char* SHOOTING_LITTLE_MAN           = "shooting_little_man";
	inline constexpr const char* FLAT_DAMAGE_REDUCTION         = "flat_damage_reduction";
	inline constexpr const char* BOOSTER_DAMAGE                = "booster_damage";
	inline constexpr const char* REVIVAL_FROM_CURLY            = "revival_from_curly";
	inline constexpr const char* HEART_MAGNET                  = "heart_magnet";
	inline constexpr const char* SAVING_AMMO_CHANCE            = "saving_ammo_chance";
	inline constexpr const char* DASH_REFRESHES_JUMP           = "dash_refreshes_jump";
	inline constexpr const char* GLIDING_AFTER_DASH            = "gliding_after_dash";
	inline constexpr const char* STRAFE                        = "strafe";
	inline constexpr const char* SCREEN_FREEZE                 = "screen_freeze";
	inline constexpr const char* LITTLE_N_CURLY_TEAM_UP        = "little_n_curly_team_up";
	inline constexpr const char* RANGED_BLOCK_BREAK            = "ranged_block_break";
	inline constexpr const char* MINING_FREEZE_AURA            = "mining_freeze_aura";
	inline constexpr const char* TELEPORT_DASH                 = "teleport_dash";
	inline constexpr const char* VELOCITY_BASED_JUMP           = "velocity_based_jump";
	inline constexpr const char* INFINITE_MINING               = "infinite_mining";
	inline constexpr const char* STAGE_COMPLETE_AMMO_BOOST     = "stage_complete_ammo_boost";
}

namespace Bullets {
	inline constexpr int NOTHING                      = 0;

	inline constexpr int SNAKE_LVL_1                  = 1;
	inline constexpr int SNAKE_LVL_2                  = 2;
	inline constexpr int SNAKE_LVL_3                  = 3;
	inline constexpr int SNAKE_LVL_4                  = 70;

	inline constexpr int FIREBALL_LVL_1               = 7;
	inline constexpr int FIREBALL_LVL_2               = 8;
	inline constexpr int FIREBALL_LVL_3               = 9;
	inline constexpr int FIREBALL_LVL_4               = 68;
	inline constexpr int FIREBALL_FLAME               = 45;

	inline constexpr int BUBBLER_LVL_1                = 19;
	inline constexpr int BUBBLER_LVL_2                = 20;
	inline constexpr int BUBBLER_LVL_3                = 21;
	inline constexpr int BUBBLER_LVL_4                = 71;
	inline constexpr int BUBBLER_SPINE                = 22;
	inline constexpr int BUBBLER_WATERGUN             = 93;

	inline constexpr int MACHINE_GUN_LVL_1            = 10;
	inline constexpr int MACHINE_GUN_LVL_2            = 11;
	inline constexpr int MACHINE_GUN_LVL_3            = 12;
	inline constexpr int MACHINE_GUN_LVL_4            = 69;
	inline constexpr int MACHINE_GUN_GATLING          = 92;

	inline constexpr int SPUR_LVL_1                   = 37;
	inline constexpr int SPUR_LVL_2                   = 38;
	inline constexpr int SPUR_LVL_3                   = 39;
	inline constexpr int SPUR_LVL_4                   = 76;
	inline constexpr int SPUR_TRAIL_LVL_1             = 40;
	inline constexpr int SPUR_TRAIL_LVL_2             = 41;
	inline constexpr int SPUR_TRAIL_LVL_3             = 42;
	inline constexpr int SPUR_LVL_0                   = 6;

	inline constexpr int BOOMERANG_BLADE_LVL_1        = 52;
	inline constexpr int BOOMERANG_BLADE_LVL_2        = 53;
	inline constexpr int BOOMERANG_BLADE_LVL_3        = 54;
	inline constexpr int BOOMERANG_BLADE_LVL_4        = 75;
	inline constexpr int BOOMERANG_BLADE_LVL_2_FLAME  = 56;
	inline constexpr int BOOMERANG_BLADE_LVL_3_SPLIT  = 55;

	inline constexpr int RIFLE_LVL_1                  = 28;
	inline constexpr int RIFLE_LVL_2                  = 29;
	inline constexpr int RIFLE_LVL_3                  = 30;

	inline constexpr int BLADE_LVL_1                  = 25;
	inline constexpr int BLADE_LVL_2                  = 27;
	inline constexpr int BLADE_LVL_4                  = 72;

	inline constexpr int MINING_LASER                 = 64;
	inline constexpr int MINING_BOOM                  = 65;

	inline constexpr int FRONTIER_BUSTER_LVL_1        = 87;
	inline constexpr int FRONTIER_BUSTER_LVL_2        = 89;
	inline constexpr int FRONTIER_BUSTER_LVL_3        = 90;
	inline constexpr int FRONTIER_BUSTER_LVL_4        = 91;
	inline constexpr int FRONTIER_BUSTER_LVL_1_HOMING = 88;

	inline constexpr int FLOODTIDE_LVL_1              = 58;
	inline constexpr int FLOODTIDE_LVL_2              = 59;
	inline constexpr int FLOODTIDE_LVL_3              = 60;
	inline constexpr int FLOODTIDE_LVL_4              = 74;
	inline constexpr int FLOODTIDE_WHIRLPOOL          = 62;

	inline constexpr int HELLSWEEP_LVL_1              = 80;
	inline constexpr int HELLSWEEP_LVL_2              = 81;
	inline constexpr int HELLSWEEP_LVL_3              = 82;
	inline constexpr int HELLSWEEP_LVL_4              = 85;
	inline constexpr int HELLSWEEP_FIREMINE           = 83;
	inline constexpr int HELLSWEEP_FLAME              = 84;
	inline constexpr int HELLSWEEP_EXPLOSION          = 18;

	inline constexpr int MOVING_MISSILE               = 14;
	inline constexpr int MISSILE_EXPLOSION            = 17;

	inline constexpr int NEMESIS_MAIN                 = 34;
	inline constexpr int NEMESIS_CURLY                = 43;

	inline constexpr int ENERGY_FLAIL_LVL_1           = 49;
	inline constexpr int ENERGY_FLAIL_LVL_2           = 50;
	inline constexpr int ENERGY_FLAIL_LVL_3           = 51;
	inline constexpr int ENERGY_FLAIL_LVL_4           = 73;
	inline constexpr int ENERGY_FLAIL_LVL_4_SPAWN     = 95;

	inline constexpr int SHOTGUN_SHOT                 = 4;
	inline constexpr int BLADE_SLASH                  = 23;
	inline constexpr int ENEMY_CLEAR                  = 44;
	inline constexpr int WATER_DROPLET                = 61;
	inline constexpr int EXPLOSION                    = 16;
	inline constexpr int BIGGER_EXPLOSION             = 31;
	inline constexpr int PROCGEN_GUN_SPARK            = 67;
	inline constexpr int DUCK                         = 36;
	inline constexpr int UNUSED_SHIELD_HITBOX         = 46;
	inline constexpr int OMEGA_SELFHURT               = 77;
	inline constexpr int RED_CRYSTAL_PROJECTILE       = 78;
	inline constexpr int FALLING_SPIKE                = 24;
	inline constexpr int HEAVY_WEIGHT_BLADES          = 26;
	inline constexpr int ENERGY_PROJECTILE            = 47;
	inline constexpr int WHIMSICAL_STAR               = 57;

	inline constexpr int BLADE_SLASH_FAKE             = 79;
	inline constexpr int MANNAN_ATTACK_VFX            = 94;
	inline constexpr int NANITES_VFX                  = 86;
}

#endif // __COMMONDEFINES_H__