#include "NpChar.h"

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>

#include "ArmsItem.h"
#include "Boss.h"
#include "Bullet.h"
#include "Caret.h"
#include "Draw.h"
#include "File.h"
#include "Flags.h"
#include "Game.h"
#include "Main.h"
#include "Map.h"
#include "MyChar.h"
#include "MycHit.h"
#include "NpcHit.h"
#include "NpcTbl.h"
#include "ResourceCode.h"
#include "Stage.h"
#include "Sound.h"
#include "ValueView.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

std::map<int, std::map<int, int>> static_iframes;
std::map<int, std::map<int, int>> local_iframes;

NPCHAR gNPC[NPC_MAX];
int gCurlyShoot_wait;
int gCurlyShoot_x;
int gCurlyShoot_y;
int gSuperXpos;
int gSuperYpos;

const char* const gPassPixEve = "PXE";

NPCHAR* GetClosest(int x, int y, int maxDistance){
    float shortestdist = 0.0;
    int npc = -1;
	maxDistance /= 1_pix;
	float actualMaxDist = maxDistance*maxDistance + maxDistance*maxDistance;
    for (int i = 0; i < NPC_MAX; i++){
        if (!((gNPC[i].cond & 0x80) && (gNPC[i].bits & NPC_SHOOTABLE && gNPC[i].code_char != 100 && std::find(etherealEntities.begin(), etherealEntities.end(), gNPC[i].code_char) == etherealEntities.end()))) continue;
        float distx = (gNPC[i].x - x) / 1_pix;
        float disty = (gNPC[i].y - y) / 1_pix;
		float dist = (distx*distx + disty*disty);
		if (dist < actualMaxDist){
			if (npc < 0 || (dist < shortestdist)){
				npc = i;
				shortestdist = dist;
			}
		}
    }
    if (npc == -1){
		return NULL;
	}
    else return &gNPC[npc];
}

static void SetUniqueParameter(NPCHAR *npc)
{
	int code = npc->code_char;
	npc->surf = (SurfaceID)gNpcTable[code].surf;
	npc->hit_voice = gNpcTable[code].hit_voice;
	npc->destroy_voice = gNpcTable[code].destroy_voice;
	npc->damage = gNpcTable[code].damage;
	npc->size = gNpcTable[code].size;
	npc->life = gNpcTable[code].life;
	float lifeMod = 1.0f;
	switch (gMC.difficulty){
		case DIFFICULTY_HARDENED: lifeMod = 1.2f; break;
		case DIFFICULTY_REINFORCED: lifeMod = 1.33f; break;
		case DIFFICULTY_RESONANT: lifeMod = 1.33f; break;
	}
	if (std::find(lifeImmuneEnemies.begin(), lifeImmuneEnemies.end(), code) == lifeImmuneEnemies.end())
		npc->life *= lifeMod;
	npc->hit.front = gNpcTable[code].hit.front * 0x200;
	npc->hit.back = gNpcTable[code].hit.back * 0x200;
	npc->hit.top = gNpcTable[code].hit.top * 0x200;
	npc->hit.bottom = gNpcTable[code].hit.bottom * 0x200;
	npc->view.front = gNpcTable[code].view.front * 0x200;
	npc->view.back = gNpcTable[code].view.back * 0x200;
	npc->view.top = gNpcTable[code].view.top * 0x200;
	npc->view.bottom = gNpcTable[code].view.bottom * 0x200;
	npc->knockEffect = 1.0f;
	npc->opacity = 1.0f;
	if (GetNPCFlag(Flags::QUATE_CHARACTER) && npc->code_char != 86){
		npc->exp *= float(Random(40, 250)) / 100.0f;
	}
}

void InitNpChar(void)
{
	memset(gNPC, 0, sizeof(gNPC));

	for (int i = 0; i < NPC_MAX; i++){
		static_iframes[i] = {};
		local_iframes[i] = {};
	}
	
	for (auto storage : static_iframes){
		storage.second.clear();
	}

	for (auto storage : local_iframes){
		storage.second.clear();
	}
}

