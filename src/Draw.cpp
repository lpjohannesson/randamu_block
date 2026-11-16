#include "Draw.h"
#define TRUE 1

#include <cstddef>
#include <cstring>
#include <string>

#include "Backends/Misc.h"
#include "Backends/Rendering.h"
#include "Bitmap.h"
#include "Ending.h"
#include "Font.h"
#include "Generic.h"
#include "Main.h"
#include "MapName.h"
#include "MyChar.h"
#include "ResourceCode.h"
#include "TextScr.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef enum SurfaceType
{
	SURFACE_SOURCE_NONE = 1,
	SURFACE_SOURCE_RESOURCE,
	SURFACE_SOURCE_FILE
} SurfaceType;

Rect grcGame = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Rect grcFull = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

static int mag;
static bool fullscreen;	// TODO - Not the original variable name

bool gb60fps;
bool gbSmoothScrolling;
bool gbVsync;
bool gbExtendedInfo;

static unsigned int vsync_fps;

static RenderBackend_Surface *framebuffer;	// TODO - Not the original variable name

static RenderBackend_Surface *surf[SURFACE_ID_MAX];

static CSE2FontObject *font;	// TODO - Not the original variable name

// This doesn't exist in the Linux port, so none of these symbol names are accurate
static struct
{
	char name[50];
	unsigned int width;
	unsigned int height;
	SurfaceType type;
	bool bSystem;	// Basically a 'do not regenerate' flag
} surface_metadata[SURFACE_ID_MAX];

bool Flip_SystemTask(void)
{
	// TODO - Not the original variable names
	static unsigned long timePrev;
	static unsigned long timeNow;

	if (gbVsync && vsync_fps == (gb60fps ? 60 : 50))
	{
		if (!SystemTask())
			return false;
	}
	else
	{
		const unsigned int frameDelays[3] = {17, 16, 17};
		static unsigned int frame;

		const unsigned int delay = gb60fps ? frameDelays[frame % 3] : 20;
		++frame;

		while (true)
		{
			if (!SystemTask())
				return false;

			// Framerate limiter
			timeNow = Backend_GetTicks();

			if (timeNow >= timePrev + delay)
				break;

			Backend_Delay(1);
		}

		if (timeNow >= timePrev + 100)
			timePrev = timeNow;	// If the timer is freakishly out of sync, panic and reset it, instead of spamming frames for who-knows how long
		else
			timePrev += delay;
	}

	RenderBackend_DrawScreen();

	if (RestoreSurfaces())
	{
		RestoreStripper();
		RestoreMapName();
		RestoreTextScript();
	}

	return true;
}

bool StartDirectDraw(const char *title, int lMagnification, bool b60fps, bool bSmoothScrolling, bool bVsync, bool bExtendedInfo)
{
	gb60fps = b60fps;
	gbSmoothScrolling = bSmoothScrolling;
	gbExtendedInfo = bExtendedInfo;

	Backend_DisplayMode display_mode;
	Backend_GetDisplayMode(&display_mode);
	vsync_fps = display_mode.refresh_rate;

	memset(surface_metadata, 0, sizeof(surface_metadata));

	switch (lMagnification)
	{
		default:
			mag = lMagnification;
			fullscreen = false;
			break;

		case 0:	// Fullscreen
			// Round to the nearest internal resolution
			mag = MIN((display_mode.width + (WINDOW_WIDTH / 2)) / WINDOW_WIDTH, (display_mode.height + (WINDOW_HEIGHT / 2)) / WINDOW_HEIGHT);
			fullscreen = true;
			break;
	}

	// Round down to the nearest multiple of gSpriteScale (we can't use 2x sprites at 1x or 3x internal resolution)
	mag -= mag % gSpriteScale;

	// Account for rounding-down to 0
	if (mag == 0)
		mag = gSpriteScale;

	// If v-sync is requested, check if it's available
	if (bVsync)
		gbVsync = vsync_fps == (b60fps ? 60 : 50);

	bool requested_vsync = gbVsync;
	framebuffer = RenderBackend_Init(title, WINDOW_WIDTH * mag, WINDOW_HEIGHT * mag, fullscreen, &requested_vsync);

	gbVsync = requested_vsync;

	if (framebuffer == NULL)
		return false;

	return true;
}

