#include "MyDriverSys.h"
#include "Hook_NtOpenProcess.h"
#include "Hook_NtOpenThread.h"
#include "Hook_NtReadVirtualMemory.h"
#include "Hook_NtWriteVirtualMemory.h"
#include "Hook_NtProtectVirtualMemory.h"
#include "Hook_KeAttachProcess.h"
#include "Hook_KeStackAttachProcess.h"
#include "Hook_KdDisableDebug.h"


#pragma INITCODE
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,PUNICODE_STRING B)
{
	KdPrint(("开始测试----\n"));
	MoveVariable_Win7();
	Hook();

	//注册派遣函数
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchRoutine_CONTROLE;
	pDriverObject->MajorFunction[IRP_MJ_READ] = DispatchRoutine_CONTROLE;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = DispatchRoutine_CONTROLE;	
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchRoutine_CONTROLE;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchRoutine_CONTROLE;

	CreateMyDevice(pDriverObject);
	pDriverObject->DriverUnload=Driver_Unload;

	return STATUS_SUCCESS;
}

VOID Hook()
{	
	HookNtOpenProcess_Win7();
	HookNtOpenThread_Win7();
	HookNtProtectVirtualMemory();
	HookNtReadVirtualMemory();
	HookNtWriteVirtualMemory();
	HookKeAttachProcess();
	HookKeStackAttachProcess();	//--error
}

#pragma PAGECODE
NTSTATUS DispatchRoutine_CONTROLE(IN PDEVICE_OBJECT pDriver, IN PIRP pIrp)
{
	ULONG info;
	//得到当前栈指针
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	//区分IRP
	ULONG mf = stack->MajorFunction;
	switch (mf)
	{
	case IRP_MJ_DEVICE_CONTROL:
		{
			NTSTATUS status = STATUS_SUCCESS;
			//得到IOCTL码
			ULONG code = stack->Parameters.DeviceIoControl.IoControlCode;
			switch (code)
			{
			case hook_code:
				{
					HookKeAttachProcess();
					HookKeStackAttachProcess();
					//设置实际缓冲区大小
					info = 4;
				}				
				break;
			case unhook_code:
				{
					UnHookKeAttachProcess();
					UnHookKeStackAttachProcess();
					//设置实际缓冲区大小
					info = 4;
				}
				break;
			default:
				break;
			}
		}
		break;
	case IRP_MJ_CREATE:
		{
			break;
		}
	case IRP_MJ_CLOSE:
		{			
			break;
		}
	case IRP_MJ_READ:
		{
			break;
		}
	case IRP_MJ_WRITE:
		{
			break;
		}
	default:
		break;
	}
	pIrp->IoStatus.Information = info;
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	KdPrint(("离开派遣函数\n"));
	return STATUS_SUCCESS;
}

//创建设备
NTSTATUS CreateMyDevice(IN PDRIVER_OBJECT pDriverObject)
{
	NTSTATUS status;
	//返回创建
	PDEVICE_OBJECT pDevobj;

	UNICODE_STRING devName;
	UNICODE_STRING symLinkName;
	//对devName初始化
	RtlInitUnicodeString(&devName,L"\\Device\\Boris_Device");

	//创建设备
	status = IoCreateDevice(pDriverObject,0,&devName,FILE_DEVICE_UNKNOWN,0,TRUE,&pDevobj);
	if (!NT_SUCCESS(status))
	{
		if (status == STATUS_INSUFFICIENT_RESOURCES)
		{
			KdPrint(("资源不足\n"));
		}
		else if (status == STATUS_OBJECT_NAME_EXISTS)
		{
			KdPrint(("设备名字已存在\n"));
		}
		else if (status == STATUS_OBJECT_NAME_COLLISION)
		{
			KdPrint(("对象名有冲突\n"));
		}
		return status;
	}

	pDevobj->Flags |= DO_BUFFERED_IO;

	//创建符号链接
	RtlInitUnicodeString(&symLinkName,L"\\??\\MyDriverLinkName");
	status = IoCreateSymbolicLink(&symLinkName,&devName);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDevobj);
		return status;
	}
	return STATUS_SUCCESS;
}

#pragma PAGECODE
VOID Driver_Unload(IN PDRIVER_OBJECT pDriverObject)
{
	PDEVICE_OBJECT pDev;
	UNICODE_STRING symName;

	UnHook();	
	//删除符号链接
	RtlInitUnicodeString(&symName,L"\\??\\MyDriverLinkName");
	IoDeleteSymbolicLink(&symName);
	//删除设备
	pDev = pDriverObject->DeviceObject;
	IoDeleteDevice(pDev);

	KdPrint(("驱动成功被卸载...OK-----------\n"));
}

VOID UnHook()
{	
	UnHookNtOpenProcess_Win7();
	UnHookNtOpenThread_Win7();
	UnHookNtProtectVirtualMemory();
	UnHookNtReadVirtualMemory();
	UnHookNtWriteVirtualMemory();
	UnHookKeAttachProcess();
	UnHookKeStackAttachProcess();
}