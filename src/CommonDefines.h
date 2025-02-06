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

	inline constexpr int REFILL_SAVE_FLAG_BEGIN   = 2630;
	inline constexpr int REFILL_SAVE_FLAG_END     = 2699;
	inline constexpr int TOROKO_BLEEDING_OUT      = 4301;

	inline constexpr int MINING_LASER_MODE_BEGIN  = 6900;
	
};

/// @brief The list of all current used flag traits. All flag traits are defined in "flagTraits" tag.
/// 
/// Flag traits are pieces of logic that can be attached to specific flag. They have a name and optionally a list of parameters to be fetched by game's logic.
namespace FlagTraits {
	/// @brief Gives player a nano-machine protection, that activates after getting hit. 
	/// @details This shield makes player invulnerable and increases their attack damage.
	/// @param[in] projectiles             The boolean toggle specifying if nano-protection should shoot projectiles. *Defaults to `false`.*
	/// @param[in] active_projectile_rate  The amount of frames between each projectile when shield is activated.
	/// @param[in] passive_projectile_rate The amount of frames between each projectile when shield is not active.
	/// @since 1.45
	inline constexpr const char* NANITES                       = "nanites";

	/// @brief Allows player to stop time when pressing Ability key.
	/// @note TIME_STOP_COEFFICIENTS NPC tag can be used to configure how much slowdown the entity will recieve during time stop effect.
	/// @since 1.45
	inline constexpr const char* TIMESTOP                      = "timestop";

	/// @brief Transforms Spur weapon into Invidia, a chargable beam of lightning power, that destroys both blocks and enemies.
	/// @todo Add parameters for weapon's icon and weapon's description.
	/// @since 1.45
	inline constexpr const char* INVIDIA                       = "invidia";

	/// @brief Empowers dashing ability when used with Blade weapon.
	/// @details Resulting dash move will be much faster and slice through enemies with Blade's projectile.
	/// @remark This trait's dash is \ref #BUFFED_DASH "empowered".
	/// @param[in] speed    The speed multiplier for Blade-empowered dash.
	/// @param[in] cooldown The cooldown multiplier for Blade-empowered dash. 
	/// @since 1.45
	inline constexpr const char* BLADE_DASH                    = "blade_dash";

	/// @brief Surrounds player with orbiting ducky stars, that shoot ducks at nearby enemies.
	/// @details The movement pattern of ducky stars is the same as with vanilla's whimsical stars.
	/// @note The duck counter can correspond to one of two things, depending on trait: health (#DUCK_HEALTH) or its own counter (#BUFFED_DUCKS).
	/// @since 1.45
	inline constexpr const char* DUCKS                         = "ducks";

	/// @brief Transforms Energy Flail weapon into Hook, that grapples player into walls with decent velocity with each swings.
	/// @todo Add parameters for weapon's icon and weapon's description.
	/// @since 1.45
	inline constexpr const char* HOOK_FLAIL                    = "hook_flail";

	/// @brief Changes Fireball weapon to have properties, typically associated with Magician's Fireball.
	/// @details Witch fireball has higher velocity, lifetime and explodes into pieces, when despawned.
	/// @todo Add parameters for velocity, bullets and lifetime.
	/// @since 1.45
	inline constexpr const char* WITCH_FIREBALL                = "witch_fireball";

	/// @brief Prompts Fireball projectiles to spew smaller flames when travelling.
	/// @param[in] rate The delay between each flame in frames.
	/// @since 1.45
	inline constexpr const char* PROPANE_TRAIL                 = "propane_trail";

	/// @brief Buffs Machine Gun weapon by making it deal more damage and apply more velocity even when not at level 3.
	/// @todo Add damage boost parameters.
	/// @since 1.45
	inline constexpr const char* BUFFED_MACHINE_GUN            = "buffed_machine_gun";

	/// @brief Allows to "control" Rocket Launcher's missile by holding Ability key in exchange for not being invincible.
	/// @since 1.45
	inline constexpr const char* CONTROLLED_ROCKET             = "controlled_rocket";

	/// @brief Empowers dashing ability with more momentum and ability to "bump" into enemies.
	/// @details Super dashe has larger hitbox when touching enemies, allowing easier connect for hits. On successful bonk, it gives 60 immunity frames and bounces player backwards.
	/// @note An enemy must have BONKABLE NPC trait, otherwise the super dash works as normal \ref #BUFFED_DASH "empowered dash".
	/// @param[in] explode     The boolean toggle for whenever player should "bonk" from enemy or "ram" through with spawning explosions. *Defaults to `false`.*
	/// @param[in] damage      The amount of damage done to enemy on successful bonk.
	/// @param[in] freeze      The boolean toggle for whenever successful bonk should freeze the enemy. *Defaults to `false`.*
	/// @param[in] freeze_time The amount of chilling frames given to enemy on successful freezing bonk.
	/// @since 1.45
	inline constexpr const char* SUPER_DASH                    = "super_dash";

	/// @brief Makes player invulnerable to spikes once certain velocity is reached.
	/// @param[in] threshold The horizontal threshold for spike evading in subpixels.
	/// @since 1.45
	inline constexpr const char* SPIKE_EVADING                 = "spike_evading";

