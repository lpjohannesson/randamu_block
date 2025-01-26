#include "ArmsItem.h"
#include <iterator>
#include <algorithm>

#include <cstring>
#include <string>
#include <map>
#include <vector>

#include "CommonDefines.h"
#include "Draw.h"
#include "Escape.h"
#include "Game.h"
#include "Generic.h"
#include "Flags.h"
#include "KeyControl.h"
#include "Log.h"
#include "Main.h"
#include "MyChar.h"
#include "MycParam.h"
#include "Pause.h"
#include "Score.h"
#include "SelStage.h"
#include "Shoot.h"
#include "Sound.h"
#include "Stage.h"
#include "TextScr.h"

ARMS gArmsData[ARMS_MAX];
ITEM gItemData[ITEM_MAX];

int gSelectedArms;
int gSelectedItem;

int gCampTitleY;

/// True if we're in the items section of the inventory (not in the weapons section) (only relevant when the inventory is open)
bool gCampActive;

int gArmsEnergyX = 16;

void to_json(json& j, const ITEM& i){
	j = json{ {"code", i.code}, {"quantity", i.quantity}, {"wepLevel", i.level}, {"wepXP", i.xp}, {"wepCurAmmo", i.ammo} };
}

void from_json(const json& j, ITEM& i) {
    j.at("code").get_to(i.code);

	if (j.contains("quantity"))
    	j.at("quantity").get_to(i.quantity);
	else
		i.quantity = 0;

	if (j.contains("wepLevel"))
    	j.at("wepLevel").get_to(i.level);
	else
		i.level = 1;

	if (j.contains("wepXP"))
    	j.at("wepXP").get_to(i.xp);
	else
		i.xp = 0;

	if (j.contains("wepCurAmmo"))
    	j.at("wepCurAmmo").get_to(i.ammo);
	else
		i.ammo = i.quantity;
}

void ClearArmsData(void)
{
#ifdef FIX_BUGS
	gSelectedArms = 0; // Should probably be done in order to avoid potential problems with the selected weapon being invalid (like is done in SubArmsData)
#endif
	gArmsEnergyX = 32;
	memset(gArmsData, 0, sizeof(gArmsData));
}

void ClearItemData(void)
{
	memset(gItemData, 0, sizeof(gItemData));
}

std::map<int, EQUIP_DEFINITION> equipMap;

std::vector<std::pair<int, int>> miracleList = {
	//super booster
	{23, 44},
	//diving suit
	{21, 28},
	//shield of resilience
	{31, 52},
	//augmented man
	{32, 55},
	//cape of shadows
	{4, 41},
	//sacred aegis
	{47, 33},
	//energy crystal
	{20, 45},
	//terra boots
	{40, 56},
	//badge of love
	{35, 39},
	//universal catalyst
	{50, 51},
	//amphibian medal
	{36, 57},
	//high-octane tank
	{48, 49},
	//discordian visor
	{53, 58},
	//winter clock
	{22, 37},
	//flaming booster
	{48, 4},
	//shield of time
	{33, 37},
	//little man's underwear
	{35, 32},
	//nuclear stomper
	{46, 47},
	//omniquick sneakers
	{40, 29},
	//freeze star
	{51, 22},
	//military armament
	{72, 55}
};

std::vector<int> miracleWeaponryList = {
	//hellsweep
	17,
	//gatling gun
	4,
	//frontier buster
	1
};

bool AddArmsData(long code, long max_num, long exp, long num, long level)
{
	if (ChallengeEnabled("no_placing") && code == 2)
		return false;
	if (ChallengeEnabled("no_breaking") && code == 12)
		return false;

	// Search for code
	int i = 0;
	while (i < ARMS_MAX)
	{
		if (gArmsData[i].code == code)
			break;	// Found identical

		if (gArmsData[i].code == 0)
			break;	// Found free slot

		++i;
	}

	if (i == 12){
		return AddItemData(code + 100, max_num, level, exp, num);	// No space left
	}

	if (gArmsData[i].code == 0)
	{
		// Initialize new weapon
		memset(&gArmsData[i], 0, sizeof(ARMS));
		gArmsData[i].level = 1;
	}

	// Set weapon and ammo
	gArmsData[i].code = code;
	if ((code == 2 || code == 12)){
		if (CheckFlagTrait(FlagTraits::MINING_STORAGE_BOOST))
			max_num *= std::max(0.0f, GetFlagTraitData<float>(FlagTraits::MINING_STORAGE_BOOST, "modifier"));
		if (CheckFlagTrait(FlagTraits::INFINITE_MINING))
			max_num = 0;
	}
	gArmsData[i].max_num += max_num;
	gArmsData[i].num += num == 0 ? max_num : num;
	gArmsData[i].exp = exp;
	gArmsData[i].level = level;

	for (auto weapon: miracleWeaponryList){
		auto it = std::distance(miracleWeaponryList.begin(), std::find(miracleWeaponryList.begin(), miracleWeaponryList.end(), weapon));
		if (weapon == code && !CheckPermitStage(36+it)){
			AddPermitStage(it+36, 636 + it);
			if (gMC.miracle > 0){
				PlaySoundObject(27, SOUND_MODE_PLAY);
				PlaySoundObject(22, SOUND_MODE_PLAY);
			}
		}
	}

	// Cap the amount of current ammo to the maximum amount of ammo
	if (gArmsData[i].num > gArmsData[i].max_num)
		gArmsData[i].num = gArmsData[i].max_num;

	return true;
}