bool LoadEvent(const char *path_event)
{
	int i, n;
	FILE *fp;
	int count;
	char code[4];
	EVENT eve;

	std::string path = path_event;

	fp = GetResource(path);
	if (fp == NULL)
		return false;

	// Read "PXE" check
	fread(code, 1, 4, fp);
	if (memcmp(code, gPassPixEve, 3) != 0)
	{
#ifdef FIX_BUGS
		// The original game forgot to close the file here
		fclose(fp);
#endif
		return false;
	}

	// Get amount of NPCs
	count = File_ReadLE32(fp);

	// Load NPCs
	memset(gNPC, 0, sizeof(gNPC));

	n = 170;
	for (i = 0; i < count; ++i)
	{
		// Get data from file
		eve.x = File_ReadLE16(fp);
		eve.y = File_ReadLE16(fp);
		eve.code_flag = File_ReadLE16(fp);
		eve.code_event = File_ReadLE16(fp);
		eve.code_char = File_ReadLE16(fp);
		eve.bits = File_ReadLE16(fp);

		// Set NPC parameters
		gNPC[n].direct = (eve.bits & NPC_SPAWN_IN_OTHER_DIRectION) ? 2 : 0;
		gNPC[n].code_char = eve.code_char;
		gNPC[n].code_event = eve.code_event;
		gNPC[n].code_flag = eve.code_flag;
		gNPC[n].x = eve.x * 0x10 * 0x200;
		gNPC[n].y = eve.y * 0x10 * 0x200;
		gNPC[n].bits = eve.bits;
		gNPC[n].bits |= gNpcTable[gNPC[n].code_char].bits;
		gNPC[n].exp = gNpcTable[gNPC[n].code_char].exp;
		gNPC[n].firstFrame = true;
		SetUniqueParameter(&gNPC[n]);

		if (gNPC[n].code_event == 0 && gNPC[n].code_char == 15){
			if (gAvailableIDs.size() > 0)
			{
				int chestID, dropID, tscEvent;
				chestID = Random(0, gAvailableIDs.size() - 1, RandomCategories::SEEDED_CHESTS);
				dropID = gAvailableIDs[chestID];
				tscEvent = ((dropID - 201) / 100);
				gNPC[n].code_event = 200 + tscEvent;
				SetNPCFlag(dropID - 1);
				std::vector<int>::iterator it = gAvailableIDs.begin();
				it += chestID;
				gAvailableIDs.erase(it);
			}
		}

		// Check flags
		if (gNPC[n].bits & NPC_APPEAR_WHEN_FLAG_SET)
		{
			if (GetNPCFlag(gNPC[n].code_flag) == true)
				gNPC[n].cond |= 0x80;
		}
		else if (gNPC[n].bits & NPC_HIDE_WHEN_FLAG_SET)
		{
			if (GetNPCFlag(gNPC[n].code_flag) == false)
				gNPC[n].cond |= 0x80;
		}
		else
		{
			gNPC[n].cond = 0x80;
		}

		// Increase index
		++n;
	}

	fclose(fp);
	return true;
}

void SetNpChar(int code_char, int x, int y, int xm, int ym, int dir, NPCHAR *npc, int start_index)
{
	int n = start_index;
	while (n < NPC_MAX && gNPC[n].cond)
		++n;

	if (n == NPC_MAX)
		return;

	// Set NPC parameters
	memset(&gNPC[n], 0, sizeof(NPCHAR));
	gNPC[n].cond |= 0x80;
	gNPC[n].firstFrame = true;
	gNPC[n].direct = dir;
	gNPC[n].code_char = code_char;
	gNPC[n].x = x;
	gNPC[n].y = y;
	gNPC[n].xm = xm;
	gNPC[n].ym = ym;
	gNPC[n].pNpc = npc;
	gNPC[n].bits = gNpcTable[gNPC[n].code_char].bits;
	gNPC[n].exp = gNpcTable[gNPC[n].code_char].exp;
	SetUniqueParameter(&gNPC[n]);
}

