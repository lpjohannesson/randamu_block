#include "Bubbler.h"

#include "../Bullet.h"
#include "../Caret.h"
#include "../Flags.h"
#include "../KeyControl.h"
#include "../MyChar.h"
#include "../Sound.h"
#include "../Triangle.h"

void ActBullet_Bubblin1(BULLET *bul, int variant = 1)
{
	if (bul->flag & 0x2FF)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 2, 0);
		return;
	}

	switch (bul->act_no)
	{
		case 0:
			bul->npc = NULL;
			bul->act_no = 1;

			switch (bul->direct)
			{
				case 0:
					bul->xm = -0x500;
					break;
				case 2:
					bul->xm = 0x500;
					break;
				case 1:
					bul->ym = -0x500;
					break;
				case 3:
					bul->ym = 0x500;
					break;
			}

			break;
	}
	
	if (bul->act_no == 1){
		switch (bul->direct)
		{
			case 0:
				bul->xm += 0x2A;
				if (bul->xm >= 0) {
					bul->xm = 0;
					bul->act_no = 2;
				}
				break;
			case 2:
				bul->xm -= 0x2A;
				if (bul->xm <= 0) {
					bul->xm = 0;
					bul->act_no = 2;
				}
				break;
			case 1:
				bul->ym += 0x2A;
				if (bul->ym >= 0) {
					bul->ym = 0;
					bul->act_no = 2;
				}
				break;
			case 3:
				bul->ym -= 0x2A;
				if (bul->ym <= 0) {
					bul->ym = 0;
					bul->act_no = 2;
				}
				break;
			case 4:
				bul->xm *= 0.98;
				bul->ym *= 0.98;
				if (std::abs(bul->ym) <= 10 || std::abs(bul->xm) <= 10) {
					bul->ym = 0;
					bul->xm = 0;
					bul->act_no = 2;
				}
				break;
		}
	} else if (bul->act_no == 2) {
		if (bul->npc == NULL || !(bul->npc->cond & 0x80)){
			bul->npc = GetClosest(bul->x, bul->y, (variant == 2 ? 56_pix : 48_pix));
		} else {
			SetCaret(bul->x, bul->y, 1, 0);
			bul->tgt_x = bul->npc->x;
			bul->tgt_y = bul->npc->y;

			int deg = GetArktan(bul->x - bul->tgt_x, bul->y - bul->tgt_y);
			deg += Random(-32, 32);
			bul->ym = 4 * GetSin(deg);
			bul->xm = 4 * GetCos(deg);
		}
	}

	bul->x += bul->xm;
	bul->y += bul->ym;

	if (++bul->act_wait > (variant == 2 ? 250 : 100))
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 15, 0);
	}

	Rect rect[4] = {
		{192, 0, 200, 8},
		{200, 0, 208, 8},
		{208, 0, 216, 8},
		{216, 0, 224, 8},
	};

	if (++bul->ani_wait > 3)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 3;

	bul->rect = rect[bul->ani_no];
}

void ActBullet_Bubblin2(BULLET *bul)
{
	bool bDelete = false;

	if (bul->direct == 0 && bul->flag & 1)
		bDelete = true;
	if (bul->direct == 2 && bul->flag & 4)
		bDelete = true;
	if (bul->direct == 1 && bul->flag & 2)
		bDelete = true;
	if (bul->direct == 3 && bul->flag & 8)
		bDelete = true;

	if (bDelete)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 2, 0);
		return;
	}

	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;
			int move = 0x900;
			int secondMove = 0x180;
			if (CheckFlagTrait(FlagTraits::INACCURATE_VELOCITY_MODIFIER)){
				move *= std::max(1.0f, GetFlagTraitData<float>(FlagTraits::INACCURATE_VELOCITY_MODIFIER, "bubbler_velocity", 1.0f));
				secondMove *= std::max(1.0f, GetFlagTraitData<float>(FlagTraits::INACCURATE_VELOCITY_MODIFIER, "bubbler_inaccuracy", 1.0f));
			}

			switch (bul->direct)
			{
				case 0:
					bul->xm = -move;
					bul->ym = Random(-secondMove, secondMove);
					break;
				case 2:
					bul->xm = move;
					bul->ym = Random(-secondMove, secondMove);
					break;
				case 1:
					bul->ym = -move;
					bul->xm = Random(-secondMove, secondMove);
					break;
				case 3:
					bul->ym = move;
					bul->xm = Random(-secondMove, secondMove);
					break;
			}

			break;
	}

	switch (bul->direct)
	{
		case 0:
			bul->xm += 0x10;
			break;
		case 2:
			bul->xm -= 0x10;
			break;
		case 1:
			bul->ym += 0x10;
			break;
		case 3:
			bul->ym -= 0x10;
			break;
	}

	bul->x += bul->xm;
	bul->y += bul->ym;

	if (++bul->act_wait > 50)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 15, 0);
	}

	Rect rect[4] = {
		{192, 8, 200, 16},
		{200, 8, 208, 16},
		{208, 8, 216, 16},
		{216, 8, 224, 16},
	};

	if (++bul->ani_wait > 3)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 3;

	bul->rect = rect[bul->ani_no];
}