bool SubArmsData(long code)
{
	// Search for code
	int i;
	for (i = 0; i < ARMS_MAX; ++i)
		if (gArmsData[i].code == code)
			break;	// Found

#ifdef FIX_BUGS
	if (i == ARMS_MAX)
#else
	if (i == ITEM_MAX)	// Wrong
#endif
		return false;	// Not found

	// Shift all arms from the right to the left
	for (++i; i < ARMS_MAX; ++i)
		gArmsData[i - 1] = gArmsData[i];

	// Clear farthest weapon and select first
	gArmsData[i - 1].code = 0;
	gSelectedArms = 0;

	for (auto weapon: miracleWeaponryList){
		auto it = std::distance(miracleWeaponryList.begin(), std::find(miracleWeaponryList.begin(), miracleWeaponryList.end(), weapon));
		if (weapon == code && CheckPermitStage(36+it)){
			SubPermitStage(it+36);
			if (gMC.miracle > 0){
				PlaySoundObject(72);
				PlaySoundObject(101);
			}
		}
	}

	return true;
}

bool TradeArms(long code1, long code2, long max_num)
{
	// Search for code1
	int i = 0;
	while (i < ARMS_MAX)
	{
		if (gArmsData[i].code == code1)
			break;	// Found identical

		++i;
	}

	if (i == ARMS_MAX)
		return false;	// Not found

	// Initialize new weapon replacing old one, but adding the maximum ammunition to that of the old weapon.
	gArmsData[i].level = 1;
	gArmsData[i].code = code2;
	gArmsData[i].max_num += max_num;
	gArmsData[i].num += max_num;
	gArmsData[i].exp = 0;

	return true;
}

bool FullInventory(){
	int i = 0;
	while (i < ITEM_MAX)
	{
		if (gItemData[i].code == 0)
			break;	// Found free slot

		++i;
	}

	return i == ITEM_MAX;
}

bool AddItemData(long code, int quantity, int level, int xp, int ammo)
{
	// Search for code
	int i = 0;
	while (i < ITEM_MAX)
	{
		if (gItemData[i].code == code)
			break;	// Found identical

		if (gItemData[i].code == 0)
			break;	// Found free slot

		++i;
	}

	if (i == ITEM_MAX)
		return false;	// Not found

	gItemData[i].code = code;
	gItemData[i].quantity = quantity;
	gItemData[i].level = level;
	gItemData[i].xp = xp;
	gItemData[i].ammo = ammo;

	if (equipMap.contains(gItemData[i].code) && equipMap[gItemData[i].code].isPassive){
		SetNPCFlag(equipMap[gItemData[i].code].flag);
	}

	for (auto miracle : miracleList){
		auto it = std::distance(miracleList.begin(), std::find(miracleList.begin(), miracleList.end(), miracle));
		if (CheckItem(miracle.first) && CheckItem(miracle.second) && !CheckPermitStage(it+1)){
			AddPermitStage(it+1, 600 + it + 1);
			if (gMC.miracle > 0){
				PlaySoundObject(27);
				PlaySoundObject(22);
			}
		}
	}

	if (code > 100){
		for (auto weapon: miracleWeaponryList){
			auto it = std::distance(miracleWeaponryList.begin(), std::find(miracleWeaponryList.begin(), miracleWeaponryList.end(), weapon));
			if (weapon == code - 100 && !CheckPermitStage(36+it)){
				AddPermitStage(it+36, 636 + it);
				if (gMC.miracle > 0){
					PlaySoundObject(27);
					PlaySoundObject(22);
				}
			}
		}
	}

	return true;
}

bool CheckItemSpace()
{
	// Search for code
	int i = 0;
	while (i < ITEM_MAX)
	{
		if (gItemData[i].code == 0)
			break;	// Found free slot

		++i;
	}

	if (i == ITEM_MAX)
		return false;	// Not found

	return true;
}

