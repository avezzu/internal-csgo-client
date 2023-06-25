#include "utils/pch.h"

DWORD WINAPI MainThread(HMODULE hModule) {

    AllocConsole();
    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout);

    std::cout << "DLL injected!" << std::endl;

    SetupNetvars(file);

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
        if (vis::enableRadar)vis::EnableRadar();
    }
    
UNLOAD:
    if (file)
        fclose(file);
    FreeConsole();
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
