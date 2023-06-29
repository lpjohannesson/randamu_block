#include "KeyControl.h"

#include "Backends/Misc.h"
#include "Draw.h"
#include "Input.h"

bool gKeyboardState[BACKEND_KEYBOARD_TOTAL];
JOYSTICK_STATUS gJoystickState;

long gKey;
long gKeyTrg;

long gKeyJump = KEY_JUMP;
long gKeyShot = KEY_SHOT;
long gKeyArms = KEY_ARMS;
long gKeyArmsRev = KEY_ARMSREV;
long gKeyItem = KEY_ITEM;
long gKeyMap = KEY_MAP;

long gKeyOk = KEY_OK;
long gKeyCancel = KEY_CANCEL;

long gKeyLeft = KEY_LEFT;
long gKeyUp = KEY_UP;
long gKeyRight = KEY_RIGHT;
long gKeyDown = KEY_DOWN;
long gKeyAbility = KEY_ABILITY;
long gKeyDash = KEY_DASH;
long gKeyTurboText = KEY_TURBO_TEXT;

bool gMouseHold;
long gMouseClick = KEY_MOUSE;
int gMouseX;
int gMouseY;
int gMouseVelX;
int gMouseVelY;

int gNumberKey = -1;

void GetTrg(void)
{
	static int key_old;
	gKeyTrg = gKey ^ key_old;
	gKeyTrg = gKey & gKeyTrg;
	key_old = gKey;
}

bool IsMouseInWindow(){
	return (gMouseX >= 0 && gMouseX <= PixelToScreenCoord(WINDOW_WIDTH)) &&
		(gMouseY >= 0 && gMouseY <= PixelToScreenCoord(WINDOW_HEIGHT));
}
