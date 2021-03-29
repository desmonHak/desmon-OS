/* Screen functions */
#ifndef ND_SCREEN_H
#define ND_SCREEN_H
#include "ND_LanguageC.h"

void ND_Screen_WriteString(const char* msg);
void ND_Screen_Clear();
void ND_Screen_SetColor(uint8_t background, uint8_t foreground);
void ND_Screen_ResetColor();
void ND_Screen_WriteHex(uint32_t);
void ND_Screen_WriteDec(uint32_t);
#endif