NPCHAR* CreateNpChar(int code_char, int x, int y, int xm, int ym, int dir, NPCHAR *npc, int start_index)
{
	int n = start_index;
	while (n < NPC_MAX && gNPC[n].cond)
		++n;

	if (n == NPC_MAX)
		return NULL;

	// Set NPC parameters
	memset(&gNPC[n], 0, sizeof(NPCHAR));
	gNPC[n].cond |= 0x80;
	gNPC[n].firstFrame = true;
	gNPC[n].direct = dir;
	gNPC[n].code_char = code_char;
	gNPC[n].x = x;
	gNPC[n].y = y;
	gNPC[n].xm = xm;
	gNPC[n].ym = ym;
	gNPC[n].pNpc = npc;
	gNPC[n].bits = gNpcTable[gNPC[n].code_char].bits;
	gNPC[n].exp = gNpcTable[gNPC[n].code_char].exp;
	SetUniqueParameter(&gNPC[n]);
	return &gNPC[n];
}

void SetNpChar_EventFlag(int code_char, int x, int y, int xm, int ym, int dir, NPCHAR *npc, int event, int event_flag)
{
	int n = 0x100;
	while (n < NPC_MAX && gNPC[n].cond)
		++n;

	if (n == NPC_MAX)
		return;

	// Set NPC parameters
	memset(&gNPC[n], 0, sizeof(NPCHAR));
	gNPC[n].cond |= 0x80;
	gNPC[n].firstFrame = true;
	gNPC[n].direct = dir;
	gNPC[n].code_char = code_char;
	gNPC[n].x = x;
	gNPC[n].y = y;
	gNPC[n].xm = xm;
	gNPC[n].ym = ym;
	gNPC[n].pNpc = npc;
	gNPC[n].bits = gNpcTable[gNPC[n].code_char].bits;
	gNPC[n].exp = gNpcTable[gNPC[n].code_char].exp;
	gNPC[n].code_event = event;
	gNPC[n].code_flag = event_flag;
	SetUniqueParameter(&gNPC[n]);
}

void SetNpChar(int code_char, int x, int y, int xm, int ym, int dir, NPCHAR *npc, int start_index, int event, int flags)
{
	int n = start_index;
	while (n < NPC_MAX && gNPC[n].cond)
		++n;

	if (n == NPC_MAX)
		return;

	// Set NPC parameters
	memset(&gNPC[n], 0, sizeof(NPCHAR));
	gNPC[n].cond |= 0x80;
	gNPC[n].firstFrame = true;
	gNPC[n].direct = dir;
	gNPC[n].code_char = code_char;
	gNPC[n].x = x;
	gNPC[n].y = y;
	gNPC[n].xm = xm;
	gNPC[n].ym = ym;
	gNPC[n].pNpc = npc;
	gNPC[n].bits = gNpcTable[gNPC[n].code_char].bits;
	gNPC[n].exp = gNpcTable[gNPC[n].code_char].exp;
    gNPC[n].code_event = event;
    gNPC[n].bits |= flags;
	SetUniqueParameter(&gNPC[n]);
}

void SetNpChar(int code_char, int x, int y, int xm, int ym, int dir, NPCHAR *npc, int start_index, int event, int flags, int event_flag)
{
	int n = start_index;
	while (n < NPC_MAX && gNPC[n].cond)
		++n;

	if (n == NPC_MAX)
		return;

	// Set NPC parameters
	memset(&gNPC[n], 0, sizeof(NPCHAR));
	gNPC[n].cond |= 0x80;
	gNPC[n].firstFrame = true;
	gNPC[n].direct = dir;
	gNPC[n].code_char = code_char;
	gNPC[n].x = x;
	gNPC[n].y = y;
	gNPC[n].xm = xm;
	gNPC[n].ym = ym;
	gNPC[n].pNpc = npc;
	gNPC[n].bits = gNpcTable[gNPC[n].code_char].bits;
	gNPC[n].exp = gNpcTable[gNPC[n].code_char].exp;
    gNPC[n].code_event = event;
    gNPC[n].bits = flags;
	gNPC[n].code_flag = event_flag;
	SetUniqueParameter(&gNPC[n]);
}

void SetDestroyNpChar(int x, int y, int w, int num)
{
	int i;
	int offset_x;
	int offset_y;

	// Create smoke
	w /= 0x200;
	for (i = 0; i < num; ++i)
	{
		offset_x = w <= 0 ? 0 : Random(-w, w) * 0x200;
		offset_y = w <= 0 ? 0 : Random(-w, w) * 0x200;
		SetNpChar(NPC_SMOKE, x + offset_x, y + offset_y, 0, 0, 0, NULL, 0x100);
	}

	// Flash effect
	SetCaret(x, y, 12, 0);
}

