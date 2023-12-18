#include "Main.h"

#include <iostream>
#include <map>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

#include "Backends/Misc.h"
#include "Bitmap.h"
#include "Config.h"
#include "Draw.h"
#ifdef EXTRA_SOUND_FORMATS
#include "ExtraSoundFormats.h"
#endif
#include "Game.h"
#include "Generic.h"
#include "Input.h"
#include "KeyControl.h"
#include "Log.h"
#include "MyChar.h"
#include "Organya.h"
#include "Random.h"
#include "Resource.h"
#include "ResourceCode.h"
#include "Score.h"
#include "Sound.h"
#include "Tags.h"
#include "Triangle.h"

bool bFullscreen;
json gJConfig = json::object();
CURLplusplus client;
bool aprilFools;
std::vector<std::string> difficultyNames;
std::map<std::string, std::string> characterNames;
std::map<std::string, std::string> stageNames;
std::vector<std::string> gimmickNames;
std::map<std::string, std::string> messageBank;
std::vector<std::string> stageSymbols;
std::vector<std::string> stageGenerationOrder;
std::vector<std::int32_t> bossNPCsList;
std::map<std::string, GenerationArguments> stageGenerationMetadata;
std::map<int, int> timeStopCoefficients;

CONFIG_BINDING bindings[BINDING_TOTAL];

static bool bActive = true;
static bool bFPS = false;

bool ChallengeEnabled(std::string option){
	return gJConfig.value(option, false);
}

bool ChallengeExists(std::string option){
	return gJConfig.find(option) != gJConfig.end();
}

int ChallengeValue(std::string option){
	return gJConfig.value(option, -1);
}

void ModifyWithChallengeValue(int *value, std::string option){
	int valueConfig = ChallengeValue(option);
	if (valueConfig != -1){
		*value = valueConfig;
	}
}

double ChallengeValueReal(std::string option){
	return gJConfig.value(option, -1);
}

void ModifyWithChallengeValueReal(double *value, std::string option){
	double valueConfig = ChallengeValueReal(option);
	if (valueConfig != -1){
		*value = valueConfig;
	}
}

std::string GetCSE2Message(const std::string& key){
	auto message = messageBank.find(key);
	if (message != messageBank.end()) return message->second;
	else return "!!!NO TEXT FOUND!!!";
}

// Framerate stuff
static unsigned long CountFramePerSecound(void)
{
	unsigned long current_tick;	// The original name for this variable is unknown
	static bool first = true;
	static unsigned long max_count;
	static unsigned long count;
	static unsigned long wait;

	if (first)
	{
		wait = Backend_GetTicks();
		first = false;
	}

	current_tick = Backend_GetTicks();
	++count;

	if (wait + 1000 <= current_tick)
	{
		wait += 1000;
		max_count = count;
		count = 0;
	}

	return max_count;
}

void PutFramePerSecound(void)
{
	if (bFPS)
		PutFramePerSecound(WINDOW_WIDTH - 40, 8);
}

void PutFramePerSecound(int x, int y)
{
		const unsigned long fps = CountFramePerSecound();
		PutNumber4(x, y, fps, false);
}

void from_json(const json &j, GenerationMetadata &gm)
{
    if (j.is_number_integer()){
		gm = j.get<int>();
	} else if (j.is_number_float()){
		gm = j.get<float>();
	} else if (j.is_boolean()){
		gm = j.get<bool>();
	} else if (j.is_string()){
		gm = j.get<std::string>();
	} else if (j.is_array()){
		auto first_value = j[0];
		if (first_value.is_number_integer())
			gm = j.get<std::vector<int>>();
		else if (first_value.is_number_float())
			gm = j.get<std::vector<float>>();
		else if (first_value.is_string())
			gm = j.get<std::vector<std::string>>();
	}
}

std::string GivePointerString(const void* p){
	std::stringstream lol;
	lol << p;
	return lol.str();
}

