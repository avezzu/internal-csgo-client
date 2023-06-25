#include "pch.h"

DWORD WINAPI MainThread(HMODULE hModule) {


    try
    {
        gui::Setup();
        hooks::Setup();
    }
    catch (std::exception& error)
    {
        MessageBeep(MB_ICONERROR);
        MessageBox(
            0,
            error.what(),
            "Failed to start ImGUI!",
            MB_OK | MB_ICONEXCLAMATION
        );

        goto UNLOAD;
    }
    
    while (!GetAsyncKeyState(VK_END))
    {
        if (vis::antiflash)vis::AntiFlash();
    }
    
UNLOAD:
    hooks::Destroy();
    gui::Destroy();
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(
    HANDLE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)

{

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0);
        if (thread) {
            CloseHandle(thread);
        }
    }

    return TRUE;
}