void EndDirectDraw(void)
{
	int i;

	// Release all surfaces
	for (i = 0; i < SURFACE_ID_MAX; ++i)
	{
		if (surf[i] != NULL)
		{
			RenderBackend_FreeSurface(surf[i]);
			surf[i] = NULL;
		}
	}

	framebuffer = NULL;

	RenderBackend_Deinit();

	memset(surface_metadata, 0, sizeof(surface_metadata));
}

void ReleaseSurface(SurfaceID s)
{
	// Release the surface we want to release
	if (surf[s] != NULL)
	{
		RenderBackend_FreeSurface(surf[s]);
		surf[s] = NULL;
	}

	memset(&surface_metadata[s], 0, sizeof(surface_metadata[0]));
}

static bool ScaleAndUploadSurface(const unsigned char *image_buffer, int width, int height, SurfaceID surf_no)
{
	const int magnification_scaled = mag / gSpriteScale;

	unsigned int pitch;
	unsigned char *pixels = RenderBackend_LockSurface(surf[surf_no], &pitch, width * magnification_scaled, height * magnification_scaled);

	if (pixels == NULL)
		return false;

	if (magnification_scaled == 1)
	{
		// Just copy the pixels the way they are
		for (int y = 0; y < height; ++y)
		{
			const unsigned char *src_row = &image_buffer[y * width * 4];
			unsigned char *dst_row = &pixels[y * pitch];

			memcpy(dst_row, src_row, width * 4);
		}
	}
	else
	{
		// Upscale the bitmap to the game's internal resolution
		for (int y = 0; y < height; ++y)
		{
			const unsigned char *src_row = &image_buffer[y * width * 4];
			unsigned char *dst_row = &pixels[y * pitch * magnification_scaled];

			const unsigned char *src_ptr = src_row;
			unsigned char *dst_ptr = dst_row;

			for (int x = 0; x < width; ++x)
			{
				for (int i = 0; i < magnification_scaled; ++i)
				{
					*dst_ptr++ = src_ptr[0];
					*dst_ptr++ = src_ptr[1];
					*dst_ptr++ = src_ptr[2];
					*dst_ptr++ = src_ptr[3];
				}

				src_ptr += 4;
			}

			for (int i = 1; i < magnification_scaled; ++i)
				memcpy(dst_row + i * pitch, dst_row, width * magnification_scaled * 4);
		}
	}

	RenderBackend_UnlockSurface(surf[surf_no], width * magnification_scaled, height * magnification_scaled);

	return true;
}

// TODO - Inaccurate stack frame
bool MakeSurface_File(const char *name, SurfaceID surf_no, bool loadGlobal)
{
	std::string path;

#ifdef FIX_BUGS
	if (surf_no >= SURFACE_ID_MAX)
#else
	if (surf_no > SURFACE_ID_MAX)
#endif
	{
		ErrorLog("surface no", surf_no);
		return false;
	}

	if (surf[surf_no] != NULL)
	{
		ErrorLog("existing", surf_no);
		return false;
	}

	unsigned int width, height;
	unsigned char *image_buffer = NULL;

	std::string file_extensions[] = {"pbm", "bmp", "png"};
	for (size_t i = 0; i < sizeof(file_extensions) / sizeof(file_extensions[0]); ++i)
	{
		path = name + std::string{'.'} + file_extensions[i];

		image_buffer = DecodeBitmapWithAlphaFromFile(path.c_str(), &width, &height, true, loadGlobal);

		if (image_buffer != NULL)
			break;
	}

	if (image_buffer == NULL)
	{
		ErrorLog(path.c_str(), 1);
		return false;
	}

	surf[surf_no] = RenderBackend_CreateSurface(width * mag / gSpriteScale, height * mag / gSpriteScale, false);

	if (surf[surf_no] == NULL)
	{
		FreeBitmap(image_buffer);
		return false;
	}

	if (!ScaleAndUploadSurface(image_buffer, width, height, surf_no))
	{
		RenderBackend_FreeSurface(surf[surf_no]);
		FreeBitmap(image_buffer);
		return false;
	}

	surface_metadata[surf_no].type = SURFACE_SOURCE_FILE;
	surface_metadata[surf_no].width = width / gSpriteScale;
	surface_metadata[surf_no].height = height / gSpriteScale;
	surface_metadata[surf_no].bSystem = false;
	strcpy(surface_metadata[surf_no].name, name);

	FreeBitmap(image_buffer);

	return true;
}

