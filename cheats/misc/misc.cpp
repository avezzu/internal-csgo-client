#include "misc.h"

void misc::enableBhop(UserCmd* cmd)
{
	auto index = interfaces::Get().engine->GetLocalPlayerIndex();
	auto localPlayer = (interfaces::Get().EntityList->GetClientEntity(index));
	if (localPlayer)
	{
		if (!(localPlayer->GetFlags() & 1))
		{
			cmd->buttons &= ~IN_JUMP;
		}
	}

}