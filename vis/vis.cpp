#include "vis.h"
#include "../csgo.hpp"
#include "../Interface/interfaces.h"


bool vis::antiflash = false;
void vis::AntiFlash() 
{	
	auto localPlayer = (interfaces::EntityList->GetClientEntity(1));
	if (localPlayer)
	{
		localPlayer->FlashDuration() = 0;
	}
}

bool vis::enableRadar = false;
void vis::EnableRadar()
{
	for (int i = 1; i <= 64; i++)
	{
		auto entity = interfaces::EntityList->GetClientEntity(i);
		if (!entity)
			continue;

		entity->Spotted() = true;
	}
}
