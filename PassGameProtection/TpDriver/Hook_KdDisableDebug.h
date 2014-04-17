#ifndef __HOOK_KDDISABLEDEBUG_H__
#define __HOOK_KDDISABLEDEBUG_H__

#include "GlobalFunction.h"

VOID LoadImageRoutine(__in_opt PUNICODE_STRING  FullImageName,
					__in HANDLE  ProcessId,
					__in PIMAGE_INFO  ImageInfo
					)
{

}


#pragma PAGECODE
VOID HookKdDisableDebugger()
{

	DisableWP();
	KIRQL kIrql = KeRaiseIrqlToDpcLevel();


	KeLowerIrql(kIrql);
	EnableWP();
	PsSetLoadImageNotifyRoutine(LoadImageRoutine);
}

#pragma PAGECODE
VOID UnHookKdDisableDebugger()
{
	DisableWP();

	EnableWP();
	PsRemoveLoadImageNotifyRoutine(LoadImageRoutine);
}

#endif