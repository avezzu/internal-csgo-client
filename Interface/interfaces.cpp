#include "interfaces.h"

IBaseClientDLL* interfaces::g_Client = GetInterface<IBaseClientDLL>("VClient018", "client.dll");
void* interfaces::g_ClientMode = **(void***)((*(unsigned int**)(g_Client))[10] + 5);
IClientEntityList* interfaces::EntityList = GetInterface<IClientEntityList>("VClientEntityList003", "client.dll");
IVEngineClient* interfaces::engine = GetInterface<IVEngineClient>("engine.dll", "VEngineClient014");
IGlowManager* interfaces::glow = (IGlowManager*)(interfaces::client_module + hazedumper::signatures::dwGlowObjectManager);