bool SubItemData(long code)
{
	// Search for code
	int i;
	for (i = 0; i < ITEM_MAX; ++i)
		if (gItemData[i].code == code)
			break;	// Found

	if (i == ITEM_MAX)
		return false;	// Not found

	// Shift all items from the right to the left
	for (++i; i < ITEM_MAX; ++i)
		gItemData[i - 1] = gItemData[i];

	gItemData[i - 1].code = 0;
	gSelectedItem = 0;
	
	CutNPCFlag(equipMap[code].flag);

	if (code > 100){
		for (auto weapon: miracleWeaponryList){
			auto it = std::distance(miracleWeaponryList.begin(), std::find(miracleWeaponryList.begin(), miracleWeaponryList.end(), weapon));
			if (weapon == code - 100){
				SubPermitStage(it+36);
				if (gMC.miracle > 0){
					PlaySoundObject(72);
					PlaySoundObject(101);
				}
			}
		}
	}

	for (auto miracle : miracleList){
		auto it = std::distance(miracleList.begin(), std::find(miracleList.begin(), miracleList.end(), miracle));
		if (!(CheckItem(miracle.first) && CheckItem(miracle.second))){
			SubPermitStage(it+1);
			if (gMC.miracle > 0){
				PlaySoundObject(72);
				PlaySoundObject(101);
			}
		}
	}

	return true;
}

void SwitchWeapon(void){
	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	Rect rcHand = {112, 88, 128, 104};

	int arms_num = 0;
	while (gArmsData[arms_num].code != 0)
		++arms_num;

	gKeyTrg = gKey = 0;
	g_GameFlags |= GAME_FLAG_IS_CONTROL_ENABLED | GAME_FLAG_iS_GAME_GOING;

	PlaySoundObject(5, SOUND_MODE_PLAY);

	for (;;) {
		GetTrg();

		if (gKeyTrg & gKeyLeft)
		{
			if (gSelectedArms % 6 == 0)
				gSelectedArms += 5;
			else
				gSelectedArms -= 1;
		}

		if (gKeyTrg & gKeyRight)
		{
			if (gSelectedItem == arms_num - 1)
				gSelectedArms = (gSelectedArms / 6) * 6;	// Round down to multiple of 6
			else if (gSelectedArms% 6 == 5)
				gSelectedArms -= 5;	// Loop around row
			else
				gSelectedArms += 1;
		}

		if (gKeyTrg & gKeyUp)
		{
			if (gSelectedArms / 6 != 0)
				gSelectedArms -= 6;
		}

		if (gKeyTrg & gKeyDown)
		{
			if (gSelectedArms / 6 == (arms_num - 1) / 6)
				gCampActive = true;	// We're on the last row, transition to items
			else
				gSelectedArms += 6;
		}

		// // Loop around gSelectedArms if needed
		if (gSelectedArms < 0)
			gSelectedArms = arms_num;

		if (gSelectedArms > arms_num - 1)
			gSelectedArms = 0;

		PutBitmap4(&rcView, 0, 0, &rcView, SURFACE_ID_SCREEN_GRAB);
		PutBitmap3(&rcView, 0, 0, &rcView, SURFACE_ID_BLACK_OVERLAY);
		PutCampObject();

		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((gSelectedArms % 6) * 40) + (WINDOW_WIDTH / 2) - 117 - 15), PixelToScreenCoord((WINDOW_HEIGHT / 2) - 96 + (gSelectedArms / 6) * 30), &rcHand, SURFACE_ID_TEXT_BOX);

		if (gKey & gKeyOk){
			PlaySoundObject(18, SOUND_MODE_PLAY);
			int replacementCode = gItemData[gSelectedItem].code - 100;
			int currentCode = gArmsData[gSelectedArms].code;
			int ammoNeeded = gItemData[gSelectedItem].ammo;
			int xpNeeded = gItemData[gSelectedItem].xp;
			int ammoExisting = gItemData[gSelectedItem].quantity;
			int levelExisting = gItemData[gSelectedItem].level;
			// switch (replacementCode){
			// 	case 2: case 12: //polar star and nemesis
			// 		ammoNeeded = 100;
			// 		if (gMC.equip & EQUIP_MORE_AMMO){
			// 			ammoNeeded *= 1.5;
			// 		}
			// 		if (GetNPCFlag(Flags::LILY_CHARACTER)){
			// 			ammoNeeded = 0;
			// 		}
			// 		break;
			// 	case 5: //rocket launcher
			// 		ammoNeeded = 40;
			// 		if (GetNPCFlag(Flags::LILY_CHARACTER)){
			// 			ammoNeeded += (gMC.depth - 1) * 4;
			// 		}
			// 		break;
			// 	case 16: //rifle
			// 		ammoNeeded = 105 + (gMC.depth - 1) * 4;
			// 		break;
			// 	case 18: //mining laser
			// 		ammoNeeded = 150;
			// 		break;
			// 	case 11: //electric shotgun
			// 		ammoNeeded = 30;
			// 		break;
			// 	case 14: //water gun
			// 		ammoNeeded = 6;
			// 		break;
			// 	case 4: case 7: //machine guns
			// 		ammoNeeded = 100;
			// 		break;
			// 	default:
			// 		ammoNeeded = 0;
			// 		break;
			// }
			gItemData[gSelectedItem].code = currentCode + 100;
			gItemData[gSelectedItem].quantity = gArmsData[gSelectedArms].num;
			gItemData[gSelectedItem].level = gArmsData[gSelectedArms].level;
			gItemData[gSelectedItem].ammo = gArmsData[gSelectedArms].num;
			gItemData[gSelectedItem].xp = gArmsData[gSelectedArms].exp;
			for (auto weapon: miracleWeaponryList){
				auto it = std::distance(miracleWeaponryList.begin(), std::find(miracleWeaponryList.begin(), miracleWeaponryList.end(), weapon));
				if (weapon == currentCode - 100 && CheckPermitStage(36+it)){
					SubPermitStage(it+36);
				}
			}

			gArmsData[gSelectedArms].code = replacementCode;
			gArmsData[gSelectedArms].max_num = ammoNeeded;
			gArmsData[gSelectedArms].num = ammoExisting;
			gArmsData[gSelectedArms].level = levelExisting;
			gArmsData[gSelectedArms].exp = xpNeeded;
			for (auto weapon: miracleWeaponryList){
				auto it = std::distance(miracleWeaponryList.begin(), std::find(miracleWeaponryList.begin(), miracleWeaponryList.end(), weapon));
				if (weapon == replacementCode && !CheckPermitStage(36+it)){
					AddPermitStage(it+36, 636 + it);
				}
			}
			StopTextScript();
			return;
		}

		gCounter++;

		if (!Flip_SystemTask())
			return;
	}
}

