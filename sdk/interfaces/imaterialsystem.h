#pragma once
#include <cstdint>
#include "../../src/virtual/virtual.h"

class IMaterial
{
public:
	enum EMaterialVarFlag : std::int32_t
	{
		IGNOREZ = 1 << 15,
		WIREFRAME = 1 << 28
	};

	void SetMaterialVarFlag(const std::int32_t flag, const bool on) noexcept
	{
		vt::Call<void>(this, 29, flag, on);
	}
};

class IMaterialSystem
{
public:
	
	IMaterial* FindMaterial(const char* name) noexcept
	{
		return vt::Call<IMaterial*>(this, 84, name, nullptr, true, nullptr);
	}
};