


DWORD ssdtaddr, 
oldreadaddr,readpush ,readjmpaddr,
oldwriteaddr,writepush ,writejmpaddr  ,
ntopenprcaddr  ,
ntopentrdaddr  ,
KiAttachProcessaddr,KeStackAttachProcess_jmpaddr,
ObOpenObjectByPointeraddr/*,
h1,h2,hr,hw*/;


#pragma PAGECODE
__declspec(naked) VOID yjxsoft_com_KeStackAttachProcess()
{  
	KdPrint(("yjxsoft_com_KeStackAttachProcess\n"));
    //KeStackAttachProcess_jmpaddr+0xD
	__asm
	{
		
		//还原jmp占用处指令字节指令
        push    dword ptr [ebp+0ch]
		push    dword ptr [ebp+8]
		push    edi
		push    esi
        //压入返回地址
		mov eax,KeStackAttachProcess_jmpaddr
		add eax,0xd //push    dword ptr [ebp+8] 至CALL KiAttachProcess后的距离
		push eax //返回地址
		//KiAttachProcess 前7字节
		mov edi,edi
		push    ebp
		mov     ebp,esp
		push    ebx
		push    esi
		//jmp 至KiAttachProcess+7位置执行，执行完返回至		KeStackAttachProcess_jmpaddr+D处继续执行
		mov eax,  KiAttachProcessaddr
		add eax,7
		jmp eax

	}


}
#pragma PAGECODE
__declspec(naked) VOID yjxsoft_com_KiAttachProcess()
{  
	//KdPrint(("yjxsoft_com_KiAttachProcess\n"));

	__asm
	{
		/*  804f9b5e ff7508          push    dword ptr [ebp+8]
		    804f9b61 57              push    edi
			804f9b62 56              push    esi

			804f9b63 e890feffff      call    nt!KiAttachProcess (804f99f8)
			804f9b68 5f              pop     edi
			804f9b69 5e              pop     esi
			804f9b6a 5d              pop     ebp
			804f9b6b c20400          ret     4 
			*/
		 push    dword ptr [ebp+8]
		 push    edi
		 push    esi

		 //恢复 前7字节指令
		/* nt!KiAttachProcess:
		 804f99f8 8bff                mov     edi,edi
			 804f99fa 55              push    ebp
			 804f99fb 8bec            mov     ebp,esp
			 804f99fd 53              push    ebx
			 804f99fe 56              push    esi
		 */
		 push endCall //压栈返回地址
		 mov edi,edi
		 push    ebp
		 mov     ebp,esp
		 push    ebx
		 push    esi
		 //		
		 mov eax,  KiAttachProcessaddr
		 add eax,7
		 jmp eax
endCall:
		 pop edi
		 pop esi 
		 pop ebp
		 ret 4

	}


}

#pragma PAGECODE
__declspec(naked) VOID yjxsoft_com_NtReadVirtualMemory()
{  
	//KdPrint(("yjxsoft_com_NtReadVirtualMemory\n"));
   
	__asm
	{
		        // 
		        push 0x1c
				push readpush ////压栈参数
				// 
				jmp  readjmpaddr
	}


}
//
//;///////////////////////////////////////////////////////////////////
//
#pragma PAGECODE
__declspec(naked)VOID yjxsoft_com_NtWriteVirtualMemory()
{
    // KdPrint(("yjxsoft_com_NtWriteVirtualMemory\n"));
	__asm
	{ 
			push 0x1c
			push writepush //压栈参数
			//  
			jmp  writejmpaddr
	}

}
//
//;/////////////////////////////////////////////////////////////////
//805cc613 8d8548ffffff    lea     eax,[ebp-0B8h]
//805cc619 50              push    eax

