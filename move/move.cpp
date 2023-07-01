#include "move.h"
#include "../csgo.hpp"
#include "../Interface/interfaces.h"

void move::enableBhop(UserCmd* cmd)
{
	auto index = interfaces::engine->GetLocalPlayerIndex();
	auto localPlayer = (interfaces::EntityList->GetClientEntity(index));
	if (localPlayer)
	{
		if (!(localPlayer->GetFlags() & 1))
		{
			cmd->buttons &= ~IN_JUMP;
		}
	}

}