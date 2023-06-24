
#include "Pause.h"

#include <map>
#include <functional>
#include <iostream>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <regex>
#include <vector>

#include "Backends/Controller.h"
#include "Backends/Misc.h"
#include "Back.h"
#include "BossLife.h"
#include "CommonDefines.h"
#include "Config.h"
#include "Draw.h"
#include "Generic.h"
#include "GenericLoad.h"
#include "Escape.h"
#include "Game.h"
#include "Fade.h"
#include "KeyControl.h"
#include "Log.h"
#include "Main.h"
#include "MyChar.h"
#include "Organya.h"
#include "Profile.h"
#include "ResourceCode.h"
#include "Sound.h"
#include "Score.h"
#include "Stage.h"
#include "TextScr.h"

#define MAX_OPTIONS ((WINDOW_HEIGHT / 18) - 3)	// The maximum number of options we can fit on-screen at once

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

enum
{
	CALLBACK_CONTINUE = -1,
	CALLBACK_PREVIOUS_MENU = -2,
	CALLBACK_RESET = -3,
	CALLBACK_EXIT = -4,
	CALLBACK_RANKS = -5
};

typedef enum CallbackAction
{
	ACTION_INIT,
	ACTION_DEINIT,
	ACTION_UPDATE,
	ACTION_OK,
	ACTION_LEFT,
	ACTION_RIGHT
} CallbackAction;

struct OptionNames {
	std::string normal_name;
	std::string alternative_name;
};

struct Option;

struct OptionsMenu
{
	std::string title;
	std::string subtitle;
	struct Option *options;
	size_t total_options;
	int x_offset;
	bool submenu;
};

using OptionCallback = std::function<int(OptionsMenu*,size_t,CallbackAction)>;

struct CallbackVariants {
	OptionCallback main_callback;
	OptionCallback alt_callback;
};

struct Option
{
	OptionNames name;
	CallbackVariants callback;
	void *user_data;
	std::string value_string;
	long value;
	bool disabled;
};

static bool restart_required;
json gConfigCopy;
int mode = 0;
char characterOrder[] = {0, 1, 2, 3, 19, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};

static std::string GetKeyName(int key)
{
	switch (key)
	{
		case BACKEND_KEYBOARD_A:
			return "A";

		case BACKEND_KEYBOARD_B:
			return "B";

		case BACKEND_KEYBOARD_C:
			return "C";

		case BACKEND_KEYBOARD_D:
			return "D";

		case BACKEND_KEYBOARD_E:
			return "E";

		case BACKEND_KEYBOARD_F:
			return "F";

		case BACKEND_KEYBOARD_G:
			return "G";

		case BACKEND_KEYBOARD_H:
			return "H";

		case BACKEND_KEYBOARD_I:
			return "I";

		case BACKEND_KEYBOARD_J:
			return "J";

		case BACKEND_KEYBOARD_K:
			return "K";

		case BACKEND_KEYBOARD_L:
			return "L";

		case BACKEND_KEYBOARD_M:
			return "M";

		case BACKEND_KEYBOARD_N:
			return "N";

		case BACKEND_KEYBOARD_O:
			return "O";

		case BACKEND_KEYBOARD_P:
			return "P";

		case BACKEND_KEYBOARD_Q:
			return "Q";

		case BACKEND_KEYBOARD_R:
			return "R";

		case BACKEND_KEYBOARD_S:
			return "S";

		case BACKEND_KEYBOARD_T:
			return "T";

		case BACKEND_KEYBOARD_U:
			return "U";

		case BACKEND_KEYBOARD_V:
			return "V";

		case BACKEND_KEYBOARD_W:
			return "W";

		case BACKEND_KEYBOARD_X:
			return "X";

		case BACKEND_KEYBOARD_Y:
			return "Y";

		case BACKEND_KEYBOARD_Z:
			return "Z";

		case BACKEND_KEYBOARD_0:
			return "0";

		case BACKEND_KEYBOARD_1:
			return "1";

		case BACKEND_KEYBOARD_2:
			return "2";

		case BACKEND_KEYBOARD_3:
			return "3";

		case BACKEND_KEYBOARD_4:
			return "4";

		case BACKEND_KEYBOARD_5:
			return "5";

		case BACKEND_KEYBOARD_6:
			return "6";

		case BACKEND_KEYBOARD_7:
			return "7";

		case BACKEND_KEYBOARD_8:
			return "8";

		case BACKEND_KEYBOARD_9:
			return "9";

		case BACKEND_KEYBOARD_F1:
			return "F1";

		case BACKEND_KEYBOARD_F2:
			return "F2";

		case BACKEND_KEYBOARD_F3:
			return "F3";

		case BACKEND_KEYBOARD_F4:
			return "F4";

		case BACKEND_KEYBOARD_F5:
			return "F5";

		case BACKEND_KEYBOARD_F6:
			return "F6";

		case BACKEND_KEYBOARD_F7:
			return "F7";

		case BACKEND_KEYBOARD_F8:
			return "F8";

		case BACKEND_KEYBOARD_F9:
			return "F9";

		case BACKEND_KEYBOARD_F10:
			return "F10";

		case BACKEND_KEYBOARD_F11:
			return "F11";

		case BACKEND_KEYBOARD_F12:
			return "F12";

		case BACKEND_KEYBOARD_UP:
			return "Up";

		case BACKEND_KEYBOARD_DOWN:
			return "Down";

		case BACKEND_KEYBOARD_LEFT:
			return "Left";

		case BACKEND_KEYBOARD_RIGHT:
			return "Right";

		case BACKEND_KEYBOARD_ESCAPE:
			return "Escape";

		case BACKEND_KEYBOARD_BACK_QUOTE:
			return "`";

		case BACKEND_KEYBOARD_TAB:
			return "Tab";

		case BACKEND_KEYBOARD_CAPS_LOCK:
			return "Caps Lock";

		case BACKEND_KEYBOARD_LEFT_SHIFT:
			return "Left Shift";

		case BACKEND_KEYBOARD_LEFT_CTRL:
			return "Left Ctrl";

		case BACKEND_KEYBOARD_LEFT_ALT:
			return "Left Alt";

		case BACKEND_KEYBOARD_SPACE:
			return "Space";

		case BACKEND_KEYBOARD_RIGHT_ALT:
			return "Right Alt";

		case BACKEND_KEYBOARD_RIGHT_CTRL:
			return "Right Ctrl";

		case BACKEND_KEYBOARD_RIGHT_SHIFT:
			return "Right Shift";

		case BACKEND_KEYBOARD_ENTER:
			return "Enter";

		case BACKEND_KEYBOARD_BACKSPACE:
			return "Backspace";

		case BACKEND_KEYBOARD_MINUS:
			return "-";

		case BACKEND_KEYBOARD_EQUALS:
			return "=";

		case BACKEND_KEYBOARD_LEFT_BRACKET:
			return "[";

		case BACKEND_KEYBOARD_RIGHT_BRACKET:
			return "]";

		case BACKEND_KEYBOARD_BACK_SLASH:
			return "\\";

		case BACKEND_KEYBOARD_SEMICOLON:
			return ";";

		case BACKEND_KEYBOARD_APOSTROPHE:
			return "'";

		case BACKEND_KEYBOARD_COMMA:
			return ",";

		case BACKEND_KEYBOARD_PERIOD:
			return ".";

		case BACKEND_KEYBOARD_FORWARD_SLASH:
			return "/";
	}

	return GetCSE2Message("key_unknown");
}


struct Control
{
	std::string name;
	size_t binding_index;
	unsigned char groups;
};


// The bitfield on the right determines which 'group' the
// control belongs to - if two controls are in the same group,
// they cannot be bound to the same key.
static Control controls[] = {
	{"Up",              BINDING_UP,     (1 << 0) | (1 << 1)},
	{"Down",            BINDING_DOWN,   (1 << 0) | (1 << 1)},
	{"Left",            BINDING_LEFT,   (1 << 0) | (1 << 1)},
	{"Right",           BINDING_RIGHT,  (1 << 0) | (1 << 1)},
	{"OK",              BINDING_OK,      1 << 1},
	{"Cancel",          BINDING_CANCEL,  1 << 1},
	{"Jump",            BINDING_JUMP,    1 << 0},
	{"Shoot",           BINDING_SHOT,    1 << 0},
	{"Next Weapon",     BINDING_ARMS,    1 << 0},
	{"Previous Weapon", BINDING_ARMSREV, 1 << 0},
	{"Inventory",       BINDING_ITEM,    1 << 0},
	{"Map",             BINDING_MAP,     1 << 0},
	{"Pause",           BINDING_PAUSE,   1 << 0},
	{"Ability",         BINDING_ABILITY, 1 << 0},
	{"Dash",            BINDING_DASH,    1 << 0}
};

