#ifndef __HOOK_KEATTACHPROCESS_H__
#define __HOOK_KEATTACHPROCESS_H__

#include "GlobalFunction.h"

ULONG g_uHookKeAttachProcessAddr;
ULONG g_uRealKeAttachProcess;
// ULONG g_uKiAttachProcessAddr;
// ULONG g_uKiMoveApcState;
// ULONG g_uKiInSwapSingleProcess;
// ULONG g_uHvlLongSpinCountMask;
// ULONG g_uHvlEnlightenments;
// ULONG g_uHvlNotifyLongSpinWait;
// ULONG g_uKiSwapProcess;
// ULONG g_uImp_KfLowerIrql;

#pragma PAGECODE
__declspec(naked) NTSTATUS MyHookKeAttachProcess()
{
	__asm
	{
		mov    edi, edi
		push   ebp
		mov    ebp, esp
		push   ecx
		push   esi
		jmp g_uHookKeAttachProcessAddr
	}


}

#pragma PAGECODE
VOID HookKeAttachProcess()
{
	ULONG uOriginKeAttachProcess = GetServiceOldAddr(L"KeAttachProcess");
	KdPrint(("OriginKeAttachProcess=%x\n",uOriginKeAttachProcess));
	g_uRealKeAttachProcess = uOriginKeAttachProcess;
	KdPrint(("RealKeAttachProcess=%x\n",g_uRealKeAttachProcess));
	g_uHookKeAttachProcessAddr = uOriginKeAttachProcess + 7;
	KdPrint(("KeAttachProcess JmpAddr=%x\n",g_uHookKeAttachProcessAddr));
	
	DisableWP();
	KIRQL Irql=KeRaiseIrqlToDpcLevel();
	uOriginKeAttachProcess = (ULONG)MyHookKeAttachProcess;
	KeLowerIrql(Irql);
	EnableWP();
}

#pragma PAGECODE
VOID UnHookKeAttachProcess()
{
	ULONG uOriginKeAttachProcess = GetServiceOldAddr(L"KeAttachProcess");
	DisableWP();
	KIRQL Irql=KeRaiseIrqlToDpcLevel();
	uOriginKeAttachProcess = g_uRealKeAttachProcess;
	KeLowerIrql(Irql);
	EnableWP();
}