/// Update the inventory cursor
void MoveCampCursor(void)
{
	bool bChange;

	// Compute the current amount of weapons and items
	int arms_num = 0;
	int item_num = 0;
	while (gArmsData[arms_num].code != 0)
		++arms_num;
	while (gItemData[item_num].code != 0)
		++item_num;

	if (arms_num == 0 && item_num == 0)
		return;	// Empty inventory

	// True if we're currently changing cursor position
	bChange = false;

	if (!gCampActive)
	{
		// Handle selected weapon
		if (gKeyTrg & gKeyLeft)
		{
			if (gSelectedArms % 6 == 0)
				gSelectedArms += 5;
			else
				gSelectedArms -= 1;
			bChange = true;
		}

		if (gKeyTrg & gKeyRight)
		{
			if (gSelectedItem == arms_num - 1)
				gSelectedArms = (gSelectedArms / 6) * 6;	// Round down to multiple of 6
			else if (gSelectedArms% 6 == 5)
				gSelectedArms -= 5;	// Loop around row
			else
				gSelectedArms += 1;
			bChange = true;
		}

		if (gKeyTrg & gKeyUp)
		{
			if (gSelectedArms / 6 != 0)
				gSelectedArms -= 6;

			bChange = true;
		}

		if (gKeyTrg & gKeyDown)
		{
			if (gSelectedArms / 6 == (arms_num - 1) / 6)
				gCampActive = true;	// We're on the last row, transition to items
			else
				gSelectedArms += 6;

			bChange = true;
		}

		if (gKeyTrg & gKeyAbility){
			if (arms_num == 1){
				StartTextScript(5996);
			} else {
				StartTextScript(5997);
			}
		}

		// // Loop around gSelectedArms if needed
		if (gSelectedArms < 0)
			gSelectedArms = arms_num;

		if (gSelectedArms > arms_num - 1)
			gSelectedArms = 0;
	}
	else
	{
		// Handle selected item
		if (gKeyTrg & gKeyLeft)
		{
			if (gSelectedItem % 7 == 0)
				gSelectedItem += 6;
			else
				gSelectedItem -= 1;

			bChange = true;
		}

		if (gKeyTrg & gKeyRight)
		{
			if (gSelectedItem == item_num - 1)
				gSelectedItem = (gSelectedItem / 7) * 7;	// Round down to multiple of 6
			else if (gSelectedItem % 7 == 6)
				gSelectedItem -= 6;	// Loop around row
			else
				gSelectedItem += 1;

			bChange = true;
		}

		if (gKeyTrg & gKeyUp)
		{
			if (gSelectedItem / 7 == 0)
				gCampActive = false;	// We're on the first row, transition to weapons
			else
				gSelectedItem -= 7;

			bChange = true;
		}

		if (gKeyTrg & gKeyDown)
		{
			if (gSelectedItem / 7 == (item_num - 1) / 6)
				gCampActive = false;	// We're on the last row, transition to weapons
			else
				gSelectedItem += 7;

			bChange = true;
		}

		if (gSelectedItem >= item_num)
			gSelectedItem = item_num - 1;	// Don't allow selecting a non-existing item

		if (gKeyTrg & gKeyDash){
			if (gItemData[gSelectedItem].code > 100){
				SwitchWeapon();
			}
		}

		if (gKeyTrg & gKeyOk){
			if (gItemData[gSelectedItem].code > 100){
				if (arms_num == 12){
					SwitchWeapon();
				}
				else {
					auto ammoItem = gItemData[gSelectedItem];
					SubItemData(gItemData[gSelectedItem].code);
					AddArmsData(ammoItem.code - 100, ammoItem.quantity, 
						ammoItem.xp, ammoItem.ammo, ammoItem.level);
					PlaySoundObject(5, SOUND_MODE_PLAY);
					StopTextScript();
				}
			} else {
				StartTextScript(6000 + gItemData[gSelectedItem].code);
			}
		}

		if (gKeyTrg & gKeyAbility){
			if (equipMap.find(gItemData[gSelectedItem].code) != std::end(equipMap) && GetNPCFlag(equipMap.at(gItemData[gSelectedItem].code).flag) && !equipMap.at(gItemData[gSelectedItem].code).isPassive){
				StartTextScript(6000 + gItemData[gSelectedItem].code);
			}
			else if (gItemData[1].code == 0){
				StartTextScript(5998);
			} else {
				StartTextScript(5999);
			}
		}
	}

	if (bChange)
	{
		if (gCampActive == false)
		{
			// Switch to a weapon
			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			if (arms_num != 0)
				StartTextScript(1000 + gArmsData[gSelectedArms].code);
			else
				StartTextScript(1000);
		}
		else
		{
			// Switch to an item
			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			if (item_num != 0){
				if (gItemData[gSelectedItem].code < 100){
					StartTextScript(5000 + gItemData[gSelectedItem].code);
				} else {
					StartTextScript(1000 + (gItemData[gSelectedItem].code - 100));
				}
			}
			else
				StartTextScript(5000);
		}
	}
}