static int EnterOptionsMenu(OptionsMenu *options_menu, size_t selected_option)
{
	int scroll = 0;

	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	Rect rcMyChar[4] = {
	{0, 16, 16, 32},
	{16, 16, 32, 32},
	{0, 16, 16, 32},
	{32, 16, 48, 32},
	};
	if (gMIMCurrentNum > 0)
	for (int i = 0; i < 4; i++){
		rcMyChar[i].top += 32 * gMIMCurrentNum;
		rcMyChar[i].bottom += 32 * gMIMCurrentNum;
	}

	unsigned int anime = 0;

	int return_value;

	// Initialise options
	for (size_t i = 0; i < options_menu->total_options; ++i)
		options_menu->options[i].callback.main_callback(options_menu, i, ACTION_INIT);

	for (;;)
	{
		// Get pressed keys
		GetTrg();

		// Allow unpausing by pressing the pause button only when in the main pause menu (not submenus)
		if (!options_menu->submenu && gKeyTrg & KEY_PAUSE)
		{
			return_value = CALLBACK_CONTINUE;
			break;
		}

		// Go back one submenu when the 'cancel' button is pressed
		if (gKeyTrg & gKeyCancel)
		{
			return_value = CALLBACK_CONTINUE;
			break;
		}

		// Handling up/down input
		if (gKeyTrg & (gKeyUp | gKeyDown))
		{
			const size_t old_selection = selected_option;

			if (gKeyTrg & gKeyDown)
				if (selected_option++ == options_menu->total_options - 1)
					selected_option = 0;

			if (gKeyTrg & gKeyUp)
				if (selected_option-- == 0)
					selected_option = options_menu->total_options - 1;

			// Update the menu-scrolling, if there are more options than can be fit on the screen
			if (selected_option < old_selection)
				scroll = MAX(0, MIN(scroll, (int)selected_option - 1));

			if (selected_option > old_selection)
				scroll = MIN(MAX(0, (int)options_menu->total_options - MAX_OPTIONS), MAX(scroll, (int)selected_option - (MAX_OPTIONS - 2)));

			PlaySoundObject(1, SOUND_MODE_PLAY);
		}

		// Run option callbacks
		for (size_t i = 0; i < options_menu->total_options; ++i)
		{
			if (!options_menu->options[i].disabled)
			{
				CallbackAction action = ACTION_UPDATE;

				if (i == selected_option)
				{
					if (gKeyTrg & gKeyOk)
						action = ACTION_OK;
					else if (gKeyTrg & gKeyLeft)
						action = ACTION_LEFT;
					else if (gKeyTrg & gKeyRight)
						action = ACTION_RIGHT;
				}

				return_value = options_menu->options[i].callback.main_callback(options_menu, i, action);

				// If the callback returned something other than CALLBACK_CONTINUE, it's time to exit this submenu
				if (return_value != CALLBACK_CONTINUE)
					break;
			}
		}

		if (return_value != CALLBACK_CONTINUE)
			break;

		// Update Quote animation counter
		if (++anime >= 40)
			anime = 0;

		// Draw screen
		PutBitmap4(&rcView, 0, 0, &rcView, SURFACE_ID_SCREEN_GRAB);
		PutBitmap3(&rcView, 0, 0, &rcView, SURFACE_ID_BLACK_OVERLAY);

		const size_t visible_options = MIN(MAX_OPTIONS, options_menu->total_options);

		int y = (WINDOW_HEIGHT / 2) - ((visible_options * 18) / 2) - (40 / 2);

		// Draw title
		PutText((WINDOW_WIDTH / 2) - ((options_menu->title.size() * 5) / 2), y, options_menu->title, RGB(0xFF, 0xFF, 0xFF));

		if (gbExtendedInfo){
		char buffer1[50], buffer2[50], buffer3[50], buffer4[100], buffer5[50], buffer6[50], buffer7[80], buffer8[60], buffer9[60],
			buffer10[60], buffer11[60];
		std::string buffer12;
		int v1, v2, v3, v4;
		GetCompileVersion(&v1, &v2, &v3, &v4);
		buffer12 = GetCSE2FormattedMessage("title_version_name", GetVersionName());
		sprintf(buffer1, "%s: %d.%d.%d.%d %s", GetCSE2Message("debug_version").c_str(), v1, v2, v3, v4, buffer12.data());
		PutText(2, 2, buffer1, RGB(0xFF, 0xFF, 0xFF));
		sprintf(buffer7, "%s: %s", GetCSE2Message("debug_compile_time").c_str(), __DATE__);
		PutText(2, 15, buffer7, RGB(0xFF, 0xFF, 0xFF));
		sprintf(buffer2, "%s: %lu", GetCSE2Message("debug_seed").c_str(), gMC.seed);
		PutText(2, 29, buffer2, RGB(0xFF, 0xFF, 0xFF));
		sprintf(buffer3, "%s: %d", GetCSE2Message("debug_score").c_str(), gMC.score);
		PutText(2, 43, buffer3, RGB(0xFF, 0xFF, 0xFF));
		sprintf(buffer3, "%s: %u", GetCSE2Message("debug_highscore").c_str(), playerHighScore);
		PutText(2, 57, buffer3, RGB(0xFF, 0xFF, 0xFF));
		std::string tilesetName = TilesetName();
		sprintf(buffer4, "%s: %s", GetCSE2Message("debug_tileset").c_str(), tilesetName.c_str());
		PutText(2, 71, buffer4, RGB(0xFF, 0xFF, 0xFF));
		sprintf(buffer5, "%s: %d", GetCSE2Message("debug_stars").c_str(), gMC.a);
		PutText(2, 85, buffer5, RGB(0xFF, 0xFF, 0xFF));
		int yy = 99;
		if (!ChallengeEnabled("one_hit")){
			sprintf(buffer6, (CharacterName() + (GetCSE2Message("debug_hp") + (std::string)": %d/%d")).c_str(), gMC.life, gMC.max_life);
			PutText(2, yy, buffer6, RGB(0xFF, 0xFF, 0xFF));
			yy += 14;
		}
		sprintf(buffer10, "%s:", GetCSE2Message("debug_fps").c_str());
		PutText(2, yy, buffer10, RGB(0xFF, 0xFF, 0xFF));
		PutFramePerSecound(10, yy+2);
		yy += 14;
		std::string gimmickName = GimmickName();
		sprintf(buffer11, "%s: %s", GetCSE2Message("debug_gimmick").c_str(), gimmickName.c_str());
		PutText(2, yy, buffer11, RGB(0xFF, 0xFF, 0xFF));
		yy += 14;
		std::string difficultyName = difficultyNames[gMC.difficulty];
		sprintf(buffer11, "%s: %s", GetCSE2Message("debug_difficulty").c_str(), difficultyName.c_str());
		PutText(2, yy, buffer11, RGB(0xFF, 0xFF, 0xFF));
		sprintf(buffer8, "%s: %.4f, %.4f", GetCSE2Message("debug_coords").c_str(), (float)gMC.x / 0x2000, (float)gMC.y / 0x2000);
		PutText(2, WINDOW_HEIGHT - 29, buffer8, RGB(0xFF, 0xFF, 0xFF));
		sprintf(buffer9, "%s: %d, %d", GetCSE2Message("debug_speed").c_str(), gMC.xm, gMC.ym);
		PutText(2, WINDOW_HEIGHT - 15, buffer9, RGB(0xFF, 0xFF, 0xFF));
		int fieldY = 2;
		PutText(WINDOW_WIDTH - 100, fieldY, GetCSE2Message("debug_config") + std::string{":"}, RGB(0xFF, 0xFF, 0xFF));
		for (auto it : gJConfig.items())
		{
			fieldY += 14;
			std::string output;
			output += it.key();
			output += ": ";
			output += it.value().dump();
			PutText(WINDOW_WIDTH - 100, fieldY, output.c_str(), RGB(0xFF, 0xFF, 0xFF));
		}
		}

		// Draw subtitle
		if (!options_menu->subtitle.empty())
			PutText((WINDOW_WIDTH / 2) - ((options_menu->subtitle.size() * 5) / 2), y + 12, options_menu->subtitle, RGB(0xFF, 0xFF, 0xFF));

		y += 30;

		for (size_t i = scroll; i < scroll + visible_options; ++i)
		{
			const int x = (WINDOW_WIDTH / 2) + options_menu->x_offset - 20;

			// Draw Quote next to the selected option
			if (i == selected_option)
				PutBitmap3(&grcFull, PixelToScreenCoord(x - 20), PixelToScreenCoord(y - 8), &rcMyChar[anime / 10 % 4], SURFACE_ID_MY_CHAR);

			unsigned long option_colour = options_menu->options[i].disabled ? RGB(0x80, 0x80, 0x80) : RGB(0xFF, 0xFF, 0xFF);

			// Draw option name
			PutText(x, y - (9 / 2), options_menu->options[i].name.normal_name, option_colour);

			// Draw option value, if it has one
			if (!options_menu->options[i].value_string.empty())
				PutText(x + 120, y - (9 / 2), options_menu->options[i].value_string, option_colour);

			y += 18;
		}

		PutFramePerSecound();

		if (!Flip_SystemTask())
		{
			// Quit if window is closed
			return_value = CALLBACK_EXIT;
			break;
		}
	}

	// Deinitialise options
	for (size_t i = 0; i < options_menu->total_options; ++i)
		options_menu->options[i].callback.main_callback(options_menu, i, ACTION_DEINIT);

	return return_value;
}

