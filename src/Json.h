#ifndef __JSON_H__
#define __JSON_H__

#include "nlohmann/json.hpp"

using json = nlohmann::basic_json<>;

enum class mode {
    encrypted, normal,
    encryptedGlobal, normalGlobal
};

json ReadJSON(const std::string &path, mode m);
void WriteJSON(const std::string &path, const json &j, mode m, int spaces = -1);
json ReadJSON(const std::string &path);
void WriteJSON(const std::string &path, const json &j, int spaces = -1);
#endif // __JSON_H__