//805cc61a ff75c8          push    dword ptr [ebp-38h]
//805cc61d ff75dc          push    dword ptr [ebp-24h]
//805cc620 e84706ffff      call    nt!ObOpenObjectByPointer (805bcc6c)
//805cc625 8bf8            mov     edi,eax
//805cc625-805cc61a= 0bh
#pragma PAGECODE
__declspec(naked)VOID yjxsoft_com_OpenProcess()
{
   
	__asm
	{
		
		push dword ptr [ebp-38h]
		push dword ptr [ebp-24h]
		mov  eax,ntopenprcaddr
		add  eax,0bh
		push eax //CALL后返回的地址 非压栈参数
		//  
		jmp  ObOpenObjectByPointeraddr
	}
}
//
//;//////////////////////////////////////////////////////////////////
//
#pragma PAGECODE
__declspec(naked)VOID yjxsoft_com_OpenTread()
{
	//KdPrint(("yjxsoft_com_OpenTread     \n"));
	__asm
	{
		push dword ptr [ebp-34h]
		push dword ptr [ebp-20h]
		mov  eax,ntopentrdaddr
		add  eax,0bh
		push eax
		//  
		jmp  ObOpenObjectByPointeraddr
	}
}
 
//
//;//////////////////////////////////////////////////////////////////
//
#pragma PAGECODE
 __declspec(naked)   DWORD __stdcall Findcodeaddr(DWORD code1,WORD code2,DWORD prcaddr)
{ 
	 
	 __asm
	 {
		mov    edx,[esp+0x0c] //prcaddr
		mov    ecx,1000h
		//while( ecx>0)
		////  
start:
		sub ecx,1h
		add edx,1h
		mov ebx,dword ptr [edx]
		xor eax,eax
		mov ax,word ptr [edx+4h]
		test ecx,ecx
			jz end
		cmp ebx,[esp+4] //code1
			jnz start
		cmp ax,[esp+8] //code2
			jnz start
		/*if (ebx==code1 && ax==code2)  break  */
end:
		mov    eax,edx
		ret 0x0c
	 }
 }