/// Draw the inventory
void PutCampObject(void)
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
	Rect rcLv = {144, 80, 160, 88};
	rcLv.left += gMC.difficulty * 16;
	rcLv.right += gMC.difficulty * 16;

	Rect rcEquip = {176, 101, 184, 108};
	Rect rcMiracle = {212, 125, 218, 131};

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

	// Draw box
	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 122), PixelToScreenCoord((WINDOW_HEIGHT / 2) - 112), &rcBoxTop, SURFACE_ID_TEXT_BOX);
	for (i = 1; i < 21; ++i)
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 122), PixelToScreenCoord(((WINDOW_HEIGHT / 2) - 120) + ((i + 1) * 8)), &rcBoxBody, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 122), PixelToScreenCoord(((WINDOW_HEIGHT / 2) - 120) + ((i + 1) * 8)), &rcBoxBottom, SURFACE_ID_TEXT_BOX);

	PutMyScore(-2);

	// Move titles
	if (gCampTitleY > (WINDOW_HEIGHT / 2) - 104)
		--gCampTitleY;

	// Draw titles
	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 112), PixelToScreenCoord(gCampTitleY), &rcTitle1, SURFACE_ID_TEXT_BOX);
	PutBitmap3_NoFlip(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) - 112), PixelToScreenCoord(gCampTitleY + 68), &rcTitle2, SURFACE_ID_TEXT_BOX);

	// Draw arms cursor
	++flash;

	if (gCampActive == false)
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((gSelectedArms % 6) * 40) + (WINDOW_WIDTH / 2) - 117), PixelToScreenCoord((WINDOW_HEIGHT / 2) - 96 + (gSelectedArms / 6) * 30), &rcCur1[(flash / 2) % 2], SURFACE_ID_TEXT_BOX);
	else
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((gSelectedArms % 6) * 40) + (WINDOW_WIDTH / 2) - 117), PixelToScreenCoord((WINDOW_HEIGHT / 2) - 96 + (gSelectedArms / 6) * 30), &rcCur1[1], SURFACE_ID_TEXT_BOX);

	// Draw weapons
	for (i = 0; i < ARMS_MAX; ++i)
	{
		if (gArmsData[i].code == 0)
			break;	// Invalid weapon

		// Get icon rect for next weapon
		rcArms.left = (gArmsData[i].code) * 16;
		rcArms.right = rcArms.left + 16;
		rcArms.top = 0;
		rcArms.bottom = rcArms.top + 16;
		if (CheckFlagTrait(FlagTraits::INVIDIA) && gArmsData[i].code == 13){
			rcArms.left = (10) * 16;
			rcArms.right = rcArms.left + 16;
			rcArms.top = 0 * 16;
			rcArms.bottom = rcArms.top + 16;
		}
		if (CheckFlagTrait(FlagTraits::HOOK_FLAIL) && gArmsData[i].code == 8){
			rcArms.left = (6) * 16;
			rcArms.right = rcArms.left + 16;
			rcArms.top = 0 * 16;
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
		PutNumber4(xPos + 6, armsYPos+1, gArmsData[i].level, false);

		// Draw ammo
		if (gArmsData[i].max_num)
		{
			PutNumber4(xPos+7, armsYPos + 11, gArmsData[i].num, false);
			PutNumber4(xPos+7, armsYPos + 19, gArmsData[i].max_num, false);
		}
		else
		{
			// Weapon doesn't use ammunition
			PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(xPos+18), PixelToScreenCoord(armsYPos + 12), &rcNone, SURFACE_ID_TEXT_BOX);
			PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(xPos+18), PixelToScreenCoord(armsYPos + 21), &rcNone, SURFACE_ID_TEXT_BOX);
		}
	}

	// Draw items cursor
	if (gCampActive == true)
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((gSelectedItem % 7) * 32) + (WINDOW_WIDTH / 2) - 112), PixelToScreenCoord(((gSelectedItem / 7) * 16) + (WINDOW_HEIGHT / 2) - 28), &rcCur2[flash / 2 % 2], SURFACE_ID_TEXT_BOX);
	else
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((gSelectedItem % 7) * 32) + (WINDOW_WIDTH / 2) - 112), PixelToScreenCoord(((gSelectedItem / 7) * 16) + (WINDOW_HEIGHT / 2) - 28), &rcCur2[1], SURFACE_ID_TEXT_BOX);

	for (i = 0; i < ITEM_MAX; ++i)
	{
		if (gItemData[i].code == 0)
			break;	// Invalid item

		int offsetX = 0;
		int offsetY = 0;

		// Get rect for next item
		if (gItemData[i].code < 100){
			rcItem.left = (gItemData[i].code % 8) * 32;
			rcItem.right = rcItem.left + 32;
			rcItem.top = (gItemData[i].code / 8) * 16;
			rcItem.bottom = rcItem.top + 16;
		}
		else
		{
			offsetX = 8;
			rcItem.left = (gItemData[i].code - 100) * 16;
			rcItem.right = rcItem.left + 16;
			rcItem.top = 0;
			rcItem.bottom = rcItem.top + 16;
			if (CheckFlagTrait(FlagTraits::INVIDIA) && gItemData[i].code == 113)
			{
				rcItem.left = (10) * 16;
				rcItem.right = rcItem.left + 16;
				rcItem.top = 0 * 16;
				rcItem.bottom = rcItem.top + 16;
			}
			if (CheckFlagTrait(FlagTraits::HOOK_FLAIL) && gItemData[i].code == 108)
			{
				rcItem.left = (6) * 16;
				rcItem.right = rcItem.left + 16;
				rcItem.top = 0 * 16;
				rcItem.bottom = rcItem.top + 16;
			}
		}

		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((i % 7) * 32 + offsetX) + (WINDOW_WIDTH / 2) - 112), PixelToScreenCoord(((i / 7) * 16 + offsetY) + (WINDOW_HEIGHT / 2) - 28), &rcItem, 
			gItemData[i].code > 100 ? SURFACE_ID_ARMS_IMAGE : SURFACE_ID_ITEM_IMAGE);

		if (equipMap.find(gItemData[i].code) != std::end(equipMap) && GetNPCFlag(equipMap.at(gItemData[i].code).flag) && !equipMap.at(gItemData[i].code).isPassive){
			PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((i % 7) * 32) + (WINDOW_WIDTH / 2) - 112 + (rcItem.right - rcItem.left) - (rcEquip.right - rcEquip.left)), PixelToScreenCoord(((i / 7) * 16) + (WINDOW_HEIGHT / 2) - 28), &rcEquip, SURFACE_ID_TEXT_BOX);
		}
	}

	for (auto miraclePair: miracleList){
		bool firstOne = miraclePair.first == gItemData[gSelectedItem].code;
		bool secondOne = miraclePair.second == gItemData[gSelectedItem].code;
		if (firstOne || secondOne){
			for (i = 0; i < ITEM_MAX; ++i)
			{
				if (i != gSelectedItem){
					if ((firstOne && miraclePair.second == gItemData[i].code) || (secondOne && miraclePair.first == gItemData[i].code)){
						PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((gSelectedItem % 7) * 32) + (WINDOW_WIDTH / 2) - 112 + (rcItem.right - rcItem.left) - (rcEquip.right - rcEquip.left)), PixelToScreenCoord(((gSelectedItem / 7) * 16) + (WINDOW_HEIGHT / 2) - 28 + 9), &rcMiracle, SURFACE_ID_TEXT_BOX);
						PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(((i % 7) * 32) + (WINDOW_WIDTH / 2) - 112 + (rcItem.right - rcItem.left) - (rcEquip.right - rcEquip.left)), PixelToScreenCoord(((i / 7) * 16) + (WINDOW_HEIGHT / 2) - 28 + 9), &rcMiracle, SURFACE_ID_TEXT_BOX);
					}
				}
			}
		}
	}
}

