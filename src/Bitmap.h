#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <cstddef>

#include "CommonDefines.h"

unsigned char* DecodeBitmapWithAlpha(const unsigned char *in_buffer, size_t in_buffer_size, unsigned int *width, unsigned int *height, bool colour_key);
unsigned char* DecodeBitmapWithAlphaFromFile(const char *path, unsigned int *width, unsigned int *height, bool colour_key, bool loadGlobal = false);
unsigned char* DecodeBitmap(const unsigned char *in_buffer, size_t in_buffer_size, unsigned int *width, unsigned int *height);
unsigned char* DecodeBitmapFromFile(const char *path, unsigned int *width, unsigned int *height, bool loadGlobal = false);
void FreeBitmap(unsigned char *buffer);

#endif // __BITMAP_H__