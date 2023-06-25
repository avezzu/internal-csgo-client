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
	
	OriginalDoPostScreenSpaceEffects(interfaces::g_ClientMode, viewSetup);
}