int CampLoop(void)
{
	std::string old_script_path;

	Rect rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	Rect rcHighScore = {0, 144, 80, 152};
	Rect rcSeed = {96, 144, 128, 152};

	// Save the current script path (to restore it when we get out of the inventory)
	old_script_path = GetTextScriptPath();

	// Load the inventory script
	LoadTextScript2("ArmsItem.tsc");

	gCampTitleY = (WINDOW_HEIGHT / 2) - 96;

	// Put the cursor on the first weapon
	gCampActive = false;
	gSelectedItem = 0;
	g_GameFlags |= GAME_FLAG_ARE_WE_IN_DIFFERENT_WINDOW;

	// Compute current amount of weapons
	int arms_num = 0;
	while (gArmsData[arms_num].code != 0)
		++arms_num;

	if (arms_num != 0)
		StartTextScript(1000 + gArmsData[gSelectedArms].code);
	else
		StartTextScript(5000 + gItemData[gSelectedItem].code);

	for (;;)
	{
		GetTrg();

		if (gKeyTrg & KEY_PAUSE)
		{
			switch (Call_Pause())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;	// Quit game

				case enum_ESCRETURN_ranks:
					return enum_ESCRETURN_ranks;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;	// Go to game intro
			}
		}

		// Handle ESC
		if (gKeyTrg & KEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;	// Quit game

				case enum_ESCRETURN_ranks:
					return enum_ESCRETURN_ranks;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;	// Go to game intro
			}
		}

		if (gKeyTrg & KEY_F2){
			gKeyTrg = 0;
			DeleteSave();
			g_GameFlags &= ~GAME_FLAG_ARE_WE_IN_DIFFERENT_WINDOW;
			return 2;
		}

		if (gKeyTrg & KEY_F3){
			ChangeMusic(MUS_MELTDOWN2);
		}

		if (gKeyTrg & gKeyTurboText){
			PlaySoundObject(80);
			turboTextEnabled = !turboTextEnabled;
		}

		if (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED)
			MoveCampCursor();

		switch (TextScriptProc())
		{
			case enum_ESCRETURN_exit:
				return enum_ESCRETURN_exit;	// Quit game

			case enum_ESCRETURN_ranks:
				return enum_ESCRETURN_ranks;

			case enum_ESCRETURN_restart:
				return enum_ESCRETURN_restart;	// Go to game intro
		}

		// Get currently displayed image
		PutBitmap4(&rcView, 0, 0, &rcView, SURFACE_ID_SCREEN_GRAB);
		PutBitmap3(&rcView, 0, 0, &rcView, SURFACE_ID_BLACK_OVERLAY);
		PutMyLife(false);
		PutArmsEnergy(false);
		PutActiveArm();
		PutCampObject();
		PutTextScript();
		PutFramePerSecound();
		int baseY = 48;
		if (ChallengeEnabled("timer")){
			baseY = 64;
			int lol = LoadTimeCounter();
				PutTimeCounter(WINDOW_WIDTH - 80, WINDOW_HEIGHT - baseY + 48, &lol);
		}
		if (!gMC.randomSeed){
			PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(WINDOW_WIDTH - 91), PixelToScreenCoord(WINDOW_HEIGHT - baseY), &rcSeed, SURFACE_ID_TEXT_BOX);
			PutNumber10(WINDOW_WIDTH - 91, WINDOW_HEIGHT - baseY + 8, gMC.seed, true);
		}
		PutBitmap3_NoFlip(&rcView, PixelToScreenCoord(WINDOW_WIDTH - 91), PixelToScreenCoord(WINDOW_HEIGHT - baseY + 24), &rcHighScore, SURFACE_ID_TEXT_BOX);
		PutNumber10(WINDOW_WIDTH - 91, WINDOW_HEIGHT - baseY + 32, playerHighScore, true);

		// Check whether we're getting out of the loop
		if (gCampActive)
		{
			if (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED && gKeyTrg & (gKeyCancel | gKeyItem))
			{
				StopTextScript();
				break;
			}
		}
		else
		{
			if (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED && gKeyTrg & (gKeyOk | gKeyCancel | gKeyItem))
			{
				StopTextScript();
				break;
			}
		}

		gCounter++;

		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;	// Quit game
	}

	// Resume original script
	LoadTextScript_Stage(old_script_path.c_str());
	gArmsEnergyX = 32; // Displays weapon rotation animation in case the weapon was changed
	g_GameFlags &= ~GAME_FLAG_ARE_WE_IN_DIFFERENT_WINDOW;
	return enum_ESCRETURN_continue;	// Go to game
}

