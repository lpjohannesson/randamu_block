#ifndef __MAPNAME_H__
#define __MAPNAME_H__


#include "CommonDefines.h"

struct MAP_NAME
{
	bool flag;
	int wait;
	char name[0x20];
};

extern MAP_NAME gMapName;

void ReadyMapName(const char *str);
void PutMapName(bool bMini);
void StartMapName(void);
void RestoreMapName(void);

#endif // __MAPNAME_H__