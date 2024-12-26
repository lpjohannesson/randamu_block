#include "Floodtide.h"

#include "..\Bullet.h"
#include "..\Caret.h"
#include "..\Cooldown.h"
#include "..\Flags.h"
#include "..\MyChar.h"
#include "..\Sound.h"
#include "..\Triangle.h"

void ActBullet_Floodtide(BULLET *bul, int level)
{
	gMC.direct = bul->direct;

	if (++bul->count1 > bul->life_count)
	{
		bul->cond = 0;
		return;
	}

	bool isBoostActive = CheckForCooldown(gCooldowns, FLOODTIDE_BOOST_ID) != -1;

	if (bul->act_no == 0){
		if (bul->direct == 2){
			bul->angle = -120;
			bul->centerX = 0;
			bul->centerY = 1;
		}
		else{
			bul->centerX = 1;
			bul->centerY = 1;
			bul->angle = 120;
		}
		if (gMC.flag & 0x100 || isBoostActive){
			bul->life_count /= 2;
			bul->count2 = 1;
		}
		bul->iframes = bul->life_count;
		bul->act_no = 1;
		if (bul->ani_no != 0){
			bul->damage *= 4;
		}
	} else {
		int spinValue = 6 + level*2;
		int swingValue = 13;
		if (bul->count2 == 1){
			spinValue *= 2;
			swingValue /= 2;
		}
		if (bul->direct == 2)
			bul->angle += spinValue;
		else
			bul->angle -= spinValue;

		int whirlLimit = 1;
		if (level == 4) whirlLimit = 2;

		if (bul->ani_no != 0){
			SetCaret(bul->x + Random(-4_pix, 4_pix), bul->y + Random(-4_pix, 4_pix), 1, 0);
		}

		if (isBoostActive && bul->count1 == swingValue){
			BULLET *quiverProj = SetBullet(47, bul->x, bul->y);
			if (quiverProj != NULL){
				quiverProj->damage = gBulletTable[bul->code_bullet].table.damage / 2;
				if (gMC.up){
					quiverProj->tgt_x = bul->x;
					quiverProj->tgt_y = bul->y - 16_pix;
				} else if (gMC.down){
					quiverProj->tgt_x = bul->x;
					quiverProj->tgt_y = bul->y + 16_pix;
				} else {
					if (gMC.direct == 0)
						quiverProj->tgt_x = bul->x - 16_pix;
					else
						quiverProj->tgt_x = bul->x + 16_pix;
					quiverProj->tgt_y = bul->y;
				}
			}
		}

		if (level >= 3 && bul->count1 == swingValue && CountBulletNum(Bullets::FLOODTIDE_WHIRLPOOL) < whirlLimit){
			int lol = 1;
			for (int i = 0; i < lol; i++)
			{
				BULLET *bully = SetBullet(Bullets::FLOODTIDE_WHIRLPOOL, bul->x, bul->y);
				if (bully != NULL)
				{
					PlaySoundObject(102, SOUND_MODE_PLAY);
					bully->direct = 0;
					if (bul->direct == 0)
						bully->xm = -0x200 * 5;
					else
						bully->xm = 0x200 * 5;
					if (gMC.up){
						bully->ym = -0x200 * 5;
						bully->xm = 0;
					}
					else if (gMC.down){
						bully->ym = 0x200 * 5;
						bully->xm = 0;
					} else 
						bully->ym = 0;

					if (bul->count2 == 1){
						bully->xm *= 1.4;
						bully->ym *= 1.4;
						bully->count2 = 1;
					}
				}
			}
		}

		if ((level == 2 || level == 4) && bul->count1 == swingValue){
			if (CheckForCooldown(gCooldowns, FLOODTIDE_ATTACK_ID) == -1){
				int lol = Random(30, 40);
				if (level == 4) lol+=20;
				if (bul->count2 == 1) lol+=8;
				InitiateCooldown(gCooldowns, FLOODTIDE_ATTACK_ID);
				for (int i = 0; i < lol; i++)
				{
					BULLET *bully = SetBullet(Bullets::WATER_DROPLET, bul->x, bul->y);
					if (bully != NULL)
					{
						PlaySoundObject(21, SOUND_MODE_PLAY);
						bully->direct = 0;
						if (bul->direct == 0)
							bully->xm = -Random(0, 2200);
						else
							bully->xm = Random(0, 2200);
						if (gMC.up){
							bully->ym = Random(-1600, -700);
							bully->xm /= 5;
						}
						else if (gMC.down){
							bully->ym = -Random(-1600, -700);
							bully->xm /= 5;
						}
						else
							bully->ym = Random(-900, 120);
						if (bul->count2 == 1){
							bully->xm *= 1.4;
							bully->ym *= 1.4;
						}
					}
				}
			} else {
				IncrementCooldown(gCooldowns, FLOODTIDE_ATTACK_ID, -1);
				std::remove_if(gCooldowns.begin(), gCooldowns.end(), [](Cooldown c){
					return c.id == FLOODTIDE_ATTACK_ID && c.duration <= 0;
				});
			}
		}

		if (bul->life != 100){
			if (bul->ani_no != 0){
				if (CheckForCooldown(gCooldowns, FLOODTIDE_BOOST_ID) == -1)
					InitiateCooldown(gCooldowns, FLOODTIDE_BOOST_ID);
				bul->damage = std::max(bul->damage - gBulletTable[bul->code_bullet].table.damage, static_cast<int>(gBulletTable[bul->code_bullet].table.damage));
				PlaySoundObject(21);
			}

			if (level == 1 || level == 4)
				bul->life = 100;
		}
	}

	bul->x = gMC.x + gMC.xm;
	if (gMC.direct == 0){
		bul->x -= 0x200 * 12 * (float(bul->count1) / float(bul->life_count));
	} else {
		bul->x += 3_pix + 0x200 * 12 * (float(bul->count1) / float(bul->life_count));
	}
	bul->y = gMC.y + gMC.ym - 0x200 * 7 + 0x200 * 5 * (float(bul->count1) / float(bul->life_count));

	Rect rcRight[4] = {
		{224, 144, 236, 156},
		{240, 144, 254, 158},
		{256, 144, 271, 159},
		{271, 144, 286, 160},
	};

	Rect rcLeft[4] = {
		{224, 156, 236, 168},
		{240, 158, 254, 172},
		{256, 159, 271, 173},
		{271, 160, 286, 176},
	};

	if (bul->direct == 0)
		bul->rect = rcLeft[level-1];
	else
		bul->rect = rcRight[level-1];
}

