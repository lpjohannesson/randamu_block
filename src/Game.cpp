#include "Game.h"

#include <cstddef>
#include <string>

#include "Backends/Misc.h"
#include "ArmsItem.h"
#include "Back.h"
#include "Boss.h"
#include "BossLife.h"
#include "BulHit.h"
#include "Bullet.h"
#include "Caret.h"
#include "Cooldown.h"
#include "Draw.h"
#include "Ending.h"
#include "Escape.h"
#include "Fade.h"
#include "Flags.h"
#include "Flash.h"
#include "Frame.h"
#include "Generic.h"
#include "GenericLoad.h"
#include "KeyControl.h"
#include "Main.h"
#include "Map.h"
#include "MapName.h"
#include "MiniMap.h"
#include "MinimapEx.h"
#include "MyChar.h"
#include "MycHit.h"
#include "MycParam.h"
#include "NpChar.h"
#include "NpcHit.h"
#include "NpcTbl.h"
#include "Organya.h"
#include "Pause.h"
#include "Profile.h"
#include "Random.h"
#include "RankingsWindow.h"
#include "ResourceCode.h"
#include "Backends/Rendering.h"
#include "Score.h"
#include "SelStage.h"
#include "Shoot.h"
#include "Sound.h"
#include "Stage.h"
#include "Star.h"
#include "TextScr.h"
#include "ValueView.h"

int g_GameFlags;
int gCounter;
int SelectedCustomItem = 1;
int gGameMode;

static bool bContinue;

int Random(int min, int max, RandomCategories::RandomCategories category)
{
	const int range = max - min + 1;
	switch (category)
	{
		case RandomCategories::SEEDED_PRIMARY:
			if (genContext.legacyRNG)
				return (msvc_rand_s() % (range+1)) + min;
			else
				return pcg32_boundedrand_r(&seed_primary, range) + min;

		case RandomCategories::SEEDED_CHESTS:
			if (genContext.legacyRNG)
				return (msvc_rand_s2() % (range+1)) + min;
			else
				return pcg32_boundedrand_r(&seed_chests, range) + min;

		case RandomCategories::SEEDED_NPCS:
			if (genContext.legacyRNG)
				return (msvc_rand_s3() % (range+1)) + min;
			else
				return pcg32_boundedrand_r(&seed_npcs, range) + min;
	
		default:
			return msvc_rand(range+1) + min;
	}
}

void PutNumber4(int x, int y, int value, bool bZero)
{
	// Define rects
	Rect rcClient = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	Rect rect[10] = {
		{0, 56, 8, 64},
		{8, 56, 16, 64},
		{16, 56, 24, 64},
		{24, 56, 32, 64},
		{32, 56, 40, 64},
		{40, 56, 48, 64},
		{48, 56, 56, 64},
		{56, 56, 64, 64},
		{64, 56, 72, 64},
		{72, 56, 80, 64},
	};

	// Digits
	int tbl[4] = {1000, 100, 10, 1};

	int a;
	int sw;
	int offset;

	// Limit value
	if (value > 9999)
		value = 9999;

	// Go through number and draw digits
	offset = 0;
	sw = 0;
	while (offset < 4)
	{
		// Get the digit that this is
		a = 0;

		while (value >= tbl[offset])
		{
			value -= tbl[offset];
			++a;
			++sw;
		}

		// Draw digit
		if ((bZero && offset == 2) || sw != 0 || offset == 3)
			PutBitmap3_NoFlip(&rcClient, PixelToScreenCoord(x + 8 * offset), PixelToScreenCoord(y), &rect[a], SURFACE_ID_TEXT_BOX);

		// Go to next digit
		++offset;
	}
}

void PutNumber5(int x, int y, int value, bool bZero)
{
	// Define rects
	Rect rcClient = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	Rect rect[10] = {
		{0, 56, 8, 64},
		{8, 56, 16, 64},
		{16, 56, 24, 64},
		{24, 56, 32, 64},
		{32, 56, 40, 64},
		{40, 56, 48, 64},
		{48, 56, 56, 64},
		{56, 56, 64, 64},
		{64, 56, 72, 64},
		{72, 56, 80, 64},
	};

	// Digits
	int tbl[6] = {100000, 10000, 1000, 100, 10, 1};

	int a;
	int sw;
	int offset;

	// Limit value
	if (value > 999999)
		value = 999999;

	// Go through number and draw digits
	offset = 0;
	sw = 0;
	while (offset < 6)
	{
		// Get the digit that this is
		a = 0;

		while (value >= tbl[offset])
		{
			value -= tbl[offset];
			++a;
			++sw;
		}

		// Draw digit
		if ((bZero) || sw != 0)
			PutBitmap3_NoFlip(&rcClient, PixelToScreenCoord(x + 8 * offset), PixelToScreenCoord(y), &rect[a], SURFACE_ID_TEXT_BOX);

		// Go to next digit
		++offset;
	}
}

