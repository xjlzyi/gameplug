#ifndef __HOOK_NTREADVIRTUALMEMORY_H__
#define __HOOK_NTREADVIRTUALMEMORY_H__

#include "GlobalFunction.h"

ULONG g_NtReadVirtualMemory;

#pragma PAGECODE
__declspec(naked) NTSTATUS __stdcall MyNtReadVirtualMemory(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress,
	OUT PVOID Buffer,
	IN ULONG NumberOfBytesToRead,
	OUT PULONG NumberOfBytesReaded OPTIONAL)
{
	_asm
	{
		push 0x18
		push 0x83e97a28
		jmp g_NtReadVirtualMemory
	}
}

#pragma PAGECODE
VOID HookNtReadVirtualMemory()
{
// 	nt!NtReadVirtualMemory:  SSDT 399=0x63C
// 	840a182c 6a18            push    18h
// 	840a182e 68287ae983      push    offset nt! ?? ::FNODOBFM::`string'+0x3ea8 (83e97a28)
// 	840a1833 e870e3e1ff      call    nt!_SEH_prolog4 (83ebfba8)

	g_NtReadVirtualMemory = GetServiceOldAddr(L"NtReadVirtualMemory") + 7;
	WPON();
	__asm
	{
		mov eax,KeServiceDescriptorTable
		add eax,0x63c
		lea ebx,MyNtReadVirtualMemory
		mov [eax],ebx
	}

	WPOFF();
}

#pragma PAGECODE
VOID UnHookNtReadVirtualMemory()
{

}

#endif