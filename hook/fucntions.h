#pragma once
#include "../Interface/usercmd.h"

namespace FN
{

	inline bool(__thiscall* OriginalCreateMove)(void*, float, UserCmd*) = nullptr;
	bool __stdcall CreateMove(float frameTime, UserCmd* userCmd);

	inline void(__thiscall* OriginalDoPostScreenSpaceEffects)(void*, const void*) = nullptr;
	void __stdcall DoPostScreenSpaceEffects(const void* viewSetup);

}