	/// @brief Makes player's hitbox significantly smaller, allowing them to dodge with more leeway.
	/// @todo Add parameters for hitbox proportions.
	/// @since 1.45
	inline constexpr const char* WITCH_HITBOX                  = "witch_hitbox";

	/// @brief Makes player invulnerable and unable to damage aquatic entities.
	/// @note Everything that has AQUATIC NPC trait counts for this trait.
	/// @since 1.45
	inline constexpr const char* WATER_MOB_FRIEND              = "water_mob_friend";

	/// @brief Makes player destroy any "projectiles" they contact during the dash.
	/// @note Projectiles are entities that have 1 HP when trait is applied.
	/// @todo Probably should make a NPC trait for projectiles...
	/// @since 1.45
	inline constexpr const char* PROJECTILE_REPEL              = "dash_projectile_repel";

	/// @brief Allows player to hurt entities via holding Ability key and falling.
	/// @note The player will not take damage from enemies while stomping.
	/// @param[in] explosive The boolean toggle for whenever the damage method will be explosive or via special bullet.
	/// @since 1.45
	inline constexpr const char* STOMPING                      = "stomping";

	/// @brief Allows player to obtain level 4 on their weapons.
	/// @since 1.45
	inline constexpr const char* LEVEL_4_WEAPONS               = "level_4_weapons";

	/// @brief Increases player's immunity frames.
	/// @note When taking 1 damage, the immunity frames will be actually decreased.
	/// @param[in] modifier       The modifier for immunity frames.
	/// @param[in] modifier_light The modifier for immunity frames received for taking 1 damage.
	/// @since 1.45
	inline constexpr const char* IFRAMES_BOOST                 = "iframes_boost";

	/// @brief Prevents knockback from taking damage.
	/// @since 1.45
	inline constexpr const char* NO_KNOCKBACK                  = "no_knockback";

	/// @brief Saves player from one hit kills by setting their HP to 1 when taking too much damage.
	/// @note This will not save the player, if they are already at 1 HP.
	/// @since 1.45
	inline constexpr const char* KO_PREVENTION                 = "ko_prevention";

	/// @brief Spawns a big explosion when player is damaged.
	/// @since 1.45
	inline constexpr const char* EXPLODE_ON_HIT                = "explode_on_hit";

	/// @brief Allows player to survive one fatal hit per generated stage and stay at 0 HP.
	/// @note While at 0 HP, the player's momentum is increased.
	/// @param[in] speed_boost The modifier applied to player's speed, when they are at 0 HP.
	/// @param[in] flag_mark   The flag used to mark the usage of this trait. *Defaults to 4301.*
	/// @since 1.45
	inline constexpr const char* LAST_STAND                    = "last_stand";

	/// @brief Splits Snake's bullet into two bullets, going in synchronized patterns, and makes Snake weapon autofire.
	/// @note Split Snake bullets have halfed damage.
	/// @todo Split into #SNAKE_EMPOWER and SNAKE_AUTOFIRE.
	/// @since 1.45
	inline constexpr const char* SNAKE_EMPOWER                 = "buffed_snake";

	/// @brief Allows player to charge up their next sword weapon swing, giving it increased damage and empowered state on successful hit.
	/// @details When empowered, swords will swing significantly faster and fire additional projectiles.
	/// @todo Probably needs a lot more parameters, like charge duration, projectile type and damage multiplier.
	/// @since 1.45
	inline constexpr const char* SWORD_SHEATHING               = "sword_sheathing";

	/// @brief Enables hunger system, turning player's health bar into food bar.
	/// @details When active, the health bar will get unique, yellow-ish color and will slowly tick down. The special food pickups will be generated sometimes, when killing enemies.
	/// @todo Add parameters for hunger tick rate and food drop chance.
	inline constexpr const char* HUNGER_SYSTEM                 = "hunger_system";

	/// @brief Makes player's bullets knock enemies in opposite directions.
	/// @note Each bullet has their own knockback "force" and each entity has set knockback resistance.
	/// 
	/// @note Entities with IMMUNE_TO_KNOCKBACK NPC tag will not be affected.
	/// @since 1.45
	inline constexpr const char* KNOCKBACK                     = "knockback";

	/// @brief Allows player to teleport into empty spaces with mouse control.
	/// @note The player will be stunned during teleportation process.
	/// @todo Add parameters for stun time and cooldown.
	/// @since 1.45
	inline constexpr const char* MOUSE_TELEPORT                = "mouse_teleport";

	/// @brief Allows player to slide on the ground without losing momentum, while holding Ability key.
	/// @note When at max velocity, the jump height will be significantly increased.
	/// @todo Add parameters for jump height modifier and max speed.
	/// @since 1.45
	inline constexpr const char* ICE_SKATES                    = "ice_skates";

	/// @brief Makes player immune to ice physics on certain stages.
	/// @since 1.45
	inline constexpr const char* NO_SLIPPERY                   = "no_slippery";

	/// @brief Switches Curly ally's sprite sheet to more dark, sinister palette.
	/// @since 1.45
	inline constexpr const char* EXCERPT_ALLIES                = "excerpt_allies";