static int EnterOptionsMenuWithAlternates(OptionsMenu *options_menu, size_t selected_option)
{
	int scroll = 0;
	int character = 0;

	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	Rect rcTitle = {0, 0, 203, 48};

	Rect rcMyChar[4] = {
	{0, 16, 16, 32},
	{16, 16, 32, 32},
	{0, 16, 16, 32},
	{32, 16, 48, 32},
	};
	for (int i = 0; i <= MAX_CHARACTERS; i++){
		if (gJConfig.value("character", "") == CharacterCode(i)){
			character = characterOrder[i];
		}
	}
	for (int i = 0; i < 4; i++){
		rcMyChar[i].top += 32 * character;
		rcMyChar[i].bottom += 32 * character;
	}

	unsigned int anime = 0;

	int wins = std::count_if(gRankings.begin(), gRankings.end(), [](json record){
			auto flagStorage = record["flags"].get<std::vector<int>>();
			return std::find(flagStorage.begin(), flagStorage.end(), 1111) != flagStorage.end();
	});

	int return_value;

	// Initialise options
	for (size_t i = 0; i < options_menu->total_options; ++i)
		options_menu->options[i].callback.main_callback(options_menu, i, ACTION_INIT);

	for (;;)
	{
		// Get pressed keys
		GetTrg();

		// Allow unpausing by pressing the pause button only when in the main pause menu (not submenus)
		if (!options_menu->submenu && gKeyTrg & KEY_PAUSE)
		{
			return_value = CALLBACK_CONTINUE;
			break;
		}

		// Go back one submenu when the 'cancel' button is pressed
		if (gKeyTrg & gKeyCancel)
		{
			return_value = CALLBACK_CONTINUE;
			break;
		}

		// Handling up/down input
		if (gKeyTrg & (gKeyUp | gKeyDown))
		{
			const size_t old_selection = selected_option;

			if (gKeyTrg & gKeyDown)
				if (selected_option++ == options_menu->total_options - 1)
					selected_option = 0;

			if (gKeyTrg & gKeyUp)
				if (selected_option-- == 0)
					selected_option = options_menu->total_options - 1;

			// Update the menu-scrolling, if there are more options than can be fit on the screen
			if (selected_option < old_selection)
				scroll = MAX(0, MIN(scroll, (int)selected_option - 1));

			if (selected_option > old_selection)
				scroll = MIN(MAX(0, (int)options_menu->total_options - MAX_OPTIONS), MAX(scroll, (int)selected_option - (MAX_OPTIONS - 2)));

			PlaySoundObject(1, SOUND_MODE_PLAY);
		}

		// Run option callbacks
		for (size_t i = 0; i < options_menu->total_options; ++i)
		{
			if (!options_menu->options[i].disabled)
			{
				CallbackAction action = ACTION_UPDATE;

				if (i == selected_option)
				{
					if (gKeyTrg & gKeyOk)
						action = ACTION_OK;
					else if (gKeyTrg & gKeyLeft)
						action = ACTION_LEFT;
					else if (gKeyTrg & gKeyRight)
						action = ACTION_RIGHT;
				}

				if (gKey & gKeyAbility && !options_menu->options[i].name.alternative_name.empty())
					return_value = options_menu->options[i].callback.alt_callback(options_menu, i, action);
				else
					return_value = options_menu->options[i].callback.main_callback(options_menu, i, action);

				// If the callback returned something other than CALLBACK_CONTINUE, it's time to exit this submenu
				if (return_value != CALLBACK_CONTINUE)
					break;
			}
		}

		if (return_value != CALLBACK_CONTINUE)
			break;

		// Update Quote animation counter
		if (++anime >= 40)
			anime = 0;

		const size_t visible_options = MIN(MAX_OPTIONS, options_menu->total_options);

		int y = (WINDOW_HEIGHT / 2) - ((visible_options * 18) / 2) - (40 / 2) + 30;
		CortBox(&grcFull, RGB(0x05, 0x05, 0x10));
		int titleX = ((WINDOW_WIDTH / 2) - (rcTitle.right)/2);
		PutBitmap3(&rcView, PixelToScreenCoord(titleX), PixelToScreenCoord(15), &rcTitle, SURFACE_ID_TITLE);

		// Draw title
		PutText((WINDOW_WIDTH / 2) - ((options_menu->title.size() * 5) / 2), y, options_menu->title, RGB(0xFF, 0xFF, 0xFF));

		char buffer1[50], buffer2[50], buffer3[50];
		std::string buffer4, buffer5;
		int v1, v2, v3, v4;
		GetCompileVersion(&v1, &v2, &v3, &v4);
		sprintf(buffer1, "%s: %d.%d.%d.%d", GetCSE2Message("debug_version").c_str(), v1, v2, v3, v4);
		sprintf(buffer2, "%s: %u", GetCSE2Message("debug_highscore").c_str(), playerHighScore);
		sprintf(buffer3, "%s: %d/%d", GetCSE2Message("title_games").c_str(), wins, gRankings.size());
		buffer4 = GetCSE2FormattedMessage("title_alternate_tip", 
			GetKeyName(bindings[controls[13].binding_index].keyboard),
			GetKeyName(bindings[controls[13].binding_index].controller));
		buffer5 = GetCSE2FormattedMessage("title_version_name", GetVersionName());
		PutText(2, WINDOW_HEIGHT - 28, buffer1, RGB(0xFF, 0xFF, 0xFF));
		PutText(2, WINDOW_HEIGHT - 15, buffer5.data(), RGB(0xFF, 0xFF, 0xFF));
		PutText(2, 2, buffer2, RGB(0xFF, 0xFF, 0xFF));
		PutText(2, 15, buffer3, RGB(0xFF, 0xFF, 0xFF));
		PutText(titleX + 2, 2, buffer4.data(), RGB(0xFF, 0xFF, 0xFF));


		// Draw subtitle
		if (!options_menu->subtitle.empty())
			PutText((WINDOW_WIDTH / 2) - ((options_menu->subtitle.size() * 5) / 2), y + 12, options_menu->subtitle, RGB(0xFF, 0xFF, 0xFF));

		y += 30;

		for (size_t i = scroll; i < scroll + visible_options; ++i)
		{
			const int x = (WINDOW_WIDTH / 2) + options_menu->x_offset - 20;

			// Draw Quote next to the selected option
			if (i == selected_option)
				PutBitmap3(&grcFull, PixelToScreenCoord(x - 20), PixelToScreenCoord(y - 8), &rcMyChar[anime / 10 % 4], SURFACE_ID_MY_CHAR);

			unsigned long option_colour = options_menu->options[i].disabled ? RGB(0x80, 0x80, 0x80) : RGB(0xFF, 0xFF, 0xFF);

			// Draw option name
			std::string_view correctName = options_menu->options[i].name.normal_name;
			if (gKey & gKeyAbility && !options_menu->options[i].name.alternative_name.empty())
				correctName = options_menu->options[i].name.alternative_name;
			PutText(x, y - (9 / 2), correctName.data(), option_colour);

			// Draw option value, if it has one
			if (!options_menu->options[i].value_string.empty())
				PutText(x + 120, y - (9 / 2), options_menu->options[i].value_string, option_colour);

			y += 18;
		}

		PutFramePerSecound();

		if (!Flip_SystemTask())
		{
			// Quit if window is closed
			return_value = CALLBACK_EXIT;
			break;
		}
	}

	// Deinitialise options
	for (size_t i = 0; i < options_menu->total_options; ++i)
		options_menu->options[i].callback.main_callback(options_menu, i, ACTION_DEINIT);

	return return_value;
}