void ActBullet_Bubblin3(BULLET *bul)
{
	bul->iframes = 2'147'483'647;
	if (!(gKey & gKeyShot))
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 2, 0);
		PlaySoundObject(100, SOUND_MODE_PLAY);

		if (gMC.up)
			SetBullet(Bullets::BUBBLER_SPINE, bul->x, bul->y, 1);
		else if (gMC.down)
			SetBullet(Bullets::BUBBLER_SPINE, bul->x, bul->y, 3);
		else
			SetBullet(Bullets::BUBBLER_SPINE, bul->x, bul->y, gMC.direct);

		return;
	}

	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;

			switch (bul->direct)
			{
				case 0:
					bul->xm = Random(-0x400, -0x200);
					bul->ym = (Random(-4, 4) * 0x200) / 2;
					break;
				case 2:
					bul->xm = Random(0x200, 0x400);
					bul->ym = (Random(-4, 4) * 0x200) / 2;
					break;
				case 1:
					bul->ym = Random(-0x400, -0x200);
					bul->xm = (Random(-4, 4) * 0x200) / 2;
					break;
				case 3:
					bul->ym = Random(0x80, 0x100);
					bul->xm = (Random(-4, 4) * 0x200) / 2;
					break;
			}

			break;
	}

	if (bul->x < gMC.x)
		bul->xm += 0x20;
	if (bul->x > gMC.x)
		bul->xm -= 0x20;

	if (bul->y < gMC.y)
		bul->ym += 0x20;
	if (bul->y > gMC.y)
		bul->ym -= 0x20;

	if (bul->xm < 0 && bul->flag & 1)
		bul->xm = 0x400;
	if (bul->xm > 0 && bul->flag & 4)
		bul->xm = -0x400;

	if (bul->ym < 0 && bul->flag & 2)
		bul->ym = 0x400;
	if (bul->ym > 0 && bul->flag & 8)
		bul->ym = -0x400;

	bul->x += bul->xm;
	bul->y += bul->ym;

	Rect rect[4] = {
		{240, 16, 248, 24},
		{248, 16, 256, 24},
		{240, 24, 248, 32},
		{248, 24, 256, 32},
	};

	if (++bul->ani_wait > 3)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 3;

	bul->rect = rect[bul->ani_no];
}

void ActBullet_Bubblin4(BULLET *bul)
{
	bul->iframes = 2'147'483'647;
	if (!(gKey & gKeyShot))
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 2, 0);
		PlaySoundObject(100, SOUND_MODE_PLAY);

		if (gMC.up)
			SetBullet(Bullets::BUBBLER_SPINE, bul->x, bul->y, 1);
		else if (gMC.down)
			SetBullet(Bullets::BUBBLER_SPINE, bul->x, bul->y, 3);
		else
			SetBullet(Bullets::BUBBLER_SPINE, bul->x, bul->y, gMC.direct);

		return;
	}

	switch (bul->act_no)
	{
		case 0:
			bul->act_no = 1;

			switch (bul->direct)
			{
				case 0:
					bul->xm = Random(-0x600, -0x400);
					bul->ym = (Random(-6, 6) * 0x200) / 2;
					break;
				case 2:
					bul->xm = Random(0x400, 0x600);
					bul->ym = (Random(-6, 6) * 0x200) / 2;
					break;
				case 1:
					bul->ym = Random(-0x600, -0x400);
					bul->xm = (Random(-6, 6) * 0x200) / 2;
					break;
				case 3:
					bul->ym = Random(0x100, 0x160);
					bul->xm = (Random(-6, 6) * 0x200) / 2;
					break;
			}

			break;
	}

	if (bul->x < gMC.x)
		bul->xm += 0x10;
	if (bul->x > gMC.x)
		bul->xm -= 0x10;

	if (bul->y < gMC.y)
		bul->ym += 0x10;
	if (bul->y > gMC.y)
		bul->ym -= 0x10;

	if (bul->xm < 0 && bul->flag & 1)
		bul->xm = 0x600;
	if (bul->xm > 0 && bul->flag & 4)
		bul->xm = -0x600;

	if (bul->ym < 0 && bul->flag & 2)
		bul->ym = 0x600;
	if (bul->ym > 0 && bul->flag & 8)
		bul->ym = -0x600;

	bul->x += bul->xm;
	bul->y += bul->ym;

	Rect rect[4] = {
		{240, 16, 248, 24},
		{248, 16, 256, 24},
		{240, 24, 248, 32},
		{248, 24, 256, 32},
	};

	if (++bul->ani_wait > 3)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 3)
		bul->ani_no = 3;

	bul->rect = rect[bul->ani_no];
}

