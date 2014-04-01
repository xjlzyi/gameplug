#ifndef __HOOK_NTREADVIRTUALMEMORY_H__
#define __HOOK_NTREADVIRTUALMEMORY_H__

#include "PageProtect.h"

__declspec(naked) NTSTATUS __stdcall MyNtReadVirtualMemory(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress,
	OUT PVOID Buffer,
	IN ULONG NumberOfBytesToRead,
	OUT PULONG NumberOfBytesReaded OPTIONAL)
{
	_asm
	{
		retn 0x14
	}
}

VOID HookNtReadVirtualMemory()
{

}

VOID UnHookNtReadVirtualMemory()
{

}

#endif