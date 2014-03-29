#ifndef	_DOLIXON_USRLIBRARY_H_
#define	_DOLIXON_USRLIBRARY_H_

#include "type.h"

struct UserLibrary
{
	static char * (* strcpy)(char* p_dst, const char* p_src);
	static int strlen(const char * str);
	static char * itoa(char *str, int num);
	static int printf(const char * fmt, ...);
	static int vsprintf(char * buf, const char * fmt, va_list args);
};

#endif