	/// @brief Makes player take less damage, when they are below certain amount of health.
	/// @todo Add parameters for threshold and damage reduction.
	/// @since 1.45
	inline constexpr const char* LOW_HP_RESIST                 = "low_hp_resist";

	/// @brief Allows player to enter special "sprinting dash" state, when holding Ability key.
	/// @details When "sprinting dash" is active, the player moves and turns much quicker, becomes much floatier and can double jump.
	///
	/// It also allows to dodge one direct hit with a cooldown.
	/// @todo Add parameters for hit cooldown and velocity.
	/// @since 1.45
	inline constexpr const char* DASH_SPRINT                   = "dash_sprint";

	/// @brief Binds duck counter to player's health and truncates all health interactions to correspond to 3 ducks.
	/// @note Those interactions include healing, taking damage or getting life upgrades.
	/// @see #DUCKS
	/// @since 1.45
	inline constexpr const char* DUCK_HEALTH                   = "duck_health";

	/// @brief Empowers dashing ability, allowing it to go through enemies and spikes without taking damage.
	/// @note Going through enemies deals at least 4 damage to them.
	/// @since 1.45
	inline constexpr const char* BUFFED_DASH                   = "buffed_dash";

	/// @brief Allows player to jump without ground, if they are next to the wall.
	/// @note Wall jump's initial momentum will always go away from wall.
	/// @since 1.45
	inline constexpr const char* WALL_JUMP                     = "wall_jump";

	/// @brief Allows player to ascend infinitely by just holding Jump button.
	/// @note With precise Jump holding, it is possible to float in place.
	/// @since 1.45
	inline constexpr const char* WITCH_ASCENDING               = "witch_ascending";

	/// @brief Makes dashing ability not experience any gravity, making it go in straight line no matter what.
	/// @since 1.45
	inline constexpr const char* DASH_FLOAT                    = "dash_float";

	/// @brief Makes dashing ability go extremely fast in one direction until hitting the wall, like a fierce cat leap.
	/// @remark This trait's dash is \ref #BUFFED_DASH "empowered" (but does 3 more damage).
	/// @param[in] speed    The speed multiplier for cat leaping dash.
	/// @param[in] cooldown The cooldown multiplier for cat leaping dash.
	/// @since 1.45
	inline constexpr const char* CAT_DASH                      = "cat_dash";

	/// @brief Reduces player's falling terminal velocity.
	/// @todo Add parameter for velocity multiplier.
	/// @since 1.45
	inline constexpr const char* CAT_FLOAT                     = "cat_float";

	/// @brief Makes dashing ability emit pink, self-transparent particles.
	/// @since 1.45
	inline constexpr const char* WITCH_DASH_VFX                = "witch_dash_vfx";

	/// @brief Makes dashing ability emit large amounts of small star trailing particles.
	/// @since 1.45
	inline constexpr const char* STAR_DASH_VFX                 = "star_dash_vfx";

	/// @brief Binds duck counter to separate counter that empties with each hit and fully regenerates on cooldown end.
	/// @details Basically it acts as 3 ducks that are bound to one player hit.
	/// @note Ducks bound to this trait shoot 2x slower.
	/// @see #DUCKS
	/// @since 1.45
	inline constexpr const char* BETTER_DUCKS                  = "better_ducks";

	/// @brief Allows player to boost with certain amount of base fuel.
	/// @warning This trait does nothing if it is not supplement by one of ways to boost: #BOOSTER_0_8 or #BOOSTER_2_0
	/// @see #BOOSTER_FUEL_BOOST
	/// @param[in] amount The base amount of fuel this trait will provide to booster. 
	/// @since 1.45
	inline constexpr const char* CAN_BOOST                     = "can_boost";

	/// @brief Sets boosting method to Booster 0.8 (straightforward accelerating upward force)
	/// @see #CAN_BOOST
	/// @since 1.45
	inline constexpr const char* BOOSTER_0_8                   = "booster_0_8";

	/// @brief Sets boosting method to Booster 2.0 (instant high speed in four cardinal directions)
	/// @param[in] no_slowdown A boolean toggle on whenever to preserve momentum on ending boosting or not.
	/// @see #CAN_BOOST
	/// @since 1.45
	inline constexpr const char* BOOSTER_2_0                   = "booster_2_0";

	/// @brief Allows player to flip their gravity with cooldown, making them fall towards ceiling instead of floor.
	/// @note The weapon shoot directions will also be reversed.
	/// @todo Add parameter for cooldown.
	/// @since 1.45
	inline constexpr const char* GRAVITY_FLIP                  = "gravity_flip";

	/// @brief Makes Fireball bullets explode on hitting enemies.
	/// @invariant The explosions will never overlap, allowing fireballs to go through more entities.
	/// @since 1.45
	inline constexpr const char* EXPLODING_FIREBALL            = "exploding_fireball";

	/// @brief Reduces falling press enemy's damage.
	/// @param[in] amount The resulting damage of press (*the initial damage is 13*).
	/// @since 1.45
	inline constexpr const char* PRESS_DMG_REDUCE              = "press_dmg_reduce";

