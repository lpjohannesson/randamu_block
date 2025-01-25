#include "Profile.h"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>

#include "Backends/Misc.h"
#include "ArmsItem.h"
#include "BossLife.h"
#include "Cooldown.h"
#include "Fade.h"
#include "File.h"
#include "Flags.h"
#include "Frame.h"
#include "Game.h"
#include "Generic.h"
#include "Json.h"
#include "Log.h"
#include "Main.h"
#include "Map.h"
#include "MiniMap.h"
#include "MyChar.h"
#include "NpChar.h"
#include "ResourceCode.h"
#include "SelStage.h"
#include "Score.h"
#include "Stage.h"
#include "Star.h"
#include "TextScr.h"
#include "ValueView.h"

const char* const gDefaultName = "Profile.json";
const char* const gProfileCode = "Do041220";
const char* const gProfileCodeExtra = "CSE2E   20200304";

#define MAX(a, b) ((a) > (b) ? (a) : (b))

bool IsProfile(void)
{
	return IsKeyFile("Profile.json");
}

#define PROFILE_VERSION "_version"
#define PROFILE_STAGE 	"stage"
#define PROFILE_X 		"mcX"
#define PROFILE_Y 		"mcY"
#define PROFILE_DIRECT  "direction"
#define PROFILE_HP      "hp"
#define PROFILE_MAX_HP  "max_hp"
#define PROFILE_STAR    "ducks"
#define PROFILE_SELECT_ARM    "selectedArm"
#define PROFILE_SELECT_ITEM   "selectedItem"
#define PROFILE_EQUIPS   "equipFlags"
#define PROFILE_UNIT   "unit"
#define PROFILE_COUNTER   "playTime"
#define PROFILE_A   "stars"
#define PROFILE_DEPTH   "depth"
#define PROFILE_SCORE   "score"
#define PROFILE_LIMIT   "lcLimit"
#define PROFILE_DIFFICULTY   "difficulty"
#define PROFILE_SEED   "seed"
#define PROFILE_ARMS   "weapons"
#define PROFILE_ITEMS   "items"
#define PROFILE_SELSTAGE   "crafts"
#define PROFILE_MAPPING   "zzz_mapping"
#define PROFILE_FLAGS   "zzz_flags"
#define PROFILE_MIM   "character"
#define PROFILE_MIRACLE   "miracleAmount"
#define PROFILE_MIRACLE_DEPTH   "miracleNextDepth"
#define PROFILE_GEN_MIRACLE   "miracleGenerated"
#define PROFILE_PHYSICS   "physicsNormal"
#define PROFILE_PHYSICS_UNDERWATER   "physicsUnderwater"
#define PROFILE_NO_WATER   "waterEffects"
#define PROFILE_MAP_MODS   "zzz_mapModifications"
#define PROFILE_GEN_CHESTS   "zzz_chestInfo"
#define PROFILE_AVAILABLE_IDS   "zzz_chestPool"
#define PROFILE_COOLDOWNS "cooldowns"
#define PROFILE_CHARACTER "character"

