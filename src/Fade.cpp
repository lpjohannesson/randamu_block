#include "Fade.h"

#include <cstring>

#include "Draw.h"

FADE gFade;

static unsigned long mask_color;

void InitFade(void)
{
	memset(&gFade, 0, sizeof(FADE));
	mask_color = GetCortBoxColor(RGB(0, 0, 0x20));
}

void SetFadeMask(void)
{
	gFade.bMask = true;
}

void ClearFade(void)
{
	gFade.bMask = false;
	gFade.mode = 0;
}

void StartFadeOut(signed char dir)
{
	int x, y;

	gFade.mode = 2;
	gFade.count = 0;
	gFade.dir = dir;
	gFade.bMask = false;

	for (y = 0; y < FADE_HEIGHT; ++y)
	{
		for (x = 0; x < FADE_WIDTH; ++x)
		{
			gFade.ani_no[y][x] = 0;
			gFade.flag[y][x] = false;
		}
	}
}

void StartFadeIn(signed char dir)
{
	int x, y;

	gFade.mode = 1;
	gFade.count = 0;
	gFade.dir = dir;
	gFade.bMask = true;

	for (y = 0; y < FADE_HEIGHT; ++y)
	{
		for (x = 0; x < FADE_WIDTH; ++x)
		{
			gFade.ani_no[y][x] = 15;
			gFade.flag[y][x] = false;
		}
	}

	x = x;	// This probably doesn't match the original source code, but it produces the same assembly
}

