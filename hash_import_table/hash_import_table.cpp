#include <iostream>
#include <windows.h>

#include ".h"

#include "../shared/get_proc_address.h"
using namespace std;

using GetProcAddress_t = FARPROC(WINAPI*)(HMODULE hModule, LPCSTR lpProcName);

int main()
{
    PVOID K32Base = GetKernel32();
    cout << hex << K32Base << endl;

    cout << hex << get_proc_address((HMODULE)K32Base, "OpenProcess") << endl;

    cout << hex << (PVOID)OpenProcess << endl;

    return 0;
}