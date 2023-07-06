#include "Boss.h"

#include <cstring>

#include "CommonDefines.h"

#include "BossAlmo1.h"
#include "BossAlmo2.h"
#include "BossBallos.h"
#include "BossFrog.h"
#include "BossIronH.h"
#include "BossOhm.h"
#include "BossPress.h"
#include "BossTwinD.h"
#include "BossX.h"
#include "Bullet.h"
#include "Caret.h"
#include "Draw.h"
#include "Flags.h"
#include "Game.h"
#include "Main.h"
#include "Map.h"
#include "MyChar.h"
#include "NpChar.h"
#include "NpcHit.h"
#include "Sound.h"
#include "Star.h"
#include "TextScr.h"
#include "ValueView.h"

NPCHAR gBoss[BOSS_MAX];

std::map<int, std::map<int, int>> boss_static_iframes;
std::map<int, std::map<int, int>> boss_local_iframes;

void InitBossChar(int code)
{
	memset(gBoss, 0, sizeof(gBoss));
	gBoss[0].cond = 0x80;
	gBoss[0].code_char = code;

	for (int i = 0; i < BOSS_MAX; i++){
		boss_static_iframes[i] = {};
		boss_local_iframes[i] = {};
	}
	
	for (auto storage : boss_static_iframes){
		for (int i = 0; i < 79; i++)
			storage.second[i] = 0;
	}

	for (auto storage : boss_local_iframes){
		storage.second.clear();
	}
}

void PutBossChar(int fx, int fy)
{
	signed char a = 0;
	int b;

	int side;

	for (b = BOSS_MAX - 1; b >= 0; --b)
	{
		if (gBoss[b].cond & 0x80)
		{
			if (gBoss[b].shock != 0)
			{
				a = ((gBoss[b].shock / 2 % 2) * 2) - 1;
			}
			else
			{
				a = 0;

				if (gBoss[b].bits & NPC_SHOW_DAMAGE && gBoss[b].damage_view != 0)
				{
					SetValueView(&gBoss[b].x, &gBoss[b].y, gBoss[b].damage_view);
					gBoss[b].damage_view = 0;
				}
			}

			if (gBoss[b].direct == 0)
				side = gBoss[b].view.front;
			else
				side = gBoss[b].view.back;

			PutBitmap3(
				&grcGame,
				SubpixelToScreenCoord(gBoss[b].x - side) - SubpixelToScreenCoord(fx) + PixelToScreenCoord(a),
				SubpixelToScreenCoord(gBoss[b].y - gBoss[b].view.top) - SubpixelToScreenCoord(fy),
				&gBoss[b].rect,
				SURFACE_ID_LEVEL_SPRITESET_2);
		}
	}
}

void SetBossCharActNo(int a)
{
	gBoss[0].act_no = a;
}