void SetDestroyNpCharUp(int x, int y, int w, int num)
{
	int i;
	int offset_x;
	int offset_y;

	// Create smoke
	w /= 0x200;
	for (i = 0; i < num; ++i)
	{
		offset_x = Random(-w, w) * 0x200;
		offset_y = Random(-w, w) * 0x200;
		SetNpChar(4, x + offset_x, y + offset_y, 0, 0, 1, NULL, 0x100);
	}

	// Flash effect
	SetCaret(x, y, 12, 0);
}

void SetExpObjects(int x, int y, int exp)
{
	int n;
	int sub_exp;

	if (ChallengeEnabled("no_exp"))
		return;

	n = 0x100;
	while (exp)
	{
		while (n < NPC_MAX && gNPC[n].cond)
			++n;

		if (n == NPC_MAX)
			break;

		memset(&gNPC[n], 0, sizeof(NPCHAR));

		if (exp >= 20)
		{
			exp -= 20;
			sub_exp = 20;
		}
		else if (exp >= 5)
		{
			exp -= 5;
			sub_exp = 5;
		}
		else if (exp >= 1)
		{
			exp -= 1;
			sub_exp = 1;
		}

		gNPC[n].cond |= 0x80;
		gNPC[n].firstFrame = true;
		gNPC[n].direct = 0;
		gNPC[n].code_char = 1;
		gNPC[n].x = x;
		gNPC[n].y = y;
		gNPC[n].bits = gNpcTable[gNPC[n].code_char].bits;
		gNPC[n].exp = sub_exp;

		SetUniqueParameter(&gNPC[n]);
	}
}

bool SetBulletObject(int x, int y, int val)
{
	int n;
	int bullet_no;	// The Linux debug data claims there's a 3-line gap between this and the next variable declaration. Just enough space for an 'if' statement.

	// if (/* unknown */)
	{	// This is necessary for accurate ASM (stack frame layout)
		int tamakazu_ari[10];
		int t = 0;

		memset(tamakazu_ari, 0, sizeof(tamakazu_ari));

		for (n = 0; n < 8; ++n)
		{
			switch (gArmsData[n].code)
			{
				case 5:
					tamakazu_ari[t++] = 0;
					break;

				case 10:
					tamakazu_ari[t++] = 1;
					break;

				default:
					tamakazu_ari[t] = 0;
					break;
			}
		}

		if (t == 0)
			return false;

		n = Random(1, 10 * t);
		bullet_no = tamakazu_ari[n % t];

		n = 0x100;
		while (n < NPC_MAX && gNPC[n].cond)
			++n;

		if (n == NPC_MAX)
			return false;

		memset(&gNPC[n], 0, sizeof(NPCHAR));
		gNPC[n].cond |= 0x80;
		gNPC[n].firstFrame = true;
		gNPC[n].direct = 0;
		gNPC[n].code_event = bullet_no;
		gNPC[n].code_char = 86;
		gNPC[n].x = x;
		gNPC[n].y = y;
		gNPC[n].bits = gNpcTable[gNPC[n].code_char].bits;
		gNPC[n].exp = val;
		SetUniqueParameter(&gNPC[n]);
	}

	return true;
}

bool SetLifeObject(int x, int y, int val)
{
	int n = 0x100;
	while (n < NPC_MAX && gNPC[n].cond)
		++n;

	if (n == NPC_MAX)
		return false;

	memset(&gNPC[n], 0, sizeof(NPCHAR));
	gNPC[n].cond |= 0x80;
	gNPC[n].firstFrame = true;
	gNPC[n].direct = 0;
	gNPC[n].code_char = 87;
	gNPC[n].x = x;
	gNPC[n].y = y;
	gNPC[n].bits = gNpcTable[gNPC[n].code_char].bits;
	gNPC[n].exp = val;
	SetUniqueParameter(&gNPC[n]);
	return true;
}

void VanishNpChar(NPCHAR *npc)
{
	int x, y;

	x = npc->x;
	y = npc->y;
	memset(npc, 0, sizeof(NPCHAR));
	npc->count1 = 0;
	npc->x = x;
	npc->y = y;
	npc->cond |= 0x80;
	npc->direct = 0;
	npc->code_char = 3;
	npc->bits = gNpcTable[npc->code_char].bits;
	npc->exp = gNpcTable[npc->code_char].exp;
	SetUniqueParameter(npc);
}

