#ifndef __BULLET_H__
#define __BULLET_H__

#include "CommonDefines.h"
#include "NpChar.h"

#define BULLET_MAX 0x400

struct BULLET
{
	int flag;
	int code_bullet;
	int bbits;
	int cond;
	int x;
	int y;
	int xm;
	int ym;
	int tgt_x;
	int tgt_y;
	int act_no;
	int act_wait;
	int ani_wait;
	int ani_no;
	unsigned char direct;
	Rect rect;
	int count1;
	int count2;
	int life_count;
	int damage;
	int life;
	int enemyXL;
	int enemyYL;
	int blockXL;
	int blockYL;
	OTHER_Rect view;
	float knockX;
	float knockY;
	NPCHAR* npc;
	int angle;
	float centerX;
	float centerY;
	int iframes;
	float opacity;
};

struct BULLET_TABLE
{
	signed char damage;
	signed char life;
	int life_count;
	int bbits;
	int enemyXL;
	int enemyYL;
	int blockXL;
	int blockYL;
	OTHER_Rect view;
	float knockX;
	float knockY;
};

extern BULLET gBul[BULLET_MAX];

void InitBullet(void);
int CountArmsBullet(int arms_code);
int CountBulletNum(int bullet_code);
void DeleteBullet(int code);
void ClearBullet(void);
void PutBullet(int fx, int fy);
void SetBullet(int no, int x, int y, int dir);
BULLET* SetBullet(int no, int x, int y);
void ActBullet(void);
bool IsActiveSomeBullet(void);
bool IsBreakingBullet(BULLET &bul);

#endif // __BULLET_H__