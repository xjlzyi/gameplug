#ifndef __HOOK_NTREADVIRTUALMEMORY_H__
#define __HOOK_NTREADVIRTUALMEMORY_H__

#include "GlobalFunction.h"
ULONG g_OriginNtReadVirtualMemoryAddr;
ULONG g_NtReadPushAddr;
//HOOK的起始地址
ULONG g_MyHookNtReadVirtualMemoryAddr;

#pragma PAGECODE
__declspec(naked) VOID __stdcall MyNtReadVirtualMemory()
{
// 	if (CheckProcessName("DNF.exe") || CheckProcessName("TenSafe_1.exe") || CheckProcessName("Client.exe"))
// 	{
// 		__asm
// 		{
// 			jmp g_OriginNtReadVirtualMemoryAddr
// 		}
// 	}
	_asm
	{
		push 0x18
		push g_NtReadPushAddr
		jmp g_MyHookNtReadVirtualMemoryAddr
	}
}

#pragma PAGECODE
VOID HookNtReadVirtualMemory()
{
// 	nt!NtReadVirtualMemory:  SSDT 277=0x115*4=0x454---win7

// 	mov eax, A040DF8C
// 	jmp eax
// 	call 8449CBA8

// 	8407382c 6a18            push    18h
// 	8407382e 68289ae683      push    offset nt! ?? ::FNODOBFM::`string'+0x3ea8 (83e69a28)
// 	84073833 e870e3e1ff      call    nt!_SEH_prolog4 (83e91ba8)
	//HOOK的起始地址
	g_OriginNtReadVirtualMemoryAddr = *((ULONG*)((ULONG)KeServiceDescriptorTable->ServiceTableBase+0x454));
	KdPrint(("NtReadVirtualMemory=%x\n",g_OriginNtReadVirtualMemoryAddr));
	//NtRead第二个push的地址
	g_NtReadPushAddr = *((ULONG*)(g_OriginNtReadVirtualMemoryAddr+3));
	KdPrint(("NtRead第二个push的地址=%x\n",g_NtReadPushAddr));
	//跳转地址
	g_MyHookNtReadVirtualMemoryAddr = g_OriginNtReadVirtualMemoryAddr + 7;
	KdPrint(("NtReadVirtualMemory中Call的地址=%x\n",g_MyHookNtReadVirtualMemoryAddr));
	WPON();
	__asm
	{
		mov eax,KeServiceDescriptorTable
		mov eax,[eax]
		add eax,0x454
		lea ebx,MyNtReadVirtualMemory
		mov [eax],ebx
	}
	WPOFF();
}

#pragma PAGECODE
VOID UnHookNtReadVirtualMemory()
{
	WPON();
	__asm
	{
		mov eax,KeServiceDescriptorTable
		mov eax,[eax]
		add eax,0x454
		mov ebx,g_OriginNtReadVirtualMemoryAddr
		mov [eax],ebx
	}
	WPOFF();
}

#endif