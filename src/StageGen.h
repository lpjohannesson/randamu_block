#ifndef __STAGEGEN_H__
#define __STAGEGEN_H__

#include <functional>
#include <map>
#include <string>
#include <variant>

struct GenerationContext;

using GenerationMetadata = std::variant<int, float, bool, std::string, std::vector<int>, std::vector<float>, std::vector<std::string>>;
using GenerationArguments = std::map<std::string, GenerationMetadata>;
using GenerationFunction = std::function<void(GenerationContext&, GenerationArguments&)>;
#endif // __STAGEGEN_H__