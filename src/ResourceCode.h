#ifndef __RESOURCECODE_H__
#define __RESOURCECODE_H__


#include <filesystem>
#include <string>
#include <string_view>
#include <vector>
#include <cstdio>
#include <cstdlib>

namespace fs = std::filesystem;

struct ResourceCode {
    std::string name;
    fs::path path;
    std::string description;
    bool jp;
    std::string titleOverride;
    int spriteScale;
    int generatedStage;
};

extern std::vector<ResourceCode> availableResources;
extern std::string gModulePath;
extern std::string gDataPath;
extern std::string gScorePath;
extern bool gJapanese;
extern std::string gWindowTitle;
extern int gSpriteScale;
extern int gGeneratedStage;

void InitResources(char *argv[]);
FILE* GetResource(std::string_view relativePath);
bool ResourceExists(std::string_view relativePath);
std::string GetResourcePath(std::string_view relativePath);
std::string GetResourcePath(int id, std::string_view relativePath);
#endif // __RESOURCECODE_H__