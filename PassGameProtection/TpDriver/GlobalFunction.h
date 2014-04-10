#ifndef __GLOBALFUNCTION_H__
#define __GLOBALFUNCTION_H__

#include "DriverHead.h"

VOID WPON()
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

VOID WPOFF()
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

//搜索特征码寻找地址
ULONG SearchCode(int StartingAddr,char* pCode,int Length)
{
	char Destination[256]="";
	int i=5000;
	while (i--)
	{
		RtlMoveMemory(Destination,(char*)StartingAddr,Length);
		if (RtlCompareMemory(pCode,Destination,Length)==Length)
		{
			return StartingAddr+Length;
		}
		StartingAddr++;
	}
	return 0;
}

//通过名称获得SSDT服务原始地址
ULONG GetServiceOldAddr(PCWSTR FunName)
{
	UNICODE_STRING _FunName;
	RtlInitUnicodeString(&_FunName,FunName);
	return (ULONG)MmGetSystemRoutineAddress(&_FunName);
}

//判断进程名称
extern"C" __declspec(dllimport) UCHAR* PsGetProcessImageFileName(IN PEPROCESS Process);
bool CheckProcessName(char* szName)
{
	//获取当前进程
	PEPROCESS nEProcess = PsGetCurrentProcess();
	//获取进程名称
	char* szProessaName = (char*)PsGetProcessImageFileName(nEProcess);
	//比较进程名
	if(strcmp(szProessaName,szName)==0)
	{		
		return true;
	}

	return false;
}

#endif