void PutNumber9(int x, int y, size_t value, bool bZero)
{
	// Define rects
	Rect rcClient = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	Rect rect[10] = {
		{0, 56, 8, 64},
		{8, 56, 16, 64},
		{16, 56, 24, 64},
		{24, 56, 32, 64},
		{32, 56, 40, 64},
		{40, 56, 48, 64},
		{48, 56, 56, 64},
		{56, 56, 64, 64},
		{64, 56, 72, 64},
		{72, 56, 80, 64},
	};

	// Digits
	int tbl[9] = { 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1 };

	int a;
	int sw;
	int offset;

	// Limit value
	if (value > 999999999)
		value = 999999999;

	// Go through number and draw digits
	offset = 0;
	sw = 0;
	while (offset < 9)
	{
		// Get the digit that this is
		a = 0;

		while (value >= tbl[offset])
		{
			value -= tbl[offset];
			++a;
			++sw;
		}

		// Draw digit
		if ((bZero) || sw != 0)
			PutBitmap3_NoFlip(&rcClient, PixelToScreenCoord(x + 8 * offset), PixelToScreenCoord(y), &rect[a], SURFACE_ID_TEXT_BOX);

		// Go to next digit
		++offset;
	}
}

void PutNumber10(int x, int y, size_t value, bool bZero)
{
	// Define rects
	Rect rcClient = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	Rect rect[10] = {
		{0, 56, 8, 64},
		{8, 56, 16, 64},
		{16, 56, 24, 64},
		{24, 56, 32, 64},
		{32, 56, 40, 64},
		{40, 56, 48, 64},
		{48, 56, 56, 64},
		{56, 56, 64, 64},
		{64, 56, 72, 64},
		{72, 56, 80, 64},
	};

	// Digits
	int tbl[10] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1 };

	int a;
	int sw;
	int offset;

	// Limit value
	if (value > 9999999999)
		value = 9999999999;

	// Go through number and draw digits
	offset = 0;
	sw = 0;
	while (offset < 10)
	{
		// Get the digit that this is
		a = 0;

		while (value >= tbl[offset])
		{
			value -= tbl[offset];
			++a;
			++sw;
		}

		// Draw digit
		if ((bZero) || sw != 0)
			PutBitmap3_NoFlip(&rcClient, PixelToScreenCoord(x + 8 * offset), PixelToScreenCoord(y), &rect[a], SURFACE_ID_TEXT_BOX);

		// Go to next digit
		++offset;
	}
}

// Draws black bars to cover the out-of-bounds parts of the screen
static void PutBlackBars(int fx, int fy)
{
	Rect rect;

	// Don't draw if credits are running
	if (g_GameFlags & GAME_FLAG_ARE_CREDITS_ROLLING)
		return;

	int stage_left;
	int stage_top;
	int stage_width;
	int stage_height;

	// Force black-bars in the IronHead battle
	// if (gStageNo == 31)
	// {
	// 	stage_left = 20 * 16;
	// 	stage_top = 0;
	// 	stage_width = 320;
	// 	stage_height = 240;
	// }
	// else
	// {
		stage_left = 0;
		stage_top = 0;
		stage_width = (gMap.width - 1) * 16;
		stage_height = (gMap.length - 1) * 16;
	// }

	// Left bar
	rect.left = 0;
	rect.top = 0;
	rect.right = stage_left - (fx / 0x200);
	rect.bottom = WINDOW_HEIGHT;
	CortBox(&rect, 0);

	// Right bar
	rect.left = stage_left + stage_width - (fx / 0x200);
	rect.top = 0;
	rect.right = WINDOW_WIDTH;
	rect.bottom = WINDOW_HEIGHT;
	CortBox(&rect, 0);

	// Top bar
	rect.left = 0;
	rect.top = 0;
	rect.right = WINDOW_WIDTH;
	rect.bottom = stage_top - (fy / 0x200);
	CortBox(&rect, 0);

	// Bottom bar
	rect.left = 0;
	rect.top = stage_top + stage_height - (fy / 0x200);
	rect.right = WINDOW_WIDTH;
	rect.bottom = WINDOW_HEIGHT;
	CortBox(&rect, 0);
}