bool CheckItem(int a)
{
	int i;

	for (i = 0; i < ITEM_MAX; ++i)
		if (gItemData[i].code == a)
			return true;	// Found

	return false;	// Not found
}

bool CheckArms(long a)
{
	int i;

	for (i = 0; i < ARMS_MAX; ++i)
		if (gArmsData[i].code == a)
			return true;	// Found

	return false;	// Not found
}

bool UseArmsEnergy(long num)
{
	if (ChallengeEnabled("debug"))
		return true;

	if (gArmsData[gSelectedArms].max_num == 0)
		return true;	// No ammo needed
	if (gArmsData[gSelectedArms].num < num)
		return false;	// No ammo left

	if (CheckFlagTrait(FlagTraits::SAVING_AMMO_CHANCE) && Random(0, GetFlagTraitData<int>(FlagTraits::SAVING_AMMO_CHANCE, "chance")-1) == 0) return true;

	gArmsData[gSelectedArms].num -= num;

	if (gArmsData[gSelectedArms].num < 0)
		gArmsData[gSelectedArms].num = 0;

	return true;	// Was able to spend ammo
}

bool ChargeArmsEnergy(long num)
{
	gArmsData[gSelectedArms].num += num;

	// Cap the ammo to the maximum ammunition
	if (gArmsData[gSelectedArms].num > gArmsData[gSelectedArms].max_num)
		gArmsData[gSelectedArms].num = gArmsData[gSelectedArms].max_num;

	return true;	// Always successfull
}

