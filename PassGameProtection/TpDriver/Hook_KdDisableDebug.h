#ifndef __HOOK_KDDISABLEDEBUG_H__
#define __HOOK_KDDISABLEDEBUG_H__

#include "GlobalFunction.h"

//KdDebuggerEnabled变量存储
BOOL g_bKdDebuggerEnabled = TRUE;
//KdPitchDebugger变量存储
BOOL g_bKdPitchDebugger = FALSE;

VOID LoadImageRoutine(__in_opt PUNICODE_STRING  FullImageName,
					__in HANDLE  ProcessId,
					__in PIMAGE_INFO  ImageInfo
					)
{

}

VOID MoveVariable_Win7()
{
// 	83e850c2 803d2cfdf68300  cmp     byte ptr [nt!KdDebuggerEnabled (83f6fd2c)],0
// 	83e850c9 7412            je      nt!KeUpdateRunTime+0x164 (83e850dd)
// 	83e850cb a1ec31f483      mov     eax,dword ptr [nt!POGOBuffer+0x6ac (83f431ec)]
// 	83e850d0 3b86cc030000    cmp     eax,dword ptr [esi+3CCh]
// 	83e850d6 7505            jne     nt!KeUpdateRunTime+0x164 (83e850dd)
// 	83e850d8 e80c000000      call    nt!KeUpdateRunTime+0x170 (83e850e9)

	//----------------------------改写KeUpdateRunTime中的KdDebuggerEnabled 
	__asm int 3;
	ULONG uKeUpdateRunTimeAddr = GetServiceOldAddr(L"KeUpdateRunTime");
	char pCode[]={(char)0x74, (char)0x12, (char)0xa1};
	ULONG uAddr = SearchCode(uKeUpdateRunTimeAddr,pCode,sizeof(pCode)) - sizeof(pCode) - 5;
	KdPrint(("KeUpdateRunTime->KdDebuggerEnabled=%x\n",uAddr));
// 	DisableWP();
// 	*(PULONG)uAddr=(ULONG)&g_bKdDebuggerEnabled;
// 	EnableWP();

	//----------------------------改写KdCheckForDebugBreak中的KdDebuggerEnabled
	char pCode2 = (char)0xE8;	
	uAddr = SearchCode(uAddr,&pCode2,sizeof(pCode2));
	KdPrint(("KdCheckForDebugBreak->KdDebuggerEnabled=%x\n",uAddr));
	//记录KdCheckForDebugBreak函数地址 用于KdPitchDebugger变量的处理
	ULONG uAddr2 = uAddr;
	char pCode3[] = {(char)0x74, (char)0x10, (char)0xe8};
	uAddr = SearchCode(uAddr,pCode3,sizeof(pCode3)) - sizeof(pCode3);
// 	DisableWP();
// 	*(PULONG)uAddr=(ULONG)&g_bKdDebuggerEnabled;
// 	EnableWP();

	//----------------------------改写KdCheckForDebugBreak中的KdPitchDebugger=ExFreePool+0x72c0
// 	83e850e9 803d273df38300  cmp     byte ptr [nt!ExFreePool+0x72c0 (83f33d27)],0
// 	83e850f0 7519            jne     nt!KeUpdateRunTime+0x192 (83e8510b)
// 	83e850f2 803d2cfdf68300  cmp     byte ptr [nt!KdDebuggerEnabled (83f6fd2c)],0
	uAddr2 += 2;
	KdPrint(("KdCheckForDebugBreak->KdPitchDebugger=%x\n",uAddr2));
// 	DisableWP();
// 	*(PULONG)uAddr2=(ULONG)&g_bKdPitchDebugger;
// 	EnableWP();


	//----------------------------改写KdPollBreakIn中的KdDebuggerEnabled
// 	83e850fb e81f000000      call    nt!KdPollBreakIn (83e8511f)
// 	83e85100 84c0            test    al,al
	char pCode4[] = {(char)0x84, (char)0xc0};
	uAddr = SearchCode(uAddr,pCode4,sizeof(pCode4)) - sizeof(pCode4);
	KdPrint(("KdCheckForDebugBreak->KdDebuggerEnabled=%x\n",uAddr));

	//记录KdPollBreakIn函数地址 用于KdPitchDebugger变量的处理
	uAddr2 = uAddr;

// 	83e85137 885dff          mov     byte ptr [ebp-1],bl
// 	83e8513a 381d2cfdf683    cmp     byte ptr [nt!KdDebuggerEnabled (83f6fd2c)],bl
// 	83e85140 0f84c0000000    je      nt!KdPollBreakIn+0xe7 (83e85206)
	char pCode5[] = {(char)0x88, (char)0x5d, (char)0xff, (char)0x38, (char)0x1d};
	uAddr = SearchCode(uAddr,pCode5,sizeof(pCode5));
	KdPrint(("KdPollBreakIn->KdDebuggerEnabled=%x\n",uAddr));
// 	DisableWP();
// 	*(PULONG)uAddr=(ULONG)&g_bKdDebuggerEnabled;
// 	EnableWP();

	//----------------------------改写KdPollBreakIn中的KdPitchDebugger
// 	83e85126 33db            xor     ebx,ebx
// 	83e85128 381d273df383    cmp     byte ptr [nt!KdPitchDebugger (83f33d27)],bl
// 	83e8512e 7407            je      nt!KdPollBreakIn+0x18 (83e85137)
	char pCode6[] = {(char)0x33, (char)0xdb, (char)0x38, (char)0x1d};
	uAddr2 = SearchCode(uAddr2,pCode6,sizeof(pCode6));
	KdPrint(("KdPollBreakIn->KdPitchDebugger=%x\n",uAddr2));
// 	DisableWP();
// 	*(PULONG)uAddr2=(ULONG)&g_bKdPitchDebugger;
// 	EnableWP();
}

#pragma PAGECODE
VOID HookKdDisableDebugger()
{

	DisableWP();
	KIRQL kIrql = KeRaiseIrqlToDpcLevel();


	KeLowerIrql(kIrql);
	EnableWP();
	PsSetLoadImageNotifyRoutine(LoadImageRoutine);
}

#pragma PAGECODE
VOID UnHookKdDisableDebugger()
{
	DisableWP();

	EnableWP();
	PsRemoveLoadImageNotifyRoutine(LoadImageRoutine);
}

#endif