static int PromptAreYouSure2(void)
{
	struct FunctionHolder
	{
		static int Callback_Yes(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 1;	// Yes
		}

		static int Callback_No(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 0;	// No
		}
	};

	Option options[] = {
		{{GetCSE2Message("title_yes"), ""}, {FunctionHolder::Callback_Yes, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("title_no"), ""}, {FunctionHolder::Callback_No, NULL}, NULL, "", 0, false}
	};

	OptionsMenu options_menu = {
		GetCSE2Message("title_you_sure"),
		GetCSE2Message("title_warning_2"),
		options,
		sizeof(options) / sizeof(options[0]),
		-10,
		true
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	int return_value = EnterOptionsMenu(&options_menu, 1);

	PlaySoundObject(18, SOUND_MODE_PLAY);

	return return_value;
}

static int PromptAreYouSure3(void)
{
	struct FunctionHolder
	{
		static int Callback_Yes(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 1;	// Yes
		}

		static int Callback_No(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 0;	// No
		}
	};

	Option options[] = {
		{{GetCSE2Message("title_yes"), ""}, {FunctionHolder::Callback_Yes, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("title_no"), ""}, {FunctionHolder::Callback_No, NULL}, NULL, "", 0, false}
	};

	OptionsMenu options_menu = {
		GetCSE2Message("title_you_sure"),
		GetCSE2Message("title_warning_3"),
		options,
		sizeof(options) / sizeof(options[0]),
		-10,
		true
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	int return_value = EnterOptionsMenu(&options_menu, 0);

	PlaySoundObject(18, SOUND_MODE_PLAY);

	return return_value;
}

static int PromptAreYouSure4(void)
{
	struct FunctionHolder
	{
		static int Callback_Yes(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 1;	// Yes
		}

		static int Callback_No(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 0;	// No
		}
	};

	Option options[] = {
		{{GetCSE2Message("title_yes"), ""}, {FunctionHolder::Callback_Yes, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("title_no"), ""}, {FunctionHolder::Callback_No, NULL}, NULL, "", 0, false}
	};

	OptionsMenu options_menu = {
		GetCSE2Message("title_start_daily"),
		GetCSE2Message("title_warning_daily"),
		options,
		sizeof(options) / sizeof(options[0]),
		-10,
		true
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	int return_value = EnterOptionsMenuWithAlternates(&options_menu, 0);

	PlaySoundObject(18, SOUND_MODE_PLAY);

	return return_value;
}

///////////////////
// Controls menu //
///////////////////

const std::vector<std::string> controlNames = {
	"controls_up",
	"controls_down",
	"controls_left",
	"controls_right",
	"controls_ok",
	"controls_cancel",
	"controls_jump",
	"controls_shoot",
	"controls_next",
	"controls_previous",
	"controls_inventory",
	"controls_map",
	"controls_pause",
	"controls_ability",
	"controls_dash"
};

static void ReloadControlNames(){
	for (int i = 0; i < controlNames.size(); i++){
		controls[i].name = GetCSE2Message(controlNames[i]);
	}
}

static int Callback_ControlsKeyboard_Rebind(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	switch (action)
	{
		default:
			break;

		case ACTION_OK:
			PlaySoundObject(5, SOUND_MODE_PLAY);

			char timeout_string[2];
			timeout_string[1] = '\0';

			bool previous_keyboard_state[BACKEND_KEYBOARD_TOTAL];
			memcpy(previous_keyboard_state, gKeyboardState, sizeof(gKeyboardState));

			// Time-out and exit if the user takes too long (they probably don't want to rebind)
			for (unsigned int timeout = (60 * 5) - 1; timeout != 0; --timeout)
			{
				for (int scancode = 0; scancode < BACKEND_KEYBOARD_TOTAL; ++scancode)
				{
					// Wait for user to press a key
					if (!previous_keyboard_state[scancode] && gKeyboardState[scancode])
					{
						std::string key_name = GetKeyName(scancode);

						// If another control in the same group uses this key, swap them
						for (size_t other_option = 0; other_option < parent_menu->total_options; ++other_option)
						{
							if (other_option != this_option && controls[other_option].groups & controls[this_option].groups && bindings[controls[other_option].binding_index].keyboard == scancode)
							{
								bindings[controls[other_option].binding_index].keyboard = bindings[controls[this_option].binding_index].keyboard;
								parent_menu->options[other_option].value_string = parent_menu->options[this_option].value_string;
								break;
							}
						}

						// Otherwise just overwrite the selected control
						bindings[controls[this_option].binding_index].keyboard = scancode;
						parent_menu->options[this_option].value_string = key_name;

						PlaySoundObject(18, SOUND_MODE_PLAY);

						gKeyTrg = gKey = 0;	// Prevent weird input-ghosting by doing this
						return CALLBACK_CONTINUE;
					}
				}

				memcpy(previous_keyboard_state, gKeyboardState, sizeof(gKeyboardState));

				// Draw screen
				CortBox(&grcFull, 0x000000);

				std::string string = GetCSE2Message("keybinding_prompt");
				PutText((WINDOW_WIDTH / 2) - ((string.size() * 5) / 2), (WINDOW_HEIGHT / 2) - 10, string, RGB(0xFF, 0xFF, 0xFF));
				PutText((WINDOW_WIDTH / 2) - ((parent_menu->options[this_option].name.normal_name.size() * 5) / 2), (WINDOW_HEIGHT / 2) + 10, parent_menu->options[this_option].name.normal_name, RGB(0xFF, 0xFF, 0xFF));

				timeout_string[0] = '0' + (timeout / 60) + 1;
				PutText((WINDOW_WIDTH / 2) - (5 / 2), (WINDOW_HEIGHT / 2) + 60, timeout_string, RGB(0xFF, 0xFF, 0xFF));

				PutFramePerSecound();

				if (!Flip_SystemTask())
				{
					// Quit if window is closed
					return CALLBACK_EXIT;
				}
			}

			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_PickDifficulty(OptionsMenu *parent_menu, size_t this_option){
	
	(void)parent_menu;
	LoadTextScript2("StageSelect.tsc");
	InitBack("bkChtulhu", 7);
	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	Rect rcSelect = {113, 90, 126, 100};
	int selectedDifficulty = 0;
	int difficultyColors[] = {RGB(0xFF, 0xFF, 0xFF), RGB(0xD5, 0xF6, 0xFF), RGB(0xFF, 0xD7, 0x00), RGB(0x00, 0xDC, 0xA2)};

	StartTextScript(2000 + selectedDifficulty);
	ChangeMusic(MUS_SELECT);

	for (;;)
	{
		GetTrg();

		PutBack(0, 0);
		ActBack();
		PutBitmap3(&rcView, 0, 0, &rcView, SURFACE_ID_BLACK_OVERLAY);
		PutBitmap3(&rcView, 0, 0, &rcView, SURFACE_ID_BLACK_OVERLAY);
		int y = WINDOW_HEIGHT / 3 - 60;
		PutText(WINDOW_WIDTH / 2 - 45, y, GetCSE2Message("difficulty_select"), RGB(0xFF, 0xFF, 0xFF));

		bool bChange = false;

		if (gKeyTrg & gKeyUp){
			if (--selectedDifficulty < 0)
				selectedDifficulty = difficultyNames.size()-1;
			bChange = true;
		}
		if (gKeyTrg & gKeyDown){
			if (++selectedDifficulty > difficultyNames.size()-1)
				selectedDifficulty = 0;
			bChange = true;
		}

		if (bChange){
			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);
			StartTextScript(2000 + selectedDifficulty);
			bChange = false;
		}

		if (gKeyTrg & gKeyCancel){
			StartTextScript(2050 + selectedDifficulty);
		}

		for (int i = 0; i < difficultyNames.size(); i++){
			PutText(WINDOW_WIDTH / 2 - 20, y + 40 + i*17, difficultyNames[i].c_str(), difficultyColors[i]);
		}

		PutBitmap3(&rcView, PixelToScreenCoord(WINDOW_WIDTH / 2 - 36), PixelToScreenCoord(y + 40 + selectedDifficulty*17), &rcSelect, SURFACE_ID_TEXT_BOX);
		PutText(0, 2, GetCSE2Message("select_ok_tip"), RGB(0xFF, 0xFF, 0xFF));
		PutText(0, 16, GetCSE2Message("select_cancel_tip"), RGB(0xFF, 0xFF, 0xFF));
		PutText(0, 30, GetCSE2Message("select_arrows_tip"), RGB(0xFF, 0xFF, 0xFF));

		if (gKeyTrg & gKeyOk){
			gConfigCopy["difficulty"] = selectedDifficulty;
			gJConfig["difficulty"] = selectedDifficulty;
			PlaySoundObject(18, SOUND_MODE_PLAY);
			StartFadeOut(4);
			break;
		}

		switch (TextScriptProc())
		{
			case enum_ESCRETURN_exit:
				return enum_ESCRETURN_exit;	// Quit game

			case enum_ESCRETURN_restart:
				return enum_ESCRETURN_restart;	// Go to game intro
		}

		PutTextScript();

		PutFramePerSecound();

		if (!Flip_SystemTask())
		{
			// Quit if window is closed
			return CALLBACK_EXIT;
			break;
		}
	}

	SetOrganyaFadeout();
	gDaily = false;

	int wait = Backend_GetTicks();
	int waitTime = 1500;
	if (mode == 1) waitTime = 0;
	while (Backend_GetTicks() < wait + waitTime)
	{
		CortBox(&grcGame, RGB(0, 0, 0x20));
		ProcFade();
		PutFade();
		PutFramePerSecound();
		if (!Flip_SystemTask())
			return CALLBACK_EXIT;
	}

	return CALLBACK_RESET;
}

std::string resourceDesc; 

static int Callback_Resources(OptionsMenu *parent_menu, size_t this_option, CallbackAction action){
	int scroll = 0;
	(void)parent_menu;
	InitBack("bkChtulhu", 7);
	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;
	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	Rect rcSelect = {113, 90, 126, 100};
	Rect rcIcon = {0, 0, 48, 48};
	int selected_option = 0;
	int return_value;
	bool iconLoaded = ReloadBitmap_File((availableResources[selected_option].path / "icon").string().c_str(), SURFACE_ID_RESOURCE_ICON, true);

	ChangeMusic(MUS_SELECT);
	resourceDesc = availableResources[selected_option].description;
	if (resourceDesc == "")
		resourceDesc = GetCSE2Message("resources_default");

	for (;;)
	{
		GetTrg();

		PutBack(0, 0);
		ActBack();
		PutBitmap3(&rcView, 0, 0, &rcView, SURFACE_ID_BLACK_OVERLAY);

		bool bChange = false;

		if (gKeyTrg & gKeyCancel)
		{
			return_value = CALLBACK_CONTINUE;
			break;
		}

		if (gKeyTrg & gKeyOk)
		{
			InitResources(NULL);
			PlaySoundObject(18, SOUND_MODE_PLAY);
			return_value = CALLBACK_CONTINUE;
			break;
		}

		if (gKeyTrg & (gKeyUp | gKeyDown))
		{
			const size_t old_selection = selected_option;

			if (gKeyTrg & gKeyDown)
				if (selected_option++ == availableResources.size() - 1)
					selected_option = 0;

			if (gKeyTrg & gKeyUp)
				if (selected_option-- == 0)
					selected_option = availableResources.size() - 1;

			// Update the menu-scrolling, if there are more options than can be fit on the screen
			if (selected_option < old_selection)
				scroll = MAX(0, MIN(scroll, (int)selected_option - 1));

			if (selected_option > old_selection)
				scroll = MIN(MAX(0, availableResources.size() - (MAX_OPTIONS-3)), MAX(scroll, (int)selected_option - (MAX_OPTIONS - 5)));

			PlaySoundObject(1, SOUND_MODE_PLAY);
			bChange = true;
		}

		const size_t visible_options = MIN(MAX_OPTIONS-3, availableResources.size());

		int y = (WINDOW_HEIGHT / 2) - ((visible_options * 18) / 2) - (40 / 2);

		if (bChange){
			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);
			resourceDesc = availableResources[selected_option].description;
			if (resourceDesc == "")
				resourceDesc = GetCSE2Message("resources_default");
			iconLoaded = ReloadBitmap_File((availableResources[selected_option].path / "icon").string().c_str(), SURFACE_ID_RESOURCE_ICON, true);
			bChange = false;
		}
		PutText(WINDOW_WIDTH / 2 - 45, y, GetCSE2Message("resources_title"), RGB(0xFF, 0xFF, 0xFF));

		y += 30;

		for (size_t i = scroll; i < scroll + visible_options; ++i)
		{
			const int x = (WINDOW_WIDTH / 2) - 34;

			// Draw Quote next to the selected option
			if (i == selected_option)
				PutBitmap3(&grcFull, PixelToScreenCoord(x - 20), PixelToScreenCoord(y - 5), &rcSelect, SURFACE_ID_TEXT_BOX);

			unsigned long option_colour = RGB(0xFF, 0xFF, 0xFF);

			// Draw option name
			PutText(x, y - (9 / 2), availableResources[i].name, option_colour);

			y += 18;
		}

		Rect rcFrame1 = {0, 0, 244, 8};
		Rect rcFrame2 = {0, 8, 244, 16};
		Rect rcFrame3 = {0, 16, 244, 24};
		int i;
		PutBitmap3(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 122), PixelToScreenCoord(WINDOW_HEIGHT - 66), &rcFrame1, SURFACE_ID_TEXT_BOX);
		for (i = 1; i < 7; ++i)
			PutBitmap3(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 122), PixelToScreenCoord((i * 8) + WINDOW_HEIGHT - 66), &rcFrame2, SURFACE_ID_TEXT_BOX);
		PutBitmap3(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 122), PixelToScreenCoord((i * 8) + WINDOW_HEIGHT - 66), &rcFrame3, SURFACE_ID_TEXT_BOX);

		int text_offset = 0;
		if (iconLoaded){
			PutBitmap3(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2 - 108)), PixelToScreenCoord(WINDOW_HEIGHT - 56), &rcIcon, SURFACE_ID_RESOURCE_ICON);
			text_offset = 56;
		}
		PutText((WINDOW_WIDTH / 2 - 108) + text_offset, WINDOW_HEIGHT - 56, resourceDesc, RGB(0xFF, 0xFF, 0xFF));
		PutText(0, 0, GetCSE2Message("select_arrows_tip"), RGB(0xFF, 0xFF, 0xFF));
		PutText(0, 15, GetCSE2Message("resources_reload"), RGB(0xFF, 0xFF, 0xFF));
		PutText(0, 30, GetCSE2Message("resources_exit"), RGB(0xFF, 0xFF, 0xFF));

		// if (gKeyTrg & gKeyOk){
		// 	gConfigCopy["difficulty"] = selectedDifficulty;
		// 	gJConfig["difficulty"] = selectedDifficulty;
		// 	PlaySoundObject(18, SOUND_MODE_PLAY);
		// 	StartFadeOut(4);
		// 	break;
		// }

		PutFramePerSecound();

		if (!Flip_SystemTask())
		{
			// Quit if window is closed
			return CALLBACK_EXIT;
			break;
		}
	}

	return return_value;
}

