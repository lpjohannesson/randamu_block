#include <algorithm>

#include "Caret.h"
#include "Cooldown.h"
#include "Draw.h"
#include "Flags.h"
#include "Flash.h"
#include "Game.h"
#include "Log.h"
#include "MyChar.h"
#include "NpChar.h"
#include "Sound.h"
#include "Star.h"
#include "Organya.h"

void InitiateCooldown(std::vector<Cooldown>& list, int id){
    InitiateCooldown(list, id, CooldownDuration(id));
}

void InitiateCooldown(std::vector<Cooldown>& list, int id, int duration){
    if (std::find_if(list.begin(), list.end(), [id](Cooldown c){
        return c.id == id;
    }) == list.end()){
        list.push_back({id, duration});
    }
}

void IncrementCooldown(std::vector<Cooldown>& list, int id, int duration){
    for (auto& cooldown: list){
        if (cooldown.id == id){
            cooldown.duration += duration;
        }
    }
}

void OverrideCooldown(std::vector<Cooldown>& list, int id, int duration){
    list.erase(std::remove_if(list.begin(), list.end(), [id](Cooldown c){
        return c.id <= id;
    }), list.end());

    InitiateCooldown(list, id, duration);
}

int CheckForCooldown(std::vector<Cooldown>& list, int id){
    for (auto& cooldown: list){
        if (cooldown.id == id && cooldown.duration >= 0)
            return cooldown.duration;
    }
    return -1;
}

void PutCooldownList(std::vector<Cooldown>& list){
    int idd = 0;
    for (auto& cooldown: list){
        Rect rcCooldownIcon = {16 + cooldown.id * 16, 16, 32 + cooldown.id*16, 32};
        Rect rcCooldownDuration = {0, 16 + 16*(((float)cooldown.duration)/((float)CooldownDuration(cooldown.id))), 16, 32};
        PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(17 + idd*15), PixelToScreenCoord(58), &rcCooldownIcon, SURFACE_ID_ARMS_IMAGE);
        PutBitmap3_NoFlip(&grcGame, PixelToScreenCoord(17 + idd*15), PixelToScreenCoord(58), &rcCooldownDuration, SURFACE_ID_ARMS_IMAGE);
        idd++;
    }
}

bool DoCooldownCountdown(int id){
    switch (id){
        default:
            return true;

        case DASH_TIMER_ID:
            return gMC.dashDuration <= 0;
    }
}

