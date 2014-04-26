#ifndef __HOOK_KDDEBUGGERENABLED_H__
#define __HOOK_KDDEBUGGERENABLED_H__

#include "..\GlobalFunction.h"

//KdDebuggerEnabled变量存储
BOOL g_bKdDebuggerEnabled = TRUE;
//KdPitchDebugger变量存储
BOOL g_bKdPitchDebugger = FALSE;

//HOOK的KdDebuggerEnabled变量和地址
HookVariableAddr g_uKdDebuggerEnabledHookAddrs[5]={0};

ULONG g_uKdDebuggerEnabled;

ULONG g_uKdPitchDebugger;

#pragma PAGECODE
VOID MovKdPitchDebugger_Win7()
{
	DisableWP();
	*(PULONG)g_uKdPitchDebugger = g_bKdPitchDebugger;
	EnableWP();
	KdPrint(("KdPitchDebugger值=%x\n",*(PULONG)g_uKdPitchDebugger));
}

#pragma PAGECODE
VOID MovKdDebuggerEnabled_Win7()
{
	DisableWP();
	*(PULONG)g_uKdDebuggerEnabled = g_bKdDebuggerEnabled;
	EnableWP();
	KdPrint(("KdDebuggerEnabled值=%x\n",*(PULONG)g_bKdDebuggerEnabled));
}

