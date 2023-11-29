#define WIN32_LEAN_AND_MEAN   

#include <windows.h>
#include <iostream>
#include "src/hook/hook.h"
#include "src/memory/mem.h"
#include "cheats/aim/aim.h"
#include "cheats/vis/vis.h"
#include "sdk/interfaces.h"

BOOL WINAPI MainThread(HMODULE hModule) {

    AllocConsole();
    FILE* file = nullptr;
    freopen_s(&file, "CONOUT$", "w", stdout);
    std::cout << "DLL injected!" << std::endl;

    try
    {
        mem::Initialize();
        hook::Init();
    }
    catch(const std::exception& e)
    {
        MessageBeep(MB_ICONERROR);
        MessageBox(
            0,
            e.what(),
            "Failed to start initialize!",
            MB_OK | MB_ICONEXCLAMATION
        );

        goto UNLOAD;
    }
    
    interfaces::Get().engine->GetScreenSize(vis::screen_width, vis::screen_hight);
    while (!GetAsyncKeyState(VK_END))
    {
        if (vis::antiflash)vis::AntiFlash();
        if (vis::enableRadar)vis::EnableRadar();
        if (aim::enableRC)aim::recoilControl();
        vis::changeFOV(vis::FOV);
    }

UNLOAD:
    hook::Shutdown();
    if (file)
        fclose(file);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(
    HANDLE hModule,
    DWORD dwReason,
    LPVOID lpReserved)

{

    if(dwReason == DLL_PROCESS_ATTACH){
        DisableThreadLibraryCalls((HMODULE)hModule);
        HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0);
        if (thread) {
            CloseHandle(thread);
        }
    }

    return TRUE;
}