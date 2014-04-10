#ifndef __MYDRIVERSYS_H__
#define __MYDRIVERSYS_H__

#include "DriverHead.h"

NTSTATUS CreateMyDevice(IN PDRIVER_OBJECT pDriverObject);
VOID Driver_Unload(IN PDRIVER_OBJECT pDriverObject);
VOID Hook();
VOID UnHook();
#endif