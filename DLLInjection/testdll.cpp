#include <windows.h>
#include "testdll.h"

BOOL WINAPI DllMain(HMODULE hmodule, DWORD reason_for_call, LPVOID lpres)
{
	if(reason_for_call == DLL_PROCESS_ATTACH)
    MessageBox(NULL,"HELLO WORLD!","HELLO",MB_OK);

    return TRUE;
}

//g++ -c -DBUILDING_EXAMPLE_DLL example_dll.cpp; g++ -shared -o example_dll.dll example_dll.o