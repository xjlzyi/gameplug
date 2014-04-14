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

#define hook_code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define unhook_code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

typedef struct _ServiceDescriptorTable 
{
	PVOID ServiceTableBase;		//System Service Dispatch Table 的基地址  
	PVOID ServiceCounterTable;	//包含着 SSDT 中每个服务被调用次数的计数器。这个计数器一般由sysenter 更新。 
	unsigned int NumberOfServices;//由 ServiceTableBase 描述的服务的数目。  
	PVOID ParamTableBase;		//包含每个系统服务参数字节数表的基地址-系统服务参数表 
}*PServiceDescriptorTable; 
extern "C" extern PServiceDescriptorTable KeServiceDescriptorTable;

#endif