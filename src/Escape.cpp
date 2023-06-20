#include "Escape.h"

#include "CommonDefines.h"
#include "Draw.h"
#include "KeyControl.h"
#include "Main.h"
#include "Stage.h"

int Call_Escape(void)
{
	Rect rc = {0, 128, 208, 144};

	while (1)
	{
		// Get pressed keys
		GetTrg();

		if (gKeyTrg & KEY_ESCAPE) // Escape is pressed, quit game
		{
			gKeyTrg = 0;
			return enum_ESCRETURN_exit;
		}
		if (gKeyTrg & KEY_F1) // F1 is pressed, continue
		{
			gKeyTrg = 0;
			return enum_ESCRETURN_continue;
		}
		if (gKeyTrg & KEY_F2) // F2 is pressed, reset
		{
			gKeyTrg = 0;
			return enum_ESCRETURN_restart;
		}
		if (gKeyTrg & KEY_F3){
			ChangeMusic(MUS_MELTDOWN2);
		}

		// Draw screen
		CortBox(&grcFull, 0x000000);
		PutBitmap3(&grcFull, PixelToScreenCoord((WINDOW_WIDTH / 2) - 104), PixelToScreenCoord((WINDOW_HEIGHT / 2) - 8), &rc, SURFACE_ID_TEXT_BOX);
		PutFramePerSecound();

		if (!Flip_SystemTask())
		{
			// Quit if window is closed
			gKeyTrg = 0;
			return enum_ESCRETURN_exit;
		}
	}

	return enum_ESCRETURN_exit;
}
