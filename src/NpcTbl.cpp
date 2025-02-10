#include "NpcTbl.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "File.h"
#include "Generic.h"
#include "Log.h"
#include "Main.h"
#include "NpcAct.h"
#include "ResourceCode.h"
#include "Stage.h"
#include "Tags.h"

// Npc function table
const NPCFUNCTION gpNpcFuncTbl[] = {
	ActNpcNull,
	ActNpcXP,
	ActNpc002,
	ActNpc003,
	ActNpcSmoke,
	ActNpc005,
	ActNpcBeetle,
	ActNpc007,
	ActNpc008,
	ActNpc009,
	ActNpc010,
	ActNpc011,
	ActNpc012,
	ActNpc013,
	ActNpc014,
	ActNpcClosedChest,
	ActNpcSavePoint,
	ActNpcRefillStation,
	ActNpcDoor,
	ActNpc019,
	ActNpcTerminal,
	ActNpcOpenChest,
	ActNpcTeleporter,
	ActNpc023,
	ActNpcPowerCritter,
	ActNpc025,
	ActNpcBatEnemy,
	ActNpc027,
	ActNpcGrassCritter,
	ActNpc029,
	ActNpcGrassObject,
	ActNpc031,
	ActNpcLifeCapsule,
	ActNpc033,
	ActNpc034,
	ActNpcMannan,
	ActNpc036,
	ActNpcTutorialSign,
	ActNpc038,
	ActNpc039,
	ActNpc040,
	ActNpc041,
	ActNpc042,
	ActNpc043,
	ActNpc044,
	ActNpc045,
	ActNpc046,
	ActNpc047,
	ActNpc048,
	ActNpcSkeleton,
	ActNpc050,
	ActNpc051,
	ActNpc052,
	ActNpc053,
	ActNpc054,
	ActNpc055,
	ActNpc056,
	ActNpcBird,
	ActNpc058,
	ActNpc059,
	ActNpc060,
	ActNpc061,
	ActNpc062,
	ActNpc063,
	ActNpc064,
	ActNpc065,
	ActNpc066,
	ActNpc067,
	ActNpc068,
	ActNpcSheep,
	ActNpcCampfire,
	ActNpc071,
	ActNpc072,
	ActNpc073,
	ActNpc074,
	ActNpc075,
	ActNpc076,
	ActNpc077,
	ActNpcPot,
	ActNpc079,
	ActNpc080,
	ActNpc081,
	ActNpc082,
	ActNpc083,
	ActNpc084,
	ActNpc085,
	ActNpcStar,
	ActNpcHeart,
	ActNpc088,
	ActNpc089,
	ActNpc090,
	ActNpc091,
	ActNpc092,
	ActNpc093,
	ActNpcKulala,
	ActNpcJelly,
	ActNpc096,
	ActNpcFirethrower,
	ActNpc098,
	ActNpcGraviController,
	ActNpcPlatform,
	ActNpc101,
	ActNpc102,
	ActNpcMannanProjectile,
	ActNpcTPFrog,
	ActNpc105,
	ActNpc106,
	ActNpc107,
	ActNpc108,
	ActNpc109,
	ActNpcLaserFrog,
	ActNpc111,
	ActNpc112,
	ActNpc113,
	ActNpc114,
	ActNpcMechaRavil,
	ActNpc116,
	ActNpc117,
	ActNpc118,
	ActNpc119,
	ActNpc120,
	ActNpc121,
	ActNpc122,
	ActNpc123,
	ActNpcNanitesTrail,
	ActNpc125,
	ActNpcPuppy,
	ActNpc127,
	ActNpc128,
	ActNpc129,
	ActNpc130,
	ActNpc131,
	ActNpc132,
	ActNpc133,
	ActNpc134,
	ActNpc135,
	ActNpc136,
	ActNpc137,
	ActNpc138,
	ActNpc139,
	ActNpcShadowToroko,
	ActNpc141,
	ActNpc142,
	ActNpc143,
	ActNpc144,
	ActNpc145,
	ActNpc146,
	ActNpcMainCritter,
	ActNpc148,
	ActNpc149,
	ActNpc150,
	ActNpc151,
	ActNpc152,
	ActNpc153,
	ActNpc154,
	ActNpcLaserGaudi,
	ActNpcLaserProjectile,
	ActNpc157,
	ActNpc158,
	ActNpc159,
	ActNpc160,
	ActNpc161,
	ActNpc162,
	ActNpc163,
	ActNpc164,
	ActNpc165,
	ActNpc166,
	ActNpc167,
	ActNpc168,
	ActNpc169,
	ActNpcMissile,
	ActNpc171,
	ActNpc172,
	ActNpcNinjaGaudi,
	ActNpc174,
	ActNpc175,
	ActNpcFireyBuyo,
	ActNpc177,
	ActNpc178,
	ActNpc179,
	ActNpcLittleMan,
	ActNpc181,
	ActNpc182,
	ActNpc183,
	ActNpc184,
	ActNpc185,
	ActNpc186,
	ActNpcSteelCore,
	ActNpcSteelFuzz,
	ActNpc189,
	ActNpc190,
	ActNpc191,
	ActNpc192,
	ActNpc193,
	ActNpc194,
	ActNpc195,
	ActNpc196,
	ActNpc197,
	ActNpc198,
	ActNpc199,
	ActNpcDragon,
	ActNpc201,
	ActNpcFlameProjectile,
	ActNpc203,
	ActNpc204,
	ActNpc205,
	ActNpcCastleBomb,
	ActNpc207,
	ActNpc208,
	ActNpcSemiHomingProjectile,
	ActNpc210,
	ActNpc211,
	ActNpc212,
	ActNpcWindController,
	ActNpcFreezeWind,
	ActNpc215,
	ActNpc216,
	ActNpc217,
	ActNpc218,
	ActNpc219,
	ActNpc220,
	ActNpc221,
	ActNpc222,
	ActNpc223,
	ActNpc224,
	ActNpc225,
	ActNpc226,
	ActNpc227,
	ActNpc228,
	ActNpc229,
	ActNpcEnergyFlower,
	ActNpcRocket,
	ActNpc232,
	ActNpc233,
	ActNpc234,
	ActNpc235,
	ActNpc236,
	ActNpc237,
	ActNpcTurret,
	ActNpcMiniTurret,
	ActNpc240,
	ActNpcDuplicateCritter,
	ActNpc242,
	ActNpc243,
	ActNpc244,
	ActNpc245,
	ActNpcMainPress,
	ActNpcMiseryEnemy,
	ActNpc248,
	ActNpc249,
	ActNpc250,
	ActNpc251,
	ActNpc252,
	ActNpcMiracleMatter,
	ActNpc254,
	ActNpc255,
	ActNpc256,
	ActNpc257,
	ActNpc258,
	ActNpc259,
	ActNpc260,
	ActNpc261,
	ActNpc262,
	ActNpcDoctorP1,
	ActNpc264,
	ActNpc265,
	ActNpc266,
	ActNpcDoctorP2,
	ActNpcIgorBoss,
	ActNpcRedBat,
	ActNpc270,
	ActNpc271,
	ActNpc272,
	ActNpc273,
	ActNpc274,
	ActNpc275,
	ActNpc276,
	ActNpc277,
	ActNpc278,
	ActNpcFallingBlock,
	ActNpc280,
	ActNpc281,
	ActNpc282,
	ActNpcWildMiseryEnemy,
	ActNpc284,
	ActNpc285,
	ActNpc286,
	ActNpc287,
	ActNpc288,
	ActNpc289,
	ActNpc290,
	ActNpc291,
	ActNpc292,
	ActNpc293,
	ActNpcFallingBlockController,
	ActNpc295,
	ActNpc296,
	ActNpc297,
	ActNpc298,
	ActNpc299,
	ActNpc300,
	ActNpc301,
	ActNpc302,
	ActNpc303,
	ActNpc304,
	ActNpc305,
	ActNpc306,
	ActNpc307,
	ActNpc308,
	ActNpc309,
	ActNpc310,
	ActNpcArcher,
	ActNpcArcherArrow,
	ActNpc313,
	ActNpc314,
	ActNpc315,
	ActNpc316,
	ActNpc317,
	ActNpc318,
	ActNpc319,
	ActNpcCurlyAlly,
	ActNpc321,
	ActNpcTNT,
	ActNpc323,
	ActNpc324,
	ActNpc325,
	ActNpc326,
	ActNpc327,
	ActNpc328,
	ActNpc329,
	ActNpcCastleRoller,
	ActNpc331,
	ActNpc332,
	ActNpc333,
	ActNpc334,
	ActNpc335,
	ActNpc336,
	ActNpc337,
	ActNpc338,
	ActNpc339,
	ActNpc340,
	ActNpc341,
	ActNpc342,
	ActNpc343,
	ActNpc344,
	ActNpc345,
	ActNpc346,
	ActNpc347,
	ActNpc348,
	ActNpc349,
	ActNpc350,
	ActNpc351,
	ActNpc352,
	ActNpc353,
	ActNpc354,
	ActNpc355,
	ActNpc356,
	ActNpc357,
	ActNpc358,
	ActNpc359,
	ActNpc360,
};

