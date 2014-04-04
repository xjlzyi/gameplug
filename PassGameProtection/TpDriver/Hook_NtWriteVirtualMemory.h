#ifndef __HOOK_NTWRITEVIRTUALMEMORY_H__
#define __HOOK_NTWRITEVIRTUALMEMORY_H__

#include "GlobalFunction.h"

__declspec(naked) NTSTATUS __stdcall MyNtWriteVirtualMemory(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress,
	IN PVOID Buffer,
	IN ULONG NumberOfBytesToWrite,
	OUT PULONG NumberOfBytesWritten OPTIONAL )
{
	_asm
	{
		retn 0x14
	}
}

VOID HookNtWriteVirtualMemory()
{

}

VOID UnHookNtWriteVirtualMemory()
{

}


#endif