#ifndef __PAGE_PROTECT_H__
#define __PAGE_PROTECT_H__

#include "DriverHead.h"

VOID RemovePageProtect()
{
	//去除保护
	_asm
	{
		cli
			mov eax,cr0
			and eax, not 10000h
			mov cr0,eax
	}
}

VOID ResetPageProtect()
{
	//还原保护
	__asm
	{
		mov eax,cr0
			or eax,10000h
			mov cr0,eax
			sti
	}
}


#endif