bool SaveProfile(const char *name)
{
	auto timing = std::chrono::steady_clock::now();
	std::string path;

	// Get path
	if (name != NULL)
		path = gModulePath + '/' + name;
	else
		path = gModulePath + '/' + gDefaultName;

	json profileJson = json::object();

	// Set up profile
	profileJson[PROFILE_VERSION] = 5;
	profileJson[PROFILE_STAGE] = gStageNo;
	profileJson[PROFILE_X] = gMC.x;
	profileJson[PROFILE_Y] = gMC.y;
	profileJson[PROFILE_DIRECT] = gMC.direct;
	profileJson[PROFILE_HP] = gMC.life;
	profileJson[PROFILE_MAX_HP] = gMC.max_life;
	profileJson[PROFILE_STAR] = gMC.star;
	profileJson[PROFILE_SELECT_ARM] = gSelectedArms;
	profileJson[PROFILE_SELECT_ITEM] = gSelectedItem;
	profileJson[PROFILE_UNIT] = gMC.unit;
	profileJson[PROFILE_COUNTER] = gCounter;
	profileJson[PROFILE_A] = gMC.a;
	profileJson[PROFILE_DEPTH] = gMC.depth;
	profileJson[PROFILE_LIMIT] = gMC.lifeCapsuleLimit;
	profileJson[PROFILE_SEED] = gMC.seed;
	profileJson[PROFILE_SCORE] = gMC.score;
	profileJson[PROFILE_ARMS] = gArmsData;
	profileJson[PROFILE_ITEMS] = gItemData;
	profileJson[PROFILE_SELSTAGE] = gPermitStage;
	profileJson[PROFILE_MAPPING] = gMapping;
	profileJson[PROFILE_DIFFICULTY] = gMC.difficulty;
	profileJson[PROFILE_COOLDOWNS] = gCooldowns;

	//saving refills
	int n = -1;
	for (int i = 0; i < NPC_MAX; i++){
		if (gNPC[i].code_char == 17){
			n++;
			if (gNPC[i].count1 == 1)
				SetNPCFlag(Flags::REFILL_SAVE_FLAG_BEGIN + n);
		}
	}

	profileJson[PROFILE_FLAGS] = json::array();
	for (int i = 0; i < 8000; i++){
		if (GetNPCFlag(i)){
			profileJson[PROFILE_FLAGS].push_back(i);
		}
	}

	profileJson[PROFILE_MIRACLE] = gMC.miracle;
	profileJson[PROFILE_GEN_MIRACLE] = gMC.miracleGenerated;
	profileJson[PROFILE_MIRACLE_DEPTH] = gMC.miracleDepth;
	profileJson[PROFILE_PHYSICS] = gMC.physics_normal;
	profileJson[PROFILE_PHYSICS_UNDERWATER] = gMC.physics_underwater;
	profileJson[PROFILE_NO_WATER] = gMC.no_splash_or_air_limit_underwater;
	auto saveMapMods = json::object({});
	for (const auto& [coords, tile]: mapModifications){
		auto coords_key = std::to_string(coords.first) + " " + std::to_string(coords.second);
		saveMapMods[coords_key] = tile;
	}
	profileJson[PROFILE_MAP_MODS] = saveMapMods;
	profileJson[PROFILE_GEN_CHESTS] = gMC.generatedChests;
	profileJson[PROFILE_AVAILABLE_IDS] = gAvailableIDs;
	profileJson[PROFILE_CHARACTER] = gMC.characterData.codeName;
	SaveRNG(profileJson);
	WriteJSON(path, profileJson, mode::normal, 3);
	WriteGameLog("Profile saved in ", since(timing).count(), " ms");
	return true;
}

