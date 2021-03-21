#include "stdint.h"

#define all_screen 80*25*2

static char* vidptr = (char*)0xb8000;
static int ultima_direcion = 0;
const char line_salto = 10; // "\n" == 0x0a == 10
uint8_t columna = 1;

/*
memoria VGA:
Color number   Color name      RGB value    Hex value
0                  Black       0 0 0        00 00 00
1                  Blue       0 0 170      00 00 AA
2                  Green       0 170 0      00 AA 00
3                  Cyan       0 170 170    00 AA AA
4                  Red          170 0 0      AA 00 00
5                  Purple       170 0 170    AA 00 AA
6                  Brown       170 85 0     AA 55 00
7                  Gray       170 170 170  AA AA AA
8                  Dark Gray    85 85 85     55 55 55
9                  Light Blue   85 85 255    55 55 FF
10                 Light Green  85 255 85    55 FF 55
11                 Light Cyan   85 255 255    55 FF FF
12                 Light Red   255 85 85    FF 55 55
13                 Light Purple 255 85 255    FF 55 FF
14                 Yellow       255 255 85    FF FF 55
15                 White        255 255 255  FF FF FF 
*/

typedef struct stdio{
    uint8_t black;
    uint8_t blue;
    uint8_t green;
    uint8_t cyan;
    uint8_t red;
    uint8_t purple;
    uint8_t brown;
    uint8_t gray;
    uint8_t DarkGray;
    uint8_t LightBlue;
    uint8_t LightGreen;
    uint8_t LightCyan;
    uint8_t LightRed;
    uint8_t LightPurple;
    uint8_t yellow;
    uint8_t white
} __attribute__((packed)) stdio;
const stdio _stdio = {0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf};

typedef struct  _screen{
    uint8_t background;
    uint8_t foreground;
} _screen;
_screen screen ={0, 10};

