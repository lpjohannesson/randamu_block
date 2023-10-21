#include "ArmsItem.h"
#include <iterator>
#include <algorithm>

#include <cstring>
#include <string>
#include <map>
#include <vector>

#include "Back.h"
#include "Draw.h"
#include "Escape.h"
#include "Game.h"
#include "Generic.h"
#include "KeyControl.h"
#include "Main.h"
#include "MyChar.h"
#include "MycParam.h"
#include "Organya.h"
#include "Pause.h"
#include "Score.h"
#include "SelStage.h"
#include "Shoot.h"
#include "Stage.h"
#include "Sound.h"
#include "TextScr.h"

int selectedRank;

int gRankingTitleY;

/// True if we're in the items section of the inventory (not in the weapons section) (only relevant when the inventory is open)
bool gRankingActive;

int gSelectedArmsRanking;

int gSelectedItemRanking;

bool GetNPCFlagRank(int a)
{
	auto flagStorage = gRankings[selectedRank]["flags"].get<std::vector<int>>();
	return std::find(flagStorage.begin(), flagStorage.end(), a) != flagStorage.end();
}

void PutActiveArmRank(void)
{
	int x;
	int a;
	Rect rect = {0, 0, 0, 16};
		// Get X position to draw at
		x = 16;

		if (x < 8)
			x += 48 + (gSelectedArmsRanking * 16);
		else if (x >= 24)
			x += 48;

		if (x >= 72 + ((gSelectedArmsRanking - 1) * 16))
			x -= 48 + (gSelectedArmsRanking * 16);
		if (x < 72 && x >= 24)
			x -= 48;

		// Draw icon
		rect.left = gRankings[selectedRank]["arms"][gSelectedArmsRanking].get<ARMS>().code * 16;
		rect.right = rect.left + 16;
		if ((GetNPCFlagRank(Flags::MISERY_CHARACTER) || GetNPCFlagRank(Flags::MAHIN_CHARACTER)) && gRankings[selectedRank]["arms"][gSelectedArmsRanking].get<ARMS>().code == 13){
			rect.left = 10 * 16;
			rect.right = rect.left + 16;
		}
		if ((GetNPCFlagRank(Flags::BRACKET_CHARACTER) || GetNPCFlagRank(Flags::MAHIN_CHARACTER)) && gRankings[selectedRank]["arms"][gSelectedArmsRanking].get<ARMS>().code == 8){
			rect.left = 6 * 16;
			rect.right = rect.left + 16;
		}
		PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(x), PixelToScreenCoord(16), &rect, SURFACE_ID_ARMS_IMAGE);
}

void PutMyLifeRank()
{
	Rect rcCase = {24, 40, 232, 48};
	Rect rcDifficulty = {184, 96, 208, 104};
	if (gRankings[selectedRank].contains("difficulty")){
	rcDifficulty.top += gRankings[selectedRank]["difficulty"].get<int>()*8;
	rcDifficulty.bottom += gRankings[selectedRank]["difficulty"].get<int>()*8;
	}
	Rect rcLife = {193, 24, 193, 32};
	Rect rcBr = {193, 32, 193, 40};
	if (GetNPCFlagRank(Flags::HUNTER_CHARACTER)){
		rcCase.top += 80;
		rcCase.bottom += 80;
		rcLife.top += 8;
		rcLife.bottom += 8;
	}

	// Draw bar
	rcCase.right = 64;
	rcLife.right = 192 + (((gRankings[selectedRank]["life"].get<int>() * 40)) / gRankings[selectedRank]["max_life"].get<int>()) - 1;

	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(16), PixelToScreenCoord(40), &rcDifficulty, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(40), PixelToScreenCoord(40), &rcCase, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(40), PixelToScreenCoord(40), &rcLife, SURFACE_ID_TEXT_BOX);
	PutNumber4(8, 40, gRankings[selectedRank]["life"].get<int>(), false);
}