void ReloadConfig(){
	auto timing = std::chrono::steady_clock::now();
	if (!IsKeyFile("config.json")){
		WriteGameLog("Config doesn't exist, recreating it.");
		WriteJSON(gModulePath + "/config.json", json::object(), mode::normal);
	}

	gJConfig = ReadJSON(gModulePath + "/config.json", mode::normalGlobal);
	if (!gJConfig.is_object()){
		gJConfig = json::object();
		WriteGameLog("Loaded config is invalid, recreating it.");
	}
	gJConfig["charge_jump"] = gJConfig.value("charge_jump", true);
	gJConfig["advanced_arms"] = gJConfig.value("advanced_arms", true);
	gJConfig["pitch_randomizer"] = gJConfig.value("pitch_randomizer", false);
	gJConfig["legacyUI"] = gJConfig.value("legacyUI", false);
	gJConfig["secondCampaign"] = gJConfig.value("secondCampaign", false);
	gJConfig["oneDamage"] = gJConfig.value("oneDamage", false);
	gJConfig["superHotMode"] = gJConfig.value("superHotMode", false);
	gJConfig["randomSizeMode"] = gJConfig.value("randomSizeMode", false);

	difficultyNames = ParseTag<std::string>("Tags/difficultyNames.json");
	gimmickNames = ParseTag<std::string>("Tags/gimmickNames.json");
	bossNPCsList = ParseTag<int>("Tags/bossNPCs.json");
	stageGenerationOrder = ParseTag<std::string>("Tags/generationOrder.json");
	json stageMetadataProto = ReadJSON("Tags/generationMetadata.json");
	for (auto &[name, data]: stageMetadataProto.items()){
		GenerationArguments metadataMap;
		for (auto &[n, d]: data.items()){
			from_json(d, metadataMap[n]);
		}
		stageGenerationMetadata[name] = metadataMap;
	}
	characterNames = ParseMapTag("Tags/characterNames.json");
	stageNames = ParseMapTag("Tags/stageNames.json");
	for (const auto &[key, _] : stageNames)
	{
		stageSymbols.push_back(key);
	}
	auto tempTSC = ParseMapTag<std::string, int>("Tags/timeStopCoefficients.json");
	for (const auto &[key, value]: tempTSC){
		timeStopCoefficients[std::stoi(key)] = value;
	}
	messageBank = ParseMapTag("Tags/messages.json");

	if (ChallengeEnabled("speedrunMetadata")){
		WriteGameLog("Writing out speedrun flag data.");
		json speedrunData = json::object();
		speedrunData["depth"] = GivePointerString(&gMC.depth);
		speedrunData["timer"] = GivePointerString(&time_count);
		speedrunData["gameMode"] = GivePointerString(&gGameMode);
		speedrunData["endFlag"] = GivePointerString(&gMC.grass);
		WriteJSON(gModulePath + "/speedrunData.json", speedrunData, mode::normal);
	}

	WriteGameLog("Config and tags reloaded in ", since<std::chrono::microseconds>(timing).count(), " us.");
}