static int Callback_PickCharacter(OptionsMenu *parent_menu, size_t this_option){
	
	(void)parent_menu;
	LoadTextScript2("StageSelect.tsc");
	InitBack("bkChtulhu", 7);
	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	Rect rcSelect = {214, 134, 234, 154};
	int selectedChar = 0;

	StartTextScript(900 + selectedChar);
	ChangeMusic(MUS_SELECT);

	for (;;)
	{
		GetTrg();

		PutBack(0, 0);
		ActBack();
		PutBitmap3(&rcView, 0, 0, &rcView, SURFACE_ID_BLACK_OVERLAY);
		int y = WINDOW_HEIGHT / 3 - 60;
		PutText(WINDOW_WIDTH / 2 - 45, y, GetCSE2Message("character_select"), RGB(0xFF, 0xFF, 0xFF));

		bool bChange = false;

		if (gKeyTrg & gKeyLeft){
			if (--selectedChar < 0)
				selectedChar = MAX_CHARACTERS;
			bChange = true;
		}
		if (gKeyTrg & gKeyRight){
			if (++selectedChar > MAX_CHARACTERS)
				selectedChar = 0;
			bChange = true;
		}
		if (gKeyTrg & gKeyUp){
			selectedChar -= 4;
			if (selectedChar < 0)
				selectedChar = MAX_CHARACTERS;
			bChange = true;
		}
		if (gKeyTrg & gKeyDown){
			selectedChar += 4;
			if (selectedChar > MAX_CHARACTERS)
				selectedChar = 0;
			bChange = true;
		}

		if (bChange){
			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);
			StartTextScript(900 + selectedChar);
			bChange = false;
		}

		if (gKeyTrg & gKeyCancel){
			StartTextScript(950 + selectedChar);
		}

		for (int i = 0; i <= MAX_CHARACTERS; i++){
			Rect rcChar = {0, 0, 16, 16};
			rcChar.top += characterOrder[i] * 32;
			rcChar.bottom = rcChar.top + 16;
			PutBitmap3(&rcView, PixelToScreenCoord(WINDOW_WIDTH / 2 - 40 + (i % 4)*20), PixelToScreenCoord(y + 25 + (i / 4)*17), &rcChar, SURFACE_ID_MY_CHAR);
		}
		PutBitmap3(&rcView, PixelToScreenCoord(WINDOW_WIDTH / 2 - 42 + (selectedChar % 4)*20), PixelToScreenCoord(y + 23 + (selectedChar / 4)*17), &rcSelect, SURFACE_ID_TEXT_BOX);
		PutText(0, 2, GetCSE2Message("select_ok_tip"), RGB(0xFF, 0xFF, 0xFF));
		PutText(0, 16, GetCSE2Message("select_cancel_tip"), RGB(0xFF, 0xFF, 0xFF));
		PutText(0, 30, GetCSE2Message("select_arrows_tip"), RGB(0xFF, 0xFF, 0xFF));

		if (gKeyTrg & gKeyOk){
			gConfigCopy["character"] = CharacterCode(selectedChar);
			PlaySoundObject(18, SOUND_MODE_PLAY);
			StartFadeOut(4);
			break;
		}

		switch (TextScriptProc())
		{
			case enum_ESCRETURN_exit:
				return enum_ESCRETURN_exit;	// Quit game

			case enum_ESCRETURN_restart:
				return enum_ESCRETURN_restart;	// Go to game intro
		}

		PutTextScript();

		PutFramePerSecound();

		if (!Flip_SystemTask())
		{
			// Quit if window is closed
			return CALLBACK_EXIT;
			break;
		}
	}

	return Callback_PickDifficulty(parent_menu, this_option);

	// SetOrganyaFadeout();

	// int wait = Backend_GetTicks();
	// int waitTime = 1500;
	// if (mode == 1) waitTime = 0;
	// while (Backend_GetTicks() < wait + waitTime)
	// {
	// 	CortBox(&grcGame, RGB(0, 0, 0x20));
	// 	ProcFade();
	// 	PutFade();
	// 	PutFramePerSecound();
	// 	if (!Flip_SystemTask())
	// 		return CALLBACK_EXIT;
	// }

	// return CALLBACK_RESET;
}