void ActBullet_Whirlpool(BULLET *bul)
{
	bul->rect = {288, 128, 298, 138};
	bul->life_count = 500;
	bul->iframes = 10;
	if (GetNPCFlag(Flags::BATTERY_PACK)){
			if (GetNPCFlag(Flags::AMPHIBIAN_MEDAL)) bul->life_count *= 1.33;
			else bul->life_count *= 1.25;
		}

	if (++bul->ani_wait > 0)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->flag & 0xF){
		int deg = GetArktan(bul->x - (bul->x + Random(-0x200 * 4, 0x200 * 4)), 
			bul->y - (bul->y + Random(-0x200 * 4, 0x200 * 4)));
		deg += (unsigned char)Random(-18, 18);
		bul->ym = GetSin(deg) * 6;
		bul->xm = GetCos(deg) * 6;
		if (bul->count2 == 1){
			bul->xm += GetCos(deg) * 2;
			bul->ym += GetSin(deg) * 2;
		}
	}

	if (++bul->act_wait > 6 - (bul->count2 == 1 ? 2 : 0) || bul->life != 100){
		bul->act_wait = 0;
		int lol = Random(2, 4);
		if (bul->life != 100)
			lol = Random(0, 3);
		bul->life = 100;
		for (int i = 0; i < lol; i++)
		{
			BULLET *bully = SetBullet(Bullets::WATER_DROPLET, bul->x, bul->y);
			if (bully != NULL)
			{
				bully->direct = 0;
				bully->xm = Random(-970, 970);
				bully->ym = Random(-970, 970);
				if (bul->count2 == 1)
				{
					bully->xm *= 1.4;
					bully->ym *= 1.4;
				}
			}
		}
	}

	if (bul->ani_no > 2)
		bul->ani_no = 0;

	SetNpChar(129, bul->x, bul->y, 0, -0x200, bul->ani_no + 3, NULL, 0x100);

	bul->y += bul->ym;
	bul->x += bul->xm;

	if (++bul->count1 > bul->life_count){
		bul->cond = 0;
		int lol = Random(40, 80);
		for (int i = 0; i < lol; i++)
		{
			BULLET *bully = SetBullet(Bullets::WATER_DROPLET, bul->x, bul->y);
			if (bully != NULL)
			{
				bully->direct = 0;
				bully->xm = Random(-1200, 1200);
				bully->ym = Random(-1000, 200);
				if (bul->count2 == 1){
					bully->xm *= 1.4;
					bully->ym *= 1.4;
				}
			}
		}
	}
}

void RegisterFloodtide(){
    gBulletTable.insert({Bullets::FLOODTIDE_LVL_1, {
        {6, 100, 24, 52, 12, 10, 4, 2, {8, 8, 8, 8}, 1, 1},
        [](BULLET* bul){ ActBullet_Floodtide(bul, 1); }
    }});
    gBulletTable.insert({Bullets::FLOODTIDE_LVL_2, {
        {9, 100, 19, 52, 13, 12, 4, 2, {10, 10, 10, 10}, 1, 1},
        [](BULLET* bul){ ActBullet_Floodtide(bul, 2); }
    }});
    gBulletTable.insert({Bullets::FLOODTIDE_LVL_3, {
        {12, 100, 17, 52, 14, 13, 4, 2, {12, 12, 12, 12}, 1, 1},
        [](BULLET* bul){ ActBullet_Floodtide(bul, 3); }
    }});
    gBulletTable.insert({Bullets::FLOODTIDE_LVL_4, {
        {15, 100, 14, 52, 12, 15, 4, 2, {12, 12, 12, 12}, 1, 1},
        [](BULLET* bul){ ActBullet_Floodtide(bul, 4); }
    }});

    gBulletTable.insert({Bullets::FLOODTIDE_WHIRLPOOL, {
        {3, 100, 200, 8, 8, 8, 5, 5, {5, 5, 5, 5}, 0.25, 0.25},
        ActBullet_Whirlpool
    }});
}