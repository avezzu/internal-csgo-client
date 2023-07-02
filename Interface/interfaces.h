#pragma once
#include "getInterface.h"
#include "EntityInterface.h"
#include "ivengineclient.h"
#include "ienginetraceclient.h"
#include "iglowmanager.h"
#include "ivdebugoverlay.h"
#include "ienginevgui.h"
#include <cstdint>
#include <functional>
#include <Windows.h>
#include "../csgo.hpp"
#include "isurface.h"
#include "IGlobalVars.h"

namespace interfaces
{
	const uintptr_t client_module = (uintptr_t)GetModuleHandle("client.dll");
	const uintptr_t engine_module = (uintptr_t)GetModuleHandle("engine.dll");

	extern IBaseClientDLL* g_Client;
	extern void* g_ClientMode;
	extern IClientEntityList* EntityList;
	extern IVEngineClient* engine;
	extern IGlowManager* glow;
	extern IEngineTraceClient* engineTrace;
	extern IVDebugOverlay* debugOverlay;
	extern IVPanel* panel;
	extern ISurface* surface;
	extern IEngineVGui* engineVGui;
	extern IGlobalVars* globals;
}