// TODO - Inaccurate stack frame
bool ReloadBitmap_File(const char *name, SurfaceID surf_no, bool loadGlobal)
{
	std::string path;

#ifdef FIX_BUGS
	if (surf_no >= SURFACE_ID_MAX)
#else
	if (surf_no > SURFACE_ID_MAX)
#endif
	{
		ErrorLog("surface no", surf_no);
		return false;
	}

	unsigned int width, height;
	unsigned char *image_buffer = NULL;

	std::string file_extensions[] = {"pbm", "bmp", "png"};
	for (size_t i = 0; i < sizeof(file_extensions) / sizeof(file_extensions[0]); ++i)
	{
		path = name + std::string{'.'} + file_extensions[i];

		image_buffer = DecodeBitmapWithAlphaFromFile(path.c_str(), &width, &height, true, loadGlobal);

		if (image_buffer != NULL)
			break;
	}

	if (image_buffer == NULL)
	{
		ErrorLog(path.c_str(), 1);
		return false;
	}

	if (!ScaleAndUploadSurface(image_buffer, width, height, surf_no))
	{
		FreeBitmap(image_buffer);
		return false;
	}

	FreeBitmap(image_buffer);

	surface_metadata[surf_no].type = SURFACE_SOURCE_FILE;
	strcpy(surface_metadata[surf_no].name, name);

	return true;
}

// TODO - Inaccurate stack frame
bool MakeSurface_Generic(int bxsize, int bysize, SurfaceID surf_no, bool bSystem)
{
#ifdef FIX_BUGS
	if (surf_no >= SURFACE_ID_MAX)
#else
	if (surf_no > SURFACE_ID_MAX)	// OOPS (should be '>=')
#endif
		return false;

	if (surf[surf_no] != NULL)
		return false;

	surf[surf_no] = RenderBackend_CreateSurface(bxsize * mag, bysize * mag, true);

	if (surf[surf_no] == NULL)
		return false;

	surface_metadata[surf_no].type = SURFACE_SOURCE_NONE;
	surface_metadata[surf_no].width = bxsize;
	surface_metadata[surf_no].height = bysize;

	if (bSystem)
		surface_metadata[surf_no].bSystem = true;
	else
		surface_metadata[surf_no].bSystem = false;

	strcpy(surface_metadata[surf_no].name, "generic");

	return true;
}

void BackupSurface(SurfaceID surf_no, const Rect *rect)
{
	static RenderBackend_Rect rcSet;	// TODO - Not the original variable name
	rcSet.left = rect->left * mag;
	rcSet.top = rect->top * mag;
	rcSet.right = rect->right * mag;
	rcSet.bottom = rect->bottom * mag;

	// Do not draw invalid Rects
	if (rcSet.right <= rcSet.left || rcSet.bottom <= rcSet.top)
		return;

	RenderBackend_Blit(framebuffer, &rcSet, surf[surf_no], rcSet.left, rcSet.top, false, false);
}

static void ScaleRect(const Rect *rect, RenderBackend_Rect *scaled_rect)
{
	scaled_rect->left = rect->left * mag;
	scaled_rect->top = rect->top * mag;
	scaled_rect->right = rect->right * mag;
	scaled_rect->bottom = rect->bottom * mag;
}

void PutBitmap3Ex(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no, int angle, float centerX, float centerY, float opacity) // Transparency
{
	static RenderBackend_Rect rcWork;
	ScaleRect(rect, &rcWork);

	static RenderBackend_Rect rcViewScaled;
	ScaleRect(rcView, &rcViewScaled);

	if (x + rcWork.right - rcWork.left > rcViewScaled.right)
		rcWork.right -= (x + rcWork.right - rcWork.left) - rcViewScaled.right;

	if (x < rcViewScaled.left)
	{
		rcWork.left += rcViewScaled.left - x;
		x = rcViewScaled.left;
	}

	if (y + rcWork.bottom - rcWork.top > rcViewScaled.bottom)
		rcWork.bottom -= (y + rcWork.bottom - rcWork.top) - rcViewScaled.bottom;

	if (y < rcViewScaled.top)
	{
		rcWork.top += rcViewScaled.top - y;
		y = rcViewScaled.top;
	}

	// Do not draw invalid RECTs
	if (rcWork.right <= rcWork.left || rcWork.bottom <= rcWork.top)
		return;

	RenderBackend_BlitEx(surf[surf_no], &rcWork, framebuffer, x, y, TRUE, angle, centerX, centerY, opacity);
}

