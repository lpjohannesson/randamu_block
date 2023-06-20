#ifndef __CONFIG_H__
#define __CONFIG_H__


#include "CommonDefines.h"

#include "Input.h"

enum
{
	BINDING_UP,
	BINDING_DOWN,
	BINDING_LEFT,
	BINDING_RIGHT,
	BINDING_OK,
	BINDING_CANCEL,
	BINDING_JUMP,
	BINDING_SHOT,
	BINDING_ARMSREV,
	BINDING_ARMS,
	BINDING_ITEM,
	BINDING_MAP,
	BINDING_PAUSE,
	BINDING_ABILITY,
	BINDING_DASH,
	BINDING_TOTAL
};

struct CONFIG_BINDING
{
	int keyboard;
	unsigned char controller;
};

struct CONFIG
{
	char proof[0x20];
	char font_name[0x40];
	unsigned char display_mode;
	bool b60fps;
	bool bVsync;
	bool bSmoothScrolling;
	CONFIG_BINDING bindings[BINDING_TOTAL];
	bool bExtendedInfo;
};

extern const char* const gConfigName;
extern const char* const gProof;

bool LoadConfigData(CONFIG *conf);
bool SaveConfigData(const CONFIG *conf);
void DefaultConfigData(CONFIG *conf);

#endif // __CONFIG_H__