	/// @brief Modifies life-span of bullets.
	/// @note This trait doesn't affect Floodtide or Hellsweep weapons.
	/// @param[in] boost The modifier to bullet lifespan.
	/// @since 1.45
	inline constexpr const char* BULLET_LIFE_BOOST             = "bullet_life_boost";

	/// @brief Allows player to jump in air once.
	/// @since 1.45
	inline constexpr const char* DOUBLE_JUMP                   = "double_jump";

	/// @brief Influences amount of chests on a stage.
	/// @param[in] base   The modifier to base amount of chests. *Defaults to 1.0.*
	/// @param[in] amount The static number added to amount of chests after other worldgen modifiers.
	/// @since 1.45
	inline constexpr const char* CHEST_BOOST                   = "chest_boost";

	/// @brief Defines flags for items that will have their overworld sprites hidden.
	/// @param[in] flags The array of flags to hide. The flag must correspond to visual equip.
	/// @since 1.45
	inline constexpr const char* HIDDEN_EQUIPS                 = "hidden_equips";

	/// @brief Allows player to breath underwater without depleting air.
	/// @note This trait also removes water splashing.
	/// @since 1.45
	inline constexpr const char* AMPHIBIOUS                    = "amphibious";

	/// @brief Defines weapons that will have their overworld sprites hidden.
	/// @param[in] ids The array of weapon IDs to hide.
	/// @since 1.45
	inline constexpr const char* HIDDEN_WEAPONS                = "hidden_weapons";

	/// @brief Modifies amount of booster fuel.
	/// @note This trait will not apply, if flag also has #CAN_BOOST trait.
	/// @param[in] amount The modifier for booster fuel.
	/// @since 1.45
	inline constexpr const char* BOOSTER_FUEL_BOOST            = "booster_fuel_boost";

	/// @brief Provides immunity to harmful water in certain stages.
	/// @since 1.45
	inline constexpr const char* ACID_IMMUNITY                 = "acid_immunity";

	/// @brief Gives player the lifesteal ability, rewarding them with hearts on dealing damage.
	/// @note Only entities with LIVING NPC trait can give hearts.
	/// @see #LIFESTEAL_MODIFIER
	/// @param[in] amount_per_heart The amount of damage needed to generate one heart pickup.
	/// @since 1.45
	inline constexpr const char* LIFESTEAL                     = "lifesteal";

	/// @brief Modifies the damage needed to generate a heart from lifestealing.
	/// @param[in] amount The modifier for damage per one heart, the lower number will make it require more damage.
	/// @since 1.45
	inline constexpr const char* LIFESTEAL_MODIFIER            = "lifesteal_modifier";

	/// @brief Allows player to jump on spikes to avoid them.
	/// @remark This trait's jump will count as usage of @ref #DOUBLE_JUMP "double jump".
	/// @note The jump velocity will be lower, when activating this trait.
	/// @todo Add parameter for jump velocity modifier.
	/// @since 1.45
	inline constexpr const char* JUMP_ON_SPIKES                = "jump_on_spikes";

	/// @brief Removes the mechanic with spikes scaling their damage with player's health.
	/// @invariant Therefore, the spike's base damage will be always 5.
	/// @since 1.45
	inline constexpr const char* SPIKE_DAMAGE_RESIST           = "spike_damage_resist";

	/// @brief Makes player's attacks chill enemies, making them move and attack slower.
	/// @param[in] chance      The chance for enemy to be chilled on attack. *Used as 1/X.*
	/// @param[in] rate        The amount of freeze frames each successful chill gives.
	/// @param[in] cap         The maximum amount of freeze amounts one enemy can have.
	/// @param[in] resist_npcs The entity IDs that will have reduced amount of chilling applied.
	/// @param[in] resist_rate The modifier to `rate` when chill is applied to resistant entities.
	/// @since 1.45
	inline constexpr const char* CHILL                         = "chill";

	/// @brief Allows player to stick to a wall with holding Ability key, nullifying their momentum.
	/// @since 1.45
	inline constexpr const char* WALL_STICKY                   = "wall_sticky";

	/// @brief Makes player attacks spawn additional energy projectiles.
	/// @param[in] chance The chance for energy projectile to spawn. *Used as 1/X.*
	/// @todo Add parameters for range and projectile.
	/// @since 1.45
	inline constexpr const char* ENERGY_PROJECTILE_ON_HIT      = "energy_projectile_on_hit";

	/// @brief Allows to place blocks outside of point-blank with mouse controls.
	/// @note The visual representation of available range will be visible when holding Polar Star weapon.
	/// @param[in] distance The max distance for placing blocks in blocks.
	/// @since 1.45
	inline constexpr const char* RANGED_BLOCK_PLACE            = "ranged_block_place";

	/// @brief Makes Polar Star weapon account for player's momentum, making it far less likely to miss.
	/// @since 1.45 
	inline constexpr const char* PRECISE_PLACEMENT             = "precise_placement";

	/// @brief Allows to manually stop Rocket Launcher's missile with Ability key.
	/// @since 1.45
	inline constexpr const char* MANUAL_ROCKET_STOP            = "manual_rocket_stop";

