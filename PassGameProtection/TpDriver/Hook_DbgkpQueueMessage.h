#ifndef __HOOK_NTDBGKPPOSTFAKEPROCESSCREATEMESSAGE_H__
#define __HOOK_NTDBGKPPOSTFAKEPROCESSCREATEMESSAGE_H__

#include "GlobalFunction.h"

// 1.nt!DbgkpPostFakeProcessCreateMessages+0x22->nt!DbgkpPostFakeThreadMessages+0x15c->nt!DbgkpQueueMessage
// 2.nt!NtDebugActiveProcess+0xe4->nt!DbgkpPostFakeProcessCreateMessages+0x22->nt!DbgkpPostFakeThreadMessages+0x29d->nt!DbgkSendSystemDllMessages+0x1dc->nt!DbgkpQueueMessage
// 3.nt!DbgkpPostFakeProcessCreateMessages+0x42->nt!DbgkpPostModuleMessages+0x13b->nt!DbgkpQueueMessage
// 4.nt!KiThreadStartup+0x19->nt!PspUserThreadStartup+0xf1->nt!DbgkCreateThread+0x363->nt!DbgkpSendApiMessage+0x44->nt!DbgkpQueueMessage

ULONG g_uDbgkpPostFakeProcessCreateMsgsAddr;
ULONG g_uDbgkpPostFakeThreadMsgsAddr;

#pragma PAGECODE
__declspec(naked) VOID __stdcall MykDbgkpQueueMessage()
{

}

#pragma region DbgkpPostFakeThreadMessages
#pragma PAGECODE
VOID HookDbgkpPostFakeThreadMessages()
{
	// 	840f2c0e 50              push    eax
	// 	840f2c0f ff75f8          push    dword ptr [ebp-8]
	// 	840f2c12 ff75fc          push    dword ptr [ebp-4]
	// 	840f2c15 e8a9feffff      call    nt!DbgkpPostFakeProcessCreateMessages (840f2ac3)
	char pCode[] = {(char)0x50,(char)0xff,(char)0x75,(char)0xf8,
		(char)0xff,(char)0x75,(char)0xfc,(char)0xe8};
	g_uDbgkpPostFakeProcessCreateMsgsAddr = SearchCode(g_uOriginNtDebugActiveProcessAddr, pCode, sizeof(pCode));
	g_uDbgkpPostFakeProcessCreateMsgsAddr = GetCallAddr(g_uDbgkpPostFakeProcessCreateMsgsAddr);
	KdPrint(("DbgkpPostFakeProcessCreateMessages=%x\n",g_uDbgkpPostFakeProcessCreateMsgsAddr));

	// 	840f2add ff7508          push    dword ptr [ebp+8]
	// 	840f2ae0 e84efbffff      call    nt!DbgkpPostFakeThreadMessages (840f2633)
	char pCode2[] = {(char)0xff,(char)0x75,(char)0x08,(char)0xe8};
	g_uDbgkpPostFakeThreadMsgsAddr = SearchCode(g_uDbgkpPostFakeProcessCreateMsgsAddr, pCode2, sizeof(pCode2));
	g_uDbgkpPostFakeThreadMsgsAddr = GetCallAddr(g_uDbgkpPostFakeThreadMsgsAddr);
	KdPrint(("DbgkpPostFakeThreadMessages=%x\n",g_uDbgkpPostFakeThreadMsgsAddr));

	// 	840f2787 50              push    eax
	// 	840f2788 57              push    edi
	// 	840f2789 56              push    esi
	// 	840f278a e84bf6ffff      call    nt!DbgkpQueueMessage (840f1dda)
	char pCode3[] = {(char)0x50,(char)0x57,(char)0x56,(char)0xe8};
	ULONG uDbgkpQueueMessage = SearchCode(g_uDbgkpPostFakeThreadMsgsAddr, pCode3, sizeof(pCode3));
	uDbgkpQueueMessage = GetCallAddr(uDbgkpQueueMessage);
	KdPrint(("DbgkpQueueMessage=%x\n",uDbgkpQueueMessage));
	// 	nt!DbgkpQueueMessage:
	// 	840f1dda 8bff            mov     edi,edi
	// 	840f1ddc 55              push    ebp
	// 	840f1ddd 8bec            mov     ebp,esp
	// 	840f1ddf 83e4f8          and     esp,0FFFFFFF8h
	// 	840f1de2 81ecf4000000    sub     esp,0F4h
	//根据HOOK的地址获取跳转地址
	DisableWP();

	EnableWP();
}

#pragma PAGECODE
VOID UnHookDbgkpPostFakeThreadMessages()
{
	DisableWP();

	EnableWP();
}
#pragma endregion