Rect targeting_visual_rect[2] = {
		{80, 120, 104, 144},
		{104, 120, 128, 144},
};

void PutNpChar(int fx, int fy)
{
	int n;
	signed char a = 0;

	int side;

	for (n = 0; n < NPC_MAX; ++n)
	{
		if (gNPC[n].cond & 0x80 && !(gNPC[n].bits & NPC_RENDER_ABOVE) && gNPC[n].code_char != 30)
		{
			if (gNPC[n].shock)
			{
				a = 2 * ((gNPC[n].shock / 2) % 2) - 1;
			}
			else
			{
				a = 0;
				if (gNPC[n].bits & NPC_SHOW_DAMAGE && gNPC[n].damage_view)
				{
					SetValueView(&gNPC[n].x, &gNPC[n].y, gNPC[n].damage_view);
					gNPC[n].damage_view = 0;
				}
			}

			if (ChallengeEnabled("darkness") && Distance(gMC.x / 0x2000, gMC.y / 0x2000, gNPC[n].x / 0x2000, gNPC[n].y / 0x2000) > 13)
				continue;

			if (gNPC[n].direct == 0)
				side = gNPC[n].view.front;
			else
				side = gNPC[n].view.back;

			PutBitmap3(
				&grcGame,
				SubpixelToScreenCoord(gNPC[n].x - side) - SubpixelToScreenCoord(fx) + PixelToScreenCoord(a),
				SubpixelToScreenCoord(gNPC[n].y - gNPC[n].view.top) - SubpixelToScreenCoord(fy),
				&gNPC[n].rect,
				(SurfaceID)gNPC[n].surf,
				gNPC[n].opacity);
			if (gNPC[n].targetingEffect > 0){
				int timing = 0;
				if (gNPC[n].targetingEffect / 2 % 2)
					timing = 0;
				else
					timing = 1;
				PutBitmap3(
					&grcGame,
					SubpixelToScreenCoord(gNPC[n].x - 12_pix) - SubpixelToScreenCoord(fx) + PixelToScreenCoord(a),
					SubpixelToScreenCoord(gNPC[n].y - 12_pix) - SubpixelToScreenCoord(fy),
					&targeting_visual_rect[timing],
					(SurfaceID)SURFACE_ID_CARET,
					gNPC[n].opacity);
			}
			if (ChallengeEnabled("hitboxes"))
			{
				int mod = 1;
				if (GetNPCFlag(Flags::MAXY_CHARACTER) && gNPC[n].damage)
				{
					mod = 3;
				}
				int hitX = gNPC[n].x - gNPC[n].hit.back / mod - fx;
				int hitY = gNPC[n].y - gNPC[n].hit.top / mod - fy;

				Rect hitbox = {
					hitX / 0x200,
					hitY / 0x200,
					(hitX + gNPC[n].hit.back / mod + gNPC[n].hit.front / mod) / 0x200,
					(hitY + gNPC[n].hit.top / mod + gNPC[n].hit.bottom / mod) / 0x200,
				};
				CortBox2(&hitbox, 0x7F0000FF);
			}
		}
	}
}

void PutNpChar_Above(int fx, int fy)
{
	int n;
	signed char a = 0;

	int side;

	for (n = 0; n < NPC_MAX; ++n)
	{
		if (gNPC[n].cond & 0x80 && gNPC[n].bits & NPC_RENDER_ABOVE)
		{
			if (gNPC[n].shock)
			{
				a = 4 * ((gNPC[n].shock / 2) % 2) - 1;
			}
			else
			{
				a = 0;
			}
			if (gNPC[n].direct == 0)
				side = gNPC[n].view.front;
			else
				side = gNPC[n].view.back;

			PutBitmap3(
				&grcGame,
				SubpixelToScreenCoord(gNPC[n].x - side) - SubpixelToScreenCoord(fx) + PixelToScreenCoord(a),
				SubpixelToScreenCoord(gNPC[n].y - gNPC[n].view.top) - SubpixelToScreenCoord(fy),
				&gNPC[n].rect,
				(SurfaceID)gNPC[n].surf);
		}
	}
}

