#include "MyDriverSys.h"
#include "Hook_NtOpenProcess.h"
#include "Hook_KiAttachProcess.h"
#include "Hook_NtReadVirtualMemory.h"
#include "Hook_NtWriteVirtualMemory.h"

#pragma INITCODE
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,PUNICODE_STRING B)
{
	KdPrint(("开始测试----"));
	HookNtOpenProcess_Win7();
	HookNtReadVirtualMemory();
	HookNtWriteVirtualMemory();

	CreateMyDevice(pDriverObject);
	pDriverObject->DriverUnload=Driver_Unload;

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

	UnHookNtOpenProcess_Win7();
	UnHookNtReadVirtualMemory();
	UnHookNtWriteVirtualMemory();
	
	//删除符号链接
	RtlInitUnicodeString(&symName,L"\\??\\MyDriverLinkName");
	IoDeleteSymbolicLink(&symName);
	//删除设备
	pDev = pDriverObject->DeviceObject;
	IoDeleteDevice(pDev);

	KdPrint(("驱动成功被卸载...OK-----------\n"));
}