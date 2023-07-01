#pragma once
#include "interfaces.h"

#include "usercmd.h"
#include "../utils/cvector.h"
#include "../utils/cmatrix.h"

class IVEngineClient
{
public:

	// returns index of local player
	std::int32_t GetLocalPlayerIndex() 
	{
		return Call<std::int32_t>(this, 12);
	}

	void GetViewAngles(const CVector& viewAngles)
	{
		Call<void, const CVector&>(this, 18, viewAngles);
	}

	void SetViewAngles(const CVector& viewAngles) 
	{
		Call<void, const CVector&>(this, 19, viewAngles);
	}

	// returns true if in-game
	bool IsInGame() 
	{
		return Call<bool>(this, 26);
	}

	CMatrix4x4& WorldToScreenMatrix() 
	{
		return Call<CMatrix4x4&>(this, 37);
	}

	void GetScreenSize(std::int32_t& width, std::int32_t& height)
	{
		Call<void>(this, 5, std::ref(width), std::ref(height));
	}

};