void PutGrass(int fx, int fy)
{
	int n;
	signed char a = 0;

	int side;

	for (n = 0; n < NPC_MAX; ++n)
	{
		if (gNPC[n].cond & 0x80 && (gNPC[n].code_char == 30 || gNPC[n].code_char == 299))
		{
			if (gNPC[n].shock)
			{
				a = 4 * ((gNPC[n].shock / 2) % 2) - 1;
			}
			else
			{
				a = 0;
			}

			if (gNPC[n].direct == 0)
				side = gNPC[n].view.front;
			else
				side = gNPC[n].view.back;

			PutBitmap3(
				&grcGame,
				SubpixelToScreenCoord(gNPC[n].x - side) - SubpixelToScreenCoord(fx) + PixelToScreenCoord(a),
				SubpixelToScreenCoord(gNPC[n].y - gNPC[n].view.top) - SubpixelToScreenCoord(fy),
				&gNPC[n].rect,
				(SurfaceID)gNPC[n].surf);
		}
	}
}

static int count;

void ActNpChar(void)
{
	int i;
	int code_char;

	for (i = 0; i < NPC_MAX; ++i)
	{
		if (gNPC[i].cond & 0x80)
		{
			code_char = gNPC[i].code_char;
			if (gNPC[i].firstFrame || !TimeStopActive(code_char))
			{
				gNPC[i].firstFrame = false;
				if (gNPC[i].freeze > 0){
					for (int l = 0; l < 3 + MAX((gNPC[i].freeze-100)/30, 0); l++){
						SetCaret(gNPC[i].x + Random(-gNPC[i].view.front, gNPC[i].view.back),
						gNPC[i].y + Random(-gNPC[i].view.top, gNPC[i].view.bottom), 13, 0);
					}
				}
				if (gNPC[i].freeze > 0 && Random(0, 3 + MAX((gNPC[i].freeze-100)/10, 0)) > 1){
					
				}
				else {
					gpNpcFuncTbl[code_char](&gNPC[i]);
					if (gNPC[i].knockx != 0){
						gNPC[i].x += gNPC[i].knockx;
						gNPC[i].knockx *= 0.9;
					}
					if (gNPC[i].knocky != 0){
						gNPC[i].y += gNPC[i].knocky;
						gNPC[i].knocky *= 0.9;
					}
				}
				gNPC[i].freeze--;
				if (gNPC[i].freeze <= 0) gNPC[i].freeze = 0;

				if (gNPC[i].targetingEffect > 0){
					gNPC[i].targetingEffect++;
				}

				if (gNPC[i].shock){
					--gNPC[i].shock;
				}
			}
		} else {
			if (static_iframes.contains(i) && static_iframes[i].size() > 0){
				static_iframes[i].clear();
			}
			if (local_iframes.contains(i) && static_iframes[i].size() > 0){
				static_iframes[i].clear();
			}
		}
	}
	for (auto& npc: static_iframes){
		for (auto& iframes: npc.second){
			if (iframes.second > 0){
				iframes.second--;
			}
		}
	}
	for (auto& npc: local_iframes){
		for (auto& iframes: npc.second){
			if (iframes.second > 0){
				iframes.second--;
			}
		}
	}
}

void ChangeNpCharByEvent(int code_event, int code_char, int dir)
{
	int n;

	for (n = 0; n < NPC_MAX; ++n)
	{
		if ((gNPC[n].cond & 0x80) && gNPC[n].code_event == code_event)
		{
			gNPC[n].bits &= ~(NPC_SOLID_SOFT | NPC_IGNORE_TILE_44 | NPC_INVULNERABLE | NPC_IGNORE_SOLIDITY | NPC_BOUNCY | NPC_SHOOTABLE | NPC_SOLID_HARD | NPC_REAR_AND_TOP_DONT_HURT | NPC_SHOW_DAMAGE);	// Clear these flags
			gNPC[n].code_char = code_char;
			gNPC[n].firstFrame = true;
			gNPC[n].bits |= gNpcTable[gNPC[n].code_char].bits;
			gNPC[n].exp = gNpcTable[gNPC[n].code_char].exp;
			SetUniqueParameter(&gNPC[n]);
			gNPC[n].cond |= 0x80;
			gNPC[n].act_no = 0;
			gNPC[n].act_wait = 0;
			gNPC[n].count1 = 0;
			gNPC[n].count2 = 0;
			gNPC[n].ani_no = 0;
			gNPC[n].ani_wait = 0;
			gNPC[n].xm = 0;
			gNPC[n].ym = 0;

			if (dir == 5)
			{
				// Another empty case that has to exist for the same assembly to be generated
			}
			else if (dir == 4)
			{
				if (gNPC[n].x < gMC.x)
					gNPC[n].direct = 2;
				else
					gNPC[n].direct = 0;
			}
			else
			{
				gNPC[n].direct = dir;
			}

			gpNpcFuncTbl[code_char](&gNPC[n]);
		}
	}
}

