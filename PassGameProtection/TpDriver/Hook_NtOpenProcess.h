#ifndef __HOOK_NTOPENPROCESS_H__
#define __HOOK_NTOPENPROCESS_H__

#include "PageProtect.h"

__declspec(naked) NTSTATUS __stdcall MyNtOpenProcess(
	PHANDLE ProcessHandle,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PCLIENT_ID ClientId) 
{
	_asm
	{
		retn 0x14
	}
}

VOID HookNtOpenProcess()
{


	RemovePageProtect();
	//INLINE HOOK

	ResetPageProtect();
}

VOID UnHookNtOpenProcess()
{
	RemovePageProtect();
	

	ResetPageProtect();
}

#endif