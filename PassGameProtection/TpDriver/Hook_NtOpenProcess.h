#ifndef __HOOK_NTOPENPROCESS_H__
#define __HOOK_NTOPENPROCESS_H__

#include "GlobalFunction.h"

ULONG g_MyHookedNtOpenProcessAddr;
//自定义NtOpenProcess InLine Hook的跳转地址
ULONG g_NtOpenProcessJmpAddr;
//原生ObOpenObjectByPointer地址
ULONG g_OriginPointAddr;
//TP InLine Hook的跳转地址
ULONG g_TPHookedNtOpenProcessJmpAddr;
#pragma PAGECODE
__declspec(naked) VOID __stdcall MyNtOpenProcess_Win7() 
{
	_asm
	{
		push dword ptr [ebp-0F4h]
		push dword ptr [ebp-0F0h]
		call g_OriginPointAddr
		jmp g_NtOpenProcessJmpAddr
	}
// 	if (CheckProcessName("DNF.exe") || CheckProcessName("TenSafe_1.exe") || CheckProcessName("Client.exe"))
// 	{
// 		__asm
// 		{
// 			jmp g_TPHookedNtOpenProcessJmpAddr
// 		}
// 	}
}

#pragma PAGECODE
VOID HookNtOpenProcess_Win7()
{
//  NtOpenProcess------------------------------------------------------------
// 	840199f8 ff75fc          push    dword ptr [ebp-4]
// 	840199fb ff75fc          push    dword ptr [ebp-4]
// 	840199fe ff750c          push    dword ptr [ebp+0Ch]
// 	84019a01 ff7508          push    dword ptr [ebp+8]
// 	84019a04 e863440600      call    nt!PsRevertThreadToSelf+0x6bb (8407de6c)

//	PsOpenProcess-------------------------------------------------------------
// 	8407e08d ffb50cffffff    push    dword ptr [ebp-0F4h]
// 	8407e093 ffb510ffffff    push    dword ptr [ebp-0F0h]
//	8407e099 e8c655fdff      call    nt!ObOpenObjectByPointer (84053664)-----HOOK的位置

	//特征码
	char pCode[] = 
	{
		(char)0xff, (char)0xb5,	(char)0x0c, 
		(char)0xff, (char)0xff, (char)0xff, 
		(char)0xff, (char)0xb5, (char)0x10, 
		(char)0xff, (char)0xff, (char)0xff,	(char)0xe8
	};
	__asm int 3;
	//特征码长度
	SIZE_T nLen = sizeof(pCode);
	//获取原生PsOpenProcess地址
	ULONG uOriginPsOpenProcessAddr = GetServiceOldAddr(L"PsRevertThreadToSelf") + 0x6bb;
	KdPrint(("PsOpenProcess地址:%x\n",uOriginPsOpenProcessAddr));
	//获取原生ObOpenObjectByPointer地址
	g_OriginPointAddr = GetServiceOldAddr(L"ObOpenObjectByPointer");
	//根据特征码获得HOOK的起始地址
	g_MyHookedNtOpenProcessAddr = SearchCode(uOriginPsOpenProcessAddr, pCode, nLen) - nLen;
	KdPrint(("HOOK的起始地址:%x\n",g_MyHookedNtOpenProcessAddr));
	//计算出自定义InLine Hook的跳转地址
	g_NtOpenProcessJmpAddr = g_MyHookedNtOpenProcessAddr + nLen + 4;
	KdPrint(("自定义InLine Hook的跳转地址:%x\n",g_NtOpenProcessJmpAddr));
	//计算出TP InLine Hook的跳转地址
	g_TPHookedNtOpenProcessJmpAddr = g_MyHookedNtOpenProcessAddr + nLen - 1;
	KdPrint(("TP InLine Hook的跳转地址:%x\n",g_TPHookedNtOpenProcessJmpAddr));
	
//	int nJmpAddr = (int)MyNtOpenProcess_Win7 - g_MyHookedNtOpenProcessAddr - 5;
// 	WPON();
// 	__asm
// 	{
// 		mov eax,g_MyHookedNtOpenProcessAddr
// 		mov byte ptr [eax],0xE9
// 		mov ebx,nJmpAddr	
// 		mov dword ptr [eax+1],ebx
// 	}
// 	WPOFF();
}

#pragma PAGECODE
VOID UnHookNtOpenProcess_Win7()
{
	char pCode[] = 
	{
		(char)0xff, (char)0xb5,	(char)0x0c, 
		(char)0xff, (char)0xff, (char)0xff, 
		(char)0xff, (char)0xb5, (char)0x10, 
		(char)0xff, (char)0xff, (char)0xff,	(char)0xe8
	};

// 	WPON();
// 	RtlMoveMemory((char*)g_MyHookedNtOpenProcessAddr, pCode, 5);
// 	WPOFF();
}

#endif