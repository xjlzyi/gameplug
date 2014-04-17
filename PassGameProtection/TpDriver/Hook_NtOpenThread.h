#ifndef __HOOK_NTOPENTHREAD_H__
#define __HOOK_NTOPENTHREAD_H__

#include "GlobalFunction.h"

//自定义NtOpenThread InLine Hook的跳转地址
ULONG g_NtOpenThreadJmpAddr;
//根据特征码获得HOOK的起始地址
ULONG g_MyHookedNtOpenThreadAddr;
#pragma PAGECODE
__declspec(naked) VOID __stdcall MyNtOpenThread_Win7()
{
	_asm
	{
		push dword ptr [ebp-0F0h]
		push dword ptr [ebp-0ECh]
		call g_OriginPointAddr
		jmp g_NtOpenThreadJmpAddr
	}
}

#pragma PAGECODE
VOID HookNtOpenThread_Win7()
{
// 	84071d0d ffb510ffffff    push    dword ptr [ebp-0F0h]
// 	84071d13 ffb514ffffff    push    dword ptr [ebp-0ECh]
// 	84071d19 e846d9feff      call    nt!ObOpenObjectByPointer (8405f664)
// 	84071d1e 8bf0            mov     esi,eax
	char pCode[] = 
	{
		(char)0xff, (char)0xb5,	(char)0x10, 
		(char)0xff, (char)0xff, (char)0xff, 
		(char)0xff, (char)0xb5, (char)0x14, 
		(char)0xff, (char)0xff, (char)0xff,	(char)0xe8
	};
	//特征码长度
	SIZE_T nLen = sizeof(pCode);
	//获取原生PsOpenThread地址
	ULONG uOriginPsOpenThreadAddr = GetServiceOldAddr(L"NtQueryInformationThread") + 0xd9d;
	KdPrint(("PsOpenThread地址:%x\n",uOriginPsOpenThreadAddr));
	//根据特征码获得HOOK的起始地址
	g_MyHookedNtOpenThreadAddr = SearchCode(uOriginPsOpenThreadAddr, pCode, nLen) - nLen;
	KdPrint(("HOOK的起始地址:%x\n",g_MyHookedNtOpenThreadAddr));
	//计算出自定义InLine Hook的跳转地址
	g_NtOpenThreadJmpAddr = g_MyHookedNtOpenThreadAddr + nLen + 4;
	KdPrint(("自定义InLine Hook的跳转地址:%x\n",g_NtOpenThreadJmpAddr));

	int nJmpAddr = (int)MyNtOpenThread_Win7 - g_MyHookedNtOpenThreadAddr - 5;
	DisableWP();
	__asm
	{
		mov eax,g_MyHookedNtOpenThreadAddr
		mov byte ptr [eax],0xE9
		mov ebx,nJmpAddr	
		mov dword ptr [eax+1],ebx
	}
	EnableWP();
}

#pragma PAGECODE
VOID UnHookNtOpenThread_Win7()
{
	char pCode[] = 
	{
		(char)0xff, (char)0xb5,	(char)0x10, 
		(char)0xff, (char)0xff, (char)0xff, 
		(char)0xff, (char)0xb5, (char)0x14, 
		(char)0xff, (char)0xff, (char)0xff,	(char)0xe8
	};
	DisableWP();
	RtlMoveMemory((char*)g_MyHookedNtOpenThreadAddr, pCode, 5);
	EnableWP();
}

#endif