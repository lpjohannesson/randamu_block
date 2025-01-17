#ifndef __FLAGS_H__
#define __FLAGS_H__


#include "CommonDefines.h"
#include "StageGen.h"

extern unsigned char gFlagNPC[1000];
extern unsigned char gSkipFlag[8];

void InitFlags(void);
void InitSkipFlags(void);
void SetNPCFlag(long a);
void CutNPCFlag(long a);
bool GetNPCFlag(long a);
void SetSkipFlag(long a);
void CutSkipFlag(long a);
bool GetSkipFlag(long a);

bool CheckFlagTrait(std::string trait);

const GenerationMetadata& GetFlagTraitData(std::string trait, std::string parameter);
std::map<int, GenerationArguments>& GetFlagTraitData(std::string trait);

#endif // __FLAGS_H__