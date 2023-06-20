#ifndef __BOSSLIFE_H__
#define __BOSSLIFE_H__

#include "CommonDefines.h"

struct BOSSLIFE	// Not the original struct name
{
	bool flag;
	int *pLife;
	int max;
	int br;
	int count;
};

extern BOSSLIFE gBL;

void InitBossLife(void);
bool StartBossLife(int code_event);
bool StartBossLife2(void);
void PutBossLife(void);

#endif // __BOSSLIFE_H__