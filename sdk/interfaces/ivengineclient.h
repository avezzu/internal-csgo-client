#pragma once

#include "usercmd.h"
#include "../../src/math/types/vector.h"
#include "../../src/math/types/matrix3x4.h"
#include "../../src/math/types/vmatrix.h"
#include "../../src/virtual/virtual.h"

class IVEngineClient
{
public:

	// returns index of local player
	std::int32_t GetLocalPlayerIndex() 
	{
		return vt::Call<std::int32_t>(this, 12);
	}

	void GetViewAngles(const Vector& viewAngles)
	{
		vt::Call<void, const Vector&>(this, 18, viewAngles);
	}

	void SetViewAngles(const Vector& viewAngles) 
	{
		vt::Call<void, const Vector&>(this, 19, viewAngles);
	}

	// returns true if in-game
	bool IsInGame() 
	{
		return vt::Call<bool>(this, 26);
	}

	VMatrix& WorldToScreenMatrix() 
	{
		return vt::Call<VMatrix&>(this, 37);
	}

	void GetScreenSize(std::int32_t& width, std::int32_t& height)
	{
		vt::Call<void>(this, 5, std::ref(width), std::ref(height));
	}

	int GetEngineBuildNumber()
	{
		return vt::Call<int>(this, 219);
	}

};