void PutBitmap3(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no) // Transparency
{
	static RenderBackend_Rect rcWork;
	ScaleRect(rect, &rcWork);

	static RenderBackend_Rect rcViewScaled;
	ScaleRect(rcView, &rcViewScaled);

	if (x + rcWork.right - rcWork.left > rcViewScaled.right)
		rcWork.right -= (x + rcWork.right - rcWork.left) - rcViewScaled.right;

	if (x < rcViewScaled.left)
	{
		rcWork.left += rcViewScaled.left - x;
		x = rcViewScaled.left;
	}

	if (y + rcWork.bottom - rcWork.top > rcViewScaled.bottom)
		rcWork.bottom -= (y + rcWork.bottom - rcWork.top) - rcViewScaled.bottom;

	if (y < rcViewScaled.top)
	{
		rcWork.top += rcViewScaled.top - y;
		y = rcViewScaled.top;
	}

	// Do not draw invalid Rects
	if (rcWork.right <= rcWork.left || rcWork.bottom <= rcWork.top)
		return;

	RenderBackend_Blit(surf[surf_no], &rcWork, framebuffer, x, y, true, false);
}

void PutBitmap3(const Rect *rcView, int x, int y, const Rect *rect, float scale, SurfaceID surf_no) // Transparency
{
	static RenderBackend_Rect rcWork;
	ScaleRect(rect, &rcWork);

	static RenderBackend_Rect rcViewScaled;
	ScaleRect(rcView, &rcViewScaled);

	if (x + rcWork.right - rcWork.left > rcViewScaled.right)
		rcWork.right -= (x + rcWork.right - rcWork.left) - rcViewScaled.right;

	if (x < rcViewScaled.left)
	{
		rcWork.left += rcViewScaled.left - x;
		x = rcViewScaled.left;
	}

	if (y + rcWork.bottom - rcWork.top > rcViewScaled.bottom)
		rcWork.bottom -= (y + rcWork.bottom - rcWork.top) - rcViewScaled.bottom;

	if (y < rcViewScaled.top)
	{
		rcWork.top += rcViewScaled.top - y;
		y = rcViewScaled.top;
	}

	// Do not draw invalid Rects
	if (rcWork.right <= rcWork.left || rcWork.bottom <= rcWork.top)
		return;

	RenderBackend_BlitScaled(surf[surf_no], &rcWork, framebuffer, x, y, scale);
}

void PutBitmap3_Flip(const Rect *rcView, int x, int y, const Rect *rect, int color, SurfaceID surf_no) // Transparency
{
	static RenderBackend_Rect rcWork;
	ScaleRect(rect, &rcWork);

	static RenderBackend_Rect rcViewScaled;
	ScaleRect(rcView, &rcViewScaled);

	if (x + rcWork.right - rcWork.left > rcViewScaled.right)
		rcWork.right -= (x + rcWork.right - rcWork.left) - rcViewScaled.right;

	if (x < rcViewScaled.left)
	{
		rcWork.left += rcViewScaled.left - x;
		x = rcViewScaled.left;
	}

	if (y + rcWork.bottom - rcWork.top > rcViewScaled.bottom)
		rcWork.bottom -= (y + rcWork.bottom - rcWork.top) - rcViewScaled.bottom;

	if (y < rcViewScaled.top)
	{
		rcWork.top += rcViewScaled.top - y;
		y = rcViewScaled.top;
	}

	// Do not draw invalid Rects
	if (rcWork.right <= rcWork.left || rcWork.bottom <= rcWork.top)
		return;

	RenderBackend_Blit(surf[surf_no], &rcWork, framebuffer, x, y, true, (int)color, gMC.flip);
}