static int ModeOpening(void)
{
	int frame_x;
	int frame_y;
	unsigned int wait;

	InitNpChar();
	InitCaret();
	InitStar();
	InitFade();
	InitFlash();
	InitBossLife();
	ChangeMusic(MUS_SILENCE);
	TransferStage(72, 100, 3, 3);
	SetFrameTargetMyChar(16);
	SetFadeMask();

	// Reset cliprect and flags
	grcGame.left = 0;
#if WINDOW_WIDTH != 320 || WINDOW_HEIGHT != 240
	// Non-vanilla: these three lines are widescreen-related
	grcGame.top = 0;
	grcGame.right = WINDOW_WIDTH;
	grcGame.bottom = WINDOW_HEIGHT;
#endif

	g_GameFlags = GAME_FLAG_IS_CONTROL_ENABLED | GAME_FLAG_iS_GAME_GOING;

	CutNoise();

	wait = 0;
	while (wait < 500)
	{
		// Increase timer
		++wait;

		// Get pressed keys
		GetTrg();

		if (gKey & KEY_PAUSE)
		{
			switch (Call_Pause())
			{
				case enum_ESCRETURN_exit:
					return 0;

				case enum_ESCRETURN_restart:
					return 3;
			}
		}

		// Escape menu
		if (gKey & KEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return 0;

				case enum_ESCRETURN_restart:
					return 3;
			}
		}

		// Skip intro if OK is pressed
		if (gKey & gKeyOk)
			break;

		// Update everything
		ActNpChar();
		ActBossChar();
		ActBack();
		ResetMyCharFlag();
		HitMyCharMap();
		HitMyCharNpChar();
		HitMyCharBoss();
		HitNpCharMap();
		HitBossMap();
		HitBossBullet();
		ActCaret();
		MoveFrame3();
		ProcFade();
		ProcFade();

		// Draw everything
		CortBox(&grcFull, 0x000000);

		GetFramePosition(&frame_x, &frame_y);
		PutBack(frame_x, frame_y);
		PutStage_Back(frame_x, frame_y);
		PutBossChar(frame_x, frame_y);
		PutNpChar(frame_x, frame_y);
		PutMapDataVector(frame_x, frame_y);
		PutStage_Front(frame_x, frame_y);
		PutFront(frame_x, frame_y);
		PutBlackBars(frame_x, frame_y);
		PutCaret(frame_x, frame_y);
		PutFade();

		// Update Text Script
		switch (TextScriptProc())
		{
			case enum_ESCRETURN_exit:
				return 0;

			case enum_ESCRETURN_restart:
				return 3;
		}

		PutMapName(false);
		PutTextScript();
		PutFramePerSecound();

		if (!Flip_SystemTask())
			return 0;

		++gCounter;
	}

	wait = Backend_GetTicks();
	while (Backend_GetTicks() < wait + 500)
	{
		CortBox(&grcGame, 0x000000);
		PutFramePerSecound();
		if (!Flip_SystemTask())
			return 0;
	}
	return 2;
}

