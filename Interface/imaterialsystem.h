#pragma once
#include <cstdint>
#include "getInterface.h"

class IMaterial
{
public:
	enum EMaterialVarFlag : std::int32_t
	{
		IGNOREZ = 1 << 15,
		WIREFRAME = 1 << 28
	};

	constexpr void SetMaterialVarFlag(const std::int32_t flag, const bool on) noexcept
	{
		Call<void>(this, 29, flag, on);
	}
};

class IMaterialSystem
{
public:
	
	constexpr IMaterial* FindMaterial(const char* name) noexcept
	{
		return Call<IMaterial*>(this, 84, name, nullptr, true, nullptr);
	}
};