void PutBitmap3_Flip(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no) // Transparency
{
	static RenderBackend_Rect rcWork;
	ScaleRect(rect, &rcWork);

	static RenderBackend_Rect rcViewScaled;
	ScaleRect(rcView, &rcViewScaled);

	if (x + rcWork.right - rcWork.left > rcViewScaled.right)
		rcWork.right -= (x + rcWork.right - rcWork.left) - rcViewScaled.right;

	if (x < rcViewScaled.left)
	{
		rcWork.left += rcViewScaled.left - x;
		x = rcViewScaled.left;
	}

	if (y + rcWork.bottom - rcWork.top > rcViewScaled.bottom)
		rcWork.bottom -= (y + rcWork.bottom - rcWork.top) - rcViewScaled.bottom;

	if (y < rcViewScaled.top)
	{
		rcWork.top += rcViewScaled.top - y;
		y = rcViewScaled.top;
	}

	// Do not draw invalid Rects
	if (rcWork.right <= rcWork.left || rcWork.bottom <= rcWork.top)
		return;

	RenderBackend_Blit(surf[surf_no], &rcWork, framebuffer, x, y, true, gMC.flip);
}

void PutBitmap3_NoFlip(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no) // Transparency
{
	static RenderBackend_Rect rcWork;
	ScaleRect(rect, &rcWork);

	static RenderBackend_Rect rcViewScaled;
	ScaleRect(rcView, &rcViewScaled);

	if (x + rcWork.right - rcWork.left > rcViewScaled.right)
		rcWork.right -= (x + rcWork.right - rcWork.left) - rcViewScaled.right;

	if (x < rcViewScaled.left)
	{
		rcWork.left += rcViewScaled.left - x;
		x = rcViewScaled.left;
	}

	if (y + rcWork.bottom - rcWork.top > rcViewScaled.bottom)
		rcWork.bottom -= (y + rcWork.bottom - rcWork.top) - rcViewScaled.bottom;

	if (y < rcViewScaled.top)
	{
		rcWork.top += rcViewScaled.top - y;
		y = rcViewScaled.top;
	}

	// Do not draw invalid Rects
	if (rcWork.right <= rcWork.left || rcWork.bottom <= rcWork.top)
		return;

	RenderBackend_Blit(surf[surf_no], &rcWork, framebuffer, x, y, true, false);
}

void PutBitmap3_Flip(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no, float opacity) // Transparency
{
	static RenderBackend_Rect rcWork;
	ScaleRect(rect, &rcWork);

	static RenderBackend_Rect rcViewScaled;
	ScaleRect(rcView, &rcViewScaled);

	if (x + rcWork.right - rcWork.left > rcViewScaled.right)
		rcWork.right -= (x + rcWork.right - rcWork.left) - rcViewScaled.right;

	if (x < rcViewScaled.left)
	{
		rcWork.left += rcViewScaled.left - x;
		x = rcViewScaled.left;
	}

	if (y + rcWork.bottom - rcWork.top > rcViewScaled.bottom)
		rcWork.bottom -= (y + rcWork.bottom - rcWork.top) - rcViewScaled.bottom;

	if (y < rcViewScaled.top)
	{
		rcWork.top += rcViewScaled.top - y;
		y = rcViewScaled.top;
	}

	// Do not draw invalid Rects
	if (rcWork.right <= rcWork.left || rcWork.bottom <= rcWork.top)
		return;

	RenderBackend_Blit(surf[surf_no], &rcWork, framebuffer, x, y, true, opacity, gMC.flip);
}

void PutBitmap3(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no, float opacity) // Transparency
{
	static RenderBackend_Rect rcWork;
	ScaleRect(rect, &rcWork);

	static RenderBackend_Rect rcViewScaled;
	ScaleRect(rcView, &rcViewScaled);

	if (x + rcWork.right - rcWork.left > rcViewScaled.right)
		rcWork.right -= (x + rcWork.right - rcWork.left) - rcViewScaled.right;

	if (x < rcViewScaled.left)
	{
		rcWork.left += rcViewScaled.left - x;
		x = rcViewScaled.left;
	}

	if (y + rcWork.bottom - rcWork.top > rcViewScaled.bottom)
		rcWork.bottom -= (y + rcWork.bottom - rcWork.top) - rcViewScaled.bottom;

	if (y < rcViewScaled.top)
	{
		rcWork.top += rcViewScaled.top - y;
		y = rcViewScaled.top;
	}

	// Do not draw invalid Rects
	if (rcWork.right <= rcWork.left || rcWork.bottom <= rcWork.top)
		return;

	RenderBackend_Blit(surf[surf_no], &rcWork, framebuffer, x, y, true, opacity, false);
}

