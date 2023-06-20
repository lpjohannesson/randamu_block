#ifndef __COMMONDEFINES_H__
#define __COMMONDEFINES_H__


#define WINDOW_WIDTH 426
#define WINDOW_HEIGHT 240

enum Collisions
{
	COLL_LEFT_WALL = 1,     // Touching a left wall
	COLL_CEILING = 2,       // Touching a ceiling
	COLL_RIGHT_WALL = 4,    // Touching a right wall
	COLL_GROUND = 8         // Touching the ground
	// To be continued
};

enum Direction
{
	DIR_LEFT = 0,
	DIR_UP = 1,
	DIR_RIGHT = 2,
	DIR_DOWN = 3,
	DIR_AUTO = 4
};

struct OTHER_Rect	// The original name for this struct is unknown
{
	int front;
	int top;
	int back;
	int bottom;
};

#define RGBA(r,g,b,a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))
#define RGB(r,g,b) ((r) | ((g) << 8) | ((b) << 16))

struct Rect
{
	long left;
	long top;
	long right;
	long bottom;
};

#endif // __COMMONDEFINES_H__