	/// @brief Makes player's turn around much quicker by applying extra velocity.
	/// @param[in] effect The multiplier to player's velocity when turning around. *Defaults to 1.*
	/// @since 1.45
	inline constexpr const char* TIGHTER_TURNING               = "tighter_turning";

	/// @brief Changes Nemesis's bullets to spend its "hitpoints" to break blocks instead of instantly disappearing.
	/// @since 1.45
	inline constexpr const char* MULTI_BLOCK_BREAKING          = "multi_block_breaking";

	/// @brief Moves Nemesis bullet's initial position upwards.
	/// @remark Synergizes with #MULTI_BLOCK_BREAKING to allow breaking 2-block-wide corridors.
	/// @todo Add parameters for exact value.
	/// @since 1.45
	inline constexpr const char* OFFSET_NEMESIS                = "offset_nemesis";

	/// @brief Modifies the amount of bullets certain weapons can have on a screen.
	/// @note The currently supported weapons are `machine_gun`, `fireball` and `bubbler`.
	/// @tparam gun_name The modifier of bullet limit for specified weapon. *Defaults to `1.0`.*
	/// @since 1.45
	inline constexpr const char* BULLET_LIMIT_BOOST            = "bullet_limit_boost";

	/// @brief Allows certain weapons to shoot an extra bullet.
	/// @note The currently supported weapons are `machine_gun`, `gatling_gun` and `bubbler`.
	/// @note Gatling Gun will have its bullets increased by 1.5x on successful proc.
	/// @tparam gun_name The chance for extra bullet to be shot. *Used as 1/X.*
	/// @since 1.45
	inline constexpr const char* EXTRA_BULLET_CHANCE           = "extra_bullet_chance";

	/// @brief Modifies the firerate of certain weapons.
	/// @note The currently supported weapons are `machine_gun`, `gatling_gun` and `bubbler`.
	/// @tparam gun_name The modifier of firerate for specified weapon. Positive modifier means faster firerate, negative modifier means slower firerate. *Defaults to 0.0.*
	/// @warning The modifier for any given weapon cannot be more than 1.0.
	/// @since 1.45
	inline constexpr const char* FIRERATE_BOOST                = "firerate_boost";

	/// @brief Modifies the velocity and inaccuracy of Machine Gun and Bubbler.
	/// @note All parameters default to 1.0.
	/// @param[in] machine_gun_velocity   The modifier for base speed of Machine Gun's bullet. 
	/// @param[in] machine_gun_inaccuracy The modifier for inaccuracy of Machine Gun's bullet. 
	/// @param[in] bubbler_velocity       The modifier for base speed of Bubbler's bullet. 
	/// @param[in] bubbler_inaccuracy     The modifier for inaccuracy of Bubbler's bullet.
	/// @since 1.45
	inline constexpr const char* INACCURATE_VELOCITY_MODIFIER  = "inaccurate_velocity_modifier";

	/// @brief Makes player instantly go to terminal falling velocity, when holding Ability key.
	/// @since 1.45
	inline constexpr const char* ACCELERATE_FALL               = "accelerate_fall";

	/// @brief Allows player to run faster than normal, including fancy particles.
	/// @param[in] speed_multiplier     The modifier for max running speed.
	/// @param[in] acceleration         The modifier for acceleration rate.
	/// @param[in] particle_threshold   The fraction of running speed at which running particles should appear.
	/// @param[in] air_speed_multiplier The modifier for max running speed in the air. *Defaults to `1.0`.*
	/// @param[in] air_acceleration     The modifier for acceleration rate in the air. *Defaults to `1.0`.*
	/// @since 1.45
	inline constexpr const char* SPEED_BOOTS                   = "speed_boots";

	/// @brief Nullifies damage taken by player, if it is below certain value, with a chance.
	/// @param[in] threshold The value, below which the damage can be nullified. *Inclusive.*
	/// @param[in] chance    The chance for damage to be nullified. *Used as 1/X.*
	/// @since 1.45
	inline constexpr const char* LOW_DAMAGE_NULLIFY            = "low_damage_nullify";

	/// @brief Makes player rise out of water instead of sinking, when not holding the Ability key.
	/// @todo Add parameter for rising speed modifier.
	/// @since 1.45
	inline constexpr const char* WATER_FLOATING                = "water_floating";

	/// @brief Allows to use Map System screen, when pressing Map key.
	/// @since 1.45
	inline constexpr const char* MAPPING                       = "mapping";

	/// @brief Decreases the amount of experience lost on taking damage.
	/// @param[in] modifier The modifier for experience loss reduction.
	/// @since 1.45
	inline constexpr const char* EXP_SAVING                    = "exp_saving";

	/// @brief Recharges Polar Star and Nemesis weapons when taking damage.
	/// @param[in] amount The base modifier for resulting weapon charge.
	/// @since 1.45
	inline constexpr const char* MINING_CHARGE_FROM_DAMAGE     = "mining_charge_from_damage";

	/// @brief Makes Polar Star and Nemesis slowly recharge passively.
	/// @param[in] polar_star The amount of frames needed for a single charge to be restored for Polar Star.
	/// @param[in] nemesis    The amount of frames needed for a single charge to be restored for Nemesis.
	/// @since 1.45
	inline constexpr const char* MINING_PASSIVE_CHARGE         = "mining_passive_charge";

