HMODULE GetKernel32() {
    HMODULE hModule;
    _asm {
        mov eax, fs: [0x30]//fs:[00]λ�ô���ŵ�ǰ�̵߳��̻߳����飨teb����teb��0x30ƫ�ƴ�����ŵ�ǰ�߳��������̵�peb
        mov eax, [eax + 0xc]//EAX = PEB->Ldr
        mov esi, [eax + 0x14]//ESI = PEB->Ldr.InMemOrderModuleList      LIST_ENTRY
        lodsd          //EAX = Second module                            
        xchg eax, esi     //EAX = ESI, ESI = EAX
        lodsd          //Next module
        mov ebx, [eax + 0x10]//EBX = Base address
        mov hModule, ebx
    }
    return hModule;
}