static int ModeTitle(void)
{
	unsigned int wait;

	int anime;
	int v1, v2, v3, v4;

	Rect char_rc;
	int char_type;
	int time_counter;
	int char_y;
	SurfaceID char_surf;
	unsigned long back_color;

	// Reset everything
	InitCaret();
	InitStar();
	CutNoise();

	// Create variables
	anime = 0;
	char_type = 0;
	time_counter = 0;
	back_color = GetCortBoxColor(RGB(0x10, 0x10, 0x20));

	GetCompileVersion(&v1, &v2, &v3, &v4);
	LoadScore(0);

	ChangeMusic(MUS_SELECT);

	// Reset cliprect, flags, and give the player the Nikumaru counter
	grcGame.left = 0;
#if WINDOW_WIDTH != 320 || WINDOW_HEIGHT != 240
	// Non-vanilla: these three lines are widescreen-related
	grcGame.top = 0;
	grcGame.right = WINDOW_WIDTH;
	grcGame.bottom = WINDOW_HEIGHT;
#endif

	g_GameFlags = 0;

	// Start loop
	wait = 0;

	while (1)
	{
		// Don't accept selection for 10 frames
		if (wait < 10)
			++wait;

		// Get pressed keys
		GetTrg(); 

		// Quit when OK is pressed
		if (wait >= 10)
		{
			if (gKeyTrg & gKeyOk)
			{
				PlaySoundObject(18, SOUND_MODE_PLAY);
				break;
			}
		}

		// Update carets
		ActCaret();

		// Animate character cursor
		anime = 0;
		bool lol = false;

		switch (Call_Title())
		{
			case enum_ESCRETURN_exit:
				return 0;

			case enum_ESCRETURN_ranks:
				return 1;

			case enum_ESCRETURN_restart:
				lol = true; break;
		}
		PutFramePerSecound();

		if (!Flip_SystemTask())
			return 0;

		if (lol){
			StartFadeOut(4);
			break;
		}
	}

	SetOrganyaFadeout();
	BackupSurface(SURFACE_ID_SCREEN_GRAB, &grcGame);

	// Black screen when option is selected
	wait = Backend_GetTicks();
	while (Backend_GetTicks() < wait + 1500)
	{
		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);
		ProcFade();
		PutFade();
		PutFramePerSecound();
		if (!Flip_SystemTask())
			return 0;
	}

	return 3;
}

bool TimeStopActive(int npc_id){
	int timeCoefficient = 1;
	bool stopTime = false;

	if (npc_id != 0 && timeStopCoefficients.contains(npc_id)){
		timeCoefficient = timeStopCoefficients[npc_id];
	} else {
		timeCoefficient = 25;
	}

	if (CheckForCooldown(gCooldowns, TIME_STOP_ID) > 0){
		stopTime = gCounter % timeCoefficient != 0;
	}

	if (ChallengeEnabled("superHotMode") && timeCoefficient != 1){
		stopTime = gKey != 0;
	}

	return stopTime;
}

