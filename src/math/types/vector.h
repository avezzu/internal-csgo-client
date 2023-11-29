#pragma once

#include "matrix3x4.h"
#include <cmath>
#include <numbers>

class Vector
{
public:
	Vector() : x(0.f), y(0.f), z(0.f) {}
	Vector(float rhs) : x(rhs), y(rhs), z(rhs) {}
	Vector(float x, float y, float z) : x(x), y(y), z(z) {}

	Vector operator+(const Vector &rhs) const { return Vector{x + rhs.x, y + rhs.y, z + rhs.z}; }
	Vector operator-(const Vector &rhs) const { return Vector{x - rhs.x, y - rhs.y, z - rhs.z}; }
	Vector operator*(float scalar) const { return Vector{x * scalar, y * scalar, z * scalar}; }

	Vector Transform(const matrix3x4_t &matrix) const
	{
		return Vector{x * matrix[0][0] + y * matrix[0][1] + z * matrix[0][2] + matrix[0][3],
					  x * matrix[1][0] + y * matrix[1][1] + z * matrix[1][2] + matrix[1][3],
					  x * matrix[2][0] + y * matrix[2][1] + z * matrix[2][2] + matrix[2][3]};
	}

	float DotProductAbsolute(const Vector &rhs) const { return std::abs(x * rhs.x) + std::abs(y * rhs.y) + std::abs(z * rhs.z); }

	Vector Min(const Vector &rhs) const { return Vector{std::min<float>(x, rhs.x), std::min<float>(y, rhs.y), std::min<float>(z, rhs.z)}; }
	Vector Max(const Vector &rhs) const { return Vector{std::max<float>(x, rhs.x), std::max<float>(y, rhs.y), std::max<float>(z, rhs.z)}; }

	constexpr float Deg2Rad(const float deg) noexcept
	{
		return deg * (std::numbers::pi_v<float> / 180.f);
	}

	// convert angles -> vector
	Vector ToVector()
	{
		return Vector{
			std::cos(Deg2Rad(x)) * std::cos(Deg2Rad(y)),
			std::cos(Deg2Rad(x)) * std::sin(Deg2Rad(y)),
			-std::sin(Deg2Rad(x))};
	}

	Vector ToAngle()
	{
		return Vector{
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f};
	}

	constexpr bool isZero()
	{
		return x == 0 && y == 0 && z == 0;
	}

	float x, y, z;
};

struct Vec2
{
	float x, y;
};

struct Vec4
{
	float x, y, z, w;
};