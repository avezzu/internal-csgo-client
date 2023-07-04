#include "interfaces.h"

IBaseClientDLL* interfaces::g_Client = GetInterface<IBaseClientDLL>("VClient018", "client.dll");
void* interfaces::g_ClientMode = **(void***)((*(unsigned int**)(g_Client))[10] + 5);
IClientEntityList* interfaces::EntityList = GetInterface<IClientEntityList>("VClientEntityList003", "client.dll");
IVEngineClient* interfaces::engine = GetInterface<IVEngineClient>("VEngineClient014","engine.dll");
IEngineTraceClient* interfaces::engineTrace = GetInterface<IEngineTraceClient>("EngineTraceClient004", "engine.dll");
IVDebugOverlay* interfaces::debugOverlay = GetInterface<IVDebugOverlay>("VDebugOverlay004", "engine.dll");
IVPanel* interfaces::panel = GetInterface<IVPanel>("VGUI_Panel009", "vgui2.dll");
ISurface* interfaces::surface = GetInterface<ISurface>("VGUI_Surface031", "vguimatsurface.dll");
IEngineVGui* interfaces::engineVGui = GetInterface<IEngineVGui>("VEngineVGui001", "engine.dll");
IGlobalVars* interfaces::globals = **(IGlobalVars***)((*(uintptr_t**)(g_Client))[11] + 10);
IVModelInfo* interfaces::modelInfo = GetInterface<IVModelInfo>("VModelInfoClient004", "engine.dll");
IGlowManager* interfaces::glow = (IGlowManager*)(*(uint8_t**)(scanner::PatternScan("client.dll", "A1 ? ? ? ? A8 01 75 4B") + 1) + 4);
