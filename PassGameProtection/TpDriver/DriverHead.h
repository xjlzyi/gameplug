#ifndef __DRIVER_HEAD_H__
#define __DRIVER_HEAD_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <NTDDK.h> //这里包含需要用C方式编译的头文件
#ifdef __cplusplus
}
#endif 

#include <windef.h>



#define INITCODE code_seg("INIT") 
#define PAGECODE code_seg("PAGE")/*内存不足时，可以置换到硬盘*/

#define hook_tp CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define unhook_tp CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define hook_windbg CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define unhook_windbg CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _ServiceDescriptorTable 
{
	PVOID ServiceTableBase;		//System Service Dispatch Table 的基地址  
	PVOID ServiceCounterTable;	//包含着 SSDT 中每个服务被调用次数的计数器。这个计数器一般由sysenter 更新。 
	unsigned int NumberOfServices;//由 ServiceTableBase 描述的服务的数目。  
	PVOID ParamTableBase;		//包含每个系统服务参数字节数表的基地址-系统服务参数表 
}*PServiceDescriptorTable; 
extern "C" extern PServiceDescriptorTable KeServiceDescriptorTable;

typedef struct _OldVariable
{
	ULONG uAddr;
	ULONG uVariableAddr;
}HookVariableAddr;

typedef enum _SYSTEM_INFORMATION_CLASS {  
	SystemBasicInformation,  
	SystemProcessorInformation,  
	SystemPerformanceInformation,  
	SystemTimeOfDayInformation,  
	SystemPathInformation,  
	SystemProcessInformation, //5  
	SystemCallCountInformation,  
	SystemDeviceInformation,  
	SystemProcessorPerformanceInformation,  
	SystemFlagsInformation,  
	SystemCallTimeInformation,  
	SystemModuleInformation,  
	SystemLocksInformation,  
	SystemStackTraceInformation,  
	SystemPagedPoolInformation,  
	SystemNonPagedPoolInformation,  
	SystemHandleInformation,  
	SystemObjectInformation,  
	SystemPageFileInformation,  
	SystemVdmInstemulInformation,  
	SystemVdmBopInformation,  
	SystemFileCacheInformation,  
	SystemPoolTagInformation,  
	SystemInterruptInformation,  
	SystemDpcBehaviorInformation,  
	SystemFullMemoryInformation,  
	SystemLoadGdiDriverInformation,  
	SystemUnloadGdiDriverInformation,  
	SystemTimeAdjustmentInformation,  
	SystemSummaryMemoryInformation,  
	SystemNextEventIdInformation,  
	SystemEventIdsInformation,  
	SystemCrashDumpInformation,  
	SystemExceptionInformation,  
	SystemCrashDumpStateInformation,  
	SystemKernelDebuggerInformation,  
	SystemContextSwitchInformation,  
	SystemRegistryQuotaInformation,  
	SystemExtendServiceTableInformation,  
	SystemPrioritySeperation,  
	SystemPlugPlayBusInformation,  
	SystemDockInformation,  
	SystemPowerInformation2,  
	SystemProcessorSpeedInformation,  
	SystemCurrentTimeZoneInformation,  
	SystemLookasideInformation  
} SYSTEM_INFORMATION_CLASS, *PSYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG Reserved[2];
	PVOID Base;
	ULONG Size;
	ULONG Flags;
	USHORT Index;
	USHORT Unknown;
	USHORT LoadCount;
	USHORT ModuleNameOffset;
	CHAR ImageName[256];
}SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

extern "C"  NTSYSAPI NTSTATUS NTAPI ZwQuerySystemInformation(   
	IN ULONG SystemInformationClass,   
	IN PVOID SystemInformation,   
	IN ULONG SystemInformationLength,   
	OUT PULONG ReturnLength);



#endif