#ifndef __HOOK_KIDEBUGROUTINE_H__
#define __HOOK_KIDEBUGROUTINE_H__

#include "..\GlobalFunction.h"


ULONG g_uKdpStubAddr;
ULONG g_uKdpTrapAddr;
//KiDebugRoutine变量存储
ULONG g_uKiDebugRoutine=0;
//HOOK的KiDebugRoutine变量和地址
HookVariableAddr g_uKiDebugRoutineHookAddrs[3]={0};

//转移win7中的KiDebugRoutine变量
#pragma PAGECODE
VOID MoveKiDebugRutine_Win7()
{
	//得到KdDisableDebugger地址
	ULONG uKdDisableDebuggerAddr = GetServiceOldAddr(L"KdDisableDebugger");
	KdPrint(("KdDisableDebugger=%x\n",uKdDisableDebuggerAddr));
	if (uKdDisableDebuggerAddr == 0)
		return;
	char pCode = (char)0xe8;
	ULONG uKdDisableDebuggerWithLockAddr = SearchCode(uKdDisableDebuggerAddr,&pCode,1);
	uKdDisableDebuggerWithLockAddr = GetCallAddr(uKdDisableDebuggerWithLockAddr);

// 	83f2f7ee 881d2c7dfb83    mov     byte ptr [nt!KdDebuggerEnabled (83fb7d2c)],bl
// 	83f2f7f4 c705bcc9fb83aff9f283 mov dword ptr [nt!KiDebugRoutine (83fbc9bc)],offset nt!KdpStub (83f2f9af)
// 	83f2f7fe 881dd402dfff    mov     byte ptr ds:[0FFDF02D4h],bl
// 	83f2f804 ff0560b1f883    inc     dword ptr [nt!KdDisableCount (83f8b160)]

	char pCode2[] = {(char)0x88,(char)0x1d,(char)0xd4,(char)0x02,(char)0xdf,(char)0xff};
	int nLen = sizeof(pCode2);
	ULONG uTempAddr = SearchCode(uKdDisableDebuggerWithLockAddr,pCode2,nLen) - nLen;
	KdPrint(("TempAddr=%x\n",uTempAddr));
	if (uTempAddr == 0)
		return;
	//KiDebugRoutine地址
	g_uKiDebugRoutine = *((PULONG)(uTempAddr-8));
	KdPrint(("KiDebugRoutineAddr=%x\n",g_uKiDebugRoutine));
	//KdpTrap地址
	g_uKdpTrapAddr = *(PULONG)g_uKiDebugRoutine;
	KdPrint(("KdpTrap=%x\n",g_uKdpTrapAddr));
	//KdpStub地址
// 	g_uKdpStubAddr = *((PULONG)(uTempAddr - 4));
// 	KdPrint(("KdpStub=%x\n",g_uKdpStubAddr));
 
// 	DisableWP();
// 	*(PULONG)g_uKiDebugRoutine = g_uKdpStubAddr;
// 	EnableWP();
// 	KdPrint(("KiDebugRoutine值=%x\n",*(PULONG)g_uKiDebugRoutine));
// 	ULONG uAddr = GetServiceOldAddr(L"KdpTrap");
// 	//设置自定义的KiDebugRoutine变量
// 	g_uKiDebugRoutine = g_uKdpTrapAddr;
//	83ef8ee0 
//	//得到原内核KiDispatchException地址
// 	dd KiDispatchException
// 	83f07ee0  0000f868 xxxxxxxx xxxxxxxx 64fffcac
// 	83f07ef0  000020a1 8c80ff00 c7000005 0017e045
	ULONG uAddr=GetFuncAddr(0x64fffcac, 0x000020a1, 0x8c80ff00, 0xc7000005) - 0xc;
// 	83efc041 ff750c          push    dword ptr [ebp+0Ch]
// 	83efc044 ff7510          push    dword ptr [ebp+10h]
// 	83efc047 ff15bc19fb83    call    dword ptr [nt!KiDebugRoutine (83fb19bc)]
 	char pCode3[] = {(char)0xFF,(char)0x75,(char)0x0c,(char)0xFF,(char)0x75,(char)0x10,(char)0xFF};
 	KdPrint(("KiDispatchException地址=%x\n",uAddr));
	uAddr = SearchCode(uAddr,pCode3,sizeof(pCode3));
	KdPrint(("KiDebugRoutine第一个地址=%x\n",uAddr));
	g_uKiDebugRoutineHookAddrs[0].uAddr = uAddr;
	g_uKiDebugRoutineHookAddrs[0].uVariableAddr = *(PULONG)uAddr;
// 	DisableWP();
// 	*(PULONG)uAddr=(ULONG)&g_uKdpTrapAddr;
// 	EnableWP();

	uAddr = SearchCode(uAddr,pCode3,sizeof(pCode3));
	g_uKiDebugRoutineHookAddrs[1].uAddr = uAddr;
	g_uKiDebugRoutineHookAddrs[1].uVariableAddr = *(PULONG)uAddr;
	KdPrint(("KiDebugRoutine第二个地址=%x\n",uAddr));
// 	DisableWP();
// 	*(PULONG)uAddr=(ULONG)&g_uKdpTrapAddr;
// 	EnableWP();

	uAddr = SearchCode(uAddr,pCode3,sizeof(pCode3));
	KdPrint(("KiDebugRoutine第三个地址=%x\n",uAddr));
	g_uKiDebugRoutineHookAddrs[2].uAddr = uAddr;
	g_uKiDebugRoutineHookAddrs[2].uVariableAddr = *(PULONG)uAddr;
// 	DisableWP();
// 	*(PULONG)uAddr=(ULONG)&g_uKdpTrapAddr;
// 	EnableWP();
}

#pragma PAGECODE
VOID ResetKiDebugRutine_Win7()
{
// 	DisableWP();
// 	*(PULONG)g_uKiDebugRoutine = g_uKdpTrapAddr;
// 	EnableWP();

// 	for (int i = 0; i < 3; i++)
// 	{
// 		DisableWP();
// 		*(PULONG)g_uKiDebugRoutineHookAddrs[i].uAddr = g_uKiDebugRoutineHookAddrs[i].uVariableAddr;
// 		EnableWP();
// 	}
}

#endif