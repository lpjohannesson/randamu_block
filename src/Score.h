#ifndef __SCORE_H__
#define __SCORE_H__


#include "MycParam.h"
#include "Json.h"

extern unsigned int playerScore;
extern unsigned int playerHighScore;
extern unsigned int playerLives;
extern unsigned int playerTokens;

void InitScore(void);

struct SCOREDATA
{
	char code[8];
	char FLAG[4];
	unsigned int highScoreData;
};

extern const char* const gDefaultScoreName;
extern const char* const gDefaultScoreExt;
extern const char* const gScoreCode;
extern unsigned int gLevelID;

extern json gRankings;

std::string ScoreFilePath(uint64_t seed);
bool SaveScore(uint64_t seed);
bool LoadScore(uint64_t seed);
void SaveRankings();
void LoadRankings();
void SaveRankingsFile();
#endif // __SCORE_H__