void ChangeCheckableNpCharByEvent(int code_event, int code_char, int dir)
{
	int n;

	for (n = 0; n < NPC_MAX; ++n)
	{
		if (!(gNPC[n].cond & 0x80) && gNPC[n].code_event == code_event)
		{
			gNPC[n].bits &= ~(NPC_SOLID_SOFT | NPC_IGNORE_TILE_44 | NPC_INVULNERABLE | NPC_IGNORE_SOLIDITY | NPC_BOUNCY | NPC_SHOOTABLE | NPC_SOLID_HARD | NPC_REAR_AND_TOP_DONT_HURT | NPC_SHOW_DAMAGE);	// Clear these flags
			gNPC[n].bits |= NPC_INTERACTABLE;
			gNPC[n].code_char = code_char;
			gNPC[n].firstFrame = true;
			gNPC[n].bits |= gNpcTable[gNPC[n].code_char].bits;
			gNPC[n].exp = gNpcTable[gNPC[n].code_char].exp;
			SetUniqueParameter(&gNPC[n]);
			gNPC[n].cond |= 0x80;
			gNPC[n].act_no = 0;
			gNPC[n].act_wait = 0;
			gNPC[n].count1 = 0;
			gNPC[n].count2 = 0;
			gNPC[n].ani_no = 0;
			gNPC[n].ani_wait = 0;
			gNPC[n].xm = 0;
			gNPC[n].ym = 0;

			if (dir == 5)
			{
				// Another empty case that has to exist for the same assembly to be generated
			}
			else if (dir == 4)
			{
				if (gNPC[n].x < gMC.x)
					gNPC[n].direct = 2;
				else
					gNPC[n].direct = 0;
			}
			else
			{
				gNPC[n].direct = (signed char)dir;
			}

			gpNpcFuncTbl[code_char](&gNPC[n]);
		}
	}
}

void SetNpCharActionNo(int code_event, int act_no, int dir)
{
	int n = 0;
	while (n < NPC_MAX)
	{
		if ((gNPC[n].cond & 0x80) && gNPC[n].code_event == code_event)
			break;

		++n;
	}

	if (n == NPC_MAX)
		return;

	gNPC[n].act_no = act_no;

	if (dir == 5)
	{
		// Another empty case that has to exist for the same assembly to be generated
	}
	else if (dir == 4)
	{
		if (gNPC[n].x < gMC.x)
			gNPC[n].direct = 2;
		else
			gNPC[n].direct = 0;
	}
	else
	{
		gNPC[n].direct = dir;
	}
}

void MoveNpChar(int code_event, int x, int y, int dir)
{
	int n = 0;
	while (n < NPC_MAX)
	{
		if ((gNPC[n].cond & 0x80) && gNPC[n].code_event == code_event)
			break;

		++n;
	}

	if (n == NPC_MAX)
		return;

	gNPC[n].x = x;
	gNPC[n].y = y;

	if (dir == 5)
	{
		// Another empty case that has to exist for the same assembly to be generated
	}
	else if (dir == 4)
	{
		if (gNPC[n].x < gMC.x)
			gNPC[n].direct = 2;
		else
			gNPC[n].direct = 0;
	}
	else
	{
		gNPC[n].direct = (signed char)dir;
	}
}

