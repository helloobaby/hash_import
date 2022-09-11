#include "get_proc_address.h"

#include <functional>
#include <string>

PVOID get_proc_address(
    HMODULE hModule,    // handle to DLL module
    LPCSTR lpProcName   // function name
)
{

    int i = 0;
    PIMAGE_DOS_HEADER pImageDosHeader = NULL;
#ifdef _X86_
    PIMAGE_NT_HEADERS32 pImageNtHeader = NULL;
#else 
    PIMAGE_NT_HEADERS64 pImageNtHeader = NULL;
#endif
    PIMAGE_EXPORT_DIRECTORY pImageExportDirectory = NULL;

    pImageDosHeader = (PIMAGE_DOS_HEADER)hModule;
    pImageNtHeader = (PIMAGE_NT_HEADERS)((ULONG_PTR)hModule + pImageDosHeader->e_lfanew);
    pImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((ULONG_PTR)hModule + pImageNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

    DWORD* pAddressOfFunction = (DWORD*)(pImageExportDirectory->AddressOfFunctions + (ULONG_PTR)hModule);
    DWORD* pAddressOfNames = (DWORD*)(pImageExportDirectory->AddressOfNames + (ULONG_PTR)hModule);
    DWORD dwNumberOfNames = pImageExportDirectory->NumberOfNames;
    ULONG_PTR dwBase = (ULONG_PTR)(pImageExportDirectory->Base);

    WORD* pAddressOfNameOrdinals = (WORD*)(pImageExportDirectory->AddressOfNameOrdinals + (ULONG_PTR)hModule);

    // 这个是查一下是按照什么方式（函数名称or函数序号）来查函数地址的
    DWORD dwName = (DWORD)lpProcName;
    if ((dwName & 0xFFFF0000) == 0)
    {
        goto index;
    }
    for (i = 0; i < (int)dwNumberOfNames; i++)
    {
        char* strFunction = (char*)(pAddressOfNames[i] + (ULONG_PTR)hModule);
        if (lstrcmpA(lpProcName, strFunction) == 0)
        {
            return (PVOID)(pAddressOfFunction[pAddressOfNameOrdinals[i]] + (ULONG_PTR)hModule);
        }
    }
    return 0;
    // 这个是通过以序号的方式来查函数地址的
index:
    if (dwName < dwBase || dwName > dwBase + pImageExportDirectory->NumberOfFunctions - 1)
    {
        return 0;
    }
    return (PVOID)(pAddressOfFunction[dwName - dwBase] + (ULONG_PTR)hModule);
}


PVOID get_proc_address(
    HMODULE hModule,    // handle to DLL module
    size_t hash   // function hash
)
{

    int i = 0;
    PIMAGE_DOS_HEADER pImageDosHeader = NULL;
#ifdef _X86_
    PIMAGE_NT_HEADERS32 pImageNtHeader = NULL;
#else 
    PIMAGE_NT_HEADERS64 pImageNtHeader = NULL;
#endif
    PIMAGE_EXPORT_DIRECTORY pImageExportDirectory = NULL;

    pImageDosHeader = (PIMAGE_DOS_HEADER)hModule;
    pImageNtHeader = (PIMAGE_NT_HEADERS)((ULONG_PTR)hModule + pImageDosHeader->e_lfanew);
    pImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((ULONG_PTR)hModule + pImageNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

    DWORD* pAddressOfFunction = (DWORD*)(pImageExportDirectory->AddressOfFunctions + (ULONG_PTR)hModule);
    DWORD* pAddressOfNames = (DWORD*)(pImageExportDirectory->AddressOfNames + (ULONG_PTR)hModule);
    DWORD dwNumberOfNames = pImageExportDirectory->NumberOfNames;
    ULONG_PTR dwBase = (ULONG_PTR)(pImageExportDirectory->Base);

    WORD* pAddressOfNameOrdinals = (WORD*)(pImageExportDirectory->AddressOfNameOrdinals + (ULONG_PTR)hModule);

    for (i = 0; i < (int)dwNumberOfNames; i++)
    {
        char* strFunction = (char*)(pAddressOfNames[i] + (ULONG_PTR)hModule);
        size_t hash_t = std::hash<std::string>()(strFunction);
        if (std::string(strFunction) == "VirtualAllocEx")
        {
            int i = 0;
            i++;
        }
        if (hash_t == hash)
        {
            return (PVOID)(pAddressOfFunction[pAddressOfNameOrdinals[i]] + (ULONG_PTR)hModule);
        }
    }
}