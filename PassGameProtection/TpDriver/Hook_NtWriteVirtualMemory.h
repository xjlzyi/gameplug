#ifndef __HOOK_NTWRITEVIRTUALMEMORY_H__
#define __HOOK_NTWRITEVIRTUALMEMORY_H__

#include "GlobalFunction.h"

//NtWriteVirtualMemoryÖÐCallµÄµØÖ·
ULONG g_NtWriteVirtualMemory;

ULONG g_NtWritePush;
#pragma PAGECODE
__declspec(naked) NTSTATUS __stdcall MyNtWriteVirtualMemory(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress,
	IN PVOID Buffer,
	IN ULONG NumberOfBytesToWrite,
	OUT PULONG NumberOfBytesWritten OPTIONAL )
{
	_asm
	{
		push 0x18
		push g_NtWritePush
		jmp g_NtWriteVirtualMemory
	}
}

#pragma PAGECODE
VOID HookNtWriteVirtualMemory()
{
// 	nt!NtWriteVirtualMemory:  SSDT 277=0x454
// 	840a171c 6a18            push    18h
// 	840a171e 68007ae983      push    offset nt! ?? ::FNODOBFM::`string'+0x3e80 (83e97a00)
// 	840a1723 e880e4e1ff      call    nt!_SEH_prolog4 (83ebfba8)

	g_NtWriteVirtualMemory = GetServiceOldAddr(L"NtWriteVirtualMemory") + 7;

	WPON();
	__asm
	{
		mov eax,KeServiceDescriptorTable
		mov eax,[eax]
		add eax,0x454
		lea ebx,MyNtWriteVirtualMemory
		mov [eax],ebx
	}
	WPOFF();

}

#pragma PAGECODE
VOID UnHookNtWriteVirtualMemory()
{

}


#endif