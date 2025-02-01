#pragma once

#include <concepts>
#include <set>
#include <string_view>
#include "Generic.h"
#include "Json.h"
#include "StageGen.h"

using TraitStorage = std::map<std::string, std::map<int, GenerationArguments>>;

namespace trait_detail {
    template<typename ArgType>
    std::map<TraitStorage*, std::set<ArgType>> defaults;

	template<typename ArgType>
	const ArgType& get(TraitStorage* storage, const ArgType val){
		auto result = defaults<ArgType>[storage].insert(val);
		return *result.first;
	}
}

template <std::predicate<int> Pred>
bool CheckGenericTrait(TraitStorage& storage, std::string_view trait, Pred&& pred){
    if (storage.contains(trait.data())){
		for (const auto &[flag, info]: storage[trait.data()]){
			if (pred(flag))
				return true;
		}
	}
	return false;
}

template <IsGenMetadata ArgType, std::predicate<int> Pred>
inline const ArgType GetGenericTraitData(TraitStorage* storage, std::string_view trait, std::string_view parameter, Pred&& pred, const ArgType defaultValue = ArgType{}){
	if (storage->contains(trait.data())){
		for (const auto &[flag, info]: storage->at(trait.data())){
			if (info.contains(parameter.data())){
				if (pred(flag))
					return GetMetadata<ArgType>(info.at(parameter.data()));
			}
		}
	}
	return trait_detail::get<ArgType>(storage, defaultValue);
}

std::map<int, GenerationArguments>& GetGenericTraitData(TraitStorage& storage, std::string_view trait);

template <std::predicate<int> Pred>
inline auto GetTraitActiveEntities(TraitStorage& storage, std::string_view trait, Pred&& pred){
    auto mapView = MapIterate<int, GenerationArguments>(GetGenericTraitData(storage, trait), [pred](int flag, GenerationArguments value)->bool{
		return pred(flag);
	});
	auto keyView = mapView | std::views::keys;
    return std::vector<int>{keyView.begin(), keyView.end()};
}

template<IsGenMetadata ArgType, std::predicate<int, GenerationArguments> Pred>
inline auto GetActualGenericTraitData(TraitStorage& storage, std::string_view trait, std::string_view parameter, Pred&& pred){
	std::map<int, GenerationArguments> mapView = MapIterate<int, GenerationArguments>(GetGenericTraitData(storage, trait), [parameter, pred](int flag, GenerationArguments value)->bool{
		return pred(flag, value);
	});
	std::map<int, ArgType> flagDataMap;
	for (auto&[key, value]: mapView){
		flagDataMap.insert(std::pair{key, GetMetadata<ArgType>(value[parameter.data()])});
	}
    return flagDataMap;
}

void from_json(const json &j, GenerationMetadata &gm);

void LoadTraits(std::string_view path, TraitStorage& storage);