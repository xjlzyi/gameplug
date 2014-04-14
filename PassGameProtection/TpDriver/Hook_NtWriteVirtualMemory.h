#ifndef __HOOK_NTWRITEVIRTUALMEMORY_H__
#define __HOOK_NTWRITEVIRTUALMEMORY_H__

#include "GlobalFunction.h"

ULONG g_OriginNtWriteVirtualMemoryAddr;
//NtWriteVirtualMemory中Call的地址
ULONG g_NtWriteVirtualMemory;
//NtWrite第二个push的地址
ULONG g_NtWritePush;
#pragma PAGECODE
__declspec(naked) VOID __stdcall MyNtWriteVirtualMemory()
{
// 	if (CheckProcessName("DNF.exe") || CheckProcessName("TenSafe_1.exe") || CheckProcessName("Client.exe"))
// 	{
// 		__asm
// 		{
// 			jmp g_OriginNtWriteVirtualMemoryAddr
// 		}
// 	}
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
// 	nt!NtWriteVirtualMemory:  SSDT 399=0x18F*4=0x63C---win7

// 	0x8467E71C    |    B8 56E040A0    |    mov eax, A040E056    |        
// 	0x8467E721    |    FFE0    |    jmp eax    |        
// 	0x8467E723    |    E8 80E4E1FF    |    call 8449CBA8    |        

// 8407371c 6a18            push    18h
// 8407371e 68009ae683      push    offset nt! ?? ::FNODOBFM::`string'+0x3e80 (83e69a00)
// 84073723 e880e4e1ff      call    nt!_SEH_prolog4 (83e91ba8)
	g_OriginNtWriteVirtualMemoryAddr = *((ULONG*)((ULONG)KeServiceDescriptorTable->ServiceTableBase+0x63C));
	KdPrint(("NtWriteVirtualMemory=%x\n",g_OriginNtWriteVirtualMemoryAddr));
	g_NtWriteVirtualMemory = g_OriginNtWriteVirtualMemoryAddr + 7;
	KdPrint(("NtWriteVirtualMemory中Call的地址=%x\n",g_NtWriteVirtualMemory));
	g_NtWritePush = *((ULONG*)(g_OriginNtWriteVirtualMemoryAddr+3));
	KdPrint(("NtWrite第二个push的地址=%x\n",g_NtWritePush));
	WPON();
	__asm
	{
		mov eax,KeServiceDescriptorTable
		mov eax,[eax]
		add eax,0x63C
		lea ebx,MyNtWriteVirtualMemory
		mov [eax],ebx
	}
	WPOFF();
}

#pragma PAGECODE
VOID UnHookNtWriteVirtualMemory()
{
	WPON();
	__asm
	{
		mov eax,KeServiceDescriptorTable
		mov eax,[eax]
		add eax,0x63C
		mov ebx,g_OriginNtWriteVirtualMemoryAddr
		mov [eax],ebx
	}
	WPOFF();
}
#endif