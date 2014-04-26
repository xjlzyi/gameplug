#ifndef __GLOBALFUNCTION_H__
#define __GLOBALFUNCTION_H__

#include "DriverHead.h"


VOID DisableWP()
{
	//去除保护
	_asm
	{
		cli
		mov eax,cr0
		and eax, not 10000h
		mov cr0,eax
	}
}

VOID EnableWP()
{
	//还原保护
	__asm
	{
		mov eax,cr0
		or eax,10000h
		mov cr0,eax
		sti
	}
}

//搜索特征码寻找地址
ULONG SearchCode(int StartingAddr,char* pCode,int Length)
{
	char Destination[256]="";
	int i=5000;
	while (i--)
	{
		RtlMoveMemory(Destination,(char*)StartingAddr,Length);
		if (RtlCompareMemory(pCode,Destination,Length)==Length)
		{
			return StartingAddr+Length;
		}
		StartingAddr++;
	}
	return 0;
}

//通过名称获得SSDT服务原始地址
ULONG GetServiceOldAddr(PCWSTR FunName)
{
	UNICODE_STRING _FunName;
	RtlInitUnicodeString(&_FunName,FunName);
	return (ULONG)MmGetSystemRoutineAddress(&_FunName);
}

//判断进程名称
extern"C" __declspec(dllimport) UCHAR* PsGetProcessImageFileName(IN PEPROCESS Process);
bool CheckProcessName(char* szName)
{
	//获取当前进程
	PEPROCESS nEProcess = PsGetCurrentProcess();
	//获取进程名称
	char* szProessaName = (char*)PsGetProcessImageFileName(nEProcess);
	//比较进程名
	if(strcmp(szProessaName,szName)==0)
	{
		KdPrint(("CheckProcessName ok"));
		return true;
	}

	return false;
}

//获取CALL后面的地址
ULONG GetCallAddr(ULONG nCallAddr)
{
	return (*((ULONG*)nCallAddr) + nCallAddr + 4);
}

//HOOK Call的地址
VOID CallHook(ULONG uFuncAddr, ULONG uCallAddr)
{
	ULONG nJmpAddr=uFuncAddr - uCallAddr - 4;
	DisableWP();
	__asm
	{
		mov eax,uCallAddr
		mov ebx,nJmpAddr
		mov dword ptr ds:[eax],ebx
	}
	EnableWP();
}

VOID InLineHookEngine(ULONG nRHookAddr,ULONG nMyFunctionAddr)
{
	int nJmpAddr=nMyFunctionAddr-nRHookAddr-5;
	DisableWP();
	__asm
	{
		mov eax,nRHookAddr
		mov byte ptr [eax],0xE9
		mov ebx,nJmpAddr	
		mov dword ptr [eax+1],ebx
	}
	EnableWP();
}

VOID UnInLineHookEngine(int nRHookAddr,char *szMacCode,int nLeng)
{
	DisableWP();

	RtlMoveMemory((char*)nRHookAddr,szMacCode,nLeng);

	EnableWP();
}

