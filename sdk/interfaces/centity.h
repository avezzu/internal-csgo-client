#pragma once
#include "../../src/math/types/vector.h"
#include "../../src/math/types/matrix3x4.h"
#include "../../src/virtual/virtual.h"
#include "../netvars/netvar.h"
#include "cclientclass.h"
#include <cstdint>


class CEntity;
class IClientUnknown
{
public:
	CEntity* GetBaseEntity() noexcept
	{
		
		return vt::Call<CEntity*>(this, 7);
	}
};

class IClientRenderable
{
public:
	IClientUnknown* GetIClientUnknown() noexcept
	{
		return vt::Call<IClientUnknown*>(this, 0);
	}
};

class CModel;
class CEntity
{
public:
	NETVAR(Spotted, "CBaseEntity->m_bSpotted", bool);
	NETVAR(FlashDuration, "CCSPlayer->m_flFlashDuration", int);
	NETVAR(GetFlags, "CBasePlayer->m_fFlags", std::int32_t);
	NETVAR(Health, "CBasePlayer->m_iHealth", std::int32_t);
	NETVAR(Team, "CBaseEntity->m_iTeamNum", std::int32_t);
	NETVAR(ShotsFired, "CCSPlayer->m_iShotsFired", std::int32_t);
	NETVAR(SpottedByMask, "CBaseEntity->m_bSpottedByMask", bool);
	NETVAR(iItemIDHigh, "CBaseAttributableItem->m_iItemIDHigh", int);
	NETVAR(FallbackPaintKit, "CBaseAttributableItem->m_nFallbackPaintKit", int);
	NETVAR(FallbackWear, "CBaseAttributableItem->m_flFallbackWear", float);
	NETVAR(DefaultFOV, "CBasePlayer->m_iDefaultFOV", int);


	//call function form interface
	//SRC: https://github.com/cazzwastaken/based/blob/master/src/valve/centity.h

	
	bool SetupBones(matrix3x4_t* out, std::int32_t max, std::int32_t mask, float currentTime) noexcept
	{
		return vt::Call<bool>(this + 0x4, 13, out, max, mask, currentTime);
	}

	DWORD* GetWeapons()
	{
		auto offset = netvars[fnv::HashConst("CBaseCombatCharacter->m_hMyWeapons")];
		return (DWORD*)(this + offset);
	}

	bool IsDormant() noexcept
	{
		return vt::Call<bool>(this + 0x8, 9);
	}

	CClientClass* GetClientClass() noexcept
	{
		return vt::Call<CClientClass*>(this + 0x8, 2);
	}
	
	Vector& GetAbsOrigin() 
	{
		return vt::Call<Vector&>(this, 10);
	}

	std::int32_t GetTeam() 
	{
		return vt::Call<std::int32_t>(this, 88);
	}

	std::int32_t GetHealth() 
	{
		return vt::Call<std::int32_t>(this, 122);
	}

	bool IsAlive() 
	{
		return vt::Call<bool>(this, 156);
	}

	bool IsPlayer() 
	{
		return vt::Call<bool>(this, 158);
	}

	bool IsWeapon() 
	{
		return vt::Call<bool>(this, 166);
	}

	CEntity* GetActiveWeapon() 
	{
		return vt::Call<CEntity*>(this, 268);
	}

	void GetEyePosition(Vector& eyePosition) 
	{
		vt::Call<void>(this, 285, std::ref(eyePosition));
	}

	CEntity* GetObserverTarget() 
	{
		return vt::Call<CEntity*>(this, 295);
	}

	void GetAimPunch(Vector& aimPunch) 
	{
		vt::Call<void>(this, 346, std::ref(aimPunch));
	}

	int GetWeaponType() 
	{
		return vt::Call<int>(this, 455);
	}

	CModel* GetModel()
	{
		return vt::Call<CModel*>(this + 0x4, 8);
	}

};


