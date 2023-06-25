#pragma once
#include "getInterface.h"
#include "EntityInterface.h"
#include "ivengineclient.h"
#include "iglowmanager.h"
#include <cstdint>
#include <Windows.h>
#include "../csgo.hpp"

namespace interfaces
{
	const uintptr_t client_module = (uintptr_t)GetModuleHandle("client.dll");
	const uintptr_t engine_module = (uintptr_t)GetModuleHandle("engine.dll");


	extern IBaseClientDLL* g_Client;
	extern void* g_ClientMode;
	extern IClientEntityList* EntityList;
	extern IVEngineClient* engine;
	extern IGlowManager* glow;


	template <typename Return, typename ... Arguments>
	constexpr Return Call(void* vmt, const std::uint32_t index, Arguments ... args)
	{
		using Function = Return(__thiscall*)(void*, decltype(args)...);
		return (*(Function**)(vmt))[index](vmt, args...);
	}
}