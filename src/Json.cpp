#include "Generic.h"
#include "Json.h"
#include "Log.h"
#include <fstream>
#include "cpp/gstream.hpp"
#include "Backends/Misc.h"
#include "ResourceCode.h"

json ReadJSON(const std::string &path){
    return ReadJSON(path, mode::encrypted);
}

void UpdateJSON(json& j, const json& overrideJ, int resourceID){
    if (overrideJ.contains("override")){
        json overrideContent = overrideJ["override"];
        for (auto &[name, data]: overrideContent.items()){
            if (j.contains(name))
                j[name] = data;
        }
    } else {
        if (availableResources[resourceID].name != "Default")
            j = overrideJ;
    }
}

json ReadJSON(const std::string &path, mode m){
    json j;
    std::ifstream i;
    igzstream i2;
    try
    {
        switch (m)
        {
        case mode::normal:
            i = std::ifstream(GetResourcePath(0, path));
            i >> j;
            for (int rID : GetNumbers(1, (int)(availableResources.size()))){
                std::ifstream overrideI = std::ifstream(GetResourcePath(rID, path));
                json overrideJ;
                overrideI >> overrideJ;
                UpdateJSON(j, overrideJ, rID);
            }
            break;
        case mode::encrypted:
            i2.open(GetResourcePath(0, path).c_str());
            i2 >> j;
            for (int rID : GetNumbers(1, (int)(availableResources.size()))){
                igzstream overrideI = igzstream(GetResourcePath(rID, path).c_str());
                json overrideJ;
                overrideI >> overrideJ;
                UpdateJSON(j, overrideJ, rID);
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
    } catch (json::exception& e){
        WriteGameLog("JSON error has been encountered: ", e.what());
        Backend_ShowMessageBox("JSON error has been encountered!", e.what());
        return json();
    }
}

void WriteJSON(const std::string &path, const json &j, int spaces){
    WriteJSON(path, j, mode::encrypted);
}

void WriteJSON(const std::string &path, const json &j, mode m, int spaces){
    std::string js = j.dump(spaces);
    if (m == mode::encrypted){
        ogzstream o(path.c_str());
        o << js << std::flush;
    } else if (m == mode::normal){
        std::ofstream o(path.c_str());
        o << std::setw(4) << js << std::flush;
    }
}