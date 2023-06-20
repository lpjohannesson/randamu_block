#ifndef __INPUT_H__
#define __INPUT_H__


#include "CommonDefines.h"

struct JOYSTICK_STATUS
{
	bool bButton[128];	// The original `Input.cpp` assumed there were 32 buttons (because of DirectInput's `DIJOYSTATE` struct)
};

void ReleaseDirectInput(void);
bool InitDirectInput(void);
bool GetJoystickStatus(JOYSTICK_STATUS *status);
bool ResetJoystickStatus(void);

#endif // __INPUT_H__