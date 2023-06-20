#ifndef __SELSTAGE_H__
#define __SELSTAGE_H__


#include "Json.h"
#include "CommonDefines.h"
#include <vector>

struct PERMIT_STAGE
{
	int index;
	int event;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PERMIT_STAGE, index, event)
};

extern std::vector<PERMIT_STAGE> gPermitStage;

extern int gSelectedStage;
extern int gStageSelectTitleY;

void ClearPermitStage(void);
bool AddPermitStage(int index, int event);
bool CheckPermitStage(int index);
bool SubPermitStage(int index);
void MoveStageSelectCursor(void);
void PutStageSelectObject(void);
int StageSelectLoop(int *p_event);

#endif // __SELSTAGE_H__