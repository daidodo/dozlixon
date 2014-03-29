#pragma once

#include "function.h"

struct CInline
{
	static int inl(int x,int y)
	{
		return fun(x,y);
	}
	static int (* fnptr)(int x,int y);
};