NPC_TABLE *gNpcTable;
std::vector<int> waterBullets;
std::map<std::string, NPC_SPAWN_CONFIG> npcSpawning;

template<typename ValueType>
void InterpretSpawnConfig(ValueType& value, json& config, std::string&& key, ValueType default_value, bool hasParent = false){
	if (!hasParent || config.contains(key))
			value = config.value<ValueType>(key, default_value);
}

template<typename ValueType>
void InterpretSpawnConfig(ValueType& value, json& config, ValueType required_value, std::string&& check_key, bool hasParent = false){
	if (!hasParent || config.contains(check_key))
			value = required_value;
}

void LoadNPCTags(){
	bool bError = false;
		waterBullets = ParseTag<int>("Tags/waterBullets.json");

		json npcSpawningProto = ReadJSON("Tags/npcSpawning.json");
		for (auto &[key, val] : npcSpawningProto.items())
		{
			bool hasParent = false;
			NPC_SPAWN_CONFIG config = {};
			if (val.contains("parent")){
				hasParent = true;
				auto parentName = val["parent"].get<std::string>();
				if (std::find_if(npcSpawning.begin(), npcSpawning.end(), [parentName](std::pair<std::string, NPC_SPAWN_CONFIG> p){return p.first == parentName;}) != npcSpawning.end()){
					auto parentConfig = npcSpawning[parentName];
					config = parentConfig;
				}
			}
			InterpretSpawnConfig<int>(config.npcCode, val, "ID", 0, hasParent);
			InterpretSpawnConfig<int>(config.minNpcs, val, "minCount", 0, hasParent);
			InterpretSpawnConfig<int>(config.maxNpcs, val, "maxCount", 0, hasParent);
			InterpretSpawnConfig<int>(config.minDepth, val, "minDepth", 1, hasParent);
			InterpretSpawnConfig<int>(config.maxDepth, val, "maxDepth", 99, hasParent);
			InterpretSpawnConfig<int>(config.distanceFromPlayer, val, "distance", 10, hasParent);
			InterpretSpawnConfig<int>(config.extraNpcs, val, "scaling", 0, hasParent);
			InterpretSpawnConfig<bool>(config.requiresSurface, val, "surface", true, hasParent);
			InterpretSpawnConfig<bool>(config.requiresEmptySpace, val, "air", false, hasParent);
			InterpretSpawnConfig<bool>(config.requiresNoEmptySpace, val, "ground", false, hasParent);
			InterpretSpawnConfig<bool>(config.requiresCeiling, val, "ceiling", false, hasParent);
			InterpretSpawnConfig<bool>(config.requiresWall, val, "wall", false, hasParent);
			InterpretSpawnConfig<int>(config.gimmickRequirement, val, GetGimmick(val.value("gimmick", "-1")), "gimmick", hasParent);
			InterpretSpawnConfig<int>(config.tilesetRequirement, val, GetTileset(val.value("tileset", "-1")), "tileset", hasParent);
			InterpretSpawnConfig<bool>(config.countFromDoctor, val, "secondCampaign", false, hasParent);
			InterpretSpawnConfig<bool>(config.wallDirection, val, "faceAwayFromWall", false, hasParent);
			InterpretSpawnConfig<bool>(config.bothCeilingAndSurface, val, "stickToCeiling", false, hasParent);
			InterpretSpawnConfig<bool>(config.logicType, val, "allConditionsMustBeTrue", true, hasParent);
			InterpretSpawnConfig<bool>(config.destroyTile, val, "trapBlock", false, hasParent);
			InterpretSpawnConfig<int>(config.sideOffset, val, "sideOffset", 0, hasParent);
			InterpretSpawnConfig<int>(config.verticalOffset, val, "verticalOffset", 0, hasParent);
			InterpretSpawnConfig<bool>(config.standardOrder, val, "spawnNormally", true, hasParent);
			InterpretSpawnConfig<bool>(config.randomDirection, val, "randomFacing", false, hasParent);
			InterpretSpawnConfig<bool>(config.mobModifier, val, "affectedByCountModifier", true, hasParent);
			InterpretSpawnConfig<bool>(config.canOverlap, val, "canOverlap", true, hasParent);
			InterpretSpawnConfig<int>(config.direct, val, "direction", -1, hasParent);
			InterpretSpawnConfig<int>(config.onTopOfMob, val, "npcToSearch", 0, hasParent);
			if (val.contains("modifiers"))
			{
				auto mods = val["modifiers"].get<std::map<std::string, float>>();
				for (auto &[id, modifier] : mods)
				{
					char tilesetID = GetTileset(id);
					if (tilesetID != -1)
						config.countModifiers.emplace(tilesetID, modifier);
				}
			}
			npcSpawning[key] = config;
		}
}

