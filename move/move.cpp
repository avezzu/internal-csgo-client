#include "move.h"
#include "../csgo.hpp"
#include "../Interface/interfaces.h"

void move::enableBhop(UserCmd* cmd)
{
	
	auto localPlayer = (interfaces::EntityList->GetClientEntity(1));
	if (localPlayer)
	{
		if (!(*(int*)(localPlayer + hazedumper::netvars::m_fFlags) & 1))
		{
			cmd->buttons &= ~IN_JUMP;
		}
	}

}