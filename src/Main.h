#ifndef __MAIN_H__
#define __MAIN_H__


#include <chrono>
#include <string>
#include <format>

#include "CommonDefines.h"
#include "Config.h"
#include "Input.h"
#include "Json.h"
#include "Request.hpp"
#include "StageGen.h"

extern json gJConfig;
extern std::map<std::string, std::string> messageBank;
extern std::vector<std::string> difficultyNames;
extern std::vector<std::string> gimmickNames;
extern std::map<std::string, std::string> characterNames;
extern std::map<std::string, std::string> stageNames;
extern std::vector<std::string> stageSymbols;
extern std::vector<std::string> stageGenerationOrder;
extern std::map<std::string, GenerationArguments> stageGenerationMetadata;

void from_json(const json& j, GenerationMetadata& gm);

extern bool bFullscreen;
extern bool gbUseJoystick;

extern int gJoystickButtonTable[8];

extern CURLplusplus client;

extern bool aprilFools;

extern CONFIG_BINDING bindings[BINDING_TOTAL];

bool ChallengeEnabled(std::string option);
bool ChallengeExists(std::string option);
int ChallengeValue(std::string option);
void ModifyWithChallengeValue(int *value, std::string option);
double ChallengeValueReal(std::string option);
void ModifyWithChallengeValueReal(double *value, std::string option);
std::string GetCSE2Message(const std::string& key);

template <class... Args>
std::string GetCSE2FormattedMessage(const std::string& key, Args&&... args){
    auto message = messageBank.find(key);
	if (message != messageBank.end()){
        std::string formatResult = std::vformat(message->second, std::make_format_args(args...));
        return formatResult;
    }
	else return "!!!NO TEXT FOUND!!!";
}

void ReloadConfig();

void PutFramePerSecound(void);
void PutFramePerSecound(int x, int y);

void InactiveWindow(void);
void ActiveWindow(void);

bool SystemTask(void);

template <
    class result_t   = std::chrono::milliseconds,
    class clock_t    = std::chrono::steady_clock,
    class duration_t = std::chrono::milliseconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}

#endif // __MAIN_H__