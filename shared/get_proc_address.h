#include <windows.h>


//
// ��ʵ��win32 GetProcAddress  �ʺ�x86 x64
//
PVOID get_proc_address(
    HMODULE hModule,    // handle to DLL module
    LPCSTR lpProcName   // function name
);

//
// ���ַ�������������
//
PVOID get_proc_address(
    HMODULE hModule,    // handle to DLL module
    size_t hash   // function name
);