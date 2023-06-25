#pragma once
#include "../netvar/netvar.h"

class CEntity
{
public:
	NETVAR(Spotted, "CBaseEntity->m_bSpotted", bool);
	NETVAR(FlashDuration, "CCSPlayer->m_flFlashDuration", int);
};


