#ifndef __MYCPARAM_H__
#define __MYCPARAM_H__


#include "CommonDefines.h"

struct ARMS_LEVEL
{
	int exp[4];
};

struct REC
{
	long counter[4];
	unsigned char random[4];
};

extern ARMS_LEVEL gArmsLevelTable[];
extern int time_count;
void AddScore(int x);
void AddExpMyChar(int x);
void ZeroExpMyChar(void);
bool IsMaxExpMyChar(void);
void DamageMyChar(int damage);
void ZeroArmsEnergy_All(void);
void AddBulletMyChar(int no, int val);
void AddLifeMyChar(int x);
void AddMaxLifeMyChar(int val);
void PutArmsEnergy(bool flash);
void PutActiveArmsList(void);
void PutActiveArm(void);
void PutMyLife(bool flash);
void PutMyScore(int y);
void PutMyAir(int x, int y);
void PutTimeCounter(int x, int y);
void PutTimeCounter(int x, int y, int *timerVar);
bool SaveTimeCounter(void);
int LoadTimeCounter(void);
void Die(void);
#endif // __MYCPARAM_H__