	/// @brief Modifies the rate of Spur weapon charging mechanic.
	/// @param[in] modifier The modifier for Spur charge rate. *Defaults to 1.0.*
	/// @since 1.45
	inline constexpr const char* SPUR_CHARGE_BOOST             = "spur_charge_boost";

	/// @brief Modifies the amount of bullets certain weapons can have on a screen.
	/// @note The currently supported weapons are `machine_gun`, `gatling_gun` and `bubbler`.
	/// @tparam gun_name The modifier of firerate of specific weapon.
	/// @since 1.45
	inline constexpr const char* WEAPON_RECHARGE_BOOST         = "weapon_recharge_boost";

	/// @brief Protects user from taking damage from lava liquid in certain stages for short amount of time.
	/// @param[in] amount The amount of lava protection time in frames.
	/// @since 1.45
	inline constexpr const char* LAVA_PROTECTION               = "lava_protection";

	/// @brief Extends the amount of time player can breath underwater without drowning.
	/// @param[in] modifier The modifier for breath length.
	/// @since 1.45
	inline constexpr const char* BREATH_EXTENSION              = "breath_extension";

	/// @brief Adds bubbler visual effect, when player is underwater.
	/// @since 1.45
	inline constexpr const char* UNDERWATER_BUBBLE_VFX         = "underwater_bubble_vfx";

	/// @brief Decreases the drowning damage rate.
	/// @param[in] modifier The modifier for drowning rate.
	/// @since 1.45
	inline constexpr const char* DROWNING_SLOWDOWN             = "drowning_slowdown";

	/// @brief Modifies the maximum amount of ammo mining weapons can have.
	/// @param[in] modifier The modifier for maximum amount of ammo. *If set to 0, the ammo will be infinite.*
	/// @since 1.45
	inline constexpr const char* MINING_STORAGE_BOOST          = "mining_storage_boost";

	/// @brief Modifies the jump velocity and charged jump force.
	/// @param[in] modifier        The modifier for base jump velocity.
	/// @param[in] charge_modifier The modifier for charged jump force amount.
	/// @since 1.45
	inline constexpr const char* JUMP_BOOST                    = "jump_boost";

	/// @brief Allows player to glide with Ability key, making them fall significantly slower and fly faster.
	/// @param[in] max_speed The modifier for max speed during gliding.
	/// @param[in] acceleration The modifier for acceleration rate during gliding.
	/// @param[in] fall_slowdown The modifier for falling speed reduction.
	/// @since 1.45 
	inline constexpr const char* GLIDING                       = "gliding";

	/// @brief Increases the swimming momentum.
	/// @param[in] modifier The modifier for swimming momentum.
	/// @since 1.45
	inline constexpr const char* SWIM_BOOST                    = "swim_boost";

	/// @brief Allows player to use charged jump mechanics when swimming.
	/// @since 1.45
	inline constexpr const char* CHARGED_SWIMS                 = "charged_swims";

	/// @brief Empowers dashing ability when used underwater.
	/// @details Resulting dash move has short cooldown by default and moves through water with ease.
	/// @remark This trait's dash is \ref #BUFFED_DASH "empowered".
	/// @param[in] speed    The speed multiplier for underwater dash.
	/// @param[in] cooldown The cooldown multiplier for underwater dash. 
	/// @since 1.45
	inline constexpr const char* UNDERWATER_DASH               = "underwater_dash";

	/// @brief Modifies the damage player does with their attacks.
	/// @param[in] modifier The modifier for attack damage.
	/// @since 1.45
	inline constexpr const char* ATTACK_DAMAGE_MODIFIER        = "attack_damage_modifier";

	/// @brief Modifies the amount of immunity frames player gets, when damaged.
	/// @param[in] modifier The modifier for amount of immunity frames.
	/// @since 1.45
	inline constexpr const char* UNIVERSAL_IFRAMES_MODIFIER    = "universal_iframes_modifier";

	/// @brief Modifies the properties of dashing ability.
	/// @param[in] duration The duration multiplier for underwater dash.
	/// @param[in] speed    The speed multiplier for underwater dash.
	/// @param[in] cooldown The cooldown multiplier for underwater dash. 
	/// @since 1.45
	inline constexpr const char* DASH_MODIFIER                 = "dash_modifier";

	/// @brief Spawns specific entities (usually floating medals) at beginning of each stage.
	/// @note The set NPCs will be automatically spawned or despawned when flag with this trait will have its state changed.
	/// @param[in] ids The array of NPC IDs that will spawn.
	/// @since 1.45
	inline constexpr const char* MEDAL_SPAWN                   = "medal_spawn";

	/// @brief Spawns little man ally at beginning of each stage.
	/// @note The little mans will be automatically spawned or despawned when flag with this trait will have its state changed.
	/// @param[in] amount The amount of little mans that will spawn.
	/// @since 1.45
	inline constexpr const char* LITTLE_MAN_SPAWN              = "little_man_spawn";

