#include <iostream>
#include <windows.h>
#include ".h"
using namespace std;

int main()
{
    PVOID K32Base = GetKernel32();
    cout << hex << K32Base << endl;


    return 0;
}

