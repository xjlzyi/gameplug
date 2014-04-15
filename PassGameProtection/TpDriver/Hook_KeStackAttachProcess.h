#ifndef __HOOK_KESTACKATTACHPROCESS_H__
#define __HOOK_KESTACKATTACHPROCESS_H__

#include "GlobalFunction.h"

ULONG g_uHookKeStackAttachProcessAddr1;
ULONG g_uHookKeStackAttachProcessAddr2;
ULONG g_uHookKeStackAttachProcessAddr3;

#pragma PAGECODE
VOID HookKeStackAttachProcess()
{

// 	83ed5e4f ff75fc          push    dword ptr [ebp-4]
// 	83ed5e52 ff7508          push    dword ptr [ebp+8]
// 	83ed5e55 e8797afeff      call    nt!KiAttachProcess (83ebd8d3)
// 	83ed5e5a eb1b            jmp     nt!KeStackAttachProcess+0xc5 (83ed5e77)
// 	83ed5e5c 8d8670010000    lea     eax,[esi+170h]
// 	83ed5e62 50              push    eax
// 	83ed5e63 ff75fc          push    dword ptr [ebp-4]
// 	83ed5e66 8bc6            mov     eax,esi
// 	83ed5e68 ff7508          push    dword ptr [ebp+8]
// 	83ed5e6b e8637afeff      call    nt!KiAttachProcess (83ebd8d3)
// 	83ed5e70 8b450c          mov     eax,dword ptr [ebp+0C]

	ULONG uKeStackAttachProcess = GetServiceOldAddr(L"KeStackAttachProcess");

	char ch = (char)0xE8;
	ULONG uCallAddr = SearchCode(uKeStackAttachProcess, &ch, 1);

	uCallAddr = SearchCode(uCallAddr, &ch, 1);
	//g_uHookKeStackAttachProcessAddr1 = uCallAddr;
	//KdPrint(("第一个Call地址=%x\n",g_uHookKeStackAttachProcessAddr1));

	uCallAddr = SearchCode(uCallAddr, &ch, 1);
	g_uHookKeStackAttachProcessAddr2 = uCallAddr;
	KdPrint(("第二个Call地址=%x\n",g_uHookKeStackAttachProcessAddr2));

	uCallAddr = SearchCode(uCallAddr, &ch, 1);
	g_uHookKeStackAttachProcessAddr3 = uCallAddr;
	KdPrint(("第三个Call地址=%x\n",g_uHookKeStackAttachProcessAddr3));

	CallHook((ULONG)MyKiAttachProcess,g_uHookKeStackAttachProcessAddr2);
	CallHook((ULONG)MyKiAttachProcess,g_uHookKeStackAttachProcessAddr3);
}

#pragma PAGECODE
VOID UnHookKeStackAttachProcess()
{	
	CallHook(g_uKiAttachProcessAddr, g_uHookKeStackAttachProcessAddr2);
	CallHook(g_uKiAttachProcessAddr, g_uHookKeStackAttachProcessAddr3);
}

#endif