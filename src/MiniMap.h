#ifndef __MINIMAP_H__
#define __MINIMAP_H__


#include "CommonDefines.h"

extern signed char gMapping[0x80];

int MiniMapLoop(void);
bool IsMapping(void);
void StartMapping(void);
void SetMapping(int a);

#endif // __MINIMAP_H__