void PutBitmap4(const Rect *rcView, int x, int y, const Rect *rect, SurfaceID surf_no) // No Transparency
{
	static RenderBackend_Rect rcWork;
	ScaleRect(rect, &rcWork);

	static RenderBackend_Rect rcViewScaled;
	ScaleRect(rcView, &rcViewScaled);

	if (x + rcWork.right - rcWork.left > rcViewScaled.right)
		rcWork.right -= (x + rcWork.right - rcWork.left) - rcViewScaled.right;

	if (x < rcViewScaled.left)
	{
		rcWork.left += rcViewScaled.left - x;
		x = rcViewScaled.left;
	}

	if (y + rcWork.bottom - rcWork.top > rcViewScaled.bottom)
		rcWork.bottom -= (y + rcWork.bottom - rcWork.top) - rcViewScaled.bottom;

	if (y < rcViewScaled.top)
	{
		rcWork.top += rcViewScaled.top - y;
		y = rcViewScaled.top;
	}

	// Do not draw invalid Rects
	if (rcWork.right <= rcWork.left || rcWork.bottom <= rcWork.top)
		return;

	RenderBackend_Blit(surf[surf_no], &rcWork, framebuffer, x, y, false, false);
}

void Surface2Surface(int x, int y, const Rect *rect, int to, int from)
{
	static RenderBackend_Rect rcWork;

	rcWork.left = rect->left * mag;
	rcWork.top = rect->top * mag;
	rcWork.right = rect->right * mag;
	rcWork.bottom = rect->bottom * mag;

	// Do not draw invalid Rects
	if (rcWork.right <= rcWork.left || rcWork.bottom <= rcWork.top)
		return;

	RenderBackend_Blit(surf[from], &rcWork, surf[to], x * mag, y * mag, true, false);
}

void Surface2Surface(int x, int y, const Rect *rect, int to, int from, float scale)
{
	static RenderBackend_Rect rcWork;

	rcWork.left = rect->left * mag;
	rcWork.top = rect->top * mag;
	rcWork.right = rect->right * mag;
	rcWork.bottom = rect->bottom * mag;

	// Do not draw invalid Rects
	if (rcWork.right <= rcWork.left || rcWork.bottom <= rcWork.top)
		return;

	RenderBackend_BlitScaled(surf[from], &rcWork, surf[to], x * mag * scale, y * mag * scale, scale);
}

unsigned long GetCortBoxColor(unsigned long col)
{
	// Comes in 00BBGGRR, goes out 00BBGGRR
	return col;
}

void CortBox2(const Rect *rect, unsigned long col)
{
	static RenderBackend_Rect rcSet;	// TODO - Not the original variable name
	rcSet.left = rect->left * mag;
	rcSet.top = rect->top * mag;
	rcSet.right = rect->right * mag;
	rcSet.bottom = rect->bottom * mag;

	const unsigned char red = col & 0xFF;
	const unsigned char green = (col >> 8) & 0xFF;
	const unsigned char blue = (col >> 16) & 0xFF;
	const unsigned char alpha = (col >> 24) & 0xFF;

	// Do not draw invalid Rects
	if (rcSet.right <= rcSet.left || rcSet.bottom <= rcSet.top)
		return;

	RenderBackend_ColourFillLol(framebuffer, &rcSet, red, green, blue, alpha);
}

void CortBox(const Rect *rect, unsigned long col)
{
	static RenderBackend_Rect rcSet;	// TODO - Not the original variable name
	rcSet.left = rect->left * mag;
	rcSet.top = rect->top * mag;
	rcSet.right = rect->right * mag;
	rcSet.bottom = rect->bottom * mag;

	const unsigned char red = col & 0xFF;
	const unsigned char green = (col >> 8) & 0xFF;
	const unsigned char blue = (col >> 16) & 0xFF;

	// Do not draw invalid Rects
	if (rcSet.right <= rcSet.left || rcSet.bottom <= rcSet.top)
		return;

	RenderBackend_ColourFill(framebuffer, &rcSet, red, green, blue, 0xFF);
}