void BackStepMyChar(int code_event)
{
	int n = 0;

	gMC.cond &= ~1;
	gMC.ym = -0x200;

	if (code_event == 0)
	{
		gMC.direct = 0;
		gMC.xm = 0x200;
	}
	else if (code_event == 2)
	{
		gMC.direct = 2;
		gMC.xm = -0x200;
	}
	else
	{
		while (n < NPC_MAX)
		{
			if ((gNPC[n].cond & 0x80) && gNPC[n].code_event == code_event)
				break;

			++n;
		}

		if (n == NPC_MAX)
			return;

		if (gNPC[n].x < gMC.x)
		{
			gMC.direct = 0;
			gMC.xm = 0x200;
		}
		else
		{
			gMC.direct = 2;
			gMC.xm = -0x200;
		}
	}
}

void DeleteNpCharEvent(int code)
{
	int i;

	for (i = 0; i < NPC_MAX; ++i)
	{
		if ((gNPC[i].cond & 0x80) && gNPC[i].code_event == code)
		{
			gNPC[i].cond = 0;
			SetNPCFlag(gNPC[i].code_flag);
		}
	}
}

void DeleteNpCharCode(int code, bool bSmoke)
{
	int n;

	for (n = 0; n < NPC_MAX; ++n)
	{
		if ((gNPC[n].cond & 0x80) && gNPC[n].code_char == code)
		{
			gNPC[n].cond = 0;
			SetNPCFlag(gNPC[n].code_flag);

			if (bSmoke)
			{
				PlaySoundObject(gNPC[n].destroy_voice, SOUND_MODE_PLAY);

				switch (gNPC[n].size)
				{
					case 1:
						SetDestroyNpChar(gNPC[n].x, gNPC[n].y, gNPC[n].view.back, 4);
						break;

					case 2:
						SetDestroyNpChar(gNPC[n].x, gNPC[n].y, gNPC[n].view.back, 8);
						break;

					case 3:
						SetDestroyNpChar(gNPC[n].x, gNPC[n].y, gNPC[n].view.back, 16);
						break;
				}
			}
		}
	}
}

void GetNpCharPosition(int *x, int *y, int i)
{
	*x = gNPC[i].x;
	*y = gNPC[i].y;
}

bool IsNpCharCode(int code)
{
	int i;

	for (i = 0; i < NPC_MAX; ++i)
		if ((gNPC[i].cond & 0x80) && gNPC[i].code_char == code)
			return true;

	return false;
}

int CheckForIDOfNpChar(NPCHAR* npc)
{
	int i;

	for (i = 0; i < NPC_MAX; ++i)
		if (&gNPC[i] == npc)
			return i;

	return -1;
}

int CountNpCharCode(int code)
{
	int i, l;
	l = 0;

	for (i = 0; i < NPC_MAX; ++i)
		if ((gNPC[i].cond & 0x80) && gNPC[i].code_char == code)
			l++;

	return l;
}

int GetNpCharHere(int x, int y)
{
	int i;

	for (i = 0; i < NPC_MAX; ++i)
		if ((gNPC[i].cond & 0x80) && (gNPC[i].x / 0x2000  == x) && gNPC[i].y / 0x2000 == y)
			return gNPC[i].code_char;

	return -1;
}

NPCHAR* FindNpCharHere(int x, int y, int xl, int yl)
{
	int i;

	for (i = 0; i < NPC_MAX; ++i)
		if ((gNPC[i].cond & 0x80) && IntersectDamageCheck(gNPC[i], x, y, xl, yl))
			return &gNPC[i];

	for (i = 0; i < BOSS_MAX; ++i)
		if ((gBoss[i].cond & 0x80) && IntersectDamageCheck(gBoss[i], x, y, xl, yl)){
			if (gBoss[i].cond & 0x10)
				i = 0;
			return &gBoss[i];
		}

	return NULL;
}

bool GetNpCharAlive(int code_event)
{
	int i;

	for (i = 0; i < NPC_MAX; ++i)
		if ((gNPC[i].cond & 0x80) && gNPC[i].code_event == code_event)
			break;

	if (i < NPC_MAX)
		return true;
	else
		return false;
}

int CountAliveNpChar(void)
{
	int n;
	int count = 0;

	for (n = 0; n < NPC_MAX; ++n)
		if (gNPC[n].cond & 0x80)
			++count;

	return count;
}
