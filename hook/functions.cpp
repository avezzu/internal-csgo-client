#include "../move/move.h"
#include "../Interface/interfaces.h"
#include "fucntions.h"


bool __stdcall FN::CreateMove(float frameTime, UserCmd* cmd)
{
	bool result = OriginalCreateMove(interfaces::g_ClientMode, frameTime, cmd);

	if (!cmd || !cmd->commandNumber) {
		return result;
	}

	if (move::bhop)move::enableBhop(cmd);

	return false;
}


void __stdcall FN::DoPostScreenSpaceEffects(const void* viewSetup)
{

	/*
	auto localPlayer = (interfaces::EntityList->GetClientEntity(1));
	if (localPlayer && interfaces::engine->IsInGame())
	{
		for (int i = 0; i < interfaces::glow->glowObjects.size; i++)
		{
			IGlowManager::CGlowObject& glowObject = interfaces::glow->glowObjects[i];

			if (glowObject.IsUnused())
				continue;

			if (!glowObject.entity)
				continue;



		}
	}*/

	OriginalDoPostScreenSpaceEffects(interfaces::g_ClientMode, viewSetup);
}