// TODO - Inaccurate stack frame
int main(int argc, char *argv[])
{
	char buffer1[80];
	int v1, v2, v3, v4;
	GetCompileVersion(&v1, &v2, &v3, &v4);
		sprintf(buffer1, "Doukutsu Randamu v%d.%d.%d.%d%s", v1, v2, v3, v4, IsBeta() ? " (beta)" : "");
	msvc_srand(time(0));
	WriteGameLog("------------------------------------");
	WriteGameLog(buffer1, " is starting.");

	(void)argc;

	time_t now = time(0);
	tm *ltm = localtime(&now);

	if (ltm->tm_mon == 3 && ltm->tm_mday == 1)
		aprilFools = true;

	auto timing = std::chrono::steady_clock::now();

	if (!Backend_Init())
		return EXIT_FAILURE;

	WriteGameLog("Backend is initialized in ", since(timing).count(), " ms.");
	timing = std::chrono::steady_clock::now();

	InitResources(argv);
	CONFIG conf;
	if (!LoadConfigData(&conf))
		DefaultConfigData(&conf);

	memcpy(bindings, conf.bindings, sizeof(bindings));

	Rect unused_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	switch (conf.display_mode)
	{
		default:
			// Windowed

		#ifdef FIX_BUGS
			if (!StartDirectDraw(buffer1, conf.display_mode, conf.b60fps, conf.bSmoothScrolling, conf.bVsync, conf.bExtendedInfo))
			{
				Backend_Deinit();
				return EXIT_FAILURE;
			}
		#else
			// Doesn't handle StartDirectDraw failing
			StartDirectDraw(buffer1, conf.display_mode, conf.b60fps, conf.bSmoothScrolling, conf.bVsync, conf.bExtendedInfo);
		#endif

			break;

		case 0:
			// Fullscreen

		#ifdef FIX_BUGS
			if (!StartDirectDraw(buffer1, 0, conf.b60fps, conf.bSmoothScrolling, conf.bVsync, conf.bExtendedInfo))
			{
				Backend_Deinit();
				return EXIT_FAILURE;
			}
		#else
			// Doesn't handle StartDirectDraw failing
			StartDirectDraw(buffer1, 0, conf.b60fps, conf.bSmoothScrolling, conf.bVsync, conf.bExtendedInfo);
		#endif

			bFullscreen = true;

			// Backend_HideMouse();
			break;
	}

#ifdef DEBUG_SAVE
	PlaybackBackend_EnableDragAndDrop();
#endif

	// Set up window icon
#ifndef _WIN32	// On Windows, we use native icons instead (so we can give the taskbar and window separate icons, like the original EXE does)
	size_t window_icon_resource_size;
	const unsigned char *window_icon_resource_data = FindResource("ICON_MINI", "ICON", &window_icon_resource_size);

	if (window_icon_resource_data != NULL)
	{
		unsigned int window_icon_width, window_icon_height;
		unsigned char *window_icon_rgb_pixels = DecodeBitmap(window_icon_resource_data, window_icon_resource_size, &window_icon_width, &window_icon_height);

		if (window_icon_rgb_pixels != NULL)
		{
			Backend_SetWindowIcon(window_icon_rgb_pixels, window_icon_width, window_icon_height);
			FreeBitmap(window_icon_rgb_pixels);
		}
	}
#endif

	// Set up the cursor
	std::string cursor_path = "Resource/CURSOR/CURSOR_NORMAL.png";

	unsigned int cursor_width, cursor_height;
	unsigned char *cursor_rgba_pixels = DecodeBitmapWithAlphaFromFile(cursor_path.c_str(), &cursor_width, &cursor_height, false);

	if (cursor_rgba_pixels != NULL)
	{
		Backend_SetCursor(cursor_rgba_pixels, cursor_width, cursor_height);
		FreeBitmap(cursor_rgba_pixels);
	}

	if (IsKeyFile("fps"))
		bFPS = true;

	// Set rects
	Rect rcLoading = {0, 0, 64, 8};
	Rect rcFull = {0, 0, 0, 0};
	rcFull.right = WINDOW_WIDTH;
	rcFull.bottom = WINDOW_HEIGHT;

	// Load the "LOADING" text
	bool b = MakeSurface_File("Loading", SURFACE_ID_LOADING);

	// Draw loading screen
	CortBox(&rcFull, 0x000000);
	PutBitmap3(&rcFull, PixelToScreenCoord((WINDOW_WIDTH / 2) - 32), PixelToScreenCoord((WINDOW_HEIGHT / 2) - 4), &rcLoading, SURFACE_ID_LOADING);

	// Draw to screen
	if (!Flip_SystemTask())
	{
		Backend_Deinit();
		return EXIT_SUCCESS;
	}

	// Initialize sound
	InitDirectSound();

	// Initialize joystick
	InitDirectInput();

	// Initialize stuff
	InitTriangleTable();
		InitTextObject();

	WriteGameLog("Game is loaded in ", since(timing).count(), " ms.");

	// Run game code
	Game();

	// End stuff
	EndTextObject();
	EndDirectSound();
	EndDirectDraw();

	Backend_Deinit();

	return EXIT_SUCCESS;
}

void InactiveWindow(void)
{
	if (bActive)
	{
		bActive = false;
		StopOrganyaMusic();
#ifdef EXTRA_SOUND_FORMATS
		ExtraSound_Stop();
#endif
		SleepNoise();
	}

	PlaySoundObject(7, SOUND_MODE_STOP);
}

void ActiveWindow(void)
{
	if (!bActive)
	{
		bActive = true;
		StopOrganyaMusic();
		PlayOrganyaMusic();
#ifdef EXTRA_SOUND_FORMATS
		ExtraSound_Play();
#endif
		ResetNoise();
	}

	PlaySoundObject(7, SOUND_MODE_PLAY_LOOP);
}

void JoystickProc(void);