bool LoadNpcTable(const char *path)
{
	FILE *fp;
	int n;
	size_t size;
	int num;
	std::string truePath = GetResourcePath(path);

	size = GetFileSizeLong(truePath.c_str());	// TODO - Investigate whether GetFileSizeLong actually returns an unsigned long or not
	if (size == -1)
		return false;

	num = (int)(size / 0x18);

	gNpcTable = (NPC_TABLE*)malloc(num * sizeof(NPC_TABLE));
	if (gNpcTable == NULL)
		return false;

	fp = GetResource(path);
	if (fp == NULL)
	{
		free(gNpcTable);
		gNpcTable = NULL;
		return false;
	}

	for (n = 0; n < num; ++n) // bits
		gNpcTable[n].bits = File_ReadLE16(fp);
	for (n = 0; n < num; ++n) // life
		gNpcTable[n].life = File_ReadLE16(fp);
	for (n = 0; n < num; ++n) // surf
		fread(&gNpcTable[n].surf, 1, 1, fp);
	for (n = 0; n < num; ++n) // destroy_voice
		fread(&gNpcTable[n].destroy_voice, 1, 1, fp);
	for (n = 0; n < num; ++n) // hit_voice
		fread(&gNpcTable[n].hit_voice, 1, 1, fp);
	for (n = 0; n < num; ++n) // size
		fread(&gNpcTable[n].size, 1, 1, fp);
	for (n = 0; n < num; ++n) // exp
		gNpcTable[n].exp = File_ReadLE32(fp);
	for (n = 0; n < num; ++n) // damage
		gNpcTable[n].damage = File_ReadLE32(fp);
	for (n = 0; n < num; ++n) // hit
		fread(&gNpcTable[n].hit, 4, 1, fp);
	for (n = 0; n < num; ++n) // view
		fread(&gNpcTable[n].view, 4, 1, fp);

	fclose(fp);
	return true;
}

void ReleaseNpcTable(void)
{
	if (gNpcTable != NULL)
	{
		free(gNpcTable);
		gNpcTable = NULL;
	}
}