static int Callback_ControlsKeyboard(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	ReloadControlNames();

	Option options[sizeof(controls) / sizeof(controls[0])];

	for (size_t i = 0; i < sizeof(controls) / sizeof(controls[0]); ++i)
	{
		options[i].name.normal_name = controls[i].name;
		options[i].callback.main_callback = Callback_ControlsKeyboard_Rebind;
		options[i].value_string = GetKeyName(bindings[controls[i].binding_index].keyboard);
		options[i].disabled = false;
	}

	OptionsMenu options_menu = {
		GetCSE2Message("keyboard_title"),
		"",
		options,
		sizeof(options) / sizeof(options[0]),
		-60,
		true
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	const int return_value = EnterOptionsMenu(&options_menu, 0);

	PlaySoundObject(5, SOUND_MODE_PLAY);

	return return_value;
}

static int Callback_ControlsController_Rebind(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	switch (action)
	{
		default:
			break;

		case ACTION_OK:
			PlaySoundObject(5, SOUND_MODE_PLAY);

			JOYSTICK_STATUS old_state = gJoystickState;

			char timeout_string[2];
			timeout_string[1] = '\0';

			// Time-out and exit if the user takes too long (they probably don't want to rebind)
			for (unsigned int timeout = (60 * 5) - 1; timeout != 0; --timeout)
			{
				for (int button = 0; button < sizeof(gJoystickState.bButton) / sizeof(gJoystickState.bButton[0]); ++button)
				{
					// Wait for user to press a button
					if (!old_state.bButton[button] && gJoystickState.bButton[button])
					{
						// If another control in the same group uses this button, swap them
						for (size_t other_option = 0; other_option < parent_menu->total_options; ++other_option)
						{
							if (other_option != this_option && controls[other_option].groups & controls[this_option].groups && bindings[controls[other_option].binding_index].controller == button)
							{
								bindings[controls[other_option].binding_index].controller = bindings[controls[this_option].binding_index].controller;
								parent_menu->options[other_option].value_string = parent_menu->options[this_option].value_string;
								break;
							}
						}

						// Otherwise just overwrite the selected control
						bindings[controls[this_option].binding_index].controller = button;
						parent_menu->options[this_option].value_string = ControllerBackend_GetButtonName(button);

						PlaySoundObject(18, SOUND_MODE_PLAY);

						gKeyTrg = gKey = 0;	// Prevent weird input-ghosting by doing this
						return CALLBACK_CONTINUE;
					}
				}

				old_state = gJoystickState;

				// Draw screen
				CortBox(&grcFull, 0x000000);

				std::string string = GetCSE2Message("gamepad_bind_prompt");
				PutText((WINDOW_WIDTH / 2) - ((string.size() * 5) / 2), (WINDOW_HEIGHT / 2) - 10, string, RGB(0xFF, 0xFF, 0xFF));
				PutText((WINDOW_WIDTH / 2) - ((parent_menu->options[this_option].name.normal_name.size() * 5) / 2), (WINDOW_HEIGHT / 2) + 10, parent_menu->options[this_option].name.normal_name, RGB(0xFF, 0xFF, 0xFF));

				timeout_string[0] = '0' + (timeout / 60) + 1;
				PutText((WINDOW_WIDTH / 2) - (5 / 2), (WINDOW_HEIGHT / 2) + 60, timeout_string, RGB(0xFF, 0xFF, 0xFF));

				PutFramePerSecound();

				if (!Flip_SystemTask())
				{
					// Quit if window is closed
					return CALLBACK_EXIT;
				}
			}

			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_ControlsController(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	ReloadControlNames();

	Option options[sizeof(controls) / sizeof(controls[0])];

	for (size_t i = 0; i < sizeof(controls) / sizeof(controls[0]); ++i)
	{
		options[i].name.normal_name = controls[i].name;
		options[i].callback.main_callback = Callback_ControlsController_Rebind;
		if (bindings[controls[i].binding_index].controller == 0xFF)
			options[i].value_string = GetCSE2Message("key_unbound");
		else
			options[i].value_string = ControllerBackend_GetButtonName(bindings[controls[i].binding_index].controller);
		options[i].disabled = false;
	}

	OptionsMenu options_menu = {
		Backend_IsConsole() ? GetCSE2Message("controls_title") : GetCSE2Message("gamepad_title"),
		"",
		options,
		sizeof(options) / sizeof(options[0]),
		-70,
		true
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	const int return_value = EnterOptionsMenu(&options_menu, 0);

	PlaySoundObject(5, SOUND_MODE_PLAY);

	return return_value;
}

//////////////////
// Options menu //
//////////////////

static int Callback_Framerate(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	std::string strings[] = {GetCSE2Message("options_50fps"), GetCSE2Message("options_60fps")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->b60fps;
			parent_menu->options[this_option].value_string = strings[conf->b60fps];
			break;

		case ACTION_DEINIT:
			conf->b60fps = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gb60fps = parent_menu->options[this_option].value;

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Vsync(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bVsync;
			parent_menu->options[this_option].value_string = strings[conf->bVsync];
			break;

		case ACTION_DEINIT:
			conf->bVsync = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_restart");

			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Resolution(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	std::string strings[] = {GetCSE2Message("options_fullscreen"),
	 GetCSE2Message("options_windowed_1"), GetCSE2Message("options_windowed_2"), GetCSE2Message("options_windowed_3"), GetCSE2Message("options_windowed_4")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->display_mode;
			parent_menu->options[this_option].value_string = strings[conf->display_mode];
			break;

		case ACTION_DEINIT:
			conf->display_mode = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_restart");

			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value = (sizeof(strings) / sizeof(strings[0])) - 1;
			}
			else
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > (sizeof(strings) / sizeof(strings[0])) - 1)
					parent_menu->options[this_option].value = 0;
			}

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_SmoothScrolling(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bSmoothScrolling;
			parent_menu->options[this_option].value_string = strings[conf->bSmoothScrolling];
			break;

		case ACTION_DEINIT:
			conf->bSmoothScrolling = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gbSmoothScrolling = parent_menu->options[this_option].value;

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_ExtendedInfo(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bExtendedInfo;
			parent_menu->options[this_option].value_string = strings[conf->bExtendedInfo];
			break;

		case ACTION_DEINIT:
			conf->bExtendedInfo = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gbExtendedInfo = parent_menu->options[this_option].value;

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}


static int Callback_Character(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	int return_value = PromptAreYouSure2();

	switch (return_value)
	{
		case 0:
			return_value = CALLBACK_CONTINUE;// Go back to previous menu
			break;

		case 1:
			DeleteSave();
			restart_required = false;
			gGameMode = 3;
			return_value = Callback_PickCharacter(parent_menu, this_option);	// Restart game
			break;
	}

	return return_value;
}

static int Callback_Character2(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;
	gConfigCopy = json(gJConfig);

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;
	restart_required = false;
	int return_value = Callback_PickCharacter(parent_menu, this_option);	// Restart game
	WriteJSON(gModulePath + "/config.json", gConfigCopy, mode::normal);
	ReloadConfig();
	LoadInterface(ChallengeEnabled("legacyUI"));
	gGameMode = 3;

	return return_value;
}

static int Callback_Darkness(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("darkness", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["darkness"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["darkness"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_LegacyUI(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("legacyUI", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["legacyUI"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["legacyUI"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_SecondCampaign(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("secondCampaign", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["secondCampaign"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["secondCampaign"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_OneDamage(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("oneDamage", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["oneDamage"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["oneDamage"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_PitchRandomizer(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("pitch_randomizer", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["pitch_randomizer"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["pitch_randomizer"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_RandomSizeMaps(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("randomSizeMode", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["randomSizeMode"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["randomSizeMode"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_RelativisticMode(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("superHotMode", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["superHotMode"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["superHotMode"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_FastRun(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("fast_run", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["fast_run"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["fast_run"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_FullClear(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("full_clear", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["full_clear"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["full_clear"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Timer(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("timer", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["timer"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["timer"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_NoExp(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("no_exp", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["no_exp"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["no_exp"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_NoHealth(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("no_health", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["no_health"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["no_health"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_AdvancedArms(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("advanced_arms", true);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["advanced_arms"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["advanced_arms"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_ChargeJump(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("charge_jump", true);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["charge_jump"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["charge_jump"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_SpikeKill(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("spike_kill", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["spike_kill"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["spike_kill"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_OneHit(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("one_hit", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["one_hit"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["one_hit"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_NoPlacing(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("no_placing", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["no_placing"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["no_placing"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_NoBreaking(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("no_breaking", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["no_breaking"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["no_breaking"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_NoMusic(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("organya_mute", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["organya_mute"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["organya_mute"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_NoMap(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("no_map", false);

	std::string strings[] = {GetCSE2Message("options_off"), GetCSE2Message("options_on")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["no_map"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["no_map"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_MapImprovement(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	bool darkness = gJConfig.value("improve_map", false);

	std::string strings[] = {GetCSE2Message("title_map_normal"), GetCSE2Message("title_map_improved")};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = darkness;
			parent_menu->options[this_option].value_string = strings[darkness];
			break;

		case ACTION_DEINIT:
			gConfigCopy["improve_map"] = bool(parent_menu->options[this_option].value);
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = true;
			parent_menu->subtitle = GetCSE2Message("options_reset");
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gConfigCopy["improve_map"] = bool(parent_menu->options[this_option].value);

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Configuration(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;
	
	gConfigCopy = json(gJConfig);

	Option options_pc[] = {
		{{GetCSE2Message("config_new_game"), ""}, {Callback_Character, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_no_music"), ""}, {Callback_NoMusic, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_darkness"), ""}, {Callback_Darkness, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_charge_jump"), ""}, {Callback_ChargeJump, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_adv_arms"), ""}, {Callback_AdvancedArms, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_pitch_random"), ""}, {Callback_PitchRandomizer, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_no_exp"), ""}, {Callback_NoExp, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_one_damage"), ""}, {Callback_OneDamage, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_relativistic"), ""}, {Callback_RelativisticMode, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_random_size_maps"), ""}, {Callback_RandomSizeMaps, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_legacyUI"), ""}, {Callback_LegacyUI, NULL}, NULL, "", 0, aprilFools},
		{{GetCSE2Message("config_secondCampaign"), ""}, {Callback_SecondCampaign, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_no_map"), ""}, {Callback_NoMap, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_improve_map"), ""}, {Callback_MapImprovement, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_fast_run"), ""}, {Callback_FastRun, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_full_clear"), ""}, {Callback_FullClear, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_no_placing"), ""}, {Callback_NoPlacing, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_no_breaking"), ""}, {Callback_NoBreaking, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_spike_kill"), ""}, {Callback_SpikeKill, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_1_hp"), ""}, {Callback_OneHit, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_timer"), ""}, {Callback_Timer, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_no_life"), ""}, {Callback_NoHealth, NULL}, NULL, "", 0, false},
	};

	OptionsMenu options_menu = {
		GetCSE2Message("config_title"),
		restart_required ? GetCSE2Message("options_reset") : "",
		options_pc,
		(sizeof(options_pc) / sizeof(options_pc[0])),
		-70,
		true
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	const int return_value = EnterOptionsMenu(&options_menu, 0);

	if (return_value == CALLBACK_CONTINUE)
		PlaySoundObject(5, SOUND_MODE_PLAY);

	WriteJSON(gModulePath + "/config.json", gConfigCopy, mode::normal);

	return return_value;
}

static int Callback_Configuration2(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;
	
	gConfigCopy = json(gJConfig);

	Option options_pc[] = {
		{{GetCSE2Message("config_new_game"), ""}, {Callback_Character, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_no_music"), ""}, {Callback_NoMusic, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_darkness"), ""}, {Callback_Darkness, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_charge_jump"), ""}, {Callback_ChargeJump, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_adv_arms"), ""}, {Callback_AdvancedArms, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_pitch_random"), ""}, {Callback_PitchRandomizer, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_no_exp"), ""}, {Callback_NoExp, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_one_damage"), ""}, {Callback_OneDamage, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_relativistic"), ""}, {Callback_RelativisticMode, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_random_size_maps"), ""}, {Callback_RandomSizeMaps, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_legacyUI"), ""}, {Callback_LegacyUI, NULL}, NULL, "", 0, aprilFools},
		{{GetCSE2Message("config_secondCampaign"), ""}, {Callback_SecondCampaign, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_no_map"), ""}, {Callback_NoMap, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_improve_map"), ""}, {Callback_MapImprovement, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_fast_run"), ""}, {Callback_FastRun, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_full_clear"), ""}, {Callback_FullClear, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_no_placing"), ""}, {Callback_NoPlacing, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_no_breaking"), ""}, {Callback_NoBreaking, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_spike_kill"), ""}, {Callback_SpikeKill, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_1_hp"), ""}, {Callback_OneHit, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_timer"), ""}, {Callback_Timer, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("config_no_life"), ""}, {Callback_NoHealth, NULL}, NULL, "", 0, false},
	};

	OptionsMenu options_menu = {
		"",
		"",
		options_pc,
		(sizeof(options_pc) / sizeof(options_pc[0])),
		-70,
		true
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	const int return_value = EnterOptionsMenuWithAlternates(&options_menu, 0);

	if (return_value == CALLBACK_CONTINUE)
		PlaySoundObject(5, SOUND_MODE_PLAY);

	WriteJSON(gModulePath + "/config.json", gConfigCopy, mode::normal);
	ReloadConfig();
	LoadInterface(ChallengeEnabled("legacyUI"));

	return return_value;
}

static int Callback_Options(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	// Make the options match the configuration data
	CONFIG conf;
	if (!LoadConfigData(&conf))
		DefaultConfigData(&conf);

	bool is_console = Backend_IsConsole();

	Option options_console[] = {
		{{GetCSE2Message("options_controls"), ""}, {Callback_ControlsController, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("options_framerate"), ""}, {Callback_Framerate, NULL}, &conf, "", 0, false},
		{{GetCSE2Message("options_scrolling"), ""}, {Callback_SmoothScrolling, NULL}, &conf, "", 0, false},
		{{GetCSE2Message("options_debug"), ""}, {Callback_ExtendedInfo, NULL}, &conf, "", 0, false}
	};

	Option options_pc[] = {
		{{GetCSE2Message("options_keyboard"), ""}, {Callback_ControlsKeyboard, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("options_gamepad"), ""}, {Callback_ControlsController, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("options_framerate"), ""}, {Callback_Framerate, NULL}, &conf, "", 0, false},
		{{GetCSE2Message("options_vsync"), ""}, {Callback_Vsync, NULL}, &conf, "", 0, false},
		{{GetCSE2Message("options_controls"), ""}, {Callback_Resolution, NULL}, &conf, "", 0, false},
		{{GetCSE2Message("options_scrolling"), ""}, {Callback_SmoothScrolling, NULL}, &conf, "", 0, false},
		{{GetCSE2Message("options_debug"), ""}, {Callback_ExtendedInfo, NULL}, &conf, "", 0, false}
	};

	OptionsMenu options_menu = {
		GetCSE2Message("options_title"),
		restart_required ? GetCSE2Message("options_restart") : "",
		is_console ? options_console : options_pc,
		is_console ? (sizeof(options_console) / sizeof(options_console[0])) : (sizeof(options_pc) / sizeof(options_pc[0])),
		is_console ? -60 : -70,
		true
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	const int return_value = EnterOptionsMenu(&options_menu, 0);

	PlaySoundObject(5, SOUND_MODE_PLAY);

	// Save our changes to the configuration file
	memcpy(conf.bindings, bindings, sizeof(bindings));

	SaveConfigData(&conf);

	return return_value;
}

static int Callback_Options2(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	// Make the options match the configuration data
	CONFIG conf;
	if (!LoadConfigData(&conf))
		DefaultConfigData(&conf);

	bool is_console = Backend_IsConsole();

	Option options_console[] = {
		{{GetCSE2Message("options_controls"), ""}, {Callback_ControlsController, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("options_framerate"), ""}, {Callback_Framerate, NULL}, &conf, "", 0, false},
		{{GetCSE2Message("options_scrolling"), ""}, {Callback_SmoothScrolling, NULL}, &conf, "", 0, false},
		{{GetCSE2Message("options_debug"), ""}, {Callback_ExtendedInfo, NULL}, &conf, "", 0, false}
	};

	Option options_pc[] = {
		{{GetCSE2Message("options_keyboard"), ""}, {Callback_ControlsKeyboard, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("options_gamepad"), ""}, {Callback_ControlsController, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("options_framerate"), ""}, {Callback_Framerate, NULL}, &conf, "", 0, false},
		{{GetCSE2Message("options_vsync"), ""}, {Callback_Vsync, NULL}, &conf, "", 0, false},
		{{GetCSE2Message("options_controls"), ""}, {Callback_Resolution, NULL}, &conf, "", 0, false},
		{{GetCSE2Message("options_scrolling"), ""}, {Callback_SmoothScrolling, NULL}, &conf, "", 0, false},
		{{GetCSE2Message("options_debug"), ""}, {Callback_ExtendedInfo, NULL}, &conf, "", 0, false}
	};

	OptionsMenu options_menu = {
		GetCSE2Message("options_title"),
		restart_required ? GetCSE2Message("options_restart") : "",
		is_console ? options_console : options_pc,
		is_console ? (sizeof(options_console) / sizeof(options_console[0])) : (sizeof(options_pc) / sizeof(options_pc[0])),
		is_console ? -60 : -70,
		true
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	const int return_value = EnterOptionsMenuWithAlternates(&options_menu, 0);

	PlaySoundObject(5, SOUND_MODE_PLAY);

	// Save our changes to the configuration file
	memcpy(conf.bindings, bindings, sizeof(bindings));

	SaveConfigData(&conf);

	return return_value;
}

////////////////
// Pause menu //
////////////////

static int PromptAreYouSure(void)
{
	struct FunctionHolder
	{
		static int Callback_Yes(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 1;	// Yes
		}

		static int Callback_No(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 0;	// No
		}
	};

	Option options[] = {
		{{GetCSE2Message("title_yes"), ""}, {FunctionHolder::Callback_Yes, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("title_no"), ""}, {FunctionHolder::Callback_No, NULL}, NULL, "", 0, false}
	};

	OptionsMenu options_menu = {
		GetCSE2Message("title_you_sure"),
		GetCSE2Message("title_warning_1"),
		options,
		sizeof(options) / sizeof(options[0]),
		-10,
		true
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	int return_value = EnterOptionsMenu(&options_menu, 1);

	PlaySoundObject(18, SOUND_MODE_PLAY);

	return return_value;
}

static int PromptSaves(void)
{
	struct FunctionHolder
	{
		static int Callback_Yes(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 1;	// Yes
		}

		static int Callback_No(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 0;	// No
		}
	};

	Option options[] = {
		{{GetCSE2Message("title_yes"), ""}, {FunctionHolder::Callback_Yes, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("title_no"), ""}, {FunctionHolder::Callback_No, NULL}, NULL, "", 0, false}
	};

	OptionsMenu options_menu = {
		GetCSE2Message("title_load_confirm"),
		GetCSE2Message("title_warning_2"),
		options,
		sizeof(options) / sizeof(options[0]),
		-10,
		true
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	int return_value = EnterOptionsMenu(&options_menu, 1);

	PlaySoundObject(18, SOUND_MODE_PLAY);

	return return_value;
}

static int Callback_Tutorial(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	PlaySoundObject(18, SOUND_MODE_PLAY);
	StartTextScript(80);
	return enum_ESCRETURN_continue;
}

static int Callback_Resume(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	PlaySoundObject(18, SOUND_MODE_PLAY);
	return enum_ESCRETURN_continue;
}

static int Callback_IssueTracker(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	PlaySoundObject(18, SOUND_MODE_PLAY);
	Backend_OpenURL("https://github.com/DoukutsuRandamu/IssueTracker/issues");
	return CALLBACK_CONTINUE;
}

static int Callback_ModPage(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	std::string requestURL = "https://itch.io/api/1/ZNrFKYrCZpVWlPEe8p08KSrjTEIyi8kV2xecFHui/game/1403989/uploads";
	std::string downloadURL = "https://itch.io/api/1/ZNrFKYrCZpVWlPEe8p08KSrjTEIyi8kV2xecFHui/upload/5283825/download";
	std::string trueDownloadUrl = "";
	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;
	std::string title = GetCSE2Message("updates_title");
	bool errored = false;
	bool succeed = false;
	bool versionsFetched = false;
	bool needUpdate = false;
	std::string errorMessage = "";
	std::string version1Message = GetCSE2Message("updates_version_1");
	std::string version2Message = GetCSE2Message("updates_version_2");
	std::string needUpdateMessage = GetCSE2Message("updates_confirm");
	std::string noUpdateMessage = GetCSE2Message("updates_no_update");
	std::string exitMessage = GetCSE2Message("updates_cancel");

	std::string currentVersion;
	std::string itchVersion;
	int v1, v2, v3, v4;
	GetCompileVersion(&v1, &v2, &v3, &v4);
	currentVersion = std::to_string(v1) + "." + std::to_string(v2) + "." + std::to_string(v3);
	int wait = 0;

	// Draw title
	for (;;){
		// Get pressed keys
		GetTrg();
		PutBitmap4(&rcView, 0, 0, &rcView, SURFACE_ID_SCREEN_GRAB);
		PutBitmap3(&rcView, 0, 0, &rcView, SURFACE_ID_BLACK_OVERLAY);
		PutText((WINDOW_WIDTH / 2) - ((strlen(title.c_str()) * 5) / 2), 25, title.c_str(), RGB(0xFF, 0xFF, 0xFF));
		if (wait < 100){
			wait++;
			if (!Flip_SystemTask())
			{
				// Quit if window is closed
				return CALLBACK_EXIT;
			}
		} else {

		try {
			if (!errored && !succeed && !needUpdate){
				std::string response = client.Get(requestURL);
				if (response != ""){
					succeed = true;
					json respJson = json::parse(response);
					if (respJson.find("errors") == respJson.end())
					{
						json respJson1 = respJson["uploads"][0];
						json respJson2 = respJson1["build"];
						itchVersion = respJson2["user_version"].get<std::string>();
						if (itchVersion != currentVersion)
						{
							needUpdate = true;
							json downloadJson = json::parse(client.Get(downloadURL));
							trueDownloadUrl = downloadJson["url"];
						}
					} else {
						errorMessage = respJson["errors"][0];
						succeed = false;
						errored = true;
					}
				}
			}

		} catch (std::runtime_error err){
			errored = true;
			errorMessage = std::string(err.what());
		}
		if (errored){
			title = GetCSE2Message("updates_error");
			PutText((WINDOW_WIDTH / 2) - ((strlen(errorMessage.c_str()) * 5) / 2), 70, errorMessage.c_str(), RGB(0xFF, 0xFF, 0xFF));
		} else if (succeed){
			title = GetCSE2Message("updates_success");
			if (!versionsFetched){
				version1Message += currentVersion;
				version2Message += itchVersion;
				versionsFetched = true;
			}
			PutText((WINDOW_WIDTH / 2) - ((strlen(version1Message.c_str()) * 5) / 2), 70, version1Message.c_str(), RGB(0xFF, 0xFF, 0xFF));
			PutText((WINDOW_WIDTH / 2) - ((strlen(version2Message.c_str()) * 5) / 2), 84, version2Message.c_str(), RGB(0xFF, 0xFF, 0xFF));
			if (needUpdate){
				PutText((WINDOW_WIDTH / 2) - ((strlen(needUpdateMessage.c_str()) * 5) / 2), 100, needUpdateMessage.c_str(), RGB(0xFF, 0xFF, 0xFF));
			} else {
				PutText((WINDOW_WIDTH / 2) - ((strlen(noUpdateMessage.c_str()) * 5) / 2), 100, noUpdateMessage.c_str(), RGB(0xFF, 0xFF, 0xFF));
			}
			PutText((WINDOW_WIDTH / 2) - ((strlen(exitMessage.c_str()) * 5) / 2), 130, exitMessage.c_str(), RGB(0xFF, 0xFF, 0xFF));
		}

		if (gKeyTrg & gKeyOk){
			if (trueDownloadUrl != "" && needUpdate)
				Backend_OpenURL(trueDownloadUrl.c_str());
			break;
		}

		if (gKeyTrg & (gKeyCancel | KEY_PAUSE | KEY_ESCAPE)){
			break;
		}

		PutFramePerSecound();

		if (!Flip_SystemTask())
		{
			// Quit if window is closed
			return CALLBACK_EXIT;
		}
		}
	}

	PlaySoundObject(18, SOUND_MODE_PLAY);
	return CALLBACK_CONTINUE;
}

static int Callback_Reset(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	int return_value = PromptAreYouSure2();

	switch (return_value)
	{
		case 0:
			return_value = CALLBACK_CONTINUE;	// Go back to previous menu
			break;

		case 1:
			SetOrganyaFadeout();
			DeleteSave();
			restart_required = false;
			gGameMode = 3;
			gDaily = false;
			return_value = CALLBACK_RESET;	// Restart game
			break;
	}

	return return_value;
}

static int Callback_Reset2(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	int return_value = PromptAreYouSure3();

	switch (return_value)
	{
		case 0:
			return_value = CALLBACK_CONTINUE;	// Go back to previous menu
			break;

		case 1:
			SetOrganyaFadeout();
			DeleteSave();
			restart_required = false;
			gGameMode = 3;
			gDaily = false;
			return_value = CALLBACK_RESET;	// Restart game
			break;
	}

	return return_value;
}

static int Callback_StartDaily(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	int return_value = PromptAreYouSure4();

	switch (return_value)
	{
		case 0:
			return_value = CALLBACK_CONTINUE;	// Go back to previous menu
			break;

		case 1:
			SetOrganyaFadeout();
			DeleteSave();
			restart_required = false;
			gGameMode = 3;
			gDaily = true;
			return_value = CALLBACK_RESET;	// Restart game
			break;
	}

	return return_value;
}

static int Callback_Save(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	int return_value = PromptSaves();

	switch (return_value)
	{
		case 0:
			return_value = CALLBACK_CONTINUE;	// Go back to previous menu
			break;

		case 1:
			return_value = CALLBACK_RESET;	// Restart game and load the save
			break;
	}

	return return_value;
}

static int Callback_Save2(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	return CALLBACK_RESET;
}

static int Callback_Rankings(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	int return_value = PromptAreYouSure();

	switch (return_value)
	{
		case 0:
			return_value = CALLBACK_CONTINUE;	// Go back to previous menu
			break;

		case 1:
			SetOrganyaFadeout();
			return_value = CALLBACK_RANKS;	// Enter Rankings
			break;
	}

	return return_value;
}

static int Callback_Rankings2(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	SetOrganyaFadeout();
	BackupSurface(SURFACE_ID_SCREEN_GRAB, &grcGame);
	StartFadeOut(4);
	int wait = Backend_GetTicks();
	while (Backend_GetTicks() < wait + 1000)
	{
		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);
		ProcFade();
		PutFade();
		PutFramePerSecound();
		if (!Flip_SystemTask())
			return CALLBACK_EXIT;
	}
	return CALLBACK_RANKS;	// Enter Rankings
}

static int Callback_Quit(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	int return_value = PromptAreYouSure();

	switch (return_value)
	{
		case 0:
			return_value = CALLBACK_CONTINUE;	// Go back to previous menu
			break;

		case 1:
			return_value = CALLBACK_EXIT;	// Exit game
			break;
	}
	if (return_value == CALLBACK_EXIT){
	SetOrganyaFadeout();
	StartFadeOut(4);
	int wait = Backend_GetTicks();
	while (Backend_GetTicks() < wait + 1000)
	{
		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);
		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_BLACK_OVERLAY);
		ProcFade();
		PutFade();
		PutFramePerSecound();
		if (!Flip_SystemTask())
			return CALLBACK_EXIT;
	}
	}

	return return_value;
}

static int Callback_Quit2(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;
	
	SetOrganyaFadeout();
	BackupSurface(SURFACE_ID_SCREEN_GRAB, &grcGame);
	StartFadeOut(4);
	int wait = Backend_GetTicks();
	while (Backend_GetTicks() < wait + 1000)
	{
		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);
		ProcFade();
		PutFade();
		PutFramePerSecound();
		if (!Flip_SystemTask())
			return CALLBACK_EXIT;
	}

	return CALLBACK_EXIT;
}

int Call_Pause(void)
{
	mode = 0;
	Option options[] = {
		{{GetCSE2Message("title_resume"), ""}, {Callback_Resume, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("title_reset"), ""}, {Callback_Reset, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("title_reload"), ""}, {Callback_Save, NULL}, NULL, "", 0, !(gMC.cond != 0 && !gBL.flag && gStageNo != 64 && IsProfile() && !ChallengeEnabled("timer"))},
		{{GetCSE2Message("title_config"), ""}, {Callback_Configuration, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("title_options"), ""}, {Callback_Options, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("title_tutorial"), ""}, {Callback_Tutorial, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("title_rankings"), ""}, {Callback_Rankings, NULL}, NULL, "", 0, gRankings.size() == 0},
		{{GetCSE2Message("title_updates"), ""}, {Callback_ModPage, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("title_quit"), ""}, {Callback_Quit, NULL}, NULL, "", 0, false}
	};

	OptionsMenu options_menu = {
		GetCSE2Message("title_paused"),
		"",
		options,
		sizeof(options) / sizeof(options[0]),
		-14,
		false
	};

	int return_value = EnterOptionsMenu(&options_menu, 0);

	// Filter internal return values to something Cave Story can understand
	switch (return_value)
	{
		case CALLBACK_CONTINUE:
			return_value = enum_ESCRETURN_continue;
			break;

		case CALLBACK_RANKS:
			return_value = enum_ESCRETURN_ranks;
			break;

		case CALLBACK_RESET:
			return_value = enum_ESCRETURN_restart;
			break;

		case CALLBACK_EXIT:
			return_value = enum_ESCRETURN_exit;
			break;
	}

	gKeyTrg = gKey = 0;	// Avoid input-ghosting

	return return_value;
}

int Call_Title(void)
{
	mode = 1;
	Option options[] = {
		{{GetCSE2Message("title_character"), GetCSE2Message("title_new_game")}, {Callback_Character2, Callback_Reset2}, NULL, "", 0, false},
		{{GetCSE2Message("title_load_game"), ""}, {Callback_Save2, NULL}, NULL, "", 0, !(IsProfile() && !ChallengeEnabled("timer"))},
		{{GetCSE2Message("title_new_daily"), ""}, {Callback_StartDaily, NULL}, NULL, "", 0, 
			fs::exists(ScoreFilePath(GetDailySeed()))},
		{{GetCSE2Message("title_options"), GetCSE2Message("title_config")}, {Callback_Options2, Callback_Configuration2}, NULL, "", 0, false},
		{{GetCSE2Message("title_rankings"), ""}, {Callback_Rankings2, NULL}, NULL, "", 0, gRankings.size() == 0},
		{{GetCSE2Message("title_resources"), ""}, {Callback_Resources, NULL}, NULL, "", 0, false},
		{{IsBeta() ? GetCSE2Message("title_issues") : GetCSE2Message("title_updates"), ""}, {IsBeta() ? Callback_IssueTracker : Callback_ModPage, NULL}, NULL, "", 0, false},
		{{GetCSE2Message("title_exit"), ""}, {Callback_Quit2, NULL}, NULL, "", 0, false}
	};

	OptionsMenu options_menu = {
		"",
		"",
		options,
		sizeof(options) / sizeof(options[0]),
		-14,
		false
	};

	int return_value = EnterOptionsMenuWithAlternates(&options_menu, 0);

	// Filter internal return values to something Cave Story can understand
	switch (return_value)
	{
		case CALLBACK_CONTINUE:
			return_value = enum_ESCRETURN_continue;
			break;

		case CALLBACK_RANKS:
			return_value = enum_ESCRETURN_ranks;
			break;

		case CALLBACK_RESET:
			return_value = enum_ESCRETURN_restart;
			break;

		case CALLBACK_EXIT:
			return_value = enum_ESCRETURN_exit;
			break;
	}

	gKeyTrg = gKey = 0;	// Avoid input-ghosting

	return return_value;
}
