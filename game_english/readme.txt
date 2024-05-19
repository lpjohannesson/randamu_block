<Doukutsu Randamu> by Trashbox Bobylev and friends

In short, procedurally generated Cave Story experience, you might call it rogue-like. If you die, you start over.

Randamu.exe - Main Game Program

Profile.json - Save data (appears after first save, gets deleted after dying or reseting)

last_seed.txt - Tracks every new seed and version on which the seed is used
rankings.json - GZIP-encrypted data for last runs (deaths or wins)
game.log - Records various information during gameplay, such as time it takes to generate the stage.

seed.txt (needs to be created) - Custom Seed file, accepts numbers up to (2^32-1) or up to 9 ASCII letters.

highscore.dat - Highscore (appears after run ends, either by dying or defeating the Doctor)
For custom seeds the highscores are recorded in score folder, with files named after seed.

resources folder - contains override/resource packs. All of them can be enabled/disabled with list.json which generates in resources folder. The order in which resource packs are applied is the order of list.json's info: the last one will override all other packs.
Each resource override folder should contain info.json (with name and description as fields) in order to be activated. Any file from this folder will replace a file from data folder when game will attempt to load this file.

- By default game runs in 60fps, 852x480 widescreen and smooth scrolling.
- Map System is available from start, press W.
- You can swim by pressing Z underwater.
- You can charge forward, or dash, by pressing Dash key, by default it's Left Shift.
- Certain items require Ability key to be binded, by default it's C.
- You can speed up TSC scripts with pressing Turbo Text button, by default it's B. 
- Use Ability key in inventory to trash items into the void.
- Use Polar Star to build blocks.
- Use Nemesis to break blocks.
- Hold Up while jumping to perform stronger jump.
- Use number keys to select your weapons without cycling.
- Life Capsules have the limit, after which they stop appearing on the map.
- Some items cannot be obtained if playing as certain custom characters.
- Some extra info can be seen on Pause screen if Extended Pause Info is enabled in options (off by default).
- Important objects are shown on Minimap by different colors:
	Aggressive red: Life Capsule
	Dark orange: Chest
	Pinkish color: Refill station
	Cyan: Star
	Faint gold: Door or Boss
	Blue: Save Point
- You can't load your save if boss is alive, or Quote is dead, or when you are at King's Table.
- Machine Gun and Bubbler have increased firerate, damage and spread.
- Fireball has increased momentum and decreased gravity.
- Ranking entries are sorted by score.

