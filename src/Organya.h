#ifndef __ORGANYA_H__
#define __ORGANYA_H__


#include "CommonDefines.h"

#define MAXTRACK 16
#define MAXMELODY 8
#define MAXDRAM 8

extern bool g_mute[MAXTRACK];	// Used by the debug Mute menu

bool MakeOrganyaWave(signed char track, signed char wave_no, signed char pipi);
void OrganyaPlayData(void);
void SetPlayPointer(long x);
bool LoadOrganya(const char *name);
void SetOrganyaPosition(unsigned int x);
unsigned int GetOrganyaPosition(void);
void PlayOrganyaMusic(void);
bool ChangeOrganyaVolume(signed int volume);
void StopOrganyaMusic(void);
void SetOrganyaFadeout(void);
bool StartOrganya(const char *wave_filename);
void EndOrganya(void);

extern bool bFadeout;
extern int Volume;
extern int pitch_mod;

#endif // __ORGANYA_H__