	/// @brief Spawns Curly Brace ally, that sits at a back and shoots specific bullets at same time as player.
	/// @note The Curly ally and her gun will be automatically spawned or despawned when flag with this trait will have its state changed.
	/// @param[in] bullet       The ID of bullet for Curly ally to shoot.
	/// @param[in] bullet_limit The maximum amount of Curly ally's bullets on screen.
	/// @since 1.45
	inline constexpr const char* CURLY_SPAWN                   = "curly_spawn";

	/// @brief Spawns special star bullet, that spins around player and latches onto things, at beginning of each stage.
	/// @note The star bullet will be automatically spawned or despawned when flag with this trait will have its state changed.
	/// @since 1.45
	inline constexpr const char* WHIMSICAL_STAR                = "whimsical_star";

	/// @brief Removes slowdown from being underwater.
	/// @since 1.45
	inline constexpr const char* UNHINDERED_DIVING             = "unhindered_diving";

	/// @brief Allows player to swim upwards with just holding Jump underwater.
	/// @since 1.45
	inline constexpr const char* UNLIMITED_SWIM                = "unlimited_swim";

	/// @brief Causes player to emit flame bullets while dashing.
	/// @param[in] rate The duration in frames between each flame. *Defaults to `3`.*
	/// @since 1.45
	inline constexpr const char* DASH_FLAME_TRAIL              = "dash_flame_trail";

	/// @brief Makes little man ally try to move and jump in same direction as player.
	/// @since 1.45 
	inline constexpr const char* CONTROLLED_LITTLE_MAN         = "controlled_little_man";

	/// @brief Makes little man ally shoot certain bullets when trying to attack.
	/// @param[in] bullet The ID of bullet for little man ally to shoot.
	/// @param[in] rate   The time between each bullet little man ally shoots in frames. *Defaults to `9`.*
	/// @since 1.45
	inline constexpr const char* SHOOTING_LITTLE_MAN           = "shooting_little_man";

	/// @brief Reduces damage taken by player with substraction.
	/// @note This trait cannot reduce the damage beyond 1.
	/// @param[in] amount The amount of damage substracted from taken damage.
	/// @since 1.45
	inline constexpr const char* FLAT_DAMAGE_REDUCTION         = "flat_damage_reduction";

	/// @brief Makes booster's particles deal damage.
	/// @param[in] range  The radius of boosting particle's hitbox in pixels.
	/// @param[in] damage The amount of damage boosting particle does per frame.
	/// @since 1.45
	inline constexpr const char* BOOSTER_DAMAGE                = "booster_damage";

	/// @brief Allows player to cheat death one time.
	/// @details When the trait triggers, the Curly ally will be removed, the special flag will be set and the item, corresponding to flag with this trait, will be removed from inventory.
	/// @note Curly ally from #CURLY_SPAWN is not necessary for this trait to activate.
	/// @param[in] mark_flag The flag used to track this trait's activation. *Defaults to `1070`.*
	/// @since 1.45
	inline constexpr const char* REVIVAL_FROM_CURLY            = "revival_from_curly";

	/// @brief Makes nearby heart pickups travel towards player.
	/// @param[in] distance The distance in pixels for hearts to start gravitating towards the player.
	/// @param[in] speed    The modifier for speed, at which hearts move.
	/// @since 1.45 
	inline constexpr const char* HEART_MAGNET                  = "heart_magnet";

	/// @brief Adds a chance for any weapon to not consume its ammo.
	/// @param[in] chance The chance for saving ammo. *Used as 1/X.*
	/// @since 1.45
	inline constexpr const char* SAVING_AMMO_CHANCE            = "saving_ammo_chance";

	/// @brief Makes dashing move refresh double jump counter.
	/// @since 1.45
	inline constexpr const char* DASH_REFRESHES_JUMP           = "dash_refreshes_jump";

	/// @brief Causes player to enter floaty state after dashing for short period of time.
	/// @param[in] speed_multiplier The modifier for maximum falling velocity during dash glide state.
	/// @param[in] duration         The duration of this mechanic in frames.
	/// @since 1.45
	inline constexpr const char* GLIDING_AFTER_DASH            = "gliding_after_dash";

	/// @brief Allows player to "lock" their shooting direction while holding Ability key.
	/// @since 1.45
	inline constexpr const char* STRAFE                        = "strafe";

	/// @brief Makes all entities on screen freeze in place when pressing Ability key.
	/// @param[in] amount The amount of freeze frames each character recieves.
	/// @since 1.45
	inline constexpr const char* SCREEN_FREEZE                 = "screen_freeze";

	/// @brief Makes little man ally shoots specific bullets at same time as Curly ally.
	/// @param[in] bullet       The ID of bullet for little man ally to shoot.
	/// @param[in] bullet_limit The maximum amount of little man ally's bullets on screen.
	/// @since 1.45
	inline constexpr const char* LITTLE_N_CURLY_TEAM_UP        = "little_n_curly_team_up";

	/// @brief Allows to break blocks outside of point-blank with mouse controls.
	/// @note The visual representation of available range will be visible when holding Nemesis weapon.
	/// @param[in] distance The max distance for breaking blocks in blocks.
	/// @since 1.45
	inline constexpr const char* RANGED_BLOCK_BREAK            = "ranged_block_break";

