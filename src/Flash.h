#ifndef __FLASH_H__
#define __FLASH_H__


extern unsigned long gFlashColor;

void InitFlash(void);
void SetFlash(int x, int y, int mode, unsigned int color = 0xFFFFFF | (0x80 << 24));
void ActFlash_Explosion(int flx, int fly);
void ActFlash_Flash(void);
void ActFlash(int flx, int fly);
void PutFlash(void);
void ResetFlash(void);

#endif // __FLASH_H__