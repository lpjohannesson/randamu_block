#ifndef __JSON_H__
#define __JSON_H__

#include <concepts>
#include <fstream>

#include "cpp/gstream.hpp"
#include "nlohmann/json.hpp"

#include "Generic.h"
#include "Log.h"
#include "Backends/Misc.h"
#include "ResourceCode.h"

using json = nlohmann::basic_json<>;
using ordered_json = nlohmann::ordered_json;

struct JsonPatch {
    std::string filePath;
    size_t priority = 0;
    std::string type = "patch";
    json patch;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(JsonPatch, filePath, priority, type, patch)
};

extern std::vector<std::pair<std::string, JsonPatch>> jsonPatches;

enum class mode {
    encrypted, normal,
    encryptedGlobal, normalGlobal
};

template<typename T>
concept IsJson = std::derived_from<T, json>;

template <IsJson JsonType = json>
void UpdateJSON(JsonType& j, const JsonPatch& patch){
    if (patch.type == "patch"){
        j = j.patch(patch.patch);
    } else if (patch.type == "merge"){
        j.merge_patch(patch.patch);
    }
}

template <IsJson JsonType = json>
JsonType ReadJSON(const std::string &path, mode m){
    JsonType j;
    std::ifstream i;
    igzstream i2;
    try
    {
        switch (m)
        {
        case mode::normal:
            i = std::ifstream(GetResourcePath(path));
            i >> j;
            for (auto& [fileName, patch] : jsonPatches | std::views::filter([path](const std::pair<std::string, JsonPatch>& p){
                return p.second.filePath == path;
            })){
                WriteGameLog("Applying ", fileName, " patch to ", path);
                UpdateJSON<JsonType>(j, patch);
            }
            break;
        case mode::encrypted:
            i2.open(GetResourcePath(path).c_str());
            i2 >> j;
            for (auto& [fileName, patch] : jsonPatches | std::views::filter([path](const std::pair<std::string, JsonPatch>& p){
                return p.second.filePath == path;
            })){
                WriteGameLog("Applying ", fileName, " patch to ", path);
                UpdateJSON<JsonType>(j, patch);
            }
            break;
        case mode::normalGlobal:
            i = std::ifstream(path.c_str());
            i >> j;
            break;
        case mode::encryptedGlobal:
            i2.open(path.c_str());
            i2 >> j;
            break;
        }
        return j;
    } catch (typename JsonType::exception& e){
        WriteGameLog("JSON error has been encountered: ", e.what());
        Backend_ShowMessageBox("JSON error has been encountered!", e.what());
        return JsonType();
    }
}
template <IsJson JsonType = json>
void WriteJSON(const std::string &path, const JsonType &j, mode m, int spaces = -1){
    std::string js = j.dump(spaces);
    if (m == mode::encrypted){
        ogzstream o(path.c_str());
        o << js << std::flush;
    } else if (m == mode::normal){
        std::ofstream o(path.c_str());
        o << std::setw(4) << js << std::flush;
    }
}

template <IsJson JsonType = json>
JsonType ReadJSON(const std::string &path){
    return ReadJSON<JsonType>(path, mode::encrypted);
}
template <IsJson JsonType = json>
void WriteJSON(const std::string &path, const JsonType &j, int spaces = -1){
    WriteJSON<JsonType>(path, j, mode::encrypted, spaces);
}
#endif // __JSON_H__