void PutMyScoreRank(int y){
	Rect rcStars = {96, 152, 137, 160};
	Rect rcStairs = {113, 112, 141, 121};
	Rect rcPoints = {81, 73, 109, 80};
	Rect rcMiracle = {228, 2, 252, 16};

	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(16), PixelToScreenCoord(2), &rcStars, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(16), PixelToScreenCoord(48), &rcStairs, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(83 + 230), PixelToScreenCoord(5 + y), &rcPoints, SURFACE_ID_TEXT_BOX);

	PutNumber4(49, 3, selectedRank+1, true);
	PutNumber4(31, 48, gRankings[selectedRank]["depth"].get<int>(), true);
	PutNumber9(116 + 230, 5, gRankings[selectedRank]["score"].get<int>(), true);
}

void PutArmsEnergyRank()
{
	static unsigned char add_flash;

	Rect rcPer = {72, 48, 80, 56};
	Rect rcLv = {144, 80, 160, 88};
	if (gRankings[selectedRank].contains("difficulty")){
		rcLv.left += gRankings[selectedRank]["difficulty"].get<int>() * 16;
		rcLv.right += gRankings[selectedRank]["difficulty"].get<int>() * 16;
	}
	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	Rect rcNone = {80, 48, 96, 56};

	gArmsEnergyX = 16;

	// Draw max ammo
	if (gRankings[selectedRank]["arms"][gSelectedArmsRanking].get<ARMS>().max_num)
	{
		PutNumber4(gArmsEnergyX + 32, 16, gRankings[selectedRank]["arms"][gSelectedArmsRanking].get<ARMS>().num, false);
		PutNumber4(gArmsEnergyX + 32, 24, gRankings[selectedRank]["arms"][gSelectedArmsRanking].get<ARMS>().max_num, false);
	}
	else
	{
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX + 48), PixelToScreenCoord(16), &rcNone, SURFACE_ID_TEXT_BOX);
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX + 48), PixelToScreenCoord(24), &rcNone, SURFACE_ID_TEXT_BOX);
	}

	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX + 32), PixelToScreenCoord(24), &rcPer, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX), PixelToScreenCoord(32), &rcLv, SURFACE_ID_TEXT_BOX);
	PutNumber4(gArmsEnergyX - 8, 32, gRankings[selectedRank]["arms"][gSelectedArmsRanking].get<ARMS>().level, false);

	Rect rcExpBox = {0, 72, 40, 80};
	Rect rcExpVal = {0, 80, 0, 88};
	Rect rcExpMax = {40, 72, 80, 80};
	Rect rcExpFlash = {40, 80, 80, 88};

	int lv = gRankings[selectedRank]["arms"][gSelectedArmsRanking].get<ARMS>().level - 1;

#ifdef FIX_BUGS
	// When the player has no weapons, the default level is 0, which becomes -1.
	// Catch it, and set it to 0 instead, so the following array-accesses aren't
	// out-of-bounds.
	if (lv < 0)
		lv = 0;
#endif

	int arms_code = gRankings[selectedRank]["arms"][gSelectedArmsRanking].get<ARMS>().code;
	int exp_now = gRankings[selectedRank]["arms"][gSelectedArmsRanking].get<ARMS>().exp;
	int exp_next = gArmsLevelTable[arms_code].exp[lv];
	int max_lv = 2;
	if (GetNPCFlagRank(Flags::SERENITY_CHARACTER) || GetNPCFlagRank(Flags::MAHIN_CHARACTER)) max_lv = 3;

	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX + 24), PixelToScreenCoord(32), &rcExpBox, SURFACE_ID_TEXT_BOX);

	if (lv == max_lv && gRankings[selectedRank]["arms"][gSelectedArmsRanking].get<ARMS>().exp == gArmsLevelTable[arms_code].exp[lv])
	{
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX + 24), PixelToScreenCoord(32), &rcExpMax, SURFACE_ID_TEXT_BOX);
	}
	else
	{
		if (exp_next != 0)
			rcExpVal.right += (exp_now * 40) / exp_next;
		else
			rcExpVal.right = 0;

		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(gArmsEnergyX + 24), PixelToScreenCoord(32), &rcExpVal, SURFACE_ID_TEXT_BOX);
	}
}