static int ModeAction(void)
{
	int frame_x;
	int frame_y;

	unsigned int swPlay;
	unsigned long color = GetCortBoxColor(RGB(0, 0, 0x20));

	swPlay = 1;

	// Reset stuff
	gCounter = 0;
	grcGame.left = 0;
#if WINDOW_WIDTH != 320 || WINDOW_HEIGHT != 240
	// Non-vanilla: these three lines are widescreen-related
	grcGame.top = 0;
	grcGame.right = WINDOW_WIDTH;
	grcGame.bottom = WINDOW_HEIGHT;
#endif
	g_GameFlags = GAME_FLAG_IS_CONTROL_ENABLED | GAME_FLAG_iS_GAME_GOING;
	g_GameFlags &= ~GAME_FLAG_ARE_WE_IN_DIFFERENT_WINDOW;
	InitMyChar();
	InitNpChar();
	InitBullet();
	InitCaret();
	InitStar();
	InitFade();
	InitFlash();
	ClearArmsData();
	ClearItemData();
	ClearPermitStage();
	StartMapping();
	InitFlags();
	InitBossLife();

	if (IsProfile())
	{
		if (!LoadProfile(NULL) && !InitializeGame())
			return 0;
	}
	else
	{
		if (!InitializeGame())
			return 0;
	}

	while (1)
	{
		// Get pressed keys
		GetTrg();

		if (gKey & KEY_PAUSE && !(g_GameFlags & GAME_FLAG_ARE_CREDITS_ROLLING))
		{
			BackupSurface(SURFACE_ID_SCREEN_GRAB, &grcGame);
			switch (Call_Pause())
			{
				case enum_ESCRETURN_exit:
					return 2;

				case enum_ESCRETURN_ranks:
					return 1;

				case enum_ESCRETURN_restart:
					return 3;
			}
		}

		// Escape menu
		if (gKey & KEY_ESCAPE && !(g_GameFlags & GAME_FLAG_ARE_CREDITS_ROLLING))
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return 0;

				case enum_ESCRETURN_restart:
					return 3;
			}
		}
		char buffer1[210];
		int v1, v2, v3, v4;
		GetCompileVersion(&v1, &v2, &v3, &v4);
		if (gbExtendedInfo)
		{
			sprintf(buffer1, "%s v%d.%d.%d.%d%s -- Seed: %u -- X: %.4g, Y: %.4g -- HP: %d/%d - %d/%d", gWindowTitle.c_str(), v1, v2, v3, v4, IsBeta() ? " (beta)" : "", gMC.seed, gMC.x / 8192.0F, gMC.y / 8192.0F, gMC.life, gMC.max_life, gKey, gKeyTrg);
		} else {
			sprintf(buffer1, "%s v%d.%d.%d.%d%s", gWindowTitle.c_str(), v1, v2, v3, v4, IsBeta() ? " (beta)" : "");
		}
		RenderBackend_ChangeWindowTitle(buffer1);

		if (gKeyTrg & KEY_F2){
			gKeyTrg = 0;
			DeleteSave();
			return 3;
		}

		if (gKeyTrg & KEY_F3){
			ChangeMusic(MUS_MELTDOWN2);
		}

		if (gKeyTrg & gKeyTurboText){
			PlaySoundObject(80);
			turboTextEnabled = !turboTextEnabled;
		}

		// if (gKey & gKeyDash){
		// 	PlaySoundObject(27, SOUND_MODE_PLAY);
		// }

		if (swPlay % 2 && g_GameFlags & GAME_FLAG_iS_GAME_GOING)	// The "swPlay % 2" part is always true
		{
			if (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED)
				ActMyChar(true);
			else
				ActMyChar(false);

			ActStar();
			ActNpChar();
			ActBossChar();
			HitNpCharMap();
			HitBulletMap();
			HitNpCharBullet();
			HitBossBullet();
			ActValueView();
			ActBack();
			ResetMyCharFlag();
			HitMyCharMap();
			HitMyCharNpChar();
			HitMyCharBoss();
			HitBossMap();
			if (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED)
				ShootBullet();
			ActBullet();
			ActCaret();
			MoveFrame3();
#ifdef FIX_BUGS
			// ActFlash uses frame_x and frame_y uninitialised
			GetFramePosition(&frame_x, &frame_y);
#endif
			ActFlash(frame_x, frame_y);

			if (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED)
				AnimationMyChar(true);
			else
				AnimationMyChar(false);
		}

		if (g_GameFlags & GAME_FLAG_ARE_CREDITS_ROLLING)
		{
			ActionCredit();
			ActionIllust();
			ActionStripper();
		}

		ProcFade();
		CortBox(&grcFull, color);
		GetFramePosition(&frame_x, &frame_y);
		PutBack(frame_x, frame_y);
		PutStage_Back(frame_x, frame_y);
		PutBossChar(frame_x, frame_y);
		PutNpChar(frame_x, frame_y);
		PutNpChar_Above(frame_x, frame_y);
		PutBullet(frame_x, frame_y);
		PutMyChar(frame_x, frame_y);
		PutStar(frame_x, frame_y);
		PutMapDataVector(frame_x, frame_y);
		PutGrass(frame_x, frame_y);
		PutStage_Front(frame_x, frame_y);
		PutFront(frame_x, frame_y);
		PutBlackBars(frame_x, frame_y);
		PutFlash();
		PutCaret(frame_x, frame_y);
		if (IsItDark())
			PutStage_Darkness(frame_x, frame_y);
		PutValueView(frame_x, frame_y);
		PutBossLife();
		PutFade();
		PutMappingName();

		if (!(g_GameFlags & GAME_FLAG_IS_GAMEPLAY_NOT_POSSIBLE))
		{
			// Open inventory
			if (gKeyTrg & gKeyItem)
			{
				BackupSurface(SURFACE_ID_SCREEN_GRAB, &grcGame);

				switch (CampLoop())
				{
					case enum_ESCRETURN_exit:
						return 0;

					case enum_ESCRETURN_restart:
						return 3;
				}

				gMC.cond &= ~1;
			}
		}

		if (gMC.equip & EQUIP_MAP && gKeyTrg & gKeyMap && IsMapping())
		{
			BackupSurface(SURFACE_ID_SCREEN_GRAB, &grcGame);

			if (ChallengeEnabled("improve_map"))
			{
				switch (MiniMapExLoop())
				{
				case enum_ESCRETURN_exit:
					return 0;

				case enum_ESCRETURN_restart:
					return 3;
				}
			}
			else
			{

				switch (MiniMapLoop())
				{
				case enum_ESCRETURN_exit:
					return 0;

				case enum_ESCRETURN_restart:
					return 3;
				}
			}
		}

		if (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED)
		{
			if (gKeyTrg & gKeyArms)
				RotationArms();
			else if (gKeyTrg & gKeyArmsRev)
				RotationArmsRev();
			if (gNumberKey != -1){
				if (ChallengeEnabled("advanced_arms")){
					RotationArmsDirect();
					gNumberKey = -1;
				}
			}
		}

		if (swPlay % 2)	// This is always true
		{
			switch (TextScriptProc())
			{
				case enum_ESCRETURN_exit:
					return 0;

				case enum_ESCRETURN_ranks:
					return 1;

				case enum_ESCRETURN_restart:
					return 3;
			}
		}

		PutMapName(false);
		if (ChallengeEnabled("timer"))
			PutTimeCounter(16, WINDOW_HEIGHT - 12);

		if (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED)
		{
			PutMyLife(true);
			PutArmsEnergy(true);
			PutMyAir((WINDOW_WIDTH / 2) - 40, (WINDOW_HEIGHT / 2) - 16);
			PutActiveArmsList();
		}

		if (g_GameFlags & GAME_FLAG_ARE_CREDITS_ROLLING)
		{
			PutIllust();
			PutStripper();
		} else {
			PutMyScore(0);
			PutCooldownList(gCooldowns);
		}

		PutTextScript();

		PutFramePerSecound();

		if (!Flip_SystemTask())
			return 0;

		++gCounter;
	}

	return 3;
}