#pragma PAGECODE
VOID MoveVariable_Win7()
{	
// 	ULONG uKeUpdateRunTimeAddr = GetServiceOldAddr(L"KeUpdateRunTime");
// 	char pCode[]={(char)0x74, (char)0x12, (char)0xa1};
// 	ULONG uAddr = SearchCode(uKeUpdateRunTimeAddr,pCode,sizeof(pCode)) - 8;
// 	KdPrint(("KeUpdateRunTime->KdDebuggerEnabled=%x\n",uAddr));
// 	g_uKdDebuggerEnabled = *(PULONG)uAddr;
// 	DisableWP();
// 	*(PULONG)(*(PULONG)uAddr) = g_bKdDebuggerEnabled;
// 	EnableWP();


	// 	83e850c2 803d2cfdf68300  cmp     byte ptr [nt!KdDebuggerEnabled (83f6fd2c)],0
	// 	83e850c9 7412            je      nt!KeUpdateRunTime+0x164 (83e850dd)
	// 	83e850cb a1ec31f483      mov     eax,dword ptr [nt!POGOBuffer+0x6ac (83f431ec)]
	// 	83e850d0 3b86cc030000    cmp     eax,dword ptr [esi+3CCh]
	// 	83e850d6 7505            jne     nt!KeUpdateRunTime+0x164 (83e850dd)
	// 	83e850d8 e80c000000      call    nt!KdCheckForDebugBreak (83e850e9)

	//----------------------------改写KeUpdateRunTime中的KdDebuggerEnabled 
	ULONG uKeUpdateRunTimeAddr = GetServiceOldAddr(L"KeUpdateRunTime");
	char pCode[]={(char)0x74, (char)0x12, (char)0xa1};
	ULONG uAddr = SearchCode(uKeUpdateRunTimeAddr,pCode,sizeof(pCode)) - 8;
	KdPrint(("KeUpdateRunTime->KdDebuggerEnabled=%x\n",uAddr));
	g_uKdDebuggerEnabledHookAddrs[0].uAddr = uAddr;
	g_uKdDebuggerEnabledHookAddrs[0].uVariableAddr = *(PULONG)uAddr;
	DisableWP();
	*(PULONG)uAddr=(ULONG)&g_bKdDebuggerEnabled;
	EnableWP();

	//----------------------------改写KdCheckForDebugBreak中的KdDebuggerEnabled
	char pCode2 = (char)0xE8;
	//KdCheckForDebugBreak的函数地址
	uAddr = SearchCode(uAddr,&pCode2,sizeof(pCode2));
	uAddr = uAddr + (*(PULONG)uAddr) + 4;
	KdPrint(("KdCheckForDebugBreak函数地址=%x\n",uAddr));
	//记录KdCheckForDebugBreak函数地址 用于KdPitchDebugger变量的处理
	ULONG uAddr2 = uAddr;
	// 	83ebe0f2 803d2c8dfa8300  cmp     byte ptr [nt!KdDebuggerEnabled (83fa8d2c)],0
	//	83ebe0f9 7410            je      nt!KdCheckForDebugBreak+0x22 (83ebe10b)
	char pCode3[] = {(char)0x74, (char)0x10, (char)0xe8};
	//KdCheckForDebugBreak中KdDebuggerEnabled的地址
	uAddr = SearchCode(uAddr,pCode3,sizeof(pCode3)) - 8;
	KdPrint(("KdCheckForDebugBreak->KdDebuggerEnabled=%x\n",uAddr));
	g_uKdDebuggerEnabledHookAddrs[1].uAddr = uAddr;
	g_uKdDebuggerEnabledHookAddrs[1].uVariableAddr = *(PULONG)uAddr;
	DisableWP();
	*(PULONG)uAddr=(ULONG)&g_bKdDebuggerEnabled;
	EnableWP();

	//----------------------------改写KdCheckForDebugBreak中的KdPitchDebugger=ExFreePool+0x72c0
	// 	83ebe0e9 803d27cdf68300  cmp     byte ptr [nt!KdPitchDebugger (83f6cd27)],0
	// 	83ebe0f0 7519            jne     nt!KdCheckForDebugBreak+0x22 (83ebe10b)
	//KdPitchDebugger的函数地址
	uAddr2 += 2;
// 	g_uKdPitchDebugger = *(PULONG)uAddr2;
// 	KdPrint(("KdPitchDebugger函数地址=%x\n",*(PULONG)uAddr2));
// 	KdPrint(("KdDebuggerEnabled函数地址=%x\n",g_uKdDebuggerEnabled));
// 	KdPrint(("KdDebuggerEnabled值=%x\n",*(PULONG)g_uKdDebuggerEnabled));
// 	DisableWP();
// 	*(PULONG)g_uKdPitchDebugger = g_bKdPitchDebugger;
// 	EnableWP();
// 	KdPrint(("KdPitchDebugger值=%x\n",*(PULONG)g_uKdPitchDebugger));
// 	MovKdPitchDebugger_Win7();
// 	DisableWP();
// 	*(PULONG)g_uKdDebuggerEnabled = g_bKdDebuggerEnabled;
// 	EnableWP();
// 	KdPrint(("KdDebuggerEnabled2值=%x\n",*(PULONG)g_uKdDebuggerEnabled));
// 	MovKdDebuggerEnabled_Win7();
	g_uKdDebuggerEnabledHookAddrs[2].uAddr = uAddr2;
	g_uKdDebuggerEnabledHookAddrs[2].uVariableAddr = *(PULONG)uAddr2;
	DisableWP();
	*(PULONG)uAddr2=(ULONG)&g_bKdPitchDebugger;
	EnableWP();

	//----------------------------改写KdPollBreakIn中的KdDebuggerEnabled
	// 	83e850fb e81f000000      call    nt!KdPollBreakIn (83e8511f)
	// 	83e85100 84c0            test    al,al
	char pCode4[] = {(char)0x84, (char)0xc0};
	//KdPollBreakIn函数地址
	uAddr = SearchCode(uAddr,pCode4,sizeof(pCode4)) - 6;
	uAddr = uAddr + *((PULONG)uAddr) + 4;
	KdPrint(("KdPollBreakIn函数地址=%x\n",uAddr));

	//记录KdPollBreakIn函数地址 用于KdPitchDebugger变量的处理
	uAddr2 = uAddr;

	// 	83e85137 885dff          mov     byte ptr [ebp-1],bl
	// 	83e8513a 381d2cfdf683    cmp     byte ptr [nt!KdDebuggerEnabled (83f6fd2c)],bl
	// 	83e85140 0f84c0000000    je      nt!KdPollBreakIn+0xe7 (83e85206)
	char pCode5[] = {(char)0x88, (char)0x5d, (char)0xff, (char)0x38, (char)0x1d};
	//KdPollBreakIn函数中KdDebuggerEnabled地址
	uAddr = SearchCode(uAddr,pCode5,sizeof(pCode5));
	KdPrint(("KdPollBreakIn->KdDebuggerEnabled=%x\n",uAddr));
	g_uKdDebuggerEnabledHookAddrs[3].uAddr = uAddr;
	g_uKdDebuggerEnabledHookAddrs[3].uVariableAddr = *(PULONG)uAddr;
	DisableWP();
	*(PULONG)uAddr=(ULONG)&g_bKdDebuggerEnabled;
	EnableWP();

	//----------------------------改写KdPollBreakIn中的KdPitchDebugger
	// 	83e85126 33db            xor     ebx,ebx
	// 	83e85128 381d273df383    cmp     byte ptr [nt!KdPitchDebugger (83f33d27)],bl
	// 	83e8512e 7407            je      nt!KdPollBreakIn+0x18 (83e85137)
	char pCode6[] = {(char)0x33, (char)0xdb, (char)0x38, (char)0x1d};
	//KdPollBreakIn中的KdPitchDebugger地址
	uAddr2 = SearchCode(uAddr2,pCode6,sizeof(pCode6));
	KdPrint(("KdPollBreakIn->KdPitchDebugger=%x\n",uAddr2));
	g_uKdDebuggerEnabledHookAddrs[4].uAddr = uAddr2;
	g_uKdDebuggerEnabledHookAddrs[4].uVariableAddr = *(PULONG)uAddr2;
	DisableWP();
	*(PULONG)uAddr2=(ULONG)&g_bKdPitchDebugger;
	EnableWP();
}
#pragma PAGECODE
VOID ResetKdPitchDebugger()
{
	DisableWP();
	*(PULONG)g_uKdPitchDebugger = 0x300;
	EnableWP();
	KdPrint(("KdPitchDebugger值=%x\n",*(PULONG)g_uKdPitchDebugger));
}

#pragma PAGECODE
VOID ResetKdDebuggerEnabled()
{
	DisableWP();
	*(PULONG)g_uKdDebuggerEnabled = g_bKdPitchDebugger;
	EnableWP();
	KdPrint(("KdDebuggerEnabled值=%x\n",*(PULONG)g_uKdDebuggerEnabled));
}

#pragma PAGECODE
VOID ResetVariable_Win7()
{
// 	ResetKdPitchDebugger();
// 	ResetKiDebugRutine_Win7();
// 	ResetKdDebuggerEnabled();
// 	ULONG uAddr = 0x83f7ad24;
// 	DisableWP();
// 	*(PULONG)uAddr = 1;
// 	EnableWP();
	for (int i = 0; i < 5; i++)
	{
		DisableWP();
		*(PULONG)g_uKdDebuggerEnabledHookAddrs[i].uAddr = g_uKdDebuggerEnabledHookAddrs[i].uVariableAddr;
		EnableWP();
	}
}

#endif