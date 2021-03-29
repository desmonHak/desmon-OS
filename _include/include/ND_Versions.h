/* NextDivel Version Headers
* (C) Adrian Arroyo Calle 2013
*/
#ifndef ND_VERSIONS_H
#define ND_VERSIONS_H
#include "ND_LanguageC.h"
/* NextDivel Main Version */
#define ND_VERSIONS_MAJOR 0
#define ND_VERSIONS_MINOR 1
#define ND_VERSIONS_PATCH 1
#define ND_VERSION "0.1.1"
/* NextDivel year */
#define ND_VERSIONS_YEAR 2013
/* NextDivel author, vendor */
#define ND_VERSIONS_AUTHOR "Adrian Arroyo Calle"
#define ND_VERSIONS_VENDOR "Divel"
#ifdef _LP64
#define ND_VERSIONS_ARCH "x86_64"
#else
#define ND_VERSIONS_ARCH "i386"
#endif
/* Common functions */
bool ND_Versions_CompatibilityCheck(int major, int minor, int patch); /* Says if the kernel supports at least the API for that older kernel*/
void ND_Versions_Startup();
#endif