void CortBox2(const Rect *rect, unsigned long col, SurfaceID surf_no)
{
	static RenderBackend_Rect rcSet;	// TODO - Not the original variable name
	rcSet.left = rect->left * mag;
	rcSet.top = rect->top * mag;
	rcSet.right = rect->right * mag;
	rcSet.bottom = rect->bottom * mag;

	surface_metadata[surf_no].type = SURFACE_SOURCE_NONE;

	const unsigned char red = col & 0xFF;
	const unsigned char green = (col >> 8) & 0xFF;
	const unsigned char blue = (col >> 16) & 0xFF;
	const unsigned char alpha = (col >> 24) & 0xFF;

	// Do not draw invalid Rects
	if (rcSet.right <= rcSet.left || rcSet.bottom <= rcSet.top)
		return;

	RenderBackend_ColourFill(surf[surf_no], &rcSet, red, green, blue, alpha);
}

// Dummied-out log function
// According to the Mac port, its name really is just "out".
bool out(int unknown)
{
	char unknown2[0x100];
	int unknown3;
	int unknown4;
	int unknown5;

	(void)unknown;
	(void)unknown2;
	(void)unknown3;
	(void)unknown4;
	(void)unknown5;

	return true;
}

// TODO - Probably not the original variable name (this is an educated guess)
int RestoreSurfaces(void)
{
	int s;
	Rect rect;
	int surfaces_regenerated = 0;

	if (framebuffer == NULL)
		return surfaces_regenerated;

	if (RenderBackend_IsSurfaceLost(framebuffer))
	{
		++surfaces_regenerated;
		RenderBackend_RestoreSurface(framebuffer);
		out('f');	// 'f' for 'frontbuffer' (or, in this branch's case, 'framebuffer')
	}

	for (s = 0; s < SURFACE_ID_MAX; ++s)
	{
		if (surf[s] != NULL)
		{
			if (RenderBackend_IsSurfaceLost(surf[s]))
			{
				++surfaces_regenerated;
				RenderBackend_RestoreSurface(surf[s]);
				out('0' + s);	// The number of the surface lost

				if (!surface_metadata[s].bSystem)
				{
					switch (surface_metadata[s].type)
					{
						case SURFACE_SOURCE_NONE:
							rect.left = 0;
							rect.top = 0;
							rect.right = surface_metadata[s].width;
							rect.bottom = surface_metadata[s].height;
							CortBox2(&rect, 0, (SurfaceID)s);
							break;

						case SURFACE_SOURCE_FILE:
							ReloadBitmap_File(surface_metadata[s].name, (SurfaceID)s);
							break;
					}
				}
			}
		}
	}

	return surfaces_regenerated;
}

int SubpixelToScreenCoord(int coord)
{
	if (gbSmoothScrolling)
		return (coord * mag) / 0x200;
	else
		return (coord / (0x200 / gSpriteScale)) * (mag / gSpriteScale);
}

int SubpixelToScreenCoord(long long coord)
{
	if (gbSmoothScrolling)
		return (coord * mag) / 0x200;
	else
		return (coord / (0x200 / gSpriteScale)) * (mag / gSpriteScale);
}

int ScreenToSubpixelCoord(int coord)
{
	if (gbSmoothScrolling)
		return (coord / mag) * 0x200;
	else
		return (coord * (0x200 / gSpriteScale)) / (mag / gSpriteScale);
}

int PixelToScreenCoord(int coord)
{
	return coord * mag;
}

void InitTextObject()
{
	font = LoadFont("Font/font", gFontData.first * mag, gFontData.second * mag);
}

void PutText(int x, int y, const std::string& text, unsigned long color, bool shift_jis)
{
	PutText(x, y, text.c_str(), color, shift_jis);
}

void PutText(int x, int y, const char *text, unsigned long color, bool shift_jis)
{
	DrawCSE2Text(font, framebuffer, x * mag, y * mag, color, text, shift_jis);
}

void PutText2(int x, int y, const char *text, unsigned long color, SurfaceID surf_no, bool shift_jis)
{
	DrawCSE2Text(font, surf[surf_no], x * mag, y * mag, color, text, shift_jis);
}

void EndTextObject(void)
{
	UnloadFont(font);
}
