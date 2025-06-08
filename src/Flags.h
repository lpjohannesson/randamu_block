#ifndef __FLAGS_H__
#define __FLAGS_H__


#include "Main.h"
#include "StageGen.h"
#include "Traits.h"

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

using FlagSetCallback = std::function<void(int)>;
extern std::vector<FlagSetCallback> flagSetCallbacks;
using FlagUnsetCallback = std::function<void(int)>;
extern std::vector<FlagUnsetCallback> flagUnsetCallbacks;

template <IsGenMetadata ArgType>
inline const ArgType GetFlagTraitData(std::string trait, std::string parameter, const ArgType defaultValue = ArgType{}){
	return GetGenericTraitData<ArgType>(&flagTraits, trait, parameter, [](int flag){return GetNPCFlag(flag);}, defaultValue);
}

std::map<int, GenerationArguments>& GetFlagTraitData(std::string trait);

inline auto GetTraitActiveFlags(std::string trait){
    return GetTraitActiveEntities(flagTraits, trait, [](int flag){return GetNPCFlag(flag);});
}

template<IsGenMetadata ArgType>
inline auto GetActualFlagTraitData(std::string trait, std::string parameter){
	return GetActualGenericTraitData<ArgType>(flagTraits, trait, parameter, [parameter](int flag, GenerationArguments value){
		return GetNPCFlag(flag) && value.contains(parameter);
	});
}
#endif // __FLAGS_H__