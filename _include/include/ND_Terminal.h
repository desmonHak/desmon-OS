/* NextDivel Terminal easy-functions */
#ifndef ND_TERMINAL_H
#define ND_TERMINAL_H
#include "ND_LanguageC.h"
enum ND_VGA_Color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};
uint8_t ND_Terminal_MakeColor(enum ND_VGA_Color fg, enum ND_VGA_Color bg);
uint16_t ND_Terminal_MakeVGAEntry(char c, uint8_t color);
size_t ND_Terminal_Strlen(const char* str);
void ND_Terminal_Initialize();
void ND_Terminal_SetColor(uint8_t color);
void ND_Terminal_PutEntryAt(char c, uint8_t color, size_t x, size_t y);
void ND_Terminal_PutChar(char c);
void ND_Terminal_WriteString(const char* data);



#endif
