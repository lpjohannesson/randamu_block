#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>

#include "ArmsItem.h"
#include "File.h"
#include "Flags.h"
#include "Generic.h"
#include "Game.h"
#include "Json.h"
#include "Log.h"
#include "Main.h"
#include "MyChar.h"
#include "RankingsWindow.h"
#include "ResourceCode.h"
#include "Score.h"
#include "TextScr.h"

#define GET_FLAG(x, i) ((x)[(i) / 8] & (1 << ((i) % 8)))

unsigned int playerScore;
unsigned int playerHighScore;

json gRankings;

void InitScore(void)
{
	playerScore = 0;
}

const char* const gDefaultScoreName = "highscore";
const char* const gDefaultScoreExt = ".dat";
const char* const gScoreCode = "modfest28";

bool SaveScore(unsigned long seed)
{
	FILE* fp;
	SCOREDATA scoredata;
	const char* FLAG = "FLAG";

	std::string path;

	// Get path
	if (seed != 0)
		path = gScorePath + "/" + std::to_string(seed) + gDefaultScoreExt;
	else
		path = gModulePath + '/' + gDefaultScoreName + gDefaultScoreExt;

	// Open file
	fp = fopen(path.c_str(), "wb");
	if (fp == NULL)
		return false;

	// Set up profile
	memset(&scoredata, 0, sizeof(SCOREDATA));
	memcpy(scoredata.code, gScoreCode, sizeof(scoredata.code));
	memcpy(scoredata.FLAG, FLAG, sizeof(scoredata.FLAG));
	scoredata.highScoreData = playerHighScore;

	// Write to file
	fwrite(scoredata.code, 8, 1, fp);
	File_WriteLE32(scoredata.highScoreData, fp);

	fclose(fp);

	return true;
}

void LoadRankings(){
	auto timing = std::chrono::steady_clock::now();
	if (!IsKeyFile("rankings.json")){
		WriteGameLog("Rankings file doesn't exist, recreating it.");
		WriteJSON(gModulePath + "/rankings.json", json::array());
	}

	gRankings = ReadJSON(gModulePath + "/rankings.json", mode::encryptedGlobal);
	WriteGameLog("Rankings loaded in ", since(timing).count(), " ms.");
}

void SaveRankings(){
	auto time = std::chrono::steady_clock::now();
	json rank;
	rank["version"] = 3;
	rank["score"] = gMC.score;
	rank["difficulty"] = gMC.difficulty;
	rank["life"] = gMC.life <= 0 ? 0 : gMC.life;
	rank["max_life"] = gMC.max_life;
	rank["character"] = gMC.characterData.codeName;

	rank["flags"] = json::array();
	for (int i = 0; i < 8000; i++){
		if (GetNPCFlag(i)) rank["flags"].push_back(i);
	}

	rank["depth"] = gMC.depth;
	rank["items"] = gItemData;
	rank["time"] = gCounter;
	rank["arms"] = gArmsData;
	rank["tileset"] = gMC.tileset;
	rank["seed"] = gMC.seed;
	rank["fast_run"] = ChallengeEnabled("fast_run");
	rank["second_camp_start"] = ChallengeEnabled("secondCampaign");
	rank["death"] = gMC.deathReason;

	int rankScore = rank["score"];

	auto preceedingRank = std::find_if(gRankings.begin(), gRankings.end(), [&rankScore](json a){
		return a["score"] < rankScore;
	});
	gRankings.insert(preceedingRank, rank);
	selectedRank = std::distance(gRankings.begin(), std::find(gRankings.begin(), gRankings.end(), rank));
	WriteGameLog("Ranking updated in ", since(time).count(), " ms.");
}

void SaveRankingsFile(){
	WriteJSON(gModulePath + "/rankings.json", gRankings);
}

std::string ScoreFilePath(unsigned long seed){
	return gScorePath + "/" + std::to_string(seed) + gDefaultScoreExt;
}

bool LoadScore(unsigned long seed)
{
	FILE* fp;
	SCOREDATA scoredata;
	std::string path;

	// Get path
	if (seed != 0)
		path = ScoreFilePath(seed);
	else
		path = gModulePath + '/' + gDefaultScoreName + gDefaultScoreExt;

	// Open file
	fp = fopen(path.c_str(), "rb");
	if (fp == NULL)
		return false;

	// Check header code
	fread(scoredata.code, 8, 1, fp);
	if (memcmp(scoredata.code, gScoreCode, 8) != 0)
	{
#ifdef FIX_BUGS
		fclose(fp);	// The original game forgets to close the file
#endif
		return false;
	}

	// Read data
	fseek(fp, 0, SEEK_SET);
	memset(&scoredata, 0, sizeof(SCOREDATA));
	fread(scoredata.code, 8, 1, fp);
	scoredata.highScoreData = File_ReadLE32(fp);

	fclose(fp);

	// Set things
	playerHighScore = scoredata.highScoreData;

	return true;
}