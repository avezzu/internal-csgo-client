#pragma once 
#include "interfaces/EntityInterface.h"
#include "interfaces/ivengineclient.h"
#include "interfaces/ienginetraceclient.h"
#include "interfaces/iglowmanager.h"
#include "interfaces/ivdebugoverlay.h"
#include "interfaces/ienginevgui.h"
#include <cstdint>
#include <functional>
#include <Windows.h>
#include "interfaces/isurface.h"
#include "interfaces/ivmodelinfo.h"
#include "interfaces/IGlobalVars.h"
#include "interfaces/istudiorender.h"
#include "interfaces/imaterialsystem.h"
#include "../src/memory/mem.h"
#include "../src/utils/constants.h"
#include "../src/signatures/sig.h"

class interfaces
{
	public:
	static interfaces& Get()
	{
		static interfaces instance;
		return instance;
	}
	
	const uintptr_t client_module = (uintptr_t)GetModuleHandle(CConstants::CLIENT_LIB);
	const uintptr_t engine_module = (uintptr_t)GetModuleHandle(CConstants::ENGINE_LIB);

	IBaseClientDLL* g_Client = mem::GetInterface(CConstants::CLIENT_LIB, "VClient018").Get<IBaseClientDLL*>();
	void* g_ClientMode = **(void***)((*(unsigned int**)(g_Client))[10] + 5);
	IClientEntityList* EntityList = mem::GetInterface(CConstants::CLIENT_LIB, "VClientEntityList003").Get<IClientEntityList*>();
	IVEngineClient* engine = mem::GetInterface(CConstants::ENGINE_LIB, "VEngineClient014").Get<IVEngineClient*>();
	IEngineTraceClient* engineTrace = mem::GetInterface(CConstants::ENGINE_LIB, "EngineTraceClient004").Get<IEngineTraceClient*>();
	IVDebugOverlay* debugOverlay = mem::GetInterface(CConstants::ENGINE_LIB, "VDebugOverlay004").Get<IVDebugOverlay*>();
	IVPanel* panel = mem::GetInterface( CConstants::VGUI_LIB, "VGUI_Panel009").Get<IVPanel*>();
	ISurface* surface = mem::GetInterface(CConstants::MATSURFACE_LIB, "VGUI_Surface031").Get<ISurface*>();
	IEngineVGui* engineVGui = mem::GetInterface(CConstants::ENGINE_LIB, "VEngineVGui001").Get<IEngineVGui*>();
	IGlobalVars* globals = **(IGlobalVars***)((*(uintptr_t**)(g_Client))[11] + 10);
	IVModelInfo* modelInfo = mem::GetInterface(CConstants::ENGINE_LIB, "VModelInfoClient004").Get<IVModelInfo*>();
	IStudioRender* studioRender = mem::GetInterface(CConstants::STUDIORENDER_LIB, "VStudioRender026").Get<IStudioRender*>();
	IMaterialSystem* materialSystem = mem::GetInterface(CConstants::MATERIAL_LIB, "VMaterialSystem080").Get<IMaterialSystem*>();
	IGlowManager* glow = signatures::GlowManager.GetPtrAs<IGlowManager*>();
};