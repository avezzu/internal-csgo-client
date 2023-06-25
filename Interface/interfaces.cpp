#include "interfaces.h"


void* interfaces::g_Client = GetInterface<void>("VClient018", "client.dll");
void* interfaces::g_ClientMode = **(void***)((*(unsigned int**)(g_Client))[10] + 5);
IClientEntityList* interfaces::EntityList = GetInterface<IClientEntityList>("VClientEntityList003", "client.dll");