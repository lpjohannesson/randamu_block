#ifndef __VALUEVIEW_H__
#define __VALUEVIEW_H__


#include "CommonDefines.h"

#define VALUEVIEW_MAX 0x10

struct VALUEVIEW
{
  bool flag;
  int *px;
  int *py;
  int offset_y;
  int value;
  int count;
  Rect rect;
};

extern VALUEVIEW gVV[VALUEVIEW_MAX];
extern int gVVIndex;

void ClearValueView(void);
void SetValueView(int *px, int *py, int value);
void ActValueView(void);
void PutValueView(int flx, int fly);

#endif // __VALUEVIEW_H__