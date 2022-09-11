.code

;
; 在obj文件里实际要的符号名字是?GetKernel32@@YAPEAXXZ
; 编译器无法解析的是?GetKernel32@@YAPAXXZ
;
;
;C:\Users\asdf>demumble ?GetKernel32@@YAPEAXXZ
;void * __cdecl GetKernel32(void)

;C:\Users\asdf>demumble ?GetKernel32@@YAPAXXZ
;void * __cdecl GetKernel32(void)
;
;
;

?GetKernel32@@YAPEAXXZ proc
	mov r12, gs:[60h]       ;peb
	mov r12, [r12 + 18h]   ;Peb --> LDR
	mov r12, [r12 + 20h]   ;Peb.Ldr.InMemoryOrderModuleList
	mov r12, [r12]          ;2st entry
	mov r15, [r12 + 20h]   ;ntdll.dll base address!
	mov r12, [r12]          ;3nd entry
	mov r12, [r12 + 20h]   ;kernel32.dll base address! We go 20 bytes in here as we are already 10 bytes into the _LDR_DATA_TABLE_ENTRY from the InMemoryOrderModuleList
	mov rax,r12
	ret
?GetKernel32@@YAPEAXXZ endp



end