static int ModeRankings(void)
{
	int frame_x;
	int frame_y;

	unsigned int swPlay;
	unsigned long color = GetCortBoxColor(RGB(0, 0, 0x20));

	swPlay = 1;

	// Reset stuff
	gCounter = 0;
	grcGame.left = 0;
#if WINDOW_WIDTH != 320 || WINDOW_HEIGHT != 240
	// Non-vanilla: these three lines are widescreen-related
	grcGame.top = 0;
	grcGame.right = WINDOW_WIDTH;
	grcGame.bottom = WINDOW_HEIGHT;
#endif
	g_GameFlags = GAME_FLAG_IS_CONTROL_ENABLED | GAME_FLAG_iS_GAME_GOING;

	while (1)
	{
		// Get pressed keys
		GetTrg();

		if (gKey & (KEY_PAUSE | KEY_ESCAPE))
		{
			StartFadeOut(4);
			break;
		}

		bool lol = false;

		switch (RankingLoop())
		{
					case enum_ESCRETURN_exit:
						return 0;

					case enum_ESCRETURN_continue:
						return 1;

					case enum_ESCRETURN_restart:
						lol = true; break;
		}

		if (lol){
			StartFadeOut(4);
			break;
		}
	}

	SetOrganyaFadeout();
	BackupSurface(SURFACE_ID_SCREEN_GRAB, &grcGame);

	int wait = Backend_GetTicks();
	while (Backend_GetTicks() < wait + 1500)
	{
		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);
		ProcFade();
		PutFade();
		PutFramePerSecound();
		if (!Flip_SystemTask())
			return 0;
	}
	return 2;
}

bool Game(void)
{
	if (!LoadGenericData())
	{
	if (gJapanese)
		Backend_ShowMessageBox("エラー", "汎用ファイルが読めない");
	else
		Backend_ShowMessageBox("Error", "Couldn't read general purpose files");

		return false;
	}

	PlaySoundObject(7, SOUND_MODE_PLAY_LOOP);

	std::string path = "npc.tbl";

	if (!LoadNpcTable(path.c_str()))
	{
	if (gJapanese)
		Backend_ShowMessageBox("エラー", "NPCテーブルが読めない");
	else
		Backend_ShowMessageBox("Error", "Couldn't read the NPC table");

		return false;
	}
	LoadNPCTags();

	LoadStageTable();

	InitTextScript2();
	InitSkipFlags();
	InitMapData2();
	InitCreditScript();

	// if (IsKeyFile("custom"))
	// 	mode = 2;
	// else
	gGameMode = 2;
	LoadRankings();
	while (gGameMode)
	{
		if (gGameMode == 1)
			gGameMode = ModeRankings();
		if (gGameMode == 2)
			gGameMode = ModeTitle();
		if (gGameMode == 3)
			gGameMode = ModeAction();
	}

	PlaySoundObject(7, SOUND_MODE_STOP);

	EndMapData();
	EndTextScript();
	ReleaseNpcTable();
	ReleaseCreditScript();
	SaveRankingsFile();

	return true;
}


int operator"" _pix (unsigned long long int units){
	return int(units * 0x200);
}

int operator"" _pix (long double units){
	return int(units * 0x200);
}
