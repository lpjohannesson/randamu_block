#ifndef __PIXTONE_H__
#define __PIXTONE_H__



struct PIXTONEPARAMETER2
{
	int model;
	double num;
	int top;
	int offset;
};

struct PIXTONEPARAMETER
{
	int use;
	int size;
	PIXTONEPARAMETER2 oMain;
	PIXTONEPARAMETER2 oPitch;
	PIXTONEPARAMETER2 oVolume;
	int initial;
	int pointAx;
	int pointAy;
	int pointBx;
	int pointBy;
	int pointCx;
	int pointCy;
};

extern signed char gWaveModelTable[6][0x100];

void MakeWaveTables(void);
bool MakePixelWaveData(const PIXTONEPARAMETER *ptp, unsigned char *pData);

#endif // __PIXTONE_H__