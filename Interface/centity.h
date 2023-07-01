#pragma once
#include "../netvar/netvar.h"
#include "../utils/cvector.h"
#include "cclientclass.h"
#include "interfaces.h"
#include <cstdint>


class CEntity;
class IClientUnknown
{
public:
	constexpr CEntity* GetBaseEntity() noexcept
	{
		return Call<CEntity*>(this, 7);
	}
};

class IClientRenderable
{
public:
	constexpr IClientUnknown* GetIClientUnknown() noexcept
	{
		return Call<IClientUnknown*>(this, 0);
	}
};


class CEntity
{
public:
	NETVAR(Spotted, "CBaseEntity->m_bSpotted", bool);
	NETVAR(FlashDuration, "CCSPlayer->m_flFlashDuration", int);
	NETVAR(GetFlags, "CBasePlayer->m_fFlags", std::int32_t);
	NETVAR(Health, "CBasePlayer->m_iHealth", std::int32_t);
	NETVAR(Team, "CBaseEntity->m_iTeamNum", std::int32_t);


	//call function form interface
	//SRC: https://github.com/cazzwastaken/based/blob/master/src/valve/centity.h

	constexpr CClientClass* GetClientClass() noexcept
	{
		return Call<CClientClass*>(this + 0x8, 2);
	}
	
	CVector& GetAbsOrigin() 
	{
		return Call<CVector&>(this, 10);
	}

	constexpr std::int32_t GetTeam() 
	{
		return Call<std::int32_t>(this, 88);
	}

	constexpr std::int32_t GetHealth() 
	{
		return Call<std::int32_t>(this, 122);
	}

	constexpr bool IsAlive() 
	{
		return Call<bool>(this, 156);
	}

	constexpr bool IsPlayer() 
	{
		return Call<bool>(this, 158);
	}

	constexpr bool IsWeapon() 
	{
		return Call<bool>(this, 166);
	}

	constexpr CEntity* GetActiveWeapon() 
	{
		return Call<CEntity*>(this, 268);
	}

	constexpr void GetEyePosition(CVector& eyePosition) 
	{
		Call<void>(this, 285, std::ref(eyePosition));
	}

	constexpr CEntity* GetObserverTarget() 
	{
		return Call<CEntity*>(this, 295);
	}

	constexpr void GetAimPunch(CVector& aimPunch) 
	{
		Call<void>(this, 346, std::ref(aimPunch));
	}

	constexpr int GetWeaponType() 
	{
		return Call<int>(this, 455);
	}

};