bool SystemTask(void)
{
	static bool previous_keyboard_state[BACKEND_KEYBOARD_TOTAL];

	do
	{
		if (!Backend_SystemTask(bActive)){
			SaveRankingsFile();
			return false;
		}
	} while(!bActive);

	Backend_GetKeyboardState(gKeyboardState);
	static bool mouse;

	Backend_GetMouseState(&gMouseX, &gMouseY, &gMouseHold, &mouse);
	Backend_GetMouseMotion(&gMouseVelX, &gMouseVelY);

	for (unsigned int i = 0; i < BACKEND_KEYBOARD_TOTAL; ++i)
	{
		if (gKeyboardState[i] && !previous_keyboard_state[i])
		{
			if (i == BACKEND_KEYBOARD_ESCAPE)
				gKey |= KEY_ESCAPE;

			if (i == BACKEND_KEYBOARD_F1)
				gKey |= KEY_F1;
			else if (i == BACKEND_KEYBOARD_F2)
				gKey |= KEY_F2;
			else if (i == BACKEND_KEYBOARD_F3)
				gKey |= KEY_F3;

			if (i == bindings[BINDING_MAP].keyboard)
				gKey |= KEY_MAP;
			if (i == bindings[BINDING_LEFT].keyboard)
				gKey |= KEY_LEFT;
			if (i == bindings[BINDING_RIGHT].keyboard)
				gKey |= KEY_RIGHT;
			if (i == bindings[BINDING_UP].keyboard)
				gKey |= KEY_UP;
			if (i == bindings[BINDING_DOWN].keyboard)
				gKey |= KEY_DOWN;
			if (i == bindings[BINDING_SHOT].keyboard)
				gKey |= KEY_SHOT;
			if (i == bindings[BINDING_JUMP].keyboard)
				gKey |= KEY_JUMP;
			if (i == bindings[BINDING_ARMS].keyboard)
				gKey |= KEY_ARMS;
			if (i == bindings[BINDING_ARMSREV].keyboard)
				gKey |= KEY_ARMSREV;
			if (i == bindings[BINDING_ITEM].keyboard)
				gKey |= KEY_ITEM;
			if (i == bindings[BINDING_CANCEL].keyboard)
				gKey |= KEY_CANCEL;
			if (i == bindings[BINDING_OK].keyboard)
				gKey |= KEY_OK;
			if (i == bindings[BINDING_PAUSE].keyboard)
				gKey |= KEY_PAUSE;
			if (i == bindings[BINDING_ABILITY].keyboard)
				gKey |= KEY_ABILITY;
			if (i == bindings[BINDING_DASH].keyboard)
				gKey |= KEY_DASH;
			if (i == bindings[BINDING_TURBO_TEXT].keyboard)
				gKey |= KEY_TURBO_TEXT;

			for (int l = BACKEND_KEYBOARD_1; l < BACKEND_KEYBOARD_9 + 1; l++){
				if (i == l){
					gNumberKey = l - 26;
				}
			}
			if (i == BACKEND_KEYBOARD_0){
				gNumberKey = 10;
			}
		}
		else if (!gKeyboardState[i] && previous_keyboard_state[i])
		{
			if (i == BACKEND_KEYBOARD_ESCAPE)
				gKey &= ~KEY_ESCAPE;

			if (i == BACKEND_KEYBOARD_F1)
				gKey &= ~KEY_F1;
			else if (i == BACKEND_KEYBOARD_F2)
				gKey &= ~KEY_F2;
			else if (i == BACKEND_KEYBOARD_F3)
				gKey &= ~KEY_F3;

			if (i == bindings[BINDING_MAP].keyboard)
				gKey &= ~KEY_MAP;
			if (i == bindings[BINDING_LEFT].keyboard)
				gKey &= ~KEY_LEFT;
			if (i == bindings[BINDING_RIGHT].keyboard)
				gKey &= ~KEY_RIGHT;
			if (i == bindings[BINDING_UP].keyboard)
				gKey &= ~KEY_UP;
			if (i == bindings[BINDING_DOWN].keyboard)
				gKey &= ~KEY_DOWN;
			if (i == bindings[BINDING_SHOT].keyboard)
				gKey &= ~KEY_SHOT;
			if (i == bindings[BINDING_JUMP].keyboard)
				gKey &= ~KEY_JUMP;
			if (i == bindings[BINDING_ARMS].keyboard)
				gKey &= ~KEY_ARMS;
			if (i == bindings[BINDING_ARMSREV].keyboard)
				gKey &= ~KEY_ARMSREV;
			if (i == bindings[BINDING_ITEM].keyboard)
				gKey &= ~KEY_ITEM;
			if (i == bindings[BINDING_CANCEL].keyboard)
				gKey &= ~KEY_CANCEL;
			if (i == bindings[BINDING_OK].keyboard)
				gKey &= ~KEY_OK;
			if (i == bindings[BINDING_PAUSE].keyboard)
				gKey &= ~KEY_PAUSE;
			if (i == bindings[BINDING_ABILITY].keyboard)
				gKey &= ~KEY_ABILITY;
			if (i == bindings[BINDING_DASH].keyboard)
				gKey &= ~KEY_DASH;
			if (i == bindings[BINDING_TURBO_TEXT].keyboard)
				gKey &= ~KEY_TURBO_TEXT;

			for (int l = BACKEND_KEYBOARD_1; l < BACKEND_KEYBOARD_9 + 1; l++){
				if (i == l){
					gNumberKey = -1;
				}
			}
			if (i == BACKEND_KEYBOARD_0){
				gNumberKey = -1;
			}
		}
	}

	if (mouse)
				gKey |= KEY_MOUSE;
			else
				gKey &= ~KEY_MOUSE;

	memcpy(previous_keyboard_state, gKeyboardState, sizeof(gKeyboardState));

	

	// Run joystick code
	JoystickProc();

	return true;
}

