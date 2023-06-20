#ifndef __TAGS_H__
#define __TAGS_H__


#include <string>
#include <vector>
#include <map>
#include "Json.h"
#include "ResourceCode.h"

template<typename T = std::string>
std::vector<T> ParseTag(const std::string& path){
    std::vector<T> output;
    json tagInfo = ReadJSON(path, mode::normal);
    output = tagInfo["values"].get<std::vector<T>>();
    return output;
}

template<typename T = std::string, typename V = std::string>
std::map<T, V> ParseMapTag(const std::string& path){
    std::map<T, V> output;
    json tagInfo = ReadJSON(path, mode::normal);
    output = tagInfo.get<std::map<T, V>>();
    return output;
}
#endif // __TAGS_H__