/// Update the inventory cursor
void MoveRankingCursor(void)
{
	bool bChange;

	// Compute the current amount of weapons and items
	int arms_num = 0;
	int item_num = 0;
	while (gRankings[selectedRank]["arms"][arms_num].get<ARMS>().code != 0)
		++arms_num;
	while (gRankings[selectedRank]["items"][item_num].get<ITEM>().code != 0)
		++item_num;

	if (arms_num == 0 && item_num == 0)
		return;	// Empty inventory

	// True if we're currently changing cursor position
	bChange = false;

	if (!gRankingActive)
	{
		// Handle selected weapon
		if (gKeyTrg & gKeyLeft)
		{
			if (gSelectedArmsRanking % 6 == 0)
				gSelectedArmsRanking += 5;
			else
				gSelectedArmsRanking -= 1;
			bChange = true;
		}

		if (gKeyTrg & gKeyRight)
		{
			if (gSelectedItemRanking == arms_num - 1)
				gSelectedArmsRanking = (gSelectedArmsRanking / 6) * 6;	// Round down to multiple of 6
			else if (gSelectedArmsRanking% 6 == 5)
				gSelectedArmsRanking -= 5;	// Loop around row
			else
				gSelectedArmsRanking += 1;
			bChange = true;
		}

		if (gKeyTrg & gKeyUp)
		{
			if (gSelectedArmsRanking / 6 != 0)
				gSelectedArmsRanking -= 6;

			bChange = true;
		}

		if (gKeyTrg & gKeyDown)
		{
			if (gSelectedArmsRanking / 6 == (arms_num - 1) / 6)
				gRankingActive = true;	// We're on the last row, transition to items
			else
				gSelectedArmsRanking += 6;

			bChange = true;
		}

		// // Loop around gSelectedArmsRanking if needed
		if (gSelectedArmsRanking < 0)
			gSelectedArmsRanking = arms_num - 1;

		if (gSelectedArmsRanking > arms_num - 1)
			gSelectedArmsRanking = 0;
	}
	else
	{
		// Handle selected item
		if (gKeyTrg & gKeyLeft)
		{
			if (gSelectedItemRanking % 7 == 0)
				gSelectedItemRanking += 6;
			else
				gSelectedItemRanking -= 1;

			bChange = true;
		}

		if (gKeyTrg & gKeyRight)
		{
			if (gSelectedItemRanking == item_num - 1)
				gSelectedItemRanking = (gSelectedItemRanking / 7) * 7;	// Round down to multiple of 6
			else if (gSelectedItemRanking % 7 == 6)
				gSelectedItemRanking -= 6;	// Loop around row
			else
				gSelectedItemRanking += 1;

			bChange = true;
		}

		if (gKeyTrg & gKeyUp)
		{
			if (gSelectedItemRanking / 7 == 0)
				gRankingActive = false;	// We're on the first row, transition to weapons
			else
				gSelectedItemRanking -= 7;

			bChange = true;
		}

		if (gKeyTrg & gKeyDown)
		{
			if (gSelectedItemRanking / 7 == (item_num - 1) / 6)
				gRankingActive = false;	// We're on the last row, transition to weapons
			else
				gSelectedItemRanking += 7;

			bChange = true;
		}

		if (gSelectedItemRanking >= item_num)
			gSelectedItemRanking = item_num - 1;	// Don't allow selecting a non-existing item
	}

	if (bChange)
	{
		if (gRankingActive == false)
		{
			// Switch to a weapon
			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			if (arms_num != 0)
				StartTextScript(1000 + gRankings[selectedRank]["arms"][gSelectedArmsRanking].get<ARMS>().code);
			else
				StartTextScript(1000);
		}
		else
		{
			// Switch to an item
			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			if (item_num != 0){
				if (gRankings[selectedRank]["items"][gSelectedItemRanking].get<ITEM>().code < 100){
					StartTextScript(5000 + gRankings[selectedRank]["items"][gSelectedItemRanking].get<ITEM>().code);
				} else {
					StartTextScript(1000 + (gRankings[selectedRank]["items"][gSelectedItemRanking].get<ITEM>().code - 100));
				}
			}
			else
				StartTextScript(5000);
		}
	}
}

