#ifndef __BACK_H__
#define __BACK_H__

#include "CommonDefines.h"

struct BACK
{
	bool flag;	// Basically unused
	int partsW;
	int partsH;
	int numX;
	int numY;
	int type;
	int fx;
};

extern BACK gBack;
extern int gWaterY;

bool InitBack(const char *fName, int type);
void ActBack(void);
void PutBack(int fx, int fy);
void PutFront(int fx, int fy);

#endif // __BACK_H__