void HitBossBullet(void)
{
	bool bHit;
	int bul;
	int bos;
	int bos_;

	for (bos = 0; bos < BOSS_MAX; ++bos)
	{
		if (!(gBoss[bos].cond & 0x80))
			continue;

		for (bul = 0; bul < BULLET_MAX; ++bul)
		{
			bool isSword = gBul[bul].code_bullet == 58 || gBul[bul].code_bullet == 60 || gBul[bul].code_bullet == 59;

			if (!(gBul[bul].cond & 0x80))
				continue;

			if (gBul[bul].iframes > 0 && boss_static_iframes[bos][gBul[bul].code_bullet] != 0)
				continue;

			if (gBul[bul].local_iframes > 0 && boss_local_iframes[bos].contains(bul)){
				if (local_iframes[bos][bul] != 0)
					continue;
			}

			if (gBoss[bos].shock > 0 && ((gBul[bul].code_bullet > 27 && gBul[bul].code_bullet < 31) ||
				 (gBul[bul].code_bullet == 57) || (gBul[bul].code_bullet == 69) || (GetNPCFlag(873) && gBul[bul].code_bullet > 6 && gBul[bul].code_bullet < 10 && CountBulletNum(16)) || (gBul[bul].code_bullet == 4 && gBul[bul].life > 0)))
				continue;

			if (gBul[bul].damage == -1)
				continue;

			if (gBoss[bos].code_char == 1 && gBoss[0].life > Omega_HealthValue() * 0.75f && gBul[bul].code_bullet != 77)
				continue;

			if (gBoss[bos].code_char == 1 && gBoss[0].life < Omega_HealthValue() * 0.75f && gBul[bul].code_bullet == 77)
				continue;

			if (gBul[bul].code_bullet == 77 && gBoss[bos].code_char == 48)
				continue;

			if (gBoss[bos].shock > 0 && gBoss[bos].code_event == 30)
				continue;

			if (gBoss[bos].code_char == 126 && gBul[bul].code_bullet == 46)
				continue;

			if (gBoss[bos].code_char == 100 && !(gBul[bul].code_bullet == 64 || gBul[bul].code_bullet == 65 || gBul[bul].code_bullet == 34 || gBul[bul].code_bullet == 35 ||
	 ((GetNPCFlag(4400) || GetNPCFlag(7999)) && (gBul[bul].code_bullet == 37 || gBul[bul].code_bullet == 38 || gBul[bul].code_bullet == 39))))
	 			continue;

			// Check if bullet touches boss
			bHit = false;
			if (gBoss[bos].bits & NPC_SHOOTABLE
				&& gBoss[bos].x - gBoss[bos].hit.back < gBul[bul].x + gBul[bul].enemyXL * (isSword && gBul[bul].direct == 0 ? 0.9f : 1.0f)
				&& gBoss[bos].x + gBoss[bos].hit.back > gBul[bul].x - gBul[bul].enemyXL * (isSword && gBul[bul].direct == 2 ? 0.9f : 1.0f)
				&& gBoss[bos].y - gBoss[bos].hit.top < gBul[bul].y + gBul[bul].enemyYL * (isSword ? 0.75f : 1.0f)
				&& gBoss[bos].y + gBoss[bos].hit.bottom > gBul[bul].y - gBul[bul].enemyYL)
				bHit = true;
			else if (gBoss[bos].bits & NPC_INVULNERABLE
				&& gBoss[bos].x - gBoss[bos].hit.back < gBul[bul].x + gBul[bul].blockXL
				&& gBoss[bos].x + gBoss[bos].hit.back > gBul[bul].x - gBul[bul].blockXL
				&& gBoss[bos].y - gBoss[bos].hit.top < gBul[bul].y + gBul[bul].blockYL
				&& gBoss[bos].y + gBoss[bos].hit.bottom > gBul[bul].y - gBul[bul].blockYL)
				bHit = true;

			if (bHit)
			{
				// Damage boss
				if (gBoss[bos].bits & NPC_SHOOTABLE)
				{
					if (gBoss[bos].cond & 0x10)
						bos_ = 0;
					else
						bos_ = bos;

					int dmg = gBul[bul].damage;
					if (gBoss[bos_].code_char == 30) dmg = 0;
					if (gMC.equip & EQUIP_NIKUMARU_COUNTER && Random(1, 3) == 1 && gBoss[bos_].damage > 0){
						if (gBoss[bos_].code_char == 263 || gBoss[bos_].code_char == 267){
							gBoss[bos_].freeze += gBul[bul].damage*12;
						} else
							gBoss[bos_].freeze += gBul[bul].damage*30;
						if (gBoss[bos_].freeze > 100){
							gBoss[bos_].freeze = 100;
						}
						continue;
					}
					if (GetNPCFlag(874)){
						dmg = round(dmg * 1.25f);
					}
					if (gMC.equip & EQUIP_SUNGLASSES){
						dmg *= 2;
					}
					if (GetNPCFlag(4242)){
						dmg *= float(Random(50, 175)) / 100.0f;
					}

					gBoss[bos_].life -= dmg;
					if (gBul[bul].code_bullet > 6 && gBul[bul].code_bullet < 10 && !CountBulletNum(16) && GetNPCFlag(873)){
						SetBullet(16, gBul[bul].x, gBul[bul].y, 0);
					}

					if (Random(0, 2) == 0 && dmg != 0 && gMC.equip & EQUIP_NANO_QUIVER && (gBul[bul].code_bullet != 47 && gBul[bul].code_bullet != 45))
					{
						int x, y;
						do
						{
							x = Random(0, gMap.width);
							y = Random(0, gMap.length);
						} while (!(Distance(gBoss[bos_].x / 0x2000, gBoss[bos_].y / 0x2000, x, y) < 6 && 
									Distance(gBoss[bos_].x / 0x2000, gBoss[bos_].y / 0x2000, x, y) > 2));
						BULLET *bul = SetBullet(47, x * 0x2000, y * 0x2000);
						if (bul != NULL){
							bul->tgt_x = gBoss[bos_].x;
							bul->tgt_y = gBoss[bos_].y;
						}
					}

					if (/* GetNPCFlag(851) */true){
						for (int b1 = 0; b1 < BULLET_MAX; b1++){
							if (gBul[b1].code_bullet == 57 && gBul[b1].npc == NULL){
								gBul[b1].npc = &gBoss[bos_];
								break;
							}
						}
					}

					if (gBoss[bos_].life < 1 || ChallengeEnabled("debug"))
					{
						gBoss[bos_].life = bos_;

						if (gMC.cond & 0x80 && gBoss[bos_].bits & NPC_EVENT_WHEN_KILLED)
						{
							StartTextScript(gBoss[bos_].code_event);
						}
						else
						{
							PlaySoundObject(gBoss[bos_].destroy_voice, SOUND_MODE_PLAY);

							switch (gBoss[bos_].size)
							{
								case 1:
									SetDestroyNpChar(gBoss[bos_].x, gBoss[bos_].y, gBoss[bos_].view.back, 4);
									break;
								case 2:
									SetDestroyNpChar(gBoss[bos_].x, gBoss[bos_].y, gBoss[bos_].view.back, 8);
									break;
								case 3:
									SetDestroyNpChar(gBoss[bos_].x, gBoss[bos_].y, gBoss[bos_].view.back, 16);
									break;
							}

							gBoss[bos_].cond = 0;
						}
					}
					else
					{
						if (gBul[bul].iframes > 0)
							boss_static_iframes[bos][gBul[bul].code_bullet] = gBul[bul].iframes;
						if (gBul[bul].local_iframes > 0)
							boss_static_iframes[bos][bul] = gBul[bul].local_iframes;
						if (gBoss[bos].shock < 14)
						{
							SetCaret(gBul[bul].x, gBul[bul].y, 11, 0);
							SetCaret(gBul[bul].x, gBul[bul].y, 11, 0);
							SetCaret(gBul[bul].x, gBul[bul].y, 11, 0);
							PlaySoundObject(gBoss[bos_].hit_voice, SOUND_MODE_PLAY);
						}

						gBoss[bos].shock = 12;
						gBoss[bos_].shock = 12;
						// if (isSword)
						// 	{
						// 		gBoss[bos_].shock = 12 + gBul[bul].life_count;
						// 		gBoss[bos].shock = 12 + gBul[bul].life_count;
						// 		gBoss[bos_].sword_iframes = gBul[bul].life_count;
						// 		gBoss[bos].sword_iframes = gBul[bul].life_count;
						// 	}
						gBoss[bos_].damage_view -= dmg;
						starTarget = &gBoss[bos_];
					}

					--gBul[bul].life;

					if (gBul[bul].life < 1)
						gBul[bul].cond = 0;
				}
				else if (gBul[bul].code_bullet == 13
					|| gBul[bul].code_bullet == 14
					|| gBul[bul].code_bullet == 15
					|| gBul[bul].code_bullet == 28
					|| gBul[bul].code_bullet == 29
					|| gBul[bul].code_bullet == 30)
				{
					--gBul[bul].life;
				}
				else
				{
					if (!(gBul[bul].bbits & 0x10))
					{
						SetCaret(gBul[bul].x, gBul[bul].y, 2, 2);
						PlaySoundObject(31, SOUND_MODE_PLAY);
						gBul[bul].cond = 0;
						continue;
					}
				}
			}
		}
	}
}

