#include "vis.h"
#include "../csgo.hpp"
#include "../Interface/interfaces.h"


bool vis::antiflash = false;

void vis::AntiFlash() 
{	
	auto localPlayer = (interfaces::EntityList->GetClientEntity(1));
	if (localPlayer) localPlayer->AntiFlash();
}