void ActCooldownList(std::vector<Cooldown>& list){
    for (auto& cooldown: list){
        if (cooldown.duration != 0 && DoCooldownCountdown(cooldown.id)){
            cooldown.duration--;
            if (cooldown.duration == 0){
                WriteGameLog("The cooldown #", cooldown.id, " has expired.");
                switch (cooldown.id){
                    case CHAOS_KEY_ID:
                        PlaySoundObject(61, SOUND_MODE_PLAY);
                        for (int i = 0; i < 35; i++)
                            SetCaret(gMC.x, gMC.y, 11, 1);
                        break;

                    case TIME_RING_ID:
                        SetFlash(gMC.x, gMC.y, 1, RGB(154, 209, 232) | (0x80 << 24));
                        SetCaret(gMC.x-0x2500, gMC.y, 2, 2);
                        SetCaret(gMC.x-0x1800, gMC.y-0x1800, 2, 2);
                        SetCaret(gMC.x+0x2500, gMC.y, 2, 2);
                        SetCaret(gMC.x+0x1800, gMC.y-0x1800, 2, 2);
                        SetCaret(gMC.x, gMC.y+0x2500, 2, 2);
                        SetCaret(gMC.x+0x1800, gMC.y+0x1800, 2, 2);
                        SetCaret(gMC.x, gMC.y-0x2500, 2, 2);
                        SetCaret(gMC.x-0x1800, gMC.y+0x1800, 2, 2);
                        PlaySoundObject(116, SOUND_MODE_PLAY);
                        break;

                    case TIME_STOP_ID:
                        InitiateCooldown(gCooldowns, TIME_RING_ID);
                        SetFlash(gMC.x, gMC.y, 1, RGB(154, 209, 232) | (0x80 << 24));
                        PlaySoundObject(64, SOUND_MODE_PLAY);
                        PlayOrganyaMusic();
                        for (int i = 0; i < SE_MAX; i++)
                            ChangeSoundFrequency(i, 2195);
                        break;

                    case NANITES_TIME_ID:
                        InitiateCooldown(gCooldowns, NANITES_CD_ID);
                        SetDestroyNpChar(gMC.x, gMC.y, 8_pix, 16);
                        PlaySoundObject(70, SOUND_MODE_PLAY);
                        PlaySoundObject(71, SOUND_MODE_PLAY);
                        break;

                    case SHEEP_CURSE_ID:
                        SetDestroyNpChar(gMC.x, gMC.y, 0x2000 * 2, 26);
                        PlaySoundObject(70, SOUND_MODE_PLAY);
                        break;

                    case SARA_DAMAGE_ID:
                    case NANITES_CD_ID:
                        SetCaret(gMC.x-0x2500, gMC.y, 2, 2);
                        SetCaret(gMC.x-0x1800, gMC.y-0x1800, 2, 2);
                        SetCaret(gMC.x+0x2500, gMC.y, 2, 2);
                        SetCaret(gMC.x+0x1800, gMC.y-0x1800, 2, 2);
                        SetCaret(gMC.x, gMC.y+0x2500, 2, 2);
                        SetCaret(gMC.x+0x1800, gMC.y+0x1800, 2, 2);
                        SetCaret(gMC.x, gMC.y-0x2500, 2, 2);
                        SetCaret(gMC.x-0x1800, gMC.y+0x1800, 2, 2);
                        PlaySoundObject(103, SOUND_MODE_PLAY);
                        break;

                    case DUCK_TIMER_ID:
                        {
                            short& star_storage_link = GetNPCFlag(4280) ? gMC.life : gMC.star;
                            int d = 0;
                            if (GetNPCFlag(4280))
                                d = 1;
                            if ((star_storage_link)-d < 3){
                                (star_storage_link) += 1;
                                SetCaret(star[(star_storage_link)-d-1].x, star[(star_storage_link)-d-1].y, 3, 0);
                                if ((star_storage_link)-d < 3)
                                    IncrementCooldown(gCooldowns, DUCK_TIMER_ID, 300);
                            }
                        }
                        break;

                    case GRAVITY_FLIP_ID:   
                        SetCaret(gMC.x, gMC.y, 2, 2);
                        SetCaret(gMC.x-0x1000, gMC.y, 2, 2);
                        SetCaret(gMC.x+0x1000, gMC.y, 2, 2);
                        SetCaret(gMC.x, gMC.y+0x1000, 2, 2);
                        SetCaret(gMC.x, gMC.y-0x1000, 2, 2);
                        break;

                    case DASH_TIMER_ID:
                        if (IsDashEquipped()){
                            SetCaret(gMC.x, gMC.y, 2, 2);
                            SetCaret(gMC.x-0x1000, gMC.y, 2, 2);
                            SetCaret(gMC.x+0x1000, gMC.y, 2, 2);
                            SetCaret(gMC.x, gMC.y+0x1000, 2, 2);
                            SetCaret(gMC.x, gMC.y-0x1000, 2, 2);
                        }
                        break;
                        
                }
            } else {
                switch (cooldown.id){
                    case NANITES_TIME_ID:
                        if (cooldown.duration % 4 == 0){
                            PlaySoundObject(65);
                            SetCaret(gMC.x-0x2500, gMC.y, 2, 2);
                            SetCaret(gMC.x-0x1800, gMC.y-0x1800, 2, 2);
                            SetCaret(gMC.x+0x2500, gMC.y, 2, 2);
                            SetCaret(gMC.x+0x1800, gMC.y-0x1800, 2, 2);
                            SetCaret(gMC.x, gMC.y+0x2500, 2, 2);
                            SetCaret(gMC.x+0x1800, gMC.y+0x1800, 2, 2);
                            SetCaret(gMC.x, gMC.y-0x2500, 2, 2);
                            SetCaret(gMC.x-0x1800, gMC.y+0x1800, 2, 2);
                        }
                        break;
                }
            }
        }
    }

    list.erase(std::remove_if(list.begin(), list.end(), [](Cooldown c){
        return c.duration <= 0;
    }), list.end());
}

int CooldownDuration(int id){
    switch (id){
        case CHAOS_KEY_ID:
            return 1200;
        case TIME_RING_ID:
            return 1800;
        case TIME_STOP_ID:
            return 350;
        case SHEEP_CURSE_ID:
            return 225;
        case SARA_DAMAGE_ID:
            return 480;
        case DUCK_TIMER_ID:
            return 500;
        case DASH_TIMER_ID:
            if ((gMC.equip & EQUIP_DASH_AMULET || GetNPCFlag(876)) && 
                CheckForCooldown(gCooldowns, id) > DashCooldown())
                return DashCooldown() + 120;
            return std::max(DashCooldown(), 1);
        case NANITES_TIME_ID:
            return 240;
        case NANITES_CD_ID:
            return 3600;
        case GRAVITY_FLIP_ID:
            return 90;
    }
    return 0;
}