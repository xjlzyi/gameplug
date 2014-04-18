#ifndef __HOOK_KIDEBUGROUTINE_H__
#define __HOOK_KIDEBUGROUTINE_H__

#include "..\GlobalFunction.h"

//KiDebugRoutine变量存储
ULONG g_uKiDebugRoutine=0;
//HOOK的KiDebugRoutine变量和地址
HookVariableAddr g_uKiDebugRoutineHookAddrs[3]={0};

//转移win7中的KiDebugRoutine变量
#pragma PAGECODE
VOID MoveKiDebugRutine_Win7()
{
	//得到KdpTrap地址
	//////////////////////////////////////////////////////////////////////////
	//如何获取未导出函数地址
	//////////////////////////////////////////////////////////////////////////
// 	ULONG uAddr = GetServiceOldAddr(L"KdpTrap");------Error
// 	//设置自定义的KiDebugRoutine变量
// 	g_uKiDebugRoutine = uAddr;
// 
// 	//得到原内核KiDispatchException地址
// 	uAddr=GetServiceOldAddr(L"KiDispatchException");

// 	83efc041 ff750c          push    dword ptr [ebp+0Ch]
// 	83efc044 ff7510          push    dword ptr [ebp+10h]
// 	83efc047 ff15bc19fb83    call    dword ptr [nt!KiDebugRoutine (83fb19bc)]
// 	char pCode[] = {(char)0xFF,(char)0x75,(char)0x0c,(char)0xFF,(char)0x75,(char)0x10,(char)0xFF};
// 	KdPrint(("KiDispatchException地址=%x\n",uAddr));
// 	uAddr = SearchCode(uAddr,pCode,sizeof(pCode));
// 	KdPrint(("KiDebugRoutine第一个地址=%x\n",uAddr));
// 	g_uKiDebugRoutineHookAddrs[0].uAddr = uAddr;
// 	g_uKiDebugRoutineHookAddrs[0].uVariableAddr = *(PULONG)uAddr;
// 	DisableWP();
// 	*(PULONG)uAddr=(ULONG)&g_uKiDebugRoutine;
// 	EnableWP();

// 	uAddr = SearchCode(uAddr,pCode,sizeof(pCode));
// 	g_uKiDebugRoutineHookAddrs[0].uAddr = uAddr;
// 	g_uKiDebugRoutineHookAddrs[0].uVariableAddr = *(PULONG)uAddr;
//	KdPrint(("KiDebugRoutine第二个地址=%x\n",uAddr));
// 	DisableWP();
// 	*(PULONG)uAddr=(ULONG)&g_uKiDebugRoutine;
// 	EnableWP();
// 
// 	uAddr = SearchCode(uAddr,pCode,sizeof(pCode));
//	KdPrint(("KiDebugRoutine第三个地址=%x\n",uAddr));
// 	g_uKiDebugRoutineHookAddrs[0].uAddr = uAddr;
// 	g_uKiDebugRoutineHookAddrs[0].uVariableAddr = *(PULONG)uAddr;
// 	DisableWP();
// 	*(PULONG)uAddr=(ULONG)&g_uKiDebugRoutine;
// 	EnableWP();
}

#pragma PAGECODE
VOID ResetKiDebugRutine_Win7()
{
// 	for (int i = 0; i < 3; i++)
// 	{
// 		DisableWP();
// 		*(PULONG)g_uKiDebugRoutineHookAddrs[i].uAddr = g_uKiDebugRoutineHookAddrs[i].uVariableAddr;
// 		EnableWP();
// 	}
}

#endif