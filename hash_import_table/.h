HMODULE GetKernel32() {
    HMODULE hModule;
    _asm {
        mov eax, fs: [0x30]//fs:[00]位置存放着当前线程的线程环境块（teb），teb的0x30偏移处存放着当前线程所属进程的peb
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