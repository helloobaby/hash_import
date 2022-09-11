#include <iostream>
#include <windows.h>
using namespace std;

PVOID GetKernel32();

using GetProcAddress_t = FARPROC (WINAPI*)(HMODULE hModule,LPCSTR lpProcName);

int main()
{
    PVOID K32Base = GetKernel32();
    cout << hex << K32Base << endl;

    GetProcAddress_t pfnGetProcAddress = (GetProcAddress_t)GetProcAddress((HMODULE)K32Base, "GetProcAddress");
    
    cout << hex << pfnGetProcAddress(LoadLibraryW(L"kernel32.dll"), "OpenProcess") << endl;
    

    return 0;
}

