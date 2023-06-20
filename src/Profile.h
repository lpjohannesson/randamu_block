#ifndef __PROFILE_H__
#define __PROFILE_H__


#include "CommonDefines.h"
#include <cstdint>

#include "ArmsItem.h"
#include "MyChar.h"
#include "SelStage.h"

extern const char* const gDefaultName;
extern const char* const gProfileCode;

bool IsProfile(void);
bool SaveProfile(const char *name);
bool LoadProfile(const char *name);
bool InitializeGame(void);

#endif // __PROFILE_H__