// 	if(CheckProcessName("DNF.exe")||CheckProcessName("TenSafe_1.exe"))
// 	{
// 		__asm
// 		{
// 			mov     edi,edi
// 			push    ebp
// 			mov     ebp,esp
// 			push    ebx
// 			mov     ebx,dword ptr [ebp+8]
// 			push    esi
// 			mov     esi,eax
// 			mov     eax,dword ptr [ebp+10h]
// 			push    edi
// 			lea     edi,[esi+40h]
// 			push    edi
// 			call    g_uKiMoveApcState //(84290959)  1
// 			mov     ecx,dword ptr [ebp+10h]
// 			mov     dword ptr [edi+4],edi
// 			mov     dword ptr [edi],edi
// 			lea     eax,[esi+48h]
// 			mov     dword ptr [eax+4],eax
// 			mov     dword ptr [eax],eax
// 			lea     eax,[esi+170h]
// 			mov     byte ptr [esi+54h],0
// 			mov     byte ptr [esi+55h],0
// 			mov     byte ptr [esi+56h],0
// 			cmp     ecx,eax
// 			jne     KiAttachProcess52// nt!KiAttachProcess+0x52 (842908c5)
// 
// KiAttachProcess3f:
// 			mov     dword ptr [esi+168h],eax
// 			mov     dword ptr [esi+16Ch],edi
// 			mov     byte ptr [esi+134h],1
// KiAttachProcess52:
// 			push    8
// 			pop     eax
// 			lea     edx,[ebx+74h]
// 			lock xadd dword ptr [edx],eax
// 			mov     dword ptr [ebp+8],eax
// 			mov     eax,dword ptr [ebp+8]
// 			test    al,7
// 			jne     KiAttachProcess76//KiAttachProcess+0x76 (842908e9)
// KiAttachProcess66:
// 			mov     dword ptr [esi+50h],ebx
// 			xor     eax,eax
// 			add     esi,60h
// 			lock and dword ptr [esi],eax
// 			push    dword ptr [ecx+10h]
// 			jmp     KiAttachProcesscb//nt!KiAttachProcess+0xcb (8429093e)
// 
// KiAttachProcess76:
// 			lea     edi,[esi+60h]
// 			xor     eax,eax
// 			mov     ecx,edi
// 			lock and dword ptr [ecx],eax
// 			mov     eax,ebx
// 			call    g_uKiInSwapSingleProcess        //2
// 			and     dword ptr [ebp+8],0
// 			jmp     KiAttachProcessb4//nt!KiAttachProcess+0xb4 (84290927)
// 
// KiAttachProcess8d:
// 			inc     dword ptr [ebp+8]
// 			mov     eax,dword ptr [ebp+8]
// 			test    dword ptr [g_uHvlLongSpinCountMask],eax//dword ptr [nt!HvlLongSpinCountMask (84376bc4)],eax   3
// 			jne     KiAttachProcessac//nt!KiAttachProcess+0xac (8429091f)
// 
// KiAttachProcess9b:
// 			test    byte ptr [g_uHvlEnlightenments],40h //byte ptr [nt!HvlEnlightenments (84376bbc)],40h   4
// 			je      KiAttachProcessac//nt!KiAttachProcess+0xac (8429091f)
// 
// KiAttachProcessa4:
// 			push    eax
// 			call    g_uHvlNotifyLongSpinWait              //5
// 			jmp     KiAttachProcessae//nt!KiAttachProcess+0xae (84290921)
// 
// KiAttachProcessac:
// 			pause
// 
// KiAttachProcessae:
// 			mov     eax,dword ptr [edi]
// 			test    eax,eax
// 			jne     KiAttachProcess8d //nt!KiAttachProcess+0x8d (84290900)
// 
// KiAttachProcessb4:
// 			xor     eax,eax
// 			mov     ecx,edi
// 			inc     eax
// 			xchg    eax,dword ptr [ecx]
// 			test    eax,eax
// 			jne     KiAttachProcess8d//nt!KiAttachProcess+0x8d (84290900)
// 
// KiAttachProcessbf:
// 			mov     dword ptr [esi+50h],ebx
// 			lock and dword ptr [edi],eax
// 			mov     eax,dword ptr [ebp+10h]
// 			push    dword ptr [eax+10h]
// 
// KiAttachProcesscb:
// 			push    ebx
// 			call    g_uKiSwapProcess
// 			mov     cl,byte ptr [ebp+0Ch]
// 			call    dword ptr [g_uImp_KfLowerIrql]//dword ptr [nt!_imp_KfLowerIrql (8420e158)]   7
// 			pop     edi
// 			pop     esi
// 			pop     ebx
// 			pop     ebp
// 			ret     0Ch
// 		}
// 	}
//__asm int 3;
//KdPrint(("g_uKiAttachProcessAddr=%x\n",g_uKiAttachProcessAddr));
//__asm jmp g_uKiAttachProcessAddr
// #pragma PAGECODE
// VOID HookKeAttachProcess()
// {
// // 	83e62062 8bc6            mov     eax,esi
// // 	83e62064 ff7508          push    dword ptr [ebp+8]
// // 	83e62067 e867b80500      call    nt!KiAttachProcess (83ebd8d3)
// 
// 	char pCode[] = 
// 	{
// 		(char)0x8b, (char)0xc6,	(char)0xff, 
// 		(char)0x75, (char)0x08, (char)0xe8
// 	};
// 	
// 	ULONG uOriginKeAttachProcess = GetServiceOldAddr(L"KeAttachProcess");
// 	ULONG uKiCallAddr = SearchCode(uOriginKeAttachProcess, pCode, sizeof(pCode));
// 	KdPrint(("KiCallAddr=%x\n",uKiCallAddr));
// 	g_uKiAttachProcessAddr= GetCallAddr(uKiCallAddr);
// 	KdPrint(("KiAttachProcessAddr=%x\n",g_uKiAttachProcessAddr));
// 
// 	char code1[2]={(char)0x57,(char)0xe8};
// 	ULONG addr=SearchCode(g_uKiAttachProcessAddr,code1,2);
// 	g_uKiMoveApcState=GetCallAddr(addr);              //1
// 	KdPrint(("KiMoveApcState1=%x\n",g_uKiMoveApcState));
// 
// 	char code2[2]={(char)0xc3,(char)0xe8};
// 	addr=SearchCode(g_uKiAttachProcessAddr,code2,2);
// 	g_uKiInSwapSingleProcess=GetCallAddr(addr);              //2
// 	KdPrint(("KiInSwapSingleProcess1=%x\n",g_uKiInSwapSingleProcess));
// 	
// 	char code3[2]={(char)0x85,(char)0x05};
// 	addr=SearchCode(g_uKiAttachProcessAddr,code3,2);
// 	g_uHvlLongSpinCountMask=*((ULONG*)addr);              //3
// 	KdPrint(("HvlLongSpinCountMask1=%x\n",g_uHvlLongSpinCountMask));
// 	
// 	char code4[2]={(char)0xf6,(char)0x05};
// 	g_uHvlEnlightenments=*((ULONG*)(SearchCode(g_uKiAttachProcessAddr,code4,2)));        //4
// 	KdPrint(("HvlEnlightenments1=%x\n",g_uHvlEnlightenments));
// 
// 	char code5[2]={(char)0x50,(char)0xe8};
// 	addr=SearchCode(g_uKiAttachProcessAddr,code5,2);
// 	g_uHvlNotifyLongSpinWait=GetCallAddr(addr);        //5
// 	KdPrint(("HvlNotifyLongSpinWait1=%x\n",g_uHvlNotifyLongSpinWait));
// 
// 	char code6[2]={(char)0x53,(char)0xe8};
// 	addr=SearchCode(g_uKiAttachProcessAddr,code6,2);
// 	g_uKiSwapProcess=GetCallAddr(addr);        //6
// 	KdPrint(("KiSwapProcess1=%x\n",g_uKiSwapProcess));
// 	
// 	char code7[2]={(char)0xff,(char)0x15};
// 	addr=SearchCode(g_uKiAttachProcessAddr,code7,2);
// 	g_uImp_KfLowerIrql=*((ULONG*)addr);        //7
// 	KdPrint(("imp_KfLowerIrql1=%x\n",g_uImp_KfLowerIrql));
// 
// 	char myway[6]={(char)0x8b,(char)0xc6,(char)0xff,(char)0x75,(char)0x08,(char)0xe8};
// 	ULONG uCallAddr=SearchCode(GetServiceOldAddr(L"KeAttachProcess"),myway,6);
// 	g_uHookKeAttachProcessAddr=uCallAddr;
// 	//ERROR
// 	CallHook((ULONG)MyKiAttachProcess, uCallAddr);
// 
// 	KdPrint(("nCallAddr=%x\n",uCallAddr));
// }
#endif 
