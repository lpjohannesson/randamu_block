#ifndef __FONT_H__
#define __FONT_H__


#include <cstddef>

#include "Backends/Rendering.h"

struct CSE2FontObject;

CSE2FontObject* LoadFontFromData(const unsigned char *data, size_t data_size, unsigned int cell_width, unsigned int cell_height);
CSE2FontObject* LoadFont(const char *font_filename, unsigned int cell_width, unsigned int cell_height);
void DrawCSE2Text(CSE2FontObject *font_object, RenderBackend_Surface *surface, int x, int y, unsigned long colour, const char *string, bool shift_jis = false);
void UnloadFont(CSE2FontObject *font_object);
#endif // __FONT_H__