bool LoadProfile(const char *name)
{
	auto timing = std::chrono::steady_clock::now();
	std::string path;

	// Get path
	if (name != NULL)
		path = name;
	else
		path = gModulePath + '/' + gDefaultName;

	if (!IsKeyFile(gDefaultName))
		return false;

	// Read data
	json profileJson = ReadJSON(path, mode::normalGlobal);

	// Set things
	int version = 1;
	if (profileJson.contains(PROFILE_VERSION))
		version = profileJson[PROFILE_VERSION].get<int>();
	gSelectedArms = profileJson[PROFILE_SELECT_ARM].get<int>();
	gSelectedItem = profileJson[PROFILE_SELECT_ITEM].get<int>();
	gCounter = profileJson[PROFILE_COUNTER].get<int>();
	auto pArms = profileJson[PROFILE_ARMS].get<std::vector<ARMS>>();
	std::copy(pArms.begin(), pArms.end(), gArmsData);
	auto pItems = profileJson[PROFILE_ITEMS].get<std::vector<ITEM>>();
	std::copy(pItems.begin(), pItems.end(), gItemData);
	if (version < 4){
		auto pStage = profileJson[PROFILE_SELSTAGE].get<std::vector<PERMIT_STAGE>>();
		gPermitStage.swap(pStage);
	} else {
		gPermitStage = profileJson[PROFILE_SELSTAGE].get<std::vector<PERMIT_STAGE>>();
		auto gC = profileJson[PROFILE_COOLDOWNS].get<std::vector<Cooldown>>();
		gCooldowns.clear();
		for (auto& cd: gC){
			InitiateCooldown(gCooldowns, cd.id, cd.duration);
		}
	}
	auto pMapping = profileJson[PROFILE_MAPPING].get<std::vector<unsigned char>>();
	std::copy(pMapping.begin(), pMapping.end(), gMapping);
	auto pFlags = profileJson[PROFILE_FLAGS].get<std::vector<int>>();
	InitFlags();
	for (auto flag: pFlags){
		SetNPCFlag(flag);
	}
	InitMyChar();
	gMC.difficulty = profileJson[PROFILE_DIFFICULTY].get<int>();
	gMC.depth = profileJson[PROFILE_DEPTH].get<int>();
	gMC.lifeCapsuleLimit = profileJson[PROFILE_LIMIT].get<int>();
	gMC.miracle = profileJson[PROFILE_MIRACLE].get<int>();
	gMC.miracleGenerated = profileJson[PROFILE_GEN_MIRACLE].get<int>();
	gMC.miracleDepth = profileJson[PROFILE_MIRACLE_DEPTH].get<int>();
	if (version >= 3){
		mapModifications = {};
		for (const auto& [coordsKey, tile] : profileJson[PROFILE_MAP_MODS].items()){
			auto coords = std::pair<short, short>{};
			std::size_t pointer;
			coords.first = std::stoi(coordsKey, &pointer);
			coords.second = std::stoi(coordsKey.substr(pointer));
			mapModifications.insert_or_assign(coords, tile);
		}
	} else if (version == 2){
		auto str = profileJson[PROFILE_MAP_MODS].get<std::string>();
		for (int i = 0; i < 13924; i++){
			char tile = str[i];
			if (tile == '_') tile = 0xFF;
			mapModifications.insert_or_assign({i % 118, i / 118}, tile);
		}
	}
	auto pChst = profileJson[PROFILE_GEN_CHESTS].get<std::vector<int>>();
	std::copy(pChst.begin(), pChst.end(), gMC.generatedChests);
	gAvailableIDs = profileJson[PROFILE_AVAILABLE_IDS].get<std::vector<int>>();
	gMC.seed = profileJson[PROFILE_SEED].get<int>();
	gMC.randomSeed = false;
	LoadRNG(profileJson);
	gMC.x = profileJson[PROFILE_X].get<int>();
	gMC.y = profileJson[PROFILE_Y].get<int>();
	if (!TransferStage(profileJson[PROFILE_STAGE].get<int>(), 0, gMC.x / 16_pix, gMC.y / 16_pix))
		return false;

	// Set character properties

	if (version < 5){
		// Migrate passive items into flags
		int equipData = profileJson[PROFILE_EQUIPS].get<int>();
		if (equipData & 16)
			SetNPCFlag(709);
		if (equipData & 4)
			SetNPCFlag(702);
		if (equipData & 32768)
			SetNPCFlag(706);
		if (equipData & 8)
			SetNPCFlag(704);
		// Migrate character data
		static std::vector<std::string> characterDefs = {"Quote", "CurlyBrac", "KingTable", "FluffMimi", "Raymundo", "Upntscale", "Russia", "SaraACSS", "ColdWorkE", "WTFEvilQu", "Jamark", "Dog", "Magician", "LuigiGuy", "Meme", "God", "AmeManga", "SamHogan", "LunarSha", "Sakamoto"};
		profileJson[PROFILE_CHARACTER] = characterDefs[profileJson[PROFILE_MIM].get<unsigned int>()];
	}
	gMC.unit =  profileJson[PROFILE_UNIT].get<int>();
	gMC.direct = profileJson[PROFILE_DIRECT].get<int>();
	gMC.max_life = profileJson[PROFILE_MAX_HP].get<short>();
	gMC.life = profileJson[PROFILE_HP].get<short>();
	gMC.star = profileJson[PROFILE_STAR].get<short>();
	gMC.cond = 0x80;
	gMC.air = 1000;
	gMC.lifeBr = profileJson[PROFILE_HP].get<int>();
	gMC.a = profileJson[PROFILE_A].get<short>();
	gMC.score = profileJson[PROFILE_SCORE].get<int>();

	gMC.rect_arms.left = (gArmsData[gSelectedArms].code % 10) * 24;
	gMC.rect_arms.right = gMC.rect_arms.left + 24;
	gMC.rect_arms.top = (gArmsData[gSelectedArms].code / 10) * 32;
	gMC.rect_arms.bottom = gMC.rect_arms.top + 16;
	
	gMC.physics_normal = profileJson[PROFILE_PHYSICS].get<MYCHAR_PHYSICS>();
	gMC.physics_underwater = profileJson[PROFILE_PHYSICS_UNDERWATER].get<MYCHAR_PHYSICS>();
	gMC.no_splash_or_air_limit_underwater = profileJson[PROFILE_NO_WATER].get<int>();
	gMC.characterData = availableCharacters[profileJson[PROFILE_CHARACTER].get<std::string>()];

	// Reset stuff
	ClearFade();
	SetFrameMyChar();
	SetFrameTargetMyChar(16);
	InitBossLife();
	CutNoise();
	InitStar();
	ClearValueView();
	gCurlyShoot_wait = 0;
	WriteGameLog("Profile loaded in ", since(timing).count(), " ms");

	return true;
}

void InitMapSize(){

}

bool InitializeGame(void)
{
	InitMyChar();
	gSelectedArms = 0;
	gSelectedItem = 0;
	gCounter = 0;
	ClearArmsData();
	ClearItemData();
	ClearPermitStage();
	StartMapping();
	InitFlags();
	gCooldowns.clear();
	if (!TransferStage(13, 200, 10, 8))
	{
    if (gJapanese)
		Backend_ShowMessageBox("エラー", "ステージの読み込みに失敗");
	else
		Backend_ShowMessageBox("Error", "Failed to load stage");

		return false;
	}

	ClearFade();
	SetFrameMyChar();
	SetFrameTargetMyChar(16);
	InitBossLife();
	CutNoise();
	ClearValueView();
	gCurlyShoot_wait = 0;
	SetFadeMask();
	SetFrameTargetMyChar(16);
	return true;
}
