// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "MinimapEx.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "Back.h"
#include "Draw.h"
#include "Escape.h"
#include "Flags.h"
#include "Generic.h"
#include "KeyControl.h"
#include "Main.h"
#include "Map.h"
#include "MapName.h"
#include "MyChar.h"
#include "MycParam.h"
#include "NpChar.h"
#include "Pause.h"
#include "Stage.h"

#define MAP_WIDTH 70
#define MAP_HEIGHT 70

int my_flash;

int WriteMiniMap(int x, int y){
	unsigned char a;
    unsigned char b;

	Rect rcLevel[] = {
		// tileset
		{212, 103, 219, 109},
		{220, 103, 226, 109},
		{228, 103, 234, 109},
		{236, 103, 242, 109},
		// exits,
		{236, 111, 242, 117},
		// items,
		{228, 111, 234, 117},
		// save disk
		{236, 119, 242, 125},
		// life station
		{228, 119, 234, 126},
		//blank tile
		{212, 111, 220, 117},
        // star
        {212, 119, 219, 126},
        // life capsule
        {220, 119, 227, 125},
        //inner terrain
        {236, 97, 242, 103},
        // chain
        {228, 85, 236, 91},
        {228, 91, 236, 97},
        {228, 97, 236, 103},
        //spike
        {222, 97, 228, 103},
        //alternative inner terrain
        {236, 91, 242, 97},
        //pot
        {216, 97, 222, 103},
        //alternate tiles
        {222, 91, 228, 97},
        {222, 85, 228, 91},
        //alternate back tiles
        {210, 85, 216, 91},
        {210, 91, 216, 97},
        {210, 97, 216, 103},
        //ceiling
        {216, 91, 222, 97},
        //press
        {216, 85, 222, 91},
        //boss
        {236, 85, 242, 91},
		//miracle
		{212, 125, 218, 131},
		//PC
		{220, 125, 226, 131},
		//Platforms
		{227, 125, 233, 128}
	};

	for(int m_y = y - MAP_HEIGHT; m_y <= y + MAP_HEIGHT; m_y ++){
		for(int m_x = x - MAP_WIDTH; m_x <= x + MAP_WIDTH; m_x ++){
			int mini_x = m_x - (x - MAP_WIDTH);
			int mini_y = m_y - (y - MAP_HEIGHT);

			if(m_x < 0 || m_x >= gMap.width || m_y < 0 || m_y >= gMap.length){
				Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[8], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
				continue;
			}

			a = GetAttribute(m_x, m_y);
            b = GetTile(m_x,m_y);

			// Yup, this really is an if/else chain.
			// No switch here.
			if (a == 0){
				if (gWaterY / 0x2000 < m_y)
				    Surface2Surface(mini_x*6, mini_y*6, &rcLevel[1], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
			    else
				    Surface2Surface(mini_x*6, mini_y*6, &rcLevel[0], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
                // if (b == 68)
                //      Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[2], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
                if (b == 43)
                     Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[12], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
                if (b == 44)
                     Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[13], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
                if (b == 45)
                     Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[14], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
                if (b == 72)
                     Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[20], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
                if (b == 73)
                     Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[21], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
                if (b == 74)
                     Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[22], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
                if (b == 57)
                     Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[23], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
            }
			else if (a == 68 ||
				a == 1 ||
				a == 64 ||
				a == 69 ||
				a == 128 ||
				a == 129 ||
				a == 130 ||
				a == 131 ||
				a == 81 ||
				a == 82 ||
				a == 85 ||
				a == 86 ||
				a == 2 ||
				a == 96 ||
				a == 113 ||
				a == 114 ||
				a == 117 ||
				a == 118 ||
				a == 160 ||
				a == 161 ||
				a == 162 ||
				a == 163){
					Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[1], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
					if (b == 13 || b == 14)
						Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[2], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
				}
            else if (a == 67 ||
				a == 99 ||
				a == 80 ||
				a == 83 ||
				a == 84 ||
				a == 87 ||
				a == 112 ||
				a == 115 ||
				a == 116 ||
				a == 119){
				Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[2], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
            }
			else {
                if (a == 66)
                    Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[15], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
                else if (b == 69)
                    Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[18], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
                else if (b == 70)
                    Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[19], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
                else
                    Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[3], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
                if (b == 34 || b == 35 || b == 50 || b == 51)
                    Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[16], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
                else if (b == 32 || b == 33 || b == 48 || b == 49)
                    Surface2Surface(mini_x * 6, mini_y * 6, &rcLevel[11], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
            }

		}
	}

	for(int i = 0; i < NPC_MAX; i++){
        if (gNPC[i].cond & 0x80){
		//doors
		if(gNPC[i].code_char == 0x12 || gNPC[i].code_char == 0x89 || gNPC[i].code_char == 0x16){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			if(npc_y >= y - MAP_HEIGHT && npc_y <= y + MAP_HEIGHT && npc_x >= x - MAP_WIDTH && npc_x <= x + MAP_WIDTH){
				int npc_max_y = npc_y - (y - MAP_HEIGHT);
				int npc_map_x = npc_x - (x - MAP_WIDTH);

				Surface2Surface(npc_map_x * 6, npc_max_y * 6, &rcLevel[4], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);

				if(gNPC[i].code_char == 0x89){ //large door is wide
					Surface2Surface((npc_map_x + 1) * 6, npc_max_y * 6, &rcLevel[4], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
				}
			}
		}

		//chests, items, life capsules
		if(gNPC[i].code_char == 0xF){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			if(npc_y >= y - MAP_HEIGHT && npc_y <= y + MAP_HEIGHT && npc_x >= x - MAP_WIDTH && npc_x <= x + MAP_WIDTH){
				int npc_max_y = npc_y - (y - MAP_HEIGHT);
				int npc_map_x = npc_x - (x - MAP_WIDTH);

				Surface2Surface(npc_map_x * 6, npc_max_y * 6, &rcLevel[5], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
			}
		}

		// save disk
		if(gNPC[i].code_char == 0x10){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			// hide if not yet spawned
			if(gNPC[i].bits & NPC_APPEAR_WHEN_FLAG_SET && !(GetNPCFlag(gNPC[i].code_flag))){
				continue;
			}

			//discard unused save scripts
			if(gNPC[i].code_event != 16){
				continue;
			}

			if(npc_y >= y - MAP_HEIGHT && npc_y <= y + MAP_HEIGHT && npc_x >= x - MAP_WIDTH && npc_x <= x + MAP_WIDTH){
				int npc_max_y = npc_y - (y - MAP_HEIGHT);
				int npc_map_x = npc_x - (x - MAP_WIDTH);

				Surface2Surface(npc_map_x * 6, npc_max_y * 6, &rcLevel[6], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
			}
		}

        // star
		if(gNPC[i].code_char == 86){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			if(npc_y >= y - MAP_HEIGHT && npc_y <= y + MAP_HEIGHT && npc_x >= x - MAP_WIDTH && npc_x <= x + MAP_WIDTH){
				int npc_max_y = npc_y - (y - MAP_HEIGHT);
				int npc_map_x = npc_x - (x - MAP_WIDTH);

				Surface2Surface(npc_map_x * 6, npc_max_y * 6, &rcLevel[9], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
			}
		}

        if(gNPC[i].code_char == 246){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			if(npc_y >= y - MAP_HEIGHT && npc_y <= y + MAP_HEIGHT && npc_x >= x - MAP_WIDTH && npc_x <= x + MAP_WIDTH){
				int npc_max_y = npc_y - (y - MAP_HEIGHT);
				int npc_map_x = npc_x - (x - MAP_WIDTH);

				Surface2Surface(npc_map_x * 6, (npc_max_y+1) * 6, &rcLevel[24], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
			}
		}

		if(gNPC[i].code_char == 253){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			if(npc_y >= y - MAP_HEIGHT && npc_y <= y + MAP_HEIGHT && npc_x >= x - MAP_WIDTH && npc_x <= x + MAP_WIDTH){
				int npc_max_y = npc_y - (y - MAP_HEIGHT);
				int npc_map_x = npc_x - (x - MAP_WIDTH);

				Surface2Surface(npc_map_x * 6, (npc_max_y) * 6, &rcLevel[26], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
			}
		}

		if(gNPC[i].code_char == 20){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			if(npc_y >= y - MAP_HEIGHT && npc_y <= y + MAP_HEIGHT && npc_x >= x - MAP_WIDTH && npc_x <= x + MAP_WIDTH){
				int npc_max_y = npc_y - (y - MAP_HEIGHT);
				int npc_map_x = npc_x - (x - MAP_WIDTH);

				Surface2Surface(npc_map_x * 6, (npc_max_y) * 6, &rcLevel[27], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
			}
		}

		if(gNPC[i].code_char == 100){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			if(npc_y >= y - MAP_HEIGHT && npc_y <= y + MAP_HEIGHT && npc_x >= x - MAP_WIDTH && npc_x <= x + MAP_WIDTH){
				int npc_max_y = npc_y - (y - MAP_HEIGHT);
				int npc_map_x = npc_x - (x - MAP_WIDTH);

				Surface2Surface(npc_map_x * 6, (npc_max_y) * 6, &rcLevel[28], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
			}
		}
        
        if(gNPC[i].code_char == 268 || gNPC[i].code_char == 276 || gNPC[i].code_char == 317 || gNPC[i].code_char == 36 || gNPC[i].code_char == 284 || gNPC[i].code_char == 44){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			if(npc_y >= y - MAP_HEIGHT && npc_y <= y + MAP_HEIGHT && npc_x >= x - MAP_WIDTH && npc_x <= x + MAP_WIDTH){
				int npc_max_y = npc_y - (y - MAP_HEIGHT);
				int npc_map_x = npc_x - (x - MAP_WIDTH);

				Surface2Surface(npc_map_x * 6, npc_max_y * 6, &rcLevel[25], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
			}
		}

        //life capsule
        if(gNPC[i].code_char == 32){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			if(npc_y >= y - MAP_HEIGHT && npc_y <= y + MAP_HEIGHT && npc_x >= x - MAP_WIDTH && npc_x <= x + MAP_WIDTH){
				int npc_max_y = npc_y - (y - MAP_HEIGHT);
				int npc_map_x = npc_x - (x - MAP_WIDTH);

				Surface2Surface(npc_map_x * 6, npc_max_y * 6, &rcLevel[10], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
			}
		}

		// life capsule
		if(gNPC[i].code_char == 0x11){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			if (gNPC[i].count1 == 1)
				continue;

			// hide if not yet spawned
			if(gNPC[i].bits & NPC_APPEAR_WHEN_FLAG_SET && !(GetNPCFlag(gNPC[i].code_flag))){
				continue;
			}

			if(npc_y >= y - MAP_HEIGHT && npc_y <= y + MAP_HEIGHT && npc_x >= x - MAP_WIDTH && npc_x <= x + MAP_WIDTH){
				int npc_max_y = npc_y - (y - MAP_HEIGHT);
				int npc_map_x = npc_x - (x - MAP_WIDTH);

				Surface2Surface(npc_map_x * 6, npc_max_y * 6, &rcLevel[7], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
			}
		}
        }
	}

	int my_x = ((gMC.x / 0x200) + 8) / 0x10;
	int my_y = ((gMC.y / 0x200) + 8) / 0x10;
	if (my_y >= y - MAP_HEIGHT && my_y <= y + MAP_HEIGHT && my_x >= x - MAP_WIDTH && my_x <= x + MAP_WIDTH)
	{
		int my_map_y = my_y - (y - MAP_HEIGHT);
		int my_map_x = my_x - (x - MAP_WIDTH);
		Rect my_rect = {220, 111, 226, 117};
		if (++my_flash / 16 % 2){
			my_rect = {0, 0, 0, 0};
		}
		Surface2Surface(my_map_x * 6, my_map_y * 6, &my_rect, SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX, 0.5f);
	}

	return enum_ESCRETURN_continue;
}

int MiniMapExLoop(void)
{
	int f;
	Rect rcView;
	Rect rcMiniMap;

	int my_x;
	int my_y;
	unsigned char my_wait;
	Rect my_rect = {220, 111, 223, 114};
	Rect rcMapCaret = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	my_x = ((gMC.x / 0x2000));
	my_y = ((gMC.y / 0x2000));

	for (f = 0; f <= 8; ++f)
	{
		GetTrg();

		if (gKey & KEY_PAUSE)
		{
			switch (Call_Pause())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;

				case enum_ESCRETURN_ranks:
					return enum_ESCRETURN_ranks;
			}
		}

		if (gKey & KEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;

				case enum_ESCRETURN_ranks:
					return enum_ESCRETURN_ranks;
			}
		}

		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);
		PutBitmap3(&grcGame, 0, 0, &grcGame, SURFACE_ID_BLACK_OVERLAY);

		rcView.left = (WINDOW_WIDTH / 2) - (((42 * f) / 8) / 2)*6;
		rcView.right = (WINDOW_WIDTH / 2) + (((42 * f) / 8) / 2)*6;
		rcView.top = (WINDOW_HEIGHT / 2) - (((22.5 * f) / 8) / 2)*6;
		rcView.bottom = (WINDOW_HEIGHT / 2) + (((22.5 * f) / 8) / 2)*6;

		CortBox(&rcView, 0);

		PutFramePerSecound();
		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;
	}

	rcMiniMap.left = 0;
	rcMiniMap.right = WINDOW_WIDTH - 16;
	rcMiniMap.top = 0;
	rcMiniMap.bottom = WINDOW_HEIGHT - 8;

	rcView.right = --rcView.left*7 + 2*7;
	rcView.bottom = --rcView.top*7 + 2*7;
	CortBox2(&rcMiniMap, 0, SURFACE_ID_MAP);

	int map_x = my_x;
	int map_y = my_y;

    static int mouseX;
    static int mouseY;

	my_wait = 0;
	while (1)
	{
		GetTrg();

		if (gKeyTrg & (gKeyOk | gKeyCancel))
			break;

		if (gKey & KEY_PAUSE)
		{
			switch (Call_Pause())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;

				case enum_ESCRETURN_ranks:
					return enum_ESCRETURN_ranks;
			}
		}

		if (gKey & KEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;

				case enum_ESCRETURN_ranks:
					return enum_ESCRETURN_ranks;
			}
		}

        if (gKeyTrg & KEY_F2){
			gKeyTrg = 0;
			DeleteSave();
			return 2;
		}

		if (gKeyTrg & KEY_F3){
			ChangeMusic(MUS_MELTDOWN2);
		}

		if(++my_wait % 2 == 0){
			if(gKey & KEY_UP){
				map_y = map_y - 2;
			}

			if(gKey & KEY_DOWN){
				map_y = map_y + 2;
			}

			if(gKey & KEY_LEFT){
				map_x = map_x - 2;
			}

			if(gKey & KEY_RIGHT){
				map_x = map_x + 2;
			}
		}

        // if (gMouseHold){
        //     // int mouse_x = gMouseX - PixelToScreenCoord((WINDOW_WIDTH / 2) - 110);
        //     // int mouse_y = gMouseY - PixelToScreenCoord((WINDOW_HEIGHT / 2) - 84);
        //     if (gMouseVelX != mouseX){
        //         map_x -= gMouseVelX;
        //         mouseX = gMouseVelX;
        //     }
        //    if (gMouseVelX != mouseY){
        //         map_y -= gMouseVelY;
        //         mouseY = gMouseVelY;
        //     }
        // }
		
		if(map_x - gMap.width / 2 < 0){
			map_x = gMap.width / 2;
		}
		
		if(map_x + gMap.width / 2 >= gMap.width){
			map_x = gMap.width - gMap.width / 2;
		}

		if(map_y - gMap.length * 5 / 10 < 0){
			map_y = gMap.length * 5 / 10;
		}
		
		if(map_y >= gMap.length ){
			map_y = gMap.length;
		}

		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);
		PutBitmap3(&grcGame, 0, 0, &grcGame, SURFACE_ID_BLACK_OVERLAY);
		// CortBox(&rcView, 0);

		WriteMiniMap(map_x, map_y);

		PutBitmap3(&grcGame, 0, 0, &rcMiniMap, SURFACE_ID_MAP);
		PutBitmap3(&grcGame, 0, 0, &rcMapCaret, SURFACE_ID_IMPROVED_MAP_FRAME);

		// if (++my_wait / 16 % 2){
		// 	int my_char_map_x = WINDOW_WIDTH/2 + (1 - (map_x - my_x))*2;
		// 	int my_char_map_y = WINDOW_HEIGHT/2 + (1 - (map_y - my_y))*2;
		// 	// if(my_char_map_x >= (WINDOW_WIDTH / 2) - 119 && my_char_map_x <= (WINDOW_WIDTH / 2) - 122 + 42*5 + 21){		
		// 	// 	if(my_char_map_y >= (WINDOW_HEIGHT / 2) - 76 && my_char_map_y <= (WINDOW_HEIGHT / 2) - 82 + 23*7){
		// 			PutBitmap3(&grcGame, PixelToScreenCoord(my_char_map_x), PixelToScreenCoord(my_char_map_y), &my_rect, SURFACE_ID_TEXT_BOX);
		// 	// 	}
		// 	// }
		// }
		PutMyScore(-12);
			
		PutFramePerSecound();
		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;
	}

	for (f = 8; f >= -1; --f)
	{
		GetTrg();

		if (gKey & KEY_PAUSE)
		{
			switch (Call_Pause())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;

				case enum_ESCRETURN_ranks:
					return enum_ESCRETURN_ranks;
			}
		}

		if (gKey & KEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;

				case enum_ESCRETURN_ranks:
					return enum_ESCRETURN_ranks;
			}
		}

        if (gKeyTrg & KEY_F2){
			gKeyTrg = 0;
			DeleteSave();
			return 2;
		}

		if (gKeyTrg & KEY_F3){
			ChangeMusic(MUS_MELTDOWN2);
		}

		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);

		rcView.left = (WINDOW_WIDTH / 2) - (((gMap.width * f) / 8) / 2);
		rcView.right = (WINDOW_WIDTH / 2) + (((gMap.width * f) / 8) / 2);
		rcView.top = (WINDOW_HEIGHT / 2) - (((gMap.length * f) / 8) / 2);
		rcView.bottom = (WINDOW_HEIGHT / 2) + (((gMap.length * f) / 8) / 2);

		CortBox(&rcView, 0);

		PutFramePerSecound();
		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;
	}

	return enum_ESCRETURN_continue;
}