void FullArmsEnergy(void)
{
	int a;

	for (a = 0; a < ARMS_MAX; ++a)
	{
		if (gArmsData[a].code == 0)
			continue;	// Don't change empty weapons

		gArmsData[a].num = gArmsData[a].max_num;
	}
}

int RotationArms(void)
{
	// Get amount of weapons
	int arms_num = 0;
	while (gArmsData[arms_num].code != 0)
		++arms_num;

	if (arms_num == 0)
		return 0;

	ResetSpurCharge();

	// Select next valid weapon
	++gSelectedArms;

	while (gSelectedArms < arms_num)
	{
		if (gArmsData[gSelectedArms].code)
			break;

		++gSelectedArms;
	}

	if (gSelectedArms == arms_num)
		gSelectedArms = 0;

	gArmsEnergyX = 32;
	PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

	return gArmsData[gSelectedArms].code;
}

int RotationArmsDirect(void)
{
	// Get amount of weapons
	int arms_num = 0;
	while (gArmsData[arms_num].code != 0)
		++arms_num;

	ResetSpurCharge();

	// Select next valid weapon
	gSelectedArms = gNumberKey-1;

	if (gSelectedArms > arms_num){
		gSelectedArms = arms_num;
	}

	gArmsEnergyX = 32;
	PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

	return gArmsData[gSelectedArms].code;
}

int RotationArmsRev(void)
{
	// Get amount of weapons
	int arms_num = 0;
	while (gArmsData[arms_num].code != 0)
		++arms_num;

	if (arms_num == 0)
		return 0;

	ResetSpurCharge();

	// Select previous valid weapon
	--gSelectedArms;

	if (gSelectedArms < 0)
		gSelectedArms = arms_num - 1;

	while (gSelectedArms < arms_num)
	{
		if (gArmsData[gSelectedArms].code)
			break;

		--gSelectedArms;
	}

	gArmsEnergyX = 0;
	PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

	return gArmsData[gSelectedArms].code;
}

void ChangeToFirstArms(void)
{
	gSelectedArms = 0;
	gArmsEnergyX = 32;
	PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);
}
