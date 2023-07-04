#pragma once
#include <numbers>
#include <cmath>

constexpr float Deg2Rad(const float deg) noexcept
{
	return deg * (std::numbers::pi_v<float> / 180.f);
}

class CVector
{
public:

	constexpr CVector(float x = 0.f, float y = 0.f, float z = 0.f) : x(x), y(y), z(z) {}

	constexpr CVector operator-(const CVector& other) const noexcept
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	constexpr CVector operator+(const CVector& other) const noexcept
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	constexpr CVector operator*(const float scale) const noexcept
	{
		return { x * scale, y * scale, z * scale };
	}

	// convert angles -> vector
	constexpr CVector ToVector() const noexcept
	{
		return {
			std::cos(Deg2Rad(x)) * std::cos(Deg2Rad(y)),
			std::cos(Deg2Rad(x)) * std::sin(Deg2Rad(y)),
			-std::sin(Deg2Rad(x))
		};
	}

	const CVector ToAngle() const noexcept
	{
		return CVector{
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f
		};
	}

	constexpr bool isZero()
	{
		return x == 0 && y == 0 && z == 0;
	}

public:
	float x;
	float y;
	float z;
};

struct Vec2
{
	float x, y;
};


struct Vec4
{
	float x, y, z, w;
};