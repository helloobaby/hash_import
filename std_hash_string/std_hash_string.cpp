#include <iostream>
#include <functional>
using namespace std;

int main()
{
    std::size_t hash_value = hash<string>{}("VirtualAllocEx");
    cout << hash_value << endl;

    hash_value = hash<string>{}("OpenProcess");
    cout << hash_value << endl;

    hash_value = hash<string>{}("WriteProcessMemory");
    cout << hash_value << endl;

    hash_value = hash<string>{}("CreateRemoteThread");
    cout << hash_value << endl;

    hash_value = hash<string>{}("CreateToolhelp32Snapshot");
    cout << hash_value << endl;

    hash_value = hash<string>{}("Process32FirstW");
    cout << hash_value << endl;

    hash_value = hash<string>{}("Process32NextW");
    cout << hash_value << endl;
    return 0;
}
