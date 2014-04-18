#ifndef __MYDRIVERSYS_H__
#define __MYDRIVERSYS_H__

#include "DriverHead.h"

NTSTATUS CreateMyDevice(IN PDRIVER_OBJECT pDriverObject);
NTSTATUS DispatchRoutine_CONTROLE(IN PDEVICE_OBJECT pDriver, IN PIRP pIrp);
VOID Driver_Unload(IN PDRIVER_OBJECT pDriverObject);
VOID Hook();
VOID UnHook();
VOID HookDebug();
VOID UnHookDebug();
#endif