#include "Flags.h"

#include <cstring>

// Macros for setting, un-setting and getting flags
// Each flag is stored in a bit, so we can use the exact same macros we'd use for bits
#define SET_FLAG(x, i) ((x)[(i) / 8] |= 1 << ((i) % 8))
#define UNSET_FLAG(x, i) ((x)[(i) / 8] &= ~(1 << ((i) % 8)))
#define GET_FLAG(x, i) ((x)[(i) / 8] & (1 << ((i) % 8)))

unsigned char gFlagNPC[1000];
unsigned char gSkipFlag[8];

// Flag initializers
void InitFlags(void)
{
	memset(gFlagNPC, 0, sizeof(gFlagNPC));
}

void InitSkipFlags(void)
{
	memset(gSkipFlag, 0, sizeof(gSkipFlag));
}

// NPC flags
void SetNPCFlag(long a)
{
	SET_FLAG(gFlagNPC, a);
}

void CutNPCFlag(long a)
{
	UNSET_FLAG(gFlagNPC, a);
}

bool GetNPCFlag(long a)
{
	if (GET_FLAG(gFlagNPC, a))
		return true;
	else
		return false;
}

// Skip flags
void SetSkipFlag(long a)
{
	SET_FLAG(gSkipFlag, a);
}

void CutSkipFlag(long a)
{
	UNSET_FLAG(gSkipFlag, a);
}

bool GetSkipFlag(long a)
{
	if (GET_FLAG(gSkipFlag, a))
		return true;
	else
		return false;
}
