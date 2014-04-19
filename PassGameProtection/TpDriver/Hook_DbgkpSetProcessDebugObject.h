#ifndef __HOOK_NTDEBUGACTIVEPROCESS_H__
#define __HOOK_NTDEBUGACTIVEPROCESS_H__

#include "GlobalFunction.h"

//////////////////////////////////////////////////////////////////////////
//NtDebugActiveProcess----SSDT 96=0x60*4=0x180
//////////////////////////////////////////////////////////////////////////

ULONG g_uOriginNtDebugActiveProcessAddr;

#pragma PAGECODE
__declspec(naked) VOID __stdcall MyDbgkpSetProcessDebugObject()
{
	__asm
	{
// 		mov	edi,edi
// 		push ebp
// 		mov ebp,esp
// 		and esp,0FFFFFFF8h
// 		sub esp,1Ch
	}
}

#pragma PAGECODE
VOID HookDbgkpSetProcessDebugObject()
{
// 	840f2c1a ff75f0          push    dword ptr [ebp-10h]
// 	840f2c1d ff75f8          push    dword ptr [ebp-8]
// 	840f2c20 ff75fc          push    dword ptr [ebp-4]
// 	840f2c23 e884f6ffff      call    nt!DbgkpSetProcessDebugObject (840f22ac)
	//NtDebugActiveProcess函数地址
	g_uOriginNtDebugActiveProcessAddr = *((ULONG*)((ULONG)KeServiceDescriptorTable->ServiceTableBase+0x180));
	char pCode[] = {(char)0xff,(char)0x75,(char)0xf0,(char)0xff,(char)0x75,
					(char)0xf8,(char)0xff,(char)0x75,(char)0xfc,(char)0xe8};
	//DbgkpSetProcessDebugObject函数地址
	ULONG uDbgkpSetProcessDebugObjectAddr = SearchCode(g_uOriginNtDebugActiveProcessAddr, pCode, sizeof(pCode));
	uDbgkpSetProcessDebugObjectAddr = GetCallAddr(uDbgkpSetProcessDebugObjectAddr);
	KdPrint(("DbgkpSetProcessDebugObjectAddr=%x\n",uDbgkpSetProcessDebugObjectAddr));
// 	nt!DbgkpSetProcessDebugObject:
// 	840ba2ac 8bff            mov     edi,edi
// 	840ba2ae 55              push    ebp
// 	840ba2af 8bec            mov     ebp,esp
// 	840ba2b1 83e4f8          and     esp,0FFFFFFF8h
// 	840ba2b4 83ec1c          sub     esp,1Ch
	//需要看下HOOK的地方
	DisableWP();
	__asm
	{

	}
	EnableWP();
}

#pragma PAGECODE
VOID UnHookDbgkpSetProcessDebugObject()
{
	DisableWP();
	__asm
	{

	}
	EnableWP();
}

#endif