	/// @brief Creates a freezing ring around player, when they hold mining weapons.
	/// @param[in] distance The range of freeze ring in pixels.
	/// @param[in] rate     The amount of freeze frames applied inside the ring.
	/// @since 1.45
	inline constexpr const char* MINING_FREEZE_AURA            = "mining_freeze_aura";

	/// @brief Turns dashing ability into short-distance teleportation.
	/// @details Resulting dash move can go through walls and any danger.
	/// @param[in] distance The length of teleportation dash in blocks.
	/// @param[in] damage   The amount of damage entities in dash's trajectory take.
	/// @since 1.45
	inline constexpr const char* TELEPORT_DASH                 = "teleport_dash";

	/// @brief Increases the jump force with player's horizontal velocity.
	/// @note The modifier is multiplied with horizontal velocity, measured in 1/512s of a pixel.
	/// @param[in] modifier The base modifier for velocity increase.
	/// @since 1.45
	inline constexpr const char* VELOCITY_BASED_JUMP           = "velocity_based_jump";

	/// @brief Adds additional ammo to specific weapons at beginning of new stage.
	/// @warning The parameters have to have same length for this trait to work.
	/// @param[in] weapons The IDs for weapons for their max ammunition to be increased.
	/// @param[in] ammo    The amount of max ammo each weapon ID would get.
	/// @since 1.45
	inline constexpr const char* STAGE_COMPLETE_AMMO_BOOST     = "stage_complete_ammo_boost";

	/// @brief Modifies the amount of experience player gets from pickups.
	/// @param[in] modifier The modifier for the experience amount.
	/// @since 1.45
	inline constexpr const char* EXPERIENCE_BOOST              = "experience_boost";

	/// @brief Modifies the amount of damage player gets from enemy hits.
	/// @param[in] modifier The modifier for the amount of taken damage.
	/// @since 1.45
	inline constexpr const char* TAKEN_DAMAGE_BOOST            = "taken_damage_boost";

	/// @brief Modifies the max speed of the player.
	/// @param[in] modifier The modifier for player's max speed.
	/// @since 1.45
	inline constexpr const char* MAX_XSPEED_LIMIT              = "max_xspeed_limit";

	/// @brief Makes puppy entities (ID 126) not hurt the player.
	/// @since 1.45
	inline constexpr const char* PUPPY_FRIEND                  = "puppy_friend";

	/// @brief Makes life capsules not spawn, regardless of "no life capsules" config setting.
	/// @since 1.45
	inline constexpr const char* NO_LIFE_CAPSULES              = "life_capsules";

	/// @brief Randomizes a lot of gameplay aspects every frame.
	/// @remark Every parameter used by this trait is a *range*: it has two variants: `min` and `max`, both defined as percentages. So, to describe `acceleration` trait parameter, for example, you need to define both `min_acceleration` and `max_acceleration`.
	/// @param[in] dealt_damage The range for player's attack damage.
	/// @param[in] damage       The range for player's taken damage.
	/// @param[in] shock        The range for player's immunity frames.
	/// @param[in] exp          The range for amount of experience player gets from pickups.
	/// @param[in] start_life   The range for player's starting life.
	/// @param[in] limit        The range for player's max movement speed.
	/// @param[in] running      The range for player's max running speed.
	/// @param[in] jump         The range for player's jumping velocity.
	/// @param[in] gravity      The range for player's falling velocity.
	/// @param[in] friction     The range for player's ground friction.
	/// @param[in] acceleration The range for player's acceleration.
	/// @since 1.45
	inline constexpr const char* TOTAL_RANDOMNESS              = "total_randomness";

	/// @brief Changes walking animation rate every frame to create stuttering effect.
	/// @since 1.45
	inline constexpr const char* ANIMATION_STUTTERING          = "animation_stuttering";

	/// @brief Modifies the amount of score from each run.
	/// @param[in] amount The modifier for the score.
	/// @since 1.45
	inline constexpr const char* SCORE_MULTIPLIER              = "score_multiplier";

	/// @brief Configures the player's walking on ground sound.
	/// @param[in] id The sound ID for walking. *Defaults to ID 24.*
	/// @since 1.45
	inline constexpr const char* WALKING_SOUND                 = "walking_sound";
	
	/// @brief Increases acceleration when flying with Robotic Equipment weapon.
	/// @param[in] speed_boost The modifier for acceleration during Robotic Equipment flights.
	/// @since 1.45
	inline constexpr const char* UNWIELDY_SPARK_FLYING         = "unwieldy_spark_flying";
}

namespace NPCTraits {
	inline constexpr const char* BONKABLE                      = "bonkable";
	inline constexpr const char* AQUATIC                       = "aquatic";
	inline constexpr const char* ETHEREAL                      = "ethereal";
	inline constexpr const char* LIVING                        = "living";
	inline constexpr const char* IMMUNE_TO_LIFE_MODIFIERS      = "immune_to_life_modifiers";
	inline constexpr const char* BOSS                          = "boss";
	inline constexpr const char* IMMUNE_TO_KNOCKBACK           = "immune_to_knockback";
	inline constexpr const char* TIME_DILATION                 = "time_dilation";
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