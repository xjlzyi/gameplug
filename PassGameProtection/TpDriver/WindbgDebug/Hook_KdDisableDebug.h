#ifndef __HOOK_KDDISABLEDEBUG_H__
#define __HOOK_KDDISABLEDEBUG_H__

#include "..\GlobalFunction.h"

DWORD TesSafeBase=0;
BYTE _pMirror[1282048]={0};
DWORD pMirror=(DWORD)_pMirror;
BYTE mKdDisableDebugger[5]={0};
DWORD kdRet=0;

// typedef PMDL (__stdcall *_MyIoAllocateMdl)(
// 	__in_opt PVOID  VirtualAddress,
// 	__in ULONG  Length,
// 	__in BOOLEAN  SecondaryBuffer,
// 	__in BOOLEAN  ChargeQuota,
// 	__inout_opt PIRP  Irp  OPTIONAL);
// 
// _MyIoAllocateMdl old_IoAllocateMdl;
// 
// PULONG pKdEnteredDebugger;
// 
// PMDL MyIoAllocateMdl(
// 	__in_opt PVOID  VirtualAddress,
// 	__in ULONG  Length,
// 	__in BOOLEAN  SecondaryBuffer,
// 	__in BOOLEAN  ChargeQuota,
// 	__inout_opt PIRP  Irp  OPTIONAL)
// {
// 	if (pKdEnteredDebugger == VirtualAddress)
// 	{
// 		 VirtualAddress = pKdEnteredDebugger + 0x20;  //+0x20  是让他读到其他的位置
// 	}
// 	return old_IoAllocateMdl(VirtualAddress,Length,SecondaryBuffer,ChargeQuota,Irp);
// }

__declspec(naked) void MyKdDisableDebugger()
{
	__asm
	{
		push eax
		mov eax,[esp+0x4]
		sub eax,TesSafeBase
		add eax,pMirror
		mov [esp+0x4],eax
		pop eax
		jmp kdRet
	}
}

VOID LoadImageRoutine(
	__in_opt PUNICODE_STRING  FullImageName,
	__in HANDLE  ProcessId,
	__in PIMAGE_INFO  ImageInfo)
{
	if(wcsstr(FullImageName->Buffer, L"TesSafe.sys")!=0)
	{
		KdPrint(("TesSafe has been discovered"));
		ULONG uImageInfo=(ULONG)ImageInfo->ImageBase;
		KdPrint(("TesSafeBase is %x",uImageInfo));
		TesSafeBase=uImageInfo;

		PMDL pMdl;
		pMdl = IoAllocateMdl(_pMirror,0x139000,FALSE,FALSE,NULL);
		MmProtectMdlSystemAddress(pMdl,PAGE_EXECUTE_READWRITE);
		IoFreeMdl(pMdl);
		//NtAllocateVirtualMemory((HANDLE)-1,pMirror,0,(PSIZE_T)0x139000,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
		UnInLineHookEngine((int)_pMirror,(char*)TesSafeBase,0x139000);

		//PassGlobalStaticCrcEx();
		kdRet=(DWORD)KdDisableDebugger+0x7;
		UnInLineHookEngine((int)mKdDisableDebugger,(char*)KdDisableDebugger,5);
		InLineHookEngine((ULONG)KdDisableDebugger,(int)MyKdDisableDebugger);
		KdPrint(("MirrorBase is %x",(int)pMirror));

		if(*(PUCHAR)(pMirror+0x92c1)==0x75)
		{
			*(PUSHORT)(pMirror+0x92c1)=0x9090;
			KdPrint(("TesSafeOffset1 is %x",pMirror+0x92c1));
		}
		if(*(PUCHAR)(pMirror+0x92c1+0x12e)==0x74)
		{
			*(PUCHAR)(pMirror+0x92c1+0x12e)=0xeb;
			KdPrint(("TesSafeOffset2 is %x",pMirror+0x92c1+0x12e));
		}
	}
}

#pragma PAGECODE
VOID HookKdDisableDebugger()
{
// 	DisableWP();
// 	KIRQL kIrql = KeRaiseIrqlToDpcLevel();
// 
// 	KeLowerIrql(kIrql);
// 	EnableWP();
	PsSetLoadImageNotifyRoutine(LoadImageRoutine);
}

#pragma PAGECODE
VOID UnHookKdDisableDebugger()
{
// 	DisableWP();
// 
// 	EnableWP();
	PsRemoveLoadImageNotifyRoutine(LoadImageRoutine);
}

#endif