void JoystickProc(void)
{
	int i;
	static JOYSTICK_STATUS old_status;

	if (!GetJoystickStatus(&gJoystickState))
		memset(&gJoystickState, 0, sizeof(gJoystickState));

	// Set held buttons
	for (i = 0; i < sizeof(gJoystickState.bButton) / sizeof(gJoystickState.bButton[0]); ++i)
	{
		if (gJoystickState.bButton[i] && !old_status.bButton[i])
		{
			if (i == bindings[BINDING_MAP].controller)
				gKey |= KEY_MAP;
			if (i == bindings[BINDING_LEFT].controller)
				gKey |= KEY_LEFT;
			if (i == bindings[BINDING_RIGHT].controller)
				gKey |= KEY_RIGHT;
			if (i == bindings[BINDING_UP].controller)
				gKey |= KEY_UP;
			if (i == bindings[BINDING_DOWN].controller)
				gKey |= KEY_DOWN;
			if (i == bindings[BINDING_SHOT].controller)
				gKey |= KEY_SHOT;
			if (i == bindings[BINDING_JUMP].controller)
				gKey |= KEY_JUMP;
			if (i == bindings[BINDING_ARMS].controller)
				gKey |= KEY_ARMS;
			if (i == bindings[BINDING_ARMSREV].controller)
				gKey |= KEY_ARMSREV;
			if (i == bindings[BINDING_ITEM].controller)
				gKey |= KEY_ITEM;
			if (i == bindings[BINDING_CANCEL].controller)
				gKey |= KEY_CANCEL;
			if (i == bindings[BINDING_OK].controller)
				gKey |= KEY_OK;
			if (i == bindings[BINDING_PAUSE].controller)
				gKey |= KEY_PAUSE;
			if (i == bindings[BINDING_ABILITY].controller)
				gKey |= KEY_ABILITY;
			if (i == bindings[BINDING_DASH].controller)
				gKey |= KEY_DASH;
			if (i == bindings[BINDING_TURBO_TEXT].controller)
				gKey |= KEY_TURBO_TEXT;
		}
		else if (!gJoystickState.bButton[i] && old_status.bButton[i])
		{
			if (i == bindings[BINDING_MAP].controller)
				gKey &= ~KEY_MAP;
			if (i == bindings[BINDING_LEFT].controller)
				gKey &= ~KEY_LEFT;
			if (i == bindings[BINDING_RIGHT].controller)
				gKey &= ~KEY_RIGHT;
			if (i == bindings[BINDING_UP].controller)
				gKey &= ~KEY_UP;
			if (i == bindings[BINDING_DOWN].controller)
				gKey &= ~KEY_DOWN;
			if (i == bindings[BINDING_SHOT].controller)
				gKey &= ~KEY_SHOT;
			if (i == bindings[BINDING_JUMP].controller)
				gKey &= ~KEY_JUMP;
			if (i == bindings[BINDING_ARMS].controller)
				gKey &= ~KEY_ARMS;
			if (i == bindings[BINDING_ARMSREV].controller)
				gKey &= ~KEY_ARMSREV;
			if (i == bindings[BINDING_ITEM].controller)
				gKey &= ~KEY_ITEM;
			if (i == bindings[BINDING_CANCEL].controller)
				gKey &= ~KEY_CANCEL;
			if (i == bindings[BINDING_OK].controller)
				gKey &= ~KEY_OK;
			if (i == bindings[BINDING_PAUSE].controller)
				gKey &= ~KEY_PAUSE;
			if (i == bindings[BINDING_ABILITY].controller)
				gKey &= ~KEY_ABILITY;
			if (i == bindings[BINDING_DASH].controller)
				gKey &= ~KEY_DASH;
			if (i == bindings[BINDING_TURBO_TEXT].controller)
				gKey &= ~KEY_TURBO_TEXT;
		}
	}

	old_status = gJoystickState;
}