void ActBullet_Spine(BULLET *bul)
{
	if (++bul->count1 > bul->life_count || bul->flag & 8)
	{
		bul->cond = 0;
		SetCaret(bul->x, bul->y, 3, 0);
		return;
	}

	if (bul->act_no == 0)
	{
		bul->act_no = 1;

		switch (bul->direct)
		{
			case 0:
				bul->xm = (-Random(10, 16) * 0x200) / 2;
				break;
			case 1:
				bul->ym = (-Random(10, 16) * 0x200) / 2;
				break;
			case 2:
				bul->xm = (Random(10, 16) * 0x200) / 2;
				break;
			case 3:
				bul->ym = (Random(10, 16) * 0x200) / 2;
				break;
		}
	}
	else
	{
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	if (++bul->ani_wait > 1)
	{
		bul->ani_wait = 0;
		++bul->ani_no;
	}

	if (bul->ani_no > 1)
		bul->ani_no = 0;

	Rect rcLeft[2] = {
		{224, 0, 232, 8},
		{232, 0, 240, 8},
	};

	Rect rcRight[2] = {
		{224, 0, 232, 8},
		{232, 0, 240, 8},
	};

	Rect rcDown[2] = {
		{224, 8, 232, 16},
		{232, 8, 240, 16},
	};

	switch (bul->direct)
	{
		case 0:
			bul->rect = rcLeft[bul->ani_no];
			break;
		case 1:
			bul->rect = rcDown[bul->ani_no];
			break;
		case 2:
			bul->rect = rcRight[bul->ani_no];
			break;
		case 3:
			bul->rect = rcDown[bul->ani_no];
			break;
	}
}

void RegisterBubbler(){
    gBulletTable.insert({Bullets::BUBBLER_LVL_1, {
        {0, 1, 30, 8, 8, 8, 2, 2, {4, 4, 4, 4}, 2, 2},
        [](BULLET* bul){ ActBullet_Bubblin1(bul, 1); }
    }});
    gBulletTable.insert({Bullets::BUBBLER_LVL_2, {
        {2, 1, 15, 8, 2, 2, 2, 2, {4, 4, 4, 4}, 0.3, 0.3},
        ActBullet_Bubblin2
    }});
    gBulletTable.insert({Bullets::BUBBLER_LVL_3, {
        {1, 1, 22, 8, 4, 4, 4, 4, {4, 4, 4, 4}, 0.25, 0.25},
        ActBullet_Bubblin3
    }});
    gBulletTable.insert({Bullets::BUBBLER_LVL_4, {
        {1, 100, 20, 8, 4, 4, 4, 4, {4, 4, 4, 4}, 0.25, 0.25},
        ActBullet_Bubblin4
    }});

    gBulletTable.insert({Bullets::BUBBLER_SPINE, {
        {3, 1, 32, 32, 2, 2, 2, 2, {4, 4, 4, 4}, 0.33, 0.33},
        ActBullet_Spine
    }});

    gBulletTable.insert({Bullets::BUBBLER_WATERGUN, {
        {0, 1, 30, 8, 8, 8, 2, 2, {4, 4, 4, 4}, 2, 2},
        [](BULLET* bul){ ActBullet_Bubblin1(bul, 2); }
    }});
}