//;////////////////////////////////////////////////////////////////
//
#pragma PAGECODE
 VOID   PassTPhook ()
{     
	  UNICODE_STRING g_ProcessAddr,g_ThreadAddr,g_KeAttachAddr,g_PointerAddr,g_KeStackAttachProcess;
	   RtlInitUnicodeString(&g_ProcessAddr,L"NtOpenProcess");
	   RtlInitUnicodeString(&g_ThreadAddr,L"NtOpenThread");
	   RtlInitUnicodeString(&g_KeAttachAddr,L"KeAttachProcess");
	   RtlInitUnicodeString(&g_PointerAddr,L"ObOpenObjectByPointer");
	   RtlInitUnicodeString(&g_KeStackAttachProcess,L"KeStackAttachProcess");

	__asm
	{
		
		//pushad
		lea eax,g_KeStackAttachProcess
		push eax
		call DWORD ptr DS:[ MmGetSystemRoutineAddress]
		push eax		//传入 KeStackAttachProcess地址
		//push 0E856h    
		//push 570875FFh		
		push 0875h
		push 0FF0C75FFh
		call Findcodeaddr
		mov    KeStackAttachProcess_jmpaddr,eax //// KeStackAttachProcess里 CALL KiAttachProcess的地址 定位push dword ptr[ebp+8]
		//	hook KeStackAttachProcess+7d
		lea ebx,yjxsoft_com_KeStackAttachProcess
		sub ebx,eax
		sub ebx,5
		mov BYTE ptr [eax],0E9h
		mov dword ptr [eax+1],ebx

		//hook end

		/////////////
		lea eax,g_ProcessAddr
		push eax
		call DWORD ptr DS:[ MmGetSystemRoutineAddress]
		push eax		//传入 NtOpenProcess地址
		push 0dc75h
		push 0ffc875ffh		
		call Findcodeaddr
		mov    ntopenprcaddr,eax ///NtOpenProcess里 CALL ObOpenObjectByPointer的地址

		lea eax, g_ThreadAddr
		push eax
		call DWORD ptr DS:[ MmGetSystemRoutineAddress]
		push eax     ////传入 NtOpenThread地址
		push 0e075h
		push 0ffcc75ffh
		call Findcodeaddr
		mov    ntopentrdaddr,eax //////存放 NtOpenThread 里 CALL ObOpenObjectByPointer的地址

		lea eax,g_PointerAddr
		push eax
		call DWORD ptr DS:[ MmGetSystemRoutineAddress]
		mov    ObOpenObjectByPointeraddr,eax //存放 ObOpenObjectByPointer地址

		//hook KiAttachProcess  begin
		lea eax,g_KeAttachAddr
		push eax
		call DWORD ptr DS:[ MmGetSystemRoutineAddress] 
		push eax    // 传入KeAttachProcess
		push 0e856h
		push 570875ffh
		call Findcodeaddr //查找KiAttachProcess地址
		add    eax,6h //指向 804f9b64//     804f9b63 e890feffff      call    nt!KiAttachProcess (804f99f8)

		mov    ebx,dword ptr [eax] //取CALL 地址
		add    ebx,eax //当前地址+取出地址+4
		add    ebx,4h
		mov    KiAttachProcessaddr,ebx //计算出 KiAttachProcess 这个未导出函数地址
		
		sub eax,6h //mov    KiAttachProcess -6
		 
		// HOOK KeAttachProcessPush->KiAttachProcessPush-5
		mov dword ptr[eax],0xE9 //jmp
		lea ebx,yjxsoft_com_KiAttachProcess
		sub ebx,eax
		sub ebx,5
		mov dword ptr[eax+1],ebx

       //hook KiAttachProcess end

		mov eax, KeServiceDescriptorTable
		mov eax,[eax]                     //address of KeServiceDescriptorTable
		mov ssdtaddr,eax

		mov eax,ssdtaddr
		add eax,2e8h                     //0xBA * 4   ntreadvirtualmemoryaddr ssdt186
		mov eax,DWORD PTR [eax]
		mov oldreadaddr,eax
		mov ebx,DWORD PTR [eax+3h]
		mov readpush,ebx
		add eax,7h
		mov readjmpaddr,eax

		mov eax,ssdtaddr
		add eax,454h                     //0x115*4    ntwritevirtualmemoryaddr ssdt227
		mov eax,DWORD PTR [eax]
		mov oldwriteaddr,eax
		mov ebx,DWORD PTR [eax+3h]
		mov writepush,ebx
		add eax,7h
		mov writejmpaddr,eax

		//开始各种HOOK
 		cli
 		mov eax,cr0
 		and eax,not 10000h
 		mov cr0,eax
		//HOOK NtReadVirtualMemory
 		mov ebx,ssdtaddr
 		mov eax,yjxsoft_com_NtReadVirtualMemory
 		mov DWORD ptr [ebx+2e8h],eax

		//HOOK NtWriteVirtualMemory
 		mov eax,yjxsoft_com_NtWriteVirtualMemory
 		mov DWORD ptr [ebx+454h],eax

		//HOOK NtOpenProcess
 		mov ebx,ntopenprcaddr
 		mov BYTE ptr [ebx],0e9h
		/*	int 3*/
 		mov eax,offset yjxsoft_com_OpenProcess
		lea eax,yjxsoft_com_OpenProcess
 		sub eax,ebx
 		sub eax,5h
 		mov DWORD ptr [ebx+1h],eax
		
		//HOOK NtOpenTread
 		mov ebx,ntopentrdaddr
 		mov BYTE ptr [ebx],0e9h
 		mov eax,offset yjxsoft_com_OpenTread
 		sub eax,ebx
 		sub eax,5h //计算跳转地址
 		mov DWORD ptr [ebx+1h],eax //紧接着 E9后边写 跳转地址
		lea ebx,DWORD ptr[ebx+1h]
 
 		mov eax,cr0
 		or  eax,10000h
 		mov cr0,eax
 		sti
		
		

}
//
//;/////////////////////////////////////////////////////////////////
//
void jxcom_UnHook()
{
	__asm
	{
		pushad
		cli
		mov  eax,cr0
		and  eax,not 10000h
		mov  cr0,eax
		//UnHook NtRead&&NtWrite
		mov eax,ssdtaddr
		mov ebx,DWORD ptr [oldreadaddr]
		mov DWORD ptr [eax+2e8h],ebx
		mov ebx,DWORD ptr [oldwriteaddr]
		mov DWORD ptr [eax+454h],ebx
		//UnHook NtOpenProcess
		mov  eax,ntopenprcaddr
		mov  dword PTR [eax],0ffc875ffh
		mov  WORD ptr [eax+4h],0dc75h
		//UnHook NtOpenThread
		mov  eax,ntopentrdaddr
		mov  dword PTR [eax],0ffcc75ffh
		mov  WORD ptr [eax+4h],0e075h

		mov  eax,cr0
		or   eax,10000h
		mov  cr0,eax
		sti
		popad
	}

}
//yjxsoft_com_UnHook end
//;////////////////////////////////////////////////////////////////////