bool fast_run;

/// Draw the inventory
void PutRankingObject(void)
{
	static unsigned int flash;

	int i;

	/// Rect for the current weapon
	Rect rcArms;

	/// Rect for the current item
	Rect rcItem;

	/// Probably the rect for the slash
	Rect rcPer = {72, 48, 80, 56};

	/// Rect for when there is no ammo (double dashes)
	Rect rcNone = {80, 48, 96, 56};

	/// Rect for the "Lv" text!
	Rect rcLv = {80, 80, 96, 88};

	Rect rcEquip = {176, 101, 184, 108};

	/// Final rect drawn on the screen
	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	/// Cursor rect array for weapons, element [1] being for when the cursor is flashing
	Rect rcCur1[2] = {{0, 88, 40, 117}, {40, 88, 80, 117}};

	/// Cursor rect array for items, element [1] being for when the cursor is flashing
	Rect rcCur2[2] = {{80, 88, 112, 104}, {80, 104, 112, 120}};

	Rect rcTitle1 = {80, 48, 144, 56};
	Rect rcTitle2 = {80, 56, 144, 64};
	Rect rcBoxTop = {0, 0, 244, 8};
	Rect rcBoxBody = {0, 8, 244, 16};
	Rect rcBoxBottom = {0, 16, 244, 24};

	static unsigned int anime = 0;

	// Draw box
	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 122), PixelToScreenCoord((WINDOW_HEIGHT / 2) - 112), &rcBoxTop, SURFACE_ID_TEXT_BOX);
	for (i = 1; i < 21; ++i)
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 122), PixelToScreenCoord(((WINDOW_HEIGHT / 2) - 120) + ((i + 1) * 8)), &rcBoxBody, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 122), PixelToScreenCoord(((WINDOW_HEIGHT / 2) - 120) + ((i + 1) * 8)), &rcBoxBottom, SURFACE_ID_TEXT_BOX);

	// Move titles
	if (gRankingTitleY > (WINDOW_HEIGHT / 2) - 104)
		--gRankingTitleY;

	// Draw titles
	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 112), PixelToScreenCoord(gRankingTitleY), &rcTitle1, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 112), PixelToScreenCoord(gRankingTitleY + 68), &rcTitle2, SURFACE_ID_TEXT_BOX);

	// Draw arms cursor
	++flash;

	if (gRankingActive == false)
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((gSelectedArmsRanking % 6) * 40) + (WINDOW_WIDTH / 2) - 117), PixelToScreenCoord((WINDOW_HEIGHT / 2) - 96 + (gSelectedArmsRanking / 6) * 30), &rcCur1[(flash / 2) % 2], SURFACE_ID_TEXT_BOX);
	else
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((gSelectedArmsRanking % 6) * 40) + (WINDOW_WIDTH / 2) - 117), PixelToScreenCoord((WINDOW_HEIGHT / 2) - 96 + (gSelectedArmsRanking / 6) * 30), &rcCur1[1], SURFACE_ID_TEXT_BOX);

	Rect rcMyChar[4] = {
	{0, 16, 16, 32},
	{16, 16, 32, 32},
	{0, 16, 16, 32},
	{32, 16, 48, 32},
	};
	if (gRankings[selectedRank]["mim"] > 0)
	for (int i = 0; i < 4; i++){
		rcMyChar[i].top += 32 * gRankings[selectedRank]["mim"].get<int>();
		rcMyChar[i].bottom += 32 * gRankings[selectedRank]["mim"].get<int>();
	}

	// Draw weapons
	for (i = 0; i < ARMS_MAX; ++i)
	{
		if (gRankings[selectedRank]["arms"][i].get<ARMS>().code == 0)
			break;	// Invalid weapon

		// Get icon rect for next weapon
		rcArms.left = (gRankings[selectedRank]["arms"][i].get<ARMS>().code) * 16;
		rcArms.right = rcArms.left + 16;
		rcArms.top = 0;
		rcArms.bottom = rcArms.top + 16;
		if ((GetNPCFlagRank(Flags::MISERY_CHARACTER) || GetNPCFlagRank(Flags::MAHIN_CHARACTER)) && gRankings[selectedRank]["arms"][i].get<ARMS>().code == 13){
			rcArms.left = (10) * 16;
			rcArms.right = rcArms.left + 16;
			rcArms.top = 0;
			rcArms.bottom = rcArms.top + 16;
		}
		if ((GetNPCFlagRank(Flags::BRACKET_CHARACTER) || GetNPCFlagRank(Flags::MAHIN_CHARACTER)) && gRankings[selectedRank]["arms"][i].get<ARMS>().code == 8){
			rcArms.left = 6 * 16;
			rcArms.right = rcArms.left + 16;
			rcArms.top = 0;
			rcArms.bottom = rcArms.top + 16;
		}
		int armsYPos = (WINDOW_HEIGHT / 2) - 96 + (i / 6) * 30;
		int slashYPos = (WINDOW_HEIGHT / 2) - 80 + (i / 6) * 30;
		int lvYPos = (WINDOW_HEIGHT / 2) - 80 + (i / 6) * 30;
		int xPos = ((i % 6) * 40) + (WINDOW_WIDTH / 2) - 117;

		// Draw the icon, slash and "Lv"
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(xPos), PixelToScreenCoord(armsYPos), &rcArms, SURFACE_ID_ARMS_IMAGE);
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(xPos+4), PixelToScreenCoord(slashYPos), &rcPer, SURFACE_ID_TEXT_BOX);
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(xPos + 17), PixelToScreenCoord(armsYPos+1), &rcLv, SURFACE_ID_TEXT_BOX);
		PutNumber4(xPos + 6, armsYPos+1, gRankings[selectedRank]["arms"][i].get<ARMS>().level, false);

		// Draw ammo
		if (gRankings[selectedRank]["arms"][i].get<ARMS>().max_num)
		{
			PutNumber4(xPos+7, armsYPos + 11, gRankings[selectedRank]["arms"][i].get<ARMS>().num, false);
			PutNumber4(xPos+7, armsYPos + 19, gRankings[selectedRank]["arms"][i].get<ARMS>().max_num, false);
		}
		else
		{
			// Weapon doesn't use ammunition
			PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(xPos+18), PixelToScreenCoord(armsYPos + 12), &rcNone, SURFACE_ID_TEXT_BOX);
			PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(xPos+18), PixelToScreenCoord(armsYPos + 21), &rcNone, SURFACE_ID_TEXT_BOX);
		}
	}

	// Draw items cursor
	if (gRankingActive == true)
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((gSelectedItemRanking % 7) * 32) + (WINDOW_WIDTH / 2) - 112), PixelToScreenCoord(((gSelectedItemRanking / 7) * 16) + (WINDOW_HEIGHT / 2) - 28), &rcCur2[flash / 2 % 2], SURFACE_ID_TEXT_BOX);
	else
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((gSelectedItemRanking % 7) * 32) + (WINDOW_WIDTH / 2) - 112), PixelToScreenCoord(((gSelectedItemRanking / 7) * 16) + (WINDOW_HEIGHT / 2) - 28), &rcCur2[1], SURFACE_ID_TEXT_BOX);

	for (i = 0; i < ITEM_MAX; ++i)
	{
		if (gRankings[selectedRank]["items"][i].get<ITEM>().code == 0)
			break;	// Invalid item

		int offsetX = 0;
		int offsetY = 0;

		// Get rect for next item
		if (gRankings[selectedRank]["items"][i].get<ITEM>().code < 100){
			rcItem.left = (gRankings[selectedRank]["items"][i].get<ITEM>().code % 8) * 32;
			rcItem.right = rcItem.left + 32;
			rcItem.top = (gRankings[selectedRank]["items"][i].get<ITEM>().code / 8) * 16;
			rcItem.bottom = rcItem.top + 16;
		}
		else
		{
			offsetX = 8;
			rcItem.left = (gRankings[selectedRank]["items"][i].get<ITEM>().code - 100) * 16;
			rcItem.right = rcItem.left + 16;
			rcItem.top = 0;
			rcItem.bottom = rcItem.top + 16;
			if ((GetNPCFlagRank(Flags::MISERY_CHARACTER) || GetNPCFlagRank(Flags::MAHIN_CHARACTER)) && gRankings[selectedRank]["items"][i].get<ITEM>().code == 113)
			{
				rcItem.left = (10) * 16;
				rcItem.right = rcItem.left + 16;
				rcItem.top = 0 * 16;
				rcItem.bottom = rcItem.top + 16;
			}
			if ((GetNPCFlagRank(Flags::BRACKET_CHARACTER) || GetNPCFlagRank(Flags::MAHIN_CHARACTER)) && gRankings[selectedRank]["items"][i].get<ITEM>().code == 108)
			{
				rcItem.left = (6) * 16;
				rcItem.right = rcItem.left + 16;
				rcItem.top = 0 * 16;
				rcItem.bottom = rcItem.top + 16;
			}
		}

		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((i % 7) * 32 + offsetX) + (WINDOW_WIDTH / 2) - 112), PixelToScreenCoord(((i / 7) * 16 + offsetY) + (WINDOW_HEIGHT / 2) - 28), &rcItem, 
			gRankings[selectedRank]["items"][i].get<ITEM>().code > 100 ? SURFACE_ID_ARMS_IMAGE : SURFACE_ID_ITEM_IMAGE);

		if (equipMap.find(gRankings[selectedRank]["items"][i].get<ITEM>().code) != std::end(equipMap) && GetNPCFlagRank(equipMap.at(gRankings[selectedRank]["items"][i].get<ITEM>().code))){
			PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((i % 7) * 32) + (WINDOW_WIDTH / 2) - 112 + (rcItem.right - rcItem.left) - (rcEquip.right - rcEquip.left)), PixelToScreenCoord(((i / 7) * 16) + (WINDOW_HEIGHT / 2) - 28), &rcEquip, SURFACE_ID_TEXT_BOX);
		}
	}

	int maxDepth = (fast_run ? 6 : 10);

	if (++anime >= 40)
			anime = 0;

		if (gRankings[selectedRank]["life"].get<int>() != 0 && !GetNPCFlagRank(Flags::DOCTOR_PHASE_2_DONE)){
			anime = 0;
			PutBitmap3_Flip(&rcView, PixelToScreenCoord(12), PixelToScreenCoord(65), &rcMyChar[anime / 10 % 4], 0xaca6ff, SURFACE_ID_MY_CHAR);
		}
		else
			PutBitmap3(&rcView, PixelToScreenCoord(12), PixelToScreenCoord(65), &rcMyChar[anime / 10 % 4], SURFACE_ID_MY_CHAR);

		if (gRankings[selectedRank]["life"].get<int>() == 0){
			if (gRankings[selectedRank].contains("death")){
				int reason = gRankings[selectedRank]["death"].get<int>();
				Rect death;
				death.left = (reason % 13)*24;
				death.right = death.left+24;
				death.top = (reason / 13)*24;
				death.bottom = death.top+24;
				PutBitmap3(&rcView, PixelToScreenCoord(48), PixelToScreenCoord(62), &death, SURFACE_ID_DEATH);
			}
		}

		int tiles[] = {
			0, 1, 57, 43,
			72, 73, 68, 45,
			69, 33, 13, 70,
			74, 51, 41, 66
		};
		if (gRankings[selectedRank]["depth"].get<int>() != maxDepth){
			if (gRankings[selectedRank]["depth"].get<int>() > maxDepth){
				Rect crystal = {76, 64, 86, 80};
				PutBitmap3(&rcView, PixelToScreenCoord(29), PixelToScreenCoord(65+8), &crystal, SURFACE_ID_ITEM_IMAGE);
			}
			for (int i = 0; i < 16; i++){
				Rect rect;
				int tileX = (tiles[i] % 16);
				int tileY = (tiles[i] / 16);
				rect.left = tileX * 16;
				rect.top = tileY * 16;
				rect.right = rect.left + 16;
				rect.bottom = rect.top + 16;
				PutBitmap3(&rcView, PixelToScreenCoord(((i % 4) * 16) + 12), PixelToScreenCoord(((i / 4) * 16) + 85+8), &rect, SURFACE_ID_LEVEL_TILESET);
			}
		} else {
			Rect rect = {58, 28, 122, 92};
			PutBitmap3(&rcView, PixelToScreenCoord(12), PixelToScreenCoord(85+8), &rect, SURFACE_ID_CREDITS_IMAGE);
		}
		if (gRankings[selectedRank]["depth"].get<int>() == maxDepth*2){
			Rect rect = {38, 20, 102, 84};
			PutBitmap3(&rcView, PixelToScreenCoord(12), PixelToScreenCoord(85+8), &rect, SURFACE_ID_CREDITS_IMAGE_2);
		}

		Rect rcTime = {112, 104, 120, 112};
		Rect rcColon = {152, 152, 157, 160};

		PutBitmap3(&rcView, PixelToScreenCoord(12), PixelToScreenCoord(152+8), &rcTime, SURFACE_ID_TEXT_BOX);
		int timePlayed = gRankings[selectedRank]["time"].get<int>();
		PutNumber4(9, 152+8, timePlayed / 216000, true);
		PutBitmap3(&rcView, PixelToScreenCoord(41), PixelToScreenCoord(152+8), &rcColon, SURFACE_ID_TEXT_BOX);
		PutNumber4(30, 152+8, (timePlayed / 3600) % 60, true);
		PutBitmap3(&rcView, PixelToScreenCoord(62), PixelToScreenCoord(152+8), &rcColon, SURFACE_ID_TEXT_BOX);
		PutNumber4(51, 152+8, (timePlayed / 60) % 60, true);
}

