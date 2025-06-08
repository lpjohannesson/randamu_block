#include "ResourceCode.h"

#include <algorithm>
#include <filesystem>
#include <cstddef>
#include "Log.h"
#include "Backends/Misc.h"
#include "Json.h"
#include "Main.h"

std::vector<ResourceCode> availableResources;
std::string gModulePath;
std::string gDataPath;
std::string gScorePath;
bool gJapanese;
std::string gWindowTitle;
std::string initModulePath;
int gSpriteScale;
int gGeneratedStage;
std::pair<int, int> gFontData;

void InitResources(char *argv[]){
	if (initModulePath.empty()){
		initModulePath = std::string(argv[0]);
	}
	availableResources.clear();
	gModulePath.clear();
	gDataPath.clear();
	gScorePath.clear();
	gJapanese = false;
	auto timing = std::chrono::steady_clock::now();
    // Get executable's path
	if (!Backend_GetBasePath(&gModulePath))
	{
		// Fall back on argv[0] if the backend cannot provide a path
		gModulePath = initModulePath;

		for (size_t i = gModulePath.length();; --i)
		{
			if (i == 0 || gModulePath[i] == '\\' || gModulePath[i] == '/')
			{
				gModulePath.resize(i);
				break;
			}
		}
	}

	// Get path of the data folder
	gDataPath = gModulePath + "/data";
	gScorePath = gModulePath + "/score";

	fs::path resourcesPath = gModulePath;
	resourcesPath = resourcesPath / "resources";
	if (!fs::exists(resourcesPath)){
		fs::create_directories(resourcesPath);
	}

    availableResources.push_back({"Default", gDataPath, "", false, "Doukutsu Randamu", 1, 13, {14, 14}});

	json resourcesList;
	if (!fs::exists(resourcesPath / "list.json")){
		resourcesList = json::object({{"enabled", json::array()}});
		WriteJSON((resourcesPath / "list.json").string(), resourcesList, mode::normal, 4);
	} else {
		resourcesList = ReadJSON((resourcesPath / "list.json").string(), mode::normalGlobal);
		auto enabledList = resourcesList["enabled"].get<std::vector<std::string>>();
		fs::recursive_directory_iterator begin(resourcesPath);
        fs::recursive_directory_iterator end;
		std::vector<fs::path> subdirs;
        std::copy_if(begin, end, std::back_inserter(subdirs), [enabledList](const fs::path& path) {
			std::string resFolder = path.filename().string();
			resFolder.erase(std::remove(resFolder.begin(), resFolder.end(), '"'), resFolder.end());
            return fs::is_directory(path) && std::find(enabledList.begin(), enabledList.end(), resFolder) != enabledList.end();
        });
		for (auto dir : subdirs){
			fs::path infoJsonPath = dir / "info.json";
			if (fs::exists(infoJsonPath)){
				std::string resFolder = dir.filename().string();
				resFolder.erase(std::remove(resFolder.begin(), resFolder.end(), '"'), resFolder.end());
				json infoJson = ReadJSON(infoJsonPath.string(), mode::normalGlobal);
				ResourceCode resource = {
					infoJson.value("name", resFolder),
					dir,
					infoJson.value("description", ""),
					infoJson.value("isJapanese", false),
					infoJson.value("windowTitle", "Doukutsu Randamu"),
					infoJson.value("resolution", 1),
					infoJson.value("generatedStageID", 13),
					{infoJson.value<int>("fontWidth", 14), infoJson.value<int>("fontHeight", 14)}
				};
				availableResources.push_back(resource);
			}
		}
		std::reverse(availableResources.begin(), availableResources.end());
	}
	WriteGameLog("Resource overloads loaded in ", since(timing).count(), " ms.");
	WriteGameLog("Available resources: ");
	for (auto resource : availableResources){
		WriteGameLog(resource.name, ": ", resource.description, " (", resource.path.string(), ")");
	}
	gJapanese = availableResources[0].jp;
	gWindowTitle = availableResources[0].titleOverride;
	gSpriteScale = availableResources[0].spriteScale;
	gGeneratedStage = availableResources[0].generatedStage;
	gFontData = availableResources[0].fontData;

	ReloadConfig();
}

std::string GetResourcePath(std::string_view relativePath){
	for (auto resource : availableResources){
		auto resourcePath = resource.path / relativePath;
		if (fs::exists(resourcePath)){
			return resourcePath.string();
		}
	}
	return gDataPath + "/" + std::string(relativePath);
}

std::string GetResourcePath(int id, std::string_view relativePath){
	auto resourcePath = availableResources[id].path / relativePath;
	if (fs::exists(resourcePath)){
		return resourcePath.string();
	}
	return gDataPath + "/" + std::string(relativePath);
}

FILE* GetResource(std::string_view relativePath){
	auto resourcePath = GetResourcePath(relativePath);
	FILE *file = fopen(resourcePath.c_str(), "rb");
	return file;
}

bool ResourceExists(std::string_view relativePath){
	return fs::exists(GetResourcePath(relativePath));
}