void ProcFade(void)
{
	int x, y;

	switch (gFade.mode)
	{
		case 2:
			switch (gFade.dir)
			{
				case 0:
					for (y = 0; y < FADE_HEIGHT; ++y)
						for (x = 0; x < FADE_WIDTH; ++x)
							if ((FADE_WIDTH - 1) - gFade.count == x)
								gFade.flag[y][x] = true;

					break;

				case 2:
					for (y = 0; y < FADE_HEIGHT; ++y)
						for (x = 0; x < FADE_WIDTH; ++x)
							if (gFade.count == x)
								gFade.flag[y][x] = true;

					break;

				case 1:
					for (y = 0; y < FADE_HEIGHT; ++y)
						for (x = 0; x < FADE_WIDTH; ++x)
							if ((FADE_HEIGHT - 1) - gFade.count == y)
								gFade.flag[y][x] = true;

					break;

				case 3:
					for (y = 0; y < FADE_HEIGHT; ++y)
						for (x = 0; x < FADE_WIDTH; ++x)
							if (gFade.count == y)
								gFade.flag[y][x] = true;

					break;

				case 4:
					for (y = 0; y < (FADE_HEIGHT / 2); ++y)
						for (x = 0; x < (FADE_WIDTH / 2); ++x)
							if (gFade.count == x + y)
								gFade.flag[y][x] = true;

					for (y = 0; y < (FADE_HEIGHT / 2); ++y)
						for (x = (FADE_WIDTH / 2); x < FADE_WIDTH; ++x)
							if (gFade.count == y + ((FADE_WIDTH - 1) - x))
								gFade.flag[y][x] = true;

					for (y = (FADE_HEIGHT / 2); y < FADE_HEIGHT; ++y)
						for (x = 0; x < (FADE_WIDTH / 2); ++x)
							if (gFade.count == x + ((FADE_HEIGHT - 1) - y))
								gFade.flag[y][x] = true;

					for (y = (FADE_HEIGHT / 2); y < FADE_HEIGHT; ++y)
						for (x = (FADE_WIDTH / 2); x < FADE_WIDTH; ++x)
							if (gFade.count == ((FADE_WIDTH - 1) - x) + ((FADE_HEIGHT - 1) - y))
								gFade.flag[y][x] = true;

					break;
			}

			for (y = 0; y < FADE_HEIGHT; ++y)
				for (x = 0; x < FADE_WIDTH; ++x)
					if (gFade.ani_no[y][x] < 15 && gFade.flag[y][x])
						++gFade.ani_no[y][x];

			if (++gFade.count > ((FADE_WIDTH > FADE_HEIGHT) ? FADE_WIDTH : FADE_HEIGHT) + 16)
			{
				gFade.bMask = true;
				gFade.mode = 0;
			}

			break;

		case 1:
			gFade.bMask = false;

			switch (gFade.dir)
			{
				case 0:
					for (y = 0; y < FADE_HEIGHT; ++y)
						for (x = 0; x < FADE_WIDTH; ++x)
							if ((FADE_WIDTH - 1) - gFade.count == x)
								gFade.flag[y][x] = true;

					break;

				case 2:
					for (y = 0; y < FADE_HEIGHT; ++y)
						for (x = 0; x < FADE_WIDTH; ++x)
							if (gFade.count == x)
								gFade.flag[y][x] = true;

					break;

				case 1:
					for (y = 0; y < FADE_HEIGHT; ++y)
						for (x = 0; x < FADE_WIDTH; ++x)
							if ((FADE_HEIGHT - 1) - gFade.count == y)
								gFade.flag[y][x] = true;

					break;

				case 3:
					for (y = 0; y < FADE_HEIGHT; ++y)
						for (x = 0; x < FADE_WIDTH; ++x)
							if (gFade.count == y)
								gFade.flag[y][x] = true;

					break;

				case 4:
					for (y = 0; y < (FADE_HEIGHT / 2); ++y)
						for (x = 0; x < (FADE_WIDTH / 2); ++x)
							if ((FADE_WIDTH - 1) - gFade.count == x + y)
								gFade.flag[y][x] = true;

					for (y = 0; y < (FADE_HEIGHT / 2); ++y)
						for (x = (FADE_WIDTH / 2); x < FADE_WIDTH; ++x)
							if ((FADE_WIDTH - 1) - gFade.count == y + ((FADE_WIDTH - 1) - x))
								gFade.flag[y][x] = true;

					for (y = (FADE_HEIGHT / 2); y < FADE_HEIGHT; ++y)
						for (x = 0; x < (FADE_WIDTH / 2); ++x)
							if ((FADE_WIDTH - 1) - gFade.count == x + ((FADE_HEIGHT - 1) - y))
								gFade.flag[y][x] = true;

					for (y = (FADE_HEIGHT / 2); y < FADE_HEIGHT; ++y)
						for (x = (FADE_WIDTH / 2); x < FADE_WIDTH; ++x)
							if ((FADE_WIDTH - 1) - gFade.count == ((FADE_WIDTH - 1) - x) + ((FADE_HEIGHT - 1) - y))
								gFade.flag[y][x] = true;

					break;
			}

			for (y = 0; y < FADE_HEIGHT; ++y)
				for (x = 0; x < FADE_WIDTH; ++x)
					if (gFade.ani_no[y][x] > 0 && gFade.flag[y][x])
						--gFade.ani_no[y][x];

			if (++gFade.count > ((FADE_WIDTH > FADE_HEIGHT) ? FADE_WIDTH : FADE_HEIGHT) + 16)
				gFade.mode = 0;

			break;
	}
}

void PutFade(void)
{
	int x, y;

	Rect rect;
	rect.top = 0;
	rect.bottom = 16;

	if (gFade.bMask)
	{
		CortBox(&grcGame, mask_color);
		return;
	}

	if (gFade.mode == 0)
		return;

	for (y = 0; y < FADE_HEIGHT; ++y)
	{
		for (x = 0; x < FADE_WIDTH; ++x)
		{
			rect.left = gFade.ani_no[y][x] * 16;
			rect.right = rect.left + 16;
			PutBitmap3(&grcGame, PixelToScreenCoord(x * 16), PixelToScreenCoord(y * 16), &rect, SURFACE_ID_FADE);
		}
	}
}

bool GetFadeActive(void)
{
	if (gFade.mode == 0)
		return false;
	else
		return true;
}