int RankingLoop(void)
{

	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	Rect rcHighScore = {0, 144, 80, 152};
	Rect rcSeed = {96, 144, 128, 152};
	int tile = gRankings[selectedRank]["tileset"].get<int>();

	std::string path_tileset = "Stage/PrtCave";
	path_tileset += (char)tile;
	ReloadBitmap_File(path_tileset.c_str(), SURFACE_ID_LEVEL_TILESET);

	// Load the inventory script
	LoadTextScript2("ArmsItem.tsc");

	gRankingTitleY = (WINDOW_HEIGHT / 2) - 96;

	if (gRankings[selectedRank].contains("fast_run")){
		if (gRankings[selectedRank]["fast_run"].get<bool>())
			fast_run = true;
		else
			fast_run = false;
	} else
		fast_run = false;

	// Put the cursor on the first weapon
	gRankingActive = false;
	gSelectedItemRanking = 0;

	// Compute current amount of weapons
	int arms_num = 0;
	while (gRankings[selectedRank]["arms"][arms_num].get<ARMS>().code != 0)
		++arms_num;

	if (arms_num != 0)
		StartTextScript(1000 + gRankings[selectedRank]["arms"][gSelectedArmsRanking].get<ARMS>().code);
	else
		StartTextScript(5000 + gRankings[selectedRank]["items"][gSelectedItemRanking].get<ITEM>().code);

	InitBack("bkChtulhu", 7);
	ChangeMusic(MUS_DARKENED_SKY);

	for (;;)
	{
		GetTrg();

		if (gKeyTrg & (gKeyCancel | KEY_PAUSE | KEY_ESCAPE))
		{
			gKeyTrg = gKey = 0;
			SetOrganyaFadeout();
			return enum_ESCRETURN_restart;	// Go to game intro
		}

		if (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED)
			MoveRankingCursor();

		if (gKey & gKeyAbility)
		{
			if (gKeyTrg & gKeyArmsRev)
			{
				if (selectedRank > 10)
				{
					selectedRank -= 10;
				}
				else
				{
					selectedRank = gRankings.size() - 1;
				}
				PlaySoundObject(4, SOUND_MODE_PLAY);
				return enum_ESCRETURN_continue;
			}
			if (gKeyTrg & gKeyArms)
			{
				if (selectedRank < gRankings.size() - 10)
				{
					selectedRank += 10;
				}
				else
				{
					selectedRank = 0;
				}
				PlaySoundObject(4, SOUND_MODE_PLAY);
				return enum_ESCRETURN_continue;
			}
		}
		else
		{
			if (gKeyTrg & gKeyArmsRev)
			{
				if (selectedRank > 0)
				{
					selectedRank--;
				}
				else
				{
					selectedRank = gRankings.size() - 1;
				}
				PlaySoundObject(4, SOUND_MODE_PLAY);
				return enum_ESCRETURN_continue;
			}
			if (gKeyTrg & gKeyArms)
			{
				if (selectedRank < gRankings.size() - 1)
				{
					selectedRank++;
				}
				else
				{
					selectedRank = 0;
				}
				PlaySoundObject(4, SOUND_MODE_PLAY);
				return enum_ESCRETURN_continue;
			}
		}

		switch (TextScriptProc())
		{
			case enum_ESCRETURN_exit:
				return enum_ESCRETURN_exit;	// Quit game

			case enum_ESCRETURN_restart:
				return enum_ESCRETURN_restart;	// Go to game intro
		}

		// Get currently displayed image
		PutBack(0, 0);
		ActBack();
		PutBitmap3(&rcView, 0, 0, &rcView, SURFACE_ID_BLACK_OVERLAY);
		PutMyLifeRank();
		PutArmsEnergyRank();
		PutActiveArmRank();
		PutMyScoreRank(-2);
		PutRankingObject();
		PutTextScript();
		PutFramePerSecound();
		int baseY = 48;
			PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(WINDOW_WIDTH - 88), PixelToScreenCoord(WINDOW_HEIGHT - baseY), &rcSeed, SURFACE_ID_TEXT_BOX);
			PutNumber10(WINDOW_WIDTH - 88, WINDOW_HEIGHT - baseY + 8, gRankings[selectedRank]["seed"], true);
		
		PutText(WINDOW_WIDTH - 90, 40, GetCSE2Message("rankings_scroll_right"), RGB(0xFF, 0xFF, 0xFF));
		PutText(WINDOW_WIDTH - 90, 54, GetCSE2Message("rankings_scroll_left"), RGB(0xFF, 0xFF, 0xFF));
		PutText(WINDOW_WIDTH - 90, 68, GetCSE2Message("rankings_scroll"), RGB(0xFF, 0xFF, 0xFF));
		PutText(WINDOW_WIDTH - 90, 96, GetCSE2Message("rankings_scroll_fast_1"), RGB(0xFF, 0xFF, 0xFF));
		PutText(WINDOW_WIDTH - 90, 110, GetCSE2Message("rankings_scroll_fast_2"), RGB(0xFF, 0xFF, 0xFF));
		PutText(WINDOW_WIDTH - 93, 138, GetCSE2Message("rankings_scroll_close_1"), RGB(0xFF, 0xFF, 0xFF));
		PutText(WINDOW_WIDTH - 90, 152, GetCSE2Message("rankings_scroll_close_2"), RGB(0xFF, 0xFF, 0xFF));

		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;	// Quit game
	}

	// Resume original script
	return enum_ESCRETURN_restart;	// Go to game
}
