#include <windows.h>


//
// 自实现win32 GetProcAddress  适合x86 x64
//
PVOID get_proc_address(
    HMODULE hModule,    // handle to DLL module
    LPCSTR lpProcName   // function name
);

//
// 无字符串特征改良版
//
PVOID get_proc_address(
    HMODULE hModule,    // handle to DLL module
    size_t hash   // function name
);