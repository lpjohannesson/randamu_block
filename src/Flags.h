#ifndef __FLAGS_H__
#define __FLAGS_H__


#include <any>
#include <set>
#include "CommonDefines.h"
#include "Generic.h"
#include "Main.h"
#include "MyChar.h"
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

using FlagSetCallback = std::function<void(int)>;
extern std::vector<FlagSetCallback> flagSetCallbacks;
using FlagUnsetCallback = std::function<void(int)>;
extern std::vector<FlagUnsetCallback> flagUnsetCallbacks;

namespace rnddetail {
	template<typename ArgType>
	const ArgType& get(const ArgType val){
		static std::set<ArgType> defaults;
		auto result = defaults.insert(val);
		return *result.first;
	}
}

template <IsGenMetadata ArgType>
inline const ArgType GetFlagTraitData(std::string trait, std::string parameter, const ArgType defaultValue = ArgType{}){
	if (flagTraits.contains(trait)){
		for (const auto &[flag, info]: flagTraits[trait]){
			if (info.contains(parameter)){
				if (GetNPCFlag(flag))
					return GetMetadata<ArgType>(info.at(parameter));
			}
		}
	}
	return rnddetail::get<ArgType>(defaultValue);
}

std::map<int, GenerationArguments>& GetFlagTraitData(std::string trait);

inline auto GetTraitActiveFlags(std::string trait){
    auto mapView = MapIterate<int, GenerationArguments>(GetFlagTraitData(trait), [](int flag, GenerationArguments value)->bool{
		return GetNPCFlag(flag);
	});
    return mapView | std::views::keys;
}

template<IsGenMetadata ArgType>
inline auto GetActualFlagTraitData(std::string trait, std::string parameter){
	std::map<int, GenerationArguments> mapView = MapIterate<int, GenerationArguments>(GetFlagTraitData(trait), [parameter](int flag, GenerationArguments value)->bool{
		return GetNPCFlag(flag) && value.contains(parameter);
	});
	std::map<int, ArgType> flagDataMap;
	for (auto&[key, value]: mapView){
		flagDataMap.insert(std::pair{key, GetMetadata<ArgType>(value[parameter])});
	}
    return flagDataMap;
}
#endif // __FLAGS_H__