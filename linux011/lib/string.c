/*
* linux/lib/string.c
*
* (C) 1991 Linus Torvalds
*/

#ifndef __GNUC__		// ĞèÒªGNU µÄC ±àÒëÆ÷±àÒë¡£
#error I want gcc!
#endif

#define extern
#define inline
#define __LIBRARY__
#include <string.h>
