#include "NierAutomata.h"
#include <string>
#include <fstream>
#include "data.h"

BOOL WINAPI MainThread()
{
    NierAutomata na;
    na.CreateConsole();
    na.Initialise(); // Resolve any runtime values
    na.Update(); // Re-read and update values
     
    return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, nullptr, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