#pragma region DbgkSendSystemDllMessages
#pragma PAGECODE
VOID HookDbgkSendSystemDllMessages()
{
// 	40f28c6 50              push    eax
// 	840f28c7 ff750c          push    dword ptr [ebp+0Ch]
// 	840f28ca 57              push    edi
// 	840f28cb e8cbf7ffff      call    nt!DbgkSendSystemDllMessages (840f209b)
	char pCode[] = {(char)0x50,(char)0xff,(char)0x75,(char)0x0c,(char)0x57,(char)0xe8};
	ULONG uDbgkSendSystemDllMessagesAddr = SearchCode(g_uDbgkpPostFakeThreadMsgsAddr,pCode,sizeof(pCode));
	uDbgkSendSystemDllMessagesAddr = GetCallAddr(uDbgkSendSystemDllMessagesAddr);
	KdPrint(("DbgkSendSystemDllMessages=%x\n",uDbgkSendSystemDllMessagesAddr));

// 	840f226c ff7508          push    dword ptr [ebp+8]
// 	840f226f ff75d4          push    dword ptr [ebp-2Ch]
// 	840f2272 e863fbffff      call    nt!DbgkpQueueMessage (840f1dda)
	char pCode2[] = {(char)0xff,(char)0x75,(char)0x08,(char)0xff,(char)0x75,(char)0xd4,(char)0xe8};
	ULONG uDbgkpQueueMessageAddr = SearchCode(uDbgkSendSystemDllMessagesAddr, pCode2, sizeof(pCode2));
	uDbgkpQueueMessageAddr = GetCallAddr(uDbgkpQueueMessageAddr);
	KdPrint(("DbgkpQueueMessage=%x\n",uDbgkpQueueMessageAddr));
	DisableWP();

	EnableWP();
}

#pragma PAGECODE
VOID UnHookDbgkSendSystemDllMessages()
{
	DisableWP();

	EnableWP();
}
#pragma endregion

#pragma region DbgkpPostModuleMessages
#pragma PAGECODE
VOID HookDbgkpPostModuleMessages()
{
// 	840f2af6 ff750c          push    dword ptr [ebp+0Ch]
// 	840f2af9 8b4d08          mov     ecx,dword ptr [ebp+8]
// 	840f2afc ff742404        push    dword ptr [esp+4]
// 	840f2b00 e81bfeffff      call    nt!DbgkpPostModuleMessages (840f2920)
	char pCode[] = {(char)0xff,(char)0x74,(char)0x24,(char)0x04,(char)0xe8};
	ULONG uDbgkpPostModuleMessagesAddr = SearchCode(g_uDbgkpPostFakeProcessCreateMsgsAddr,pCode,sizeof(pCode));
	uDbgkpPostModuleMessagesAddr = GetCallAddr(uDbgkpPostModuleMessagesAddr);
	KdPrint(("DbgkpPostModuleMessages=%x\n",uDbgkpPostModuleMessagesAddr));

// 	840f2a4c ff750c          push    dword ptr [ebp+0Ch]
// 	840f2a4f 6a02            push    2
// 	840f2a51 50              push    eax
// 	840f2a52 ff7508          push    dword ptr [ebp+8]
// 	840f2a55 57              push    edi
// 	840f2a56 e87ff3ffff      call    nt!DbgkpQueueMessage (840f1dda)
	char pCode2[] = {(char)0xff,(char)0x75,(char)0x08,(char)0x57,(char)0xe8};
	ULONG uDbgkpQueueMessageAddr = SearchCode(uDbgkpPostModuleMessagesAddr,pCode2,sizeof(pCode2));
	uDbgkpQueueMessageAddr = GetCallAddr(uDbgkpQueueMessageAddr);
	KdPrint(("DbgkpQueueMessage=%x\n",uDbgkpQueueMessageAddr));

	DisableWP();

	EnableWP();
}

#pragma PAGECODE
VOID UnHookDbgkpPostModuleMessages()
{
	DisableWP();

	EnableWP();
}
#pragma endregion

#pragma region DbgkpSendApiMessage
#pragma PAGECODE
VOID HookDbgkpSendApiMessage()
{
// 	nt!DbgkpSendApiMessage:
//	840f354f 8bff            mov     edi,edi

// 	840f3589 8b4050          mov     eax,dword ptr [eax+50h]
// 	840f358c 51              push    ecx
// 	840f358d 50              push    eax
// 	840f358e e847e8ffff      call    nt!DbgkpQueueMessage (840f1dda)
	char pCode[] = {(char)0x8b,(char)0x40,(char)0x50,(char)0x51,(char)0x50,(char)0xe8};
	ULONG uDbgkpSendApiMessageAddr = SearchCode(g_uDbgkpPostFakeProcessCreateMsgsAddr, pCode, sizeof(pCode));
	uDbgkpSendApiMessageAddr = GetCallAddr(uDbgkpSendApiMessageAddr);
	KdPrint(("DbgkpQueueMessage=%x\n",uDbgkpSendApiMessageAddr));
}

#pragma PAGECODE
VOID UnHookDbgkpSendApiMessage()
{
	DisableWP();

	EnableWP();
}
#pragma endregion

#pragma PAGECODE
VOID HookDbgkpQueueMessage()
{
	//--------------------------DbgkpPostFakeThreadMessages调用了DbgkpQueueMessage
	HookDbgkpPostFakeThreadMessages();

	//--------------------------DbgkSendSystemDllMessages调用了DbgkpQueueMessage
	HookDbgkSendSystemDllMessages();

	//--------------------------DbgkpPostFakeModuleMessages调用了DbgkpQueueMessage
	HookDbgkpPostModuleMessages();

	//--------------------------DbgkpSendApiMessage调用了DbgkpQueueMessage
	HookDbgkpSendApiMessage();
}

#pragma PAGECODE
VOID UnHookDbgkpQueueMessage()
{
	UnHookDbgkpPostFakeThreadMessages();

	UnHookDbgkSendSystemDllMessages();

	UnHookDbgkpPostModuleMessages();

	UnHookDbgkpSendApiMessage();
}
#endif
