#include "Traits.h"

#include <vector>

void from_json(const json &j, GenerationMetadata &gm) {
    if (j.is_number()){
		gm = j.get<float>();
	} else if (j.is_boolean()){
		gm = static_cast<float>(j.get<bool>());
	} else if (j.is_string()){
		gm = j.get<std::string>();
	} else if (j.is_array()){
		auto first_value = j[0];
		if (first_value.is_number())
			gm = j.get<std::vector<float>>();
		else if (first_value.is_string())
			gm = j.get<std::vector<std::string>>();
	}
}

std::map<int, GenerationArguments> emptydata;

std::map<int, GenerationArguments>& GetGenericTraitData(TraitStorage& storage, std::string_view trait){
	if (storage.contains(trait.data())){
		return storage[trait.data()];
	}
	return emptydata;
}

void LoadTraits(std::string_view path, TraitStorage& storage){
    json protoTraits = ReadJSON(path.data());
	for (auto &[flag, traits]: protoTraits.items()){
		for (auto &[traitName, traitData]: traits.items()){
			storage.try_emplace(traitName, std::map<int, GenerationArguments>{});
			auto traitInfo = traitData;
			GenerationArguments metadataMap;
			for (auto &[n, d]: traitInfo.items()){
				from_json(d, metadataMap[n]);
			}
			storage[traitName].try_emplace(std::stoi(flag), metadataMap);
		}
	}
}