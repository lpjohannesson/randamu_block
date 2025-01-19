#ifndef __FLAGS_H__
#define __FLAGS_H__


#include <any>
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

template<typename T> struct get_detail { static inline T _empty; };

template <IsGenMetadata ArgType>
inline const ArgType& GetFlagTraitData(std::string trait, std::string parameter){
	if (flagTraits.contains(trait)){
		for (const auto &[flag, info]: flagTraits[trait]){
			if (info.contains(parameter)){
				if (flag < 0){
					if (gMC.equip & std::abs(flag))
						return std::get<ArgType>(info.at(parameter));
				} else if (GetNPCFlag(flag))
					return std::get<ArgType>(info.at(parameter));
			}
		}
	}
	return get_detail<ArgType>::_empty;
}

std::map<int, GenerationArguments>& GetFlagTraitData(std::string trait);

inline auto GetTraitActiveFlags(std::string trait){
    auto mapView = MapIterate<int, GenerationArguments>(GetFlagTraitData(trait), [](int flag, GenerationArguments value)->bool{
		return (flag < 0 ? (gMC.equip & std::abs(flag)) : GetNPCFlag(flag));
	});
    return mapView | std::views::keys;
}

template<IsGenMetadata ArgType>
inline auto GetActualFlagTraitData(std::string trait, std::string parameter){
	auto mapView = MapIterate<int, GenerationArguments>(GetFlagTraitData(trait), [parameter](int flag, GenerationArguments value)->bool{
		return (flag < 0 ? (gMC.equip & std::abs(flag)) : GetNPCFlag(flag)) && value.contains(parameter);
	});
    return std::views::zip(mapView | std::views::keys, mapView | std::views::values | std::views::values | std::views::transform([](auto&& arg)->ArgType{ return std::get<ArgType>(arg);}));
}
#endif // __FLAGS_H__