// typedef struct _SYSTEM_MODULE_INFORMATION {//Information Class 11 
// 	ULONG Reserved [2]; 
// 	PVOID Base; 
// 	ULONG Size; 
// 	ULONG Flags; 
// 	USHORT Index; 
// 	USHORT Unknown; 
// 	USHORT LoadCount; 
// 	USHORT ModuleNameOffset; 
// 	CHAR ImageName [256 ]; 
// } SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION; 
// 
// typedef struct { 
// 	ULONG  dwNumberOfModules; 
// 	SYSTEM_MODULE_INFORMATION  smi; 
// } MODULES, *PMODULES; 
// 
// typedef enum _SYSTEM_INFORMATION_CLASS     //    Q S
// {
// 	SystemBasicInformation,                // 00 Y N
// 	SystemProcessorInformation,            // 01 Y N
// 	SystemPerformanceInformation,          // 02 Y N
// 	SystemTimeOfDayInformation,            // 03 Y N
// 	SystemNotImplemented1,                 // 04 Y N
// 	SystemProcessesAndThreadsInformation,  // 05 Y N
// 	SystemCallCounts,                      // 06 Y N
// 	SystemConfigurationInformation,        // 07 Y N
// 	SystemProcessorTimes,                  // 08 Y N
// 	SystemGlobalFlag,                      // 09 Y Y
// 	SystemNotImplemented2,                 // 10 Y N
// 	SystemModuleInformation,               // 11 Y N
// 	SystemLockInformation,                 // 12 Y N
// 	SystemNotImplemented3,                 // 13 Y N
// 	SystemNotImplemented4,                 // 14 Y N
// 	SystemNotImplemented5,                 // 15 Y N
// 	SystemHandleInformation,               // 16 Y N
// 	SystemObjectInformation,               // 17 Y N
// 	SystemPagefileInformation,             // 18 Y N
// 	SystemInstructionEmulationCounts,      // 19 Y N
// 	SystemInvalidInfoClass1,               // 20
// 	SystemCacheInformation,                // 21 Y Y
// 	SystemPoolTagInformation,              // 22 Y N
// 	SystemProcessorStatistics,             // 23 Y N
// 	SystemDpcInformation,                  // 24 Y Y
// 	SystemNotImplemented6,                 // 25 Y N
// 	SystemLoadImage,                       // 26 N Y
// 	SystemUnloadImage,                     // 27 N Y
// 	SystemTimeAdjustment,                  // 28 Y Y
// 	SystemNotImplemented7,                 // 29 Y N
// 	SystemNotImplemented8,                 // 30 Y N
// 	SystemNotImplemented9,                 // 31 Y N
// 	SystemCrashDumpInformation,            // 32 Y N
// 	SystemExceptionInformation,            // 33 Y N
// 	SystemCrashDumpStateInformation,       // 34 Y Y/N
// 	SystemKernelDebuggerInformation,       // 35 Y N
// 	SystemContextSwitchInformation,        // 36 Y N
// 	SystemRegistryQuotaInformation,        // 37 Y Y
// 	SystemLoadAndCallImage,                // 38 N Y
// 	SystemPrioritySeparation,              // 39 N Y
// 	SystemNotImplemented10,                // 40 Y N
// 	SystemNotImplemented11,                // 41 Y N
// 	SystemInvalidInfoClass2,               // 42
// 	SystemInvalidInfoClass3,               // 43
// 	SystemTimeZoneInformation,             // 44 Y N
// 	SystemLookasideInformation,            // 45 Y N
// 	SystemSetTimeSlipEvent,                // 46 N Y
// 	SystemCreateSession,                   // 47 N Y
// 	SystemDeleteSession,                   // 48 N Y
// 	SystemInvalidInfoClass4,               // 49
// 	SystemRangeStartInformation,           // 50 Y N
// 	SystemVerifierInformation,             // 51 Y Y
// 	SystemAddVerifier,                     // 52 N Y
// 	SystemSessionProcessesInformation      // 53 Y N
// } SYSTEM_INFORMATION_CLASS;
// 
// typedef NTSTATUS ( __stdcall *ZWQUERYSYSTEMINFORMATION ) ( 
// 	IN SYSTEM_INFORMATION_CLASS SystemInformationClass, 
// 	IN OUT PVOID SystemInformation, 
// 	IN ULONG SystemInformationLength, 
// 	OUT PULONG ReturnLength OPTIONAL );
// static ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation = NULL;

ULONG GetFuncAddr(ULONG FuncFeature1, ULONG FuncFeature2, ULONG FuncFeature3, ULONG FuncFeature4) 
{      
	ULONG size;
	ULONG i;
	ULONG j;
	PULONG p;
	NTSTATUS ntStatus;
	PSYSTEM_MODULE_INFORMATION pSystemModel;
	ULONG ulModelAddrStart;
	ULONG ulModelAddrEnd;
	ZwQuerySystemInformation(SystemModuleInformation, &size, 0, &size);
	p = (PULONG)ExAllocatePool(NonPagedPool, size);
	if (p==NULL)
	{          
		return STATUS_SEVERITY_SUCCESS;
	}       
	ntStatus = ZwQuerySystemInformation( SystemModuleInformation, p, size * sizeof(p), 0);
	if (!NT_SUCCESS(ntStatus))
	{
		KdPrint(("ZwQuerySystemInformation Error!\n"));
		return NULL;
	}
	pSystemModel = (PSYSTEM_MODULE_INFORMATION)( p + 1 );
	for ( i = 0; i < *p; i++ )
	{
// 		KdPrint(( "SystemMode,Base: %x\n", pSystemModel[i].Base));
// 		KdPrint(( "SystemMode, ImageName,: %s\n", pSystemModel[i].ImageName));
// 		KdPrint(( "SystemMode, Size: %d\n", pSystemModel[i].Size));
		ulModelAddrStart = (ULONG)pSystemModel[i].Base;
		ulModelAddrEnd = (ULONG)pSystemModel[i].Base + pSystemModel[i].Size;
		for (j = ulModelAddrStart; j < ulModelAddrEnd; j++)
		{
			// 如果 j + 12 已经大于 最大值，那就已经没有希望找到特征值了T_T               
			if (j + 12 > ulModelAddrEnd)
			{
				break;
			}               
			if (*((ULONG *)j) == FuncFeature1 
				&& *((ULONG *)(j + 4)) == FuncFeature2 
				&& *((ULONG *)(j + 8)) == FuncFeature3 
				&& *((ULONG *)(j + 12)) == FuncFeature4)
			{
//				KdPrint(( "Find the Function Addr: %x!\n", j));
				return j;
			}
		}     
	}
	KdPrint(( "Can't Find the Function Addr!\n"));
	return 0; 
}

#endif