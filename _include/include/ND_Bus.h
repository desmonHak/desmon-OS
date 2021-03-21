/* Common operations with Bus I/O */
#ifndef ND_BUS_H
#define ND_BUS_H
#include "ND_LanguageC.h"
void ND_Bus_outb(uint16_t port, uint8_t value);
uint8_t ND_Bus_inb(uint16_t port);
uint16_t ND_Bus_inw(uint16_t port);

#endif
