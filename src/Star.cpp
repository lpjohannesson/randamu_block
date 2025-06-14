#include "Star.h"

#include <cstring>

#include "CommonDefines.h"

#include "Bullet.h"
#include "Draw.h"
#include "Flags.h"
#include "Game.h"
#include "Map.h"
#include "MyChar.h"
#include "Triangle.h"

STAR star[3];

NPCHAR* starTarget;

void InitStar(void)
{
	// Clear stars
	memset(star, 0, sizeof(star));
	starTarget = NULL;

	// Position
	star[0].x = gMC.x;
	star[0].y = gMC.y;

	star[1].x = gMC.x;
	star[1].y = gMC.y;

	star[2].x = gMC.x;
	star[2].y = gMC.y;

	// Speed
	star[0].xm = 0x200;
	star[0].ym = -0x100;

	star[1].xm = -0x100;
	star[1].ym = 0x200;

	star[2].xm = 0x100;
	star[2].ym = 0x100;
}

void ActStar(void)
{
	int i;
	static int a;

	if (TimeStopActive()) return;

	if (!CheckFlagTrait(FlagTraits::DUCKS)) return;

	++a;
	a %= 3;

	for (i = 0; i < 3; ++i)
	{
		if (i != 0)
		{
			if (star[i - 1].x < star[i].x)
				star[i].xm -= 0x50;
			else
				star[i].xm += 0x50;

			if (star[i - 1].y < star[i].y)
				star[i].ym -= 0x80;
			else
				star[i].ym += 0x80;
		}
		else
		{
			if (gMC.x < star[i].x)
				star[i].xm -= 0x50;
			else
				star[i].xm += 0x50;

			if (gMC.y < star[i].y)
				star[i].ym -= 0x80;
			else
				star[i].ym += 0x80;
		}

		if (star[i].xm > 0x700)
			star[i].xm = 0x700;
		if (star[i].xm < -0x700)
			star[i].xm = -0x700;

		if (star[i].ym > 0x700)
			star[i].ym = 0x700;
		if (star[i].ym < -0x700)
			star[i].ym = -0x700;

		star[i].x += star[i].xm;
		star[i].y += star[i].ym;
		if (starTarget == NULL || !(starTarget->cond & 0x80)){
			NPCHAR* character = GetClosest(star[i].x, star[i].y);
			if (character != NULL && Distance(character->x, character->y, star[i].x, star[i].y) < 0x2000 * 6.0f){
				starTarget = character;
			}
		}
		int starCount = gMC.life-2;
		if (CheckFlagTrait(FlagTraits::BETTER_DUCKS)) starCount = gMC.star-1;

		if (starTarget != NULL){
			if (starTarget->cond & 0x80 && Distance(starTarget->x, starTarget->y, star[i].x, star[i].y) < 0x2000 * 6.0f && i <= starCount){
				if (--star[i].act_no <= 0){
					int deg = GetArktan(star[i].x - starTarget->x, star[i].y - starTarget->y);
					auto duck = SetBullet(Bullets::DUCK, star[i].x, star[i].y);
					duck->xm = GetCos(deg - Random(-16, 16)) * Random(1, 6);
					duck->ym = GetSin(deg - Random(-16, 16)) * Random(1, 6);
					star[i].act_no = Random(5, 15);
					if (CheckFlagTrait(FlagTraits::BETTER_DUCKS)) star[i].act_no *= 2;
				}
			} else {
				starTarget = NULL;
			}
		}
	}
}

void PutStar(int fx, int fy)
{
	Rect rc[3] = {
		{192, 0, 200, 8},
		{192, 8, 200, 16},
		{192, 16, 200, 24},
	};

	int i;

	if (!CheckFlagTrait(FlagTraits::DUCKS)) return;

	if (gMC.cond & 2)
		return;

	int starCount = gMC.life-1;
	if (CheckFlagTrait(FlagTraits::BETTER_DUCKS)) starCount = gMC.star;

	for (i = 0; i < 3; ++i)
		if (i < starCount)
			PutBitmap3(&grcGame, SubpixelToScreenCoord(star[i].x) - SubpixelToScreenCoord(fx) - PixelToScreenCoord(4), SubpixelToScreenCoord(star[i].y) - SubpixelToScreenCoord(fy) - PixelToScreenCoord(4), &rc[i], SURFACE_ID_MY_CHAR);
}
