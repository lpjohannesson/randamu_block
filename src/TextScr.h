#ifndef __TEXTSCR_H__
#define __TEXTSCR_H__


#include <string>

#include "CommonDefines.h"

struct TEXT_SCRIPT
{
	// Path (reload when exit teleporter menu/inventory)
	std::string path;

	// Script buffer
	long size;
	char *data;

	// Mode (ex. NOD, WAI)
	signed char mode;

	// Flags
	signed char flags;

	// Current positions (read position in buffer, x position in line)
	int p_read;
	int p_write;

	// Current line to write to
	int line;

	// Line y positions
	int ypos_line[4];

	// Event stuff
	int wait;
	int wait_next;
	int next_event;

	// Yes/no selected
	signed char select;

	// Current face
	int face;
	int face_x;

	// Current item
	int item;
	int item_y;

	// Text rect
	Rect rcText;

	// ..?
	int offsetY;

	// NOD cursor blink
	unsigned char wait_beam;
};

extern TEXT_SCRIPT gTS;

extern int lastInteractedEntityID;

extern const Rect gRect_line;

/// Contains the latest value given through <MIM
extern unsigned int gMIMCurrentNum;

bool InitTextScript2(void);
void EndTextScript(void);
void EncryptionBinaryData2(unsigned char *pData, long size);
bool LoadTextScript2(const char *name);
bool LoadTextScript_Stage(const char *name);
std::string GetTextScriptPath(void);
bool StartTextScript(int no);
void StopTextScript(void);
void PutTextScript(void);
int ActualTextScriptProc(void);
int TextScriptProc(void);
void RestoreTextScript(void);

#endif // __TEXTSCR_H__