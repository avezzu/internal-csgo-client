#pragma once
#include "cvector.h"
#include "../vis/vis.h"

class CMatrix3x4
{
public:
	constexpr float* operator[](size_t index) noexcept
	{
		return data[index];
	}

	constexpr CVector Origin() noexcept
	{
		return { data[0][3], data[1][3], data[2][3] };
	}

	float data[3][4];
};

class CMatrix4x4
{
public:
	float data[4][4];

	bool WorldToSchreen(CVector& pos, Vec2& screen)
	{
		Vec4 clipCoords;
		clipCoords.x = pos.x * data[0][0] + pos.y * data[0][1] + pos.z * data[0][2] + data[0][3];
		clipCoords.y = pos.x * data[1][0] + pos.y * data[1][1] + pos.z * data[1][2] + data[1][3];
		clipCoords.z = pos.x * data[2][0] + pos.y * data[2][1] + pos.z * data[2][2] + data[2][3];
		clipCoords.w = pos.x * data[3][0] + pos.y * data[3][1] + pos.z * data[3][2] + data[3][3];

		if (clipCoords.w < 0.01f)
			return false;

		CVector NDC; //normalized device coordinates (NDC)
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		screen.x = (vis::screen_width / 2 * NDC.x) + (NDC.x + vis::screen_width / 2);
		screen.y = -(vis::screen_hight / 2 * NDC.y) + (NDC.y + vis::screen_hight / 2);

		return true;
	}
};