void ActBossChar_0(void)
{
	
}

BOSSFUNCTION gpBossFuncTbl[10] =
{
	ActBossChar_0,
	ActBossChar_Omega,
	ActBossChar_Frog,
	ActBossChar_MonstX,
	ActBossChar_Core,
	ActBossChar_Ironhead,
	ActBossChar_Twin,
	ActBossChar_Undead,
	ActBossChar_Press,
	ActBossChar_Ballos
};

void ActBossChar(void)
{
	int code_char;
	int bos;

	if (!(gBoss[0].cond & 0x80))
		return;

	code_char = gBoss[0].code_char;

	if (!TimeStopActive(code_char + 1000)){
		gpBossFuncTbl[code_char]();

		for (bos = 0; bos < BOSS_MAX; ++bos)
			if (gBoss[bos].shock)
				--gBoss[bos].shock;

		for (auto& npc: boss_static_iframes){
			for (auto& iframes: npc.second){
				if (iframes.second > 0){
					iframes.second--;
				}
			}
		}
		for (auto& npc: boss_local_iframes){
			for (auto& iframes: npc.second){
				if (iframes.second > 0){
					iframes.second--;
				}
			}
		}
	}
}

void HitBossMap(void)
{
	int x, y;
	unsigned char atrb[16];
	int judg;
	int offx[16];
	int offy[16];
	int b, j;

	offx[0] = 0;
	offx[1] = 1;
	offx[2] = 0;
	offx[3] = 1;
	offx[4] = 2;
	offx[5] = 2;
	offx[6] = 2;
	offx[7] = 0;
	offx[8] = 1;
	offx[9] = -1;
	offx[10] = -1;
	offx[11] = -1;
	offx[12] = -1;
	offx[13] = 0;
	offx[14] = 1;
	offx[15] = 2;

	offy[0] = 0;
	offy[1] = 0;
	offy[2] = 1;
	offy[3] = 1;
	offy[4] = 0;
	offy[5] = 1;
	offy[6] = 2;
	offy[7] = 2;
	offy[8] = 2;
	offy[9] = -1;
	offy[10] = 0;
	offy[11] = 1;
	offy[12] = 2;
	offy[13] = -1;
	offy[14] = -1;
	offy[15] = -1;

	for (b = 0; b < BOSS_MAX; ++b)
	{
		if (!(gBoss[b].cond & 0x80))
			continue;

		if (gBoss[b].bits & NPC_IGNORE_SOLIDITY)
			continue;

		if (gBoss[b].size >= 3)
		{
			judg = 16;
			x = gBoss[b].x / 16_pix;
			y = gBoss[b].y / 16_pix;
		}
		else
		{
			judg = 4;
			x = gBoss[b].x / 16_pix;
			y = gBoss[b].y / 16_pix;
		}

		gBoss[b].flag = 0;
		for (j = 0; j < judg; ++j)
		{
			atrb[j] = GetAttribute(x + offx[j], y + offy[j]);

			switch (atrb[j])
			{
				case 0x44:
					if (gBoss[b].bits & NPC_IGNORE_TILE_44)
						break;
					// Fallthrough
				case 0x05:
				case 0x41:
				case 0x43:
					JadgeHitNpCharBlock(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x50:
					JudgeHitNpCharTriangleA(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x51:
					JudgeHitNpCharTriangleB(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x52:
					JudgeHitNpCharTriangleC(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x53:
					JudgeHitNpCharTriangleD(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x54:
					JudgeHitNpCharTriangleE(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x55:
					JudgeHitNpCharTriangleF(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x56:
					JudgeHitNpCharTriangleG(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x57:
					JudgeHitNpCharTriangleH(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x02:
				case 0x60:
				case 0x61:
				case 0x64:
					JadgeHitNpCharBlock(&gBoss[b], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x70:
					JudgeHitNpCharTriangleA(&gBoss[b], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x71:
					JudgeHitNpCharTriangleB(&gBoss[b], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x72:
					JudgeHitNpCharTriangleC(&gBoss[b], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x73:
					JudgeHitNpCharTriangleD(&gBoss[b], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x74:
					JudgeHitNpCharTriangleE(&gBoss[b], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x75:
					JudgeHitNpCharTriangleF(&gBoss[b], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x76:
					JudgeHitNpCharTriangleG(&gBoss[b], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
					break;
				case 0x77:
					JudgeHitNpCharTriangleH(&gBoss[b], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
					break;
			}
		}
	}
}
