#ifndef __HOOK_KEATTACHPROCESS_H__
#define __HOOK_KEATTACHPROCESS_H__

#include "GlobalFunction.h"

#pragma PAGECODE
__declspec(naked) VOID __stdcall MyKeAttachProcess()
{

}

#pragma PAGECODE
VOID HookKeAttachProcess()
{
// 	83e62062 8bc6            mov     eax,esi
// 	83e62064 ff7508          push    dword ptr [ebp+8]
// 	83e62067 e867b80500      call    nt!KiAttachProcess (83ebd8d)


}

#pragma PAGECODE
VOID UnHookKeAttachProcess()
{

}

#endif