Challenges/modifiers system (config.json):
To set the value, put it as ""name": value" in config.json (it's created automatically).
- 'no_exp': when set to true, enemies will stop dropping exp (they still can drop hearts)
- 'health_limit': limits how much life capsules are placed on each level
- 'chest_limit': limits how much chests are placed on each level, if more than 2, forces amount of chests to spawn
- 'refill_limit': limits how much refill are placed on each level, if more than 3, forces amount of life capsules to spawn
- 'fast_run': when set to true, makes bosses appear at floors 2 and 4, and Doctor at floor 6, instead of 4/8/10
- 'space_modifier': roughly controls how many solid blocks are on level, 1 is default, 0.5 is roughly 2.5x more empty space, 2 is roughly 3x less empty space
- 'spike_modifier': a multiplier for how many spikes are placed on level
- 'mob_modifier': a multiplier for how many monsters are placed on level
- 'no_health': when set to true, no life capsules will spawn at all
- 'spike_kill': when set to true, presses and spikes kill regardless of health, for extra sadism
- 'one_hit': when set to true, you die in one hit and health bar disappears
- 'no_placing': when set to true, removes Polar Star from starting inventory
- 'no_breaking': when set to true, removes Nemesis from starting inventory
- 'movement_speed': multiplier for character's movement speed
- 'jump_speed': multiplier for character's jump force
- 'timer': when set to true, puts 290 Counter at bottom left and removes all save points, allows your time to be saved after Doctor
- 'no_map': when set to true, removes ability to see minimap
- 'tileset': locks levels to certain tileset. The name must be case-sensitive. To get the name, check tileset info with Extended Pause Info on.
- 'gimmick': locks levels to certain gimmick. The name must be case-sensitive. To get the name, check gimmick info with Extended Pause Info on.
- 'darkness': when set to true, limits player's vision to circle around them.
- 'full_clear': when set to true, doors require 20 stars.
- 'organya_mute': when set to true, only music that will play will be silence.
- 'charge_jump': enables/disables charge jump mechanic.
- 'advanced_arms': enables/disables advanced arms selection mechanic.
- 'pitch_randomizer': enables/disables the pitch randomization function, intended to make Organya soundtrack more varied to listen to.
- 'debug': enables/disables debug mode. In debug mode, you can move in every direction without gravity, enemies die instantly, you take no damage, use no ammo and doors open without requiring stars.
- 'stage_debug": enables/disables dumping the order of level generation stages into game log.
- 'speedrunMetadata': enables/disables creating "speedrunData.json" with some pointers on every config reload.
- 'hitboxes': enables/disables overlay that shows entity hitboxes.
- 'legacyUI': enables/disables programmer art designs used in early releases. Cannot be toggled off on April 1st.
- 'oneDamage': enables/disables the configuration to make every weapon deal one damage per hit at max.
- 'secondCampaign': enables/disables the starting point being levels after Doctor's boss fight, also doubles starting HP to compensate.
- 'superHotMode': enables/disables the game mode where holding any keys stops the flow of time.
- 'randomSizeMode': enables/disables the ability for generated levels to have variable size, not just 118x118.
- 'sheepCharacter': enables/disables the character being permanently a sheep.
- 'character': defines what custom character to use.
Character codes:
- CurlyBrac - Curly Brace
- KingTable - King
- FluffMimi - Toroko
- Sakamoto - Sue
- Raymundo - Misery
- Upntscale - Lethrys Quote
- Russia - Hunter
- SaraACSS - Sara
- ColdWorkE - Lily
- WTFEvilQu - Excerpt
- Jamark - Bracket
- Dog - Puppy
- Magician - Magician
- LuigiGuy - Duckman
- Meme - Quate
- God - Mahin
- AmeManga - Ame
- SamHogan - ProcGen
- LunarSha - Serenity

Credits:
- Cave Story: Daisuke "Pixel" Amaya
- Based on: CSE2 Enhanced by Clownacy and GabrielTFS
- Music: Cory Welch, Tinned Duck, 2dbro, Zenix, txin, mLurker, Alula, Safusaka, zeroflafi, Dredged Crusader, AquaDoesStuff, yellow, Raymundo
- Used sprites: ZarroTsu, AsperDev, Tert, Taiki, MintyFresh, Raymundo, lolkit
- Coding help: Autumn Hazel
- Bracket OC: Jamark
- Improved Map System: Ikuyo and Onyx
- Monogram font: datagoblin
- Zlib for C++: Piti118, Deepak Bandyopadhyay, Lutz Kettner, zlib team
- JSON decoding: Niels Lohmann and contributors
- Update checking: quyse, Mark Lakata and libcurl team
- Crossovers: 2dbro and Cybryll/Kim Tyranto
- Translation help: Fluffy-Fuwafuwa
- Ideas: NethoWarrior, eli, Tert, ColdCallerLoopy, Zierin705, eviled, Thyme Warp, duncathan, Daedily, Woddles, rde, CyberSlash, IdioticBaka1824, Ryla, yellow

Changelog:
1.44 (Seconds & Moments With You) (May 20th 2024):
New content:
- Added default dash:
	* it is weaker and doesn't grant immunity frames, so it cannot be used to go through enemies and spikes
	* Silver Locket allows dashes to have immunity frames
	* characters that had dashes with immunity frames before this update will still have them
- Overhauled second campaign:
	* now has its own tileset art, backgrounds and stage-specific music
	* tilesets show different names when appearing in second campaign to accompany this
	* "Start from Level 11" is now named "Second Campaign Mode", properly uses post-Doctor's tileset rotation and gives double health to player to compensate for harder enemies at the start
- Added Experimental Resource Pack (or "aspermental") as package-in:
	* it includes resprites and reimaginations of game's assets by AsperDev, that clash with current art style
	* feedback on how it fits and how good it is will be appreciated

Balance changes:
- Buffed Snake:
	* increased lvl3 damage from 5 to 6
- Nerfed Curly's Underwear:
	* decreased damage from 4/4 to 8/1
	* decreased Curly's Nemesis bullet's lifespan from 20 frames to 17 frames
- Nerfed Defended gimmick:
	* now is less likely to appear
	* reduced amount of firethrowers from 15-30 (60-115 in 2nd campaign) to 10-18 (30-75)
	* reduced amount of fire turrets from 30-40 to 15-25
- Adjusted some knockback modifiers (mostly to make associated weapons' knockback weaker).
- Reworked Iron Bond:
	* now provides active healing: each 24 dealt damage a heart drops at entity
	* overkill damage doesn't contribute to this healing
	* lifestealEntities tag provides all valid entities for tracking and dropping hearts
- Nerfed Ballos fight's red crystal helper attack rate from every 2-16 frames to every 7-28 frames.
- Buffed Super Booster:
	* increased DPS by 2x
	* vertical boosts now damage enemies as well
	* increased hitbox from 8x8 to 12x12 pixels
- Changed Machine Gun:
	* reduced bullet lifespan from 22 to 16 frames
	* increased bullet hitboxes
- Buffed Energy Flail:
	* flail's moving speed is significantly increased
	* decreased gravity-affected damage by 2x, but decreased its static immunity frames from 15 to 6
	* energy bullets only appear from lvl2 flail, but changed the amount of bullets from 10 every 50 frames to 1-2 every 4 frames
	* slightly decreased the lifespan of energy bullets
- Nerfed Bubbler:
	* lvl1 bullets now explode into 5-10 pellets instead of 10-30
	* decreased lvl1 bullet lifespan from 250 to 100 frames
- Reworked Water Gun:
	* renamed into Water Sprayer
	* increased max ammo from 25 to 100
	* now shoots homing exploding bubbles as primary attack
	* now has charge (hold X) attack that brings the water shield
	* water shield now gives 120 frames of wetness to enemies that touch it
	* water shield can expire after 250 frames and explode into 16 smaller exploding bubbles
- Refactored/buffed Nano Quiver:
	* now takes place in 100x100 pixel circle around the enemy
	* removed the "too far" restriction on quiver projectile's position, leaving only too close restriction
- Nerfed Timeline Ring:
	* increased timestop duration from 5.83 s to 6.33 s
	* increased timestop cooldown from 30 s to 40 s
- Buffed Nanites Absorber:
	* decreased cooldown from 60 s to 40 s
	* decreased duration from 4 s to 3 s but increased damage boost from +66% to +100%
- Removed Resonant difficulty's health increase for enemies and bosses.
- Changed Resonant Press AI:
	* now retracts back to ceiling instead of hopping like a crazy critter
- Nerfed some late game enemies:
	* reduced core base's HP from 210 to 140
	* reduced laser gaudi's HP from 110 to 70
	* reduced shuriken gaudi's HP from 100 to 85
	* reduced chaos frog's HP from 85 to 55
	* reduced dragon's HP from 150 to 110
	* reduced laser frog's HP from 200 to 115
	* reduced shadow toroko's HP from 250 to 140
	* reduced shadow misery's HP from 175 to 95
	* reduced shadow wildsery's HP from 125 to 90
- Damage-negating effects no longer deplete weapon's EXP and player's air supply.
- Added Ballos into list of enemies possible to hit with Copper Bulwark.

Misc/Quality of Life:
- Turbo Text button is now usable in inventory and Miracle Matter crafting screens.
- Increased item limit by 3.
- Configuration's JSON file is now properly spaced out.
- Darkness-specific tileset variants now show with "darkness" config option.
- Reimplemented female-specific character sounds and added ones to Misery and Puppy.
- Life Capsules and Energy Capsules are no longer openable during boss fights.
- Missile Launcher no longer allows player to go through map bounds in sky and Jethead stage.
- Updated Hekichu Caverns tileset (by Tert).
- Daily runs can no longer be replayed upon death.
- Air Tank is no longer named "Curly's Air Tank" in inventory.
- Added a bit for second campaign existence in Red Crystal claim after Doctor's fight.
- Moved some options from Gameplay section into General section.

Bug fixes:
- Badge of Love's Curly ally no longer comes back to life after being sacrified on previous stage.
- Fixed Miracle Matter icon being smaller than it should be.
- Reloading saves in pause no longer works on non-randomized stages (fixes #24).
- Missile Launcher's missile no longer breaks camera during Jethead fight (fixes #61).
- Fixed a bunch of visual issues related to boss chambers (fixes #62 and satisfies #5).
- Fixed Clay Figure Medal and Alien Medal overworld NPCs reappearing when getting their Miracle Matter upgrades (fixes #63).
- Fixed Energy Crystal not providing effects of Turbocharge and Creativity Cartridge (fixes #65).
- Switching weapons in inventory now accounts for Miracle Matter weapons (fixes #68).
- Made fire turrets immune to knockback (fixes #69).
- Solid NPCs no longer push player beyond the bottom of the stage (fixes #77).

1.43.1 (Gamekutsu Rebaldamu, Part 2) (December 19th 2023):
- Tweaked/buffed Electric Shotgun:
	* reduced max ammo from 20 to 5
	* now much more spammable: pressing Shoot when anything is targeted will remove all existing shots
	* reduced effective recharge speed from 100 frames to 130 frames
- Massively nerfed/reworked Gatling Gun:
	* dear god, this was overpowered in a way I forgot it was
	* now has its own bullet instead of reducing normal Machine Gun's bullets
	* reduced damage from 3-3 to 1-2, and no longer affected by Curly's boost
	* reduced effective firerate from 4(3)/2(1)/1(1) to 5(3)
	* reduced effective amount of bullets from 1-2/2-6/3-12 to 1-3/1-5/1-7
	* reduced recharge rate from 3/2/1 to 5
	* increased spread of bullets by ~33%
	* decreased the range and knockback of bullets by ~20%
- Added sword charging/sheathing ability for Ame: 
	* hold any sword weapon for 3.5 seconds without using to charge it up
	* hitting enemies with charged sword does 4x more damage and empowers it for 7 seconds
	* empowered swords swing 2x faster and have additional projectiles
- Buffed Hellsweep's projectiles: 
	* lvl1 bomb now explodes on touching enemies, but only after 10 frames of existing, and lasts 75 frames instead of 100
	* increased piercing of lvl2 fireballs from 1-4 to 2-5
	* increased max speed of lvl3 flamings by 11%
- Buffed Little Man: 
	* increased his damage from 1/3 to 2/5
- Tweaked some of Resonant's difficulty:
	* increased item limit from 4 to 5
	* increased air gain outside of water to be 3x less than normal rather than 25x
	* now has guaranteed refill station
- Tutorial can no longer be invoked when another script is running.
- Replaced Reinforced boss music with different track.
- Added "sheepCharacter" config option to permanently turn player into a sheep.
- Added "speedrunMetadata" config option to generate some pointer data for speedruns as JSON.
- Changed Missile Launcher's description to be character-independent.
- Changed waiting on Energy Crystal's description to be <NOD.
- Fixed a typo on Water Thrusters equipping message.
- Fixed Military Armament crafting not taking away Nanites Absorber.
- Edited normal ending's choice script to be more clear.
- Fixed Iron Bond's nerf not working as it should.
- Fixed second campaign Grasstown critters deadlock (fixes #46).
- Fixed some stunlock issues with puppies trying to damage themselves out of bounds (fixes #47?).
- Fixed Super Booster applying its effect to unrelated accessories (fixes #48).
- Fixed the issues of allies and floating items persisting when their source is unequipped (fixes #50).
- Fixed second campaign wins being recognized as drowned rather than completed (fixes #52).
- Last depth of second campaign now always sends to Final Chamber even if tileset is locked (fixes #53).
- Fixed issues with Ballos not transitioning into his map boss forms (fixes #54).
- Fixed Mimiga Village's campfires rendering as chests on Improved Map System (fixes #55).
- Fixed fast ranking scroll crash if there is less than 10 entries (fixes #56).
- Fixed all reported cases of transitioning to Rankings and Title Screen not working properly (fixes #57).
- Removed Shadow Toroko's transformation entirely (fixes #58).
- "No Miracle Matter" message now stops the game action while it is running (fixes #59).

1.43 (Gamekutsu Rebaldamu) (December 2nd 2023):
New content:
- Overhauled air supply mechanic:
	* no longer is restored instantly, but at 2.5%/frame
	* air tank/diving suit no longer remove air supply, but make it deplete 3x slower (doesn't apply to lava)
	* drowning is slowed by 33% if air tank is equipped
	* hearts restore 10%-30% of air when picked up
- Added new gimmick, Defended: 
	* it spawns flamethrowers and mini-turrets
	* mini-turret is a smaller variant of turret, that shoots flames
	* mini-turret cannot be destroyed, but gets shut down after taking 50 damage
	* mini-turret will reenable itself after 25 seconds of downtime

Balance changes:
- Taking 1 damage gives less immunity frames.
- Significantly buffed wetness effect:
	* wet projectiles now inflict effect for longer and stack up to 120 frames
- Buffed Spur:
	* increased the hitting rate for spur beams from 3/2 to 2/1
	* uncharged shots now have no range limit but travel slightly slower and deal 4 damage instead of 5
- Buffed Snake:
	* now travels 2.5x faster (and covers 1.5x more range on lvl2+)
	* reduced lifetime from 20/21/26/38 to 9/12/15/19
	* changed damage from 3/5/7/10 to 4/5/5/5
	* now can pierce 2/3/4 times with 15 local immunity frames at lvl2+
	* slightly increased hitbox
- Changed Fireball:
	* now always have 2 hits instead of 3 on lvl3
	* reduced damage of lvl3 to 8(4\*2) from 9(3\*3)
	* extended its lifetime from 100 to 120 frames
- Reworked Machine Gun:
	* reduced firing and recharge rate by half
	* bullets move 2x faster and have 2x larger hitbox
	* increased damage from 2/3/4/6 to 3/5/7/9
	* increased Curly's damage from 2-3/3-4/4-5/6-7 to 3-4/5-6/7-8/9-10
	* now can pierce 2 (6 on lvl4) enemies with 10 local immunity frames
- Reworked Bubbler lvl1:
	* reduced damage from 4 to 0
	* now has 3 bullet limit
	* increased lifetime from 40 to 250 frames
	* bullets travel slightly slower
	* now acts as homing mine, flying to closest enemies and exploding into wet droplets
	* increased xp requirement from 5 to 25
- Changed Bubbler lvl2:
	* now has 1.5x faster speed and 25% less spread
	* reduced its damage from 3 to 2
	* reduced its lifetime from 60 to 50 frames 
	* reduced its attack rate from 3 to 4
- Reworked Floodtide:
	* increased hitbox and changed animation to be more accurate
	* increased damage from 5/7/10/11 to 6/9/12/15
	* increased attack speed from 32/27/24/22 to 24/19/17/14
	* lvl1 watery explosion now triggers when killing the enemy, increased amount of droplets by 2x
	* lvl2 droplets now appear only every 5 swings
	* only lvl3 whirlpool can now exist on the level, extended its lifetime by 2.5x
	* lvl3 whirlpools spawn less droplets, but can do 3 damage with 16 iframes
	* lvl3 whirpools explode into 2x more droplets
	* lvl3 sword spawns nano quiver projectiles when hitting
- Buffed Boomerang Blades in general:
	* now have local immunity frames instead of static, allowing each one to hit individually
	* now deal same amount of damage on way forward and way back
- Buffed Boomerang Blades lvl1:
	* increased lifetime from 40 to 50 frames
	* blades move 16% faster
	* decreased bullet limit from 11 to 3
- Reworked Boomerang Blades lvl2:
	* the blade travels 2x faster
	* reduced lifetime from 20 to 13 frames
	* reduced base damage from 15 to 7
	* now deals more damage if it travels longer, with up to 15 damage at max range
	* reduced amount of flames at max range by 2x
	* now shoots occasional flames on its way back
- Changed Boomerang Blades lvl3:
	* reduced its damage from 7 to 0 (still can hit enemies with no damage)
	* increased child blade damage from 4 to 5
	* blade travels 20% faster, but reduced lifetime from 33 to 30 frames
- Nerfed Boomerang Blades lvl4:
	* changed damage to 8
	* now is only limited to one blade on the screen
	* reduced amount of spawning slashes by 2x
- Reworked Electrum Shotgun:
	* increased slightly the spread and the lifetime of bullets
	* decreased max ammo by 2x
	* increased recharge duration from 60 to 100 frames
	* now deals 0 damage, but marks enemies with target signs
	* targeted enemies take 3x more damage from next hit, but deal 1.5x more damage to player
	* attacking with shotgun again with any enemies targeted will zap them to deal heavy damage
- Reworked Rocket Launcher:
	* decreased max ammo by 2x
	* now can collect xp, collecting 20 xp refills one ammo
	* missile deals damage 2x more frequently
- Nerfed/tweaked Blade:
	* blade's slashes now deal 0-1 damage instead of 1
	* decreased hitbox of slashes by 2 pixels
	* increased hitbox of main blade by 4 pixels
	* increased damage of main blade from 1/2 to 2/3
- Reworked Rifle:
	* removed ammo requirement entirely
	* now pierces infinitely, with 10 local immunity frames instead of 16 static-ish immunity frames
	* reduced damage from 10/14/22 to 4/7/10
	* reduced lifetime from 27/36/52 to 24/30/46 frames
	* increased lvl1's hitbox by 50%
	* increased lvl2/3's hitbox by 12.5%
- Buffed Mining Laser:
	* reduced its static iframes from 8 to 4
	* doubled its recharge speed
	* recharging no longer applies when xp bar is empty and only reduces movement speed by 33%
	* recharging no longer reduces teleport-dash's distance
- Reworked Energy Whip/Hook Shot:
	* renamed into Energy Flail 
	* updated sprites
	* removed ability to control with a mouse
	* reduced static iframes from 25 into 15
	* increased lifetime from 15/20/52/6 to 25/30/70/12 frames
	* changed damage from 10/13/15 to 12/15/10, but damage is halved on retrieval
	* increased both enemy and tile hitbox by ~40%
	* the bullet now moves 30%-40% slower
	* on retrieving and when holding Shoot, drops down and shoots energy bullets every second
	* when in such state, the projectile follows gravity and player's horizontal velocity
	* those changes do not appear when using Bracket's hook shot variation
- Reworked/tweaked Gatling Gun: 
	* reduced firerate and recharge rate by 1.5x, to fit with Machine Gun's changes
	* now fires 1-(level\*1.5) bullets at once, not just one
	* this has a chance to be doubled, with overclock equipped
	* now always consumes 1 ammo
	* reduced max ammo from 600 to 200
	- reduced recharge without Turbocharge from 2 to 3
- Buffed Robotic Equipment:
	* no longer uses its booster functionality while underwater
	* increased Snake's projectile probability per shot from 4% to 4.5%
	* increased Bubbler's spike projectile probability per shot from 25% to 33%
- Reworked Hellsweep:
	* damage adjusted from 5/7/10/11 to 12/19/25/32
	* lifetime increased from 26/20/18/16 to 48/38/34/28 frames
	* ported animation code edits from Floodtide
	* lvl2 spawns 2 fireballs instead of 1
	* lvl2 fireballs spawn 0.5 tiles lower
	* reduced amount of lvl3 flames from 10-25 to 5-20, but made them last 2x longer
	* empowered state now triggers at 33% max HP, not 50%
- Buffed Frontier Buster: 
	* increased use time from 9/40/3/40 to 7/35/3/35
- Buffed Water Gun:
	* can now slowly charge when player is underwater
- Rebalanced Little Man/Augmented Man:
	* significantly increased his inertion, acceleration and friction
	* he now follows player more tightly and jumps more frequently
	* fixed damage dealing not properly conforming to 3 damage per 25 frames
	* set his damage rate to 1 damage per 4 frames
	* going into interacting animation now makes him hunt nearby enemies
	* in hunting state, little man shows targets over enemies and deals 3 damage instead of 1
	* reduced Excerpt's little man count from 4 to 2
	* reduced Augmented Man's attack rate from 6 to 9
- Changed Ballos Burger:
	* reduced its bullet's damage per frame from 2 to 1
	* increased explosion lifetime from 15 to 35 frames
	* explosion now roughly follows the player
	* explosion's area is increased to better match visuals
	* reduced the radius of visuals from 48 pixels to 32 pixels
	* reduced visual smoke rate from 7 to 5
- Nerfed Iron Bond: 
	* added 1 second to initial regen cooldown after taking damage
- Reworked Cool Sunglasses: 
	* now reduce player's immunity frames by 3x instead of making player take 1.75x damage
- Buffed Nice Skates/Terra Boots: 
	* now removes low friction in Hekichu Caverns
- Buffed Cape of Shadows: 
	* increased the speed modifier of its dash by 12.5%
- Buffed Gravity Necklace: 
	* reduced its cooldown from 90 to 60 frames
- Nerfed Pair of Wings: 
	* reduced charged jump's power modifier from 0.4 to 0.75
- Tweaked Sara's dashing state: 
	* now deals 2 damage, instead of 1
	* fixed dashing hitting beetles and similar enemies across entire map
- Hunter's food now actually drops and does so more frequently.

Misc/Quality of Life:
- Re-added water splashing for Diving Suit.
- Curly now starts with Air Tank.
- Improved ProcGen's air control.
- Whimsical Star now follows player instead of mouse's cursor by default.
- Removed rest of irrelevant references to vanilla's Curly AI.
- Switched hardcoded NPC flags to use constants.
- Attempting to fix GetClosest function being weird (due to overflowing).
- NPC's immunity frames are now reset on its death.
- Attempted to fix the strange things and crashes with newer RNG behavior, which may change stages on reloading the save.

Bug fixes:
- Rewritten some TSC scripts to fix <NOD new line issues (fixes #13).
- Added some gravity force to Doctor P1's projectile phase (fixes #25).
- Fixed Dragon's left side's stray pixels (fixes #26).
- Dragon is now invulnerable before first attack (fixes #27).
- Fixed wrong weapon icon for Frontier Buster's crafting info (fixes #28).
- Cape of Shadows now properly applies slow gravity after a dash (fixes #29).
- Failing to do teleport-dash will still replenish Cape of Shadows's double jump (fixes #30).
- Added a check for weapons going past max xp in some rare cases (fixes #31).
- Shadow Toroko's blocks now self-destruct after their master dies (fixes #32).
- The level boss door now spawns at highest priority possible (probably fixes #34).
- Fixed Ame's up-aiming and interacting sprites shifting too much (fixes #35).
- Fixed saved game's cooldowns not properly restored (fixes #36).
- Bosses can no longer be hurt during star collect jingle (fixes #40).
- Made a bunch of other effects not trigger on ethereal entities (fixes #41).
- Fixed Terra Boots mislabeling Nice Skates in crafting description (fixes #42).
- Fixed Bubbler lvl1 spawning bullets on grass (fixes #44).
- Fixed Flaming Booster and Universal Catalyst conflicting with each other.

1.42.3 (Bossing Issues Away) (July 8th 2023):
- Implemented turbo text button toggle: it makes TSC parse 2x faster when active (<WAI stays at normal speed if the speed would break the script), it is B by default.
- Implemented Miracle Matter upgrade for Snake: Frontier Buster.
- Bosses inside boss arena are now protected from damage if player is not inside arena.
- Entering level boss fight regenerates its boss arena.
- Reduced player's Nemesis's damage from 4/4 to 3/3, but it no longer consumes ammo during boss fights.
- Nerfed Iron Bond: now slows down with more time the player doesn't take damage, until capping out; this slowdown is reset with taking damage.
- Fixed incorrect bounds for crow Reinforced's teleportation (issue #2).
- Sheeps no longer jump in the air (issue #5).
- Fixed main tiling on random size maps setting (issue #8).
- Fixed unnecessary <NOD before equipping High-Octane Tank (issue #16).
- Procgen depth values no longer show during credits (issue #15).
- Pause menu cannot be opened during credits anymore (issue #14).
- Removed Y/N confirm sound on selecting characters from gameplay options (issue #11).
- Chaos Key and Discordian Visor can no longer be used during level boss fights.
- Refactored damage dealing into new function.
- Dashes, Little Man, Discordian Visor, Super-Booster, Lightnings and Puppies now use new damage function instead of spawning special bullets.
- Tweaked Little Man's damage: now does 3 damage every 25 frames, instead of 1 damage every 10 frames and no longer damages sheeps and laser gaudi's lasers.
- Made the rest of weapon data be handled by weapon item mechanic (issues #1 and #10).
- If beta toggle inside code is enabled, the update part of title screen goes to issue tracker and the window title says "(beta)".
- Fixed Fragile gimmick's falling blocks not damaging players from the left (issue #17).
- Disallowed any kind of breaking blocks when boss fight is on, not just Nemesis's.
- Added additional checks for water spreading to fix incorrect visuals (issue #19).
- Refactored timestop effect to have its effects be regulated by its own tag (issue #6).
- Allies, Whimsical Star and platforms are no longer affected by timestop (issue #6).
- Reduced max amount of NPCs from 8192 to 4096.
- Removed leftover Mimiga Mask calls for sprite change (issue #12).
- Corrected the placement of allies and player in ending cutscene (issue #12).
- Allies now attempt to follow Quote NPCs when player is hidden (issue #12).
- Certain projectiles no longer target ethereal entities.
- Miracle sound cue no longer plays without miracle matter, to prevent some confusion.
- Item pickups now render above platforms (issue #21).
- Quick game start on title screen no longer asks player about save reset if there is no save.
- Stars can no longer be picked up and items can no longer be picked if level boss is alive (issue #22).
- Slightly nerfed scaling of spike damage from 16.6% of max life to 11.1% of max life.

1.42.2 (June 12th 2023):
- Fixed the crash caused by new PRNG algorithm crashing on NPC generation entries with same minimum NPCs and maximum NPCs.
- Fixed the issue with crafting menu removing every available crafting recipe after crafting one item.
- Actually made the fix for issue with save points blocking the rest of the level from generating work; also reduced the amount of tries for save points to generate to 250.
- Max life increasing no longer has any effect on Duckman.
- Heavy Weight falling overrides Pair of Wings gliding, if both are equipped.
- Fixed the crash on trying to enter the last tileset of the game.

1.42.1 (June 9th 2023):
- Rewritten GetCSE2FormattedMessage to use C++20's formatting instead of fmt library, eliminating the crash involving it trying to use libraries from Bobylev's dev environment paths.
- Compilation is switched to use newer standard.
- Fixed Ability-key enabled options being executed even after releasing Ability key.
- Fixed Curly's Underwear being in starting inventory (leftover from debugging).
- Fixed crashes connected to SetDestroyNpChar explosions having zero width in some cases.

1.42 (It is in the Name, Huh) (June 9th 2023):
- Added Nanites Absorber as chest loot.
- Added Military Armament as craftable item.
- Reworked/nerfed Gravity Necklace: now can be used regardless of being on the ground, but has 1.5 seconds cooldown to compensate.
- Switched the generation order of multitile structures, like clouds, and pickups, like chests.
- Weapons can be now turned into weapon items with pressing Ability key while having them selected.
- Weapon switching is now also accessible if you press Dash key on weapon items.
- Replaced the movement section of tutorial signs with new inventory management section.
- Implemented daily seeds functionality: they set the same seed and playable character each day as long as players have same gameplay options that can affect worldgen, such no Polar Star or no Nemesis. Daily seeds can be replayed as long as player doesn't record their final score for it via dying or winning.
- Removed Improved Map System as chest loot. It is still available for older saves, but will not work properly.
- Added Map System Type as Configuration option, which defines, what kind of map should be used on pressing Map: Normal or Improved.
- Refactored seeded randomization code to be more understandable and clear.
- Fixed platforms and terminals rendering one tile below their position on Improved Map System.
- Redrawn many tiles on Improved Map System to be have less brightness and constrast.
- Redrawn Improved Map System's life capsules and refills to be more noticeable.
- Fixed Gatling Gun's terminal description not showing the correct weapon icon.
- Fixed Gatling Gun not having weapon icon in Legacy Interface mode.
- Fullscreen mode no longer hides mouse cursor.
- Fixed the issue with cooldowns not being drawn at correct position in all resolutions.
- Increased the internal limit of damage/exp value view visuals from 16 to 256.
- Fixed some critical issues, caused by game still assuming that Miracle Matter list is C-style array.
- Switched both normal and levelgen randomization from multiply-with-carry to PCG (more info on https://www.pcg-random.org/)
- Pre-1.42 saves will still use multiply-with-carry when loaded, with minor differences like doors moving into different place and tutorial signs being replaced with warning signs, telling player about PRNG algorithm change.
- Power-up solid pedestals no longer will override enemies and other power-ups when being drawn.
- Hunter's food no longer drops from NPCs that drop zero EXP.
- Rewritten the text on refill station if player has empty Life Pot.
- Tweaked Aqualite Caves's background to tile better. (Tert)
- As a test, semi-solid platforms now render at lowest priority.
- Crafting terminals no longer generate on depth 1 with "stage_debug" option on.
- Fixed Iron Bond not having "'s" in its equipping description.
- Fixed map modifications not actually regenerating.
- Fixed ProcGen's weapon not having proper icon in Legacy Interface mode.
- Fixed the Duckman's ducks having the initial speed of 0 in certain cases.
- Refactored title screen menu to allow for Ability-key activated conditions.
- Significantly optimized title screen code by making it not calculate winrate every frame.
- Moved version info to bottom of title screen.
- Added version name to title screen and debug info block in pause screen.
- Merged "Select Character" and "Start New Game" into just "Start New Game", that selects a character. The quick game start can be activated while holding Ability key.
- Quick game start now has "Yes" selected by default.
- Renamed "Configuration" into "Gameplay Options"
- Merged "Configuration" and "Options" on title screen into one option selection, which is "General Options" normally and "Gameplay Options" with Ability key.

1.41 (Measuring Cooldowns) (March 29th 2023):
- Refactored the list of crafts to use dynamic array instead of C-style array, which also fixes the issue with entire miracle matter list disappearing after one craft.
- Fixed the TSC error with crafting some of new items.
- Attempting to fix the issue with save points blocking the generation of the rest of the level.
- Implemented first two Miracle Weapon Upgrades: Hellsweep and Gatling Gun.
- Refactored timer-based items to use special cooldown system, which also makes timers show up as icons and be saved.
- Removed the cooldown for dashing at the start of the game.
- Reduced Snake's damage from 4/7/9 to 3/5/7.
- Reduced Snake's duration from 22/25/32 to 20/21/26.
- Buffed Spur's static immunity frames from 4 to 3(head)/2(tail).
- Reduced Curly's Machine Gun damage from 3/4-5/5-7/6-8 to 2-3/3-4/4-5/6-7.
- Duckman now loses the same amount of experience as every other character.
- Added "stage_debug" config option for debugging level generation stages.
- Nerfed Duckman's duck firing cooldown from 4-13 frames to 5-25 frames.
- Falling blocks spawn 3 blocks higher, 3 blocks wider and 50 frames less frequently.
- Actually fixed puppies dealing damage to other enemies when sleeping.
- Changed drowning timer to 30 frames on Resonant difficulty.

1.40 (Retrospective and Perspective) (March 4th 2023):
New content:
- Added its own music for Shadowbuilt Crags, instead of reusing Deepest Cave one.
- Replaced Doctor phase 2's BGM with "silent howling".
- Fighting with level bosses in Reinforced+ difficulty now plays new BGM "Enemy Hall".
- Added 6 new universal BGMs.
- Implemented "pitch_randomizer" configuration option to enable an option to tweak the pitch of any Organya song to make it sound less boring.
- Implemented "randomSizeMode" configuration option to allow procedural maps to use other sizes than 118x118.
- Implemented "superHotMode" configuration option to enable a game mode where holding any keys stops the flow of time.
- Implemented "oneDamage" configuration option to make every player's bullet deal 1 damage.
- Added Flaming Booster, Shield of Time, Little Man's Underwear, Nuclear Stomper, Omniquick Sneakers and Freeze Star as craftable items.
- Implemented the option to start from second campaign levels with 'config.json' 'secondCampaign' option.
- Reworked drowning: you don't just die instantly, but lose 1 hp every 60 frames once your air supply hits 0.

Balance changes:
- Buffed Water Gun from having 6 ammo to 25 ammo.
- Reworked Bubbler lv3/lv4: now only deals 1 damage to destroy projectiles, inflicts infinity immunity frames on enemies to only hit once and no longer pops after set amount of time.
- Bubbler thorns now only deal 3 damage instead of 4 and last 3 frames lower.
- Nerfed Bubbler lv2's damage from 4 to 3.
- Every level of Boomerang Blade now does 20 static immunity frames.
- Every level of Boomerang Blade now does much more damage on returning.
- Every level of Energy Whip now does 25 static immunity frames.
- Increased Energy Whip's damage from 4/6/9 to 10/13/15.
- Reduced Electrum Shotgun's recharge rate from 100 ticks to 60 ticks.
- Reduced Electrum Shotgun's ammo from 30/60 to 20/40.
- Reduced Electrum Shotgun's damage to 2.
- Increased Electrum Shotgun's amount of bullets from 2-4/3-6/4-8/12-20 to 6-14/8-20/13-26/20-42.
- Increased Electrum Shotgun's spread from 12/16/20/14 to 15/20/26/18.
- Electrum Shotgun's bullets no longer pierce and use static immunity frames.
- Increased Electrum Shotgun's range by 50%.
- Nerfed Machine Gun's damage from 3/4/5/9 to 2/3/4/6.
- Nerfed Rifle's damage from 11/18/30 to 10/14/22.
- Floodtide lv3's whirlpool travels 33% faster and releases 2x more droplet projectiles.
- Reworked Floodtide's immunity frames to use new static iframes system.
- Nerfed Curly's Machine Gun's damage from 4-5/6-7/8-9/10-12 to 3/4-5/5-7/6-8.
- Nerfed Spur: its damage is reduced to vanilla values and each bullet type hits only every 4 frames, putting emphasis on its piercing capabilities, but increased the duration of time Spur's beams last on the screen.
- Significantly buffed Mining Laser as a weapon: now does 9 damage instead of 2 but only pierces every 8 frames.
- Reworked Mining Laser's charging: now restores 1 ammo every 5 frames instead of 35, but reduced max ammo from 150 to 70 and recharging consumes experience; Mining Laser can store up to 100 experience.
- Mining Laser's mode 5 (death beam) was tweaked to consume less energy due to static iframes on it.
- Whimsical Star now uses static iframes and pierces every 8 frames.

Visual changes:
- Implemented decorative sword slash from Temperature Beyond Zero Cold mod.
- Improved the visuals for Fireball and Snake trails.
- Improved the visuals of small star, chaos and boosting particles.
- Improved the visuals for Boomerang Blade lv2 and Magician flames.
- Improved the visuals for first half of on-character item sprites. (AsperDev)

Quality of Life features:
- Rebound all dashing-related items to their own Dash keybind, which is Left Shift.
- Ability key's default value is now C.
- Using refill stations now restores air supply.
- Added F3 quick music reset hotkey.
- Getting an item now plays a sound effect if said item can be combined with other item using Miracle Matter.
- If you select any item in inventory and you can combine it with other item with crafting, there will be the Miracle Matter icon shown under both of them, like equip icon.
- Added ability to call normal Map System while having Improved Map System with holding Ability key while using map.
- Tweaked Ame's dash to use Dash button instead of double tapping the direction.
- Added the clause about permadeath in save point's TSC.
- Crafting menu organizes item recipes in rows of 6 if there is more than 6 items.
- Added a note about Turbocharge affecting Spur.
- Made boosting take priority over swimming.
- Improved the Improved Map System's visuals to have less clutter and be more contrasted in terms of color.
- Rephrased Cool Sunglasses TSC to be more clear.
- Whimsical Star can be disconnected and reconnected from mouse cursor by clicking; in this disconnected state it moves 1.5x faster and randomly orbits player's position.
- Used refill stations no longer show up on both Map Systems.

Difficulty changes:
- Mesa boss jumps higher, attacks faster and his bricks travel faster but spawn butes only with 50% chance.
- Reduced amount of red balls in Doctor phase 1's circle attack, they also travel slower.
- Red Crystal's fish missiles travel faster and give 1 less XP.
- Removed the mechanic of cutting off certain enemy spawns with increasing depth.
- Reduced mob health boost in each difficulty.
- Increased the max HP of level bosses by 25.
- Nerfed air reduction from taking damage in Resonant.
- Second campaign's dragons now give more time to damage them.
- Second campaign's armored gaudis spawn 1.5x less frequently and have 1.5x less HP.
- Significantly reduced the chance of pot ambush and XP reduction in Resonant.
- Removed additional life capsules from Resonant difficulty.
- Added mentions about experience and water features of Resonant difficulty.
- Removed damage increase part of Resonant difficulty.
- Significantly reduced vertical acceleration and max speed of Reinforced's power critters.
- Reworked Reinforced's crows: no longer have increased acceleration, instead they have a chance to teleport with resetting acceleration and healing 1 HP, when damaged.
- Reworked Reinforced's skullheads: no longer shoot energy blasts into player, instead they blast inaccurate barrage of arrows when opening their mouth, the attack detection range was also increased.
- Skullheads now jump slightly higher.
- Reinforced's arrow projectiles disappear significantly faster.
- Reduced max speed and bounce speed of crows by 25%.
- Tweaked Reinforced's critters: their bullets travel slower, but they shoot double amount of them per attack but with higher inaccuracy.
- Reworked Reinforced's Igor: removed shooting time boost, but tweaked projectile speed from 3-7x to 2x-5x and added a chance to shoot Doctor's red bouncing projectiles instead of regular energy blasts; also increased his movement speed by 50%.
- Reduced Igor's energy blast base speed from 6x to 5x.
- Tweaked Reinforced's Colonball: removed additional energy blasts and adjusted boost to attack rate from 2-3 to 3-6, but made small colons shoot turret's missiles instead of bouncy balls.
- Tweaked Reinforced's Red Demon: now only throws one projectile, but it bounces of walls one additional time and has more initial speed.
- Reworked Reinforced's Possessed Sue: replaced energy blasts from phase 1 with lightning attacks, added red bats summons in phase 2, removed attack number increase from additional difficulty.
- Removed the "homing" movement from falling Reinforced's power critter, but increased his detect range and jumping speed.
- Increased the size of Reinforced's boss arenas from 22x7 to 24x8.
- Nerfed Reinforced's dragon buyobuyos: reduced amount of fireballs from 3-6 to 1-2, reduced inaccuracy from 60 degrees to 40 degrees.

Bugfixes:
- Stars can no longer spawn on places with any NPCs on them, which should remove the issue with stars being blocked by platforms on the map.
- Removing Improved Map System from inventory now properly makes it not appear on pressing Map key.
- Tweaked the behavior of Water Gun to not cause any acid poisoning while being inside it on levels with acidic water.
- Tweaked generation of exit door in a way to make next level be 100% consistent.
- Whimsical Star and Red Crystal in Ballos's fight no longer target NPCs listed in "etherealEntities" tag.
- Removing items from inventory now also properly removes crafting recipes, associated with them.
- Fixed second campaign's red plants having the wrong sprites.
- Fixed the bug which caused left-side Speed Shoes effect apply to player without them.
- Fixed the bug which caused right-side Speed Shoes effect to not appear at all.
- Fixed the issue of Floodtide ignoring knockback immunity tag data.
- Fixed the typo in Bubbler's chest's TSC.
- Fixed "20 Stars instead of 10" configuration's exit door not getting opened when used for its purpose.

Resource packs and tags functionality:
- Added 'generatedStageID' optional field to resource pack's 'info.json' to tell the game what stage should be generated as random level.
- Added 'resolution' optional field to resource pack's 'info.json' to tell the game at what resolution sprites should be loaded.
- Added 'windowTitle' optional field to resource pack's 'info.json' to tell the game the window title.
- Fixed OGG music loading.
- Pregenerated stages can use chests without events on them to generate event numbers used by random levels.
- Externalized the spawning of most enemies into its own tag. Some aspects could be changed due to it not 100%-accurate representation of old mechanics.
- Externalized level generation code into its own tags: "generationOrder" and "generationMetadata".
- Implemented support for JSON overrides. If necessary json files in resource pack contain "override" field, then values from it will be applied to base json, otherwise base json will be fully replaced, unless the resource pack is Default.
- Added 'gimmick' variable to lock gimmick.
- Attempted to fix the issue with English text clipping due to fix code for Japanese text.

Refactoring/misc:
- JSON code will attempt to output errors into game log.
- Changed the size change of Carved and Spacious gimmicks from 2x to 1.5x.
- Reduced max amount of NPCs from 16384 to 8192.
- Added bullet's hitboxes into "hitboxes" config option's visuals.
- Added release dates to each update in readme.txt.
- Added line breaks between major/important updates in readme.txt.
- Removed the compatibility with version 1 save files.
- Refactored the way player's map modifications work to use less memory and be compatible with variable-sized maps.
- Switched save file's version to 3.


1.37.3 (Patching Rough Edges) (May 31th 2022):
- Fixed the teleportation gimmick of Spectral Frog and Dark Misery.
- Fixed the bug with character screen selection overriding config.json.
- Pots no longer render on Improved Map System.
- Fixed the bug with Badge of Love spawning second Nemesis.
- Fixed the inability to place blocks at left edge of the map.
- Fixed the bizzare particle at top of the map.
- Fixed the ability to "break" blocks on pregenerated stages.
- Fixed Aqualite Reefs's track not looping.
- Fixed rendering of weapon items in rankings.
- Fixed the wrong string for in-game Configuration's character select.
- Fixed seed's number display breaking beyond 32-bit integer limit.
- Certain enemies are now immune to knockback, added corresponding tag.
- Added tag for entities which can be shot, but recieve no damage and do not deplete life of bullets that go through them. (current such entities are grass and fire)
- Added tag for entities that are affected by Fishy Tube.
- Fixed the error in English script which stated than Winter Clock's cooldown is 50 seconds.
- Freezing wind now has limit for how long it will freeze the character.
- Updated Japanese resource pack to v2.
- Nerfed Duckman duck's DPS (ducks per second), but increased the damage from 1 to 2.
- Ducks no longer leave annoying effect of colliding with ground.
- Heavily nerfed knockback from Fireball.
- Increased the base damage of Heavy Weight from 6 to 10.
- Fixed the bug with Shield of Resilience crafting.

1.37.2 (Journey to the East) (May 21th 2022):
- Updated Serenity's sprites. (Cybrill)
- Power Critter no longer accelerates into infinity and outside of Reinforced.
- Fixed the door that appears after level boss's defeat.
- Fixed wrong string used on difficulty select.
- Added 'isJapanese' optional field to resource pack's 'info.json' to tell the game to load in Japanese mode.
- Executable now supports both English and Japanese.
- Fixed the crash when opening options from pause menu.
- Added ability to partially reload resource packs in game via pressing OK.
- No gimmick name is no longer hardcoded at "None".

1.37.1 (Ocean of Possibilities) (May 13th 2022):
- Updated item sprites. (AsperDev)
- Updated Excerpt, Quate and ProcGen sprites. (AsperDev)
- Updated Andesite Dungeon's tileset. (AsperDev)
- Updated ProcGen's weapon projectiles. (Taiki)
- Updated difficulty indicators. (Taiki)
- Redesigned Map System to be less of eyesore and be more friendly to colorblind users.
- Fixed bug with doors not requiring any stars to complete.
- Sheeps are no longer solid.
- Reworked spikes: now deal 5 + 17% of max HP damage instead of 10.
- Alien Medal removes spike damage scaling instead of reducing it by half.
- Fixed the existence of glitched difficulty beyond Resonant.
- Reworked Water Gun: instead of flooding the level, spawns water bubble which flags entities as being underwater.
- Tweaked Nano Beacon to cause less freezes.
- Implemented resource overrides/resource packs.
- Reworked/nerfed Monster X: removed spinning blades, fireballs are a lot slower, in phase 2 uses more fire.
- Fixed Monster X not healing when moving in certain direction.
- Fixed Balfrog attack that was incorrectly placed in Basic difficulty.
- Corrected all death source icons in future runs.
- Nerfed Reinforced Balrog boss: homing shots are replaced by laser shots.
- All in-game messages that are not in TSC are fully externalized into messages.json.
- TNT Deleet no longer gains hp in higher difficulties.
- Equip limit feature actually works now.
- Reduced Ballos's movement speed to vanilla values.
- Reduced amount of red orbs from Ballos P1.
- Reworked Reinforced Ballos P2.
- Ballos P3 eyes spin faster and do not disappear on P4.
- Increased Ballos's total HP.
- More entities are affected by water.
- Fire is extinguished when it touches water.
- Bubbler and Floodkite make enemies wet with their attacks.
- Nerfed Iron Bond's regen delay by 50%.
- Nerfed Frostbite Stopwatch's freezing efficiency.
- Nerfed Booster's fuel from 18/50/110 frames to 15/40/90 frames.
- Buffed Terra Boots's upwards acceleration by 50%.
- Clay Figure Medal's effect no longer gives additional i-frames.
- Explosion effects last for little bit less time.
- Nerfed Spur's uncharged shot damage from 7 to 5, but increased the range.
- Slightly increased the range for all vanilla weapons.
- Moved Press's chasing AI to Resonant.
- Resonant difficulty makes gimmicks spawn from depth 1.
- Added score values for second campaign mobs.
- Rankings now use special flag for determining win image.
- Final Chamber is properly recognized as floor 12/20.

1.37 (Difficulty Spike) (May 3rd 2022):
- Updated item sprites. (AsperDev, Taiki)
- Updated rocket launcher projectile sprite. (AsperDev)
- Updated stars sprite. (AsperDev)
- Updated several tilesets. (AsperDev)
- Fixed Augmented Man and Badge of Love allies not appearing at Doctor's stage.
- Fixed Augmented Man not persisting between levels.
- Fixed Augmented Man not being removed when unequipped.
- Updated crafting menu sprites to fit new item sprites.
- Fixed the bug that made tileset config option non-functional.
- Optimized size of Profile.json and rankings.json.
- Reworked refill stations: they only work once per use and will stay disabled.
- Reworked life pots: now only restore 35 HP but can be refilled at refill stations, discharging them in process.
- Refactored level generation code.
- Added a second campaign which can be accessed after beating Doctor.
- Added credits sequence.
- Reworked Copper Bulwark and Sacred Aegis: reduced cooldown and distance, only damage specific enemies and deal 30 damage instead of 25.
- Significantly buffed Sacred Aegis's explosion damage.
- Redesigned Lethrys Quote into Modern Quote. (AsperDev)
- Platforms are less "solid" and let you go even after smaller Down presses.
- Changed sprites of eye-based enemies.
- Removed the majority of lag on dying.
- Added game.log which records some data during gameplay, mostly for benchmark purposes.
- Bunch of previously hardcoded data is now located in Tags folder.
- Fixed the crash if you save then die (with save removed) and try to reload.
- Fixed the profile loading error where you always load with full health.
- Grassy Bushtown and Egg Factory are more open.
- Nerfed Crow's movement speed.
- Added Ikachan-themed Aqualite Reefs tileset.
- Mannans and Buyo Buyos explode like other enemies.
- Added numbers to boss health bar.
- Increased boss arena sizes.
- Tweaked title screen logo, fixed the bug with it being off-center.
- Increased difficulty of Doctor's phase 2.
- Nerfed Machine Gun's damage from 3/5/7 to 3/4/5.
- Buffed Bubbler's damage from 4/3/3-3 to 4/4/3-4.
- Grass entities have same distance limitation as enemies.
- Tileset selection is a bit more randomized.
- Random seed selection uses RNG seeded with system time instead of just system time.
- Changed how Energy Whip's chain is rendered.
- Buffed Marshy Grotto's critters.
- Added support for high-definition audio music. Just put OGG/MP3 file with same name as ORG file in music folder.
- Mining Laser projectiles no longer deal knockback.
- Massively nerfed Energy Whip lv3 by fixing the bug, adjusted XP requirements to compensate.
- Fixed Timeline Ring's effect dealing extreme damage, but reduced cooldown from 50 to 35 seconds.
- Increased damage modifier of Cool Sunglasses from 50% to 75%. 
- Doubled the ammo of Rocket Launcher.
- Removed ability to place or destroy blocks on pregenerated stages.
- Fixed unnecessary d for Debug Info's high score.
- Added highscore and runs counter on title-screen. Only counts wins done on 1.37 unfortunately.

1.36.2 (April 7th 2022):
- Updated tutorial sprites. (AsperDev)
- Updated Endless Frontdoors. (AsperDev)
- Adjusted Floodkite: nerfed damage and swing speed, but the weapon can be used holding Attack.
- Fixed Ability key on gamepad.
- Fixed the ability to trash the last item in inventory.
- The game now asks to unequip the equippable item before trashing it.
- Ame starts with Floodkite instead of Fireball.
- Added config regeneration for some specific cases.
- Added 3 new characters: ProcGen, Serenity and Sue.
- Resetting with inventory open properly reset the game instead of going into main menu.
- Steel Plating no longer resets regen timer to be higher.
- Clay Figure Medal significantly delays regen and grants less iframes from Steel Plating on activation.
- Iron Bond's regen rate is no longer influenced by other items.
- Attempted to fix the issue with chests not loaded on reloading the save.
- Redesigned the logo and icon. (AsperDev)
- Updated some item icons. (AsperDev, Taiki)
- Fixed Miracle Matter sometimes "generating" on Doctor's level.
1.36.1 (March 31th 2022):
- Fixed platform's wonky hitbox.
- Rockets only appear in technological locations.
- Updated some item sprites (Taiki).
- Fixed sheepification not paralysing you.
- Fixed the bug with 12 weapon limit being actually 11.
- Weapons that don't fit into 12 weapons limit now go into inventory. They can be swapped with any other weapon in your inventory.
- Expanded item limit by 3 items.
- You can trash unneeded items with Ability key.
- Added April 1st features.
- If your inventory is full, chests will display the info how to free space instead of giving the item.
- Added Legacy Graphics config option which brings sprites from older releases.
1.36 (March 29th 2022):
- Reworked saves. They are now in JSON and split into two files.
- Old saves are deprecated and cannot be loaded.
- Added one-way platforms. You can jump on them but you need to hold Down to descend from them.
- Added rockets which allow to move upwards quickly.
- Fixed the bug with terminals being invisible on Map System.
- Fixed the bug with Miracle Matter generation.
- Added "debug" config option.
- Added Ame as playable character.
- Reduced the cost of Mining Laser modes and increased self-charge speed and damage.
- Gaudi's laser projectiles can be "deflected" by attacking them. Deflected lasers deal less damage and move slower.
- Loosened the requirement to die from falling on sky stages from edges of map to 4 blocks beyond edges of map.

1.35.4 (March 16th 2022):
Changes item RNG.
- Improved Moonlight Frame visuals. (Taiku)
- Fixed Underground Pasture not having night variant.
- Fixed Winter Clock not being craftable.
- Fixed other minor TSC bugs.
- Fixed rare drowning bug.
- Fixed rankings being not saved.
- Added Mining Laser as chest loot.
1.35.3 (March 14th 2022):
- Fixed Rifle not working properly.
- Fixed High-Octane Tank's description saying "Overclock".
- Increased Spur's charge time from 0.66/1/3.33 s to 1/1.5/4.33 seconds.
- Increased XP requirements for Floodtide from 30/50/15 to 40/60/20.
- Increased XP requirements for Boomerang Blades from 20/35/5 to 30/60/10.
- Increased XP requirements for Snake from 18/24/12 to 25/55/20.
- Increased XP requirements for Machine Gun from 20/20/20 to 30/30/30.
- Increased XP requirements for Energy Whip from 30/50/5 to 30/90/0.
- Increased XP requirements for Electrum Shotgun from 30/40/15 to 40/50/20.
- Reduced size of Floodtide LV3's projectile to prevent clipping.
- Reduced amount of droplets Floodtide LV3's projectile spawns.
- Massively reduced the knockback from Boomerang Blades.
- Reduced damage from Blade's slices from 1-2 to 1.
- Reduced Floodtide's knockback.
- Rankings file is saved in more occasions.
- Iron Bond cannot heal in cutscene mode.
- With disabled Polar Star, bosses no longer build arena around themselves.
- Removed confirmation for exiting from title screen.
- Fixed golden highlights on Doctor stage's minimap.
- Selecting a character on title screen actually does something.
- Character on title screen matches one in the config.
- Fixed the second fadeout when selecting character on title screen.
- RNG for gameplay uses the same algorithm as generation RNG.
1.35.2 (March 13th 2022):
- Removed the second zlib dll.
- Reworked charged jumps: increased max height, but they can only be done from ground and require 25 ticks charge up.
- Pair of Wings affects charged jumps.
- Water Thursters allow for charged jumps while swimming.
- Added Winter Clock as craftable item.
- Chilling effect is spread across entire sprite instead of concentrating in upper half.
- Stylized more aspects of TextBox.png. (AsperDev)
- New Loading.png. (AsperDev)
- Edited some weapon descriptions to make more sense.
- Added a proper title screen.
- Refusing to retry after dying or winning the run brings Rankings screen.
- Quit on Pause menu is now Quit to Title Screen.
- Entering and exiting rankings now has fade-out effect.
1.35.1 (March 12th 2022):
- Fixed invisible Floodkite icon in chest's event.
- Improved Electric Shotgun's and Floodkite's sprites. (AsperDev)
- Droplets hit enemies sooner.
- Increased Floodkite's attack speed.
- Fixed Amphibian Medal's weapon range extension.
- Weapon range extension no longer affect Floodkite.
- Level 2 Floodkite shoots more droplets.
- Level 3 Floodkite projectile spawns way more droplets but projectile limit is reduced from 6 to 3.
- Dynamite triggering other dynamite now has 1 second delay.
- Reduced press's detection height from 12 to 8.5 blocks.
- Updated sprites for sheep and early enemies. (Taiki).
- Marsh critters spawn copies only every 3 jumps, the aggressiveness of them is also reduced.
- Reduced HP of Possessed Sue boss to 275.
- Map System can be opened during script events.
- Fixed life capsules reappearing after reloading.
- Added charged jump: hold Up while jumping to gain extra height, this can be disabled with config.
- Added advanced arms selection: you can use numbers on your keyboard to select weapons, this can be disabled with config.
- Fixed the case when you pick up a star during loading the stage and losing it when you gain control. 
- Removed most of unused vanilla music from distribution.
1.35 (March 9th 2022):
- Added 9 new tilesets which are spread across the game!
- Added Electrum Shotgun and Floodtide as chest loot.
- Replaced Mod Webpage button with Check for Updates button on pause menu.
- Fixed the undefined character selection number.
- Rankings are saved even when window is closed via external means.
- Standardized level names.
- Treasure gimmick cannot appear on first floors.
- Invidia has unique icon.
- Added death icon for environment critters.
- Slightly improved the sprites on existing tilesets.
- Gimmicks do not appear on floor 1.
- Big jellies do not deal contact damage and are solid while undisturbed.
- Nano Beacon no longer triggers from grass.
- Nerfed Fireball's XP requirements from 10/20/20 to 16/30/20.
- Reworked how tilesets are selected. More dangerous and technological locations are pushed towards the end of list, while easiest disappear from rotation.
- Fast runs experience same algorithm as if their depth was worth 2 regular depths.

1.34.1 (March 3rd 2022):
- Fixed Level 4 weapons.
- Fixed terminals falling out of level.
- Changed score granted from Archers, Skullheads, Crows, Mannans, Turrets and Laser Gaudis.
- Added Mahin as playable character.
1.34 (March 2nd 2022):
This update is not compatible with seeds for 1.33.
- Fixed Duckman being able to find life capsules.
- Fixed the crash if ducks had nothing in their range of attack at start of the level.
- Slightly buffed duck's firerate.
- Refactored Map System's object tracking, now should show things even if they are obstructed by other objects.
- Fixed completely botched character select screen, when resolution is not 852x480.
- Fixed Iron Bond not working on certain stages and buffed life regen.
- Fixed Duckman's description TSC.
- Tutorial signs cannot spawn on top of other objects.
- Reduced mob modifier on Treasure gimmick from 2x to 1.5x, now always spawns 4 life capsules.
- Double jumping is recharged when standing on any solid surface.
- Igor, Red Demon, Mesa, Critters, Beetles, Archers, Crows, Buyo Wisps, Mannans, Wisp Bases, Laser Gaudis and Doctor do not deal contact damage when standing still.
- Mesa jumps a little while attacking.
- Increased HP of level bosses to 375.
- Archers give 7 XP.
- Mannans give 15 XP.
- Endgame enemies again deal 6 damage with their projectiles.
- Doctor attacks deal more damage.
- Skullheads and Crows deal 5 damage, have 2x more HP and move significantly faster.
- Jellies deal 3 damage, big jellies deal 7 damage.
- Experience is carried to next level on levelling up.
- Carved levels are less obnoxious.
- Reduced amount of refills from 2-3 to 2.
- Player no longer stands in air if he fells out of Outer Wall.
- Tileset gimmicks are no longer carried to Doctor's stage.
- Added ability to mute music.
- Removed the limitation for size of rankings.
- Holding Ability key allows to scroll for 10 entries at once at rankings screen.
- Added instructions for controls in rankings.
- Added the name for Magician.

1.33.1 (February 25th 2022):
- Fixed Nano Quiver.
- Fixed Improved Map System being available on start.
- Nerfed Creation Energizer and Universal Catalyst: free placing costs 3 ammo and free breaking costs 2 ammo.
- Iron Bond no longer resets its timer from any i-frames, only from taking damage; increased timer to compensate.
- Blocks cannot be placed during boss battles.
- Improved Bracket's sprites. (Jamark)
- Edited some backgrouns, Rifle and Water Gun. (AsperDev)
- Added 4 new characters: Puppy, Magician, Duckman and Quate.
- Fixed Whimsical Star disappearing after hitting certain enemies.
1.33 (February 23rd 2022):
This update is not compatible with seeds for 1.32.
- Grass critters no longer push you, reduced their HP to 10.
- Fragile and Doggy gimmicks are not rolled for first few floors.
- Reduced mob modifier for Treasure gimmick from 3x to 2x.
- Gimmicks do not carry over to depth 10.
- Water now always generates in bottom half of stage.
- Increased Improved Map System's size by roughly 4x.
- Removed mouse scrolling from IMS.
- Added tileset and gimmick showcase at start of the level.
- Renamed certain stages.
- config.ini is replaced with config.json.
- Added Configuration screen to pause menu.
- Added character select screen, it's accessible within Configuration screen.
- seed2.txt is deprecated.

1.32.1 (February 18th 2022):
- Fixed the generation of terminals.
- Certain objects render above all other NPCs.
- Collectables are no longer obstructed by other NPCs on minimap and overworld.
- Signposts have gravity.
- Renamed "Extended Pause Info" into "Debug Info".
- Window title shows extra info when Debug Info is enabled.
- Nerfed Copper Bulwark i-frames from 90 to 40.
- Cool Sunglasses no longer give extra i-frames for dash.
- Reduced grace periods on dashes from 15 frames to 10.
- Increased dash cooldowns by 10%.
- Increased cooldown for sunglasses dashes by 100%.
1.32 (February 17th 2022):
First release for itch.io.
- Changed King's Table music and visuals.
- Changed level boss battle theme.
- Changed the jingle for defeating Doctor.
- Added a track for Tutorial option in menu.
- Added a jingle for collecting 10 stars.
- Fixed music fadeout continuing to next stage in some cases.
- Fixed computer spawning on depth 1.
- Tried to fix lack of allies on doctor's stage.
- Stars, depth and score are shown on minimap.
- Changed the name and icon of executable.

1.31.1 (February 17th 2022):
- Tried to fix miracle matter again.
- Added death icon for puppies.
- Fixed optimization flags.
- Added full clear config option.
1.31 (February 16th 2022):
- Changed HUD to be more techy and stylized (Tert, Taiki).
- Added level gimmicks that change how stage is played.
- Added tutorial signs. The full tutorial is available via Pause Menu.

1.30.3 (February 15th 2022):
- Fixed the generating miracle matter beyond first one.
- Updated rankings to track fast run mode and death causes.
- Sitting in map and inventory increments game played counter.
- Fixed wacky display of Miracle Matter objects on IMS.
- Fixed Whimsical Star unequipping.
- Fixed PressClip.
- Changed Energy Whip's damage from 3/5/13 to 4/6/9.
- Changed Rifle's damage from 10/16/27 to 11/18/30.
- Buffed Spur's damage from 4/4-3/8-6/12-11 to 7/5-4/9-7/13-12.
- Buffed Snake's damage from 4/6/8 to 4/7/9.
- Buffed Machine Gun's recharge rate from 4 frames to 3 frames.
- Bubbler is affected by Turbocharge.
1.30.2 (February 12th 2022):
- Rankings support drowning-caused deaths.
- Inventory seed display shows 10 numbers instead of 9.
- Fixed sprite of Creation Catalyst.
- Fixed invalid sprite size for won runs in rankings.
1.30.1.1 (February 11th 2022):
- Fixed Destruction Energizer conflicting with Booster 2.0.
1.30.1 (February 11th 2022):
- Fixed Improved Map System not working.
- Changed weapon icons and textbox lines (AsperDev).
- Fixed computer's hitbox.
- Fixed life bar going wack when getting hit with Iron Bond.
- Fixed items with floating NPC visuals not disappearing when unequipping the medal.
- Added new bosses to minimaps.
- Fixed time counter and level display in rankings.
1.30 (February 10th 2022):
Not compatible with previous seeds and saves.
- Added Miracle Matter collectable.
- Added new 13 items that can be crafted with Miracle Matter and collected items at computers.
- Fixed Nice Shoes's effect.
- Fixed Whimsical Star.
- Increased the falling speed of Heavy Weight, but changed damage to only be active with Ability key.
- Changed color of Chaos Key particles.
- Added Bracket character seed.
- Music fades out when exiting the level.
- Added rankings. When you die or win, your stats are saved in rankings.json.
- runs.txt is deprecated and can be safely deleted.
- Seed value is displayed properly beyond 1 billion.
- Fixed some bugs.

1.29.2 (February 5th 2022):
Not compatible with previous Profile.dat.
- Fixed the bug with collecting stars and life capsules colliding with chest flags.
- Timeline Ring flashes when cooldown ends.
- Timeline Ring shows equip icon when applicable.
- Adjusted Energy Whip lv3 by reducing slash ratio but increasing damage.
- Double jump procs before booster.
1.29.1 (February 4th 2022):
This update changes chest RNG.
- Fixed code optimizations.
- Nerfed Chaos Key: increased CD from 12 seconds to 20 seconds.
- Fixed Cool Sunglasses not showing correct damage numbers.
- Changed the visuals for stars and chicken legs. (AsperDev)
- Nerfed Bubbler lv3: reduced damage by 1, 70% reduced knockback.
- Nerfed Bubbler's recharge.
- Nerfed Cool Sunglasses's life regen boost.
- Added Whimsical Star and Timeline Ring as chest loot.
1.29-re1 (February 3rd 2022):
- Fixed Possessed Sue not unlocking the level after defeat.
1.29 (February 3rd 2022):
This is not compatible with 1.29 seeds.
- Fixed allies not actually appearing on Doctor's stage.
- Fixed Red Crystal not spawning rockets if Nice Skates is equipped.
- Reduced speed of circle projectiles.
- Curly at ending scene no longer looks up and synces with custom character.
- Removed vertical clouds from ending scene.
- Chaos Key no longer allows to teleport outside of map.
- Reduced the damage of lategame enemies projectiles to 5.
- Reduced HP of dragon buyobuyo by 100.
- Reduced ascending critter's damage from 4 to 3.
- Massively nerfed critters in post-Doctor stages.
- Laser gaudies fire lasers less frequently and with less velocity.
- Dragon buyobuyos fire fireballs less frequently and with less accuracy.
- Every boss was standartized to have 300 HP and 50 XP.
- Fixed Improved Map System display of Waterway water.
- Added icon display for config.ini options.
- Chests stop generating if there is no space in inventory to pick them up.
- Enemies cannot spawn in boss rooms.
- Chests on minimap have darker icon.
- Sara can negate damage only once every 480 frames.
- Nano Quiver projectiles no longer have knockback and do not produce sounds.
- Fixed door spawning in fast runs.
- Added Hovering Balrog and Possessed Sue as level bosses.
- Rewrote the description of Water Thursters.

1.28.4 (February 2nd 2022):
- Sara's direction switching is less janky.
- Sara can double jump while dashing.
- Added darkness config option.
1.28.3 (February 1st 2022):
- Fixed the bug with Doctor being stuck in place if enough damage is dealt.
- Reduced Doctor's HP by 100 in both phases.
- Reworked circle projectile attack: only one barrage, but the amount of projectiles and their speed is doubled.
- Reduced attack speed of Doctor P1's main attack.
- Sara's dash can be activated without gaining speed.
- Sara's dash fades out instantly.
- Falling-related items work in reversed gravity.
- Added an icon indicating if item is equipped.
- Added a reward for collecting every star on the level.
- Allies are properly preserved on Doctor's stage.
- Entering Doctor's stage resets star counter.
- Lava can only fill up the half of Hell/Arcadia level.
1.28.2 (January 31st 2022):
- Improved Map System no longer deletes old Map System out of inventory.
- Increased the size of IMS from 39x29 to 68x37 tiles.
- Added dark screen captures to minimap, inventory and pause screens.
- Opening inventory and finding items no longer resets i-frames.
- Reduced jingle duration for picking up items.
- Sara's dash is only activated holding Ability key.
- Sara's dash effects are not canceled with loss of speed.
- Sara gets damaged for 1 damage while dash is active.
- Lily's Rocket Launcher control reworked: holding Jump stops rocket in place, holding movement keys move the rocket, doesn't grant i-frames.
- Fixed TSC errors for Nemesis ammo.
- Inventory screen shows the health and weapon's experience.
- Changed weapon icons.
- Repetir.org loops earlier.
1.28.1 (January 30th 2022):
- Improved Map System actually works.
1.28 (January 30th 2022):
This is not compatible with 1.27 seeds.
- Fixed Cool Sunglasses expotentially scaling damage on piercing weapons.
- Fixed fast run config not working as intended.
- Reworked the TSC for level transitioning.
- This fixes item reequipping items on some of characters.
- Misery cannot drown anymore.
- Lily cannot get Creativity Cartridge anymore.
- Toroko starts with Bubbler.
- Added new universal ORG.
- Added Improved Map System as chest loot.
- Sara and Lily use Curly's hurt sounds.
- Changed UI and font.
- Changed item get jingle.

1.27.4 (January 29th 2022):
- Increased underwater movement speed.
- Decreased cold water's movement speed reduction.
- Ending sequence shows custom characters instead of just Quote.
- Increased knockback resistance on bosses.
- Increased Rifle's damage from 6x3/6x4/8x4 to 8x2/16x3/27x4, reduced its knockback and speed.
- Rifle does damage only every 16 frames per each enemy.
- Increased Rifle's XP requirements from 20/20/20 to 30/50/20.
- Decreased Energy Whip's XP requirements from 35/80/0 to 30/50/5.
- Fixed Lead Weight's TSC.
1.27.3 (January 28th 2022):
- Fixed Lead Weight effects activating when obtaining Air Tank.
- Updated Nice Skates sprite. (MintyFresh)
- Reduced ground acceleration in Hekichu Caverns to make it less annoying to navigate.
- Added 'tileset' variable to lock tileset.
- config.ini now reloads with each new level.
1.27.2 (January 27th 2022):
Changed chest RPG.
- Sara only falls slower if holding Jump.
- Updated item sprites (Taiku, AsperDev).
- Reworked Rocket Launcher: now pushes enemies forward, no longer renders player, massively reduced damage.
- Rocket Launcher gives fixed 30 iframes on landing.
- Lily can hold Z to slow down while using rocket launcher.
- Added Lead Weight as chest loot.
- Nerfed Mesa's block damage from 10 to 5.
- Nerfed Doctor's HP by 13%.
- Weapons that use no exp do not show numbers while collecting XP anymore.
- Fixed Nemesis breaking blocks down in reversed gravity.
1.27.1 (January 26th 2022):
- Updated backend to 2.0.20.
- Sara can instantly switch directions during her glide and doesn't lose it from a brief amount of time.
- Sara's glide damage is significantly reduced.
- Alien Medal's spike jump is less tight.
- Fixed minimap's closed chest debug thing.
- Fixed debug star requirement.
- Lily only gains 2 rockets from new stages instead of 20. 
1.27 (January 24th 2022):
- Fixed the bug with saving that would cause tileset to change.
- Recoded chests to eliminate "empty" chests and fix their saving.
- Reworked Living Waterway: no longer includes more water, instead uses rising water and bubble towers.
- Reworked Sand Zone to use more clustery generator.
- Reworked Labyrinth to use maze-like structures.
- Mimiga Villages no longer have water to prevent overflooding.
- Increased Hunter's jump height, but increased hunger rate and decreased movement speed.
- Increased chest limit to 44.
- Added Sara, Lily and Excerpt character seeds.
- Fixed Creation Energizer's function for momentum.

1.26.1 (January 21st 2022):
- Fully fixed time saving.
- The time is saved even when you decide to continue the run.
1.26 (January 20th 2022):
- Buffed Energy Whip's damage from 2/4/6 to 3/5/7.
- Buffed Fireball Level 3 to 9 damage total.
- Added Upntscale and Russia character seeds.
- Reduced spam of endgame enemies.
- Reworked Creation Energizer: now allows to place blocks remotely with mouse.
- Reworked Nice Shoes: now easier to control.
- Added Chaos Key into chest loot.
- Fixed 290.sec saving not working.
- Nerfed ice level's friction.
- Nerfed Frostbite Stopwatch: has limit on freeze frames, gives less frames and even less on Doctor.

1.25.2 (January 19th 2022):
- Fixed Nice Shoes and Battery Pack TSC.
1.25.1 (January 19th 2022):
Changed chest RNG.
- Changed chest distribution again to eliminate more empty chests.
- Equippable items change inventory text if they are equipped.
- Wall Hook now can stick player to the wall with Ability key (doesn't work with Toroko).
- Fixed new universal ORG not looping.
- 290.rec times are individually saved for each seed.
1.25 (January 18th 2022):
This version is not compatible with 1.24 seeds.
- Added 2 new tilesets from Kero Blaster (Tert).
- Added Nice Skates and Battery Pack as chest loot.
- Added a new universal BGM.
- Fixed the bug with energy whip being unusable.
- Fixed the bug with weapon selecting.
- Bute's arrows no longer home.
- Updated backend to 2.0.16.
- Added Mod Webpage button.
- 'mob_modifier' behavior is reversed.
- Pickup limit configs can increase amount of pickups.
- Added more info to extended info.

1.24 (January 17th 2022):
Changed terrain and object RNG.
- Updated item sprites. (AsperDev)
- Fixed Hell BGM not looping.
- Nerfed Doctor's both phases HP.
- Reworked bouncing projectile attack: shoots waves way faster, but projectiles disappear faster.
- Nerfed Doctor's phase 1 attack speed.
- Added challenge/modifier system, using inih and config.ini file.
- Doors actually open now.
- Fixed Doctor's initial speech TSC.
- Fixed bizzare Spur bullet behavior.
- Collecting a star shows starry particle on position of star and star indicator.

1.23 (January 16th 2022):
Changed terrain and chest RNG.
- Chests and refills are easier to distinguish on minimap.
- Added Boomerang Blades as chest loot.
- King gets Energy Whip and Boomerang Blades from start.
- Reduced King's movement speed.
- Dissolve damage timer from Core and Egg Corridor is no longer reset out of water.
- Most dangerous tilesets are not rolled for first 2 levels.
- Quick reset works in inventory and minimap screens.
- Buffed Nano Quiver's projectile rate.
- Updated readme to include credits.
- Fixed shock values on enemies.

1.22 (January 15th 2022):
This version is not compatible with 1.20 seeds.
- With 33% chance the new music will be chosen depending on tileset.
- Arcadia and Hell always play their new tracks.
- Improved Arcadia's tileset.
- Added two more universal ORGs.
- Overhauled weapon display to allow for 2 rows.
- Buffed Nano Quiver's projectile rate.
- Removed fireball from everyone's starting inventories.
- Reduced Energy Whip's spread and increased its attack rate.

1.21.2 (January 15th 2022):
Affects chest RNG.
- There is always 2 chests instead of 2-3 chests.
- Added Energy Whip as chest loot.
- Fixed Nano Quiver's TSC, reworked it to work better with Spur and Blade.
1.21.1 (January 13th 2022):
Affects chest RNG.
- Nerfed Propane Tank's flame firerate by 44%.
- Gravity Necklace's effect is less distracting.
- Snake gets autofire and splits into two projectiles with propane tank.
- Changed the chest generation algorithm again.
1.21 (January 12th 2022):
This version is not compatible with 1.20 seeds.
- Added Plantation, Core and Arcadia tilesets.
- Egg Corridor and Core water dissolve your skin. Equip Life Bond to negate.
- Buffed Critters and Bats appear after Doctor.
- Doctor's stage is properly marked as depth 10 now.
- Reworked backgrounds, now each stage has its own bk.
- Added Laser Gaudis as depth 9+ enemy.
- Increased Curly's ascend and descend speed.
- Decreased Curly's movement speed.
- Added Nano Quiver into chest loot.
- Added F2 quick reset hotkey.
- Water Gun no longer functions inside of Sand Zone or Outer Wall.
- Updated readme.

1.20 (January 11th 2022):
This version is not compatible with 1.19 seeds.
- Fixed the bug with iron bond which was negating all its buffs.
- To compensate, all iron bond cooldowns are increased by 60%.
- Fixed the bug with using dashes with different cooldowns.
- Reduced enemy amount growth with depth again and adjusted how they spawn:
* Critters no longer appear after depth 5 and there is 50% less critters on depth 5.
* Bats no longer appear after depth 4 and there is 50% less bats on depth 4.
* Beetles no longer appear after depth 5.
* Power critters appear starting on depth 2 and no longer appear after depth 8.
* Archers appear starting on depth 3.
* Skullheads and crows appear starting on depth 4 and no longer appear after depth 8.
- Reduced pot and spikes amount growth with depth.
- Buffed Doctor's damage and health, removed invincibility on some of his projectiles.
- Reverted Fade speedup.
- Added 4 ORGs by mLurker and Tinned Duck.
- Added new background tile.
1.19 (January 10th 2022):
This version is not compatible with 1.18 seeds.
- Improved water visuals.
- Flowing water overrides background tiles.
- Added water gun as chest loot.
- Added small background tiles that are placed like spikes.
- Fixed the bug with Curly not showing on pause screen.
- Fading in and out is significantly faster.

1.18.6 (January 9th 2022):
- Fixed the bug with absent life capsules. (cannot be fixed for saves)
- Buffed Curly's machine gun damage.
- Fixed Iron Bond UI visuals.
- Curly can recoil from any level of machine gun, with force depending on level.
- Curly's damage reduction effects works from 50% instead of 40%.
- Added readme file.
1.18.5 (January 9th 2022):
This update sorta breaks saves due to new code regarding life capsules. It just means than older saves get less life capsules.
- Reworked the handling for life capsules again to prevent life capsule save scumming.
- Rocket Launcher deals 4x less damage.
- Curly gets damage reduction on 40% HP instead of 30%.
- Defeating bosses is recorded on your save.
- You can't reload the save on Doctor's stage.
1.18.4 (January 9th 2022):
- Fixed the issue with chests sometimes being empty.
- Water Thrusters no longer impact ground dashes.
1.18.3 (January 9th 2022):
- Fixed the issue with life capsules disappearing with save reloading.
- King can dash extremely strong with blade in hands.
- King can't pick up dash items from chests.
- Buffed's King movement speed and acceleration.
- When below 30% HP, Curly has 33% damage reduction.
1.18.2 (January 9th 2022):
- Buffed King's Blade piercing, removed extended duration of slashing on level 2 and 3.
- Buffed's King movement speed and acceleration.
1.18.1 (January 9th 2022):
- Removed Cool Sunglasses from starting inventory.
- More descriptions support character names.
- Pause menu shows custom characters when applicable.
- Nerfed King's Blade piercing, buffed slash projectile's frequency.
- King's Blade can gain experience and upgrade.
- Buffed's King movement speed and acceleration.
- Window title shows the version of Randamu.
1.18 (January 8th 2022):
This version is not compatible with 1.17 seeds.
- Added ability to save. Still might be unstable, so please report any issues.
- Pause menu and inventory text supports custom characters when applicable.
- Changed some RNG to make things less dependent on each other.
- Added BuyoBuyo bases to floor 8 and lower.
- Implemented new limit on life capsules, now can't be cheesed by not picking up certain capsules.

1.17.4.1 (January 7th 2022):
- Fixed iframes overflow with Curly and Steel Plating.
1.17.4 (January 7th 2022):
- Curly has extra second of invulnerability when getting hit.
- Reduced XP requirements on Machine Gun from 30/40/10 to 20/20/20.
- Increased the limit for life capsules on Curly from 80 to 100.
- Increased Curly's jump height.
- Decreased Curly's max movement speed.
1.17.3 (January 7th 2022):
- Buffed Toroko's movement speed and jump height.
- Toroko can refuse to die once per stage, staying at 0 hp and gaining movement speed boost.
1.17.2.1 (January 7th 2022):
- Fixed Machine Gun being picked up instead of Fireball on Curly, for real this time.
1.17.2 (January 7th 2022):
- Decreased Iron Bond regeneration delay by 0.75 seconds.
- Playing as Curly decreases regeneration delay further by 0.58 seconds.
- Very slightly increased Curly's ground movement speed and acceleration.
- Decreased damage taken by King from 2x to 1.5x.
- Fixed increasing ammo on Machine Gun if Curly is used.
- Fixed Machine Gun being picked up instead of Fireball on Curly.
1.17.1 (January 7th 2022):
- Buffed Toroko's starting HP from 2 to 3.
1.17 (January 7th 2022):
Changes chest and object RNG.
- Changed drowning animation to be darker and to support equipped items.
- Fixed debug thing with exit doors.
- Heavy Weight now leaves afterimages with its ability.
- Added Gravity Necklace as chest loot.
- Added character seeds. `CurlyBrac` gives you Curly, `KingTable` gives you King, `FluffMimi` gives you Toroko, `Raymundo` gives you Misery.
- Redesigned depth counter.
- Added Turrets into depth 7 or lower.
- Fixed some bugs with scores.
- Saying No to death prompt closes the game.

1.16.1 (January 6th 2022):
- Fixed the crash with spawning Mannans.
1.16 (January 6th 2022):
- Moved Doctor's depth to end of depth 9.
- Dashing gives 15 frames of invulnerability at end of it to prevent cheap hits.
- Improved the visuals for dashing.
- Ending dash early decreases the cooldown of dash.
- Removed the ram functionality from Ballos Burger + Silver Locket.
- Added openworld visuals for many items.
- Added Copper Bulwark as a chest loot.
- Reworked Water Thrusters: no longer grant increased movement speed in water, but more upward momentum and grants fast dashing underwater.
- Fixed Curly clipping through tiles.
- Fixed Fishy Tube and Cool Sunglasses TSC overlapping.
- Added Mannans into Depth 5 and more.
- Massively reduced the max amount of spikes.
- Reduced the spread between min and max amount for many enemies, which should make stages less spammy.
- Increased the health limit for life capsules by 60.

1.15 (January 5th 2022):
Changed chest RNG.
- Fixed duplicated chest loot in depth 1 in some cases.
- Redesigned many item sprites, thanks to AsperDev.
- Reworked Creation Catalyst.
- Life Capsules give 3 HP instead of 4.
- Now you can actually finish the run by fighting buffed Doctor on end of depth 10.
- Added Silver Locket as chest loot.
- Added 3 more orgs for Doctor bossfight.
- Rocket Launcher deals 15x less damage in boss fights, fixed the crash with too many enemies.

1.14.5 (January 4th 2022):
- Attempt at fixing chests not respawning if they were not picked up again.
1.14.4 (January 4th 2022):
- Redesigned Fishy Motor into Fishy Tube. (AsperDev)
- Pair of Wings, Heavy Weight and Fishy Tube now use special Ability key for their abilities. On old saves it will be B, but default should be Left Shift.
- Sinking with Fishy Tube is even faster.
- Interrupting Rocket Launcher with Momentum Throttler now also uses Ability key.
- Reworked Momentum Throttler: now doesn't fully remove inertia, just makes Quote switch directions faster.
- Fixed Arms Barrier not filling up ammo in some cases, buffed ammo gain by 50%.
1.14.3 (January 4th 2022):
- Movement Throttler stops Rocket Launcher with Z and works properly.
- Quote no longer dies if he stands right on the edge of Outer Wall level.
- Hitting ground with rocket launcher no longer nullifies Quote's iframes for 20 frames.
- Heavy Weight's effects are triggered when holding Z instead of being stopped by Z.
1.14.2 (January 3rd 2022):
- Fixed seeds not working after first stage reset.
1.14.1 (January 3rd 2022):
- Fixed TSC overlap with Booster 2.0 and Overclock.
1.14 (January 3rd 2022):
- Recoded RNG algorithm. All previous seeds are useless.
- Custom seeds now support up to 9 letters, from 4, using same principle as TSC.
- Decreased regeneration delay of Iron Bond by 1 second.
- Descending with Fishy Motor is faster.

1.13 (January 3rd 2022):
Changes chest RNG.
- Polar Star, Nemesis and Rocket Launcher now nullify any collected EXP.
- Fixed the regeneration of chests that were not picked up.
- Quote will not die if he vents above Outer Wall level.
- Fixed Little Man and Clay Figure Medal TSCs.
- Momentum Throttler allows to stop Rocket Launcher with X button.
- Decreased Iron Bond's regeneration timer by 1 second.
- Iron Bond's timer doesn't get reset on getting damage with Heavy Plating, it also starts ticking on last second of invulnerability.
- Overclock allows to shoot one extra fireball.
- Buffed Steel Plating's nullification chance to 33%.
- Arms Barrier recharges ammo on Polar Star and Nemesis if you hold them.
- Pair of Wings now allow Quote to glide while holding Z.
- Cool Sunglasses increase Iron Bond's regeneration speed by 25%.
- Added Fishy Motor as chest loot.
- All bosses are now only provoked by damaging them.

1.12.1 (January 2nd 2022):
- Rocket Launcher doesn't level up anymore.
- Fixed the bug with having max amount of weapons.
1.12 (January 2nd 2022):
Not good with 1.11 seeds, yada yada.
- Added 6 more orgs, from 2bro, Zenix and Tinned Duck, replaced 1 org.
- Finished Outer Wall tileset.
- Added Steel Plating, Momentum Throttler and Cool Sunglasses as chest loot.
- Reworked Heavy Weight: no longer gives you expotential fall speed, allows to slow down with Z.
- Reworked Speed Shoes: no longer accelerates in air, only on ground, creates a trail when you approach max speed.
- Changed Frostbite Stopwatch to not nullify damage from chilling bullets.
- Changed minimap to show important items with individual colors.
- Fixed First Cave tileset.

1.11.1 (January 1st 2022):
- Fixed Curly showing up as little man.
- Added custom taskbar icon.
1.11 (January 1st 2022):
Not compatible with 1.10 seeds, you get the gist.
- Level generation, chest placement and entity placement use separate RNGs, this will assure than future updates will break seeds less frequently.
- Added 4 more orgs and replaced Lol3 with Temporal from Zenix.
- Added Eggs tileset.
- Fixed chest bugs for real.
- Added extended info into pause menu.
- Moved custom seed scores into separate scores.
- Added "outer_side_pls" secret.
- Added version text into seed logs.
- Fixed music being stuck at Meltdown 2.

1.10 (January 1st 2022):
THIS VERSION IS INCOMPATIBLE WITH 1.9 SEEDS!
- Finally fixed "empty" full chests.
- Fixed the issue of infinite Overclocks.
- Added Little Man as chest loot.
- Reduced the increase of spikes with each depth by 50%.
- Chests cannot spawn on each other.

1.9.0.1 (December 31st 2021):
- Fixed the crash with placement of exit door.
1.9 (December 31st 2021):
THIS VERSION IS NOT COMPATIBLE WITH 1.8.3 SEEDS!
- Presses deal 13 damage instead of 127.
- Reduced the amount of enemies and adjusted how much enemies is added with depth.
- Added Life Pot, Clay Figure Medal, Iron Bond, Propane Tank, Overclock, Destruction Energizer, Creation Energizer as chest loot.
- Chests, life capsules and refill stations cannot spawn on each other.
- Doors into next stage now always spawn with tile above them.

1.8.3.1 (December 30th 2021):
- Fixed "Seed" text in TextBox.png.
1.8.3 (December 30th 2021):
- The game now tracks highscores for custom seeds.
- Custom seed is shown in inventory if set.
1.8.2 (December 30th 2021):
- Added Map System item into inventory.
- Adjusted Speed Shoes's and Heavy Weight's speed multipliers to make them more usable.
1.8.1 (December 30th 2021):
- Fixed the looping for one of randomized songs.
1.8 (December 30th 2021):
- Repackaged some of 1.7.1 improvements like lava and freeze fixes (they weren't in actual 1.7.1 for some reason).
- Added Frostbite Stopwatch as chest loot.
- Added second alternative solid tile type.
- Level generation uses separate RNG generator, so seeds are useful beyond depth 1.

1.7.1 (December 30th 2021):
- Fixed certain world gen freezes.
- Fixed "underwater" water surface tiles.
- Numerical seeds are properly interpreted now.
- Hell's lava is semitransparent.

1.7 (December 29th 2021):
- Added highscore tracking and UI adjustments by Kaitlyn.
- Game tracks your score from each failed run.
- Fixed Heavy Weight not appearing at all.
- Added Ballos Burger.
- Buffed Bubbler and Fireball's range and damage.
- Buffed Bubbler and Machine Gun's firerate.
- Added 7 more orgs (3 from KB and 3 from 2dbro)
- Reduced Hell's spikes multiplier from 3x to 2x.
- Hell levels have 7 more stars.

1.6 (December 29th 2021):
- Moved lone refill station in Hell levels next to player.
- Air Tank provides 8 seconds of immunity to Hell's lava.
- Fighting bosses now plays Eye of Flame.
- Starting door poofs out of existence.
- Stars cannot spawn in Hell's lava.
- Jumping off spikes is easier.
- Stars no longer delete tiles.
- Fixed uncharged Spur's range.

1.5 (December 29th 2021):
- Added seed entering. Just use `seed.txt` with any number below 2^32 or any 4-letter combination.
- Score and stars are shown when event is ongoing.
- Added 3 randomly-chosen bosses at each 4th depth, they hold the door to next level.
- While boss is active, you can't break blocks.
- Added Grasstown and Hell tilesets.
- Added backgrounds to each tileset.
- Tilesets now alter level gen, some are smaller changes, some are bigger.
- Added Turbocharge, Heavy Weight and Alien Medal as chest loot.
- Added Crows, Skullheads and Power Critters as mobs encountered from depth 3.
- Ceilings and chains are sorta affected by gravity.
- Adjusted some mob score values.

1.4.1 (December 28th 2021):
- Fixed typos and errors with Creativity Cartridge in inventory.
1.4 (December 28th 2021):
- Reworked depth counter, depth also affects the amount of enemies and obstacles.
- Added score, you can obtain score by obtaining stars and treasures and killing enemies.
- Reworked points of interest on minimap. They also now disappear properly.
- Added Arms Barrier, Energy Canister, Curly's Underwear and Creativity Cartridge as chest loot.
- Chest loot can reappear if you didn't picked it up in previous level.
- Life Capsules stop spawning if you hit 55 HP.
- Quote on minimap now fits into grid.

1.3 (December 28th 2021):
- Nerfed critter's damage and jump height.
- Reworked archers: they shoot a lot faster, their arrows have randomized trajectories and home on player slightly, but only appear if player has 10 HP.
- Added destroyable pots.
- Added Waterway tileset.
- Both versions of Meltdown can play randomly.
- Added Rocket Launcher, Auxiliary Booster, Wall Hook.
- Modded items can be equipped and unequipped.

1.2 (December 27th 2021):
- Butes can fall.
- Fixed life capsules disappearing.
- Fixed empty chests.
- Added Speed Shoes, Pair of Wings and Water Thrusters.
- Increased amount of stars generated.

1.1 (December 27th 2021):
- Reduced map size from 200x200 to 118x118 and tweaked level generation.
- Fixed Polar Star's block placement.
- Fixed escalating amount of enemies.
- Fixed Spur placing blocks.
- Changed PRNG algorithm.
- Added Bute Archers, Beetles and Jellies.
- Added background tile generation.
- Added chains, ceilings and refill stations.
- Fixed several issues with map's boundary.
- Solid tiles can be randomly replaced.
- Added Fireball as starting weapon.
- Some objects and spikes can be fall.
- Quote starts with 6 HP instead of 3.

1.0 (December 26th 2021):
- Initial release.